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
#ifndef CXXCLEANUPHANDLER_H_
//@隐藏{
#define CXXCLEANUPHANDLER_H_
//@隐藏}



#include <vector>
#include <algorithm>
#include <cstdlib>
#include <type_traits>

//@分组{［内存安全］.炫　炫堆管理器

//@隐藏{

namespace cxx
{
	enum class CleanupType
	{
		// 普通指针，用 delete 释放
		Single,
		// 数组指针，用 delete[] 释放
		Array,
		// malloc 分配的内存，用 free() 释放
		Malloc
	};

	class CleanupItem
	{
	public:
		CleanupItem(void* ptr, cxx::CleanupType type);

		// 析构函数，根据类型释放指针
		virtual ~CleanupItem();

		void* ptr;           // 指针
		cxx::CleanupType type;    // 清理类型

	};
}


//@隐藏}

//@备注 CXXCleanupHandler 类用于托管和释放内存指针
//       该类支持管理三种类型的指针：普通指针、数组指针、通过 malloc 分配的指针。
//       在析构时自动释放所有托管的内存。
//@别名 炫堆管理器
class CXXCleanupHandler
{
public:
	//@备注 构造函数
	//@别名 构造函数
	CXXCleanupHandler();

	//@备注 析构函数，自动释放所有托管的内存
	//@别名 析构函数
	virtual ~CXXCleanupHandler();

	//@备注 添加普通指针到托管列表，析构时会自动 delete 指针 (new T)
	//@参数 object 要托管的普通指针
	//@返回 返回托管的指针
	//@别名 加指针(指针)
	template <typename T
		//@隐藏{
		, typename = std::enable_if_t<!std::is_array<T>::value>
		//@隐藏}
	>
	T* add(T* object)
	{
		if (object)
		{
			cleanupList_.push_back(new cxx::CleanupItem(object, cxx::CleanupType::Single));
		}
		return object;
	}

	//@备注 添加数组指针到托管列表，析构时会自动 delete[] 指针 (new T[])
	//@参数 array 要托管的数组指针
	//@返回 返回托管的指针
	//@别名 加数组指针(数组指针)
	template <typename T
		//@隐藏{
		, typename = std::enable_if_t<std::is_array<T>::value>
		//@隐藏}
	>
	T* addArray(T* array)
	{
		if (array)
		{
			cleanupList_.push_back(new cxx::CleanupItem(array, cxx::CleanupType::Array));
		}
		return array;
	}

	//@备注 添加通过 malloc 分配的指针，析构时会自动调用 free() (malloc)
	//@参数 mem 要托管的 malloc 分配的内存指针
	//@返回 返回托管的指针
	//@别名 加裸指针(指针)
	void* addMalloc(void* mem);

	//@备注 清除所有托管的指针，并释放内存
	//@别名 清空()
	void clear();

	//@备注 判断是否为空，即没有托管任何指针
	//@返回 布尔值，表示是否为空
	//@别名 是否空()
	bool isEmpty() const;

	//@备注 判断是否为空，即没有托管任何指针
	//@返回 布尔值，表示是否为空
	bool empty() const;

	//@备注 获取托管的指针数量
	//@返回 指针数量
	//@别名 大小()
	__int64 size() const;

	//@备注 获取托管的指针数量
	//@返回 指针数量
	__int64 count() const;

	//@备注 获取托管的指针数量
	//@返回 指针数量
	__int64 length() const;

	//@备注 移除指定的指针并不释放内存
	//@参数 object 要移除的指针
	//@别名 移除(指针)
	template <typename T>
	void remove(T* object)
	{
		auto it = std::find_if(cleanupList_.begin(), cleanupList_.end(),
			[object](const cxx::CleanupItem* item) { return item->ptr == object; });

		if (it != cleanupList_.end())
		{
			delete* it;  // 删除 CleanupItem
			cleanupList_.erase(it);
		}
	}


private:
	//@隐藏{
	//@备注 释放 CleanupItem 对象所管理的内存
	//@参数 item 要释放的 CleanupItem 对象
	void release(cxx::CleanupItem* item);
	//@隐藏}
private:
	//@隐藏{
	std::vector<cxx::CleanupItem*> cleanupList_;  // 存储 CleanupItem 对象的列表

	//@隐藏}
};

//@分组}


#endif // CXXCLEANUPHANDLER_H_
