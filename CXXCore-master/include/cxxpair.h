/*
				GNU GENERAL PUBLIC LICENSE
				   Version 3, June 1991
COPYRIGHT (C) Bestkakkoii 2024 All Rights Reserved.
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/
#ifndef CXXPAIR_H_
//@隐藏{
#define CXXPAIR_H_
//@隐藏}



#include <cxxglobal.h>
#include <utility>
#include <algorithm>

//@分组{［容器］.炫　配对类

//@备注 基于 std::pair 的 CXXPair 类，类似 QPair
//@别名 炫配对
template<typename T1, typename T2>
class CXXPair
{
public:
	//@隐藏{
	// 公有类型定义
	typedef T1 first_type;
	typedef T2 second_type;
	//@隐藏}

	//@别名 第一
	T1 first;

	//@别名 第二
	T2 second;

	//@备注 默认构造函数，创建空的键值对
	//@别名 默认构造 ()
	CXXPair()
		: first(T1())
		, second(T2())
	{
	}

	//@备注 构造函数，使用两个值初始化
	//@参数 value1 第一个值
	//@参数 value2 第二个值
	//@别名 构造 (第一值, 第二值)
	CXXPair(const T1& value1, const T2& value2)
		: first(value1)
		, second(value2)
	{
	}

	//@备注 拷贝构造函数
	//@参数 p 被拷贝的 CXXPair
	//@别名 拷贝构造 (配对)
	CXXPair(const CXXPair<T1, T2>& p)
		: first(p.first)
		, second(p.second)
	{
	}

	//@备注 移动构造函数
	//@参数 p 被移动的 CXXPair
	//@别名 移动构造 (配对)
	CXXPair(CXXPair<T1, T2>&& p) noexcept
		: first(std::move(p.first))
		, second(std::move(p.second))
	{
	}

	//@隐藏{
	//STL Compatibility
	CXXPair(const std::pair<T1, T2>& other)
		: first(other.first)
		, second(other.second)
	{

	}

	CXXPair(std::pair<T1, T2>&& other) noexcept
		: first(std::move(other.first))
		, second(std::move(other.second))
	{

	}


	//@隐藏}

	std::pair<T1, T2> toStdPair()
	{
		return std::pair<T1, T2>(first, second);
	}

	//@备注 交换两个 CXXPair 对象的内容
	//@参数 other 另一个 CXXPair
	//@别名 交换 (配对)
	void swap(CXXPair<T1, T2>& other) noexcept
	{
		std::swap(first, other.first);
		std::swap(second, other.second);
	}

public:
	//@隐藏{

	//@备注 赋值运算符重载
	//@参数 p 被赋值的 CXXPair
	//@返回 当前对象的引用
	//@别名 赋值 (配对)
	CXXPair<T1, T2>& operator=(const CXXPair<T1, T2>& p)
	{
		if (this != &p)
		{
			first = p.first;
			second = p.second;
		}
		return *this;
	}

	//@备注 移动赋值运算符重载
	//@参数 p 被赋值的 CXXPair
	//@返回 当前对象的引用
	//@别名 移动赋值 (配对)
	CXXPair<T1, T2>& operator=(CXXPair<T1, T2>&& p) noexcept
	{
		if (this != &p)
		{
			first = std::move(p.first);
			second = std::move(p.second);
		}
		return *this;
	}

	//@备注 比较两个 CXXPair 对象是否相等
	//@参数 p1 第一个 CXXPair
	//@参数 p2 第二个 CXXPair
	//@返回 布尔值，表示是否相等
	//@别名 相等运算符 (配对1, 配对2)
	friend bool operator==(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
	{
		return p1.first == p2.first && p1.second == p2.second;
	}

	//@备注 比较两个 CXXPair 对象是否不相等
	//@参数 p1 第一个 CXXPair
	//@参数 p2 第二个 CXXPair
	//@返回 布尔值，表示是否不相等
	//@别名 不等运算符 (配对1, 配对2)
	friend bool operator!=(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
	{
		return !(p1 == p2);
	}

	//@备注 比较两个 CXXPair 对象的大小关系
	//@参数 p1 第一个 CXXPair
	//@参数 p2 第二个 CXXPair
	//@返回 布尔值，表示 p1 是否小于 p2
	//@别名 小于运算符 (配对1, 配对2)
	friend bool operator<(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
	{
		return p1.first < p2.first || (!(p2.first < p1.first) && p1.second < p2.second);
	}

	//@备注 比较两个 CXXPair 对象是否小于或等于
	//@参数 p1 第一个 CXXPair
	//@参数 p2 第二个 CXXPair
	//@返回 布尔值，表示 p1 是否小于等于 p2
	//@别名 小于等于运算符 (配对1, 配对2)
	friend bool operator<=(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
	{
		return !(p2 < p1);
	}

	//@备注 比较两个 CXXPair 对象是否大于
	//@参数 p1 第一个 CXXPair
	//@参数 p2 第二个 CXXPair
	//@返回 布尔值，表示 p1 是否大于 p2
	//@别名 大于运算符 (配对1, 配对2)
	friend bool operator>(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
	{
		return p2 < p1;
	}

	//@备注 比较两个 CXXPair 对象是否大于或等于
	//@参数 p1 第一个 CXXPair
	//@参数 p2 第二个 CXXPair
	//@返回 布尔值，表示 p1 是否大于等于 p2
	//@别名 大于等于运算符 (配对1, 配对2)
	friend bool operator>=(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
	{
		return !(p1 < p2);
	}
	//@隐藏}
};

//@备注 创建一个 CXXPair 对象
//@参数 value1 第一个值
//@参数 value2 第二个值
//@返回 CXXPair 对象
//@别名 创建配对 (第一值, 第二值)
template<typename T1, typename T2>
static CXXPair<T1, T2> cxxMakePair(const T1& value1, const T2& value2)
{
	return CXXPair<T1, T2>(value1, value2);
}

//@备注 交换两个 CXXPair 对象的内容
//@参数 lhs 第一个 CXXPair
//@参数 rhs 第二个 CXXPair
//@别名 交换 (配对1, 配对2)
template<typename T1, typename T2>
static void swap(CXXPair<T1, T2>& lhs, CXXPair<T1, T2>& rhs) noexcept
{
	lhs.swap(rhs);
}

//@隐藏{
template<typename T1, typename T2>
bool operator!=(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
{
	return !(p1 == p2);
}

template<typename T1, typename T2>
bool operator<(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
{
	if (p1.first < p2.first)
		return true;
	if (p2.first < p1.first)
		return false;
	return p1.second < p2.second;
}

template<typename T1, typename T2>
bool operator<=(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
{
	return !(p2 < p1);
}

template<typename T1, typename T2>
bool operator==(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
{
	return p1.first == p2.first && p1.second == p2.second;
}

template<typename T1, typename T2>
bool operator>(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
{
	return p2 < p1;
}

template<typename T1, typename T2>
bool operator>=(const CXXPair<T1, T2>& p1, const CXXPair<T1, T2>& p2)
{
	return !(p1 < p2);
}

//@隐藏}






//@分组}

#endif // CXXPAIR_H
