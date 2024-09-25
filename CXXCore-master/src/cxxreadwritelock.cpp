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

#include <cxxreadwritelock.h>
#include <stdexcept>
#include <cassert>

CXXReadWriteLock::CXXReadWriteLock(cxx::RecursionMode recursionMode)
	: numReaders_(0),
	numPendingWriters_(0),
	numWriters_(0),
	recursionMode_(recursionMode),
	writerThreadId_(0)
{
	InitializeSRWLock(&srwlock_);
	InitializeConditionVariable(&condition_);
}

CXXReadWriteLock::~CXXReadWriteLock()
{
	CXX_ASSERT_X(numReaders_.load() == 0 && numWriters_.load() == 0, L"CXXReadWriteLock: Locks held on destruction");
}

void CXXReadWriteLock::lockForRead()
{
	if (recursionMode_ == cxx::RecursionMode::NonRecursive && writerThreadId_ == GetCurrentThreadId())
	{
		CXX_ASSERT_X(false, L"CXXReadWriteLock: Recursive lock not allowed");
	}
	AcquireSRWLockShared(&srwlock_);
	numReaders_.fetch_add(1, std::memory_order_release);
}

void CXXReadWriteLock::lockForWrite()
{
	if (recursionMode_ == cxx::RecursionMode::NonRecursive && writerThreadId_ == GetCurrentThreadId())
	{
		CXX_ASSERT_X(false, L"CXXReadWriteLock: Recursive lock not allowed");
	}
	numPendingWriters_.fetch_add(1, std::memory_order_release);
	AcquireSRWLockExclusive(&srwlock_);
	numPendingWriters_.fetch_sub(1, std::memory_order_release);
	numWriters_.fetch_add(1, std::memory_order_release);
	writerThreadId_ = GetCurrentThreadId();
}

bool CXXReadWriteLock::tryLockForRead()
{
	if (recursionMode_ == cxx::RecursionMode::NonRecursive && writerThreadId_ == GetCurrentThreadId())
	{
		CXX_ASSERT_X(false, L"CXXReadWriteLock: Recursive lock not allowed");
	}
	if (TryAcquireSRWLockShared(&srwlock_))
	{
		numReaders_.fetch_add(1, std::memory_order_release);
		return true;
	}
	else
	{
		return false;
	}
}

bool CXXReadWriteLock::tryLockForRead(int timeout)
{
	if (recursionMode_ == cxx::RecursionMode::NonRecursive && writerThreadId_ == GetCurrentThreadId())
	{
		CXX_ASSERT_X(false, L"CXXReadWriteLock: Recursive lock not allowed");
	}
	DWORD dwMilliseconds = (timeout == 0) ? INFINITE : timeout;
	if (TryAcquireSRWLockShared(&srwlock_))
	{
		numReaders_.fetch_add(1, std::memory_order_release);
		return true;
	}
	else if (SleepConditionVariableSRW(&condition_, &srwlock_, dwMilliseconds, CONDITION_VARIABLE_LOCKMODE_SHARED))
	{
		AcquireSRWLockShared(&srwlock_);
		numReaders_.fetch_add(1, std::memory_order_release);
		return true;
	}
	else
	{
		return false;
	}
}

bool CXXReadWriteLock::tryLockForWrite()
{
	if (recursionMode_ == cxx::RecursionMode::NonRecursive && writerThreadId_ == GetCurrentThreadId())
	{
		CXX_ASSERT_X(false, L"CXXReadWriteLock: Recursive lock not allowed");
	}
	if (TryAcquireSRWLockExclusive(&srwlock_))
	{
		numWriters_.fetch_add(1, std::memory_order_release);
		writerThreadId_ = GetCurrentThreadId();
		return true;
	}
	else
	{
		return false;
	}
}

bool CXXReadWriteLock::tryLockForWrite(int timeout)
{
	if (recursionMode_ == cxx::RecursionMode::NonRecursive && writerThreadId_ == GetCurrentThreadId())
	{
		CXX_ASSERT_X(false, L"CXXReadWriteLock: Recursive lock not allowed");
	}
	DWORD dwMilliseconds = (timeout == 0) ? INFINITE : timeout;
	if (TryAcquireSRWLockExclusive(&srwlock_))
	{
		numWriters_.fetch_add(1, std::memory_order_release);
		writerThreadId_ = GetCurrentThreadId();
		return true;
	}
	else if (SleepConditionVariableSRW(&condition_, &srwlock_, dwMilliseconds, 0))
	{
		numWriters_.fetch_add(1, std::memory_order_release);
		AcquireSRWLockExclusive(&srwlock_);
		writerThreadId_ = GetCurrentThreadId();
		return true;
	}
	else
	{
		return false;
	}
}

void CXXReadWriteLock::unlock()
{
	if (numReaders_.load(std::memory_order_acquire) > 0)
	{
		ReleaseSRWLockShared(&srwlock_);
		numReaders_.fetch_sub(1, std::memory_order_relaxed);
	}
	else if (numWriters_.load(std::memory_order_acquire) > 0)
	{
		if (numPendingWriters_.load(std::memory_order_acquire) > 0)
		{
			numWriters_.fetch_sub(1, std::memory_order_relaxed);
			writerThreadId_ = 0;
		}
		else
		{
			numWriters_.fetch_sub(1, std::memory_order_relaxed);
			writerThreadId_ = 0;
			WakeAllConditionVariable(&condition_);
		}
	}
	else
	{
		CXX_ASSERT_X(false, L"CXXReadWriteLock: Unlock with no locks held");
	}
}