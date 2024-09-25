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

#include <cxxscopedhandle_p.h>

CXXScopedHandlePrivate::CXXScopedHandlePrivate(CXXScopedHandle* q, void* handle)
	: q_ptr(q)
	, handle_(handle)
{
}

CXXScopedHandlePrivate::~CXXScopedHandlePrivate()
{
}

CXXScopedHandle::CXXScopedHandle()
	: d_ptr(new CXXScopedHandlePrivate(this, CXX_NULLPTR))
{}

//@参数 other 句柄
CXXScopedHandle::CXXScopedHandle(HANDLE other)
	: d_ptr(new CXXScopedHandlePrivate(this, other))
{}

//@参数 other 句柄
CXXScopedHandle::CXXScopedHandle(const CXXScopedHandle& other)
{
	CXX_D(CXXScopedHandle);
	d->handle_.store(other.d_ptr->handle_, std::memory_order_release);
}

CXXScopedHandle::~CXXScopedHandle()
{
	close();
}

CXXScopedHandle& CXXScopedHandle::operator=(HANDLE other)
{
	CXX_D(CXXScopedHandle);
	if (d->handle_ != other)
	{
		close();
		d->handle_.store(other, std::memory_order_release);
	}
	return *this;
}

CXXScopedHandle& CXXScopedHandle::operator=(const CXXScopedHandle& other)
{
	CXX_D(CXXScopedHandle);
	if (d->handle_ != other.d_ptr->handle_)
	{
		close();
		d->handle_.store(other.d_ptr->handle_, std::memory_order_release);
	}
	return *this;
}

CXXScopedHandle& CXXScopedHandle::operator=(CXXScopedHandle&& other) noexcept
{
	CXX_D(CXXScopedHandle);
	if (d->handle_ != other.d_ptr->handle_)
	{
		close();
		d->handle_.store(other.d_ptr->handle_, std::memory_order_release);
		other.d_ptr->handle_.store(CXX_NULLPTR, std::memory_order_release);
	}
	return *this;
}

CXXScopedHandle::operator HANDLE()
{
	CXX_D(CXXScopedHandle);
	d->refcount_.fetch_add(1, std::memory_order_release);
	return d->handle_.load(std::memory_order_acquire);
}

//@备注 获取句柄，使用此函数不会增加引用计数
//@参数 无
//@返回 句柄
//@别名 取()
HANDLE CXXScopedHandle::get() const
{
	CXX_D(const CXXScopedHandle);
	return d->handle_.load(std::memory_order_acquire);
}

//@备注 获取引用计数
//@参数 无
//@返回 引用计数
//@别名 引用计数()
__int64 CXXScopedHandle::refcount() const
{
	CXX_D(const CXXScopedHandle);
	return d->refcount_.load(std::memory_order_acquire);
}

//@备注 关闭句柄
//@参数 无
//@返回 无
//@别名 关闭句柄()
void CXXScopedHandle::close()
{
	CXX_D(CXXScopedHandle);
	if (d->handle_ != CXX_NULLPTR && d->handle_ != INVALID_HANDLE_VALUE)
	{
		MINT::NtClose(d->handle_.load(std::memory_order_acquire));
		d->handle_.store(CXX_NULLPTR, std::memory_order_release);
	}
}

//@备注 判断句柄是否有效
//@参数 无
//@返回 有效返回true，无效返回false
//@别名 是否有效()
bool CXXScopedHandle::isValid() const
{
	CXX_D(const CXXScopedHandle);
	void* h = d->handle_.load(std::memory_order_acquire);
	return h != CXX_NULLPTR && h != INVALID_HANDLE_VALUE;
}

//@备注 转换为整数
//@参数 无
//@返回 整数
//@别名 到正整型()
size_t CXXScopedHandle::toUInt() const
{
	CXX_D(const CXXScopedHandle);
	return reinterpret_cast<size_t>(d->handle_.load(std::memory_order_acquire));
}

//@备注 转换为整数
//@参数 无
//@返回 整数
//@别名 到整型()
intptr_t CXXScopedHandle::toInt() const
{
	CXX_D(const CXXScopedHandle);
	return reinterpret_cast<intptr_t>(d->handle_.load(std::memory_order_acquire));
}