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

#ifndef CXXFILEDEVICE_H_
//@隐藏{
#define CXXFILEDEVICE_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <shlwapi.h>
#include <regex>

//@别名 炫
namespace cxx
{
	//@分组{［文件／文件夹］.炫　文件类

	//@别名  打开模式
	enum FileOpenModeFlag
	{
		//@别名 读取
		Read = 1 << 0,
		//@别名  写入
		Write = 1 << 1,
		//@别名  读写
		ReadWrite = 1 << 2,
		//@别名  从文件尾
		Ate = 1 << 3,
		//@别名  附加文件尾
		Append = 1 << 4,
		//@别名  清空文件
		Truncate = 1 << 5,
		//@别名  读写清空
		ReadWriteTruncate = ReadWrite | Truncate,
		//@别名  二进制
		Binary = 1 << 6,
		//@别名  读写二进制
		ReadWriteBinary = ReadWrite | Binary,
		Nocreate = 1 << 7,  //@别名  文件不存在时失败
		Noreplace = 1 << 8, //@别名  文件存在时失败
	};
	//@隐藏{
	CXX_DECLARE_FLAGS(FileOpenMode, FileOpenModeFlag);
	//@隐藏}

	//@别名  指针位置
	enum SeekDir
	{
		Begin = 0,   //@别名 开头
		Current = 1, //@别名 当前
		End = 2,     //@别名 结尾
	};

	//@分组}


} // namespace cxx

//@分组{［文件／文件夹］.炫　文件类

//@隐藏{
class CXXFilePrivate;
//@隐藏}

//@备注 用于管理文件路径、操作、读写等，不需要自己关闭文件，析构函数会自动关闭文件
//@别名 炫文件
class CXXFile
{
public:
	CXXFile();

	//@参数 文件路径
	CXXFile(const CXXString& path);

	//@参数 文件路径
	//@参数 打开模式
	//@参数 是否共享
	CXXFile(const CXXString& path, cxx::FileOpenMode type, bool isShared = true);

	virtual ~CXXFile();

	//@备注 打开文件
	//@参数 (可选)打开模式
	//@参数 (可选)是否共享
	//@返回 是否成功
	//@别名 打开(打开模式, 是否共享)
	bool open(cxx::FileOpenMode type = cxx::ReadWrite, bool isShared = true);

	//@备注 打开文件
	//@参数 文件路径
	//@参数 (可选)打开模式
	//@参数 (可选)是否共享
	//@返回 是否成功
	//@别名 打开(文件路径, 打开模式, 是否共享)
	bool open(const CXXString& path, cxx::FileOpenMode type = cxx::ReadWrite, bool isShared = true);

	//@备注 是否打开
	//@返回 是否打开
	//@别名 是否打开()
	bool isOpen() const;

	//@备注 是否存在
	//@返回 是否存在
	//@别名 是否存在()
	bool exists() const;

	//@备注 获取文件名
	//@返回 文件名
	//@别名 文件名()
	CXXString fileName();

	//@备注 获取目录路径
	//@返回 目录路径
	//@别名 目录()
	CXXString directory() const;

	//@备注 改变文件名
	//@参数 新文件名 (完整路径 | 相对路径)
	//@返回 是否成功
	//@别名 改名(新文件名)
	bool rename(const CXXString& newpath);

	//@备注 删除文件
	//@返回 是否成功
	//@别名 移除()
	bool remove();

	//@备注 复制文件
	//@参数 新文件名 (完整路径 | 相对路径)
	//@返回 是否成功
	//@别名 复制(新文件名)
	bool copy(const std::wstring& newpath);

	//@备注 创建硬链接
	//@参数 新文件名 (完整路径 | 相对路径)
	//@返回 是否成功
	//@别名 链接(新文件名)
	bool link(const CXXString& newpath);

	//@备注 是否为可执行文件
	//@返回 是否为可执行文件
	//@别名 是否可执行()
	bool isExecutable() const;

	//@备注 是否为文件
	//@返回 是否为文件
	//@别名 是否文件()
	bool isFile() const;

	//@备注 是否为目录
	//@返回 是否为目录
	//@别名 是否目录()
	bool isDir() const;

	//@备注 获取文件扩展名
	//@返回 文件扩展名
	//@别名 扩展名()
	CXXString suffix() const;

	//@备注 是否有效
	//@返回 是否有效
	//@别名 是否有效()
	bool isValid() const;

	//@备注 刷新文件，将缓冲区内容写入文件
	//@返回 无
	//@别名 刷新()
	void flush();

	//@备注 关闭文件
	//@返回 是否成功
	//@别名 关闭()
	bool close();

	//@备注 从当前文件指针位置读取至结尾
	//@返回 读取内容
	//@别名 读()
	CXXString read() const;

	//@备注 读取整个文件
	//@返回 读取内容
	//@别名 读全部()
	CXXString readAll();

	//@备注 读取一行
	//@参数 行号
	//@返回 读取内容
	//@别名 读行(行号)
	CXXString readLine(__int64 index);

	//@备注 读取指定长度，如果长度为-1则从开始读取至结尾
	//@参数 接收数据的指针
	//@参数 读取长度
	//@返回 读取长度
	//@别名 读(数据指针, 长度)
	__int64 read(char* data, __int64 size = -1);

	//@备注 写入内容
	//@参数 内容
	//@返回 无
	//@别名 写(内容)
	void write(const CXXString& content);

