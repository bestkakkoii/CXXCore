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

#ifndef CXXHASH_H_
//@隐藏{
#define CXXHASH_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxvector.h>
#include <cxxstring.h>

#include <windows.h>
#include <unordered_map>
#include <shared_mutex>
#include <algorithm>
#include <atomic>
#include <map>
#include <utility>

//@分组{［容器］.炫　哈希容器类

//@备注 声明方式如: CXXHash<int, int> hash;
//@别名 炫哈希表
template<typename Key, typename T, typename Hash
	//@隐藏{
	= std::hash<Key>
	//@隐藏}
>
class CXXHash
{
public:
	//@备注 构造函数，初始化哈希表
	//@参数 useLock 是否使用锁, 默认加锁
	CXXHash(bool useLock = true)
		: useLock_(useLock)
	{
	}

	//@备注 拷贝构造函数
	//@参数 other 要复制的哈希表
	//@别名 复制构造
	CXXHash(const CXXHash<Key, T, Hash>& other)
	{
		lockForWrite();
		data_ = other.data_;
		unlockForWrite();
	}

	//@备注 初始化列表构造函数
	//@参数 other 初始化列表
	//@别名 初始化列表构造
	CXXHash(const std::initializer_list<std::pair<Key, T>>& other)
	{
		lockForWrite();
		for (const auto& it : other)
		{
			data_.emplace(it.first, it.second);
		}
		unlockForWrite();
	}

	//@隐藏{

	//STL Compatibility
	CXXHash(const std::vector<std::pair<Key, T>>& other)
	{
		lockForWrite();
		for (const auto& it : other)
		{
			data_.emplace(it.first, it.second);
		}
		unlockForWrite();
	}

	CXXHash(const std::unordered_map<Key, T, Hash>& other)
	{
		lockForWrite();
		data_ = other;
		unlockForWrite();
	}

	CXXHash(std::unordered_map<Key, T, Hash>&& other)
	{
		lockForWrite();
		data_ = std::move(other);
		unlockForWrite();
	}

	CXXHash(std::map<Key, T>&& other) noexcept
	{
		for (auto& pair : other)
		{
			data_.insert(pair);
		}
	}

	CXXHash(const std::map<Key, T>& other)
	{
		lockForWrite();
		for (const auto& it : other)
		{
			data_.emplace(it.first, it.second);
		}
		unlockForWrite();
	}



	//@隐藏}

	//@备注 移动构造函数
	//@参数 other 要移动的哈希表
	//@别名 移动构造
	CXXHash(CXXHash<Key, T, Hash>&& other)
	{
		lockForWrite();
		data_ = std::move(other.data_);
		unlockForWrite();
	}

	//@备注 析构函数
	virtual ~CXXHash()
	{
	}

	//@备注 获取哈希表中的元素数量
	//@参数 无
	//@返回 整型，元素数量
	//@别名 大小()
	int size() const
	{
		lockForRead();
		int size = static_cast<int>(data_.size());
		unlockForRead();
		return size;
	}

	//@备注 获取哈希表中的元素数量
	//@参数 无
	//@返回 整型，元素数量
	int count() const
	{
		return size();
	}

	//@备注 获取哈希表中的元素数量
	//@参数 无
	//@返回 整型，元素数量
	int length() const
	{
		return size();
	}

	//@备注 透过键值获取哈希表中的元素数值，也可以透过 hash[键值] 获取
	//@参数 key 键值
	//@返回 自订，元素数值
	//@别名 数值(键值)
	T value(const Key& key, const T& defaultValue = T()) const
	{
		lockForRead();
		auto it = data_.find(key);
		T result = (it != data_.end()) ? it->second : defaultValue;
		unlockForRead();
		return result;
	}

