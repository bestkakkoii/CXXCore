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
#ifndef CXXSHAREDMEMORY_H_
//@隐藏{
#define CXXSHAREDMEMORY_H_
//@隐藏}



#include <string>
#include <windows.h>
#include <stdexcept>
#include <string>
#include <shared_mutex>
#include <cstring>

//@别名 炫
namespace cxx
{


}

//@分组{［内存安全］.炫　共享内存类


//@备注 CXXSharedMemory 类用于在 Windows 平台上实现共享内存的管理。
//       它提供了创建共享内存、附加、分离、写入、读取等功能，支持 A 和 W 版本切换。
//@别名 炫共享内存
class CXXSharedMemory
{
public:
	//@隐藏{
	explicit
		//@隐藏}

	//@备注 通过键创建共享内存对象，并可切换版本
	//@参数 key 共享内存的唯一键
	//@参数 version 共享内存的版本，ANSI 或 Unicode
	//@别名 构造函数 (键, 版本)
		CXXSharedMemory(const std::string& key, cxx::SharedMemoryVersion version = cxx::SharedMemoryANSI);

	//@备注 析构函数，自动分离共享内存
	//@别名 析构函数
	virtual ~CXXSharedMemory();

	//@备注 创建共享内存段
	//@参数 size 共享内存大小（字节）
	//@参数 mode 访问模式（默认读写模式）
	//@返回 是否成功创建共享内存段
	//@别名 创建 (大小, 访问模式)
	bool create(__int64 size, cxx::SharedMemoryAccessMode mode = cxx::SharedMemoryReadWrite);

	//@备注 附加到现有共享内存段
	//@参数 mode 访问模式（默认读写模式）
	//@返回 是否成功附加
	//@别名 附加 (访问模式)
	bool attach(cxx::SharedMemoryAccessMode mode = cxx::SharedMemoryReadWrite);

	//@备注 分离共享内存并释放资源
	//@返回 是否成功分离
	//@别名 分离()
	bool detach();

	//@备注 获取共享内存的指针
	//@返回 指向共享内存数据的指针
	//@别名 地址()
	void* data() const;

	//@备注 获取共享内存的指针
	//@返回 指向共享内存数据的指针
	//@别名 取()
	void* get() const;

	//@备注 获取共享内存的大小
	//@返回 共享内存的大小
	//@别名 大小()
	__int64 size() const;

	//@备注 获取共享内存的大小
	//@返回 共享内存的大小
	__int64 count() const;

	//@备注 获取共享内存的大小
	//@返回 共享内存的大小
	__int64 length() const;

	//@备注 写入共享内存
	//@参数 src 源数据的指针
	//@参数 len 要写入的字节数
	//@返回 是否写入成功
	//@别名 写(数据指针, 长度)
	bool write(const void* src, __int64 len);

	//@备注 读取共享内存
	//@参数 dest 目标缓冲区的指针
	//@参数 len 要读取的字节数
	//@返回 是否读取成功
	//@别名 读(目标指针, 长度)
	bool read(void* dest, __int64 len);

	//@备注 返回共享内存的最后错误状态
	//@返回 错误状态枚举
	//@别名 错误()
	cxx::SharedMemoryError error() const;

	//@备注 返回共享内存的键
	//@返回 共享内存的唯一键
	//@别名 键()
	std::string key() const;

	//@备注 返回共享内存的错误信息
	//@返回 错误信息字符串
	//@别名 错误文本()
	std::string errorString() const;

	//@备注 检查共享内存是否已经附加
	//@返回 布尔值，表示是否附加
	//@别名 是否附加()
	bool isAttached() const;

private:
	//@隐藏{
	void setError(cxx::SharedMemoryError error);
	//@隐藏}
private:
	//@隐藏{
	std::string key_;                // 共享内存的唯一键
	HANDLE memoryHandle_ = nullptr;  // 文件映射对象句柄
	void* memory_;                   // 映射到的共享内存地址
	__int64 size_;               // 共享内存大小
	bool attached_;                  // 是否已附加共享内存
	cxx::SharedMemoryError error_;        // 错误状态
	cxx::SharedMemoryVersion version_;                // ANSI 或 Unicode 版本

	mutable std::shared_mutex mutex_;  // 读写锁，确保线程安全
	//@隐藏}
};

//@分组}

#endif // CXXSHAREDMEMORY_H_
