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

#ifndef CXXMUTEXLOCKER_H_
//@隐藏{
#define CXXMUTEXLOCKER_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxmutex.h>

//@分组{［线程安全］.炫　线程独享锁类

//@备注 传入互斥锁指针，自动锁定，解构自动解锁
//@别名 炫作用域互斥锁
class CXXMutexLocker
{
public:


	//@备注 构造函数
	//@参数 CXMutex互斥锁指针
	//@返回 无
	CXXMutexLocker(CXXMutex* mutex);

	//@备注 构造函数 
	//@参数 CXMutexLocker其他智能互斥锁对象
	//@返回 无
	CXXMutexLocker(CXXMutexLocker&& other)
		//@隐藏{
		noexcept
		//@隐藏}
		;

	//@隐藏{
	virtual ~CXXMutexLocker();
	//@隐藏}

	//@备注 是否锁定
	//@参数 无
	//@返回 如果互斥量被锁定，则返回 true；如果互斥量未被锁定，则返回 false
	//@别名 是否锁()
	bool isLocked() const;

	//@备注 获取原始互斥锁指针
	//@参数 无
	//@返回 CXMutex互斥锁指针
	//@别名 取()
	CXXMutex* get() const;

	//@备注 获取原始互斥锁指针
	//@参数 无
	//@返回 CXMutex互斥锁指针
	//@别名 地址()
	CXXMutex* data() const { return get(); }

	//@备注 重新锁定
	//@参数 无
	//@返回 无
	//@别名 重新锁()
	void relock();

	//@备注 交换锁
	//@参数 CXMutexLocker其他智能互斥锁对象
	//@返回 无
	//@别名 交换锁()
	void swap(CXXMutexLocker& other);

	//@备注 解锁
	//@参数 无
	//@返回 无
	//@别名 解锁()
	void unlock();

	//@隐藏{
	CXXMutexLocker& operator=(CXXMutexLocker&& other);
	//@隐藏}

private:
	CXXMutex* pMutex_ = CXX_NULLPTR;
};

//@分组}


#endif // CXXMUTEXLOCKER_H_
