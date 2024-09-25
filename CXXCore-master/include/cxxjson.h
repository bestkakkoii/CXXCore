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
#ifndef CXXJSON_H_
//@隐藏{
#define CXXJSON_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>

#include <nlohmann/json.hpp>

#include <iostream>
#include <stdexcept>

namespace cxx
{
	//@分组{ ［文件／文件夹］.炫　ＪＳＯＮ类


	//@别名 JSON格式
	enum JsonFormat
	{
		JsonIndented, //@别名 缩进
		JsonCompact   //@别名 紧凑
	};

	//@分组}
}

//@分组{ ［文件／文件夹］.炫　ＪＳＯＮ类

//@隐藏{
using CXXJsonObject = nlohmann::json;
//@隐藏}
/*@声明

@备注 JSON核心对象
@别名 炫JSON对象
class CXXJsonObject
{
public:
};

*/

//@备注 用于管理JSON解析错误和位置
//@别名 炫JSON解析错误
class CXXJsonParseError
{
public:
	// @备注 错误消息
	// @别名 消息
	CXXString message;

	// @备注 错误位置
	// @别名 位置
	int position;
};

//@备注 JSON对象、兼具JSON序列化和反序列化，支持宽字符输入输出
//@别名 炫JSON
class CXXJson : protected CXXJsonObject
{
public:
	CXXJson()
	{
	}

	template <typename T>
	CXXJson(T value)
		: CXXJsonObject(value)
	{
	}

	CXXJson(const CXXString& value)
		: CXXJsonObject(nlohmann::json::parse(value.toUtf8()))
	{
	}

	CXXJson(const wchar_t* value)
		: CXXJsonObject(nlohmann::json::parse(CXXString(value).toUtf8()))
	{
	}

	CXXJson(wchar_t* value)
		: CXXJsonObject(nlohmann::json::parse(CXXString(value).toUtf8()))
	{
	}

	CXXJson(CXXJson&& other) noexcept
		: CXXJsonObject(std::move(other))
	{
		other.clear();
	}

	CXXJson(const CXXJsonObject& other)
		: CXXJsonObject(other)
	{
	}

	virtual ~CXXJson()
	{

	}

	//@备注 检查当前JSON对象是否为整型
	//@参数 无
	//@返回 逻辑型
	//@别名 是否整型()
	bool isInt() const
	{
		return CXXJsonObject::is_number_integer();
	}

	//@备注 检查当前JSON对象是否为逻辑型
	//@参数 无
	//@返回 逻辑型
	//@别名 是否逻辑型()
	bool isBool() const
	{
		return CXXJsonObject::is_boolean();
	}

	//@备注 检查当前JSON对象是否为双浮点型
	//@参数 无
	//@返回 逻辑型
	//@别名 是否双浮点型()
	bool isDouble() const
	{
		return CXXJsonObject::is_number_float();
	}

	//@备注 检查当前JSON对象是否为文本
	//@参数 无
	//@返回 逻辑型
	//@别名 是否文本()
	bool isString() const
	{
		return CXXJsonObject::is_string();
	}

	//@备注 检查当前JSON对象是否为JSON对象
	//@参数 无
	//@返回 逻辑型
	//@别名 是否对象()
	bool isObject() const
	{
		return CXXJsonObject::is_object();
	}

	//@备注 检查当前JSON对象是否为JSON数组
	//@参数 无
	//@返回 逻辑型
	//@别名 是否数组()
	bool isArray() const
	{
		return CXXJsonObject::is_array();
	}

	//@备注 将当前JSON对象转换为 整型
	//@参数 无
	//@返回 整型
	//@别名 到整型()
	__int64 toInt(__int64 defaultValue = 0) const
	{
		if (!isInt())
		{
			return defaultValue;
		}

		return CXXJsonObject::get<__int64>();
	}

	//@备注 将当前JSON对象转换为 逻辑型
	//@参数 无
	//@返回 逻辑型
	//@别名 到逻辑型()
	bool toBool(bool defaultValue = false) const
	{
		if (!isBool())
		{
			return defaultValue;
		}

		return CXXJsonObject::get<bool>();
	}

	//@备注 将当前JSON对象转换为 双浮点型
	//@参数 无
	//@返回 双浮点型
	//@别名 到双浮点型()
	double toDouble(double defaultValue = 0.0) const
	{
		if (!isDouble())
		{
			return defaultValue;
		}

		return CXXJsonObject::get< double >();
	}

	//@备注 将当前JSON对象转换为 炫文本
	//@参数 无
	//@返回 文本
	//@别名 到文本()
	CXXString toString(const CXXString& defaultValue = {}) const
	{
		if (!isString())
		{
			return defaultValue;
		}

		return CXXString::fromUtf8(CXXJsonObject::get<std::string>().c_str());
	}

	//@备注 根据文本键获取JSON对象中的JSON对象
	//@参数 key 文本键
	//@返回 JSON对象
	//@别名 在(文本键)
	CXXJson& at(const CXXString& key)
	{
		return static_cast<CXXJson&>(CXXJsonObject::operator[](key.toUtf8()));
	}

	//@备注 根据文本键获取JSON对象中的JSON常量对象
	//@参数 key 文本键
	//@返回 常量JSON对象
	//@别名 在(文本键)
	const CXXJson at(const CXXString& key) const
	{
		return CXXJsonObject::at(key.toUtf8());
	}

	//@备注 根据整数索引获取数组中的JSON对象
	//@参数 key 文本键
	//@返回 JSON对象
	//@别名 在(文本键)
	CXXJson& at(__int64 index)
	{
		return static_cast<CXXJson&>(CXXJsonObject::operator[](static_cast<size_t>(index)));
	}

