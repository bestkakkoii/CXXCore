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
#include <pch.h>

#include <cxxelapsedtimer_p.h>

#include <windows.h>
#include <cassert>
#include <tuple>
#include <crtdbg.h>

/*!
	\class CXElapsedTimer
	\inmodule QtCore
	\brief The CXElapsedTimer class provides a fast way to calculate elapsed times.
	\since 4.7

	\reentrant
	\ingroup tools

	The CXElapsedTimer class is usually used to quickly calculate how much
	time has elapsed between two events. Its API is similar to that of QTime,
	so code that was using that can be ported quickly to the new class.

	However, unlike QTime, CXElapsedTimer tries to use monotonic clocks if
	possible. This means it's not possible to convert CXElapsedTimer objects
	to a human-readable time.

	The typical use-case for the class is to determine how much time was
	spent in a slow operation. The simplest example of such a case is for
	debugging purposes, as in the following example:

	\snippet qelapsedtimer/main.cpp 0

	In this example, the timer is started by a call to start() and the
	elapsed time is calculated by the elapsed() function.

	The time elapsed can also be used to recalculate the time available for
	another operation, after the first one is complete. This is useful when
	the execution must complete within a certain time period, but several
	steps are needed. The \tt{waitFor}-type functions in QIODevice and its
	subclasses are good examples of such need. In that case, the code could
	be as follows:

	\snippet qelapsedtimer/main.cpp 1

	Another use-case is to execute a certain operation for a specific
	timeslice. For this, CXElapsedTimer provides the hasExpired() convenience
	function, which can be used to determine if a certain number of
	milliseconds has already elapsed:

	\snippet qelapsedtimer/main.cpp 2

	It is often more convenient to use \l{QDeadlineTimer} in this case, which
	counts towards a timeout in the future instead of tracking elapsed time.

	\section1 Reference Clocks

	CXElapsedTimer will use the platform's monotonic reference clock in all
	platforms that support it (see CXXElapsedTimer::isMonotonic()). This has
	the added benefit that CXElapsedTimer is immune to time adjustments, such
	as the user correcting the time. Also unlike QTime, CXElapsedTimer is
	immune to changes in the timezone settings, such as daylight-saving
	periods.

	On the other hand, this means CXElapsedTimer values can only be compared
	with other values that use the same reference. This is especially true if
	the time since the reference is extracted from the CXElapsedTimer object
	(CXXElapsedTimer::msecsSinceReference()) and serialised. These values
	should never be exchanged across the network or saved to disk, since
	there's no telling whether the computer node receiving the data is the
	same as the one originating it or if it has rebooted since.

	It is, however, possible to exchange the value with other processes
	running on the same machine, provided that they also use the same
	reference clock. CXElapsedTimer will always use the same clock, so it's
	safe to compare with the value coming from another process in the same
	machine. If comparing to values produced by other APIs, you should check
	that the clock used is the same as CXElapsedTimer (see
	CXXElapsedTimer::clockType()).

	\section2 32-bit overflows

	Some of the clocks used by CXElapsedTimer have a limited range and may
	overflow after hitting the upper limit (usually 32-bit). CXElapsedTimer
	deals with this overflow issue and presents a consistent timing. However,
	when extracting the time since reference from CXElapsedTimer, two
	different processes in the same machine may have different understanding
	of how much time has actually elapsed.

	The information on which clocks types may overflow and how to remedy that
	issue is documented along with the clock types.

	\sa QTime, QTimer, QDeadlineTimer
*/

