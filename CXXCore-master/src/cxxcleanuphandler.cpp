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

#include <cxxcleanuphandler.h>

cxx::CleanupItem::CleanupItem(void* ptr, cxx::CleanupType type)
	: ptr(ptr)
	, type(type)
{
}

// 析构函数，根据类型释放指针
cxx::CleanupItem::~CleanupItem()
{
	switch (type)
	{
	case cxx::CleanupType::Single:
	{
		delete static_cast<char*>(ptr);  // 使用 char* 来兼容不同的类型
		break;
	}
	case cxx::CleanupType::Array:
	{
		delete[] static_cast<char*>(ptr);
		break;
	}
	case cxx::CleanupType::Malloc:
	{
		free(ptr);
		break;
	}
	}
}

CXXCleanupHandler::CXXCleanupHandler()
{
}

CXXCleanupHandler::~CXXCleanupHandler()
{
	clear();
}

void* CXXCleanupHandler::addMalloc(void* mem)
{
	if (mem)
	{
		cleanupList_.push_back(new cxx::CleanupItem(mem, cxx::CleanupType::Malloc));
	}
	return mem;
}

void CXXCleanupHandler::clear()
{
	for (auto& item : cleanupList_)
	{
		release(item);
	}
	cleanupList_.clear();
}

bool CXXCleanupHandler::isEmpty() const
{
	return cleanupList_.empty();
}

bool CXXCleanupHandler::empty() const
{
	return cleanupList_.empty();
}

__int64 CXXCleanupHandler::size() const
{
	return cleanupList_.size();
}

__int64 CXXCleanupHandler::count() const
{
	return size();
}

__int64 CXXCleanupHandler::length() const
{
	return size();
}

void CXXCleanupHandler::release(cxx::CleanupItem* item)
{
	delete item;  // 触发 CleanupItem 的析构函数
}
