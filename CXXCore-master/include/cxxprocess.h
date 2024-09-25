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
#ifndef CXXPROCESS_H_
//@隐藏{
#define CXXPROCESS_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxvector.h>
#include <cxxscopedpointer.h>

#include <functional>

//@别名 炫
namespace cxx
{
	//@分组{［其他］.炫　进程类

	//@别名 进程错误
	enum ProcessError
	{
		FailedToStart,  //@别名 进程启动失败
		Crashed,		//@别名 进程崩溃
		Timedout,		//@别名 进程超时
		WriteError,		//@别名 进程写入错误
		ReadError, 		//@别名 进程读取错误
		UnknownError	//@别名 进程未知错误
	};

	//@别名 进程状态
	enum ProcessState
	{
		NotRunning,		//@别名 未运行
		Starting, 		//@别名 启动中
		Running,		//@别名 运行中
	};

	//@别名 退出状态
	enum ExitStatus
	{
		NormalExit,		//@别名 正常退出
		CrashExit,		//@别名 崩溃退出
	};

	//@别名 进程流打开模式
	enum ProcessOpenModeFlag
	{
		ProcessNotOpen = 0x0000,	//@别名 进程流未打开
		ProcessReadOnly = 0x0001,	//@别名 进程流只读
		ProcessWriteOnly = 0x0002,	//@别名 进程流只写
		//@别名 进程流读写
		ProcessReadWrite = ProcessReadOnly | ProcessWriteOnly
	};

	//@隐藏{
	CXX_DECLARE_FLAGS(ProcessOpenMode, ProcessOpenModeFlag);
	//@隐藏}

	//@分组}
}

//@分组{［其他］.炫　进程类

//@备注 回调函数类型，接收流数据作为参数
// typedef std::function<void(const CXXString&)> CXXIOCallback;
//@参数 流数据
//@别名 炫流回调函数
typedef std::function<void(const CXXString&)> CXXIOCallback;

//@隐藏{
class CXXProcessPrivate;
//@隐藏}


//@备注 用于启动进程，可接管进程的输入输出流，支持进程分离，亦可用于查询当前进程信息
//@别名 炫进程
class CXXProcess
{
public:
	CXXProcess();
	virtual ~CXXProcess();

	//@隐藏{
	void start(const CXXString& program, const CXXVector<CXXString>& arguments = CXXVector<CXXString>(), cxx::ProcessOpenMode mode = cxx::ProcessReadWrite);
	//@隐藏}

	/*@声明
	//@备注 启动进程，可接管进程的输入输出流
	//@参数 program 进程名称(可留空)
	//@参数 arguments 进程启动参数
	//@参数 mode 进程打开模式
	//@返回 无
	//@别名 启动(进程名称, 进程启动参数, 进程打开模式)
	void start(const CXXString& program, const CXXVector<CXXString>& arguments = {}, cxx::ProcessOpenMode mode = cxx::ProcessReadWrite);
	*/


	//@备注 启动进程，可接管进程的输入输出流
	//@参数 mode 进程打开模式
	//@返回 无
	//@别名 启动(进程打开模式)
	void start(cxx::ProcessOpenMode mode = cxx::ProcessReadWrite);

	//@备注 启动分离进程
	//@参数 pid 用于接收进程ID的指针
	//@返回 启动成功返回true，否则返回false
	//@别名 分离启动(接收指针)
	bool startDetached(__int64* pid = nullptr);

	//@备注 取进程ID
	//@参数 无
	//@返回 进程ID
	//@别名 编号()
	__int64 id() const;

	//@备注 取进程退出码，仅对非分离进程有效
	//@参数 无
	//@返回 进程退出码
	//@别名 退出码()
	__int64 exitCode() const;

	//@备注 取进程退出状态
	//@参数 无
	//@返回 进程退出状态
	//@别名 退出状态()
	cxx::ExitStatus exitStatus() const;

	//@备注 关闭进程
	//@参数 无
	//@返回 无
	//@别名 关闭()
	void close();

	//@备注 设置启动参数
	//@参数 arguments 启动参数文本动态数组
	//@返回 无
	//@别名 置启动参数(启动参数文本动态数组)
	void setArguments(const CXXVector<CXXString>& arguments);

	//@备注 设置工作目录
	//@参数 dir 工作目录
	//@返回 无
	//@别名 置工作目录(工作目录)
	void setWorkingDirectory(const CXXString& dir);

	//@备注 设置是否在启动进程时挂起
	//@参数 suspend 是否挂起
	//@返回 无
	//@别名 置启动挂起(是否挂起)
	void setSuspendWhenStarted(bool suspend);

	//@备注 进程状态
	//@参数 无
	//@返回 进程状态
	//@别名 状态()
	cxx::ProcessState state() const;

	//@备注 进程错误
	//@参数 无
	//@返回 进程错误
	//@别名 错误()
	cxx::ProcessError error() const;

	//@备注 等待进程结束
	//@参数 msecs 等待时间(毫秒)
	//@返回 等待成功返回true，否则返回false
	//@别名 等待结束(等待时间)
	bool waitForFinished(__int64 msecs = -1) const;

	//@备注 等待进程启动
	//@参数 msecs 等待时间(毫秒)
	//@返回 等待成功返回true，否则返回false
	//@别名 等待启动(等待时间)
	bool waitForStarted(__int64 msecs = -1) const;

	//@备注 挂起进程
	//@参数 是否挂起
	//@返回 无
	//@别名 挂起(是否挂起)
	void setProcessSuspend(bool suspend);

	//@备注 设置标准输出流回调函数
	//@参数 callback 标准输出回调
	//@返回 无
	//@别名 置标准输出回调(回调函数)
	void setStdOutCallback(const CXXIOCallback& callback);

	//@备注 设置标准输出流回调函数
	//@参数 callback 类回调函数指针
	//@参数 obj 类实例指针
	//@返回 无
	//@别名 置标准输出回调(类回调函数指针, 类实例指针)
	template<typename T, typename U>
	void setStdOutCallback(T&& callback, U* obj)
	{
		setStdOutCallback(std::bind(callback, obj, std::placeholders::_1));
	}

	//@备注 设置标准错误流回调函数
	//@参数 callback 标准错误回调
	//@返回 无
	//@别名 置标准错误回调(回调函数)
	void setStdErrCallback(const CXXIOCallback& callback);

	//@备注 设置标准错误流回调函数
	//@参数 callback 类回调函数指针
	//@参数 obj 类实例指针
	//@返回 无
	//@别名 置标准错误回调(类回调函数指针, 类实例指针)
	template<typename T, typename U>
	void setStdErrCallback(T&& callback, U* obj)
	{
		setStdErrCallback(std::bind(callback, obj, std::placeholders::_1));
	}

public:
	//@备注 取当前进程ID
	//@返回 进程ID
	//@别名 编号()
	static __int64 currentId();

	//@备注 取当前进程伪句柄
	//@返回 进程伪句柄
	//@别名 句柄()
	static HANDLE currentHandle();

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXProcess);
	CXXScopedPointer<CXXProcessPrivate> d_ptr;
	//@隐藏}
};

//@分组}

#endif // CXXPROCESS_H_