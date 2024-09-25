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

#include <cxxsharedmemory.h>

CXXSharedMemory::CXXSharedMemory(const std::string& key, cxx::SharedMemoryVersion version)
	: key_(key)
	, memory_(nullptr)
	, size_(0)
	, attached_(false)
	, error_(cxx::SharedMemoryNoError)
	, version_(version)
{
}

CXXSharedMemory::~CXXSharedMemory()
{
	detach();
}

bool CXXSharedMemory::create(__int64 size, cxx::SharedMemoryAccessMode mode)
{
	std::unique_lock lock(mutex_);

	if (attached_)
	{
		setError(cxx::SharedMemoryAlreadyExists);
		return false; // 防止重复创建
	}

	DWORD protection = (mode == cxx::SharedMemoryReadWrite) ? PAGE_READWRITE : PAGE_READONLY;

	// 使用不同的API函数基于版本选择
	if (version_ == cxx::SharedMemoryANSI)
	{
		memoryHandle_ = CreateFileMappingA(
			INVALID_HANDLE_VALUE, nullptr, protection, 0, static_cast<DWORD>(size), key_.c_str());
	}
	else
	{
		std::wstring wkey(key_.begin(), key_.end());
		memoryHandle_ = CreateFileMappingW(
			INVALID_HANDLE_VALUE, nullptr, protection, 0, static_cast<DWORD>(size), wkey.c_str());
	}

	if (memoryHandle_ == nullptr)
	{
		setError(cxx::SharedMemoryAlreadyExists);
		return false;
	}

	size_ = size;
	return attach(mode);
}

bool CXXSharedMemory::attach(cxx::SharedMemoryAccessMode mode)
{
	std::unique_lock lock(mutex_);

	DWORD access = (mode == cxx::SharedMemoryReadWrite) ? FILE_MAP_WRITE : FILE_MAP_READ;
	memory_ = MapViewOfFile(memoryHandle_, access, 0, 0, static_cast<size_t>(size_));
	if (memory_ == nullptr)
	{
		setError(cxx::SharedMemoryKeyError);
		return false;
	}

	attached_ = true;
	return true;
}

bool CXXSharedMemory::detach()
{
	std::unique_lock lock(mutex_);
	if (memory_ != nullptr)
	{
		UnmapViewOfFile(memory_);
		MINT::NtClose(memoryHandle_);
		memory_ = nullptr;
		attached_ = false;
		return true;
	}
	return false;
}

void* CXXSharedMemory::data() const
{
	std::shared_lock lock(mutex_);
	return memory_;
}

void* CXXSharedMemory::get() const
{
	std::shared_lock lock(mutex_);
	return memory_;
}

__int64 CXXSharedMemory::size() const
{
	std::shared_lock lock(mutex_);
	return size_;
}

__int64 CXXSharedMemory::count() const
{
	return size();
}

__int64 CXXSharedMemory::length() const
{
	return size();
}

bool CXXSharedMemory::write(const void* src, __int64 len)
{
	std::unique_lock lock(mutex_);
	if (len > size_ || memory_ == nullptr)
	{
		setError(cxx::SharedMemoryInvalidSize);
		return false;
	}
	if (memmove_s(memory_, static_cast<size_t>(size_), src, static_cast<size_t>(len)) != 0)
	{
		setError(cxx::SharedMemoryUnknownError);
		return false;
	}
	return true;
}

bool CXXSharedMemory::read(void* dest, __int64 len)
{
	std::shared_lock lock(mutex_);
	if (len > size_ || memory_ == nullptr)
	{
		setError(cxx::SharedMemoryInvalidSize);
		return false;
	}
	if (memmove_s(dest, static_cast<size_t>(len), memory_, static_cast<size_t>(len)) != 0)
	{
		setError(cxx::SharedMemoryUnknownError);
		return false;
	}
	return true;
}

cxx::SharedMemoryError CXXSharedMemory::error() const
{
	std::shared_lock lock(mutex_);
	return error_;
}

std::string CXXSharedMemory::key() const
{
	std::shared_lock lock(mutex_);
	return key_;
}

std::string CXXSharedMemory::errorString() const
{
	std::shared_lock lock(mutex_);
	switch (error_) {
	case cxx::SharedMemoryNoError: return "No error";
	case cxx::SharedMemoryPermissionDenied: return "Permission denied";
	case cxx::SharedMemoryInvalidSize: return "Invalid size";
	case cxx::SharedMemoryKeyError: return "Key error";
	case cxx::SharedMemoryAlreadyExists: return "Shared memory already exists";
	case cxx::SharedMemoryNotFound: return "Shared memory not found";
	case cxx::SharedMemoryUnknownError: return "Unknown error";
	default: return "Unknown error";
	}
}

bool CXXSharedMemory::isAttached() const
{
	std::shared_lock lock(mutex_);
	return attached_;
}

void CXXSharedMemory::setError(cxx::SharedMemoryError error)
{
	error_ = error;
}
