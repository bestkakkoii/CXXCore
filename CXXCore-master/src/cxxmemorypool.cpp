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
#include <pch.h>

#include <cxxmemorypool.h>
#include <limits.h>
#include <memory.h>
#include <stddef.h>
#include <malloc.h>
#include <assert.h>

#if _WIN64
#pragma pack(push, 8)
#else
#pragma pack(push, 4)
#endif

namespace cxx
{
	namespace
	{
		constexpr size_t block_sizes_[cxx::kBlockSizes] = {
		16,     // 0
		32,     // 1
		64,     // 2
		96,     // 3
		128,    // 4
		160,    // 5
		192,    // 6
		224,    // 7
		256,    // 8
		320,    // 9
		384,    // 10
		448,    // 11
		512,    // 12
		640,    // 13
		768,    // 14
		896,    // 15
		1024,   // 16
		1280,   // 17
		1536,   // 18
		1792,   // 19
		2048,   // 20
		2304,   // 21
		2560,   // 22
		2816,   // 23
		3072,   // 24
		3328,   // 25
		3584,   // 26
		3840,   // 27
		4096,   // 28
		4608,   // 29
		5120,   // 30
		5632,   // 31
		6144,   // 32
		6656,   // 33
		7168,   // 34
		7680,   // 35
		8192,   // 36
		9216,   // 37
		10240,  // 38
		11264,  // 39
		12288,  // 40
		13312,  // 41
		14336,  // 42
		15360,  // 43
		16384,  // 44
		18432,  // 45
		20480,  // 46
		22528,  // 47
		24576,  // 48
		26624,  // 49
		28672,  // 50
		30720,  // 51
		32768,  // 52
		36864,  // 53
		40960,  // 54
		45056,  // 55
		49152,  // 56
		53248,  // 57
		57344,  // 58
		61440,  // 59
		};
	}
}

#pragma pack(pop)

CXXMemoryPool::CXXMemoryPool()
{
	assert(cxx::kBlockSizes < UCHAR_MAX);

	mutex_.lock();
	lock_enabled_ = false;
	mutex_.unlock();

	num_chunk_space_ = cxx::kChunkArrayIncrement;
	num_chunk_count_ = 0;
	chunks_ = reinterpret_cast<Chunk*>(malloc(num_chunk_space_ * sizeof(Chunk)));
	if (!chunks_)
	{
		assert(false);
		return;
	}

	memset(chunks_, 0, num_chunk_space_ * sizeof(Chunk));
	memset(free_lists_, 0, sizeof(free_lists_));

	if (s_block_size_lookup_initialized_ == false)
	{
		size_t j(0);
		for (size_t i = 1; i <= cxx::kMaxBlockSize; ++i)
		{
			assert(j < cxx::kBlockSizes);
			if (i <= cxx::block_sizes_[j])
			{
				s_block_size_lookup_[i] = static_cast<BYTE>(j);
			}
			else
			{
				++j;
				s_block_size_lookup_[i] = static_cast<BYTE>(j);
			}
		}
		s_block_size_lookup_initialized_ = true;
	}
}

