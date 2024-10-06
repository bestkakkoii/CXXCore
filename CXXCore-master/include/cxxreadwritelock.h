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

#ifndef CXXREADWRITELOCK_H_
//@隐藏{
#define CXXREADWRITELOCK_H_
//@隐藏}



#include <windows.h>
#include <atomic>
#include <cxxglobal.h>

//@别名 炫
namespace cxx
{

}

//@分组{［线程安全］.炫　线程共享锁类

//@备注 适用场合：多读少写的场合，当有写操作时，所有读操作都会被阻塞，直到写操作完成
// 而读取操作不会互斥，所以可以多个线程同时进行读取操作
//@别名 炫读写锁
class CXXReadWriteLock
{
public:
	//@备注 构造函数
	//@参数 递归模式(不填默认为非递归)
	//@返回 无
	//@别名 读写锁(递归模式)
	explicit CXXReadWriteLock(cxx::RecursionMode recursionMode = cxx::NonRecursive);
	virtual ~CXXReadWriteLock();

	//@备注 锁定共享读，如果有其他线程正在写则阻塞
	//@参数 无
	//@返回 无
	//@别名 锁读()
	void lockForRead();

	//@备注 锁定独占写，如果有其他线程正在读或写则阻塞
	//@参数 无
	//@返回 无
	//@别名 锁写()
	void lockForWrite();

	//@备注 尝试锁定共享读，如果锁定失败则立即返回
	//@参数 无
	//@返回 是否锁定成功，true为成功，false为失败
	//@别名 尝试锁读()
	bool tryLockForRead();

	//@备注 尝试锁定共享读，如果锁定失败则等待指定时间，如果超时则返回
	//@参数 超时时间(毫秒)
	//@返回 是否锁定成功，true为成功，false为失败
	//@别名 尝试锁读(超时时间)
	bool tryLockForRead(int timeout);

	//@备注 尝试锁定独占写，如果锁定失败则立即返回
	//@参数 无
	//@返回 是否锁定成功，true为成功，false为失败
	//@别名 尝试锁写()
	bool tryLockForWrite();

	//@备注 尝试锁定独占写，如果锁定失败则等待指定时间，如果超时则返回
	//@参数 超时时间(毫秒)
	//@返回 是否锁定成功，true为成功，false为失败
	//@别名 尝试锁写(超时时间)
	bool tryLockForWrite(int timeout);

	//@备注 解锁
	//@参数 无
	//@返回 无
	//@别名 解锁()
	void unlock();
private:
	SRWLOCK srwlock_;
	CONDITION_VARIABLE condition_;
	std::atomic<__int64> numReaders_;
	std::atomic<__int64> numPendingWriters_;
	std::atomic<__int64> numWriters_;
	const cxx::RecursionMode recursionMode_;
	DWORD writerThreadId_;
};

//@分组}

#endif // CXXREADWRITELOCK_H_
