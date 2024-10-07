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
#ifndef CXXVARIANT_H_
//@隐藏{
#define CXXVARIANT_H_
//@隐藏}



#include <windows.h>
#include <iostream>

#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

//@别名 炫
namespace cxx
{
	//@分组{［容器］.炫　通用容器类

	//@别名 数据类型
	enum DataType
		//@隐藏{
		: __int64
		//@隐藏}
	{
		UnknownType,		//@别名 未知类型
		Bool,				//@别名 逻辑型
		Int,				//@别名 整型
		UInt,				//@别名 正整型
		LongLong,			//@别名 长整型
		ULongLong,			//@别名 正长整型
		Double,				//@别名 双浮点型
		Short,				//@别名 短整型
		Char,				//@别名 字符型A
		UShort,				//@别名 正短整型
		UChar,				//@别名 字节型
		Float,				//@别名 浮点型
		VoidStar,			//@别名 指针型
		WString,			//@别名 宽文本型
		Point,				//@别名 坐标
		PointF,				//@别名 双浮点坐标
		Void,				//@别名 空类型
		Nullptr,			//@别名 空指针
		User = 1024			//@别名 自定义类型
	};

	//@分组}

} // namespace cxx

//@分组{［容器］.炫　通用容器类

//@隐藏{
class CXXVariantPrivate;
//@隐藏}

//@别名 炫通用型
//@备註 万能类型的变量基本常用类型、文本、自订类、结构体都可以保存
// 支持转换成其他类型输出(不保证能成功，必须是合理的转换比如文本纯数字转整型)
class CXXVariant
{
public:
	//@参数 无
	CXXVariant();

	//@参数 other 另一个CXXVariant
	CXXVariant(const CXXVariant& other);

	//@参数 other 另一个CXXVariant
	CXXVariant(CXXVariant&& other) noexcept;

	//move assignment
	CXXVariant& operator=(CXXVariant&& other) noexcept;

	//copy assignment
	CXXVariant& operator=(const CXXVariant& other);

	//@参数 value 逻辑型
	CXXVariant(bool value);

	//@参数 value 字符型A
	CXXVariant(char value);

	//@参数 value 字节型
	CXXVariant(BYTE value);

	//@参数 value 短整型
	CXXVariant(short value);

	//@参数 value 字符型
	CXXVariant(wchar_t value);

	//@参数 value 正短整型
	CXXVariant(USHORT value);

	//@参数 value 整型
	CXXVariant(int value);

	//@参数 value 正整型
	CXXVariant(UINT value);

	//@参数 value 长整型
	CXXVariant(__int64 value);

	//@参数 value 正长整型
	CXXVariant(UINT64 value);

	//@参数 value 浮点型
	CXXVariant(float value);

	//@参数 value 双浮点型
	CXXVariant(double value);

	//@参数 value 常量字符指针型
	CXXVariant(const wchar_t* value);

	//@参数 value 字符指针型
	CXXVariant(wchar_t* value);

	//@参数 value 常量字符A指针型
	CXXVariant(const char* value);

	//@参数 value 字符A指针型
	CXXVariant(char* value);

	//@参数 value 炫文本型
	CXXVariant(const CXXString& value);

	//@参数 type 数据类型
	//@参数 copy 数据指针
	//@参数 size 数据大小
	CXXVariant(cxx::DataType type, const void* copy = CXX_NULLPTR, size_t size = 0);

	//@参数 value 模板指针型
	template <typename T>
	CXXVariant(T* value)
	{
		reset(this, cxx::DataType::VoidStar);
		if (value == CXX_NULLPTR)
		{
			dataRef() = CXX_NULLPTR;
			typeRef() = cxx::DataType::Nullptr;
			return;
		}

		dataRef() = reinterpret_cast<void*>(value);
	}

	//@参数 value 模板型
	template <typename T>
	CXXVariant(T value)
	{
		reset(this, cxx::DataType::User);
		dataRef() = new T(value);
		sizeRef() = sizeof(T);
	}

	virtual ~CXXVariant();

	//@备註 检查是否可以被转换至指定的数据类型
	//@别名 是否可至(数据类型)
	bool canConvert(cxx::DataType targetType) const;

	//@备註 模板转换至指定的数据类型  炫通用型::到<类型>()
	//@别名 值()
	template <typename T>
	T value() const { CXX_ASSERT_X(false, L"Unsupported type conversion."); }

	//@隐藏{
	template <>
	bool value<bool>() const;

	template <>
	char value<char>() const;

	template <>
	BYTE value<BYTE>() const;

	template <>
	short value<short>() const;

	template <>
	USHORT value<USHORT>() const;

	template <>
	int value<int>() const;

	template <>
	UINT value<UINT>() const;

	template <>
	__int64 value<__int64>() const;

	template <>
	UINT64 value<UINT64>() const;

	template <>
	float value<float>() const;

	template <>
	double value<double>() const;

	template <>
	CXXString value<CXXString>() const;

	template <>
	void* value<void*>() const;

	friend std::wostream& operator<<(std::wostream& os, const CXXVariant& var)
	{
		os << L"CXXVariant(" << var.typeName() << L", " << var.value<CXXString>() << L")";
		return os;
	}
	//@隐藏} 

	//@备註 获取用户自定义类型的大小
	//@别名 大小()
	size_t userSize() const;

	//@备註 获取数据指针
	//@别名 地址()
	void* data() const;

	//@备註 检查是否为空
	//@别名 是否空()
	bool isNull() const;

	//@备註 获取数据类型名称
	//@别名 类型名()
	CXXString typeName() const;

	//@备註 获取数据类型
	//@别名 类型()
	cxx::DataType type() const;

	//@备註 转到换逻辑
	//@别名 到逻辑()
	bool toBool() const;

	//@备註 转换到有符号8位整数
	//@别名 到字符A()
	char toChar() const;

	//@备註 转换到无符号8位整数
	//@别名 到字节()
	BYTE toUChar() const;

	//@备註 转换到有符号16位整数
	//@别名 到短整型()
	short toShort() const;

	//@备註 转换到无符号16位整数
	//@别名 到正短整型()
	USHORT toUShort() const;

	//@备註 转换有符号32位整数
	//@别名 到整型()
	int toInt() const;

	//@备註 转换到无符号32位整数
	//@别名 到正整型()
	UINT toUInt() const;

	//@备註 转换到有符号64位整数
	//@别名 到长整型()
	__int64 toLongLong() const;

	//@备註 转换到无符号64位整数
	//@别名 到正长整型()
	UINT64 toULongLong() const;

	//@备註 转换到单精度浮点数
	//@别名 到浮点()
	float toFloat() const;

	//@备註 转换到双精度浮点数
	//@别名 到双浮点()
	double toDouble() const;

	//@备註 转换到宽字符串
	//@别名 到文本()
	CXXString toString() const;

	//@备註 转换到指针
	//@别名 到指针()
	void* toVoidStar() const;

	//@隐藏{
	template <typename T>
	T* toPointer() const { return value<T>(); }

	bool operator==(const CXXVariant& other) const;

	bool operator!=(const CXXVariant& other) const;
	//@隐藏}

private:
	void*& dataRef();
	void reset(CXXVariant* q, cxx::DataType type);
	cxx::DataType& typeRef();
	size_t& sizeRef();

private:
	CXX_DECLARE_PRIVATE(CXXVariant);
	CXXScopedPointer<CXXVariantPrivate> d_ptr;
};

//@分组}

#endif // CXXVARIANT_H_