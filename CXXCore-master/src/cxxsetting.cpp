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

#include <cxxsetting_p.h>

#include <memory>
#include <map>

CXXSettingPrivate::CXXSettingPrivate(CXXSetting* q, cxx::SettingFormat format)
	: q_ptr(q)
	, format_(format)
{
}

CXXSettingPrivate::~CXXSettingPrivate()
{
}

// 解析完整的路径，并设置根键和子路径
void CXXSettingPrivate::parsePath(const std::wstring& fullPath)
{
	static const std::map<std::wstring, HKEY> knownRootKeys = {
		{L"HKEY_CURRENT_USER", HKEY_CURRENT_USER},
		{L"HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE},
		{L"HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT},
		{L"HKEY_USERS", HKEY_USERS},
		{L"HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG}
	};

	size_t pos = fullPath.find(L'\\');
	if (pos == std::wstring::npos)
	{
		return; // 无效的路径
	}

	auto rootKeyStr = fullPath.substr(0, pos);
	auto it = knownRootKeys.find(rootKeyStr);
	if (it == knownRootKeys.end())
	{
		return; // 无效的根键
	}

	root_key_ = it->second;
	root_path_ = rootKeyStr;
	sub_path_ = fullPath.substr(pos + 1);

	// Separate the parent path and the direct key name for deletion purposes
	size_t lastBackslashPos = sub_path_.rfind(L'\\');
	if (lastBackslashPos != std::string::npos)
	{
		parent_path_ = sub_path_.substr(0, lastBackslashPos);
		sub_key_name_ = sub_path_.substr(lastBackslashPos + 1);
	}
	else
	{
		parent_path_ = L"";
		sub_key_name_ = sub_path_;
	}

	isValid_.store(true, std::memory_order_relaxed);
}

// 创建或打开注册表键
void CXXSettingPrivate::createOrOpenKey(HKEY* hkey)
{
	if (!isValid_.load(std::memory_order_relaxed))
	{
		*hkey = CXX_NULLPTR;
		return;
	}

	isValid_.store(false, std::memory_order_relaxed);

	DWORD disposition;
	if (RegCreateKeyExW(
		root_key_,
		sub_path_.c_str(),
		0,
		CXX_NULLPTR,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		CXX_NULLPTR,
		hkey,
		&disposition
	) != ERROR_SUCCESS)
	{
		*hkey = CXX_NULLPTR; // 失败时设置为CXX_NULLPTR
	}

	isValid_.store(*hkey != CXX_NULLPTR, std::memory_order_relaxed);
}

//@参数 fullPath: 完整路径
	//@参数 format: 格式
CXXSetting::CXXSetting(const CXXString& fullPath, cxx::SettingFormat format)
	: d_ptr(new CXXSettingPrivate(this, format))
{
	CXX_D(CXXSetting);

	if (format == cxx::RegistryFormat)
	{
		d->parsePath(fullPath.c_str());
		d->createOrOpenKey(&d->hkey_);
	}
	else
	{
		d->root_path_ = fullPath;
	}
}

CXXSetting::~CXXSetting()
{
	CXX_D(CXXSetting);

	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return;
	}

	d->isValid_.store(false, std::memory_order_relaxed);

	if (d->format_ == cxx::RegistryFormat && d->hkey_ != CXX_NULLPTR)
	{
		RegCloseKey(d->hkey_);
	}
}


//@参数 清空INI指定键的值或者清空注册表指定键的值
//@别名 删除(值名)
bool CXXSetting::deleteValue(const CXXString& valueName)
{
	CXX_D(CXXSetting);

	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return false;
	}

	if (d->format_ == cxx::RegistryFormat)
	{
		return RegDeleteValueW(d->hkey_, valueName.c_str()) == ERROR_SUCCESS;
	}
	else if (d->format_ == cxx::IniFormat)
	{
		return WritePrivateProfileStringW(CXX_NULLPTR, valueName.c_str(), CXX_NULLPTR, d->root_path_.c_str()) != FALSE;
	}

	return false;
}

//@备注 删除注册表的键
//@参数 键
//@别名 删除注册表键()
bool CXXSetting::deleteKey()
{
	CXX_D(CXXSetting);

	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return false;
	}

	if (d->format_ == cxx::RegistryFormat)
	{
		HKEY parentKey;
		if (RegOpenKeyExW(d->root_key_, d->parent_path_.c_str(), 0, KEY_ALL_ACCESS, &parentKey) == ERROR_SUCCESS)
		{
			bool result = RegDeleteKey(parentKey, d->sub_key_name_.c_str()) == ERROR_SUCCESS;
			RegCloseKey(parentKey);
			return result;
		}
	}

	return false;
}

