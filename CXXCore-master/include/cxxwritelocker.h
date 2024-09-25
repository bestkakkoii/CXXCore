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

#ifndef  CXXWRITELOCKER_H_
//@隐藏{
#define  CXXWRITELOCKER_H_
//@隐藏}

#include <cxxglobal.h>
#include <cxxreadwritelock.h>

//@分组{［线程安全］.炫　线程共享锁类

//@别名 炫作用域写锁
class CXXWriteLocker
{
public:
	//@备注 构造函数
	//@参数 CXXReadWriteLock读写锁指针
	//@返回 无
	CXXWriteLocker(CXXReadWriteLock* lock)
		: pLock_(lock)
	{
		if (CXX_NULLPTR == pLock_)
		{
			return;
		}

		pLock_->lockForWrite();
	}

	~CXXWriteLocker()
	{
		if (CXX_NULLPTR == pLock_)
		{
			return;
		}

		pLock_->unlock();
	}

	//@备注 获取读写锁指针
	//@参数 无
	//@返回 CXReadWriteLock读写锁指针
	//@别名 取()
	const CXXReadWriteLock* get() const
	{
		return pLock_;
	}

	//@备注 获取读写锁指针
	//@参数 无
	//@返回 CXReadWriteLock读写锁指针
	//@别名 地址()
	const CXXReadWriteLock* data() const
	{
		return get();
	}

	//@备注 获取读写锁指针
	//@参数 无
	//@返回 CXReadWriteLock读写锁指针
	//@别名 常量地址()
	const CXXReadWriteLock* constData() const
	{
		return get();
	}

	//@备注 获取读写锁指针
	//@参数 无
	//@返回 CXReadWriteLock读写锁指针
	//@别名 取()
	CXXReadWriteLock* get()
	{
		return pLock_;
	}

	//@备注 获取读写锁指针
	//@参数 无
	//@返回 CXReadWriteLock读写锁指针
	//@别名 地址()
	CXXReadWriteLock* data()
	{
		return get();
	}


	//@备注 重新锁定
	//@参数 无
	//@返回 无
	//@别名 重新锁()
	void relock()
	{
		if (CXX_NULLPTR == pLock_)
		{
			return;
		}

		pLock_->lockForWrite();
	}

	//@备注 解锁
	//@参数 无
	//@返回 无
	//@别名 解锁()
	void unlock()
	{
		pLock_->unlock();
		pLock_ = CXX_NULLPTR;
	}

private:
	CXXReadWriteLock* pLock_ = CXX_NULLPTR;
};

//@分组}


#endif // CXXWRITELOCKER_H_