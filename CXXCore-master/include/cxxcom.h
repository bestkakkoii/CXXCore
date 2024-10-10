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
#ifndef CXXCOM_H_
//@隐藏{
#define CXXCOM_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxatomic.h>
#include <cxxscopedpointer.h>

#include <windows.h>
#include <atlbase.h>
#include <objbase.h>
#include <unordered_map>
#include <mutex>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <atlcomcli.h>
#include <atlbase.h>
#include <comdef.h>
#include <oaidl.h>
#include <objbase.h>

//@别名 炫
namespace cxx
{

}

//@分组{［其他］.炫　ＣＯＭ组件类

//@隐藏{
class CXXComVariantPrivate;
class CXXComObject;
//@隐藏}

//@别名 炫组件变体型
class CXXComVariant
{
public:
	CXXComVariant();

	virtual ~CXXComVariant();

	CXXComVariant(const VARIANT& result);

	CXXComVariant(const CXXString& str);

	CXXComVariant(const wchar_t* str);

	CXXComVariant(const char* str);

	CXXComVariant(bool b);

	CXXComVariant(double d);

	CXXComVariant(float f);

	CXXComVariant(char i);

	CXXComVariant(short i);

	CXXComVariant(int i);

	CXXComVariant(__int64 i);

	CXXComVariant(BYTE i);

	CXXComVariant(USHORT i);

	CXXComVariant(UINT i);

	CXXComVariant(UINT64 i);

	//@别名 是否空()
	bool isNull() const;

	//@别名 清空()
	void clear();

	//@别名 到整型()
	int toInt() const;

	//@别名 到长整型()
	__int64 toInt64() const;

	//@别名 到双浮点()
	double toDouble() const;

	//@别名 到文本()
	CXXString toString() const;

	//@别名 到逻辑()
	bool toBool() const;

	//@别名 到分派指针()
	IDispatch* toDispatch() const;

	//@别名 类型名()
	CXXString typeName() const;

	//@别名 地址()
	void* data();

	//@别名 取()
	void* get();

	//@隐藏{
	template<typename... Args>
	CXXComVariant invoke(const CXXString& methodName, Args&&... args)
	{
		IDispatch* pDispatch = toDispatch();
		if (!pDispatch)
		{
			std::wcerr << L"Failed: no IDispatch available." << std::endl;
			return CXXComVariant();
		}

		DISPID dispid;
		std::wstring methodNameStr = methodName.toStdWString();  // 将 methodName 转换为 std::wstring
		LPOLESTR methodNameOLE = const_cast<LPOLESTR>(methodNameStr.c_str());  // 获取 LPOLESTR
		HRESULT hr = pDispatch->GetIDsOfNames(IID_NULL, &methodNameOLE, 1, LOCALE_USER_DEFAULT, &dispid);
		if (FAILED(hr))
		{
			std::wcerr << L"Failed to get DISPID for " << methodName.toStdWString() << std::endl;
			return CXXComVariant();
		}

		std::vector<CComVariant> varArgs = CXXComObject::prepareArguments(std::forward<Args>(args)...);
		std::reverse(varArgs.begin(), varArgs.end());

		DISPPARAMS dispparams = { CXX_NULLPTR, CXX_NULLPTR, 0, 0 };
		dispparams.cArgs = static_cast<UINT>(varArgs.size());
		dispparams.rgvarg = varArgs.size() > 0 ? &varArgs[0] : CXX_NULLPTR;

		VARIANT result;
		VariantInit(&result);
		hr = pDispatch->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispparams, &result, CXX_NULLPTR, CXX_NULLPTR);
		if (FAILED(hr))
		{
			std::wcerr << L"Failed to invoke " << methodName.toStdWString() << std::endl;
			return CXXComVariant();
		}

		return CXXComVariant(result);
	}
	//@隐藏}
	/*@声明
	template<typename Args>
	CXXComVariant invoke(const CXXString& methodName, Args args);
	*/
private:
	friend class CXXComObject;

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXComVariant);
	CXXScopedPointer<CXXComVariantPrivate> d_ptr;
	//@隐藏}
};

//@隐藏{
class CXXComObjectPrivate;
//@隐藏}

//@别名 炫组件对象
class CXXComObject
{
public:
	//@隐藏{
	CXXComObject();

	virtual ~CXXComObject();
	//@隐藏}

	//@参数 COM对象名称 比如:dm.dmsoft
	//@参数 COM线程套间类型
	//@别名 创建(默认对象名称)
	bool initialize(const CXXString& defaultObjName, cxx::ThreadApartmentType type = cxx::STA);