	//@备注 根据整数索引获取数组中的JSON常量对象
	//@参数 key 文本键
	//@返回 常量JSON对象
	//@别名 在(文本键)
	const CXXJson at(__int64 index) const
	{
		return CXXJsonObject::at(static_cast<size_t>(index));
	}

	//@备注 将一个JSON对象添加到当前JSON对象中
	//@参数 value JSON对象
	//@返回 无
	//@别名 追加(对象)
	void append(const CXXJson& value)
	{
		if (!isArray())
		{
			throw std::runtime_error("Type is not array");
		}

		CXXJsonObject::push_back(value);
	}

	//@备注 Json数组或对象大小
	//@参数 无
	//@返回 Json数组或对象大小
	//@别名 大小()
	__int64 size() const
	{
		return CXXJsonObject::size();
	}

	//@备注 Json数组或对象大小
	//@参数 无
	//@返回 Json数组或对象大小
	__int64 count() const
	{
		return size();
	}

	//@备注 Json数组或对象大小
	//@参数 无
	//@返回 Json数组或对象大小
	__int64 length() const
	{
		return size();
	}

	//@备注 检查当前JSON对象是否为空
	//@参数 无
	//@返回 逻辑型
	//@别名 是否有效()
	bool isValid() const
	{
		return !CXXJsonObject::empty();
	}

	//@备注 检查当前JSON对象是否为空
	//@参数 无
	//@返回 逻辑型
	//@别名 是否空()
	bool isEmpty() const
	{
		return !isValid();
	}

	//@备注 检查当前JSON对象是否为空
	//@参数 无
	//@返回 逻辑型
	bool empty() const
	{
		return isEmpty();
	}

	//@备注 检查当前JSON对象是否为空
	//@参数 无
	//@返回 逻辑型
	bool isNull() const
	{
		return isEmpty();
	}

	//@备注 将当前JSON对象转换为炫文本
	//@参数 format JSON格式
	//@返回 JSON炫文本
	//@别名 到JSON(格式)
	CXXString toJson(cxx::JsonFormat format = cxx::JsonIndented) const
	{
		try
		{
			if (format == cxx::JsonIndented)
			{
				std::string str = CXXJsonObject::dump(4);
				return CXXString::fromUtf8(str.c_str());
			}

			std::string str = CXXJsonObject::dump();
			return CXXString::fromUtf8(str.c_str());
		}
		catch (const nlohmann::json::exception& e)
		{
			return CXXString::fromUtf8(e.what());
		}
	}

public:
	//@备注 将JSON对象转换为炫文本
	//@参数 object JSON对象
	//@参数 format JSON格式
	//@返回 JSON炫文本
	//@别名 到JSON(格式)
	static CXXString toJson(const CXXJson& object, cxx::JsonFormat format = cxx::JsonIndented)
	{
		return object.toJson(format);
	}

	//@备注 将炫文本转换为JSON对象
	//@参数 json JSON炫文本
	//@参数 error 炫JSON解析错误指针
	//@返回 JSON对象
	//@别名 从JSON(文本, 解析错误指针)
	static CXXJson fromJson(const CXXString& json, CXXJsonParseError* error = nullptr)
	{
		CXXJson result;
		try
		{
			result = nlohmann::json::parse(json.toUtf8());
			if (error)
			{
				error->message = L"";
				error->position = 0;
			}
		}
		catch (const nlohmann::json::exception& e)
		{
			if (error)
			{
				error->message = CXXString::fromUtf8(e.what());
				error->position = static_cast<int>(e.id);
			}
		}
		return result;
	}

public:
	//@隐藏{
	template<typename T>
	CXXJson& operator=(T value)
	{
		CXXJsonObject::operator=(value);
		return *this;
	}

	CXXJson& operator=(const CXXJsonObject& other)
	{
		CXXJsonObject::operator=(other);
		return *this;
	}

	CXXJson& operator=(const CXXJson& other)
	{
		CXXJsonObject::operator=(other);
		return *this;
	}

	CXXJson& operator=(CXXJson&& other) noexcept
	{
		*this = std::move(other);
		other.clear();
		return *this;
	}

	CXXJson& operator[](const CXXString& key)
	{
		return static_cast<CXXJson&>(CXXJsonObject::operator[](key.toUtf8()));
	}

	const CXXJson& operator[](const CXXString& key) const
	{
		std::string str = key.toUtf8();
		return CXXJsonObject::at(str);
	}

	CXXJson& operator[](const wchar_t* key)
	{
		std::string str = CXXString(key).toUtf8();
		return static_cast<CXXJson&>(CXXJsonObject::operator[](str));
	}

	const CXXJson& operator[](const wchar_t* key) const
	{
		std::string str = CXXString(key).toUtf8();
		return CXXJsonObject::at(str);
	}

	CXXJson& operator[](wchar_t* key)
	{
		std::string str = CXXString(key).toUtf8();
		return static_cast<CXXJson&>(CXXJsonObject::operator[](str));
	}

	const CXXJson& operator[](wchar_t* key) const
	{
		std::string str = CXXString(key).toUtf8();
		return CXXJsonObject::at(str);
	}

	CXXJson& operator[](__int64 index)
	{
		return static_cast<CXXJson&>(CXXJsonObject::operator[](static_cast<size_t>(index)));
	}

	const CXXJson& operator[](__int64 index) const
	{
		return CXXJsonObject::at(static_cast<size_t>(index));
	}
	//@隐藏}

private:

};

//@分组}
#endif // CXXJSON_H
