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

//@分组{［容器］.炫　集合类

#ifndef CXXSET_H_
//@隐藏{
#define CXXSET_H_
//@隐藏}



#include <set>
#include <algorithm>

//@备注 CXXSet 是一个基于 std::set 的容器类，适合存储有序且不重复的元素。该容器特别适用于需要快速查找元素存在性、自动排序、避免重复元素的场景。
//       CXXSet 的底层实现使用红黑树，因此在插入、删除、查找操作上具有 O(log n) 的时间复杂度。
//       这个容器适合用于存储集合数据，像是管理用户 ID 集合、记录已访问过的页面等。
//@别名 炫集合
template<typename T>
class CXXSet
{
public:
	//@备注 默认构造函数，创建一个空的集合。集合中元素按默认的升序排列，且每个元素只能存在一次。
	//@别名 默认构造 ()
	CXXSet() = default;

	//@备注 拷贝构造函数，创建一个新集合，并将另一个 CXXSet 集合的内容复制到该集合中。
	//@参数 other 被拷贝的 CXXSet 对象
	//@别名 拷贝构造 (集合)
	CXXSet(const CXXSet<T>& other)
		: set_(other.set_)
	{
	}

	//@备注 移动构造函数，创建一个新集合，并从另一个 CXXSet 集合中移动内容到该集合，避免拷贝。
	//@参数 other 被移动的 CXXSet 对象
	//@别名 移动构造 (集合)
	CXXSet(CXXSet<T>&& other) noexcept
		: set_(std::move(other.set_))
	{
	}

	virtual ~CXXSet()
	{

	}

	//@备注 插入一个元素到集合中。如果元素已经存在，插入将会失败，因为集合中的元素是唯一的。
	//@参数 value 要插入的元素
	//@返回 布尔值，表示插入是否成功（如果元素已经存在则返回 false）
	//@别名 插入 (元素)
	bool insert(const T& value)
	{
		return set_.insert(value).second;
	}

	//@备注 移除集合中的指定元素。如果该元素存在于集合中，它将被移除。
	//@参数 value 要移除的元素
	//@返回 size_t 类型，表示成功移除的元素个数（如果元素不存在，返回 0）
	//@别名 移除 (元素)
	__int64 remove(const T& value)
	{
		return set_.erase(value);
	}

	//@备注 检查集合中是否包含指定的元素。
	//@参数 value 要检查的元素
	//@返回 布尔值，表示集合中是否包含该元素
	//@别名 包含 (元素)
	bool contains(const T& value) const
	{
		return set_.find(value) != set_.end();
	}

	//@备注 获取集合中的元素个数。
	//@返回 集合中元素的个数
	//@别名 大小 ()
	__int64 size() const
	{
		return set_.size();
	}

	//@备注 获取集合中的元素个数。
	//@返回 集合中元素的个数
	__int64 count() const
	{
		return size();
	}

	//@备注 获取集合中的元素个数。
	//@返回 集合中元素的个数
	__int64 length() const
	{
		return size();
	}

	//@备注 清空集合，移除集合中的所有元素。
	//@别名 清空 ()
	void clear()
	{
		set_.clear();
	}

	//@备注 检查集合是否为空。
	//@返回 布尔值，表示集合是否为空（true 表示集合为空）
	//@别名 是否空 ()
	bool isEmpty() const
	{
		return set_.empty();
	}

	//@备注 检查集合是否为空。
	//@返回 布尔值，表示集合是否为空（true 表示集合为空）
	bool empty() const
	{
		return isEmpty();
	}

	//@备注 交换两个集合的内容。
	//@参数 other 要交换的 CXXSet 对象
	//@别名 交换 (集合)
	void swap(CXXSet<T>& other)
	{
		set_.swap(other.set_);
	}

	//@隐藏{
	typedef typename std::set<T>::iterator iterator;
	typedef typename std::set<T>::const_iterator const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	//@隐藏}

	//@备注 获取集合的底层迭代器，可以用于遍历集合中的元素。
	//@返回 集合的迭代器，指向集合中第一个元素
	//@别名 开始 ()
	iterator begin()
	{
		return set_.begin();
	}

	//@备注 获取集合的底层迭代器，表示遍历结束。
	//@返回 集合的迭代器，指向集合结束的位置（不指向任何元素）
	//@别名 结束 ()
	iterator end()
	{
		return set_.end();
	}

	//@备注 常量版本的 begin 函数，用于只读访问集合。
	//@返回 常量迭代器，指向集合中第一个元素
	const_iterator begin() const
	{
		return set_.begin();
	}

	//@备注 常量版本的 end 函数，用于只读访问集合。
	//@返回 常量迭代器，指向集合结束的位置（不指向任何元素）
	const_iterator end() const
	{
		return set_.end();
	}

	//@备注 常量版本的 begin 函数，用于只读访问集合。
	//@返回 常量迭代器，指向集合中第一个元素
	//@别名 常量开始 ()
	const_iterator cbegin() const
	{
		return set_.begin();
	}

	//@备注 常量版本的 end 函数，用于只读访问集合。
	//@返回 常量迭代器，指向集合结束的位置（不指向任何元素）
	//@别名 常量结束 ()
	const_iterator cend() const
	{
		return set_.end();
	}

	//@备注 常量版本的 begin 函数，用于只读访问集合。
	//@返回 常量迭代器，指向集合中第一个元素
	const_iterator constBegin() const
	{
		return set_.begin();
	}

	//@备注 常量版本的 end 函数，用于只读访问集合。
	//@返回 常量迭代器，指向集合结束的位置（不指向任何元素）
	const_iterator constEnd() const
	{
		return set_.end();
	}

	//@备注 反向版本的 begin 函数，用于只读访问集合。
	//@返回 迭代器，指向集合中第一个元素
	//@别名 反向开始 ()
	reverse_iterator rbegin() const
	{
		return set_.begin();
	}

	//@备注 反向版本的 end 函数，用于只读访问集合。
	//@返回 迭代器，指向集合结束的位置（不指向任何元素）
	//@别名 反向结束 ()
	reverse_iterator crend() const
	{
		return set_.end();
	}

	//@备注 常量反向版本的 begin 函数，用于只读访问集合。
	//@返回 迭代器，指向集合中第一个元素
	//@别名 常量反向开始()
	reverse_iterator crbegin() const
	{
		return set_.begin();
	}

	//@备注 常量反向版本的 end 函数，用于只读访问集合。
	//@返回 迭代器，指向集合结束的位置（不指向任何元素）
	//@别名 常量反向结束 ()
	reverse_iterator rend() const
	{
		return set_.end();
	}

private:
	//@隐藏{
	std::set<T> set_;  // 使用 std::set 作为底层实现，保证元素唯一且有序
	//@隐藏}
};

//@分组}

#endif // CXXSET_H_



