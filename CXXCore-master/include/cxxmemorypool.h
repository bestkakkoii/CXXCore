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
#ifndef CXXMEMORYPOOL_H_
//@隐藏{
#define CXXMEMORYPOOL_H_
//@隐藏}

#include <cxxglobal.h>
#include <cstdint>
#include <memory_resource>
#include <mutex>
#include <assert.h>

//@别名 炫
namespace cxx
{
	//@隐藏{
	constexpr __int64 MemoryPoolDefaultPoolSize = 1024 * 1024; // 1MB default pool size
	//@隐藏}
}

//@分组{［内存安全］.炫　內存池类

//@备注 内存池的
//优点 1.减少内存碎片 2.提高内存分配效率 3.提高内存分配效率 4.类型安全
//缺点 1.初始内存池的大小是固定的，但可以通过参数调整，且可以根据需要增长
// 使用场景 1.频繁申请和释放内存 2.需要高效的内存分配和释放 3.需要类型安全的内存分配和释放
// 使用方法 1.创建内存池对象 2.申请内存 3.使用内存 4.释放内存 5.清空内存池
// CXXMemoryPool memoryPool(1024); // 创建内存池对象
// int* p = memoryPool.alloc(100); // 申请内存
// memoryPool.free(p, 100); // 释放内存
// memoryPool.clear(); // 清空内存池
//@别名 炫内存池
template<typename T>
class CXXMemoryPool
{
public:
	CXXMemoryPool(__int64 poolSize = cxx::MemoryPoolDefaultPoolSize);
	virtual ~CXXMemoryPool();

	//@备注 申请内存
	//@参数 count 申请对象的数量
	//@返回 申请到的内存指针
	//@别名 申请(数量)
	T* alloc(__int64 count = 1);

	//@备注 释放内存
	//@参数 p 申请到的内存指针
	//@参数 count 申请对象的数量
	//@返回 无
	//@别名 释放(指针, 数量)
	void free(T* p, __int64 count = 1);

	//@备注 清空内存池
	//@参数 无
	//@返回 无
	//@别名 清空()
	void clear();

private:
	//@隐藏{
	CXX_DISABLE_COPY_MOVE(CXXMemoryPool);

	std::unique_ptr<std::pmr::synchronized_pool_resource> pool_resource_;
	std::pmr::memory_resource* upstream_resource_;
	//@隐藏}
};

//@备注 使用 CXXScopedMemoryPool 内存池申请指定大小內存并自动在离开作用域后释放
// 使用方法:
// CXXScopedMemoryPool<int> scopedAlloc(100);
// int* p = scopedAlloc.get();
//@别名 炫作用域內存池
template <typename T>
class CXXScopedMemoryPool
{
public:
	CXXScopedMemoryPool(__int64 count)
		: p_(CXXMemoryPool<T>::getInstance().alloc(count)), count_(count)
	{
		assert(p_ != nullptr);
	}
	virtual ~CXXScopedMemoryPool() { CXXMemoryPool<T>::getInstance().free(p_, count_); }

	//@备注 获取已申请到的内存
	//@参数 无
	//@返回 指定类型的内存指针
	//@别名 取()
	inline T* get() { return p_; }

	//@备注 获取已申请到的内存
	//@参数 无
	//@返回 指定类型的内存指针
	//@别名 地址()
	inline T* data() { return p_; }

	//@别名 到指针()
	inline void* toVoidStar() { return static_cast<void*>(p_); }

	//@别名 大小()
	inline int64_t size() { return static_cast<int64_t>(count_); }

	inline int64_t count() { return size(); }

	inline int64_t length() { return size(); }

	//@别名 到整型()
	inline int toInt() { return reinterpret_cast<int>(p_); }

	//@别名 到正整型()
	inline unsigned int toUInt() { return reinterpret_cast<unsigned int>(p_); }

	//@别名 到长整型()
	inline int64_t toInt64() { return reinterpret_cast<int64_t>(p_); }

	//@别名 到正长整型()
	inline uint64_t toUInt64() { return reinterpret_cast<uint64_t>(p_); }

private:
	T* p_ = nullptr;
	__int64 count_ = 0;
};

//@隐藏{
template<typename T>
CXXMemoryPool<T>::CXXMemoryPool(__int64 poolSize)
{
	upstream_resource_ = std::pmr::new_delete_resource();
	pool_resource_ = std::make_unique<std::pmr::synchronized_pool_resource>(
		std::pmr::pool_options{ 0, std::numeric_limits<size_t>::max() },
		upstream_resource_
	);
}

template<typename T>
CXXMemoryPool<T>::~CXXMemoryPool() = default;

template<typename T>
T* CXXMemoryPool<T>::alloc(__int64 count)
{
	if (count <= 0)
	{
		return nullptr;
	}

	return static_cast<T*>(pool_resource_->allocate(static_cast<size_t>(count) * sizeof(T), alignof(T)));
}

template<typename T>
void CXXMemoryPool<T>::free(T* p, __int64 count)
{
	if (p == nullptr || count <= 0)
	{
		return;
	}

	pool_resource_->deallocate(p, static_cast<size_t>(count) * sizeof(T), alignof(T));
}

template<typename T>
void CXXMemoryPool<T>::clear()
{
	// Recreate the pool to achieve a similar effect to clearing.
	pool_resource_ = std::make_unique<std::pmr::synchronized_pool_resource>(
		std::pmr::pool_options{ 0, std::numeric_limits<size_t>::max() },
		upstream_resource_
	);
}
//@隐藏}

//@分组}

#endif // CXXMEMORYPOOL_H_