	//@备注 透过数值获取哈希表中的键值
	//@参数 键值
	//@返回 自订，元素数值
	//@别名 键值(数值)
	Key key(const T& value) const
	{
		lockForRead();
		auto it = std::find_if(data_.begin(), data_.end(), [value](const std::pair<Key, T>& item) { return item.second == value; });
		unlockForRead();
		if (it != data_.end())
		{
			return it->first;
		}
		return Key();
	}

	//@备注 以键值为索引，插入元素到哈希表中，也可以透过 hash[键值] = 数值 插入
	//@参数 键值
	//@返回 自订，元素数值
	//@别名 插入(键值, 数值)
	void insert(const Key& key, const T& value)
	{
		lockForWrite();
		data_.emplace(key, value);
		unlockForWrite();
	}

	//@备注 将另一个哈希表中的元素插入到当前哈希表中
	//@参数 另一个哈希表
	//@返回 无
	//@别名 插入(哈希表)
	void insert(const CXXHash<Key, T, Hash>& other)
	{
		lockForWrite();
		data_.insert(other.data_.begin(), other.data_.end());
		unlockForWrite();
	}

	//@备注 以键值为索引，移除哈希表中的元素
	//@参数 键值
	//@返回 无
	//@别名 移除(键值)
	void remove(const Key& key)
	{
		lockForWrite();
		data_.erase(key);
		unlockForWrite();
	}

	//@备注 以数值计算哈希表同等数值的元素数量
	//@参数 数值
	//@返回 整型，元素数量
	//@别名 数量(数值)
	__int64 count(const T& value) const
	{
		lockForRead();
		__int64 result = std::count_if(data_.begin(), data_.end(), [value](const std::pair<Key, T>& item) { return item.second == value; });
		unlockForRead();
		return result;
	}

	//@备注 交换两个哈希表中的所有元素
	//@参数 另一个哈希表
	//@返回 无
	//@别名 交换(哈希表)
	void swap(CXXHash<Key, T, Hash>& other)
	{
		lockForWrite();
		other.lockForWrite();
		data_.swap(other.data_);
		other.unlockForWrite();
		unlockForWrite();
	}

	//@备注 交换两个键值对应的元素数值
	//@参数 键值1
	//@参数 键值2
	//@返回 无
	//@别名 交换(键值1, 键值2)
	void swap(const Key& key1, const Key& key2)
	{
		lockForWrite();
		auto it1 = data_.find(key1);
		auto it2 = data_.find(key2);
		if (it1 != data_.end() && it2 != data_.end())
		{
			std::swap(it1->second, it2->second);
		}
		unlockForWrite();
	}

	//@备注 检查哈希表是否为空
	//@参数 无
	//@返回 布尔值，是否为空
	//@别名 是否空()
	bool isEmpty() const
	{
		lockForRead();
		bool result = data_.empty();
		unlockForRead();
		return result;
	}

	//@备注 获取哈希表中的所有数值
	//@参数 无
	//@返回 自订，数值集合
	//@别名 所有值()
	CXXVector<T> values() const
	{
		lockForRead();
		CXXVector<T> result;
		for (const auto& it : data_)
		{
			result.push_back(it.second);
		}
		unlockForRead();
		return result;
	}

	//@备注 获取哈希表中的所有键值
	//@参数 无
	//@返回 自订，键值集合
	//@别名 所有键()
	CXXVector<Key> keys() const
	{
		lockForRead();
		CXXVector<Key> result;
		for (const auto& it : data_)
		{
			result.push_back(it.first);
		}
		unlockForRead();
		return result;
	}

	//@备注 获取哈希表中的所有指定数值的键值
	//@参数 数值
	//@返回 自订，键值集合
	//@别名 值取所有键(数值)
	CXXVector<Key> keysByValue(const T& value) const
	{
		lockForRead();
		CXXVector<Key> result;
		for (const auto& it : data_)
		{
			if (it.second == value)
			{
				result.push_back(it.first);
			}
		}
		unlockForRead();
		return result;
	}