/*!
	\enum CXXElapsedTimer::ClockType

	This enum contains the different clock types that CXElapsedTimer may use.

	CXElapsedTimer will always use the same clock type in a particular
	machine, so this value will not change during the lifetime of a program.
	It is provided so that CXElapsedTimer can be used with other non-Qt
	implementations, to guarantee that the same reference clock is being
	used.

	\value SystemTime         The human-readable system time. This clock is not monotonic.
	\value MonotonicClock     The system's monotonic clock, usually found in Unix systems. This clock is monotonic and does not overflow.
	\value TickCounter        The system's tick counter, used on Windows systems. This clock may overflow.
	\value MachAbsoluteTime   The Mach kernel's absolute time (\macos and iOS). This clock is monotonic and does not overflow.
	\value PerformanceCounter The high-resolution performance counter provided by Windows. This clock is monotonic and does not overflow.

	\section2 SystemTime

	The system time clock is purely the real time, expressed in milliseconds
	since Jan 1, 1970 at 0:00 UTC. It's equivalent to the value returned by
	the C and POSIX \tt{time} function, with the milliseconds added. This
	clock type is currently only used on Unix systems that do not support
	monotonic clocks (see below).

	This is the only non-monotonic clock that CXElapsedTimer may use.

	\section2 MonotonicClock

	This is the system's monotonic clock, expressed in milliseconds since an
	arbitrary point in the past. This clock type is used on Unix systems
	which support POSIX monotonic clocks (\tt{_POSIX_MONOTONIC_CLOCK}).

	This clock does not overflow.

	\section2 TickCounter

	The tick counter clock type is based on the system's or the processor's
	tick counter, multiplied by the duration of a tick. This clock type is
	used on Windows platforms. If the high-precision performance
	counter is available on Windows, the \tt{PerformanceCounter} clock type
	is used instead.

	The TickCounter clock type is the only clock type that may overflow.
	Windows Vista and Windows Server 2008 support the extended 64-bit tick
	counter, which allows avoiding the overflow.

	On Windows systems, the clock overflows after 2^32 milliseconds, which
	corresponds to roughly 49.7 days. This means two processes' reckoning of
	the time since the reference may be different by multiples of 2^32
	milliseconds. When comparing such values, it's recommended that the high
	32 bits of the millisecond count be masked off.

	\section2 MachAbsoluteTime

	This clock type is based on the absolute time presented by Mach kernels,
	such as that found on \macos. This clock type is presented separately
	from MonotonicClock since \macos and iOS are also Unix systems and may support
	a POSIX monotonic clock with values differing from the Mach absolute
	time.

	This clock is monotonic and does not overflow.

	\section2 PerformanceCounter

	This clock uses the Windows functions \tt{QueryPerformanceCounter} and
	\tt{QueryPerformanceFrequency} to access the system's high-precision
	performance counter. Since this counter may not be available on all
	systems, CXElapsedTimer will fall back to the \tt{TickCounter} clock
	automatically, if this clock cannot be used.

	This clock is monotonic and does not overflow.

	\sa clockType(), isMonotonic()
*/

/*!
	\fn CXXElapsedTimer::CXElapsedTimer()
	\since 5.4

	Constructs an invalid CXElapsedTimer. A timer becomes valid once it has been
	started.

	\sa isValid(), start()
*/


/*!
	\fn bool CXXElapsedTimer::operator ==(const CXElapsedTimer &other) const

	Returns \c true if this object and \a other contain the same time.
*/

/*!
	\fn bool CXXElapsedTimer::operator !=(const CXElapsedTimer &other) const

	Returns \c true if this object and \a other contain different times.
*/

namespace cxx
{
	namespace
	{
		constexpr __int64 elapsedTimerInvalidData(0x8000000000000000i64);

		// Result of QueryPerformanceFrequency, 0 indicates that the high resolution timer is unavailable
		static std::atomic<unsigned long long> counterFrequency = 0ui64;
	}
}

void CXXElapsedTimerPrivate::resolveCounterFrequency()
{
	static bool done = false;
	if (done)
		return;

	// Retrieve the number of high-resolution performance counter ticks per second
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
	{
		//qFatal("QueryPerformanceFrequency failed, even though Microsoft documentation promises it wouldn't.");
		cxx::counterFrequency.store(0ui64, std::memory_order_release);
	}
	else
	{
		cxx::counterFrequency.store(frequency.QuadPart, std::memory_order_release);
	}

	done = true;
}

