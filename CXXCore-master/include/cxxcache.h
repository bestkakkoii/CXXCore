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

#ifndef CXXCACHE_H_
//@隐藏{
#define CXXCACHE_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>

#include <unordered_map>
#include <list>
#include <vector>
#include <stdexcept>
#include <shared_mutex>
#include <atomic>

//@分组{ ［容器］.炫　缓存类

//@备注 炫缓存类，用于存储对象并提供缓存管理功能。
// 当缓存中的对象数量超过最大值时，会自动移除最少使用的对象。
// 该类支持多线程的读写保护，并且允许用户设置是否启用线程安全保护。
//@别名 炫缓存
template<typename Key, typename T>
class CXXCache
{
public:
	//@备注 构造函数，初始化缓存并设置最大成本。
	//@参数 maxCost 缓存的最大成本。超过此成本时将自动移除最旧的缓存项。
	//@别名 构造 (最大成本)
	CXXCache(__int64 maxCost = 100)
		: maxCost_(maxCost), currentCost_(0), isLockEnabled_(true)
	{
	}

	//@备注 析构函数，清理缓存中所有对象，释放内存。
	virtual ~CXXCache()
	{
		clear();
	}

	//@备注 清除缓存中的所有项。
	//@别名 清空 ()
	void clear()
	{
		if (isLockEnabled())
		{
			std::unique_lock lock(mutex_);
			clearInternal();
		}
		else
		{
			clearInternal();
		}
	}

	//@备注 检查缓存中是否包含指定的键。
	//@参数 key 要检查的键
	//@返回 布尔值，表示缓存中是否包含该键。
	//@别名 包含 (键)
	bool contains(const Key& key) const
	{
		if (isLockEnabled())
		{
			std::shared_lock lock(mutex_);
			return cacheMap_.find(key) != cacheMap_.end();
		}
		return cacheMap_.find(key) != cacheMap_.end();
	}

	//@备注 返回缓存中当前存储对象的数量。
	//@返回 缓存中对象的数量。
	//@别名 大小 ()
	__int64 size() const
	{
		if (isLockEnabled())
		{
			std::shared_lock lock(mutex_);
			return cacheMap_.size();
		}
		return cacheMap_.size();
	}

	//@备注 返回缓存中当前存储对象的数量。与 大小 () 相同
	//@返回 缓存中对象的数量。
	__int64 count() const
	{
		return size();
	}

	//@备注 返回缓存中当前存储对象的数量。与 大小 () 相同
	//@返回 缓存中对象的数量。
	__int64 length() const
	{
		return size();
	}

	//@备注 插入对象到缓存中。如果缓存已满，移除最少使用的对象。
	//@参数 key 要插入的键
	//@参数 object 要插入的对象
	//@参数 cost 该对象的成本，默认为 1。如果成本超过缓存的最大成本，插入将失败。
	//@返回 布尔值，表示插入是否成功。
	//@别名 插入 (键, 对象, 成本)
	bool insert(const Key& key, T* object, __int64 cost = 1)
	{
		if (cost > maxCost_)
		{
			delete object; // 如果成本超过最大值，直接删除对象
			return false;
		}

		if (isLockEnabled())
		{
			std::unique_lock lock(mutex_);
			return insertInternal(key, object, cost);
		}
		return insertInternal(key, object, cost);
	}

	//@备注 检查缓存是否为空。
	//@返回 布尔值，表示缓存是否为空。
	//@别名 是否空 ()
	bool isEmpty() const
	{
		if (isLockEnabled())
		{
			std::shared_lock lock(mutex_);
			return cacheMap_.empty();
		}
		return cacheMap_.empty();
	}

	//@备注 检查缓存是否为空。与 是否空 () 相同
	//@返回 布尔值，表示缓存是否为空。
	bool empty() const
	{
		return isEmpty();
	}

	//@备注 返回缓存中所有键的列表。
	//@返回 包含缓存中所有键的向量。
	//@别名 取所有键 ()
	std::vector<Key> keys() const
	{
		if (isLockEnabled())
		{
			std::shared_lock lock(mutex_);
			return std::vector<Key>(cacheList_.begin(), cacheList_.end());
		}
		return std::vector<Key>(cacheList_.begin(), cacheList_.end());
	}

	//@备注 返回缓存的最大成本。
	//@返回 最大成本值。
	//@别名 取最大成本 ()
	__int64 maxCost() const
	{
		return maxCost_;
	}

	//@备注 根据键获取对象指针。如果该键存在，将其移动到最近使用位置。
	//@参数 key 要获取的键
	//@返回 键对应的对象指针，如果键不存在返回 nullptr。
	//@别名 取(键)
	T* get(const Key& key)
	{
		return object(key);
	}

	//@备注 根据键获取对象指针。如果该键存在，将其移动到最近使用位置。
	//@参数 key 要获取的键
	//@返回 键对应的对象指针，如果键不存在返回 nullptr。
	const T* get(const Key& key) const
	{
		return constObject(key);
	}

	//@备注 移除指定键的对象，并释放该对象的内存。
	//@参数 key 要移除的键
	//@返回 布尔值，表示移除是否成功。
	//@别名 移除 (键)
	bool remove(const Key& key)
	{
		if (isLockEnabled())
		{
			std::unique_lock lock(mutex_);
			return removeInternal(key);
		}
		return removeInternal(key);
	}

