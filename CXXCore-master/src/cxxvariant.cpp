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

#include <cxxvariant_p.h>

#include <string.h>
#include <iomanip>
#include <map>
#include <sstream>

#pragma region Private
CXXVariantPrivate::CXXVariantPrivate(CXXVariant* q, cxx::DataType type)
	: q_ptr(q)
	, type_(type)
{
}

CXXVariantPrivate::~CXXVariantPrivate()
{
	if (type_ == cxx::WString)
	{
		delete data_.wstringVal;
	}
}

void CXXVariantPrivate::clear()
{
	switch (type_)
	{
	case cxx::DataType::WString:
	{
		CXX_ASSERT_X(data_.wstringVal != CXX_NULLPTR, L"Invalid wstring pointer.");
		if (data_.wstringVal != CXX_NULLPTR)
		{
			delete data_.wstringVal;
		}

		break;
	}
	case cxx::DataType::User:
	{
		CXX_ASSERT_X(data_.voidPtrVal != CXX_NULLPTR, L"Invalid user pointer.");
		if (data_.voidPtrVal != CXX_NULLPTR)
		{
			delete data_.voidPtrVal;
		}

		userSize_ = 0;
		break;
	}
	}

	type_ = cxx::DataType::UnknownType;
}

void CXXVariantPrivate::copyFrom(const CXXVariant& other)
{
	switch (other.d_ptr->type_)
	{
	case cxx::DataType::WString:
	{
		data_.wstringVal = new CXXString(*other.d_ptr->data_.wstringVal);
		break;
	}
	default:
	{
		data_ = other.d_ptr->data_;
		break;
	}
	}
}

void CXXVariantPrivate::moveFrom(CXXVariant&& other)
{
	// 根据 other 的 type_ 和 data_ 进行移动
	// 例如：
	switch (other.d_ptr->type_)
	{
	case cxx::DataType::WString:
	{
		data_.wstringVal = other.d_ptr->data_.wstringVal;
		other.d_ptr->clear();
		break;
	}
	default:
	{
		data_ = other.d_ptr->data_;
		break;
	}
	}

	type_ = other.d_ptr->type_;
	other.d_ptr->type_ = cxx::DataType::UnknownType;
}

bool CXXVariantPrivate::compareFrom(const CXXVariant& other) const
{
	if (type_ == cxx::DataType::WString && other.d_ptr->type_ == cxx::DataType::WString)
	{
		if (data_.wstringVal == CXX_NULLPTR || other.d_ptr->data_.wstringVal == CXX_NULLPTR)
		{
			return false;
		}
		return *data_.wstringVal == *other.d_ptr->data_.wstringVal;
	}
	else if (type_ == cxx::DataType::Nullptr && other.d_ptr->type_ == cxx::DataType::Nullptr)
	{
		return true;
	}
	else if (type_ == cxx::DataType::VoidStar && other.d_ptr->type_ == cxx::DataType::VoidStar)
	{
		return data_.voidPtrVal == other.d_ptr->data_.voidPtrVal;
	}

	auto convertToReal = [](const CXXVariant& v, double* preal) -> bool
		{
			if (preal == CXX_NULLPTR)
			{
				return false;
			}

			switch (v.d_ptr->type_)
			{
			case cxx::DataType::Int:
			{
				*preal = static_cast<double>(v.d_ptr->data_.intVal);
				return true;
			}
			case cxx::DataType::UInt:
			{
				*preal = static_cast<double>(v.d_ptr->data_.uintVal);
				return true;
			}
			case cxx::DataType::LongLong:
			{
				*preal = static_cast<double>(v.d_ptr->data_.int64Val);
				return true;
			}
			case cxx::DataType::ULongLong:
			{
				*preal = static_cast<double>(v.d_ptr->data_.uint64Val);
				return true;
			}
			case cxx::DataType::Short:
			{
				*preal = static_cast<double>(v.d_ptr->data_.shortVal);
				return true;
			}
			case cxx::DataType::UShort:
			{
				*preal = static_cast<double>(v.d_ptr->data_.ushortVal);
				return true;
			}
			case cxx::DataType::Char:
			{
				*preal = static_cast<double>(v.d_ptr->data_.charVal);
				return true;
			}
			case cxx::DataType::UChar:
			{
				*preal = static_cast<double>(v.d_ptr->data_.ucharVal);
				return true;
			}
			case cxx::DataType::Float:
			{
				*preal = static_cast<double>(v.d_ptr->data_.floatVal);
				return true;
			}
			case cxx::DataType::Double:
			{
				*preal = v.d_ptr->data_.doubleVal;
				return true;
			}
			default:
			{
				break;
			}
			}
			return false;
		};

	double left = 0;
	double right = 0;
	return convertToReal(*this, &left) && convertToReal(other, &right) && left == right;
}

