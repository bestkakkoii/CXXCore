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

#ifndef CXXVECTOR_H_
//@隐藏{
#define CXXVECTOR_H_
//@隐藏}


#include <cxxglobal.h>


#include <vector>
#include <mutex>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <shared_mutex>
#include <iterator>
#include <initializer_list>
#include <type_traits>
#include <functional>

//@分组{［容器］.炫　动态数组类

//@备注 因为会与炫官方的 动态数组 冲突 所以改名为 CXXVector(动数组)
//@别名 炫动态数组
template <typename T>
class CXXVector
{
public:
	//@隐藏{
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	//@隐藏}

	//@备注 构造函数
	//@参数 无
	//@返回 无
	//@别名 构造()
	inline CXXVector()
	{}

	//@备注 构造函数
	//@参数 size 数量
	//@返回 无
	//@别名 构造(数量)
	explicit CXXVector(__int64 size)
	{
		v_.resize(static_cast<size_t>(size));
	}

	//@备注 构造函数
	//@参数 size 数量
	//@参数 t    值
	//@返回 无
	//@别名 构造(数量, 值)
	CXXVector(__int64 size, const T& t)
	{
		v_.resize(static_cast<size_t>(size), t);
	}

	//@备注 构造函数
	//@参数 v 动态数组
	//@返回 无
	//@别名 构造(动态数组)
	inline CXXVector(const CXXVector<T>& v)
		: v_(v.v_)
	{}



	//@备注 构造函数(移动)
	//@参数 other 动态数组
	//@返回 无
	//@别名 构造(动态数组)
	CXXVector(CXXVector<T>&& other)
		//@隐藏{
		noexcept
		: v_(other.v_)
		//@隐藏}
	{
		other.v_ = std::vector<T>();
	}

	//@隐藏{

	//STL Compatibility

	inline CXXVector(std::initializer_list<T> args)
		: v_(args)
	{}

	//@参数 std::vector 标准动态数组
	CXXVector(const std::vector<T>& vec)
		: v_(vec)
	{}

	CXXVector(std::vector<T>&& other)
		: v_(std::move(other))
	{

	}

	template<typename InputIt>
	CXXVector(InputIt first, InputIt last)
	{
		v_.assign(first, last);
	}

	//@隐藏}

	//@备注 析构函数
	//@参数 无
	//@返回 无
	//@别名 析构()
	virtual ~CXXVector()
	{
	}

	template<typename InputIt>
	void assign(InputIt first, InputIt last)
	{
		v_.assign(first, last);
	}


	//@备注 交换
	//@参数 other 动态数组
	//@返回 无
	//@别名 交换(动态数组)
	void swap(CXXVector<T>& other)
		//@隐藏{
		noexcept
		//@隐藏}
	{
		std::swap(v_, other.v_);
	}

	//@备注 大小
	//@参数 无
	//@返回 数量
	//@别名 大小()
	inline __int64 size() const
		//@隐藏{
		noexcept
		//@隐藏}
	{
		return v_.size();
	}

	//@备注 大小
	//@参数 无
	//@返回 数量
	inline __int64 count() const
	{
		return size();
	}

	//@备注 大小
	//@参数 无
	//@返回 数量
	inline __int64 length() const
	{
		return size();
	}

	//@备注 是否为空
	//@参数 无
	//@返回 是否为空
	//@别名 是否空()
	inline bool isEmpty() const
	{
		return static_cast<__int64>(v_.size()) <= 0;
	}

	//@备注 在不改变元素的情况下调整大小
	//@参数 size 数量
	//@返回 无
	//@别名 置大小(数量)
	void resize(__int64 size)
	{
		v_.resize(static_cast<size_t>(size));
	}

	//@备注 返回容量
	//@参数 无
	//@返回 容量
	//@别名 容量()
	inline __int64 capacity() const
	{
		return v_.capacity();
	}

	//@备注 预留空间
	//@参数 size 数量
	//@返回 无
	//@别名 预留(数量)
	void reserve(__int64 size)
	{
		v_.reserve(static_cast<size_t>(size));
	}

	//@备注 压缩 释放多余的空间
	//@参数 无
	//@返回 无
	//@别名 收缩()
	inline void squeeze()
	{
		v_.shrink_to_fit();
	}

	//@备注 指向数据的指针
	//@参数 无
	//@返回 指向数据的指针
	//@别名 地址()
	inline T* data()
	{
		return v_.data();
	}

	inline const T* data() const
	{
		return v_.data();
	}

	//@备注 指向数据的指针
	//@参数 无
	//@返回 指向数据的指针
	//@别名 取()
	inline T* get()
	{
		return v_.data();
	}