	//@隐藏{
	template<typename... Args>
	CXXComVariant invoke(const CXXString& methodName, Args&&... args)
	{
		VARIANT result = {};
		VariantInit(&result);

		DISPID dispid;

		{
			std::lock_guard<std::mutex> lock(mapMutexRef());
			std::wstring methodNameStr = methodName.toStdWString();
			CXX_AUTO it = dispidMapRef().find(methodNameStr);
			if (it != dispidMapRef().end())
			{
				dispid = it->second;
			}
			else
			{
				HRESULT hr = getIDOfName(methodName.c_str(), &dispid);
				if (FAILED(hr))
				{
					std::wcerr << L"Failed to get dispid of " << methodName.toStdWString() << std::endl;
					return CXXComVariant(result);
				}

				dispidMapRef().insert(std::pair<std::wstring, DISPID>(methodNameStr, dispid));
			}
		}

		std::vector<CComVariant> varArgs = prepareArguments(std::forward<Args>(args)...);
		std::reverse(varArgs.begin(), varArgs.end());

		DISPPARAMS dispparams = { CXX_NULLPTR, CXX_NULLPTR, 0, 0 };
		dispparams.cArgs = static_cast<UINT>(varArgs.size());
		dispparams.rgvarg = varArgs.size() > 0 ? &varArgs[0] : CXX_NULLPTR;

		HRESULT hr = invoke(dispid, dispparams.rgvarg, static_cast<int>(varArgs.size()), &result);
		if (FAILED(hr))
		{
			std::wcerr << L"Failed to invoke " << methodName.toStdWString() << std::endl;
		}
		return CXXComVariant(result);
	}
	//@隐藏}

	/*@声明
	//@备注 根据COM组件的方法(函数)名称调用
	//@参数 方法名称
	//@参数 其他参数...
	//@返回 COM变体型 数值结果
	//@别名 通用方法(方法名称)
	template<typename Args>
	CXXComVariant invoke(const string& methodName, Args args);
	*/

	//@备注 设置对象变量值
	//@参数 属性名称
	//@参数 属性值
	//@别名 置属性(属性名称, 属性值)
	void setProperty(const CXXString& propertyName, CXXComVariant value);


	//@备注 获取COM组件信息(如CLSID, ProgID等)
	//@参数 插件文件名(相对或绝对路径)
	//@参数 接收CLSID的炫文本指针
	//@参数 接收ProgID的炫文本指针 (选填)
	//@参数 接收CLSID的指针 (选填)
	//@返回 COM变体型 数值结果
	//@别名 取信息(库文件名, 接收CLSID的炫文本指针, 接收ProgID的炫文本指针, 接收CLSID的指针)
	static bool getInfo(const CXXString& libFileName, CXXVector<CXXString>* strCLSIDs, CXXVector<CXXString>* progIDs, CXXVector<CLSID>* CLSIDs = {});

private:
	//@隐藏{
	static bool getInfoFromRegistry(const CXXString& libFileName, CXXVector<CXXString>* strCLSIDs, CXXVector<CXXString>* progIDs, CXXVector<CLSID>* CLSIDs);

	static void coInitHelper();

	static void coUninitHelper();

	static CComVariant toVariant(const wchar_t* arg);

	static CComVariant toVariant(VARIANT* arg);

	template<typename T>
	static CComVariant toVariant(T&& arg)
	{
		return CComVariant(std::forward<T>(arg));
	}

	static std::vector<CComVariant> prepareArguments() { return {}; }

	template<typename T, typename... Args>
	static std::vector<CComVariant> prepareArguments(T&& first, Args&&... rest)
	{
		std::vector<CComVariant> args = { toVariant(std::forward<T>(first)) };
		std::vector<CComVariant> restArgs = prepareArguments(std::forward<Args>(rest)...);
		args.insert(args.end(), restArgs.begin(), restArgs.end());
		return args;
	}

	std::mutex& mapMutexRef();
	std::unordered_map<std::wstring, DISPID>& dispidMapRef();
	IDispatch* objectRef();
	HRESULT getIDOfName(LPCOLESTR lpsz, DISPID* pdispid);
	HRESULT invoke(DISPID dispid, VARIANT* pvarRet);
	HRESULT invoke(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet);
	//@隐藏}
private:
	//@隐藏{
	friend class CXXComVariant;
	CXX_DECLARE_PRIVATE(CXXComObject);
	CXX_DISABLE_COPY(CXXComObject);

	CXXScopedPointer<CXXComObjectPrivate> d_ptr;

	//@隐藏}
};


//@分组}

#endif // CXXCOM_H_