/*
				GNU GENERAL PUBLIC LICENSE
				   Version 3, June 1991
COPYRIGHT (C) Bestkakkoii 2024 All Rights Reserved.
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/
#ifndef CXXPROCESS_P_H_
#define CXXPROCESS_P_H_

#include <cxxprocess.h>
#include <cxxmutex.h>
#include <windows.h>
#include <functional>
#include <string>

class CXXProcessPrivate
{
public:
	explicit CXXProcessPrivate(CXXProcess* q);
	virtual ~CXXProcessPrivate();

	// 定义回调函数类型，接收流数据作为参数
	CXXIOCallback stdOutCallback_;
	CXXIOCallback stdErrCallback_;

	// 管道管理等功能
	bool createPipes();
	void closePipes();
	void readFromPipe(HANDLE pipe, CXXIOCallback callback) const;

	// 设置回调
	void setStdOutCallback(const CXXIOCallback& callback)
	{
		stdOutCallback_ = callback;
	}

	void setStdErrCallback(const CXXIOCallback& callback)
	{
		stdErrCallback_ = callback;
	}

	bool isIOReady() const;

	void combineCommand();

private:
	// 管道句柄
	HANDLE hStdInRead_ = nullptr;
	HANDLE hStdInWrite_ = nullptr;
	HANDLE hStdOutRead_ = nullptr;
	HANDLE hStdOutWrite_ = nullptr;
	HANDLE hStdErrRead_ = nullptr;
	HANDLE hStdErrWrite_ = nullptr;

	STARTUPINFOW startupInfo_ = {};
	SECURITY_ATTRIBUTES processAttributes = {};
	SECURITY_ATTRIBUTES threadAttributes = {};
	PROCESS_INFORMATION processInfo_ = {};
	CXXString program_;
	CXXString workingDirectory_;
	CXXString commandLine_;
	CXXVector<CXXString> arguments_;
	cxx::ProcessState processState_ = cxx::NotRunning;
	cxx::ProcessError processError_ = cxx::UnknownError;
	cxx::ExitStatus exitStatus_ = cxx::NormalExit;
	__int64 exitCode_ = 0;
	bool isDetached_ = false;
	bool isSuspenWhenStarted_ = false;

private:
	CXX_DECLARE_PUBLIC(CXXProcess);

	CXXProcess* q_ptr = CXX_NULLPTR;
};



#endif // CXXPROCESS_P_H_