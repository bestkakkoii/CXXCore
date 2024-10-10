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

#include <cxxfiledialog_p.h>

#include <utility>

CXXVector<std::wstring> CXXFileDialogPrivate::history_;

CXXFileDialogPrivate::CXXFileDialogPrivate(CXXFileDialog* q)
	: q_ptr(q)
{

}

CXXFileDialogPrivate::CXXFileDialogPrivate(CXXFileDialog* q, cxx::FileDialogAcceptMode mode,
	HWND parent,
	const CXXString& caption,
	const CXXString& directory)
	: q_ptr(q)
	, parent_(parent)
	, fileMode_(mode)
	, caption_(caption)
	, directory_(directory)
{

}

void CXXFileDialogPrivate::setDialog(IFileDialog* pDialog)
{
	pDialog_ = pDialog;
}

void CXXFileDialogPrivate::clear()
{
	if (pOpenDialog_ != CXX_NULLPTR)
	{
		pOpenDialog_->Release();
		pOpenDialog_ = CXX_NULLPTR;
	}

	if (pSaveDialog_ != CXX_NULLPTR)
	{
		pSaveDialog_->Release();
		pSaveDialog_ = CXX_NULLPTR;
	}

	if (pDialog_ != CXX_NULLPTR)
	{
		pDialog_ = CXX_NULLPTR;
	}

	//uninstall the COM library
	CoUninitialize();
}

HRESULT CXXFileDialogPrivate::setNameFilters()
{
	if (pDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return E_INVALIDARG;
	}

	if (nameFilters_.isEmpty())
	{
		std::wcerr << L"filter is empty" << std::endl;
		return E_INVALIDARG;
	}

	if (fileMode_ == cxx::FileDialogAcceptMode::OpenDirectory)
	{
		return S_OK;
	}

	CXXVector<std::pair<std::wstring, std::wstring>> rgSpecVec;

	size_t sepPos = 0;
	size_t size = static_cast<size_t>(nameFilters_.size());
	std::wstring name;
	std::wstring spec;
	std::wstring temp;

	for (size_t i = 0; i < size; ++i)
	{
		temp = nameFilters_.value(i);
		if (static_cast<__int64>(temp.size()) <= 0)
		{
			continue;
		}

		sepPos = temp.find(L'|');
		if (sepPos == std::wstring::npos)
		{
			continue;
		}

		name = temp.substr(0, sepPos);
		if (static_cast<__int64>(name.size()) <= 0)
		{
			continue;
		}

		spec = temp.substr(sepPos + 1);
		if (static_cast<__int64>(spec.size()) <= 0)
		{
			continue;
		}

		rgSpecVec.append(std::make_pair(name, spec));
	}

	if (static_cast<__int64>(rgSpecVec.size()) <= 0)
	{
		std::wcerr << L"none of the filter is valid" << std::endl;
		return E_INVALIDARG;
	}

	size = static_cast<size_t>(rgSpecVec.size());

	std::unique_ptr<COMDLG_FILTERSPEC[]> prgSpec(new COMDLG_FILTERSPEC[size]());
	for (size_t i = 0; i < size; ++i)
	{
		prgSpec[i].pszName = rgSpecVec[i].first.c_str();
		prgSpec[i].pszSpec = rgSpecVec[i].second.c_str();
	}

	// 设置档案类型过滤器
	HRESULT hr = pDialog_->SetFileTypes(static_cast<UINT>(size), prgSpec.get());
	if (SUCCEEDED(hr))
	{
		hr = pDialog_->SetFileTypeIndex(1);
	}

	return hr;
}

HRESULT CXXFileDialogPrivate::handleFilesOrDirectoriesSelection()
{
	if (pOpenDialog_ == CXX_NULLPTR)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;
	do
	{
		// Retrieve the selected file(s)
		IShellItemArray* pResults = CXX_NULLPTR;
		hr = pOpenDialog_->GetResults(&pResults);
		if (FAILED(hr))
		{
			break;
		}

		DWORD itemCount = 0;
		hr = pResults->GetCount(&itemCount);
		if (FAILED(hr) || itemCount == 0)
		{
			pResults->Release();
			break;
		}

		// Process the selected file(s)
		for (DWORD i = 0; i < itemCount; ++i)
		{
			IShellItem* pItem = CXX_NULLPTR;
			hr = pResults->GetItemAt(i, &pItem);
			if (FAILED(hr))
			{
				continue;
			}

			PWSTR pszFilePath = CXX_NULLPTR;
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
			if (SUCCEEDED(hr) && pszFilePath != CXX_NULLPTR)
			{
				selectedFiles_.push_back(pszFilePath);
				if (pszFilePath != CXX_NULLPTR && pszFilePath[0] != L'\0')
				{
					history_.push_back(pszFilePath);
				}

				// Don't forget to free the allocated memory for pszFilePath
				CoTaskMemFree(pszFilePath);
			}
			pItem->Release();
		}

		pResults->Release();
	} while (false);

	return hr;
}