//@别名 取(键名)
CXXVariant CXXSetting::get(const CXXString& key)
{
	CXX_D(CXXSetting);

	CXXVariant defaultValue;
	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return defaultValue;
	}

	if (d->format_ != cxx::RegistryFormat)
	{
		return defaultValue;
	}

	DWORD type = 0;
	DWORD dataSize = 8192;
	std::unique_ptr<BYTE[]> data(new BYTE[dataSize]());
	LSTATUS status = RegQueryValueExW(d->hkey_, key.c_str(), CXX_NULLPTR, &type, reinterpret_cast<LPBYTE>(&data), &dataSize);

	if (status == ERROR_SUCCESS)
	{
		if (type == REG_SZ)
		{
			return 	CXXVariant(CXXString(reinterpret_cast<wchar_t*>(data.get())));
		}
		else if (type == REG_DWORD)
		{
			return CXXVariant(*reinterpret_cast<UINT*>(data.get()));
		}
		else if (type == REG_QWORD)
		{
			return CXXVariant(*reinterpret_cast<UINT64*>(data.get()));
		}
	}

	return defaultValue;
}

//@别名 取(键名, 默认值)
CXXVariant CXXSetting::get(const CXXString& section, const CXXString& key)
{
	CXX_D(CXXSetting);

	CXXVariant defaultValue;
	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return defaultValue;
	}

	if (d->format_ != cxx::RegistryFormat)
	{
		return defaultValue;
	}

	constexpr size_t bufferSize = 8192;
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[bufferSize]());

	DWORD charsCopied = GetPrivateProfileStringW(
		section.c_str(), // 节名
		key.c_str(),     // 键名
		CXX_NULLPTR, // 默认值
		buffer.get(),          // 读取值的缓冲区
		bufferSize,      // 缓冲区大小
		d->root_path_.c_str() // INI文件路径
	);

	if (charsCopied == 0)
	{
		return defaultValue;
	}

	//if not start with " and end with " then it is a number
	if (buffer[0] != L'"' && buffer[static_cast<size_t>(charsCopied) - 1] != L'"')
	{
		return CXXVariant(_wtoi64(buffer.get()));
	}

	return CXXVariant(CXXString(buffer.get() + 1, charsCopied - 2));
}

//@备注 枚举注册表键的名称
//@别名 枚举注册表键()
CXXVector<CXXString> CXXSetting::keys()
{
	CXX_D(CXXSetting);

	CXXVector<CXXString> keysList;
	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return keysList;
	}

	if (d->format_ == cxx::RegistryFormat)
	{
		// 对于注册表，枚举所有键的名称
		TCHAR keyName[256] = {}; // 假设键名不超过256个字符
		DWORD keyNameSize;
		DWORD index = 0;

		for (;;)
		{
			keyNameSize = sizeof(keyName) / sizeof(keyName[0]);
			LONG ret = RegEnumValueW(d->hkey_, index, keyName, &keyNameSize, CXX_NULLPTR, CXX_NULLPTR, CXX_NULLPTR, CXX_NULLPTR);
			if (ret != ERROR_SUCCESS)
			{
				break;
			}

			keysList.push_back(CXXString(keyName));
			++index;
		}
	}
	return keysList;
}

//@备注 根据节名枚举所有键的名称
//@别名 枚举配置键(节名)
CXXVector<CXXString> CXXSetting::keys(const CXXString section)
{
	CXX_D(CXXSetting);

	CXXVector<CXXString> keysList;
	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return keysList;
	}

	if (d->format_ == cxx::IniFormat)
	{
		constexpr size_t bufferSize = 32767;
		std::unique_ptr<wchar_t[]> buffer(new wchar_t[bufferSize]());
		DWORD charsCopied = GetPrivateProfileStringW(
			section.c_str(), // 节名
			CXX_NULLPTR,  // 获取所有键的名称
			L"",      // 默认值
			buffer.get(), // 读取值的缓冲区
			bufferSize, // 缓冲区大小
			d->root_path_.c_str() // INI文件路径
		);

		if (0 == charsCopied)
		{
			return keysList;
		}

		wchar_t* p = buffer.get();
		while (*p)
		{
			keysList.push_back(CXXString(p));
			p += wcslen(p) + 1;
		}
	}

	return keysList;
}