void* CXXMemoryPool::alloc(size_t size)
{
	if (lock_enabled_) mutex_.lock();

	if (size == 0)
	{
		if (lock_enabled_) mutex_.unlock();
		return CXX_NULLPTR;
	}

	assert(0 < size);

	if (size > cxx::kMaxBlockSize)
	{
		if (lock_enabled_) mutex_.unlock();
		return malloc(size);
	}

	size_t index(s_block_size_lookup_[size]);
	assert((0 <= index) && (index < cxx::kBlockSizes));

	if (free_lists_[index])
	{
		Block* block(free_lists_[index]);
		free_lists_[index] = block->next;
		if (lock_enabled_) mutex_.unlock();
		return block;
	}
	else
	{
		if (num_chunk_count_ == num_chunk_space_)
		{
			Chunk* oldChunks(chunks_);
			if (oldChunks)
			{
				num_chunk_space_ += cxx::kChunkArrayIncrement;
				chunks_ = (Chunk*)malloc(num_chunk_space_ * sizeof(Chunk));
				if (chunks_)
				{
					memmove_s(chunks_, num_chunk_space_ * sizeof(Chunk), oldChunks, num_chunk_count_ * sizeof(Chunk));
					memset(chunks_ + num_chunk_count_, 0, cxx::kChunkArrayIncrement * sizeof(Chunk));
				}
				::free(oldChunks);
			}
		}

		Chunk* chunk(chunks_ + num_chunk_count_);
		if (chunk)
			chunk->blocks = reinterpret_cast<Block*>(malloc(cxx::kChunkSize));
#if defined(_DEBUG)
		if (chunk && chunk->blocks)
			memset(chunk->blocks, 0xcd, cxx::kChunkSize);
#endif
		size_t block_size(cxx::block_sizes_[index]);
		if (chunk)
			chunk->block_size = block_size;
		size_t block_count(cxx::kChunkSize / block_size);

		assert((block_count * block_size) <= cxx::kChunkSize);

		for (size_t i = 0; i < block_count - 1; ++i)
		{
			if (!chunk) break;
			Block* block(reinterpret_cast<Block*>(reinterpret_cast<BYTE*>(chunk->blocks) + (block_size * i)));
			Block* next(reinterpret_cast<Block*>(reinterpret_cast<BYTE*>(chunk->blocks) + (block_size * (i + 1))));
			if (block)
				block->next = next;
		}
		if (chunk)
		{
			Block* last(reinterpret_cast<Block*>(reinterpret_cast<BYTE*>(chunk->blocks) + (block_size * (block_count - 1))));
			if (last)
			{
				last->next = CXX_NULLPTR;

				if (chunk->blocks)
				{
					free_lists_[index] = chunk->blocks->next;
					++num_chunk_count_;
					if (lock_enabled_) mutex_.unlock();
					return chunk->blocks;
				}
			}
		}
		if (lock_enabled_) mutex_.unlock();
		return CXX_NULLPTR;
	}
}

CXXMemoryPool::~CXXMemoryPool()
{
	if (chunks_)
	{
		for (size_t i = 0; i < num_chunk_count_; ++i)
		{
			if (chunks_[i].blocks)
			{
				::free(chunks_[i].blocks);
			}
		}

		::free(chunks_);
	}
}

void CXXMemoryPool::free(void* p, size_t size)
{
	if (lock_enabled_) mutex_.lock();
	if ((0 == size) || (CXX_NULLPTR == p))
	{
		if (lock_enabled_) mutex_.unlock();
		return;
	}

	assert(0 < size);

	if (size > cxx::kMaxBlockSize)
	{
		::free(p);
		if (lock_enabled_) mutex_.unlock();
		return;
	}

	size_t index(s_block_size_lookup_[size]);
	assert((0 <= index) && (index < cxx::kBlockSizes));

#ifdef _DEBUG
	size_t block_size(cxx::block_sizes_[index]);
	bool found(false);
	for (size_t i = 0; i < num_chunk_count_; ++i)
	{
		Chunk* chunk(chunks_ + i);
		if (chunk->block_size != block_size)
		{
			assert(((reinterpret_cast<BYTE*>(p) + block_size) <= reinterpret_cast<BYTE*>(chunk->blocks)) ||
				((reinterpret_cast<BYTE*>(chunk->blocks) + cxx::kChunkSize) <= reinterpret_cast<BYTE*>(p)));
		}
		else
		{
			if ((reinterpret_cast<BYTE*>(chunk->blocks) <= reinterpret_cast<BYTE*>(p))
				&& (reinterpret_cast<BYTE*>(p) + block_size <= (reinterpret_cast<BYTE*>(chunk->blocks) + cxx::kChunkSize)))
			{
				found = true;
			}
		}
	}

	assert(found);

	memset(p, 0xfd, block_size);
#endif

	Block* block(reinterpret_cast<Block*>(p));
	block->next = free_lists_[index];
	free_lists_[index] = block;
	if (lock_enabled_) mutex_.unlock();
}

void CXXMemoryPool::clear()
{
	if (lock_enabled_) mutex_.lock();
	for (size_t i = 0; i < num_chunk_count_; ++i)
	{
		::free(chunks_[i].blocks);
	}

	num_chunk_count_ = 0;
	memset(chunks_, 0, num_chunk_space_ * sizeof(Chunk));
	memset(free_lists_, 0, sizeof(free_lists_));
	if (lock_enabled_) mutex_.unlock();
}