HRESULT CXXFileDialogPrivate::handleSaveFileSelection()
{
	if (pSaveDialog_ == CXX_NULLPTR)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;
	do
	{
		// Retrieve the selected file to save
		IShellItem* pResult = CXX_NULLPTR;
		hr = pSaveDialog_->GetResult(&pResult);
		if (FAILED(hr))
		{
			break;
		}

		PWSTR pszFilePath = CXX_NULLPTR;
		hr = pResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
		if (SUCCEEDED(hr) && pszFilePath != CXX_NULLPTR)
		{
			selectedFiles_.push_back(pszFilePath);
			if (pszFilePath != CXX_NULLPTR && pszFilePath[0] != L'\0')
			{
				history_.push_back(pszFilePath);
			}

			// Don't forget to free the allocated memory for pszFilePath
			CoTaskMemFree(pszFilePath);

			// Release the IShellItem
			pResult->Release();
		}

	} while (false);

	return hr;
}

HRESULT CXXFileDialogPrivate::create()
{
	CXX_Q(CXXFileDialog);
	HRESULT hr = S_OK;
	//Create instance of IFileOpenDialog or IFileSaveDialog
	switch (fileMode_)
	{
	case cxx::FileDialogAcceptMode::OpenFile:
	{
		hr = CoCreateInstance(CLSID_FileOpenDialog, CXX_NULLPTR, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pOpenDialog_));
		setDialog(pOpenDialog_);
		q->setOptions(cxx::FileDialogOption::DefaultOpenFile);
		break;
	}
	case cxx::FileDialogAcceptMode::OpenDirectory:
	{
		hr = CoCreateInstance(CLSID_FileOpenDialog, CXX_NULLPTR, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pOpenDialog_));
		setDialog(pOpenDialog_);
		q->setOptions(cxx::FileDialogOption::DefaultOpenDirectory);
		break;
	}
	case cxx::FileDialogAcceptMode::SaveFile:
	{
		hr = CoCreateInstance(CLSID_FileSaveDialog, CXX_NULLPTR, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSaveDialog_));
		setDialog(pSaveDialog_);
		q->setOptions(cxx::FileDialogOption::DefaultSaveFile);
		break;
	}
	default:
	{
		hr = E_FAIL; // 不支持的模式
		break;
	}
	}

	return hr;
}

CXXFileDialog::CXXFileDialog(cxx::FileDialogAcceptMode mode, HWND parent, const CXXString& caption, const CXXString& directory)
	: d_ptr(new CXXFileDialogPrivate(this, mode, parent, caption, directory))
{
	HRESULT hr = CoInitialize(CXX_NULLPTR);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to initialize COM library" << std::endl;
		return;
	}

	CXX_D(CXXFileDialog);
	if (d->fileMode_ != cxx::FileDialogAcceptMode::OpenDirectory)
	{
		d->nameFilters_.append(L"All Files|*.*");
	}

	d->create();
}

CXXFileDialog::CXXFileDialog(__int64 mode, HWND parent = CXX_NULLPTR)
	: d_ptr(new CXXFileDialogPrivate(this, static_cast<cxx::FileDialogAcceptMode>(mode), parent, L"", L""))
{
	HRESULT hr = CoInitialize(CXX_NULLPTR);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to initialize COM library" << std::endl;
		return;
	}

	CXX_D(CXXFileDialog);
	if (d->fileMode_ != cxx::FileDialogAcceptMode::OpenDirectory)
	{
		d->nameFilters_.append(L"All Files|*.*");
	}

	d->create();
}

CXXFileDialog::~CXXFileDialog()
{
	CXX_D(CXXFileDialog);
	d->clear();
}

void CXXFileDialog::setWindowTitle(const CXXString& title)
{
	CXX_D(CXXFileDialog);
	if (d->pDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return;
	}

	if (title.isEmpty())
	{
		return;
	}

	d->pDialog_->SetTitle(title.c_str());
}

void CXXFileDialog::setDefaultFileName(const CXXString& filename)
{
	CXX_D(CXXFileDialog);

	if (CXX_NULLPTR == d->pSaveDialog_)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return;
	}

	d->pSaveDialog_->SetFileName(filename.c_str());
}