//@备注 枚举所有节的名称
//@别名 枚举配置节()
CXXVector<CXXString> CXXSetting::sections()
{
	CXX_D(CXXSetting);

	CXXVector<CXXString> sectionsList;
	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return sectionsList;
	}

	if (d->format_ == cxx::IniFormat)
	{
		constexpr size_t bufferSize = 32767;
		std::unique_ptr<wchar_t[]> buffer(new wchar_t[bufferSize]);
		DWORD charsCopied = GetPrivateProfileStringW(
			CXX_NULLPTR,
			CXX_NULLPTR,
			L"",
			buffer.get(),
			bufferSize,
			d->root_path_.c_str()
		);

		if (0 == charsCopied)
		{
			return sectionsList;
		}

		wchar_t* p = buffer.get();
		while (*p)
		{
			sectionsList.push_back(CXXString(p));
			p += wcslen(p) + 1;
		}
	}

	return sectionsList;
}

//@备注 设置INI文件的文本值
//@别名 置(节名, 键名, 值)
bool CXXSetting::set(const CXXString& section, const CXXString& key, const CXXString& value)
{
	CXX_D(CXXSetting);

	if (static_cast<__int64>(d->root_path_.size()) <= 0)
	{
		return false;
	}

	if (d->format_ != cxx::IniFormat)
	{
		return false;
	}

	return WritePrivateProfileStringW(
		section.c_str(), // 节名
		key.c_str(),     // 键名
		value.c_str(),   // 键值
		d->root_path_.c_str() // INI文件路径
	) != FALSE;
}

//@备注 设置INI文件的整数值
//@别名 置(节名, 键名, 值)
bool CXXSetting::set(const CXXString& section, const CXXString& key, __int64 value)
{
	CXX_D(CXXSetting);

	if (static_cast<__int64>(d->root_path_.size()) <= 0)
	{
		return false;
	}

	if (d->format_ != cxx::IniFormat)
	{
		return false;
	}

	return WritePrivateProfileStringW(
		section.c_str(), // 节名
		key.c_str(),     // 键名
		std::to_wstring(value).c_str(),   // 键值
		d->root_path_.c_str() // INI文件路径
	) != FALSE;
}

//@备注 设置注册表文件 文本或正整型
//@别名 置(键名, 值)
bool CXXSetting::set(const CXXString& key, const CXXVariant& value)
{
	CXX_D(CXXSetting);

	if (!d->isValid_.load(std::memory_order_relaxed))
	{
		return false;
	}

	if (d->format_ != cxx::RegistryFormat)
	{
		return false;
	}

	DWORD type = REG_NONE;

	CXXScopedArrayPointer<BYTE> data;
	size_t dataSize = 0;
	switch (value.type())
	{
	case cxx::DataType::Bool:
	case cxx::DataType::Char:
	case cxx::DataType::UChar:
	case cxx::DataType::Short:
	case cxx::DataType::UShort:
	case cxx::DataType::Int:
	case cxx::DataType::UInt:
	{
		UINT u = value.value<UINT>();
		dataSize = sizeof(u);
		data.reset(new BYTE[dataSize]());
		memmove_s(data.get(), dataSize, &u, dataSize);

		type = REG_DWORD;
		break;
	}
	case cxx::DataType::LongLong:
	case cxx::DataType::ULongLong:
	{
		UINT64 ul64 = value.value<UINT64>();
		dataSize = sizeof(ul64);
		data.reset(new BYTE[dataSize]());
		memmove_s(data.get(), dataSize, &ul64, dataSize);

		type = REG_QWORD;
		break;
	}
	case cxx::DataType::WString:
	{
		CXXString text = value.value<CXXString>();
		dataSize = static_cast<size_t>((text.size() + 1) * sizeof(wchar_t));
		data.reset(new BYTE[dataSize]());
		memmove_s(data.get(), dataSize, text.c_str(), dataSize);
		type = REG_SZ;
		break;
	}
	default:
	{
		return false;
	}
	}

	if (CXX_NULLPTR == data)
	{
		return false;
	}

	LSTATUS status = RegSetValueExW(
		d->hkey_,    // 键句柄
		key.c_str(), // 键名
		NULL,        // 保留
		type,        // 数据类型
		data.get(),  // 数据
		static_cast<DWORD>(dataSize) // 数据大小
	);

	return status == ERROR_SUCCESS;
}