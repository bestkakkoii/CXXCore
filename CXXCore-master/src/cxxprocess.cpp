/*
				GNU GENERAL PUBLIC LICENSE
				   Version 2, June 1991
COPYRIGHT (C) Bestkakkoii 2024 All Rights Reserved.
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/
#include <pch.h>

#include <cxxprocess_p.h>
#include <cxxmutexlocker.h>

CXXProcessPrivate::CXXProcessPrivate(CXXProcess* q)
	: q_ptr(q)
{
}

CXXProcessPrivate::~CXXProcessPrivate()
{

}

bool CXXProcessPrivate::createPipes()
{
	SECURITY_ATTRIBUTES saAttr = {};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;  // 管道允许子进程继承
	saAttr.lpSecurityDescriptor = CXX_NULLPTR;

	// 创建标准输入管道
	if (FALSE == CreatePipe(&hStdInRead_, &hStdInWrite_, &saAttr, 0))
	{
		return false;
	}

	// 确保写入句柄不会被子进程继承
	if (FALSE == SetHandleInformation(hStdInWrite_, HANDLE_FLAG_INHERIT, 0))
	{
		return false;
	}

	// 创建标准输出管道
	if (FALSE == CreatePipe(&hStdOutRead_, &hStdOutWrite_, &saAttr, 0))
	{
		return false;
	}

	// 确保读取句柄不会被子进程继承
	if (FALSE == SetHandleInformation(hStdOutRead_, HANDLE_FLAG_INHERIT, 0))
	{
		return false;
	}

	// 创建标准错误管道
	if (FALSE == CreatePipe(&hStdErrRead_, &hStdErrWrite_, &saAttr, 0))
	{
		return false;
	}

	// 确保读取句柄不会被子进程继承
	if (FALSE == SetHandleInformation(hStdErrRead_, HANDLE_FLAG_INHERIT, 0))
	{
		return false;
	}

	return true;
}

void CXXProcessPrivate::closePipes()
{
	// 关闭所有管道句柄
	if (hStdInRead_ != CXX_NULLPTR)
	{
		MINT::NtClose(hStdInRead_);
		hStdInRead_ = CXX_NULLPTR;
	}

	if (hStdInWrite_ != CXX_NULLPTR)
	{
		MINT::NtClose(hStdInWrite_);
		hStdInWrite_ = CXX_NULLPTR;
	}

	if (hStdOutRead_ != CXX_NULLPTR)
	{
		MINT::NtClose(hStdOutRead_);
		hStdOutRead_ = CXX_NULLPTR;
	}

	if (hStdOutWrite_ != CXX_NULLPTR)
	{
		MINT::NtClose(hStdOutWrite_);
		hStdOutWrite_ = CXX_NULLPTR;
	}

	if (hStdErrRead_ != CXX_NULLPTR)
	{
		MINT::NtClose(hStdErrRead_);
		hStdErrRead_ = CXX_NULLPTR;
	}

	if (hStdErrWrite_ != CXX_NULLPTR)
	{
		MINT::NtClose(hStdErrWrite_);
		hStdErrWrite_ = CXX_NULLPTR;
	}
}

void CXXProcessPrivate::readFromPipe(HANDLE pipe, CXXIOCallback callback) const
{
	for (;;)
	{
		std::string result;

		for (;;)
		{
			DWORD bytesRead;
			CHAR buffer[8192] = {};

			// 从管道中读取数据
			if ((FALSE == ReadFile(pipe, buffer, sizeof(buffer) - 1, &bytesRead, CXX_NULLPTR)) || 0 == bytesRead)
			{
				break;
			}

			buffer[bytesRead] = '\0';

			if (callback != CXX_NULLPTR)
			{
				callback(CXXString::fromLocal8Bit(buffer));  // 将读取到的数据通过回调函数返回
			}
		}

		if (processState_ != cxx::Running)
		{
			break;  // 如果进程不在运行，退出循环
		}

		Sleep(10);
	}
}

bool CXXProcessPrivate::isIOReady() const
{
	DWORD bytesAvailable = 0;
	BOOL stdoutReady = FALSE;
	BOOL stderrReady = FALSE;

	// 检查标准输出管道是否准备好读取
	if (hStdOutRead_ != nullptr)
	{
		stdoutReady = PeekNamedPipe(hStdOutRead_, nullptr, NULL, nullptr, &bytesAvailable, nullptr);
		if (!stdoutReady || bytesAvailable == 0)  // 如果没有准备好或没有数据可读取
		{
			stdoutReady = FALSE;
		}
	}

	// 检查标准错误管道是否准备好读取
	if (hStdErrRead_ != nullptr)
	{
		stderrReady = PeekNamedPipe(hStdErrRead_, nullptr, NULL, nullptr, &bytesAvailable, nullptr);
		if (!stderrReady || bytesAvailable == 0)
		{
			stderrReady = FALSE;
		}
	}

	// 当 stdout 和 stderr 都已准备好时，返回 true
	return stdoutReady || stderrReady;
}

void CXXProcessPrivate::combineCommand()
{
	commandLine_ = program_ + L" ";
	for (const CXXString& arg : arguments_)
	{
		commandLine_ += arg + L" ";
	}

}

CXXProcess::CXXProcess()
	: d_ptr(new CXXProcessPrivate(this))
{

}

CXXProcess::~CXXProcess()
{

}

void CXXProcess::start(const CXXString& program, const CXXVector<CXXString>& arguments, cxx::ProcessOpenMode mode)
{
	CXX_D(CXXProcess);

	// 检查是否已经有进程在运行
	if (d->processState_ == cxx::Running)
	{
		d->processError_ = cxx::FailedToStart;
		return;
	}

	d->program_ = program;
	d->arguments_ = arguments;

	// 先获取当前进程的启动信息，作为基础
	GetStartupInfoW(&d->startupInfo_);

	// 初始化进程和线程的安全属性
	// 默认将这些属性初始化为当前进程和线程的常用属性值
	memset(&d->processAttributes, 0, sizeof(SECURITY_ATTRIBUTES));
	d->processAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	d->processAttributes.bInheritHandle = TRUE;  // 允许子进程继承句柄
	d->processAttributes.lpSecurityDescriptor = nullptr;  // 使用默认的安全描述符

	memset(&d->threadAttributes, 0, sizeof(SECURITY_ATTRIBUTES));
	d->threadAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	d->threadAttributes.bInheritHandle = TRUE;  // 允许线程继承句柄
	d->threadAttributes.lpSecurityDescriptor = nullptr;  // 使用默认的安全描述符

	// 创建管道（如果需要）
	if (mode != cxx::ProcessNotOpen && !d->createPipes())
	{
		d->processError_ = cxx::FailedToStart;
		return;
	}

	// 使用 isSuspenWhenStarted_ 决定是否挂起
	DWORD creationFlags = 0;
	if (d->isSuspenWhenStarted_)
	{
		creationFlags |= CREATE_SUSPENDED;
	}

	d->createPipes();

	// 根据 OpenMode 处理管道连接
	if (mode & cxx::ProcessReadWrite)
	{
		d->startupInfo_.dwFlags |= STARTF_USESTDHANDLES;
		d->startupInfo_.hStdInput = d->hStdInRead_;
		d->startupInfo_.hStdOutput = d->hStdOutWrite_;
		d->startupInfo_.hStdError = d->hStdErrWrite_;
	}
	else if (mode & cxx::ProcessReadOnly)
	{
		d->startupInfo_.dwFlags |= STARTF_USESTDHANDLES;
		d->startupInfo_.hStdInput = d->hStdInRead_;
	}
	else if (mode & cxx::ProcessWriteOnly)
	{
		d->startupInfo_.dwFlags |= STARTF_USESTDHANDLES;
		d->startupInfo_.hStdOutput = d->hStdOutWrite_;
		d->startupInfo_.hStdError = d->hStdErrWrite_;
	}

	// 生成命令行
	d->combineCommand();

	d->processState_ = cxx::Running;

	// 创建进程
	BOOL success = CreateProcessW(
		CXX_NULLPTR,                                // 应用程序名
		d->commandLine_.data(),                        // 命令行
		&d->processAttributes,                  // 进程安全属性
		&d->threadAttributes,                   // 线程安全属性
		TRUE,                                   // 继承句柄
		creationFlags,                          // 创建标志，根据挂起模式
		CXX_NULLPTR,                                // 环境变量
		d->workingDirectory_.empty() ? CXX_NULLPTR : d->workingDirectory_.c_str(), // 工作目录
		&d->startupInfo_,                       // 启动信息
		&d->processInfo_                        // 进程信息
	) == TRUE;

	if (!success)
	{
		d->processError_ = cxx::FailedToStart;
		d->closePipes();  // 清理管道
		return;
	}

	// 启动读取 stdout 和 stderr 的线程
	std::thread([this]()
		{
			CXX_D(CXXProcess);
			d->readFromPipe(d->hStdOutRead_, d->stdOutCallback_);
		}
	).detach();

	std::thread([this]()
		{
			CXX_D(CXXProcess);
			d->readFromPipe(d->hStdErrRead_, d->stdErrCallback_);
		}
	).detach();

	// 启动监控线程以检测进程是否仍在运行
	std::thread([this]()
		{
			CXX_D(CXXProcess);
			WaitForSingleObject(d->processInfo_.hProcess, INFINITE);  // 等待进程结束

			d->processState_ = cxx::NotRunning;  // 更新进程状态

			// 获取退出代码并更新退出状态
			GetExitCodeProcess(d->processInfo_.hProcess, reinterpret_cast<LPDWORD>(&d->exitCode_));
			d->exitStatus_ = d->exitCode_ == 0 ? cxx::NormalExit : cxx::CrashExit;
		}
	).detach();

}

void CXXProcess::start(cxx::ProcessOpenMode mode)
{
	CXX_D(CXXProcess);
	start(d->program_, d->arguments_, mode);
}

bool CXXProcess::startDetached(__int64* pid)
{
	CXX_D(CXXProcess);

	// 检查是否已经有进程在运行
	if (d->processState_ == cxx::Running)
	{
		d->processError_ = cxx::FailedToStart;
		return false;
	}


	d->isDetached_ = true;

	// 先获取当前进程的启动信息，作为基础
	GetStartupInfoW(&d->startupInfo_);

	// 初始化进程和线程的安全属性
	// 默认将这些属性初始化为当前进程和线程的常用属性值
	memset(&d->processAttributes, 0, sizeof(SECURITY_ATTRIBUTES));
	d->processAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	d->processAttributes.bInheritHandle = TRUE;  // 允许子进程继承句柄
	d->processAttributes.lpSecurityDescriptor = nullptr;  // 使用默认的安全描述符

	memset(&d->threadAttributes, 0, sizeof(SECURITY_ATTRIBUTES));
	d->threadAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	d->threadAttributes.bInheritHandle = TRUE;  // 允许线程继承句柄
	d->threadAttributes.lpSecurityDescriptor = nullptr;  // 使用默认的安全描述符

	// 设置创建标志为分离进程
	DWORD creationFlags = CREATE_NEW_CONSOLE | DETACHED_PROCESS;

	// 生成命令行
	d->combineCommand();

	// 创建分离进程
	bool success = CreateProcessW(
		CXX_NULLPTR,
		d->commandLine_.data(),
		&d->processAttributes,
		&d->threadAttributes,
		FALSE,
		creationFlags,
		CXX_NULLPTR,
		d->workingDirectory_.empty() ? CXX_NULLPTR : d->workingDirectory_.c_str(),
		&d->startupInfo_,
		&d->processInfo_
	) == TRUE;

	if (!success)
	{
		d->processError_ = cxx::FailedToStart;
		d->closePipes();
		return false;
	}

	// 设置返回的进程ID
	if (pid != CXX_NULLPTR)
	{
		*pid = d->processInfo_.dwProcessId;
	}

	d->processState_ = cxx::Running;
	return true;
}

__int64 CXXProcess::id() const
{
	CXX_D(const CXXProcess);
	return d->processInfo_.dwProcessId;
}

__int64 CXXProcess::exitCode() const
{
	CXX_D(const CXXProcess);
	return d->exitCode_;
}

cxx::ExitStatus CXXProcess::exitStatus() const
{
	CXX_D(const CXXProcess);
	return d->exitStatus_;
}

void CXXProcess::close()
{
	CXX_D(CXXProcess);

	if (d->processInfo_.hThread != CXX_NULLPTR)
	{
		MINT::NtClose(d->processInfo_.hThread);
	}

	if (d->processInfo_.hProcess != CXX_NULLPTR)
	{
		MINT::NtClose(d->processInfo_.hProcess);
	}

	MINT::NtTerminateProcess(d->processInfo_.hProcess, 0);
}

void CXXProcess::setArguments(const CXXVector<CXXString>& arguments)
{
	CXX_D(CXXProcess);
	d->arguments_ = arguments;
}

void CXXProcess::setWorkingDirectory(const CXXString& dir)
{
	CXX_D(CXXProcess);
	d->workingDirectory_ = dir;
}

void CXXProcess::setSuspendWhenStarted(bool suspend)
{
	CXX_D(CXXProcess);
	d->isSuspenWhenStarted_ = suspend;
}

cxx::ProcessState CXXProcess::state() const
{
	CXX_D(const CXXProcess);
	return d->processState_;
}

cxx::ProcessError CXXProcess::error() const
{
	CXX_D(const CXXProcess);
	return d->processError_;
}

// attach模式下则直接建立回调，detach模式下则创建线程监测 -1 为永久等待
bool CXXProcess::waitForFinished(__int64 msecs) const
{
	CXX_D(const CXXProcess);
	if (d->processState_ != cxx::Running)
	{
		return true;
	}

	// 等待进程结束
	DWORD result = WaitForSingleObject(d->processInfo_.hProcess, static_cast<DWORD>(msecs));
	if (WAIT_OBJECT_0 == result)
	{
		return true;
	}

	return false;  // 超时
}

// attach模式下则直接建立回调，detach模式下则创建线程监测 -1 为永久等待
bool CXXProcess::waitForStarted(__int64 msecs) const
{
	CXX_D(const CXXProcess);

	if (d->processState_ != cxx::Running)
	{
		return false;
	}

	__int64 elapsedTime = 0;
	const __int64 pollInterval = 100; // 每100毫秒轮询一次
	DWORD waitTime = static_cast<DWORD>(std::min(msecs, pollInterval));

	while (elapsedTime < msecs)
	{
		// 检查进程是否仍在运行，如果已经退出，则返回 false
		DWORD result = WaitForSingleObject(d->processInfo_.hProcess, 0);  // 非阻塞检查进程状态
		if (WAIT_OBJECT_0 == result)
		{
			return false;  // 如果进程结束，则返回 false
		}

		// 检查 I/O 是否已经准备好
		if (d->isIOReady())
		{
			return true;  // I/O 已经准备好，进程正在运行
		}

		// 等待指定的间隔时间，然后再进行检查
		Sleep(waitTime);
		elapsedTime += pollInterval;
	}

	return false;  // 超时，I/O 仍未准备好
}

void CXXProcess::setProcessSuspend(bool suspend)
{
	CXX_D(CXXProcess);

	if (CXX_NULLPTR == d->processInfo_.hThread)
	{
		return;
	}

	if (suspend)
	{
		SuspendThread(d->processInfo_.hThread);
	}
	else
	{
		ResumeThread(d->processInfo_.hThread);
	}

}

void CXXProcess::setStdOutCallback(const CXXIOCallback& callback)
{
	CXX_D(CXXProcess);
	d->setStdOutCallback(callback);
}

void CXXProcess::setStdErrCallback(const CXXIOCallback& callback)
{
	CXX_D(CXXProcess);
	d->setStdErrCallback(callback);
}

//@备注 取当前进程ID
//@返回 进程ID
//@别名 编号()
__int64 CXXProcess::currentId()
{
#if defined(CXX_OS_WIN)
	return GetCurrentProcessId();
#else
	return _getpid();
#endif
}

//@备注 取当前进程伪句柄
//@返回 进程伪句柄
//@别名 伪句柄()
HANDLE CXXProcess::currentHandle()
{
	return GetCurrentProcess();
}