	//@备注 检查哈希表中是否包含指定键值的元素
	//@参数 键值
	//@返回 布尔值，是否包含
	//@别名 包含(键值)
	bool contains(const Key& key) const
	{
		lockForRead();
		bool result = data_.find(key) != data_.end();
		unlockForRead();
		return result;
	}

	//@备注 以键值为索引，取出哈希表中的元素，并从哈希表中移除
	//@参数 键值
	//@返回 自订，元素数值
	//@别名 取出(键值)
	T take(const Key& key)
	{
		lockForWrite();
		auto it = data_.find(key);
		if (it != data_.end())
		{
			T result = it->second;
			data_.erase(it);
			unlockForWrite();
			return result;
		}
		unlockForWrite();
		return T();
	}

	//@备注 清空哈希表中的所有元素
	//@参数 无
	//@返回 无
	//@别名 清空()
	void clear()
	{
		lockForWrite();
		data_.clear();
		unlockForWrite();
	}

	//@备注 设置是否使用锁
	//@参数 useLock 是否使用锁
	//@返回 无
	//@别名 置使用锁(是否使用锁)
	void setLockEnabled(bool useLock)
	{
		useLock_.store(useLock, std::memory_order_release);
	}

	//@隐藏{
	typedef typename std::unordered_map<Key, T, Hash>::iterator iterator;
	typedef typename std::unordered_map<Key, T, Hash>::const_iterator const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	//@隐藏}

	//@备注 获取哈希表的起始迭代器
	//@返回 哈希表的起始迭代器
	//@别名 开始()
	iterator begin()
	{
		lockForWrite();
		return data_.begin();
	}

	//@备注 获取哈希表的起始迭代器
	//@返回 哈希表的起始迭代器
	const_iterator begin() const
	{
		lockForRead();
		return data_.begin();
	}

	//@备注 获取哈希表的结束迭代器
	//@返回 哈希表的结束迭代器
	//@别名 结束()
	iterator end()
	{
		lockForWrite();
		return data_.end();
	}

	//@备注 获取哈希表的结束迭代器
	//@返回 哈希表的结束迭代器
	const_iterator end() const
	{
		lockForRead();
		return data_.end();
	}

	//@备注 获取哈希表的起始迭代器
	//@返回 哈希表的起始迭代器
	//@别名 常量开始()
	const_iterator cbegin() const
	{
		lockForRead();
		return data_.begin();
	}

	//@备注 获取哈希表的结束迭代器
	//@返回 哈希表的结束迭代器
	//@别名 常量结束()
	const_iterator cend() const
	{
		lockForRead();
		return data_.end();
	}

	//@备注 获取哈希表的起始迭代器
	//@返回 哈希表的起始迭代器
	//@别名 常量开始()
	const_iterator constBegin() const
	{
		lockForRead();
		return data_.begin();
	}

	//@备注 获取哈希表的结束迭代器
	//@返回 哈希表的结束迭代器
	//@别名 常量结束()
	const_iterator constEnd() const
	{
		lockForRead();
		return data_.end();
	}

	//@备注 获取哈希表的反向起始迭代器
	//@返回 哈希表的反向起始迭代器
	//@别名 反向开始()
	reverse_iterator rbegin()
	{
		lockForWrite();
		return std::make_reverse_iterator(data_.end());
	}

	//@备注 获取哈希表的反向结束迭代器
	//@返回 哈希表的反向结束迭代器
	//@别名 反向结束()
	reverse_iterator rend()
	{
		lockForWrite();
		return  std::make_reverse_iterator(data_.begin());
	}

	//@备注 获取哈希表的反向起始迭代器
	//@返回 哈希表的反向起始迭代器
	//@别名 常量反向开始()
	const_reverse_iterator crbegin() const
	{
		lockForRead();
		return std::make_reverse_iterator(data_.cend());
	}