	//@备注 写入内容
	//@参数 内容指针
	//@参数 内容长度
	//@返回 无
	//@别名 写(内容指针, 内容长度)
	void write(const char* data, __int64 size);

	//@备注 移动文件指针
	//@参数 位置
	//@参数 方向
	//@返回 无
	//@别名 移动指针(位置, 方向)
	void seek(__int64 pos, cxx::SeekDir dir = cxx::Begin);

	//@隐藏{
	CXXFile& operator<<(wchar_t* data);

	CXXFile& operator<<(const wchar_t* data);

	CXXFile& operator<<(const CXXString& data);

	CXXFile& operator<<(const char* data);
	//@隐藏}

	//@备注 获取文件大小
	//@参数 无
	//@返回 文件字节大小
	//@别名 大小()
	__int64 size() const;

	//@备注 获取文件大小
	//@参数 无
	//@返回 文件字节大小
	__int64 count() const;

	//@备注 获取文件大小
	//@参数 无
	//@返回 文件字节大小
	__int64 length() const;

	//@备注 文件最后修改日期
	//@参数 无
	//@返回 最后修改日期
	//@别名 最后修改时间()
	CXXString lastModified() const;

	//@备注 不需要先行打开文件，映射文件到內存，返回的指针可以选择自行释放也可以由析构函数自动释放，
	//@参数 偏移量
	//@参数 长度
	//@返回 映射指针
	//@别名 映射(偏移量, 长度)
	BYTE* map(__int64 offset, __int64 size);

	//@备注 释放映射
	//@参数 指针
	//@返回 无
	//@别名 释放映射(映射指针)
	void unmap(BYTE* ptr);
public:
	//@备注 静态:检查文件是否存在
	//@参数 文件路径
	//@返回 是否存在
	//@别名 存在(文件路径)
	static bool exists(const CXXString& path);

	//@备注 静态:获取文件扩展名
	//@参数 文件路径
	//@返回 扩展名
	//@别名 扩展名(文件路径)
	static CXXString suffix(const CXXString& path);

	//@备注 静态:移除文件
	//@参数 文件路径
	//@返回 是否成功
	//@别名 移除(文件路径)
	static bool remove(const CXXString& path);

	//@备注 静态:重命名文件
	//@参数 原文件名 (完整路径 | 相对路径)
	//@参数 新文件名 (完整路径 | 相对路径)
	//@返回 是否成功
	//@别名 重命名(原文件名, 新文件名)
	static bool rename(const CXXString& oldname, const CXXString& newname);

	//@备注 静态:复制文件
	//@参数 原文件名 (完整路径 | 相对路径)
	//@参数 新文件名 (完整路径 | 相对路径)
	//@返回 是否成功
	//@别名 复制(原文件名, 新文件名)
	static bool copy(const CXXString& src, const CXXString& dst);

	//@备注 静态:移除文件夹
	//@参数 文件夹路径
	//@返回 是否成功
	//@别名 移除文件夹(文件夹路径)
	static bool removeDir(const CXXString& path);

	//@备注 静态:创建符号链接
	//@参数 原文件名 (完整路径 | 相对路径)
	//@参数 新文件名 (完整路径 | 相对路径)
	//@返回 是否成功
	//@别名 链接(原文件名, 新文件名)
	static bool link(const CXXString& src, const CXXString& dst);

	//@备注 静态:是否为目录
	//@参数 路径
	//@返回 是否为文件夹
	//@别名 是否目录(路径)
	static bool isDir(const CXXString& path);

	//@备注 静态:是否为文件
	//@参数 路径
	//@返回 是否为文件
	//@别名 是否文件(路径)
	static bool isFile(const CXXString& path);

	//@备注 静态:是否为可执行文件
	//@参数 路径
	//@返回 是否为可执行文件
	//@别名 是否可执行(路径)
	static bool isExecutable(const CXXString& path);

	//@备注 静态:获取文件最后修改时间
	//@参数 路径
	//@返回 文件最后修改时间
	//@别名 最后修改时间(路径)
	static CXXString lastModified(const CXXString& filename);

	//@备注 静态:是否为隐藏文件
	//@参数 路径
	//@返回 是否为隐藏文件
	//@别名 是否隐藏(路径)
	static bool isHidden(const CXXString& path);

	//@备注 静态:是否为系统文件
	//@参数 路径
	//@返回 是否为系统文件
	//@别名 是否系统(路径)
	static bool isSystem(const CXXString& path);

	//@备注 静态:设置文件隐藏
	//@参数 路径
	//@参数 是否隐藏
	//@返回 是否成功
	//@别名 置隐藏(路径, 是否隐藏)
	static bool setFileHidden(const CXXString& path, bool hidden);

	//@备注 静态:设置文件只读
	//@参数 路径
	//@参数 是否只读
	//@返回 是否成功
	//@别名 置只读(路径, 是否只读)
	static bool setFileReadOnly(const CXXString& path, bool readonly);

	//@备注 静态:设置文件正常
	//@参数 路径
	//@返回 是否成功
	//@别名 置预设(路径)
	static bool setFileNormal(const CXXString& path);

	//@备注 静态:获取文件大小
	//@参数 路径
	//@返回 文件大小
	//@别名 大小(路径)
	static __int64 size(const CXXString& path);

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXFile);
	CXXScopedPointer<CXXFilePrivate> d_ptr;
	//@隐藏}
};

//@分组}

#endif // CXXFILEDEVICE_H_