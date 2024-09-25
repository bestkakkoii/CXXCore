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
#ifndef CXXSCOPEDHANDLE_H_
//@隐藏{
#define CXXSCOPEDHANDLE_H_
//@隐藏}


#include <cxxglobal.h>
#include <cxxscopedpointer.h>

#include <windows.h>

//@分组{［内存安全］.炫　作用域句柄类

//@隐藏{
class CXXScopedHandlePrivate;
//@隐藏}

//@备注 接受任意类型的 HANDLE 句柄，并在析构时自动关闭句柄
//@别名 炫作用域句柄
class CXXScopedHandle
{
public:
	//@参数 无
	CXXScopedHandle();

	//@参数 other 句柄
	CXXScopedHandle(HANDLE other);

	//@参数 other 句柄
	CXXScopedHandle(const CXXScopedHandle& other);

	//@隐藏{
	virtual ~CXXScopedHandle();

	CXXScopedHandle& operator=(HANDLE other);

	CXXScopedHandle& operator=(const CXXScopedHandle& other);

	CXXScopedHandle& operator=(CXXScopedHandle&& other) noexcept;

	operator HANDLE();
	//@隐藏}

	//@备注 获取句柄，使用此函数不会增加引用计数
	//@参数 无
	//@返回 句柄
	//@别名 取()
	HANDLE get() const;

	//@备注 获取引用计数
	//@参数 无
	//@返回 引用计数
	//@别名 引用计数()
	__int64 refcount() const;

	//@备注 关闭句柄
	//@参数 无
	//@返回 无
	//@别名 关闭()
	void close();

	//@备注 判断句柄是否有效
	//@参数 无
	//@返回 有效返回true，无效返回false
	//@别名 是否有效()
	bool isValid() const;

	//@备注 转换为整数
	//@参数 无
	//@返回 整数
	//@别名 到正整型()
	size_t toUInt() const;

	//@备注 转换为整数
	//@参数 无
	//@返回 整数
	//@别名 到整型()
	intptr_t toInt() const;

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXScopedHandle);
	CXXScopedPointer<CXXScopedHandlePrivate> d_ptr;
	//@隐藏}
};


//@分组}

#endif // CXXSCOPEDHANDLE_H_