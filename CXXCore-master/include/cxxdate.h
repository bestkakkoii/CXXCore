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
#ifndef CXXDATE_H_
//@隐藏{
#define CXXDATE_H_
//@隐藏}



#include <CXXString.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <chrono>

//@分组{［其他］.炫　日期时间类

//@备注 CXXDate 类，提供日期操作功能，并支持日期格式化输出
//@别名 炫日期
class CXXDate
{
public:
	//@备注 默认构造函数，初始化为无效日期
	CXXDate();

	//@备注 构造函数，初始化为指定的年月日
	CXXDate(__int64 y, __int64 m, __int64 d);

	//@备注 设置日期
	//@参数 y 年份
	//@参数 m 月份
	//@参数 d 日期
	//@返回 布尔值，表示设置是否成功
	//@别名 置日期(年, 月, 日)

	bool setDate(__int64 y, __int64 m, __int64 d);

	//@备注 判断日期是否有效
	//@参数 y 年份
	//@参数 m 月份
	//@参数 d 日期
	//@返回 布尔值，表示日期是否有效
	//@别名 是否有效(年, 月, 日)
	static bool isValid(__int64 y, __int64 m, __int64 d);

	//@备注 判断是否为闰年
	//@参数 year 年份
	//@返回 布尔值，表示是否为闰年
	//@别名 是否闰年(年)
	static bool isLeapYear(__int64 year);

	//@备注 获取年份
	//@返回 年份
	//@别名 年()
	__int64 year() const;

	//@备注 获取月份
	//@返回 月份
	//@别名 月()
	__int64 month() const;

	//@备注 获取日期
	//@返回 日期
	//@别名 日()
	__int64 day() const;

	//@备注 获取星期几（1 = 周一，7 = 周日）
	//@返回 星期几
	//@别名 星期()
	__int64 dayOfWeek() const;

	//@备注 获取当前月份的天数
	//@返回 当前月份的天数
	//@别名 经历日()
	__int64 daysInMonth() const;

	//@备注 将日期对象转换为格式化的宽字符字符串
	//@参数 format 格式化字符串 (宽字符)
	//@返回 格式化后的宽字符字符串
	//@别名 到文本(格式化字符串)
	CXXString toString(const CXXString& format = L"%d.%M.%yyyy") const;

	//@别名 加日(日)
	CXXDate addDays(__int64 day) const;

	//@别名 当前日期()
	static CXXDate currentDate();

private:
	//@隐藏{

	//@备注 返回指定月份的名称
	//@参数 month 月份 (1-12)
	//@参数 abbreviated 是否为缩写
	//@返回 月份名称
	CXXString getMonthName(__int64 month, bool abbreviated) const;

	//@备注 返回指定星期几的名称
	//@参数 dayOfWeek 星期几 (1-7, 1=周一, 7=周日)
	//@参数 abbreviated 是否为缩写
	//@返回 星期几名称
	CXXString getDayOfWeekName(__int64 dayOfWeek, bool abbreviated) const;

	//@隐藏}

private:
	//@隐藏{
	__int64 year_ = 0;
	__int64 month_ = 0;
	__int64 day_ = 0;
	//@隐藏}
};

//@分组}

#endif // CXXDATE_H_
