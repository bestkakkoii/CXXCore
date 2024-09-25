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

#include <cxxmutexlocker.h>

CXXMutexLocker::CXXMutexLocker(CXXMutex* mutex)
	: pMutex_(mutex)
{
	if (pMutex_)
	{
		pMutex_->lock();
	}
}

CXXMutexLocker::CXXMutexLocker(CXXMutexLocker&& other) noexcept
	: pMutex_(CXX_NULLPTR)
{
	std::swap(pMutex_, other.pMutex_);
}

CXXMutexLocker::~CXXMutexLocker()
{
	unlock();
}

bool CXXMutexLocker::isLocked() const
{
	return pMutex_->isLocked();
}

CXXMutex* CXXMutexLocker::get() const
{
	return pMutex_;
}

void CXXMutexLocker::relock()
{
	unlock();
	pMutex_->lock();
}

void CXXMutexLocker::swap(CXXMutexLocker& other)
{
	std::swap(pMutex_, other.pMutex_);
}

void CXXMutexLocker::unlock()
{
	if (pMutex_ && pMutex_->isLocked())
	{
		pMutex_->unlock();
	}
	pMutex_ = CXX_NULLPTR;
}

CXXMutexLocker& CXXMutexLocker::operator=(CXXMutexLocker&& other)
{
	if (this != &other)
	{
		unlock();
		std::swap(pMutex_, other.pMutex_);
	}
	return *this;
}