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
#ifndef CXXMAP_H_
//@隐藏{
#define CXXMAP_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>  // 假设 CXXString 已经定义
#include <cxxvector.h>  // 假设 CXXVector 已经定义


#include <map>
#include <algorithm>
#include <utility>
#include <unordered_map>

//@分组{［容器］.炫　映射类

//@备注 CXXMap 是一个基于 std::map 的容器类，适合存储键值对，并保持键的有序性。支持常见类型作为键值，并额外支持 CXXString、POINT、CXXPoint、CXXPointF 等作为键。
//       CXXMap 使用红黑树来管理键值对，提供高效的查找、插入和删除操作，适用于需要快速查找并保持有序键集合的场景。
//@别名 炫映射
template<typename Key, typename Value>
class CXXMap
{
public:
	//@备注 默认构造函数，创建一个空的映射表。
	//@别名 默认构造 ()
	CXXMap()
	{
	}

	//@备注 拷贝构造函数，创建一个新映射表，并将另一个 CXXMap 的内容复制到该映射表中。
	//@参数 other 被拷贝的 CXXMap 对象
	//@别名 拷贝构造 (映射)
	CXXMap(const CXXMap<Key, Value>& other)
		: map_(other.map_)
	{
	}

	//@备注 移动构造函数，创建一个新映射表，并从另一个 CXXMap 中移动内容到该映射表，避免不必要的拷贝。
	//@参数 other 被移动的 CXXMap 对象
	//@别名 移动构造 (映射)
	CXXMap(CXXMap<Key, Value>&& other) noexcept
		: map_(std::move(other.map_))
	{
	}

	//@隐藏{

	//STL Compatibility

	CXXMap(const std::map<Key, Value>& other)
		: map_(other)
	{
	}

	CXXMap(std::map<Key, Value>&& other) noexcept
		: map_(std::move(other))
	{
	}

	CXXMap(const std::unordered_map<Key, Value>& other)
	{
		for (const auto& pair : other)
		{
			map_.insert(pair);
		}
	}

	CXXMap(std::initializer_list<std::pair<Key, Value>> init)
		: map_(init)
	{
	}

	CXXMap(std::initializer_list<std::pair<const Key, Value>> init)
		: map_(init)
	{
	}

	CXXMap(std::unordered_map<Key, Value>&& other) noexcept
	{
		for (auto& pair : other)
		{
			map_.insert(std::move(pair));
		}
	}

	virtual ~CXXMap()
	{

	}

	//@隐藏}

	//@备注 插入一个键值对到映射表中。如果键已经存在，则更新对应的值。
	//@参数 key 要插入的键
	//@参数 value 要插入的值
	//@返回 布尔值，表示插入是否成功（true 表示插入新键，false 表示更新已有键的值）
	//@别名 插入 (键, 值)
	bool insert(const Key& key, const Value& value)
	{
		auto result = map_.insert_or_assign(key, value);
		return result.second;
	}

	//@备注 根据键移除映射表中的元素。
	//@参数 key 要移除的键
	//@返回 被移除的元素个数（0 表示键不存在）
	//@别名 移除 (键)
	__int64 remove(const Key& key)
	{
		return static_cast<__int64>(map_.erase(key));
	}

	//@备注 检查映射表中是否包含指定的键。
	//@参数 key 要检查的键
	//@返回 布尔值，表示键是否存在
	//@别名 包含 (键)
	bool contains(const Key& key) const
	{
		return map_.find(key) != map_.end();
	}

	//@备注 获取映射表中元素的数量。
	//@返回 映射表中键值对的数量
	//@别名 大小 ()
	__int64 size() const
	{
		return static_cast<__int64>(map_.size());
	}

	//@备注 获取映射表中元素的数量。与 大小() 相同
	//@返回 映射表中键值对的数量
	__int64 count() const
	{
		return size();
	}

	//@备注 获取映射表中元素的数量。与 大小() 相同
	//@返回 映射表中键值对的数量
	__int64 length() const
	{
		return size();
	}

	//@备注 获取指定元素在映射表中元素的数量。
	//@返回 指定元素数量
	//@别名 数量 (元素)
	__int64 count(const Value value) const
	{
		int count = std::count_if(map_.begin(), map_.end(), [value](const std::pair<Key, Value>& pair)
			{
				return pair.second == value;
			});

		return static_cast<__int64>(count);
	}