	//@备注 指向数据的指针
	//@参数 无
	//@返回 指向数据的指针
	//@别名 取()
	inline const T* get() const
	{
		return v_.data();
	}

	//@备注 指向数据的常量指针
	//@参数 无
	//@返回 指向数据的常量指针
	//@别名 常量数据指针()
	inline const T* constData() const
	{
		return v_.data();
	}

	//@备注 清空
	//@参数 无
	//@返回 无
	//@别名 清空()
	void clear()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		v_.clear();
		v_.shrink_to_fit();
	}

	//@备注 常量引用返回元素
	//@参数 i 索引
	//@返回 元素
	//@别名 在(索引)
	const T& at(__int64 i) const
	{
		return v_.at(static_cast<size_t>(i));
	}

	//@备注 将元素添加到末尾
	//@参数 t 元素
	//@返回 无
	//@别名 追加(元素)
	void append(const T& t)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		v_.push_back(t);
	}

	//@别名 将元素添加到末尾(移动)
	//@参数 t 元素
	//@返回 无
	//@别名 追加(元素)
	void append(T&& t)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		v_.push_back(std::move(t));
	}

	//@备注 将动态数组添加到末尾
	//@参数 l 动态数组
	//@返回 无
	//@别名 追加(动态数组)
	inline void append(const CXXVector<T>& l)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		v_.reserve(static_cast<size_t>(v_.size() + l.size()));
		v_.insert(v_.end(), l.v_.begin(), l.v_.end());
	}

	//@备注 将动态数组添加到最前面
	//@参数 l 动态数组
	//@返回 无
	//@别名 追加首(动态数组)
	void prepend(T&& t)
	{
		v_.insert(v_.begin(), std::move(t));
	}

	//@备注 将元素添加到最前面
	//@参数 t 元素
	//@返回 无
	//@别名 追加首(元素)
	void prepend(const T& t)
	{
		v_.insert(v_.begin(), t);
	}

	//@备注 将动态数组插入到指定位置
	//@参数 i 索引
	//@参数 l 动态数组
	//@返回 无
	//@别名 插入(索引, 动态数组)
	void insert(__int64 i, T&& t)
	{
		v_.insert(v_.begin() + static_cast<intptr_t>(i), std::move(t));
	}

	//@备注 将元素插入到指定位置
	//@参数 i 索引
	//@参数 t 元素
	//@返回 无
	//@别名 插入(索引, 元素)
	void insert(__int64 i, const T& t)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		v_.insert(v_.begin() + static_cast<intptr_t>(i), t);
	}

	//@备注 将元素插入到指定位置
	//@参数 首迭代器
	//@参数 尾迭代器
	//@参数 起始迭代器
	//@返回 无
	//@别名 插入(迭代器, 迭代器, 迭代器)
	void insert(const_iterator first, const_iterator last, const_iterator pos)
	{
		v_.insert(pos, first, last);
	}

	//@备注 将元素插入到指定位置
	//@参数 i 索引
	//@参数 n 数量
	//@参数 t 元素
	//@返回 无
	//@别名 插入(索引, 数量, 元素)
	void insert(__int64 i, __int64 n, const T& t)
	{
		if (static_cast<size_t>(i) > v_.size())
		{
			// 防止插入位置超出 vector 的有效范围
			throw std::out_of_range("Index out of range");
		}

		// 插入元素 t，共插入 n 次到位置 i 处
		v_.insert(v_.begin() + static_cast<std::ptrdiff_t>(i), static_cast<size_t>(n), t);
	}

	//@备注 替换元素
	//@参数 i 索引
	//@参数 t 元素
	//@返回 无
	//@别名 替换(索引, 元素)
	void replace(__int64 i, const T& t)
	{
		v_[static_cast<size_t>(i)] = t;
	}

	//@备注 移除元素
	//@参数 i 索引
	//@返回 无
	//@别名 移除(索引)
	void remove(__int64 i)
	{
		v_.erase(v_.begin() + static_cast<intptr_t>(i));
	}

	//@备注 移除元素
	//@参数 i 索引
	//@参数 n 数量
	//@返回 无
	//@别名 移除(索引, 数量)
	void remove(__int64 i, __int64 n)
	{
		v_.erase(v_.begin() + static_cast<intptr_t>(i), v_.begin() + static_cast<intptr_t>(i + n));
	}

	//@备注 移除第一个元素
	//@参数 无
	//@返回 无
	//@别名 移除首()
	inline void removeFirst()
	{
		assert(!isEmpty());
		v_.erase(v_.begin());
	}

	//@备注 移除最后一个元素
	//@参数 无
	//@返回 无
	//@别名 移除尾()
	inline void removeLast()
	{
		assert(!isEmpty());
		remove(size() - 1);
	}

	//@备注 取出第一个元素
	//@参数 无
	//@返回 元素
	//@别名 取出首()
	T takeFirst()
	{
		assert(!isEmpty());
		T r = v_.front();
		v_.erase(v_.begin());
		return r;
	}

	//@备注 取出最后一个元素
	//@参数 无
	//@返回 元素
	//@别名 取出尾()
	T takeLast()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		assert(!isEmpty());
		T value = std::move(v_.back());
		v_.pop_back();
		return value;
	}

	//@备注 填充
	//@参数 t    元素
	//@参数 size 数量
	//@返回 引用 CXXVector<T> 动态数组
	//@别名 填充(元素, 数量)
	CXXVector<T>& fill(const T& t, __int64 size = -1)
	{
		if (size < 0)
		{
			size = v_.size();
		}
		v_.assign(static_cast<size_t>(size), t);
		return *this;
	}

	//@备注 查找元素索引
	//@参数 t    元素
	//@参数 from 开始位置
	//@返回 索引 -1表示未找到
	//@别名 找索引(元素, 开始位置)
	__int64 indexOf(const T& t, __int64 from = 0) const
	{
		auto it = std::find(v_.begin() + static_cast<intptr_t>(from), v_.end(), t);
		return (it != v_.end()) ? std::distance(v_.begin(), it) : -1;
	}

	//@备注 从后面查找元素索引
	//@参数 t    元素
	//@参数 from 开始位置 -1表示最后一个
	//@返回 索引 -1表示未找到
	//@别名 尾找索引(元素, 开始位置)
	__int64 lastIndexOf(const T& t, __int64 from = -1) const
	{
		auto it = std::find(v_.rbegin(), v_.rend() - static_cast<intptr_t>(from), t);
		return (it != v_.rend()) ? std::distance(v_.rbegin(), it) : -1;
	}

	//@备注 是否包含元素
	//@参数 t 元素
	//@返回 是否包含
	//@别名 包含(元素)
	bool contains(const T& t) const
	{
		return std::find(v_.begin(), v_.end(), t) != v_.end();
	}

	//@备注 元素出现次数
	//@参数 t 元素
	//@返回 次数
	//@别名 数量(元素)
	__int64 count(const T& t) const
	{
		return std::count(v_.begin(), v_.end(), t);
	}

	// List compatibility

	//@备注 移除元素
	//@参数 i 索引
	//@返回 无
	//@别名 移除在(索引)
	void removeAt(__int64 i)
	{
		v_.erase(v_.begin() + static_cast<intptr_t>(i));
	}

	//@备注 移除所有指定元素
	//@参数 t 元素
	//@返回 移除的数量
	//@别名 移除所有(元素)
	__int64 removeAll(const T& t)
	{
		const const_iterator ce = cend(), cit = std::find(cbegin(), ce, t);
		if (cit == ce)
		{
			return 0;
		}

		const T tCopy = t;

		const __int64 firstFoundIdx = std::distance(this->cbegin(), cit);
		const iterator e = end(), it = std::remove(begin() + static_cast<intptr_t>(firstFoundIdx), e, tCopy);
		const __int64 result = std::distance(it, e);
		erase(it, e);
		return result;
	}

	//@备注 移除第一个找到的指定元素
	//@参数 t 元素
	//@返回 是否移除成功
	//@别名 移除一个(元素)
	bool removeOne(const T& t)
	{
		const __int64 i = indexOf(t);
		if (i < 0)
		{
			return false;
		}

		remove(i);
		return true;
	}

	//@备注 从指定位置取出元素
	//@参数 i   索引
	//@返回 元素
	//@别名 取出在(索引)
	T takeAt(__int64 i)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		T t = std::move(v_.at(static_cast<size_t>(i)));  // 使用at方法代替[]
		v_.erase(v_.begin() + static_cast<intptr_t>(i));
		return t;
	}

	//@备注 移动元素
	//@参数 from 来源索引
	//@参数 to   目标索引
	//@返回 无
	//@别名 移动(来源索引, 目标索引)
	void move(__int64 from, __int64 to)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		T t = std::move(v_.at(static_cast<size_t>(from)));
		if (from < to)
		{
			std::move(v_.begin() + static_cast<intptr_t>(from + 1), v_.begin() + static_cast<intptr_t>(to + 1), v_.begin() + static_cast<intptr_t>(from));
		}
		else
		{
			std::move_backward(v_.begin() + static_cast<intptr_t>(to), v_.begin() + static_cast<intptr_t>(from), v_.begin() + static_cast<intptr_t>(from + 1));
		}
		v_.at(static_cast<size_t>(to)) = std::move(t);  // 使用at方法代替[]
	}

	// STL-style
	//@备注 开始迭代器
	//@参数 无
	//@返回 迭代器
	//@别名 开始()
	inline iterator begin()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return v_.begin();
	}

	//@备注 常量开始迭代器
	//@参数 无
	//@返回 常量迭代器
	inline const_iterator begin() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_.cbegin();
	}

	//@备注 常量开始迭代器
	//@参数 无
	//@返回 常量迭代器
	//@别名 常量开始()
	inline const_iterator cbegin() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_.cbegin();
	}

	//@备注 常量开始迭代器
	//@参数 无
	//@返回 常量迭代器
	inline const_iterator constBegin() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_.cbegin();
	}

	//@备注 结束迭代器
	//@参数 无
	//@返回 迭代器
	//@别名 结束()
	inline iterator end()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return v_.end();
	}

	//@备注 常量结束迭代器
	//@参数 无
	//@返回 常量迭代器
	inline const_iterator end() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_.cend();
	}

	//@备注 常量结束迭代器
	//@参数 无
	//@返回 常量迭代器
	//@别名 常量结束()
	inline const_iterator cend() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_.cend();
	}

	//@备注 常量结束迭代器
	//@参数 无
	//@返回 常量迭代器
	inline const_iterator constEnd() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_.cend();
	}

	//@备注 反向开始迭代器
	//@参数 无
	//@返回 反向迭代器
	//@别名 反向开始()
	reverse_iterator rbegin()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return reverse_iterator(v_.end());
	}

	//@备注 反向结束迭代器
	//@参数 无
	//@返回 反向迭代器
	//@别名 反向结束()
	reverse_iterator rend()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return reverse_iterator(v_.begin());
	}

	//@备注 常量反向开始迭代器
	//@参数 无
	//@返回 常量反向迭代器
	const_reverse_iterator rbegin() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return const_reverse_iterator(v_.cend());
	}

	//@备注 常量反向结束迭代器
	//@参数 无
	//@返回 常量反向迭代器
	const_reverse_iterator rend() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return const_reverse_iterator(v_.cbegin());
	}

	//@备注 常量反向开始迭代器
	//@参数 无
	//@返回 常量反向迭代器
	//@别名 常量反向开始()
	const_reverse_iterator crbegin() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return const_reverse_iterator(v_.cend());
	}

	//@备注 常量反向结束迭代器
	//@参数 无
	//@返回 常量反向迭代器
	//@别名 常量反向结束()
	const_reverse_iterator crend() const noexcept
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return const_reverse_iterator(v_.cbegin());
	}

	//@备注 插入元素
	//@参数 before 位置迭代器
	//@参数 n      数量
	//@参数 x      元素
	//@返回 迭代器
	//@别名 插入(位置迭代器, 数量, 元素)
	iterator insert(iterator before, __int64 n, const T& x)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return v_.insert(before, static_cast<size_t>(n), x);
	}

	//@备注 插入元素
	//@参数 before 位置迭代器
	//@参数 x      元素
	//@返回 迭代器
	//@别名 插入(位置迭代器, 元素)
	inline iterator insert(iterator before, const T& x)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return v_.insert(before, x);
	}

	//@备注 插入元素
	//@参数 before 位置迭代器
	//@参数 x      元素
	//@返回 迭代器
	//@别名 插入(位置迭代器, 元素)
	inline iterator insert(iterator before, T&& x)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return v_.insert(before, std::move(x));
	}

	//@备注 移除元素
	//@参数 begin 开始迭代器
	//@参数 end   结束迭代器
	//@返回 迭代器
	//@别名 擦除(开始迭代器, 结束迭代器)
	iterator erase(iterator begin, iterator end)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return v_.erase(begin, end);
	}

	//@备注 移除元素
	//@参数 pos 位置迭代器
	//@返回 迭代器
	//@别名 擦除(位置迭代器)
	inline iterator erase(iterator pos)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return v_.erase(pos);
	}

	//@备注 第一个元素
	//@参数 无
	//@返回 元素
	//@别名 首()
	T& first()
	{
		assert(!v_.empty());
		return *v_.begin();
	}

	//@备注 常量第一个元素
	//@参数 无
	//@返回 元素
	inline const T& first() const
	{
		return *v_.begin();
	}

	//@备注 常量第一个元素
	//@参数 无
	//@返回 元素
	//@别名 常量首()
	inline const T& constFirst() const
	{
		return *v_.cbegin();
	}

	//@备注 最后一个元素
	//@参数 无
	//@返回 元素
	//@别名 尾()
	inline T& last()
	{
		return *v_.rbegin();
	}

	//@备注 常量最后一个元素
	//@参数 无
	//@返回 元素
	inline const T& last() const
	{
		return *v_.rbegin();
	}

	//@备注 常量最后一个元素
	//@参数 无
	//@返回 元素
	//@别名 常量尾()
	inline const T& constLast() const
	{
		return *v_.crbegin();
	}

	//@备注 检查是否以指定元素开头
	//@参数 t 元素
	//@返回 是否起始为
	//@别名 是否首为(元素)
	inline bool startsWith(const T& t) const
	{
		return !isEmpty() && v_.front() == t;
	}

	//@备注 检查是否以指定元素结尾
	//@参数 t 元素
	//@返回 是否结尾为
	//@别名 是否尾为(元素)
	inline bool endsWith(const T& t) const
	{
		return !isEmpty() && v_.back() == t;
	}

	//@备注 取出子动态数组
	//@参数 pos 开始位置
	//@参数 len 长度
	//@返回 子动态数组
	//@别名 中(开始位置, 长度)
	CXXVector<T> mid(__int64 pos, __int64 len = -1) const
	{
		return CXXVector<T>(v_.begin() + static_cast<intptr_t>(pos), v_.begin() + static_cast<intptr_t>(len < 0LL ? static_cast<intptr_t>(v_.size()) : pos + len));
	}

	//@备注 获取元素
	//@参数 i 索引
	//@返回 元素
	//@别名 值(索引)
	T value(__int64 i) const
	{
		return i < 0 || i >= (__int64)v_.size() ? T() : v_[static_cast<size_t>(i)];
	}

	//@备注 获取元素
	//@参数 i            索引
	//@参数 defaultValue 默认值 假设索引超出范围则返回 默认值
	//@返回 元素
	//@别名 值(索引, 默认值)
	T value(__int64 i, const T& defaultValue) const
	{
		return i < 0 || i >= static_cast<__int64>(v_.size()) ? defaultValue : v_[static_cast<size_t>(i)];
	}

	//@备注 将指定索引的元素交换
	//@参数 i 索引
	//@参数 j 索引
	//@返回 无
	//@别名 交换(索引, 索引)
	void swapItemsAt(__int64 i, __int64 j)
	{
		assert(i >= 0 && i < size());
		assert(j >= 0 && j < size());
		std::swap((*this)[i], (*this)[j]);
	}

	//@备注 排序
	//@别名 排序()
	void sort()
	{
		std::sort(v_.begin(), v_.end());
	}

	//@备注 排序
	//@参数 compare 比较函数
	//@别名 排序( 比较函数 )
	void sort(std::function<bool(const T&, const T&)> compare)
	{
		std::sort(v_.begin(), v_.end(), compare);
	}

	//@备注 逆序
	//@别名 反向排序()
	void rsort()
	{
		std::sort(v_.rbegin(), v_.rend());
	}

	//@备注 逆序
	//@参数 compare 比较函数
	//@别名 反向排序( 比较函数 )
	void rsort(std::function<bool(const T&, const T&)> compare)
	{
		std::sort(v_.rbegin(), v_.rend(), compare);
	}