#pragma endregion


CXXVariant::CXXVariant()
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::UnknownType))
{
	CXX_D(CXXVariant);
	d->data_.voidPtrVal = CXX_NULLPTR;
}

CXXVariant::CXXVariant(const CXXVariant& other)
{
	CXX_D(CXXVariant);
	d->copyFrom(other);
}

CXXVariant::CXXVariant(CXXVariant&& other) noexcept
{
	CXX_D(CXXVariant);
	d->moveFrom(std::move(other));
}

//move assignment
CXXVariant& CXXVariant::operator=(CXXVariant&& other) noexcept
{
	if (this != &other)
	{
		CXX_D(CXXVariant);
		d->clear();
		d->moveFrom(std::move(other));
	}
	return *this;
}

//copy assignment
CXXVariant& CXXVariant::operator=(const CXXVariant& other)
{
	if (this != &other)
	{
		CXX_D(CXXVariant);
		d->clear();
		d->copyFrom(other);
	}
	return *this;
}

CXXVariant::CXXVariant(bool value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::Bool))
{
	CXX_D(CXXVariant);
	d->data_.charVal = value;
}

CXXVariant::CXXVariant(char value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::Char))
{
	CXX_D(CXXVariant);
	d->data_.charVal = value;
}

CXXVariant::CXXVariant(BYTE value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::UChar))
{
	CXX_D(CXXVariant);
	d->data_.ucharVal = value;
}

CXXVariant::CXXVariant(short value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::Short))
{
	CXX_D(CXXVariant);
	d->data_.shortVal = value;
}

CXXVariant::CXXVariant(wchar_t value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::Short))
{
	CXX_D(CXXVariant);
	d->data_.shortVal = value;
}

CXXVariant::CXXVariant(USHORT value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::UShort))
{
	CXX_D(CXXVariant);
	d->data_.ushortVal = value;
}

CXXVariant::CXXVariant(int value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::Int))
{
	CXX_D(CXXVariant);
	d->data_.intVal = value;
}

CXXVariant::CXXVariant(UINT value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::UInt))
{
	CXX_D(CXXVariant);
	d->data_.uintVal = value;
}

CXXVariant::CXXVariant(__int64 value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::LongLong))
{
	CXX_D(CXXVariant);
	d->data_.int64Val = value;
}

CXXVariant::CXXVariant(UINT64 value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::ULongLong))
{
	CXX_D(CXXVariant);
	d->data_.uint64Val = value;
}

CXXVariant::CXXVariant(float value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::Float))
{
	CXX_D(CXXVariant);
	d->data_.floatVal = value;
}

CXXVariant::CXXVariant(double value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::Double))
{
	CXX_D(CXXVariant);
	d->data_.doubleVal = value;
}

CXXVariant::CXXVariant(const wchar_t* value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::WString))
{
	CXX_D(CXXVariant);

	CXX_ASSERT_X(value != CXX_NULLPTR, L"Invalid wstring pointer.");
	d->data_.wstringVal = new CXXString(value);
	CXX_ASSERT_X(d->data_.wstringVal != CXX_NULLPTR, L"Failed to allocate memory for wchar_t*.");
}

