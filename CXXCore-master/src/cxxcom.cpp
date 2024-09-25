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
#include <pch.h>

#include <cxxcom_p.h>

#include <oleauto.h>

CXXComObjectPrivate::CXXComObjectPrivate(CXXComObject* parent, IDispatch* lp)
	: q_ptr(parent)
	, p(lp)
{
	if (p != CXX_NULLPTR)
	{
		p->AddRef();
	}
}

CXXComObjectPrivate::~CXXComObjectPrivate()
{
	if (p != CXX_NULLPTR)
	{
		p->Release();
	}
}

HRESULT CXXComObjectPrivate::getIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
{
	HRESULT hr = E_FAIL;
	if (p == CXX_NULLPTR)
	{
		return hr;
	}

	return p->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, pdispid);
}

HRESULT CXXComObjectPrivate::invoke(DISPID dispid, VARIANT* pvarRet)
{
	HRESULT hr = E_FAIL;
	DISPPARAMS dispparams = { CXX_NULLPTR, CXX_NULLPTR, 0, 0 };

	if (p == CXX_NULLPTR)
	{
		return hr;
	}

	return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, CXX_NULLPTR, CXX_NULLPTR);
}

HRESULT CXXComObjectPrivate::invoke(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet)
{
	HRESULT hr = E_FAIL;
	DISPPARAMS dispparams = { pvarParams, CXX_NULLPTR, (UINT)nParams, 0 };

	if (p == CXX_NULLPTR)
	{
		return hr;
	}

	return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, CXX_NULLPTR, CXX_NULLPTR);
}

HRESULT CXXComObjectPrivate::setProperty(const CXXString& propertyName, const _variant_t& value)
{
	DISPID dispid;
	OLECHAR* olePropertyName = const_cast<OLECHAR*>(propertyName.c_str());
	HRESULT hr = p->GetIDsOfNames(IID_NULL, &olePropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to get DISPID for property '" << propertyName << L"': " << std::hex << hr << std::endl;
		return hr;
	}

	DISPPARAMS params = { CXX_NULLPTR, CXX_NULLPTR, 0, 0 };
	VARIANTARG rgvarg[1] = {};
	VariantInit(&rgvarg[0]);
	rgvarg[0] = value;
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	params.cArgs = 1;
	params.rgvarg = rgvarg;
	params.cNamedArgs = 1;  // 这里指定命名参数的数量为1
	params.rgdispidNamedArgs = &dispidNamed;  // 并将命名参数设置为DISPID_PROPERTYPUT

	hr = p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, CXX_NULLPTR, CXX_NULLPTR, CXX_NULLPTR);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to set property '" << propertyName << L"': " << std::hex << hr << std::endl;
	}
	return hr;
}

CXXComVariantPrivate::CXXComVariantPrivate(CXXComVariant* parent)
	: q_ptr(parent)
{
	VariantInit(&result_);
}

CXXComVariantPrivate::CXXComVariantPrivate(CXXComVariant* parent, const VARIANT& result)
	: q_ptr(parent)
{
	VariantInit(&result_);
	HRESULT hr = VariantCopy(&result_, &result);
	isValid_ = SUCCEEDED(hr);
}

CXXComVariantPrivate::~CXXComVariantPrivate()
{

}

CXXComVariant::CXXComVariant()
	: d_ptr(new CXXComVariantPrivate(this))
{

}

CXXComVariant::~CXXComVariant()
{

}

CXXComVariant::CXXComVariant(const VARIANT& result)
	: d_ptr(new CXXComVariantPrivate(this, result))
{

}