public:
	// STL compatibility

	//@备注 適配C++的STL (std::vector)
	inline void push_back(const T& t) { v_.push_back(t); }

	//@备注 適配C++的STL (std::vector)
	void push_back(T&& t) { v_.push_back(std::move(t)); }

	//@备注 適配C++的STL (std::vector)
	void push_front(T&& t) { v_.insert(v_.begin(), std::move(t)); }

	//@备注 適配C++的STL (std::vector)
	inline void push_front(const T& t) { v_.insert(v_.begin(), t); }

	//@备注 適配C++的STL (std::vector)
	T pop_back()
	{
		assert(!isEmpty());
		T t = std::move(v_.back());
		v_.pop_back();

		return t;
	}

	//@备注 適配C++的STL (std::vector)
	T pop_front()
	{
		assert(!isEmpty());
		T t = std::move(v_.front());
		v_.erase(v_.begin());

		return t;
	}

	//@备注 適配C++的STL (std::vector)
	inline bool empty() const { return isEmpty(); }

	//@备注 適配C++的STL (std::vector)
	inline T& front() { return v_.front(); }

	//@备注 適配C++的STL (std::vector)
	inline const T& front() const { return v_.front(); }

	//@备注 適配C++的STL (std::vector)
	inline T& back() { return v_.back(); }

	//@备注 適配C++的STL (std::vector)
	inline const T& back() const { return last(); }

	//@备注 適配C++的STL (std::vector)
	void shrink_to_fit() { v_.shrink_to_fit(); }

	//@备注 適配C++的STL (std::vector)
	inline iterator find(const T& t)
	{
		return std::find(v_.begin(), v_.end(), t);
	}

	//@备注 適配C++的STL (std::vector)
	inline const_iterator find(const T& t) const
	{
		return std::find(v_.cbegin(), v_.cend(), t);
	}

	//@备注 適配C++的STL (std::vector)
	std::vector<T> toStdVector() const
	{
		return v_;
	}

