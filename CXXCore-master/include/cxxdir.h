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
#ifndef CXXDIR_H_
//@隐藏{
#define CXXDIR_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

//@分组{［文件／文件夹］.炫　文件夹类

//@隐藏{
class CXXDirPrivate;
//@隐藏}

//@备注 用于管理文件夹、及文件夹操作
//@别名 炫文件夹
class CXXDir
{
public:
	//@参数 文件夹路径
	CXXDir(const CXXString& path = CXXString());

	virtual ~CXXDir();

	//@备注 获取文件夹绝对路径 + 文件名称
	//@参数 文件名
	//@返回 文件夹绝对路径 + 文件名称
	//@别名 绝对路径(文件名)
	CXXString absoluteFilePath(const CXXString& fileName) const;

	//@备注 获取文件夹绝对路径
	//@返回 文件夹绝对路径
	//@别名 绝对路径()
	CXXString absolutePath() const;

	//@备注 获取文件夹规范路径
	//@返回 文件夹规范路径
	//@别名 规范路径()
	CXXString canonicalPath() const;

	//@备注 改变当前已存文件夹
	//@参数 文件夹名称
	//@返回 是否成功
	//@别名 到(文件夹名称)
	bool cd(const CXXString& dirName);

	//@备注 改变当前已存文件夹到上一级
	//@返回 是否成功
	//@别名 到上一级()
	bool cdUp();

	//@备注 获取文件夹名称
	//@返回 文件夹名称
	//@别名 名称()
	CXXString name() const;

	//@备注 检查是否可读
	//@返回 是否可读
	//@别名 是否可读()
	bool isReadable() const;

	//@备注 检查是否是根目录
	//@返回 是否是根目录
	//@别名 是否根目录()
	bool isRoot() const;

	//@备注 基于当前文件夹创建新文件夹
	//@参数 文件夹名称
	//@返回 是否成功
	//@别名 创建(文件夹名称)
	bool make(const CXXString& path);

	//@备注 移除文件夹
	//@返回 是否成功
	//@别名 移除()
	bool remove();

	//@备注 重命名文件夹
	//@参数 新文件夹名称
	//@返回 是否成功
	//@别名 重命名(新文件夹名称)
	bool rename(const CXXString& newName);

public:
	//@备注 获取系统暂存目录
	//@返回 临时目录
	//@别名 暂存目录()
	static CXXString tempPath();

	//@备注 获取 user/appdata/roaming 路径
	//@返回 AppData目录
	//@别名 roamingAppData()
	static CXXString roamingAppDataPath();

	//@备注 获取 user/appdata/local 路径
	//@返回 LocalAppData目录
	//@别名 localAppData()
	static CXXString localAppDataPath();

	//@备注 取桌面目录
	//@返回 桌面目录
	//@别名 桌面目录()
	static CXXString desktopPath();

	//@备注 取启动程序目录
	//@返回 启动程序目录
	//@别名 启动程序目录()
	static CXXString startupPath();

	//@备注 静态:递归创建目录
	//@参数 文件夹路径
	//@返回 是否成功
	//@别名 创建路径(文件夹路径)
	static bool mkpath(const CXXString& path);

	//@备注 静态:移除文件夹
	//@参数 文件夹路径
	//@返回 是否成功
	//@别名 移除(文件夹路径)
	static bool remove(const CXXString& path);

	//@备注 静态:打开文件夹
	//@参数 文件夹路径
	//@返回 是否成功
	//@别名 打开(文件夹路径)
	static bool open(const CXXString& path);

	//@备注 静态:设置当前工作文件夹
	//@参数 文件夹路径
	//@返回 是否成功
	//@别名 置当前工作目录(文件夹路径)
	static bool setCurrentDir(const CXXString& path);

	//@备注 静态:设置当前工作文件夹
	//@参数 文件夹路径
	//@返回 是否成功
	//@别名 当前工作目录(文件夹路径)
	static CXXString currentDir();

	//@备注 静态:重命名文件夹
	//@参数 旧文件夹名称
	//@参数 新文件夹名称
	//@返回 是否成功
	//@别名 重命名(旧文件夹名称, 新文件夹名称)
	static bool rename(const CXXString& oldName, const CXXString& newName);

public:
	//@隐藏{
	bool operator!=(const CXXDir& dir) const;
	CXXDir& operator=(const CXXDir& dir);
	CXXDir& operator=(CXXDir&& other);
	bool operator==(const CXXDir& dir) const;
	//@隐藏}

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXDir);
	CXXScopedPointer<CXXDirPrivate> d_ptr;
	//@隐藏}

};

//@分组}

#endif // CXXDIR_H_