	//@备注 清空映射表，移除所有的键值对。
	//@别名 清空 ()
	void clear()
	{
		map_.clear();
	}

	//@备注 检查映射表是否为空。
	//@返回 布尔值，表示映射表是否为空（true 表示为空）
	//@别名 是否空 ()
	bool isEmpty() const
	{
		return map_.empty();
	}

	//@备注 检查映射表是否为空。
	//@返回 布尔值，表示映射表是否为空（true 表示为空）
	bool empty() const
	{
		return map_.empty();
	}

	//@备注 根据键获取值。如果键不存在，返回默认值。
	//@参数 key 要查找的键
	//@参数 defaultValue 当键不存在时返回的默认值
	//@返回 键对应的值或默认值
	//@别名 值 (键, 默认值)
	Value value(const Key& key, const Value& defaultValue = Value()) const
	{
		auto it = map_.find(key);
		if (it != map_.end())
		{
			return it->second;
		}
		return defaultValue;
	}

	//@备注 从映射表中移除指定键，并返回对应的值。如果键不存在，返回默认值。
	//@参数 key 要移除的键
	//@参数 defaultValue 当键不存在时返回的默认值
	//@返回 移除的值或默认值
	//@别名 取出 (键, 默认值)
	Value take(const Key& key, const Value& defaultValue = Value())
	{
		auto it = map_.find(key);
		if (it != map_.end())
		{
			Value value = it->second;
			map_.erase(it);
			return value;
		}
		return defaultValue;
	}

	//@备注 获取映射表中所有的键。
	//@返回 所有键的向量
	//@别名 所有键 ()
	CXXVector<Value> keys() const
	{
		CXXVector<Value> keyList;
		for (const auto& pair : map_)
		{
			keyList.push_back(pair.first);
		}
		return keyList;
	}

	//@备注 获取映射表中所有的值。
	//@返回 所有值的向量
	//@别名 所有值 ()
	CXXVector<Value> values() const
	{
		CXXVector<Value> valueList;
		for (const auto& pair : map_)
		{
			valueList.push_back(pair.second);
		}
		return valueList;
	}

	//@备注 访问或修改指定键对应的值。如果键不存在，会自动插入并初始化为默认值。
	//@参数 key 要访问的键
	//@返回 键对应的值的引用
	//@别名 访问 (键)
	Value& operator[](const Key& key)
	{
		return map_[key];
	}

	//@隐藏{
	typedef typename std::map<Key, Value>::iterator iterator;
	typedef typename std::map<Key, Value>::const_iterator const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	//@隐藏}

	//@备注 获取映射表的底层迭代器，用于遍历映射表中的键值对。
	//@返回 映射表的迭代器，指向第一个键值对
	//@别名 开始 ()
	iterator begin()
	{
		return map_.begin();
	}

	//@备注 获取映射表的底层迭代器，表示遍历结束。
	//@返回 映射表的迭代器，指向映射表结束的位置（不指向任何键值对）
	//@别名 结束 ()
	iterator end()
	{
		return map_.end();
	}

	//@备注 常量版本的 begin 函数，用于只读访问映射表。
	//@返回 常量迭代器，指向第一个键值对
	const_iterator begin() const
	{
		return map_.begin();
	}

	//@备注 常量版本的 end 函数，用于只读访问映射表。
	//@返回 常量迭代器，指向映射表结束的位置（不指向任何键值对）
	const_iterator end() const
	{
		return map_.end();
	}

	//@备注 常量版本的 begin 函数，用于只读访问映射表。
	//@返回 常量迭代器，指向第一个键值对
	//@别名 常量开始 ()
	const_iterator cbegin() const
	{
		return map_.begin();
	}

	//@备注 常量版本的 end 函数，用于只读访问映射表。
	//@返回 常量迭代器，指向映射表结束的位置（不指向任何键值对）
	//@别名 常量结束 ()
	const_iterator cend() const
	{
		return map_.cend();
	}

	//@备注 常量版本的 begin 函数，用于只读访问映射表。
	//@返回 常量迭代器，指向第一个键值对
	const_iterator constBegin() const
	{
		return map_.begin();
	}

