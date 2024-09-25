#pragma once

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
#ifndef CXXSEMAPHORE_H_
//@隐藏{
#define CXXSEMAPHORE_H_
//@隐藏}

//@arc "cxxsemaphore.cpp"

#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

//@分组{［线程安全］.炫　进程同步类

//@隐藏{
class CXXSemaphorePrivate;
//@隐藏}

//@别名 炫信号量
class CXXSemaphore
{
public:
	//@备注 构造函数
	//@参数 n 信号量的初始值(默认为0)
	//@返回 无
	//@别名 共享信号量(初始信号量值)
	CXXSemaphore(__int64 n = 0, const CXXString& name = L"");

	virtual ~CXXSemaphore();

	//@备注 占用信号量数量
	//@参数 n 占用信号量数量(默认为1)
	//@返回 无
	//@别名 占用()
	void acquire(__int64 n = 1);

	//@备注 获取可用的信号量数量
	//@参数 无
	//@返回 信号量数量
	//@别名 可用数量()
	__int64 available() const;

	//@备注 释放信号量数量
	//@参数 n 释放信号量数量(默认为1)
	//@返回 无
	//@别名 释放()
	bool release(__int64 n = 1);

	//@备注 尝试占用信号量数量
	//@参数 n 占用信号量数量(默认为1)
	//@返回 是否占用成功
	//@别名 尝试占用( 占用信号量数量 )
	bool tryAcquire(__int64 n = 1);

	//@备注 在指定时间内尝试占用信号量数量
	//@参数 n 占用信号量数量(默认为1)
	//@参数 timeout 超时时间(单位:毫秒)
	//@返回 是否占用成功
	//@别名 尝试占用( 占用信号量数量, 超时时间 )
	bool tryAcquire(__int64 n, __int64 timeout);

private:
	CXX_DECLARE_PRIVATE(CXXSemaphore);
	CXXScopedPointer<CXXSemaphorePrivate> d_ptr;
};

//@分组}

#endif // CXXSEMAPHORE_H_