public:
	//@隐藏{

	T& operator[](__int64 i)
	{
		assert(i >= 0 && i < static_cast<__int64>(v_.size()));
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_[static_cast<size_t>(i)];
	}


	const T& operator[](__int64 i) const
	{
		assert(i >= 0 && i < static_cast<__int64>(v_.size()));
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return v_[static_cast<size_t>(i)];
	}

	CXXVector<T>& operator=(std::initializer_list<T> args) { v_ = args; return *this; }

	CXXVector<T>& operator=(CXXVector<T>&& other) noexcept
	{
		if (this != &other)
		{
			v_ = std::move(other.v_);
		}
		return *this;
	}

	CXXVector<T>& operator=(const CXXVector<T>& v)
	{
		if (this != &v)
		{
			v_ = v.v_;
		}
		return *this;
	}

	bool operator==(const CXXVector<T>& v) const { return v_ == v.v_; }

	inline bool operator!=(const CXXVector<T>& v) const { return !(*this == v); }

	CXXVector<T>& operator+=(const CXXVector<T>& l)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		v_.insert(v_.end(), l.v_.begin(), l.v_.end());
		return *this;
	}

	CXXVector<T>& operator+=(CXXVector<T>&& l)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		v_.insert(v_.end(), std::make_move_iterator(l.v_.begin()), std::make_move_iterator(l.v_.end()));
		return *this;
	}

	inline CXXVector<T> operator+(const CXXVector<T>& l) const
	{
		CXXVector<T> n(std::move(*this));
		n += l;
		return n;
	}

	inline CXXVector<T>& operator+=(const T& t)
	{
		v_.push_back(t); return *this;
	}

	inline CXXVector<T>& operator<< (const T& t)
	{
		v_.push_back(t); return *this;
	}

	inline CXXVector<T>& operator<<(const CXXVector<T>& l)
	{
		*this += l; return *this;
	}

	inline CXXVector<T>& operator+=(T&& t)
	{
		v_.push_back(std::move(t)); return *this;
	}


	// 重载 += 操作符来拼接另一个数组
	CXXVector<T>& operator+=(const std::vector<T>& vec)
	{
		v_.insert(v_.end(), vec.begin(), vec.end());
		return *this;
	}

	// 重载 * 操作符，返回元素的倍数 (适用于数字类型 T)
	template<typename U = T, typename = std::enable_if_t<std::is_arithmetic<U>::value>>
	CXXVector<U> operator*(U scalar) const
	{
		CXXVector<U> result(*this);
		for (auto& item : result.v_)
		{
			item *= scalar;
		}
		return result;
	}

	// 支持用下标进行区间访问
	CXXVector<T> operator()(__int64 start, __int64 _length) const
	{
		assert(start + length <= v_.size());
		return CXXVector<T>(v_.begin() + static_cast<size_t>(start), v_.begin() + static_cast<size_t>(start) + static_cast<size_t>(_length));
	}

	operator std::vector<T>() const { return v_; }

	template<typename U = T, typename = std::enable_if_t<std::is_arithmetic<U>::value>>
	CXXVector<U> operator/(U scalar) const
	{
		CXXVector<U> result(*this);
		for (auto& item : result.v_)
		{
			item /= scalar;
		}
		return result;
	}

	//@隐藏}

