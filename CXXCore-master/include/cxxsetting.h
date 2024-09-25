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
#ifndef CXXSETTING_H_
//@隐藏{
#define CXXSETTING_H_
//@隐藏}

#include <cxxglobal.h>
#include <cxxvector.h>
#include <cxxstring.h>
#include <cxxvariant.h>
#include <cxxscopedpointer.h>


#include <string>


//@别名 炫
namespace cxx
{
	//@分组{［文本］.炫　配置类（ＩＮＩ／注册表）

	//@别名 配置格式
	enum SettingFormat
	{
		InvalidFormat = 0, //@别名 无效格式

		IniFormat, //@别名 INI格式
		RegistryFormat, //@别名 注册表格式
	};

	//@分组}
}

//@分组{［文本］.炫　配置类（ＩＮＩ／注册表）

//@隐藏{
class CXXSettingPrivate;
//@隐藏}

//@备注 配置类，用于读写INI文件或者注册表
//@别名 炫配置
class CXXSetting
{
public:
	//@参数 fullPath: 完整路径
	//@参数 format: 格式
	CXXSetting(const CXXString& fullPath, cxx::SettingFormat format);

	//@隐藏{
	~CXXSetting();
	//@隐藏}

	//@参数 清空INI指定键的值或者清空注册表指定键的值
	//@别名 删除值(值名)
	bool deleteValue(const CXXString& valueName);

	//@备注 删除注册表的键
	//@参数 键
	//@别名 删除注册表键()
	bool deleteKey();

	//@备注 根据键名取註册表数值
	//@别名 取(键名)
	CXXVariant get(const CXXString& key);

	//@备注 根据INI文件的节名和键名取配置值
	//@别名 取(节名,键名)
	CXXVariant get(const CXXString& section, const CXXString& key);

	//@备注 枚举注册表键的名称
	//@别名 枚举注册表键()
	CXXVector<CXXString> keys();

	//@备注 根据节名枚举所有键的名称
	//@别名 枚举配置键(节名)
	CXXVector<CXXString> keys(const CXXString section);

	//@备注 枚举所有节的名称
	//@别名 枚举配置节()
	CXXVector<CXXString> sections();

	//@备注 设置INI文件的文本值
	//@别名 置(节名, 键名, 值)
	bool set(const CXXString& section, const CXXString& key, const CXXString& value);

	//@备注 设置INI文件的整数值
	//@别名 置(节名, 键名, 值)
	bool set(const CXXString& section, const CXXString& key, __int64 value);

	//@备注 设置注册表文件 文本或正整型
	//@别名 置(键名, 值)
	bool set(const CXXString& key, const CXXVariant& value);

private:
	CXX_DECLARE_PRIVATE(CXXSetting);
	CXXScopedPointer<CXXSettingPrivate> d_ptr;
};


//@分组}

#endif // CXXSETTING_H_