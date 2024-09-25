// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#include <pch.h>
#include <cxxpoint.h>
#include <iostream>


std::wostream& operator<<(std::wostream& s, const CXXPoint& p)
{
	s << (intptr_t)p.x() << (intptr_t)p.y();

	return s;
}

std::wistream& operator>>(std::wistream& s, CXXPoint& p)
{
	intptr_t x, y;
	s >> x;
	p.rx() = x;
	s >> y;
	p.ry() = y;
	return s;
}

std::wostream& operator<<(std::wostream& s, const CXXPointF& p)
{
	s << double(p.x()) << double(p.y());
	return s;
}

std::wistream& operator>>(std::wistream& s, CXXPointF& p)
{
	double x, y;
	s >> x;
	s >> y;
	p.setX(double(x));
	p.setY(double(y));
	return s;
}
