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

#ifndef CXXDATETIME_H_
//@隐藏{
#define CXXDATETIME_H_
//@隐藏}



#include <cxxdate.h>
#include <cxxtime.h>
#include <chrono>
#include <sstream>
#include <regex>

//@分组{［其他］.炫　日期时间类

//@备注 CXXDateTime 类，提供日期和时间的管理功能
//@别名 炫日期时间
class CXXDateTime
{
public:
	//@备注 默认构造函数，初始化为无效的日期时间
	CXXDateTime();

	//@备注 构造函数，使用指定的日期和时间初始化
	//@参数 date 日期对象
	//@参数 time 时间对象
	CXXDateTime(const CXXDate& date, const CXXTime& time);

	virtual ~CXXDateTime();

	//@备注 静态函数：获取当前日期和时间
	//@返回 当前日期时间对象
	//@别名 当前日期时间()
	static CXXDateTime currentDateTime();

	//@备注 设置日期
	//@参数 date 日期对象
	//@别名 置日期
	void setDate(const CXXDate& date);

	//@备注 获取日期
	//@返回 当前的日期对象
	//@别名 日期
	CXXDate date() const;

	//@备注 设置时间
	//@参数 time 时间对象
	//@别名 置时间(时间对象)
	void setTime(const CXXTime& time);

	//@备注 获取时间
	//@返回 当前的时间对象
	//@别名 时间()
	CXXTime time() const;

	//@备注 将日期时间转换为从1970年1月1日以来的毫秒数
	//@返回 从1970年1月1日以来的毫秒数
	__int64 toMSecsSinceEpoch() const;

	//@备注 从毫秒数设置日期时间
	//@参数 msecs 从1970年1月1日以来的毫秒数
	static CXXDateTime fromMSecsSinceEpoch(__int64 msecs);

	//@备注 添加天数
	//@参数 ndays 要添加的天数
	//@返回 新的 CXXDateTime 对象
	//@别名 加日()
	CXXDateTime addDays(__int64 ndays) const;

	//@备注 添加秒数
	//@参数 secs 要添加的秒数
	//@返回 新的 CXXDateTime 对象
	//@别名 加秒()
	CXXDateTime addSecs(__int64 secs) const;

	//@备注 添加毫秒数
	//@参数 msecs 要添加的毫秒数
	//@返回 新的 CXXDateTime 对象
	//@别名 加分()
	CXXDateTime addMSecs(__int64 msecs) const;

	//@备注 将日期时间对象转换为字符串
	//@参数 format 格式化字符串
	//@返回 格式化后的宽字符字符串
	//@别名 到文本(格式化字符串)
	CXXString toString(const CXXString& format = L"%d-%M-%yyyy %H:%m:%s.%z") const;

	//@备注 从字符串解析日期时间对象
	//@参数 string 输入的字符串
	//@参数 format 格式化字符串
	//@返回 解析后的 CXXDateTime 对象
	//@别名 从文本(文本, 格式化字符串)
	static CXXDateTime fromString(const CXXString& string, const CXXString& format = L"%d-%M-%yyyy %H:%m:%s.%z");

	//@隐藏{


	//@备注 比较两个日期时间是否相等
	bool operator==(const CXXDateTime& other) const;

	//@备注 比较两个日期时间是否不等
	bool operator!=(const CXXDateTime& other) const;

	//@备注 比较两个日期时间的大小
	bool operator<(const CXXDateTime& other) const;

	//@备注 比较两个日期时间的大小
	bool operator<=(const CXXDateTime& other) const;

	//@备注 比较两个日期时间的大小
	bool operator>(const CXXDateTime& other) const;

	//@备注 比较两个日期时间的大小
	bool operator>=(const CXXDateTime& other) const;

	//@隐藏}

private:
	//@隐藏{
	CXXDate date_;
	CXXTime time_;
	//@隐藏}
};

//@分组}

#endif // CXXDATETIME_H_