__int64 CXXElapsedTimerPrivate::ticksToNanoseconds(__int64 ticks)
{
	if (cxx::counterFrequency.load(std::memory_order_acquire) > 0ui64)
	{
		// QueryPerformanceCounter uses an arbitrary frequency
		__int64 seconds = ticks / cxx::counterFrequency.load(std::memory_order_acquire);
		__int64 nanoSeconds = (ticks - seconds * cxx::counterFrequency.load(std::memory_order_acquire))
			* 1000000000i64 / cxx::counterFrequency.load(std::memory_order_acquire);
		return seconds * 1000000000i64 + nanoSeconds;
	}
	// GetTickCount(64) returns milliseconds
	return ticks * 1000000i64;
}

unsigned long long CXXElapsedTimerPrivate::getTickCount()
{
	resolveCounterFrequency();

	// This avoids a division by zero and disables the high performance counter if it's not available
	if (cxx::counterFrequency.load(std::memory_order_acquire) > 0)
	{
		LARGE_INTEGER counter;

		bool ok = QueryPerformanceCounter(&counter);
		_ASSERT_EXPR(ok, "QueryPerformanceCounter failed, although QueryPerformanceFrequency succeeded.");
		std::ignore = ok;
		return counter.QuadPart;
	}

	return GetTickCount64();
}

//static unsigned long long _cx_msectime()
//{
//	return _ticksToNanoseconds(_getTickCount()) / 1000000i64;
//}

CXXElapsedTimerPrivate::CXXElapsedTimerPrivate(CXXElapsedTimer* q)
	: q_ptr(q)
	, t1(0x8000000000000000i64)
	, t2(0x8000000000000000i64)
{
	start();
}

CXXElapsedTimerPrivate::CXXElapsedTimerPrivate(CXXElapsedTimer* q, cxx::TimerMode mode)
	: q_ptr(q)
	, t1(0x8000000000000000i64)
	, t2(0x8000000000000000i64)
{
	if (mode == cxx::AutoStart)
	{
		start();
	}
}

CXXElapsedTimerPrivate::~CXXElapsedTimerPrivate()
{

}

void CXXElapsedTimerPrivate::start()
{
	//only allow start once
	if (t1.load(std::memory_order_acquire) != cxx::elapsedTimerInvalidData)
	{
		return;
	}

	t1.store(getTickCount(), std::memory_order_release);
	t2.store(0i64, std::memory_order_release);
}

CXXElapsedTimer::CXXElapsedTimer()
	: d_ptr(new CXXElapsedTimerPrivate(this))
{

}

//@参数 TimerMode模式 1为自动开始 0为手动开始
CXXElapsedTimer::CXXElapsedTimer(cxx::TimerMode mode)
	: d_ptr(new CXXElapsedTimerPrivate(this, mode))
{

}

CXXElapsedTimer::~CXXElapsedTimer()
{

}

/*!
	Marks this CXElapsedTimer object as invalid.

	An invalid object can be checked with isValid(). Calculations of timer
	elapsed since invalid data are undefined and will likely produce bizarre
	results.

	\sa isValid(), start(), restart()
*/

void CXXElapsedTimer::invalidate()
{
	CXX_D(CXXElapsedTimer);
	d->t1.store(cxx::elapsedTimerInvalidData, std::memory_order_release);
	d->t2.store(cxx::elapsedTimerInvalidData, std::memory_order_release);
}

/*!
	Returns \c false if the timer has never been started or invalidated by a
	call to invalidate().

	\sa invalidate(), start(), restart()
*/
bool CXXElapsedTimer::isValid() const
{
	CXX_D(const CXXElapsedTimer);
	return d->t1.load(std::memory_order_acquire) != cxx::elapsedTimerInvalidData
		&& d->t2.load(std::memory_order_acquire) != cxx::elapsedTimerInvalidData;
}

/*!
	Returns \c true if this CXElapsedTimer has already expired by \a timeout
	milliseconds (that is, more than \a timeout milliseconds have elapsed).
	The value of \a timeout can be -1 to indicate that this timer does not
	expire, in which case this function will always return false.

	\sa elapsed(), QDeadlineTimer
*/
bool CXXElapsedTimer::hasExpired(__int64 timeout) const
{
	// if timeout is -1, unsigned long long(timeout) is LLINT_MAX, so this will be
	// considered as never expired
	return unsigned long long(elapsed()) > unsigned long long(timeout);
}