CXXVariant::CXXVariant(wchar_t* value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::WString))
{
	CXX_D(CXXVariant);

	CXX_ASSERT_X(value != CXX_NULLPTR, L"Invalid wstring pointer.");
	d->data_.wstringVal = new CXXString(value);
	CXX_ASSERT_X(d->data_.wstringVal != CXX_NULLPTR, L"Failed to allocate memory for wchar_t*.");
}

CXXVariant::CXXVariant(const char* value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::WString))
{
	CXX_D(CXXVariant);

	CXX_ASSERT_X(value != CXX_NULLPTR, L"Invalid string pointer.");
	d->data_.wstringVal = new CXXString(CXXString::fromLocal8Bit(value));
	CXX_ASSERT_X(d->data_.wstringVal != CXX_NULLPTR, L"Failed to allocate memory for char*.");
}

CXXVariant::CXXVariant(char* value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::WString))
{
	CXX_D(CXXVariant);

	CXX_ASSERT_X(value != CXX_NULLPTR, L"Invalid string pointer.");
	d->data_.wstringVal = new CXXString(CXXString::fromLocal8Bit(value));
	CXX_ASSERT_X(d->data_.wstringVal != CXX_NULLPTR, L"Failed to allocate memory for char*.");
}

CXXVariant::CXXVariant(cxx::DataType type, const void* copy, size_t size)
	: d_ptr(new CXXVariantPrivate(this, type))
{
	CXX_D(CXXVariant);

	switch (type)
	{
	case cxx::DataType::WString:
	{
		d->data_.wstringVal = new CXXString(*static_cast<const CXXString*>(copy));
		break;
	}
	default:
	{
		d->data_.voidPtrVal = new BYTE[size];
		memmove_s(d->data_.voidPtrVal, size, copy, size);
		break;
	}
	}
}

CXXVariant::CXXVariant(const CXXString& value)
	: d_ptr(new CXXVariantPrivate(this, cxx::DataType::WString))
{
	CXX_D(CXXVariant);
	d->data_.wstringVal = new CXXString(value);
}

void*& CXXVariant::dataRef()
{
	CXX_D(CXXVariant);
	return d->data_.voidPtrVal;
}

void CXXVariant::reset(CXXVariant* q, cxx::DataType type)
{
	d_ptr.reset(new CXXVariantPrivate(q, type));
	CXX_D(CXXVariant);
	d->type_ = type;
	d->data_.voidPtrVal = CXX_NULLPTR;
}

cxx::DataType& CXXVariant::typeRef()
{
	CXX_D(CXXVariant);
	return d->type_;
}

size_t& CXXVariant::sizeRef()
{
	CXX_D(CXXVariant);
	return d->userSize_;
}

CXXVariant::~CXXVariant()
{
	CXX_D(CXXVariant);

	d->clear();
}

bool CXXVariant::canConvert(cxx::DataType targetType) const
{
	CXX_D(const CXXVariant);

	switch (d->type_)
	{
	case cxx::DataType::WString:
	{
		return targetType == cxx::DataType::WString;
	}
	case cxx::DataType::VoidStar:
	{
		return targetType == cxx::DataType::Nullptr;
	}
	case cxx::DataType::Nullptr:
	{
		return targetType == cxx::DataType::VoidStar;
	}
	case cxx::DataType::Bool:
	case cxx::DataType::Int:
	case cxx::DataType::UInt:
	case cxx::DataType::LongLong:
	case cxx::DataType::ULongLong:
	case cxx::DataType::Short:
	case cxx::DataType::Char:
	case cxx::DataType::UShort:
	case cxx::DataType::UChar:
	case cxx::DataType::Float:
	case cxx::DataType::Double:
	{
		return true;
	}
	case cxx::DataType::User:
	{
		return targetType == cxx::DataType::User;
	}
	default:
	{
		break;
	}
	}

	return false;
}

size_t CXXVariant::userSize() const
{
	CXX_D(const CXXVariant);
	return d->userSize_;
}