	//@备注 常量版本的 end 函数，用于只读访问映射表。
	//@返回 常量迭代器，指向映射表结束的位置（不指向任何键值对）
	const_iterator constEnd() const
	{
		return map_.cend();
	}

	//@备注 获取映射表的底层迭代器，用于逆向遍历映射表中的键值对。
	//@返回 映射表的逆向迭代器，指向最后一个键值对
	//@别名 反向开始 ()
	reverse_iterator rbegin()
	{
		return map_.rbegin();
	}

	//@备注 获取映射表的底层迭代器，表示逆向遍历结束。
	//@返回 映射表的逆向迭代器，指向映射表结束的位置（不指向任何键值对）
	//@别名 反向结束 ()
	reverse_iterator rend()
	{
		return map_.rend();
	}

	//@备注 常量版本的 begin 函数，用于只读逆向遍历映射表。
	//@返回 常量逆向迭代器，指向最后一个键值对
	//@别名 常量反向开始 ()
	const_reverse_iterator crbegin() const
	{
		return map_.crbegin();
	}

	//@备注 常量版本的 end 函数，用于只读逆向遍历映射表。
	//@返回 常量逆向迭代器，指向映射表结束的位置（不指向任何键值对）
	//@别名 常量反向结束 ()
	const_reverse_iterator crend() const
	{
		return map_.crend();
	}

private:
	//@隐藏{
	std::map<Key, Value> map_;  // 使用 std::map 作为底层实现，保证键值对有序
	//@隐藏}
};

// 特殊化声明，支持 CXXString, POINT, CXXPoint 和 CXXPointF 作为 Key
//@备注 为 CXXMap 特殊化，支持 CXXString 作为键
//@别名 炫文本映射 (炫字符串, 值)
template<typename Value>
using CXXMapCXXString = CXXMap<CXXString, Value>;

#ifdef CXXPOINT_H_
//@备注 为 CXXMap 特殊化，支持 CXXPoint 作为键
//@别名 炫坐标映射 (炫点, 值)
template<typename Value>
using CXXMapCXXPoint = CXXMap<CXXPoint, Value>;

//@备注 为 CXXMap 特殊化，支持 CXXPointF 作为键
//@别名 炫双浮点坐标映射 (炫浮点, 值)
template<typename Value>
using CXXMapCXXPointF = CXXMap<CXXPointF, Value>;
#endif


//@隐藏{
// 重载 POINT 的 < 运算符，用于 std::map 键的比较
inline bool operator<(const POINT& p1, const POINT& p2)
{
	if (p1.x != p2.x)
	{
		return p1.x < p2.x;
	}
	return p1.y < p2.y;
}

template <typename Key, typename T, typename Predicate>
std::size_t erase_if(CXXMap<Key, T>& map, Predicate pred)
{
	std::size_t erasedCount = 0;
	for (auto it = map.begin(); it != map.end();)
	{
		if (pred(*it))
		{
			it = map.erase(it); // 移除符合条件的元素
			++erasedCount;
		}
		else
		{
			++it;
		}
	}
	return erasedCount;
}

// 重载操作符 !=
template <typename Key, typename T>
bool operator!=(const CXXMap<Key, T>& lhs, const CXXMap<Key, T>& rhs)
{
	return !(lhs == rhs);
}

// 重载操作符 ==
template <typename Key, typename T>
bool operator==(const CXXMap<Key, T>& lhs, const CXXMap<Key, T>& rhs)
{
	return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

// 流运算符 << 用于序列化 CXXMap
template <typename Key, typename T>
std::wostream& operator<<(std::wostream& out, const CXXMap<Key, T>& map)
{
	out << L"{ ";
	for (const auto& pair : map)
	{
		out << L"(" << pair.first << L", " << pair.second << L") ";
	}
	out << L"}";
	return out;
}

// 宽字符流运算符 >> 用于反序列化 CXXMap
template <typename Key, typename T>
std::wistream& operator>>(std::wistream& in, CXXMap<Key, T>& map)
{
	Key key;
	T value;
	while (in >> key >> value)
	{
		map[key] = value;
	}
	return in;
}
//@隐藏}

//@分组}

#endif // CXXMAP_H_