	//@备注 获取哈希表的反向结束迭代器
	//@返回 哈希表的反向结束迭代器
	//@别名 常量反向结束()
	const_reverse_iterator crend() const
	{
		lockForRead();
		return std::make_reverse_iterator(data_.cbegin());
	}

	// STL Compatibility

	//@备注 检查哈希表是否为空
	bool empty() const
	{
		return isEmpty();
	}

	std::unordered_map<Key, T, Hash> toStdUnorderedMap() const
	{
		lockForWrite();
		return data_;
	}


	//@隐藏{
	bool operator!=(const CXXHash<Key, T, Hash>& other) const
	{
		lockForRead();
		other.lockForRead();
		bool result = data_ != other.data_;
		unlockForRead();
		other.unlockForRead();
		return result;
	}

	CXXHash<Key, T, Hash>& operator=(const CXXHash<Key, T, Hash>& other)
	{
		lockForWrite();
		other.lockForRead();
		data_ = other.data_;
		other.unlockForRead();
		unlockForWrite();
		return *this;
	}

	CXXHash<Key, T, Hash>& operator=(CXXHash<Key, T, Hash>&& other)
	{
		lockForWrite();
		other.lockForWrite();
		data_ = std::move(other.data_);
		other.unlockForWrite();
		unlockForWrite();
		return *this;
	}

	bool operator==(const CXXHash<Key, T, Hash>& other) const
	{
		lockForRead();
		other.lockForRead();
		bool result = data_ == other.data_;
		unlockForRead();
		other.unlockForRead();
		return result;
	}

	T& operator[](const Key& key)
	{
		lockForWrite();
		T& result = data_[key];
		unlockForWrite();
		return result;
	}

	const T operator[](const Key& key) const
	{
		lockForRead();
		const T result = data_.at(key);
		unlockForRead();
		return result;
	}
	//@隐藏}

private:
	//@隐藏{
	std::unordered_map<Key, T, Hash> data_; // 实际存储哈希表数据
	mutable std::shared_mutex mutex_;       // 读写锁保护
	std::atomic_bool useLock_;              // 是否启用锁保护

	// 加读锁
	void lockForRead() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mutex_.lock_shared();
		}
	}

	// 解读锁
	void unlockForRead() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mutex_.unlock_shared();
		}
	}

	// 加写锁
	void lockForWrite() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mutex_.lock();
		}
	}

	// 解写锁
	void unlockForWrite() const
	{
		if (useLock_.load(std::memory_order_acquire))
		{
			mutex_.unlock();
		}
	}
	//@隐藏}
};

//@隐藏{

// 重载 << 操作符，用于输出 CXXHash 到 wostream
template<typename Key, typename T, typename Hash>
std::wostream& operator<<(std::wostream& out, const CXXHash<Key, T, Hash>& hash)
{
	out << L"{ ";
	for (const auto& pair : hash)
	{
		out << L"[" << pair.first << L": " << pair.second << L"] ";
	}
	out << L"}";
	return out;
}

// 重载 >> 操作符，用于从 wistream 读取数据到 CXXHash
template<typename Key, typename T, typename Hash>
std::wistream& operator>>(std::wistream& in, CXXHash<Key, T, Hash>& hash)
{
	Key key;
	T value;
	wchar_t brace;
	in >> brace; // 读取起始的 '{'

	while (in >> key >> value)
	{
		hash.insert(key, value);
		wchar_t separator;
		in >> separator; // 读取结束符号或分隔符
		if (separator == L'}')
		{
			break;
		}
	}
	return in;
}

template <typename T>
using CXXStringHash = CXXHash<CXXString, T>;
//@隐藏}

/*@声明
//@备注 声明方式如: CXXStringHash<int> hash, 键值为:CXXString 等同 CXXHash<CXXString, int>
//@别名 炫文本哈希表
template<typename T>
class CXXStringHash : public CXXHash<CXXString, T>
{
public:
};
*/

//@分组}

#endif // CXXHASH_H_