	//@备注 设置缓存的最大成本。
	//@参数 cost 新的最大成本值。
	//@别名 置最大成本 (最大成本)
	void setMaxCost(__int64 cost)
	{
		if (isLockEnabled())
		{
			std::unique_lock lock(mutex_);
			maxCost_ = cost;
			// 检查当前成本并移除多余的对象
			while (currentCost_ > maxCost_)
			{
				takeOldest();
			}
		}
		else
		{
			maxCost_ = cost;
			while (currentCost_ > maxCost_)
			{
				takeOldest();
			}
		}
	}

	//@备注 移除并返回指定键的对象。如果该键不存在，返回 nullptr。
	//@参数 key 要移除的键
	//@返回 被移除的对象指针，如果键不存在返回 nullptr。
	//@别名 取出对象 (键)
	T* take(const Key& key)
	{
		if (isLockEnabled())
		{
			std::unique_lock lock(mutex_);
			return takeInternal(key);
		}
		return takeInternal(key);
	}

	//@备注 返回缓存的总成本。
	//@返回 当前缓存的总成本。
	//@别名 取总成本 ()
	__int64 totalCost() const
	{
		if (isLockEnabled())
		{
			std::shared_lock lock(mutex_);
			return currentCost_;
		}
		return currentCost_;
	}


	T* operator[](const Key& key)
	{
		return object(key);
	}

	const T* operator[](const Key& key) const
	{
		return constObject(key);
	}

	//@备注 设置是否启用多线程保护（锁保护）。
	//@参数 enabled 如果为 true 则启用保护，否则禁用保护。
	//@别名 置保护启用 (启用状态)
	void setLockEnabled(bool enabled)
	{
		isLockEnabled_.store(enabled);
	}

	//@备注 获取当前是否启用锁保护状态。
	//@返回 布尔值，表示锁保护是否启用。
	//@别名 取保护状态 ()
	bool isLockEnabled() const
	{
		return isLockEnabled_.load();
	}

private:
	//@隐藏{
	T* object(const Key& key)
	{
		if (isLockEnabled())
		{
			std::unique_lock lock(mutex_);

			auto it = cacheMap_.find(key);
			if (it == cacheMap_.end())
			{
				return nullptr;
			}

			// 更新使用顺序
			cacheList_.splice(cacheList_.begin(), cacheList_, it->second.iterator);
			return it->second.object;
		}

		auto it = cacheMap_.find(key);
		if (it == cacheMap_.end())
		{
			return nullptr;
		}

		cacheList_.splice(cacheList_.begin(), cacheList_, it->second.iterator);
		return it->second.object;
	}

	const T* constObject(const Key& key) const
	{
		if (isLockEnabled())
		{
			std::unique_lock lock(mutex_);

			auto it = cacheMap_.find(key);
			if (it == cacheMap_.end())
			{
				return nullptr;
			}

			// 更新使用顺序
			cacheList_.splice(cacheList_.begin(), cacheList_, it->second.iterator);
			return it->second.object;
		}

		auto it = cacheMap_.find(key);
		if (it == cacheMap_.end())
		{
			return nullptr;
		}

		cacheList_.splice(cacheList_.begin(), cacheList_, it->second.iterator);
		return it->second.object;
	}


	// 用于存储缓存对象的内部结构体
	struct CacheItem
	{
		T* object;
		typename std::list<Key>::iterator iterator;
		__int64 cost;
	};

	// 内部工具函数，移除最旧的缓存项
	void takeOldest()
	{
		if (cacheList_.empty())
		{
			return;
		}

		const Key& oldestKey = cacheList_.back();
		remove(oldestKey);
	}

	// 插入内部实现，不带锁
	bool insertInternal(const Key& key, T* object, __int64 cost)
	{
		// 如果键已经存在，先移除旧的
		if (contains(key))
		{
			removeInternal(key);
		}

		// 进行缓存空间检查，如果总成本超出最大值，移除最少使用的对象
		while (currentCost_ + cost > maxCost_)
		{
			takeOldest();
		}

		cacheList_.push_front(key);
		cacheMap_[key] = { object, cacheList_.begin(), cost };
		currentCost_ += cost;

		return true;
	}

	// 移除内部实现，不带锁
	bool removeInternal(const Key& key)
	{
		auto it = cacheMap_.find(key);
		if (it == cacheMap_.end())
		{
			return false;
		}

		currentCost_ -= it->second.cost;
		cacheList_.erase(it->second.iterator);
		delete it->second.object;
		cacheMap_.erase(it);
		return true;
	}

	// 清除内部实现，不带锁
	void clearInternal()
	{
		for (auto& item : cacheMap_)
		{
			delete item.second.object;
		}
		cacheMap_.clear();
		cacheList_.clear();
		currentCost_ = 0;
	}

	// 取内部实现，不带锁
	T* takeInternal(const Key& key)
	{
		auto it = cacheMap_.find(key);
		if (it == cacheMap_.end())
		{
			return nullptr;
		}

		T* object = it->second.object;
		currentCost_ -= it->second.cost;
		cacheList_.erase(it->second.iterator);
		cacheMap_.erase(it);
		return object;
	}
	//@隐藏}

private:
	//@隐藏{
	__int64 maxCost_ = 100;  // 最大成本
	__int64 currentCost_ = 0;  // 当前已用成本

	mutable std::shared_mutex mutex_;  // 读写锁

	mutable std::list<Key> cacheList_;  // 存储键的使用顺序
	mutable std::unordered_map<Key, CacheItem> cacheMap_;  // 存储键值对及其迭代器和成本

	std::atomic_bool isLockEnabled_ = true;  // 是否启用锁保护的原子变量
	//@隐藏}
};

//@分组}

#endif // CXXCACHE_H_