cxx::ClockType CXXElapsedTimer::clockType()
{
	CXXElapsedTimerPrivate::resolveCounterFrequency();
	return cxx::counterFrequency.load(std::memory_order_acquire) > 0 ? cxx::ClockType::PerformanceCounter : cxx::ClockType::TickCounter;
}

bool CXXElapsedTimer::isMonotonic()
{
	return cxx::counterFrequency.load(std::memory_order_acquire) > 0;
}

void CXXElapsedTimer::start()
{
	CXX_D(CXXElapsedTimer);
	d->start();
}

__int64 CXXElapsedTimer::restart()
{
	CXX_D(CXXElapsedTimer);

	__int64 oldt1 = d->t1;
	d->t1.store(d->getTickCount(), std::memory_order_release);
	d->t2.store(0i64, std::memory_order_release);
	return d->ticksToNanoseconds(d->t1.load(std::memory_order_acquire) - oldt1) / 1000000i64;
}

__int64 CXXElapsedTimer::nsecsElapsed() const
{
	CXX_D(const CXXElapsedTimer);

	__int64 elapsed = d->getTickCount() - d->t1.load(std::memory_order_acquire);
	return d->ticksToNanoseconds(elapsed) / 1000000i64 / 1000i64;
}

__int64 CXXElapsedTimer::elapsed() const
{
	CXX_D(const CXXElapsedTimer);

	__int64 elapsed = d->getTickCount() - d->t1.load(std::memory_order_acquire);
	return d->ticksToNanoseconds(elapsed) / 1000000i64;
}

__int64 CXXElapsedTimer::elapsedMicro() const
{
	CXX_D(const CXXElapsedTimer);

	__int64 elapsed = d->getTickCount() - d->t1.load(std::memory_order_acquire);
	return d->ticksToNanoseconds(elapsed) / 1000i64;
}

__int64 CXXElapsedTimer::msecsSinceReference() const
{
	CXX_D(const CXXElapsedTimer);

	return d->ticksToNanoseconds(d->t1.load(std::memory_order_acquire)) / 1000000i64;
}

__int64 CXXElapsedTimer::msecsTo(const CXXElapsedTimer& other) const
{
	CXX_D(const CXXElapsedTimer);

	__int64 difference = other.d_ptr->t1.load(std::memory_order_acquire) - d->t1.load(std::memory_order_acquire);
	return d->ticksToNanoseconds(difference) / 1000000i64;
}

__int64 CXXElapsedTimer::secsTo(const CXXElapsedTimer& other) const
{
	return msecsTo(other) / 1000i64;
}

bool CXXElapsedTimer::operator<(const CXXElapsedTimer& v2) const
{
	CXX_D(const CXXElapsedTimer);
	return (d->t1.load(std::memory_order_acquire) - v2.d_ptr->t1.load(std::memory_order_acquire)) < 0i64;
}

bool CXXElapsedTimer::operator<=(const CXXElapsedTimer& v2) const
{
	CXX_D(const CXXElapsedTimer);
	return (d->t1.load(std::memory_order_acquire) - v2.d_ptr->t1.load(std::memory_order_acquire)) <= 0i64;
}

bool CXXElapsedTimer::operator>(const CXXElapsedTimer& v2) const
{
	CXX_D(const CXXElapsedTimer);
	return (d->t1.load(std::memory_order_acquire) - v2.d_ptr->t1.load(std::memory_order_acquire)) > 0i64;
}

bool CXXElapsedTimer::operator>=(const CXXElapsedTimer& v2) const
{
	CXX_D(const CXXElapsedTimer);
	return (d->t1.load(std::memory_order_acquire) - v2.d_ptr->t1.load(std::memory_order_acquire)) >= 0i64;
}


bool CXXElapsedTimer::operator==(const CXXElapsedTimer& other) const
{
	CXX_D(const CXXElapsedTimer);
	return d->t1.load(std::memory_order_acquire) == other.d_ptr->t1.load(std::memory_order_acquire)
		&& d->t2.load(std::memory_order_acquire) == other.d_ptr->t2.load(std::memory_order_acquire);
}

bool CXXElapsedTimer::operator!=(const CXXElapsedTimer& other) const
{
	return !(*this == other);
}