void CXXFileDialog::setLabelText(cxx::FileDialogLabel label, const CXXString& text)
{
	CXX_D(CXXFileDialog);
	if (d->pDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return;
	}

	switch (label)
	{
	case cxx::FileDialogLabel::OkButton:
	{
		d->pDialog_->SetOkButtonLabel(text.c_str());
		break;
	}
	case cxx::FileDialogLabel::FileName:
	{
		d->pDialog_->SetFileNameLabel(text.c_str());
		break;
	}
	default:
	{
		std::wcerr << L"invalid label type" << std::endl;
		break;
	}
	}
}

void CXXFileDialog::setDefaultFolder(const CXXString& folder)
{
	CXX_D(CXXFileDialog);

	if (d->pDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return;
	}

	IShellItem* pFolder = CXX_NULLPTR;
	HRESULT hr = SHCreateItemFromParsingName(folder.c_str(), CXX_NULLPTR, IID_PPV_ARGS(&pFolder));
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to create IShellItem from folder path" << std::endl;
		return;
	}

	d->pDialog_->SetFolder(pFolder);
	pFolder->Release();
}

void CXXFileDialog::setSaveAsItem(const CXXString& filePath)
{
	CXX_D(CXXFileDialog);

	if (d->pSaveDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"SaveDialog pointer is null" << std::endl;
		return;
	}

	IShellItem* psi = CXX_NULLPTR;
	HRESULT hr = SHCreateItemFromParsingName(filePath.c_str(), CXX_NULLPTR, IID_PPV_ARGS(&psi));
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to create IShellItem from file path" << std::endl;
		return;
	}

	hr = d->pSaveDialog_->SetSaveAsItem(psi);
	psi->Release();
}

void CXXFileDialog::setDefaultExtension(const CXXString& extension)
{
	CXX_D(CXXFileDialog);

	if (d->pDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return;
	}

	d->pDialog_->SetDefaultExtension(extension.c_str());
}

void CXXFileDialog::setOptions(cxx::FileDialogOptions options)
{
	CXX_D(CXXFileDialog);

	if (d->pDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return;
	}

	DWORD dwOptions;
	HRESULT hr = d->pDialog_->GetOptions(&dwOptions);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to get options" << std::endl;
		return;
	}

	if (dwOptions == static_cast<DWORD>(options))
	{
		return;
	}

	hr = d->pDialog_->SetOptions(static_cast<FILEOPENDIALOGOPTIONS>(d->options_));
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to set options" << std::endl;
	}

	if (d->options_ != options)
	{
		d->options_ = options;
	}
}

void CXXFileDialog::setOption(cxx::FileDialogOption option, bool on)
{
	CXX_D(CXXFileDialog);

	if (d->pDialog_ == CXX_NULLPTR)
	{
		std::wcerr << L"dialog pointer is null" << std::endl;
		return;
	}

	//get old options
	DWORD dwOptions;
	HRESULT hr = d->pDialog_->GetOptions(&dwOptions);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to get options" << std::endl;
		return;
	}

	bool b = (dwOptions & static_cast<DWORD>(option)) == static_cast<DWORD>(option);

	//set new options
	if (on)
	{
		if (b)
			return;
		dwOptions |= static_cast<DWORD>(option);
	}
	else
	{
		if (!b)
			return;
		dwOptions &= ~static_cast<DWORD>(option);
	}

	hr = d->pDialog_->SetOptions(dwOptions);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to set options" << std::endl;
	}

	if (static_cast<DWORD>(d->options_) != dwOptions)
	{
		d->options_ = static_cast<cxx::FileDialogOptions>(dwOptions);
	}
}

bool CXXFileDialog::isValid() const
{
	CXX_D(const CXXFileDialog);
	return d->pDialog_ != CXX_NULLPTR;
}

bool CXXFileDialog::exec()
{
	CXX_D(CXXFileDialog);

	HRESULT hr = S_OK;
	do
	{
		if (d->pDialog_ == CXX_NULLPTR)
		{
			hr = d->create();
			if (FAILED(hr))
			{
				break;
			}
		}

		//设置 Options
		setOptions(d->options_);

		// 设置文件过滤器
		hr = d->setNameFilters();
		if (FAILED(hr))
		{
			break;
		}

		// 设置窗口标题
		setWindowTitle(d->caption_);

		hr = d->pDialog_->Show(d->parent_);
		if (FAILED(hr))
		{
			break;
		}

		// 根据不同的文件接受模式设置对话框类型
		switch (d->fileMode_)
		{
		case cxx::FileDialogAcceptMode::OpenFile:
		case cxx::FileDialogAcceptMode::OpenDirectory:
		{
			hr = d->handleFilesOrDirectoriesSelection();
			break;
		}
		case cxx::FileDialogAcceptMode::SaveFile:
		{
			hr = d->handleSaveFileSelection();
			break;
		}
		default:
			std::wcerr << L"Invalid file mode" << std::endl;
			hr = E_FAIL;
			break;
		}

		if (FAILED(hr))
		{
			break;
		}
	} while (false);

	d->clear();

	return SUCCEEDED(hr);
}

