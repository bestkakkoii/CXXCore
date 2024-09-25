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

#ifndef CXXMUTEX_H_
//@隐藏{
#define CXXMUTEX_H_
//@隐藏}



#include <mutex>
#include <atomic>
#include <cxxglobal.h>

//@分组{［线程安全］.炫　线程独享锁类

//@别名 炫互斥锁
class CXXMutex
{
public:
	CXXMutex();
	virtual ~CXXMutex();
	//@备注 所有线程都将阻塞，直到该互斥量被解锁
	//@参数 无
	//@返回 无
	//@别名 锁()
	void lock();

	//@备注 尝试锁定互斥量，如果互斥量已被锁定，则立即返回。如果互斥量未被锁定，则锁定互斥量并立即返回
	//@参数 无
	//@返回 如果互斥量被锁定，则返回 true；如果互斥量未被锁定，则返回 false
	//@别名 尝试锁()
	bool tryLock();

	//@备注 尝试锁定互斥量，如果互斥量已被锁定，则立即返回。如果互斥量未被锁定，则锁定互斥量并立即返回
	//@参数 timeout 超时时间，单位毫秒
	//@返回 如果互斥量被锁定，则返回 true；如果互斥量未被锁定，则返回 false
	//@别名 尝试锁定(超时时间)
	bool tryLockFor(UINT timeout);

	//@备注 解锁互斥量。如果互斥量未被锁定，则此函数不执行任何操作
	//@参数 无
	//@返回 无
	//@别名 解锁()
	void unlock();

	//@备注 是否锁定
	//@参数 无
	//@返回 如果互斥量被锁定，则返回 true；如果互斥量未被锁定，则返回 false
	//@别名 是否锁定()
	bool isLocked() const;
private:
	mutable std::mutex mutex_;
	std::atomic_bool locked_;
};

//@分组}

#endif // CXXMUTEX_H_

