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
#include <pch.h>

#include <cxxsemaphore_p.h>

#include <stdexcept>
#include <cassert>

CXXSemaphorePrivate::CXXSemaphorePrivate(CXXSemaphore* q)
	: q_ptr(q)
{
}

CXXSemaphorePrivate::~CXXSemaphorePrivate()
{
}

CXXSemaphore::CXXSemaphore(__int64 n, const CXXString& name)
	: d_ptr(new CXXSemaphorePrivate(this))
{
	CXX_D(CXXSemaphore);

	d->semaphoreHandle_ = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, name.c_str());

	if (d->semaphoreHandle_ == CXX_NULLPTR)
	{
		d->semaphoreHandle_ = CreateSemaphoreW(CXX_NULLPTR, static_cast<LONG>(n), MAXLONG, name.c_str());
		if (d->semaphoreHandle_ == CXX_NULLPTR)
		{
			CXX_ASSERT_X(false, L"CreateSemaphoreW failed");
		}
	}

	d->count_.store(n, std::memory_order_release);
}

CXXSemaphore::~CXXSemaphore()
{
	CXX_D(CXXSemaphore);

	if (d->semaphoreHandle_ != CXX_NULLPTR)
	{
		MINT::NtClose(d->semaphoreHandle_);
	}
}

void CXXSemaphore::acquire(__int64 n)
{
	CXX_D(CXXSemaphore);

	for (int i = 0; i < n; ++i)
	{
		if (d->count_.load(std::memory_order_acquire) <= 0)
		{
			WaitForSingleObject(d->semaphoreHandle_, INFINITE);
			d->count_.fetch_sub(1, std::memory_order_release);
		}
	}
}

__int64 CXXSemaphore::available() const
{
	CXX_D(const CXXSemaphore);

	return d->count_.load(std::memory_order_acquire);
}

bool CXXSemaphore::release(__int64 n)
{
	CXX_D(CXXSemaphore);

	LONG prevCount = 0;
	BOOL ret = ReleaseSemaphore(d->semaphoreHandle_, static_cast<LONG>(n), &prevCount);
	if (ret == TRUE)
	{
		d->count_.store(prevCount + n, std::memory_order_release);
	}

	return ret == TRUE;
}

bool CXXSemaphore::tryAcquire(__int64 n)
{
	return tryAcquire(n, 0);
}

bool CXXSemaphore::tryAcquire(__int64 n, __int64 timeout)
{
	CXX_D(CXXSemaphore);

	using namespace MINT;

	DWORD dwWaitResult = WaitForSingleObject(d->semaphoreHandle_, static_cast<DWORD>(std::abs(timeout)));
	if (dwWaitResult != WAIT_OBJECT_0)
	{
		return false;
	}

	d->count_.fetch_add(1, std::memory_order_release);
	release(1);
	acquire(n);
	return true;
}