CXXComVariant::CXXComVariant(const CXXString& str)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_BSTR;
	result.bstrVal = SysAllocString(str.toStdWString().c_str());
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(const wchar_t* str)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_BSTR;
	result.bstrVal = SysAllocString(str);
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(const char* str)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_BSTR;
	result.bstrVal = SysAllocString(CXXString::fromLocal8Bit(str).toStdWString().c_str());
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(bool b)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_BOOL;
	result.boolVal = b;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(double d)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_R8;
	result.dblVal = d;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(float f)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_R4;
	result.fltVal = f;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(char i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_I1;
	result.cVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(short i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_I2;
	result.iVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(int i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_I4;
	result.intVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(__int64 i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_I8;
	result.llVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(BYTE i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_UI1;
	result.bVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(USHORT i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_UI2;
	result.uiVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(UINT i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_UI4;
	result.uintVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

CXXComVariant::CXXComVariant(UINT64 i)
{
	VARIANT result;
	VariantInit(&result);
	result.vt = VT_UI8;
	result.ullVal = i;
	d_ptr.reset(new CXXComVariantPrivate(this, result));
}

bool CXXComVariant::isNull() const
{
	CXX_D(const CXXComVariant);
	return d->result_.vt == VT_NULL || d->result_.vt == VT_EMPTY || !d->isValid_;
}

//@别名 清除()
void CXXComVariant::clear()
{
	CXX_D(CXXComVariant);
	d->isValid_ = false;
	VariantClear(&d->result_);
}

//@别名 到整型()
int CXXComVariant::toInt() const
{
	CXX_D(const CXXComVariant);
	return d->result_.intVal;
}

//@别名 到长整型()
__int64 CXXComVariant::toInt64() const
{
	CXX_D(const CXXComVariant);
	return d->result_.llVal;
}

//@别名 取数值()
double CXXComVariant::toDouble() const
{
	CXX_D(const CXXComVariant);
	return d->result_.dblVal;
}

//@别名 到文本()
CXXString CXXComVariant::toString() const
{
	CXX_D(const CXXComVariant);
	if (d->result_.bstrVal == CXX_NULLPTR || d->result_.vt == VT_NULL || d->result_.vt == VT_EMPTY)
	{
		return L"";
	}

	return std::wstring(d->result_.bstrVal);
}

//@别名 到逻辑()
bool CXXComVariant::toBool() const
{
	CXX_D(const CXXComVariant);
	return d->result_.boolVal;
}

//@别名 到分派指针()
IDispatch* CXXComVariant::toDispatch() const
{
	CXX_D(const CXXComVariant);
	return d->result_.pdispVal;
}

//@别名 类型名()
CXXString CXXComVariant::typeName() const
{
	static const std::map<intptr_t, std::wstring> typeNames = {
		{VT_EMPTY, L"empty"},
		{VT_NULL, L"null"},
		{VT_I2, L"short"},
		{VT_I4, L"int"},
		{VT_R4, L"float"},
		{VT_R8, L"double"},
		{VT_CY, L"currency"},
		{VT_DATE, L"date"},
		{VT_BSTR, L"string"},
		{VT_DISPATCH, L"dispatch"},
		{VT_ERROR, L"error"},
		{VT_BOOL, L"bool"},
		{VT_VARIANT, L"variant"},
		{VT_UNKNOWN, L"unknown"},
		{VT_DECIMAL, L"decimal"},
		{VT_I1, L"char"},
		{VT_UI1, L"uchar"},
		{VT_UI2, L"ushort"},
		{VT_UI4, L"uint"},
		{VT_I8, L"int64"},
		{VT_UI8, L"uint64"},
		{VT_INT, L"int"},
		{VT_UINT, L"uint"},
		{VT_VOID, L"void"},
		{VT_HRESULT, L"hresult"},
		{VT_PTR, L"ptr"},
		{VT_SAFEARRAY, L"safearray"},
		{VT_CARRAY, L"carray"},
		{VT_USERDEFINED, L"userdefined"},
		{VT_LPSTR, L"lpstr"},
		{VT_LPWSTR, L"lpwstr"},
		{VT_RECORD, L"record"},
		{VT_INT_PTR, L"intptr"},
		{VT_UINT_PTR, L"uintptr"},
		{VT_FILETIME, L"filetime"},
		{VT_BLOB, L"blob"},
		{VT_STREAM, L"stream"},
		{VT_STORAGE, L"storage"},
		{VT_STREAMED_OBJECT, L"streamedobject"},
		{VT_STORED_OBJECT, L"storedobject"},
		{VT_BLOB_OBJECT, L"blobobject"},
		{VT_CF, L"cf"},
		{VT_CLSID, L"clsid"},
		{VT_VERSIONED_STREAM, L"versionedstream"},
		{VT_BSTR_BLOB, L"bstrblob"},
		{VT_VECTOR, L"vector"},
		{VT_ARRAY, L"array"},
		{VT_BYREF, L"byref"},
		{VT_RESERVED, L"reserved"},
		{VT_ILLEGAL, L"illegal"},

	};

	CXX_D(const CXXComVariant);

	auto it = typeNames.find(static_cast<intptr_t>(d->result_.vt));
	if (it == typeNames.end())
	{
		return L"unknown";
	}

	return it->second;
}

//@别名 数据指针()
void* CXXComVariant::data()
{
	CXX_D(CXXComVariant);
	return reinterpret_cast<void*>(&d->result_);
}

CXXComObject::CXXComObject()
	: d_ptr(new CXXComObjectPrivate(this, CXX_NULLPTR))
{

}

CXXComObject::~CXXComObject()
{
	CXX_D(CXXComObject);
	if (d->obj_ != CXX_NULLPTR)
	{
		d->obj_->Release();
	}
}

bool CXXComObject::initialize(const CXXString& defaultObjName, cxx::ThreadApartmentType type)
{
	CXX_D(CXXComObject);

	if (d->obj_ != CXX_NULLPTR)
	{
		return false;
	}

	d->type_ = type;

	HRESULT hr = CoInitializeEx(CXX_NULLPTR, static_cast<DWORD>(d->type_));
	if (FAILED(hr))
	{
		return false;
	}

	hr = E_FAIL;

	CLSID clsid;
	IUnknown* pUnknown = CXX_NULLPTR;

	d->obj_ = CXX_NULLPTR;
	hr = ::CLSIDFromProgID(defaultObjName.data(), &clsid);
	if (FAILED(hr))
	{
		return false;
	}

	hr = ::CoCreateInstance(clsid, CXX_NULLPTR, CLSCTX_ALL, IID_IUnknown, (LPVOID*)&pUnknown);
	if (FAILED(hr))
	{
		return false;
	}

	pUnknown->QueryInterface(IID_IDispatch, (void**)&d->obj_);

	if (pUnknown)
	{
		pUnknown->Release();
	}

	if (d->obj_ == CXX_NULLPTR)
	{
		return false;
	}

	d_ptr.reset(new CXXComObjectPrivate(this, d->obj_));
	return true;
}

void CXXComObject::setProperty(const CXXString& propertyName, CXXComVariant value)
{
	CXX_D(CXXComObject);
	if (d == CXX_NULLPTR)
	{
		return;
	}

	_variant_t var;
	VariantInit(&var);
	std::ignore = VariantCopy(&var, reinterpret_cast<VARIANT*>(&value));
	d->setProperty(propertyName, var);
}

void CXXComObject::coInitHelper()
{
	// 初始化 COM
	HRESULT hr = CoInitializeEx(CXX_NULLPTR, COINIT_APARTMENTTHREADED);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to initialize COM library." << std::endl;
	}
}

void CXXComObject::coUninitHelper()
{
	// 反初始化 COM
	CoUninitialize();
}

bool CXXComObject::getInfoFromRegistry(const CXXString& libFileName, CXXVector<CXXString>* strCLSIDs, CXXVector<CXXString>* progIDs, CXXVector<CLSID>* CLSIDs)
{
	HKEY hKey = CXX_NULLPTR;
	LONG lResult = ERROR_SUCCESS;

	// 打开 HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID
	lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\CLSID", 0, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS)
	{
		std::wcerr << L"Failed to open CLSID key, error: " << GetLastError() << std::endl;
		return false;
	}

	DWORD index = 0;
	WCHAR clsidKeyName[256] = {};
	DWORD clsidKeyNameSize = sizeof(clsidKeyName) / sizeof(clsidKeyName[0]);

	// 枚举所有 CLSID 键
	while (RegEnumKeyExW(hKey, index++, clsidKeyName, &clsidKeyNameSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
	{
		HKEY hSubKey;
		std::wstring subKeyPath = L"SOFTWARE\\Classes\\CLSID\\" + std::wstring(clsidKeyName);

		// 尝试打开 InprocServer32 或 LocalServer32 子键
		lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, (subKeyPath + L"\\InprocServer32").c_str(), 0, KEY_READ, &hSubKey);
		if (lResult != ERROR_SUCCESS)
		{
			lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, (subKeyPath + L"\\LocalServer32").c_str(), 0, KEY_READ, &hSubKey);
			if (lResult != ERROR_SUCCESS)
			{
				clsidKeyNameSize = sizeof(clsidKeyName) / sizeof(clsidKeyName[0]);
				continue;  // 没有找到 InprocServer32 或 LocalServer32，继续下一个 CLSID
			}
		}

		// 获取 InprocServer32 或 LocalServer32 的默认值（DLL/EXE 路径）
		WCHAR serverPath[512] = {};
		DWORD serverPathSize = sizeof(serverPath);
		lResult = RegQueryValueExW(hSubKey, nullptr, nullptr, nullptr, (LPBYTE)serverPath, &serverPathSize);
		RegCloseKey(hSubKey);

		CXXString serverPathStr = serverPath;

		if (lResult == ERROR_SUCCESS && !serverPathStr.isEmpty())
		{
			// 比较路径与提供的文件名是否匹配
			if (serverPathStr.endsWith(libFileName))
			{
				// 匹配到相应的 CLSID
				if (strCLSIDs)
				{
					strCLSIDs->append(clsidKeyName);
				}

				// 获取 ProgID（如果存在）
				HKEY hProgIDKey;
				if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, (subKeyPath + L"\\ProgID").c_str(), 0, KEY_READ, &hProgIDKey) == ERROR_SUCCESS)
				{
					WCHAR progID[256] = {};
					DWORD progIDSize = sizeof(progID);
					if (RegQueryValueExW(hProgIDKey, nullptr, nullptr, nullptr, (LPBYTE)progID, &progIDSize) == ERROR_SUCCESS && progIDs)
					{
						progIDs->append(progID);
					}

					if (CLSIDs)
					{
						CLSID retClsid = {};
						if (SUCCEEDED(CLSIDFromString(clsidKeyName, &retClsid)))
						{
							CLSIDs->append(retClsid);
						}
					}

					RegCloseKey(hProgIDKey);
				}
			}
		}

		clsidKeyNameSize = sizeof(clsidKeyName) / sizeof(clsidKeyName[0]);
	}

	RegCloseKey(hKey);
	if (strCLSIDs->isEmpty())
	{
		std::wcerr << L"Failed to find matching CLSID for " << libFileName.toStdWString() << std::endl;
		return false;
	}

	return true;
}

bool CXXComObject::getInfo(const CXXString& libFileName, CXXVector<CXXString>* strCLSIDs, CXXVector<CXXString>* progIDs, CXXVector<CLSID>* CLSIDs)
{
	HMODULE hModule = LoadLibraryExW(libFileName.data(), CXX_NULLPTR, LOAD_LIBRARY_AS_DATAFILE);
	if (!hModule)
	{
		std::wcerr << L"Failed to load DLL." << GetLastError() << std::endl;
		return false;
	}

	// 1. 通过资源表查找 CLSID 和 ProgID
	WORD resourceId = 0;
	bool foundInResource = EnumResourceNamesW(
		hModule,
		L"REGISTRY",
		[](HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName, LONG_PTR lParam) -> BOOL
		{
			std::ignore = lpszType;
			if (!IS_INTRESOURCE(lpszName))
			{
				return TRUE;
			}

			*(WORD*)lParam = (WORD)reinterpret_cast<ULONG_PTR>(lpszName);

			CXXString str = CXXComObjectPrivate::loadRCRegistryString(hModule, *(WORD*)lParam);

			if (!str.contains(L"CLSID") || !str.contains(L"ProgID"))
			{
				return TRUE;
			}

			return FALSE;
		},
		reinterpret_cast<LONG_PTR>(&resourceId));

	if (foundInResource)
	{
		CXXString str = CXXComObjectPrivate::loadRCRegistryString(hModule, resourceId);

		// 提取 CLSID
		__int64 index = str.indexOf(L"CLSID = s '{");
		if (index != -1)
		{
			CXXString clsid = str.mid(index + 11, 38);
			if (strCLSIDs)
			{
				strCLSIDs->append(clsid);
			}

			if (CLSIDs)
			{
				CLSID retClsid = {};
				if (SUCCEEDED(CLSIDFromString(clsid.toStdWString().c_str(), &retClsid)))
				{
					CLSIDs->append(retClsid);
				}
			}
		}

		// 提取 ProgID
		index = str.indexOf(L"ProgID = s '");
		if (index != -1)
		{
			__int64 quote = str.indexOf(L"'", index + 12);
			if (quote != -1)
			{
				CXXString progid = str.mid(index + 12, quote - index - 12);
				if (progIDs != CXX_NULLPTR)
				{
					progIDs->append(progid);
				}
			}
		}

		FreeLibrary(hModule);
		return true;
	}

	// 2. 如果通过资源表查找失败，则通过注册表查找
	FreeLibrary(hModule);

	if (strCLSIDs || progIDs)
	{
		if (getInfoFromRegistry(libFileName, strCLSIDs, progIDs, CLSIDs))
		{
			return true;
		}
	}

	hModule = LoadLibraryW(libFileName.data());
	if (!hModule)
	{
		std::wcerr << L"Failed to load DLL." << GetLastError() << std::endl;
		return false;
	}

	typedef HRESULT(__stdcall* DllRegisterServerFunc)();
	typedef HRESULT(__stdcall* DllGetClassObjectFunc)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

	// 尝试调用 DllRegisterServer 来获取 COM 组件的自注册信息
	DllRegisterServerFunc dllRegisterServer = (DllRegisterServerFunc)GetProcAddress(hModule, "DllRegisterServer");
	if (dllRegisterServer)
	{
		HRESULT hr = dllRegisterServer();
		if (SUCCEEDED(hr))
		{
			std::wcout << L"DllRegisterServer 调用成功，组件可能已经注册。" << std::endl;
			FreeLibrary(hModule);
			if (getInfoFromRegistry(libFileName, strCLSIDs, progIDs, CLSIDs))
			{
				return true;
			}
			return true;
		}
		else
		{
			std::wcerr << L"DllRegisterServer 调用失败，HRESULT: " << hr << std::endl;
		}
	}
	else
	{
		std::wcerr << L"DllRegisterServer 函数不存在。" << std::endl;
	}

	FreeLibrary(hModule);

	std::wcerr << L"No manifest resource found and no registry entry found." << std::endl;
	return false;
}


CComVariant CXXComObject::toVariant(const wchar_t* arg)
{
	CComVariant var;
	var.vt = VT_BSTR;
	var.bstrVal = SysAllocString(arg);
	return var;
}

CComVariant CXXComObject::toVariant(VARIANT* arg)
{
	CComVariant var;
	var.vt = VT_BYREF | VT_VARIANT;
	var.pvarVal = arg;
	return var;
}



CXXString CXXComObjectPrivate::loadRCRegistryString(HMODULE hModule, WORD resourceId)
{
	HRSRC hRes = FindResourceW(hModule, MAKEINTRESOURCEW(resourceId), L"REGISTRY");
	if (!hRes)
	{
		return CXXString();
	}

	HGLOBAL hMem = LoadResource(hModule, hRes);
	if (!hMem)
	{
		return CXXString();
	}

	DWORD dwSize = SizeofResource(hModule, hRes);
	if (0 == dwSize)
	{
		return CXXString();
	}

	CXXString str = CXXString::fromLocal8Bit(reinterpret_cast<const char*>(LockResource(hMem)));

	FreeResource(hMem);

	return str;
}

std::mutex& CXXComObject::mapMutexRef()
{
	CXX_D(CXXComObject);
	return d->mapMutex_;
}

std::unordered_map<std::wstring, DISPID>& CXXComObject::dispidMapRef()
{
	CXX_D(CXXComObject);
	return d->dispidMap_;
}

IDispatch* CXXComObject::objectRef()
{
	CXX_D(CXXComObject);
	return d->obj_;
}

HRESULT CXXComObject::getIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
{
	CXX_D(CXXComObject);
	return d->getIDOfName(lpsz, pdispid);
}

HRESULT CXXComObject::invoke(DISPID dispid, VARIANT* pvarRet)
{
	CXX_D(CXXComObject);
	return d->invoke(dispid, pvarRet);

}
HRESULT CXXComObject::invoke(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet)
{
	CXX_D(CXXComObject);
	return d->invoke(dispid, pvarParams, nParams, pvarRet);
}