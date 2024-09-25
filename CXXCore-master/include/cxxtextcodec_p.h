﻿/*
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
#ifndef CXXTEXTCODEC_P_H_
#define CXXTEXTCODEC_P_H_

#include <unordered_map>
#include <mutex>

#include <cxxtextcodec.h>

class CXXTextCodecPrivate
{
public:
	explicit CXXTextCodecPrivate(CXXTextCodec* q, __int64 acp);
	virtual ~CXXTextCodecPrivate();

private:
	__int64 acp_ = 0;

	static std::unordered_map<__int64, std::atomic<void*>> instances_;
	static std::mutex mutex_;

	CXX_DECLARE_PUBLIC(CXXTextCodec);
	CXXTextCodec* q_ptr = CXX_NULLPTR;

};


#endif // CXXTEXTCODEC_P_H_