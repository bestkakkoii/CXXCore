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

#ifndef MODULE_CXZIP_H_
//@隐藏{
#define MODULE_CXZIP_H_
//@隐藏}

//@模块名称 炫扩展模块-ZIP压缩
//@版本     炫扩展-子模块
//@日期     请参考炫扩展-核心模块
//@作者     Bestkakkoii
//@依赖     "module_cxxcore.h"
//@QQ       1642687802
//@默认安装位置 炫扩展\
//@包含目录 @当前模块路径 "CXXCore-master\include"
//@包含目录 @当前模块路径 "CXXCore-master\3rdparty"
//@模块备注 基於开源网 CodeProject 的 Zip Utils(轻量级库)修改 http://www.codeproject.com/Articles/7530/Zip-Utils-clean-elegant-simple-C-Win

#include <cxxglobal.h>
#include <cxxstring.h>
#include "../3rdparty/zip/zip.h"
#include "../3rdparty/zip/unzip.h"

//@src "CXXCore-master/3rdparty/zip/zip.cpp"
//@src "CXXCore-master/3rdparty/zip/unzip.cpp"

//@分组{ ［工具］.炫　ZIP压缩

//@别名 炫ZIP
class CXXZip
{
public:
	CXXZip(const CXXString& source, const CXXString& destination)
		: source_(source), destination_(destination)
	{

	}

	//@备註 解压缩ZIP文件
	//@参数 来源ZIP文件路径 (如果路径扩展名不为.zip，则内部会自动添加)
	//@参数 存放解压缩后文件的文件夹路径
	//@别名 解压缩(来源, 目的)
	bool uncompress() const
	{
		return uncompress(source_, destination_);
	}

	//@备註 压缩文件夹或文件
	//@参数 来源文件夹路径、或来源文件路径
	//@参数 存放压缩文件的路径 (如果路径扩展名不为.zip，则内部会自动添加)
	//@别名 压缩(来源, 目的)
	bool compress() const
	{
		return compress(source_, destination_);
	}

public:
	//@备註 静态-解压缩ZIP文件
	//@参数 来源ZIP文件路径 (如果路径扩展名不为.zip，则内部会自动添加)
	//@参数 存放解压缩后文件的文件夹路径
	//@别名 解压缩(来源, 目的)
	static bool uncompress(const CXXString& source, const CXXString& destination)
	{
		try
		{
			return zipper::uncompress(source.get(), destination.get());
		}
		catch (...)
		{
			return false;
		}
	}

	//@备註 静态-压缩文件夹或文件
	//@参数 来源文件夹路径、或来源文件路径
	//@参数 存放压缩文件的路径 (如果路径扩展名不为.zip，则内部会自动添加)
	//@别名 压缩(来源, 目的)
	static bool compress(const CXXString& source, const CXXString& destination)
	{
		try
		{
			return zipper::compress(source.get(), destination.get());
		}
		catch (...)
		{
			return false;
		}
	}

private:
	CXXString source_;
	CXXString destination_;
};


//@分组}

#endif // MODULE_CXZIP_H_