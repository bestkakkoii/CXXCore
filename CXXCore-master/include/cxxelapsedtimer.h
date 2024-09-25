/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CXXELAPSEDTIMER_H_
//@隐藏{
#define CXXELAPSEDTIMER_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxscopedpointer.h>

//@别名 炫
namespace cxx
{
	//@分组{［工具］.炫　计时类


	//@别名 时钟类型
	enum ClockType
	{
		SystemTime,        //@别名 系统时间
		MonotonicClock,    //@别名 单调时钟
		TickCounter,       //@别名 系统滴答计数器
		MachAbsoluteTime,  //@别名 Mach绝对时间
		PerformanceCounter //@别名 性能计数器
	};

	//@别名 计时器模式
	enum TimerMode
	{
		DefaultMode = 0, //@别名 默认模式
		AutoStart = 1 //@别名 自动开始
	};

	//@分组}
}

//@分组{［工具］.炫　计时类


//@隐藏{
class CXXElapsedTimerPrivate;
//@隐藏}

//@备注 用于计时
// 使用方法:
// CXXElapsedTimer t; 
// t.start();
// print() << t.elapsed();
//@别名 炫计时器
class CXXElapsedTimer
{
public:
	//@参数 无
	CXXElapsedTimer();

	virtual ~CXXElapsedTimer();

	//@参数 TimerMode模式 1为自动开始 0为手动开始
	explicit CXXElapsedTimer(cxx::TimerMode mode);

	//@备注 取时钟类型
	//@参数 无
	//@返回 时钟类型
	//@别名 时钟类型()
	static cxx::ClockType clockType();

	//@备注 是否单调
	//@参数 无
	//@返回 是否单调
	//@别名 是否单调()
	static bool isMonotonic();

	//@备注 开始计时
	//@参数 无
	//@返回 无
	//@别名 开始()
	void start();

	//@备注 重新开始计时
	//@参数 无
	//@返回 无
	//@别名 重启()
	__int64 restart();

	//@备注 标记此计时器为无效
	//@参数 无
	//@返回 无
	//@别名 无效化()
	void invalidate();

	//@备注 是否有效
	//@参数 无
	//@返回 是否有效
	//@别名 是否有效()
	bool isValid() const;

	//@备注 获取当前计时经历秒数
	//@参数 无
	//@返回 秒数
	//@别名 秒()
	__int64 nsecsElapsed() const;

	//@备注 获取当前计时经历毫秒数
	//@参数 无
	//@返回 毫秒数
	//@别名 毫秒()
	__int64 elapsed() const;

	//@备注 获取当前计时经历微秒数
	//@参数 无
	//@返回 微秒数
	//@别名 微秒()
	__int64 elapsedMicro() const;

	//@备注 是否超时
	//@参数 超时时间
	//@返回 是否超时
	//@别名 是否超时(毫秒数)
	bool hasExpired(__int64 timeout) const;

	//@备注 取自1970年1月1日UTC至今的毫秒数
	//@参数 无
	//@返回 毫秒数
	//@别名 取UTC毫秒数()
	__int64 msecsSinceReference() const;

	//@备注 从另一个CXElapsedTimer返回相差的毫秒数
	//@参数 另一个CXElapsedTimer
	//@返回 毫秒数
	//@别名 毫秒差(计时器)
	__int64 msecsTo(const CXXElapsedTimer& other) const;

	//@备注 从另一个CXElapsedTimer返回相差的秒数
	//@参数 另一个CXElapsedTimer
	//@返回 秒数
	//@别名 秒差(计时器)
	__int64 secsTo(const CXXElapsedTimer& other) const;

	//@隐藏{
	bool operator==(const CXXElapsedTimer& other) const;

	bool operator!=(const CXXElapsedTimer& other) const;

	bool operator<(const CXXElapsedTimer& v2) const;

	bool operator>(const CXXElapsedTimer& v2) const;

	bool operator<=(const CXXElapsedTimer& v2) const;

	bool operator>=(const CXXElapsedTimer& v2) const;
	//@隐藏}
private:
	//@隐藏{

	CXX_DECLARE_PRIVATE(CXXElapsedTimer);
	CXXScopedPointer<CXXElapsedTimerPrivate> d_ptr;

	//@隐藏}
};

//@分组}

#endif // CXXELAPSEDTIMER_H_