//@备注 获取父窗口句柄
//@返回 父窗口句柄
//@别名 父窗口()
HWND CXXFileDialog::parent() const
{
	CXX_D(const CXXFileDialog);
	return d->parent_;
}

//@备注 获取目录
//@返回 目录
//@别名 目录()
CXXString CXXFileDialog::directory() const
{
	CXX_D(const CXXFileDialog);
	return d->directory_.c_str();
}

//@备注 获取文件模式
//@返回 文件模式
//@别名 模式()
cxx::FileDialogAcceptMode CXXFileDialog::fileMode() const
{
	CXX_D(const CXXFileDialog);
	return d->fileMode_;
}

//@备注 获取历史纪录
//@返回 历史纪录
//@别名 纪录()
CXXVector<CXXString> CXXFileDialog::history() const
{
	CXX_D(const CXXFileDialog);
	CXXVector<CXXString> files;
	for (const std::wstring& file : d->history_)
	{
		files.push_back(file);
	}
	return files;
}

//@备注 获取过滤器 
//@返回 过滤器
//@别名 过滤器()
CXXVector<CXXString> CXXFileDialog::nameFilters() const
{
	CXX_D(const CXXFileDialog);

	CXXVector<CXXString> filters;
	for (const std::wstring& filter : d->nameFilters_)
	{
		filters.push_back(filter);
	}
	return filters;
}

//@备注 获取选项
//@返回 选项
//@别名 选项()
cxx::FileDialogOptions CXXFileDialog::options() const
{
	CXX_D(const CXXFileDialog);
	return d->options_;
}

//@备注 设置过滤器 格式: "文本文件|*.txt" 或 "所有文件|*.*"
//@参数 单个过滤器
//@返回 无
//@别名 置过滤器(过滤格式字符串)
void CXXFileDialog::setFilter(const CXXString& filter)
{
	CXX_D(CXXFileDialog);
	d->nameFilters_.clear();
	d->nameFilters_.push_back(filter.c_str());
}

//@备注 设置过滤器 格式: { "文本文件|*.txt", "所有文件|*.*" }
//@参数 多个过滤器
//@返回 无
//@别名 置过滤器()
void CXXFileDialog::setFilter(const CXXVector<CXXString>& filters)
{
	CXX_D(CXXFileDialog);
	d->nameFilters_.clear();
	for (const CXXString& filter : filters)
	{
		d->nameFilters_.push_back(filter.c_str());
	}
}

//@备注 获取已选择的文件或目录
//@返回 已选择的文件或目录
//@别名 已选择()
CXXVector<CXXString> CXXFileDialog::selectedFiles() const
{
	CXX_D(const CXXFileDialog);
	CXXVector<CXXString> files;
	for (const std::wstring& file : d->selectedFiles_)
	{
		files.push_back(file);
	}
	return files;
}

//@备注 设置起始目录
//@参数 目录
//@返回 无
//@别名 置目录(目录路径)
void CXXFileDialog::setDirectory(const CXXString& directory)
{
	CXX_D(CXXFileDialog);
	d->directory_ = directory.c_str();
}

//@备注 设置文件模式
//@参数 文件模式
//@返回 无
//@别名 置模式(文件模式)
void CXXFileDialog::setFileMode(cxx::FileDialogAcceptMode mode)
{
	CXX_D(CXXFileDialog);
	d->fileMode_ = mode;
}

CXXVector<CXXString> cxxCreateOpenFileDialog(const CXXString& directory, const CXXString& caption, HWND parent)
{
	static CXXVector<CXXString> empty;
	CXXFileDialog dialog(cxx::FileDialogAcceptMode::OpenFile, parent, caption, directory);
	if (dialog.exec())
	{
		return dialog.selectedFiles();
	}

	return empty;
}

CXXVector<CXXString> cxxCreateOpenDirectoryDialog(const CXXString& directory, const CXXString& caption, HWND parent)
{
	static CXXVector<CXXString> empty;
	CXXFileDialog dialog(cxx::FileDialogAcceptMode::OpenDirectory, parent, caption, directory);
	if (dialog.exec())
	{
		return dialog.selectedFiles();
	}

	return empty;
}

CXXVector<CXXString> cxxCreateSaveFileDialog(const CXXString& directory, const CXXString& caption, HWND parent)
{
	static CXXVector<CXXString> empty;
	CXXFileDialog dialog(cxx::FileDialogAcceptMode::SaveFile, parent, caption, directory);
	if (dialog.exec())
	{
		return dialog.selectedFiles();
	}

	return empty;
}