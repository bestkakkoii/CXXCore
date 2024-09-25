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
#ifndef CXXAPPLICATION_H_
//@隐藏{
#define CXXAPPLICATION_H_
//@隐藏}

#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

//@隐藏{
class CXXApplicationPrivate;
//@隐藏}

//@分组{［其他］.炫　程序类

//@别名 炫程序
class CXXApplication
{
public:
	CXXApplication();
	virtual ~CXXApplication();

	//@备注 主动处理消息队列，用于强制处理消息
	//@返回 无
	//@别名 处理事件()
	static void processEvents();

	//@备注 获取当前进程绝对路径
	//@返回 进程绝对路径
	//@别名 路径()
	static CXXString filePath();

	//@备注 获取当前进程所在目录
	//@返回 进程所在目录
	//@别名 目录()
	static CXXString dirPath();

	//@备注 获取当前进程名称
	//@返回 进程名称
	//@别名 名称()
	static CXXString name();

	//@备注 获取当前程序版本
	//@返回 程序版本
	//@别名 版本()
	static CXXString version();

private:
	CXX_DECLARE_PRIVATE(CXXApplication);
	CXXScopedPointer<CXXApplicationPrivate> d_ptr;
};

//@分组}


#endif // CXXAPPLICATION_H_