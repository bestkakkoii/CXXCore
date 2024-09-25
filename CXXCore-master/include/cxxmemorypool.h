/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

//bestkakkoii modified on 2022/11/08

#ifndef CXXMEMORYPOOL_H_
//@隐藏{
#define CXXMEMORYPOOL_H_
//@隐藏}



#include <cxxglobal.h>

#include <cstdint>
#include <mutex>
#include <assert.h>

/// This is a small object allocator used for allocating small
/// objects that persist for more than one time step.
/// See: http://www.codeproject.com/useritems/Small_Block_Allocator.asp

//@别名 炫
namespace cxx
{
	//@分组{［内存安全］.炫　內存池类

	//@隐藏{
#if _WIN64
#pragma pack(push, 8)
	constexpr size_t kChunkSize = 16i64 * 1024i64;    // default: 16kb (16 * 1024)
	constexpr size_t kMaxBlockSize = 640i64;          // default: 640
	constexpr size_t kBlockSizes = 60i64;             // default: 14
	constexpr size_t kChunkArrayIncrement = 128i64;   // default: 128
#pragma pack(pop)
#else
#pragma pack(push, 4)
	constexpr size_t kChunkSize = 16 * 1024;     // default: 16kb (16 * 1024)
	constexpr size_t kMaxBlockSize = 61440;      // default: 640
	constexpr size_t kBlockSizes = 60;           // default: 14
	constexpr size_t kChunkArrayIncrement = 128; // default: 128
#pragma pack(pop)
#endif
	//@隐藏}

	//@分组}
}

//@分组{［内存安全］.炫　內存池类

//@隐藏{
#if _WIN64
#pragma pack(push, 8)
#else
#pragma pack(push, 4)
#endif
//@隐藏}

//@备注 内存池的
//优点 1.减少内存碎片 2.提高内存分配效率 3.提高内存分配效率
//缺点 1.内存池的大小是固定的
//@别名 炫内存池
class CXXMemoryPool
{
	//@隐藏{
	CXX_DISABLE_COPY_MOVE(CXXMemoryPool);
	//@隐藏}
public:
	//@隐藏{
	virtual ~CXXMemoryPool();
	//@隐藏}

public:
	//@备注 申请内存
	//@参数 size 申请内存大小(64位系统下size_t为__int64, 32位系统下size_t为int) 
	//@返回 申请到的内存指针
	//@别名 申请(大小)
	void* alloc(size_t size);

	//@备注 释放内存
	//@参数 p 申请到的内存指针
	//@参数 size 申请内存大小(64位系统下size_t为__int64, 32位系统下size_t为int)
	//@返回 无
	//@别名 释放(指针, 大小)
	void free(void* p, size_t size);

	//@备注 清空内存池
	//@参数 无
	//@返回 无
	//@别名 清空()
	void clear();

	//@备注 设置内存池是否加锁
	//@参数 enabled 是否加锁 TRUE:加锁 FALSE:不加锁 默認不加锁
	//@返回 无
	//@别名 置锁(是否加锁)
	inline void setLockEnabled(bool enabled) { mutex_.lock(); lock_enabled_ = enabled; mutex_.unlock(); }

private:
	//@隐藏{
	CXXMemoryPool();

	typedef struct tagBlock
	{
		tagBlock* next = CXX_NULLPTR;
	}Block;

	typedef struct tagChunk
	{
		size_t block_size = 0;
		Block* blocks = CXX_NULLPTR;
	}Chunk;

	size_t            num_chunk_count_ = 0;
	size_t            num_chunk_space_ = 0;
	Chunk* chunks_ = CXX_NULLPTR;
	Block* free_lists_[cxx::kBlockSizes] = { CXX_NULLPTR };

	BYTE			    s_block_size_lookup_[cxx::kMaxBlockSize + 1] = {};
	bool              s_block_size_lookup_initialized_ = false;
	bool              lock_enabled_ = false;
	std::mutex          mutex_;
	//@隐藏
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
	CXXScopedMemoryPool(size_t size)
		: p_(CXXScopedMemoryPool::getInstance().alloc(size)), size_(size)
	{
		assert(p_ != CXX_NULLPTR);
	}
	~CXXScopedMemoryPool() { CXXScopedMemoryPool::getInstance().free(p_, size_); }

	//@备注 获取已申请到的内存
	//@参数 无
	//@返回 指定类型的内存指针
	//@别名 取()
	inline T* get() { return reinterpret_cast<T*>(p_); }

	//@备注 获取已申请到的内存
	//@参数 无
	//@返回 指定类型的内存指针
	//@别名 地址()
	inline T* data() { return reinterpret_cast<T*>(p_); }

	//@别名 到指针()
	inline void* toVoidStar() { return p_; }

	//@别名 大小()
	inline __int64 size() { return static_cast<__int64>(size_); }

	inline __int64 count() { return size(); }

	inline __int64 length() { return size(); }

	//@别名 到整型()
	inline int toInt() { return reinterpret_cast<int>(p_); }

	//@别名 到正整型()
	inline UINT toUInt() { return reinterpret_cast<unsigned int>(p_); }

	//@别名 到长整型()
	inline __int64 toInt64() { return reinterpret_cast<__int64>(p_); }

	//@别名 到正长整型()
	inline UINT64 toUInt64() { return reinterpret_cast<unsigned __int64>(p_); }

private:
	//@隐藏{
	void* p_ = CXX_NULLPTR;
	size_t size_ = 0;
	//@隐藏 
};
//@分组}

//@隐藏{
#pragma pack(pop)
//@隐藏}

#endif // CXXMEMORYPOOL_H_