void* CXXVariant::data() const
{
	CXX_D(const CXXVariant);
	return d->data_.voidPtrVal;
}

bool CXXVariant::isNull() const
{
	CXX_D(const CXXVariant);
	return d->type_ == cxx::DataType::UnknownType || d->type_ == cxx::DataType::Nullptr;
}

CXXString CXXVariant::typeName() const
{
	CXX_D(const CXXVariant);

	static std::map<cxx::DataType, std::wstring> names;
	if (static_cast<__int64>(names.size()) <= 0)
	{
		names[cxx::DataType::UnknownType] = L"unknown";
		names[cxx::DataType::Bool] = L"bool";
		names[cxx::DataType::Int] = L"int";
		names[cxx::DataType::UInt] = L"uint";
		names[cxx::DataType::LongLong] = L"longlong";
		names[cxx::DataType::ULongLong] = L"ulonglong";
		names[cxx::DataType::Double] = L"double";
		names[cxx::DataType::Short] = L"short";
		names[cxx::DataType::Char] = L"char";
		names[cxx::DataType::UShort] = L"ushort";
		names[cxx::DataType::UChar] = L"uchar";
		names[cxx::DataType::Float] = L"float";
		names[cxx::DataType::VoidStar] = L"pointer";
		names[cxx::DataType::WString] = L"string";
		names[cxx::DataType::Point] = L"point";
		names[cxx::DataType::PointF] = L"pointF";
		names[cxx::DataType::Void] = L"void";
		names[cxx::DataType::Nullptr] = L"nullptr";
		names[cxx::DataType::User] = L"user";
	}

	if ((int)d->type_ <= 0 || (int)d->type_ > (int)cxx::DataType::User)
	{
		return L"unknown";
	}

	return names[d->type_];
}

cxx::DataType CXXVariant::type() const
{
	CXX_D(const CXXVariant);
	return d->type_;
}

bool CXXVariant::toBool() const { return value<bool>(); }

char CXXVariant::toChar() const { return value<char>(); }

BYTE CXXVariant::toUChar() const { return value<BYTE>(); }

short CXXVariant::toShort() const { return value<short>(); }

USHORT CXXVariant::toUShort() const { return value<USHORT>(); }

int CXXVariant::toInt() const { return value<int>(); }

UINT CXXVariant::toUInt() const { return value<UINT>(); }

__int64 CXXVariant::toLongLong() const { return value<__int64>(); }

UINT64 CXXVariant::toULongLong() const { return value<UINT64>(); }

float CXXVariant::toFloat() const { return value<float>(); }

double CXXVariant::toDouble() const { return value<double>(); }

CXXString CXXVariant::toString() const { return value<CXXString>(); }

//@备註 转换到指针
//@别名 到指针()
void* CXXVariant::toVoidStar() const { return value<void*>(); }

bool CXXVariant::operator==(const CXXVariant& other) const
{
	CXX_D(const CXXVariant);
	return d->compareFrom(other);
}

bool CXXVariant::operator!=(const CXXVariant& other) const
{
	CXX_D(const CXXVariant);
	return !d->compareFrom(other);
}

#pragma region template
template <>
bool CXXVariant::value<bool>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::Bool)
	{
		return static_cast<bool>(d->data_.charVal);
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'bool' to 'bool'.");

	return false;
}

template <>
char CXXVariant::value<char>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::Char)
	{
		return d->data_.charVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion fomr 'char' to 'char'.");

	return '\0';
}

template <>
BYTE CXXVariant::value<BYTE>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::UChar)
	{
		return d->data_.ucharVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'unsigned char' to 'BYTE'.");

	return 0;
}

template <>
short CXXVariant::value<short>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::Short)
	{
		return d->data_.shortVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'short' to 'short'.");

	return 0;
}

template <>
USHORT CXXVariant::value<USHORT>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::UShort)
	{
		return d->data_.ushortVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'unsigned short' to 'USHORT'.");

	return 0;
}

