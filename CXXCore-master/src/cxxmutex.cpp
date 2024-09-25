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

#include <cxxmutex.h>
#include <chrono>

CXXMutex::CXXMutex()
	: locked_(false)
{
}

CXXMutex::~CXXMutex()
{

}

void CXXMutex::lock()
{
	if (locked_.load(std::memory_order_acquire))
	{
		return;
	}

	locked_.store(true, std::memory_order_release);
	mutex_.lock();
}

void CXXMutex::unlock()
{
	if (!locked_.load(std::memory_order_acquire))
	{
		return;
	}

#pragma warning(push)
#pragma warning(disable : 26110)
	mutex_.unlock();
	locked_.store(false, std::memory_order_release);
#pragma warning(pop)

}

bool CXXMutex::tryLock()
{
	bool locked = mutex_.try_lock();
	if (locked)
	{
		locked_.store(true, std::memory_order_release);
	}

	return locked;
}

bool CXXMutex::tryLockFor(UINT duration)
{
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point end = start + std::chrono::milliseconds(duration);
	bool locked = false;
	do
	{
		locked = tryLock();
		if (locked)
		{
			locked_.store(true, std::memory_order_release);
			break;
		}

		std::this_thread::yield();
	} while (std::chrono::system_clock::now() < end);
	return locked;
}

bool CXXMutex::isLocked() const
{
	return locked_.load(std::memory_order_acquire);
}