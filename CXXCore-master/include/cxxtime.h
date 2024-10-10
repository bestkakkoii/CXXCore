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
#ifndef CXXTIME_H_
//@隐藏{
#define CXXTIME_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>

#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cwchar>

//@分组{［其他］.炫　日期时间类

//@备注 CXXTime 类，提供时分秒毫秒的时间操作接口。
//       支持宽字符，并能够根据格式输出时间。
//@别名 炫时间
class CXXTime
{
public:
	//@备注 默认构造函数，初始化为空时间（00:00:00.000）
	CXXTime();

	//@备注 构造函数，初始化为指定时间
	CXXTime(__int64 h, __int64 m, __int64 s = 0, __int64 ms = 0);

	//@备注 将时间对象转换为格式化的宽字符字符串
	//@参数 format 格式化字符串 (宽字符)
	//@返回 格式化后的宽字符字符串
	//@别名 格式化为宽字符字符串
	//@别名 到文本(格式化字符串)
	CXXString toString(const CXXString& format = L"%H:%M:%S.%f") const;

	//@备注 设置小时、分钟、秒和毫秒
	//@参数 h 小时
	//@参数 m 分钟
	//@参数 s 秒
	//@参数 ms 毫秒
	//@返回 布尔值，表示设置是否成功
	//@别名 置时间(时,分,秒)
	bool setHMS(__int64 h, __int64 m, __int64 s, __int64 ms = 0);

	//@备注 判断时间是否有效
	//@返回 布尔值，表示时间是否有效
	//@别名 是否有效()
	bool isValid() const;

	//@备注 判断时间是否为空
	//@返回 布尔值，表示时间是否为空
	//@别名 是否空()
	bool isNull() const;

	//@备注 静态函数，检查给定时间是否有效
	//@参数 h 小时
	//@参数 m 分钟
	//@参数 s 秒
	//@参数 ms 毫秒
	//@返回 布尔值，表示时间是否有效
	//@别名 是否有效(时, 分, 秒, 毫秒)
	static bool isValid(__int64 h, __int64 m, __int64 s, __int64 ms = 0);

	//@备注 静态函数，获取当前系统时间
	//@返回 当前时间对象
	//@别名 当前时间()
	static CXXTime currentTime();

	//@备注 静态函数，根据从一天开始的毫秒数创建时间对象
	//@参数 msecs 自一天开始的毫秒数
	//@返回 创建的时间对象
	static CXXTime fromMSecsSinceStartOfDay(__int64 msecs);

	//@备注 静态函数，从宽字符字符串解析时间对象
	//@参数 string 输入的宽字符字符串
	//@参数 format 格式化字符串 (宽字符)
	//@返回 解析后的时间对象
	//@别名 从文本(文本时间, 格式化字符串)
	static CXXTime fromString(const CXXString& string, const CXXString& format = L"%H:%M:%S:%f");

	//@别名 时()
	__int64 hours() const;

	//@别名 分()
	__int64 minutes() const;

	//@别名 秒()
	__int64 seconds() const;

	//@别名 毫秒()
	__int64 milliseconds() const;

private:
	//@隐藏{

	//@备注 将时间转换为毫秒
	//@返回 毫秒数
	__int64 toMilliseconds() const;
	//@隐藏}

private:
	//@隐藏{
	__int64 hours_ = 0;
	__int64 minutes_ = 0;
	__int64 seconds_ = 0;
	__int64 milliseconds_ = 0;
	bool isNull_ = true;
	//@隐藏}
};

//@分组}

#endif // CXXTIME_H_