template <>
int CXXVariant::value<int>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::Int)
	{
		return d->data_.intVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'int' to 'int'.");

	return 0;
}

template <>
UINT CXXVariant::value<UINT>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::UInt)
	{
		return d->data_.uintVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'unsigned int' to 'UINT'.");

	return 0;
}

template <>
__int64 CXXVariant::value<__int64>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::LongLong)
	{
		return d->data_.int64Val;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from '__int64' to '__int64'.");

	return 0;
}

template <>
UINT64 CXXVariant::value<UINT64>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::ULongLong)
	{
		return d->data_.uint64Val;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'unsigned __int64' to 'UINT64'.");

	return 0;
}

template <>
float CXXVariant::value<float>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::Float)
	{
		return d->data_.floatVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'float' to 'float'.");

	return 0.0f;
}

template <>
double CXXVariant::value<double>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::Double)
	{
		return d->data_.doubleVal;
	}
	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'double' to 'double'.");

	return 0.0;
}

template <>
CXXString CXXVariant::value<CXXString>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::WString)
	{
		return *d->data_.wstringVal;
	}
	else if (d->type_ != cxx::DataType::User)
	{
		//強轉字符串
		std::wstring text;
		switch (d->type_)
		{
		case cxx::DataType::Bool:
		{
			text = static_cast<bool>(d->data_.charVal) ? L"true" : L"false";
			break;
		}
		case cxx::DataType::Char:
		{
			text = std::to_wstring(d->data_.charVal);
			break;
		}
		case cxx::DataType::UChar:
		{
			text = std::to_wstring(d->data_.ucharVal);
			break;
		}
		case cxx::DataType::Short:
		{
			text = std::to_wstring(d->data_.shortVal);
			break;
		}
		case cxx::DataType::UShort:
		{
			text = std::to_wstring(d->data_.ushortVal);
			break;
		}
		case cxx::DataType::Int:
		{
			text = std::to_wstring(d->data_.intVal);
			break;
		}
		case cxx::DataType::UInt:
		{
			text = std::to_wstring(d->data_.uintVal);
			break;
		}
		case cxx::DataType::LongLong:
		{
			text = std::to_wstring(d->data_.int64Val);
			break;
		}
		case cxx::DataType::ULongLong:
		{
			text = std::to_wstring(d->data_.uint64Val);
			break;
		}
		case cxx::DataType::Float:
		{
			std::wostringstream os;
			os << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10) << d->data_.floatVal;
			text = os.str();
			break;
		}
		case cxx::DataType::Double:
		{
			std::wostringstream os;
			os << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << d->data_.doubleVal;
			text = os.str();
			break;
		}
		case cxx::DataType::VoidStar:
		{
			std::wostringstream os;
			os << L"0x" << std::hex << reinterpret_cast<UINT64>(d->data_.voidPtrVal);
			text = os.str();
			break;
		}
		case cxx::DataType::Nullptr:
		{
			text = L"nullptr";
			break;
		}
		default:
		{
			break;
		}
		}

		return text;

	}
	else
	{
		std::wostringstream os;
		os << L"0x" << std::hex << reinterpret_cast<UINT64>(d->data_.voidPtrVal);
		return os.str();
	}

	//CXX_ASSERT_X(false, L"Incompatible type conversion. from 'wchar_t*' or 'std::wstring' to 'CXXString'.");
}

template <>
void* CXXVariant::value<void*>() const
{
	CXX_D(const CXXVariant);

	if (type() == cxx::DataType::VoidStar)
	{
		return d->data_.voidPtrVal;
	}
	else if (type() == cxx::DataType::Nullptr)
	{
		return CXX_NULLPTR;
	}
	else if (type() == cxx::DataType::User)
	{
		return d->data_.voidPtrVal;
	}

	CXX_ASSERT_X(false, L"Incompatible type conversion. from 'void*' to 'void*'.");
	return CXX_NULLPTR;
}
#pragma endregion
