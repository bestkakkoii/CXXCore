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
#ifndef CXXSEMAPHORE_P_H_
#define CXXSEMAPHORE_P_H_

#include <cxxsemaphore.h>

class CXXSemaphorePrivate
{
public:
	explicit CXXSemaphorePrivate(CXXSemaphore* q);

	~CXXSemaphorePrivate();

private:
	HANDLE semaphoreHandle_ = CXX_NULLPTR;
	std::atomic_llong count_ = 0;

private:
	CXX_DECLARE_PUBLIC(CXXSemaphore);
	CXXSemaphore* q_ptr = CXX_NULLPTR;

};


#endif // CXXSEMAPHORE_P_H_
