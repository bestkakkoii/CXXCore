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

#ifndef CXXQUEUE_H_
//@隐藏{
#define CXXQUEUE_H_
//@隐藏}



#include <cxxglobal.h>

#include <atomic>
#include <queue>
#include <shared_mutex>

//@分组{［容器］.炫　队列容器类

//@备注 先进先出（First In First Out, FIFO）数据结构
//@别名 炫队列
template <typename T>
class CXXQueue
{
public:
	//@参数  是否加锁, 默认不加锁
	//@参数  队列最大长度, 默认不限制
	CXXQueue(bool useLock_ = false, int size = -1)
		: useLock_(useLock_)
		, maxSizeAllowed_(size == -1 ? static_cast<size_t>(-1) : static_cast<size_t>(size))
	{
	}

	//@隐藏{
	virtual ~CXXQueue()
	{}
	//@隐藏}

	//@备注 出队
	//@返回  返回队首元素
	//@别名  出队()
	T dequeue()
	{
		lockForWrite();
		if (static_cast<__int64>(q_.size()) <= 0)
		{
			unlockForWrite();
			return T();
		}

		T value = q_.front();
		q_.pop();
		unlockForWrite();
		return value;
	}

	//@备注 入队
	//@参数  要加入队尾的元素
	//返回  无
	//@别名  入队()
	void enqueue(const T& t)
	{
		lockForWrite();
		if (maxSizeAllowed_ > 0 && q_.size() >= maxSizeAllowed_)
		{
			q_.pop();
		}
		q_.push(t);
		unlockForWrite();
	}

	//@备注 获取队首元素（非常量版本）
	//@返回  返回队首元素的引用
	//@别名 首()
	T& front()
	{
		lockForWrite();
		T& value = q_.front();
		unlockForWrite();
		return value;
	}

	//@备注 获取队首元素（常量版本）
	//@返回  返回队首元素的常量引用
	//@别名 常量首() const
	const T& cfront() const
	{
		lockForRead();
		const T& value = q_.front();
		unlockForRead();
		return value;
	}

	//@备注 获取队尾元素（非常量版本）
	//@返回  返回队尾元素的引用
	//@别名 尾()
	T& back()
	{
		lockForWrite();
		T& value = q_.back();
		unlockForWrite();
		return value;
	}

	//@备注 获取队尾元素（常量版本）
	//@返回  返回队尾元素的常量引用
	//@别名 常量尾() const
	const T& cback() const
	{
		lockForRead();
		const T& value = q_.back();
		unlockForRead();
		return value;
	}

	//@备注 交换两个队列
	//@参数  另一个队列的引用
	//@返回  无
	//@别名 交换(另一个队列)
	void swap(CXXQueue<T>& other)
	{
		lockForWrite();
		other.lockForWrite();
		q_.swap(other.q_);
		other.unlockForWrite();

		unlockForWrite();
	}

	//@备注 大小
	//@返回  返回队列的大小
	//@别名 大小() const
	__int64 size() const
	{
		lockForRead();
		__int64 size = static_cast<__int64>(q_.size());
		unlockForRead();
		return size;
	}

	//@备注 判断队列是否为空
	//@返回  返回队列是否为空
	//@别名 是否空() const
	bool isEmpty() const
	{
		lockForRead();
		bool empty = static_cast<__int64>(q_.size()) <= 0;
		unlockForRead();
		return empty;
	}

	//@备注 判断队列是否为空 STL
	//@返回  返回队列是否为空
	bool empty() const
	{
		return isEmpty();
	}

	//@备注 设置是否使用锁
	//@参数  是否使用锁
	//@返回  无
	//@别名 置使用锁(是否使用锁)
	void setLockEnabled(bool useLock)
	{
		useLock_.store(useLock, std::memory_order_release);
	}

private:
	//@隐藏{
	//加读锁
	void lockForRead() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mtx_.lock_shared();
		}
	}

	//解读锁
	void unlockForRead() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mtx_.unlock();  // 在非 Windows 平台上，使用普通互斥锁作为替代方案
		}
	}

	//加写锁
	void lockForWrite() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mtx_.lock();
		}
	}

	//解写锁
	void unlockForWrite() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mtx_.lock_shared();
		}
	}
	//@隐藏}

private:
	//@隐藏{
	std::queue<T> q_;
	std::atomic_bool useLock_;
	size_t maxSizeAllowed_;

	mutable std::shared_mutex mtx_;
	//@隐藏}
};

//@分组}

#endif // CXXQUEUE_H_