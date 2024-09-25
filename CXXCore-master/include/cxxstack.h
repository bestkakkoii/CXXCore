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

#ifndef CXXSTACK_H_
//@隐藏{
#define CXXSTACK_H_
//@隐藏}



#include <cxxglobal.h>

#include <stack>
#include <atomic>
#include <shared_mutex>

//@分组{［容器］.炫　栈容器类

//@备注 后进先出（Last In First Out, LIFO）数据结构
//@别名 炫栈
template <typename T>
class CXXStack
{
public:
	//@参数 是否加锁, 默认不加锁
	CXXStack(bool useLock = false)
		: useLock(useLock)
	{
	}

	//@隐藏{
	virtual ~CXXStack() {}
	//@隐藏}

	//@备注 弹出栈顶元素
	//@返回 无
	//@别名 弹出()
	void pop()
	{
		lockForWrite();
		if (static_cast<__int64>(s.size()) > 0)
		{
			s.pop();
		}
		unlockForWrite();
	}

	//@备注 向栈中压入元素
	//@参数 要压入栈中的元素
	//@返回 无
	//@别名 压入()
	void push(const T& t)
	{
		lockForWrite();
		s.push(t);
		unlockForWrite();
	}

	//@备注 交换两个栈的内容
	//@参数 另一个栈的引用
	//@返回 无
	//@别名 交换(另一个栈)
	void swap(CXXStack<T>& other)
	{
		lockForWrite();
		other.lockForWrite();
		s.swap(other.s);
		other.unlockForWrite();

		unlockForWrite();
	}

	//@备注 获取栈顶元素（非常量版本）
	//@返回 返回栈顶元素的引用
	//@别名 顶()
	T& top()
	{
		lockForWrite();
		T& value = s.top();
		unlockForWrite();
		return value;
	}

	//@备注 获取栈顶元素（常量版本）
	//@返回 返回栈顶元素的常量引用
	//@别名 常量顶() const
	const T& ctop() const
	{
		lockForRead();
		const T& value = s.top();
		unlockForRead();
		return value;
	}

	//@备注 判断栈是否为空
	//@返回 返回栈是否为空
	//@别名 是否空() const
	bool isEmpty() const
	{
		lockForRead();
		bool empty = static_cast<__int64>(s.size()) <= 0;
		unlockForRead();
		return empty;
	}

	//@备注 判断栈是否为空 STL 风格
	//@返回 返回栈是否为空
	bool empty() const
	{
		return isEmpty();
	}

	//@备注 获取栈的大小
	//@返回 返回栈的大小
	//@别名 大小()
	__int64 size() const
	{
		lockForRead();
		__int64 size = static_cast<__int64>(s.size());
		unlockForRead();
		return size;
	}

	//@备注 获取栈的大小
	//@返回 返回栈的大小
	__int64 count() const
	{
		return size();
	}

	//@备注 获取栈的大小
	//@返回 返回栈的大小
	__int64 length() const
	{
		return size();
	}
private:
	//@加读锁
	void lockForRead() const
	{
		if (useLock.load(std::memory_order_acquire))
		{
			mtx.lock_shared();
		}
	}
	//@解读锁
	void unlockForRead() const
	{
		if (useLock.load(std::memory_order_acquire))
		{
			mtx.unlock_shared();
		}
	}
	//@加写锁
	void lockForWrite() const
	{
		if (useLock.load(std::memory_order_acquire))
		{
			mtx.lock_shared();
		}
	}
	//@解写锁
	void unlockForWrite() const
	{
		if (useLock.load(std::memory_order_acquire))
		{
			mtx.unlock();
		}
	}

private:
	//@隐藏{
	std::stack<T> s;
	std::atomic_bool useLock;
	mutable std::shared_mutex mtx;
	//@隐藏{
};

//@分组}

#endif // CXXSTACK_H_