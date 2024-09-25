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
#ifndef CXXVARIANT_P_H_
#define CXXVARIANT_P_H_

#include <cxxvariant.h>
#include <cxxstring.h>

#include <windows.h>

class CXXVariantPrivate
{
public:
	explicit CXXVariantPrivate(CXXVariant* q, cxx::DataType type);
	virtual ~CXXVariantPrivate();

public:
	void clear();

	void copyFrom(const CXXVariant& other);

	void moveFrom(CXXVariant&& other);

	bool compareFrom(const CXXVariant& other) const;


private:
	union Data
	{
		char charVal;
		BYTE ucharVal;
		short shortVal;
		USHORT ushortVal;
		int intVal;
		UINT uintVal;
		__int64 int64Val;
		UINT64 uint64Val;
		float floatVal;
		double doubleVal;
		CXXString* wstringVal;
		void* voidPtrVal;

	} data_ = {};

	cxx::DataType type_ = cxx::UnknownType;
	size_t userSize_ = 0;

private:
	CXX_DECLARE_PUBLIC(CXXVariant);
	CXXVariant* q_ptr = CXX_NULLPTR;

};



#endif // CXXVARIANT_P_H_