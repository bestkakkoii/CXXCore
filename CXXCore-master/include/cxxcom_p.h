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
#ifndef CXXCOM_P_H_
#define CXXCOM_P_H_

#include <cxxcom.h>
#include <cxxatomic.h>

#include <windows.h>
#include <oaidl.h>
#include <atlcomcli.h>
#include <atlbase.h>
#include <objbase.h>
#include <comdef.h>
#include <unordered_map>
#include <mutex>
#include <atomic>

class CXXComVariantPrivate
{
public:
	explicit CXXComVariantPrivate(CXXComVariant* parent);
	CXXComVariantPrivate(CXXComVariant* parent, const VARIANT& result);

	virtual ~CXXComVariantPrivate();

private:
	std::atomic_bool isValid_ = true;
	VARIANT result_;

private:
	CXX_DECLARE_PUBLIC(CXXComVariant);
	CXXComVariant* q_ptr = CXX_NULLPTR;
};

class CXXComObjectPrivate
{
public:
	CXXComObjectPrivate(CXXComObject* parent, IDispatch* lp);

	virtual ~CXXComObjectPrivate();

	HRESULT getIDOfName(LPCOLESTR lpsz, DISPID* pdispid);

	HRESULT invoke(DISPID dispid, VARIANT* pvarRet);

	HRESULT invoke(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet);

	HRESULT setProperty(const CXXString& propertyName, const _variant_t& value);

private:
	static CXXString loadRCRegistryString(HMODULE hModule, WORD resourceId);

private:
	IDispatch* p = CXX_NULLPTR;
	IDispatch* obj_ = CXX_NULLPTR;

	cxx::ThreadApartmentType type_ = cxx::STA;

	std::unordered_map<std::wstring, DISPID> dispidMap_;
	std::mutex mapMutex_;

private:
	CXX_DECLARE_PUBLIC(CXXComObject);
	CXXComObject* q_ptr;
};

#endif // CXXCOM_P_H_