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
#ifndef CXXSETTING_P_H_
#define CXXSETTING_P_H_

#include <cxxsetting.h>

#include <windows.h>
#include <map>
#include <stdexcept>
#include <fstream>

class CXXSettingPrivate
{
public:
	explicit CXXSettingPrivate(CXXSetting* q, cxx::SettingFormat format);

	virtual ~CXXSettingPrivate();

	// 解析完整的路径，并设置根键和子路径
	void parsePath(const std::wstring& fullPath);

	// 创建或打开注册表键
	void createOrOpenKey(HKEY* hkey);

private:
	HKEY hkey_ = CXX_NULLPTR; // 注册表键的句柄
	HKEY root_key_ = CXX_NULLPTR; // 根键句柄
	std::wstring root_path_; // 根键路径
	std::wstring parent_path_; // 父键路径
	std::wstring sub_key_name_; // 子键名称
	std::wstring sub_path_; // 子路径
	std::atomic_bool isValid_ = false; // 是否有效
	cxx::SettingFormat format_ = cxx::SettingFormat::InvalidFormat; // 格式

private:
	CXX_DECLARE_PUBLIC(CXXSetting);
	CXXSetting* q_ptr = CXX_NULLPTR;
};

#endif // CXXSETTING_P_H_