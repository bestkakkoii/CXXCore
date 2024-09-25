


#ifndef CXXCONTIGUOUSCACHE_H_
//@隐藏{
#define CXXCONTIGUOUSCACHE_H_
//@隐藏}

#include <deque>
#include <unordered_map>
#include <stdexcept>



//@分组{［容器］.炫　缓存类

//@备注 CXXContiguousCache 是一个连续缓存类，支持存储有限大小的键值对，并提供自动淘汰机制。类似于 QCache，具有固定容量，超过容量后将删除旧项。
//       该缓存使用 std::deque 和 std::unordered_map 结合实现，既保证连续存储，又保证高效查找。
//@别名 炫连续缓存
template<typename Key, typename Value>
class CXXContiguousCache
{
public:
	//@隐藏{
	explicit
		//@隐藏}

	//@备注 构造函数，初始化缓存，设置最大容量。
	//@参数 maxSize 缓存的最大容量
	//@别名 构造 (最大容量)
		CXXContiguousCache(__int64 maxSize)
		: maxSize_(maxSize)
	{
		if (maxSize <= 0)
		{
			throw std::invalid_argument("Cache size must be greater than 0.");
		}
	}

	//@备注 插入一个键值对到缓存中。如果键已经存在，则更新值。如果缓存超过最大容量，将移除最久未使用的项。
	//@参数 key 键
	//@参数 value 值
	//@别名 插入 (键, 值)
	void insert(const Key& key, const Value& value)
	{
		auto it = cacheMap_.find(key);
		if (it != cacheMap_.end())
		{
			// 更新已存在的项
			it->second.value = value;
			touch(it);  // 更新最近使用顺序
		}
		else
		{
			// 如果缓存已满，删除最旧的项
			if (cache_.size() >= maxSize_)
			{
				evict();
			}
			// 插入新项
			cache_.emplace_front(key, value);
			cacheMap_[key] = cache_.begin();
		}
	}

	//@备注 检查缓存中是否包含某个键。
	//@参数 key 键
	//@返回 布尔值，表示缓存是否包含该键
	//@别名 包含 (键)
	bool contains(const Key& key) const
	{
		return cacheMap_.find(key) != cacheMap_.end();
	}

	//@备注 根据键获取对应的值。如果键不存在，则返回 nullptr。
	//@参数 key 键
	//@返回 键对应的值指针，若不存在则返回 nullptr
	//@别名 值 (键)
	Value* value(const Key& key)
	{
		auto it = cacheMap_.find(key);
		if (it != cacheMap_.end())
		{
			touch(it);  // 更新最近使用顺序
			return &it->second->value;
		}
		return nullptr;
	}

	//@备注 从缓存中移除指定键的项。
	//@参数 key 要移除的键
	//@别名 移除 (键)
	void remove(const Key& key)
	{
		auto it = cacheMap_.find(key);
		if (it != cacheMap_.end())
		{
			cache_.erase(it->second);
			cacheMap_.erase(it);
		}
	}

	//@备注 返回缓存中的项数。
	//@返回 缓存中的项数
	//@别名 大小 ()
	__int64 size() const
	{
		return static_cast<__int64>(cache_.size());
	}

	//@备注 返回缓存中的项数。与 大小 () 相同
	//@返回 缓存中的项数
	__int64 count() const
	{
		return size();
	}

	//@备注 返回缓存中的项数。与 大小 () 相同
	//@返回 缓存中的项数
	__int64 length() const
	{
		return size();
	}

	//@备注 判断缓存是否为空。
	//@返回 布尔值，表示缓存是否为空
	//@别名 是否空() const
	bool isEmpty() const
	{
		return cache_.empty();
	}

	//@备注 判断缓存是否为空。与 是否空() const 相同
	//@返回 布尔值，表示缓存是否为空
	bool empty() const
	{
		return isEmpty();
	}

	//@备注 清空缓存，移除所有项。
	//@别名 清空 ()
	void clear()
	{
		cache_.clear();
		cacheMap_.clear();
	}

	//@备注 返回缓存的最大容量。
	//@返回 缓存的最大容量
	//@别名 最大容量 ()
	__int64 maxSize() const
	{
		return maxSize_;
	}

private:
	//@隐藏{
	struct CacheItem
	{
		Key key;
		Value value;
	};

	//@备注 移除最久未使用的项
	void evict()
	{
		const auto& oldest = cache_.back();
		cacheMap_.erase(oldest.key);
		cache_.pop_back();
	}

	//@备注 更新缓存的最近使用顺序
	//@参数 it 需要更新的缓存项的迭代器
	void touch(typename std::unordered_map<Key, typename std::deque<CacheItem>::iterator>::iterator it)
	{
		// 将该项移动到缓存的最前面
		cache_.splice(cache_.begin(), cache_, it->second);
	}
	//@隐藏}

private:
	//@隐藏{
	__int64 maxSize_;  // 缓存的最大容量
	std::deque<CacheItem> cache_;  // 存储缓存项，按最近使用顺序排列
	std::unordered_map<Key, typename std::deque<CacheItem>::iterator> cacheMap_;  // 快速查找键对应的缓存项
	//@隐藏}
};

//@分组}

#endif // CXXCONTIGUOUSCACHE_H_
