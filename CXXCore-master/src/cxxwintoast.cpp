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

#include <cxxwintoast_p.h>
#include <cxxmutex.h>
#include <cxxmutexlocker.h>

#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <chrono>
#include <algorithm>
#include <atomic>

namespace cxx
{
	namespace
	{
		std::atomic_int actionIndex_ = -1;

		CXXToastResultCallback resultCallback_;

		CXXMutex mutex_;
	}
}

#pragma region CXXToastHandler
CXXToastHandler::CXXToastHandler()
{

}

CXXToastHandler::~CXXToastHandler()
{

}

void CXXToastHandler::toastActivated() const
{
	CXXMutexLocker locker(&cxx::mutex_);
	if (CXX_NULLPTR == cxx::resultCallback_)
	{
		return;
	}

	cxx::resultCallback_(cxx::WinToastResult::Successed, -1);
}

void CXXToastHandler::toastActivated(int actionIndex) const
{
	CXXMutexLocker locker(&cxx::mutex_);
	if (CXX_NULLPTR == cxx::resultCallback_)
	{
		return;
	}
	cxx::actionIndex_ = actionIndex;
	cxx::resultCallback_(cxx::WinToastResult::Successed, actionIndex);
}

void CXXToastHandler::toastFailed() const
{
	CXXMutexLocker locker(&cxx::mutex_);
	if (CXX_NULLPTR == cxx::resultCallback_)
	{
		return;
	}
	cxx::resultCallback_(cxx::WinToastResult::Failured, -1);
}

void CXXToastHandler::toastDismissed(WinToastLib::IWinToastHandler::WinToastDismissalReason state) const
{
	CXXMutexLocker locker(&cxx::mutex_);
	if (CXX_NULLPTR == cxx::resultCallback_)
	{
		return;
	}

	cxx::resultCallback_(static_cast<cxx::WinToastResult>(state), -1);
}
#pragma endregion

CXXWinToastPrivate::CXXWinToastPrivate(CXXWinToast* q)
	: q_ptr(q)
	, templ_(WinToastLib::WinToastTemplate::WinToastTemplateType::Text01)
{

}
CXXWinToastPrivate::CXXWinToastPrivate(CXXWinToast* q, cxx::WinToastType type)
	: q_ptr(q)
	, templ_(static_cast<WinToastLib::WinToastTemplate::WinToastTemplateType>(type))
{

}

CXXWinToastPrivate::~CXXWinToastPrivate()
{

}

bool CXXWinToast::initialize()
{
	return WinToastLib::WinToast::instance()->initialize();
}

CXXWinToast::CXXWinToast()
	: d_ptr(new CXXWinToastPrivate(this))
{

}

CXXWinToast::CXXWinToast(cxx::WinToastType type)
	: d_ptr(new CXXWinToastPrivate(this, type))
{

}

CXXWinToast::~CXXWinToast()
{
}

CXXToastResultCallback& CXXWinToast::toastResultCallbackRef()
{
	return cxx::resultCallback_;
}

cxx::WinToastType CXXWinToast::type() const
{
	CXX_D(const CXXWinToast);
	return d->type_;
}

__int64 CXXWinToast::activateIndex() const
{
	CXXMutexLocker locker(&cxx::mutex_);
	return cxx::actionIndex_;
}

void CXXWinToast::setType(cxx::WinToastType type)
{
	CXX_D(CXXWinToast);
	d->type_ = type;
	d->templ_ = WinToastLib::WinToastTemplate(static_cast<WinToastLib::WinToastTemplate::WinToastTemplateType>(type));
}

void CXXWinToast::setAppName(const CXXString& appName)
{
	WinToastLib::WinToast::instance()->setAppName(appName.toStdWString());
}

void CXXWinToast::setAppUserModelId(const CXXString& companyName,
	const CXXString& productName,
	const CXXString& subProduct,
	const CXXString& versionInformation)
{
	WinToastLib::WinToast::instance()->
		setAppUserModelId(
			WinToastLib::WinToast::configureAUMI(
				companyName.toStdWString(),
				productName.toStdWString(),
				subProduct.toStdWString(),
				versionInformation.toStdWString()
			)
		);
}

void CXXWinToast::setTimeout(__int64 timeout)
{
	CXX_D(CXXWinToast);
	d->templ_.setExpiration(timeout);
}

void CXXWinToast::setAudio(cxx::WinToastAudio audio, cxx::AudioOption option)
{
	CXX_D(CXXWinToast);
	d->templ_.setAudioPath(static_cast<WinToastLib::WinToastTemplate::AudioSystemFile>(audio));
	d->templ_.setAudioOption(static_cast<WinToastLib::WinToastTemplate::AudioOption>(option));
}

void CXXWinToast::setImage(const CXXString& imagePath, cxx::CropHint hint)
{
	CXX_D(CXXWinToast);
	d->templ_.setImagePath(imagePath.toStdWString(), static_cast<WinToastLib::WinToastTemplate::CropHint>(hint));
}

void CXXWinToast::setHeroImage(const CXXString& imagePath, bool isInline)
{
	CXX_D(CXXWinToast);
	d->templ_.setHeroImagePath(imagePath.toStdWString(), isInline);
}

void CXXWinToast::setTextField(const CXXString& text, __int64 fieldIndex)
{
	CXX_D(CXXWinToast);
	if (fieldIndex < 0 || fieldIndex >  WinToastLib::WinToastTemplate::TextField::ThirdLine)
	{
		fieldIndex = WinToastLib::WinToastTemplate::TextField::FirstLine;
	}

	d->templ_.setTextField(text.toStdWString(), static_cast<WinToastLib::WinToastTemplate::TextField>(fieldIndex));
}

void CXXWinToast::setAttributionText(const CXXString& attributionText)
{
	CXX_D(CXXWinToast);
	d->templ_.setAttributionText(attributionText.toStdWString());
}

void CXXWinToast::addAction(const CXXString& label)
{
	CXX_D(CXXWinToast);
	d->templ_.addAction(label.toStdWString());
}

bool CXXWinToast::show() const
{
	CXX_D(const CXXWinToast);
	WinToastLib::WinToast::WinToastError error;
	if (WinToastLib::WinToast::instance()->showToast(d->templ_, new CXXToastHandler(), &error) < 0)
	{
		MessageBoxW(nullptr, WinToastLib::WinToast::strerror(error).c_str(), L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