private:
	//@隐藏{
	mutable std::shared_mutex mutex_;
	std::vector<T> v_;
	//@隐藏}
};

//@隐藏{

// 重载全局运算符
template<typename T>
bool operator<(const CXXVector<T>& lhs, const CXXVector<T>& rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T>
bool operator<=(const CXXVector<T>& lhs, const CXXVector<T>& rhs)
{
	return !(rhs < lhs);
}

template<typename T>
bool operator>(const CXXVector<T>& lhs, const CXXVector<T>& rhs)
{
	return rhs < lhs;
}

template<typename T>
bool operator>=(const CXXVector<T>& lhs, const CXXVector<T>& rhs)
{
	return !(lhs < rhs);
}

// 宽字符输出运算符重载 <<
template<typename T>
std::wostream& operator<<(std::wostream& out, const CXXVector<T>& vector)
{
	out << L"[ ";
	for (const auto& elem : vector)
	{
		out << elem << L" "; // 输出每个元素
	}
	out << L"]";
	return out;
}

// 宽字符输入运算符重载 >>
template<typename T>
std::wistream& operator>>(std::wistream& in, CXXVector<T>& vector)
{
	T value;
	while (in >> value)
	{
		vector.push_back(value); // 从输入流中读取数据并存入 vector
	}
	return in;
}
//@隐藏}




//@备注 声明方式如: CXXByteArray 等同 CXXVector<BYTE>
// 除了 CXXVector 本身的函数之外，还增加了一些字节集相关的函数
//@别名 炫字节集
class CXXByteArray : public CXXVector<BYTE>
{
public:
	CXXByteArray()
	{}

	virtual ~CXXByteArray()
	{}

	CXXByteArray(const CXXVector<BYTE>& v)
		: CXXVector<BYTE>(v)
	{}

	CXXByteArray(const CXXByteArray& v)
		: CXXVector<BYTE>(v)
	{}

	//@隐藏{
	CXXByteArray(CXXVector<BYTE>&& v) noexcept
		: CXXVector<BYTE>(std::move(v))
	{}

	CXXByteArray(const std::vector<BYTE>& v)
		: CXXVector<BYTE>(v)
	{}

	CXXByteArray(std::vector<BYTE>&& v) noexcept
		: CXXVector<BYTE>(std::move(v))
	{}

	CXXByteArray(std::initializer_list<BYTE> args)
		: CXXVector<BYTE>(args)
	{}

	CXXByteArray& operator=(std::initializer_list<BYTE> args)
	{
		CXXVector<BYTE>::operator=(args);
		return *this;
	}

	CXXByteArray& operator=(const CXXVector<BYTE>& v)
	{
		CXXVector<BYTE>::operator=(v);
		return *this;
	}

	CXXByteArray& operator=(const CXXByteArray& v)
	{
		CXXVector<BYTE>::operator=(v);
		return *this;
	}

	CXXByteArray& operator=(CXXByteArray&& v) noexcept
	{
		CXXVector<BYTE>::operator=(std::move(v));
		return *this;
	}

	CXXByteArray& operator=(const std::vector<BYTE>& v)
	{
		CXXVector<BYTE>::operator=(v);
		return *this;
	}

	CXXByteArray& operator=(std::vector<BYTE>&& v) noexcept
	{
		CXXVector<BYTE>::operator=(std::move(v));
		return *this;
	}

	//@隐藏}

	CXXByteArray(const BYTE* begin, const BYTE* end)
		: CXXVector<BYTE>(begin, end)
	{}

	CXXByteArray(const_iterator first, const_iterator last)
		: CXXVector<BYTE>(first, last)
	{}

	CXXByteArray(iterator first, iterator last)
		: CXXVector<BYTE>(first, last)
	{}

	CXXByteArray(const BYTE* data, __int64 size)
	{
		resize(size);
		memmove_s(this->data(), static_cast<size_t>(size), data, static_cast<size_t>(size));
	}

	CXXByteArray(const std::string& str)
	{
		resize(str.size());
		memmove_s(this->data(), static_cast<size_t>(str.size()), str.data(), str.size());
	}

	CXXByteArray(const std::wstring& str)
	{
		resize(str.size() * sizeof(wchar_t));
		memmove_s(this->data(), static_cast<size_t>(str.size() * sizeof(wchar_t)), str.data(), str.size() * sizeof(wchar_t));
	}

	CXXByteArray(const char* data, __int64 size)
	{
		resize(size);
		memmove_s(this->data(), static_cast<size_t>(size), data, static_cast<size_t>(size));
	}

	CXXByteArray(const wchar_t* data, __int64 size)
	{
		resize(size * sizeof(wchar_t));
		memmove_s(this->data(), static_cast<size_t>(size * sizeof(wchar_t)), data, static_cast<size_t>(size * sizeof(wchar_t)));
	}

	//@备注 比较两个字节集是否相等
	//@参数 b 字节集
	//@返回 是否相等
	//@别名 比较(字节集)
	bool compare(const CXXByteArray& b) const
	{
		return *this == b;
	}

	//@备注 将字节集以指定字节分割
	//@参数 sep 分隔符
	//@返回 分割后的字节集
	//@别名 分割(分隔符)
	CXXByteArray split(BYTE sep) const
	{
		CXXByteArray result;
		__int64 start = 0;
		for (__int64 i = 0; i < size(); i++)
		{
			if (at(i) == sep)
			{
				result.append(mid(start, i - start));
				start = i + 1;
			}
		}
		result.append(mid(start));
		return result;
	}

	//@备注 转换为十六进制
	//@返回 十六进制字节集
	//@别名 到十六进制()
	CXXByteArray toHex() const
	{
		CXXByteArray hex;
		hex.reserve(size() * 2);
		for (BYTE b : *this)
		{
			hex.append(static_cast<BYTE>(b >> 4));
			hex.append(static_cast<BYTE>(b & 0x0F));
		}
		return hex;
	}

	//@备注 转换StdString
	//@返回 字符串
	//@别名 到字符串()
	std::string toStdString() const
	{
		return std::string(reinterpret_cast<const char*>(data()), static_cast<size_t>(size()));
	}

	//@备注 将头尾的空白字符去掉
	//@返回 去掉空白字符的字节集
	//@别名 修剪()
	CXXByteArray trimmed() const
	{
		__int64 i = 0;
		for (; i < size(); i++)
		{
			if (at(i) != ' ' && at(i) != '\t' && at(i) != '\r' && at(i) != '\n')
			{
				break;
			}
		}

		__int64 j = size() - 1;
		for (; j >= 0; j--)
		{
			if (at(j) != ' ' && at(j) != '\t' && at(j) != '\r' && at(j) != '\n')
			{
				break;
			}
		}

		return mid(i, j - i + 1);
	}

	//@备注 转换为浮点型
	//@返回 浮点数
	//@别名 到浮点()
	float toFloat() const
	{
		return *reinterpret_cast<const float*>(data());
	}

	//@备注 转换为双精度浮点型
	//@返回 双精度浮点数
	//@别名 到双浮点()
	double toDouble() const
	{
		return *reinterpret_cast<const double*>(data());
	}

	//@备注 转换为64位整数
	//@返回 64位整数
	//@别名 到长整型()
	__int64 toInt64() const
	{
		return *reinterpret_cast<const __int64*>(data());
	}

	//@备注 转换为32位整数
	//@返回 32位整数
	//@别名 到整型()
	int toInt() const
	{
		return *reinterpret_cast<const int*>(data());
	}

	//@备注 转换为16位整数
	//@返回 16位整数
	//@别名 到短整型()
	short toShort() const
	{
		return *reinterpret_cast<const short*>(data());
	}

	//@备注 转换base64
	//@返回 base64字节集
	//@别名 到BASE64()
	CXXByteArray toBase64() const
	{
		constexpr const char* base64EncodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		CXXByteArray base64;
		base64.reserve(size() * 4 / 3 + 4);
		CXXByteArray::const_iterator it = cbegin();
		while (it != cend())
		{
			BYTE b1 = *it++;
			BYTE b2 = (it != cend()) ? *it++ : 0;
			BYTE b3 = (it != cend()) ? *it++ : 0;

			BYTE c1 = b1 >> 2;
			BYTE c2 = ((b1 & 0x03) << 4) | (b2 >> 4);
			BYTE c3 = ((b2 & 0x0F) << 2) | (b3 >> 6);
			BYTE c4 = b3 & 0x3F;

			base64.push_back(base64EncodeTable[c1]);
			base64.push_back(base64EncodeTable[c2]);
			base64.push_back((b2 != 0) ? base64EncodeTable[c3] : '=');
			base64.push_back((b3 != 0) ? base64EncodeTable[c4] : '=');
		}
		return base64;
	}

	//@备注 转换为大写
	//@返回 大写字节集
	//@别名 到大写()
	CXXByteArray toUpper() const
	{
		CXXByteArray upper;
		upper.reserve(size());
		for (BYTE b : *this)
		{
			upper.push_back(static_cast<BYTE>(toupper(b)));
		}
		return upper;
	}

	//@备注 转换为小写
	//@返回 小写字节集
	//@别名 到小写()
	CXXByteArray toLower() const
	{
		CXXByteArray lower;
		lower.reserve(size());
		for (BYTE b : *this)
		{
			lower.push_back(static_cast<BYTE>(tolower(b)));
		}
		return lower;
	}
};


//@分组}


#endif // CXXVECTOR_H_