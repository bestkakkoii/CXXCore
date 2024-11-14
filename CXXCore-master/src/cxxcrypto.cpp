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

#include <cxxcrypto.h>
#include <bcrypt.h>

#pragma comment (lib, "bcrypt.lib")

class CXXCryptoPrivate
{
public:
	CXXCryptoPrivate(CXXCrypto* q) : q_ptr(q) {}
	~CXXCryptoPrivate() {}

	static CXXString getAlgorithmName(cxx::HashAlgorithm algorithm)
	{
		static const std::map<cxx::HashAlgorithm, CXXString> algorithmNames = {
			{ cxx::HashAlgorithm::MD2, BCRYPT_MD2_ALGORITHM },
			{ cxx::HashAlgorithm::MD4, BCRYPT_MD4_ALGORITHM },
			{ cxx::HashAlgorithm::MD5, BCRYPT_MD5_ALGORITHM },
			{ cxx::HashAlgorithm::SHA1, BCRYPT_SHA1_ALGORITHM },
			{ cxx::HashAlgorithm::SHA256, BCRYPT_SHA256_ALGORITHM },
			{ cxx::HashAlgorithm::SHA384, BCRYPT_SHA384_ALGORITHM },
			{ cxx::HashAlgorithm::SHA512, BCRYPT_SHA512_ALGORITHM }
		};

		auto it = algorithmNames.find(algorithm);
		if (it == algorithmNames.end())
		{
			std::cerr << "Unsupported hash algorithm." << std::endl;
			return L"";
		}

		return it->second;
	}

	static std::wstring getAlgorithmName(cxx::CryptAlgorithm algorithm)
	{
		static const std::map<cxx::CryptAlgorithm, std::wstring> algorithmNames = {
			{ cxx::CryptAlgorithm::RSA, BCRYPT_RSA_ALGORITHM },
			{ cxx::CryptAlgorithm::RSA_SIGN, BCRYPT_RSA_SIGN_ALGORITHM },
			{ cxx::CryptAlgorithm::DH, BCRYPT_DH_ALGORITHM },
			{ cxx::CryptAlgorithm::DSA, BCRYPT_DSA_ALGORITHM },
			{ cxx::CryptAlgorithm::RC2, BCRYPT_RC2_ALGORITHM },
			{ cxx::CryptAlgorithm::RC4, BCRYPT_RC4_ALGORITHM },
			{ cxx::CryptAlgorithm::AES, BCRYPT_AES_ALGORITHM },
			{ cxx::CryptAlgorithm::DES, BCRYPT_DES_ALGORITHM },
			{ cxx::CryptAlgorithm::DESX, BCRYPT_DESX_ALGORITHM },
			{ cxx::CryptAlgorithm::TripleDES, BCRYPT_3DES_ALGORITHM },
			{ cxx::CryptAlgorithm::TripleDES_112, BCRYPT_3DES_112_ALGORITHM },
			{ cxx::CryptAlgorithm::AES_GMAC, BCRYPT_AES_GMAC_ALGORITHM },
			{ cxx::CryptAlgorithm::AES_CMAC, BCRYPT_AES_CMAC_ALGORITHM },
			{ cxx::CryptAlgorithm::ECDSA_P256, BCRYPT_ECDSA_P256_ALGORITHM },
			{ cxx::CryptAlgorithm::ECDSA_P384, BCRYPT_ECDSA_P384_ALGORITHM },
			{ cxx::CryptAlgorithm::ECDSA_P521, BCRYPT_ECDSA_P521_ALGORITHM },
			{ cxx::CryptAlgorithm::ECDH_P256, BCRYPT_ECDH_P256_ALGORITHM },
			{ cxx::CryptAlgorithm::ECDH_P384, BCRYPT_ECDH_P384_ALGORITHM },
			{ cxx::CryptAlgorithm::ECDH_P521, BCRYPT_ECDH_P521_ALGORITHM },
			{ cxx::CryptAlgorithm::RNG_FIPS186_DSA, BCRYPT_RNG_FIPS186_DSA_ALGORITHM },
			{ cxx::CryptAlgorithm::RNG_DUAL_EC, BCRYPT_RNG_DUAL_EC_ALGORITHM }
		};

		auto it = algorithmNames.find(algorithm);
		if (it == algorithmNames.end())
		{
			std::cerr << "Unsupported crypt algorithm." << std::endl;
			return L"";
		}

		return it->second;
	}

	static std::wstring getChainModeName(cxx::ChainMode chainMode)
	{
		static const std::map<cxx::ChainMode, std::wstring> chainModeNames = {
			{ cxx::ChainMode::NA, L"" },
			{ cxx::ChainMode::CBC, BCRYPT_CHAIN_MODE_CBC },
			{ cxx::ChainMode::ECB, BCRYPT_CHAIN_MODE_ECB },
			{ cxx::ChainMode::CFB, BCRYPT_CHAIN_MODE_CFB },
			{ cxx::ChainMode::CCM, BCRYPT_CHAIN_MODE_CCM },
			{ cxx::ChainMode::GCM, BCRYPT_CHAIN_MODE_GCM }
		};

		auto it = chainModeNames.find(chainMode);
		if (it == chainModeNames.end())
		{
			std::cerr << "Unsupported chain mode." << std::endl;
			return L"";
		}

		return it->second;
	}

	// 生成加密密钥
	static bool generateSymmetricKey(BCRYPT_ALG_HANDLE hAlgorithm, BCRYPT_KEY_HANDLE* hKey, const CXXByteArray& key)
	{
		NTSTATUS status = BCryptGenerateSymmetricKey(hAlgorithm, hKey, nullptr, 0, const_cast<PUCHAR>(key.data()), (ULONG)key.size(), 0);
		if (!BCRYPT_SUCCESS(status))
		{
			return false;
		}
		return true;
	}

	// 封装 BCryptOpenAlgorithmProvider
	static bool openAlgorithmProvider(BCRYPT_ALG_HANDLE* hAlgorithm, const std::wstring& algorithmName)
	{
		NTSTATUS status = BCryptOpenAlgorithmProvider(hAlgorithm, algorithmName.c_str(), nullptr, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			return false;
		}
		return true;
	}

	// 封装资源清理
	static void closeAlgorithmProvider(BCRYPT_ALG_HANDLE& hAlgorithm)
	{
		if (hAlgorithm != nullptr)
		{
			BCryptCloseAlgorithmProvider(hAlgorithm, 0);
			hAlgorithm = nullptr;
		}
	}

	static void destroyKey(BCRYPT_KEY_HANDLE& hKey)
	{
		if (hKey != nullptr)
		{
			BCryptDestroyKey(hKey);
			hKey = nullptr;
		}
	}

	static bool encrypt(BCRYPT_KEY_HANDLE hKey, const BYTE* inputData, ULONG inputSize, BYTE* outputData, ULONG outputSize, DWORD& encryptedSize, ULONG flags = 0)
	{
		NTSTATUS status = BCryptEncrypt(hKey, const_cast<BYTE*>(inputData), inputSize, nullptr, nullptr, 0, outputData, outputSize, &encryptedSize, flags);
		if (!BCRYPT_SUCCESS(status))
		{
			std::wcerr << L"BCryptEncrypt failed with status: " << status << std::endl;
			return false;
		}
		return true;
	}

	static bool encryptData(BCRYPT_KEY_HANDLE hKey, const BYTE* inputData, ULONG inputSize, BYTE* outputData, ULONG outputSize, DWORD& encryptedSize, cxx::CryptAlgorithm algorithm)
	{
		NTSTATUS status;

		if (algorithm == cxx::CryptAlgorithm::RSA ||
			algorithm == cxx::CryptAlgorithm::RSA_SIGN ||
			algorithm == cxx::CryptAlgorithm::DH ||
			algorithm == cxx::CryptAlgorithm::DSA ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P256 ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P384 ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P521 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P256 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P384 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P521)
		{
			// 非对称加密的处理
			status = encrypt(hKey, const_cast<BYTE*>(inputData), inputSize, outputData, outputSize, encryptedSize, BCRYPT_PAD_PKCS1);
		}
		else
		{
			// 对称加密的处理
			status = encrypt(hKey, const_cast<BYTE*>(inputData), inputSize, outputData, outputSize, encryptedSize, 0);
		}

		if (!BCRYPT_SUCCESS(status))
		{
			return false;
		}
		return true;
	}

	static bool decrypt(BCRYPT_KEY_HANDLE hKey, const BYTE* data, ULONG dataSize, CXXByteArray* output, ULONG flags = 0)
	{
		DWORD decryptedSize = 0;
		NTSTATUS status = BCryptDecrypt(hKey, const_cast<BYTE*>(data), dataSize, nullptr, nullptr, 0, output->data(), (ULONG)output->size(), &decryptedSize, flags);
		if (!BCRYPT_SUCCESS(status))
		{
			std::wcerr << L"BCryptDecrypt failed with status: " << status << std::endl;
			return false;
		}
		return true;
	}

	static bool decryptData(BCRYPT_KEY_HANDLE hKey, const BYTE* data, ULONG dataSize, CXXByteArray* output, cxx::CryptAlgorithm algorithm)
	{
		NTSTATUS status;

		if (algorithm == cxx::CryptAlgorithm::RSA ||
			algorithm == cxx::CryptAlgorithm::RSA_SIGN ||
			algorithm == cxx::CryptAlgorithm::DH ||
			algorithm == cxx::CryptAlgorithm::DSA ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P256 ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P384 ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P521 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P256 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P384 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P521)
		{
			// 非对称加密的处理
			status = decrypt(hKey, data, dataSize, output, BCRYPT_PAD_PKCS1);
		}
		else
		{
			// 对称加密的处理
			status = decrypt(hKey, data, dataSize, output, 0);
		}

		if (!BCRYPT_SUCCESS(status))
		{
			return false;
		}
		return true;
	}

	static bool importKeyPair(BCRYPT_ALG_HANDLE hAlgorithm, BCRYPT_KEY_HANDLE* hKey, const CXXByteArray& key, const std::wstring& blobType)
	{
		NTSTATUS status = BCryptImportKeyPair(hAlgorithm, nullptr, blobType.c_str(), hKey, const_cast<PUCHAR>(key.data()), (ULONG)key.size(), 0);
		if (!BCRYPT_SUCCESS(status))
		{
			std::wcerr << L"BCryptImportKeyPair failed with status: " << status << std::endl;
			return false;
		}
		return true;
	}

	// 获取块大小
	static bool getBlockSize(BCRYPT_ALG_HANDLE hAlgorithm, DWORD* blockSize)
	{
		DWORD result = 0;
		NTSTATUS status = BCryptGetProperty(hAlgorithm, BCRYPT_BLOCK_LENGTH, reinterpret_cast<PUCHAR>(blockSize), sizeof(DWORD), &result, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			return false;
		}

		return true;
	}

	// 设置加密模式（例如 ECB 模式）
	static bool setECBMode(BCRYPT_ALG_HANDLE hAlgorithm)
	{
		NTSTATUS status = BCryptSetProperty(hAlgorithm, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_ECB, sizeof(BCRYPT_CHAIN_MODE_ECB), 0);
		if (!BCRYPT_SUCCESS(status))
		{
			return false;
		}
		return true;
	}

	// 设置加密模式（例如 CBC 模式）
	static bool setCBCMode(BCRYPT_ALG_HANDLE hAlgorithm)
	{
		NTSTATUS status = BCryptSetProperty(hAlgorithm, BCRYPT_CHAINING_MODE,
			(PUCHAR)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
		return BCRYPT_SUCCESS(status);
	}

	// 判断是否应该跳过 ECB 模式
	static bool shouldSkipECB(cxx::CryptAlgorithm algorithm)
	{
		switch (algorithm)
		{
		case cxx::CryptAlgorithm::RSA:
		case cxx::CryptAlgorithm::RSA_SIGN:
		case cxx::CryptAlgorithm::DH:
		case cxx::CryptAlgorithm::DSA:
		case cxx::CryptAlgorithm::ECDSA_P256:
		case cxx::CryptAlgorithm::ECDSA_P384:
		case cxx::CryptAlgorithm::ECDSA_P521:
		case cxx::CryptAlgorithm::ECDH_P256:
		case cxx::CryptAlgorithm::ECDH_P384:
		case cxx::CryptAlgorithm::ECDH_P521:
		case cxx::CryptAlgorithm::RC4:
		case cxx::CryptAlgorithm::AES_GMAC:
		case cxx::CryptAlgorithm::AES_CMAC:
			return true;  // 这些算法不适用 ECB 模式
		default:
			return false;  // 其他算法使用 ECB 模式
		}
	}

	// 判断是否支持 CBC 模式
	static bool supportsCBC(cxx::CryptAlgorithm algorithm)
	{
		switch (algorithm)
		{
		case cxx::CryptAlgorithm::AES:
		case cxx::CryptAlgorithm::DES:
		case cxx::CryptAlgorithm::DESX:
		case cxx::CryptAlgorithm::TripleDES:
		case cxx::CryptAlgorithm::TripleDES_112:
			return true;
		default:
			return false;
		}
	}

	// 判断数据是否为可打印文本
	static bool isPrintableText(const CXXByteArray& data)
	{
		for (const auto& byte : data)
		{
			if (byte < 32 || byte > 126)
			{
				return false;  // 检测到非可打印字符
			}
		}
		return true;
	}

	// PKCS#7 填充
	static CXXByteArray applyPKCS7Padding(const BYTE* data, size_t dataSize, size_t blockSize)
	{
		if (dataSize % blockSize == 0)
		{
			return CXXByteArray(data, data + dataSize);  // 数据已对齐，不需填充
		}

		size_t paddingSize = blockSize - (dataSize % blockSize);
		CXXByteArray paddedData(data, data + dataSize);
		paddedData.insert(paddedData.end(), paddingSize, (BYTE)paddingSize);  // 添加填充
		return paddedData;
	}

	// 存储密钥
	void setKey(const CXXByteArray& key)
	{
		if (key.empty())
		{
			throw std::runtime_error("Key cannot be empty");
		}
		key_ = key;
	}

	// 存储初始化向量
	void setIV(const CXXByteArray& iv)
	{
		if (iv.empty())
		{
			throw std::runtime_error("IV cannot be empty");
		}
		iv_ = iv;
	}

	// 获取存储的密钥
	const CXXByteArray& getKey() const
	{
		if (key_.empty())
		{
			throw std::runtime_error("Key is not set");
		}
		return key_;
	}

	// 获取存储的初始化向量
	const CXXByteArray& getIV() const
	{
		if (iv_.empty())
		{
			throw std::runtime_error("IV is not set");
		}
		return iv_;
	}

private:
	CXXByteArray key_;  // 存储密钥
	CXXByteArray iv_;   // 存储初始化向量
	CXXByteArray outputBuffer_;  // 存储加密后的数据

private:
	CXX_DECLARE_PUBLIC(CXXCrypto);
	CXXCrypto* q_ptr = CXX_NULLPTR;
};

CXXCrypto::CXXCrypto()
	: d_ptr(new CXXCryptoPrivate(this))
{

}

CXXCrypto::~CXXCrypto()
{

}

// 设置密钥，输入为文本
void CXXCrypto::setKey(const CXXString& key)
{
	std::string keyWStr = key.toUtf8();
	CXXVector<BYTE> keyBytes(keyWStr.begin(), keyWStr.end());
	d_ptr->setKey(keyBytes);
}

// 设置初始化向量，输入为文本
void CXXCrypto::setIV(const CXXString& iv)
{
	std::string ivWStr = iv.toUtf8();
	CXXVector<BYTE> ivBytes(ivWStr.begin(), ivWStr.end());
	d_ptr->setIV(ivBytes);
}

// 设置初始化向量，输入为字节集
void CXXCrypto::setIV(const CXXVector<BYTE>& iv)
{
	d_ptr->setIV(iv);
}

// 设置密钥，输入为字节集
void CXXCrypto::setKey(const CXXVector<BYTE>& key)
{
	d_ptr->setKey(key);
}

bool CXXCrypto::encryptCBC(const CXXString& input)
{
	try
	{
		d_ptr->outputBuffer_.clear();
		std::string inputStr = input.toUtf8();
		return encryptDataCBC(reinterpret_cast<const BYTE*>(inputStr.data()),
			inputStr.size(),
			&d_ptr->outputBuffer_);
	}
	catch (...)
	{
		return false;
	}
}

bool CXXCrypto::encryptCBC(const CXXVector<BYTE>& input)
{
	return encryptDataCBC(input.data(), input.size(), &d_ptr->outputBuffer_);
}

bool CXXCrypto::decryptCBC(const CXXString& input)
{
	try
	{
		d_ptr->outputBuffer_.clear();
		CXXVector<BYTE> inputBytes = CXXString::fromHexString(input);
		if (!decryptDataCBC(inputBytes.data(), inputBytes.size(), &d_ptr->outputBuffer_))
		{
			return false;
		}

		// Remove PKCS7 padding
		if (!d_ptr->outputBuffer_.empty())
		{
			size_t paddingSize = d_ptr->outputBuffer_.back();
			if (paddingSize <= d_ptr->outputBuffer_.size())
			{
				d_ptr->outputBuffer_.resize(d_ptr->outputBuffer_.size() - paddingSize);
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool CXXCrypto::decryptCBC(const CXXVector<BYTE>& input)
{
	return decryptDataCBC(input.data(), input.size(), &d_ptr->outputBuffer_);
}

// 提供加密操作，输入为文本形式的 CXXString，返回加密后的 CXXString
bool CXXCrypto::encrypt(const CXXString& input, cxx::CryptAlgorithm algorithm)
{
	try
	{
		d_ptr->outputBuffer_.clear();
		std::string inputStr = input.toUtf8();
		if (!encryptData(reinterpret_cast<const BYTE*>(inputStr.data()), inputStr.size(), &d_ptr->outputBuffer_, algorithm))
		{
			return false;
		}

		return 	d_ptr->outputBuffer_.size() > 0;
	}
	catch (...)
	{
		return false;
	}
}

// 提供解密操作，输入为加密后的 CXXString，返回解密后的 CXXString（文本）
bool CXXCrypto::decrypt(const CXXString& input, cxx::CryptAlgorithm algorithm)
{
	try
	{
		d_ptr->outputBuffer_.clear();
		CXXVector<BYTE> inputBytes = CXXString::fromHexString(input);
		if (!decryptData(inputBytes.data(), static_cast<size_t>(inputBytes.size()), &d_ptr->outputBuffer_, algorithm))
		{
			return false;
		}

		// 去除多余的大小
		d_ptr->outputBuffer_.resize(d_ptr->outputBuffer_.size() - d_ptr->outputBuffer_.back());

		return 	d_ptr->outputBuffer_.size() > 0;
	}
	catch (...)
	{
		return false;
	}
}

// 提供加密操作，输入为字节集，返回加密后的字节集
bool CXXCrypto::encrypt(const CXXVector<BYTE>& input, cxx::CryptAlgorithm algorithm)
{
	return encryptData(input.data(), static_cast<size_t>(input.size()), &d_ptr->outputBuffer_, algorithm);
}

// 提供解密操作，输入为加密后的字节集，返回解密后的字节集
bool CXXCrypto::decrypt(const CXXVector<BYTE>& input, cxx::CryptAlgorithm algorithm)
{
	return decryptData(input.data(), static_cast<size_t>(input.size()), &d_ptr->outputBuffer_, algorithm);
}

//@备注 提供加密操作，输入为字节集，返回加密后的文本
//@别名 到文本()
CXXString CXXCrypto::toString() const
{
	if (d_ptr->isPrintableText(d_ptr->outputBuffer_))
	{
		std::string outputStr(d_ptr->outputBuffer_.begin(), d_ptr->outputBuffer_.end());

		return CXXString::fromUtf8(outputStr.c_str());
	}
	else
	{
		// 否则，转换为十六进制
		return CXXString::toHexString(d_ptr->outputBuffer_);
	}
}

//@备注 提供解密操作，输入为加密后的字节集，返回解密后的字节集
//@别名 到字节集()
CXXVector<BYTE> CXXCrypto::toByteArray() const
{
	return d_ptr->outputBuffer_;
}

CXXString CXXCrypto::toBase64() const
{
	return CXXString::toBase64(d_ptr->outputBuffer_);
}

bool CXXCrypto::handleAsymmetricEncryption(BCRYPT_ALG_HANDLE hAlgorithm, const BYTE* data, __int64 dataSize, CXXByteArray* output, cxx::CryptAlgorithm algorithm) const
{
	BCRYPT_KEY_HANDLE hKey = nullptr;
	bool success = false;
	DWORD requiredSize = 0;
	DWORD encryptedSize = 0;
	const CXXVector<BYTE>& key = d_ptr->getKey(); // 获取密钥

	do
	{
		if (algorithm == cxx::CryptAlgorithm::RSA || algorithm == cxx::CryptAlgorithm::RSA_SIGN)
		{
			// 使用封装的 importKeyPair 导入 RSA 公钥
			if (!CXXCryptoPrivate::importKeyPair(hAlgorithm, &hKey, key, BCRYPT_RSAPUBLIC_BLOB))
			{
				break;
			}

			// 获取加密后的所需空间
			if (!CXXCryptoPrivate::encrypt(hKey, data, (ULONG)dataSize, nullptr, 0, requiredSize, BCRYPT_PAD_PKCS1))
			{
				break;
			}

			output->resize(requiredSize);

			// 执行 RSA 加密
			if (!CXXCryptoPrivate::encrypt(hKey, data, (ULONG)dataSize, output->data(), (ULONG)output->size(), encryptedSize, BCRYPT_PAD_PKCS1))
			{
				break;
			}

			output->resize(encryptedSize);
			success = true;
		}
		else if (algorithm == cxx::CryptAlgorithm::DH || algorithm == cxx::CryptAlgorithm::DSA ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P256 || algorithm == cxx::CryptAlgorithm::ECDSA_P384 ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P521 || algorithm == cxx::CryptAlgorithm::ECDH_P256 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P384 || algorithm == cxx::CryptAlgorithm::ECDH_P521)
		{
			// 其他非对称加密算法的处理逻辑
			std::wcerr << L"Asymmetric encryption not yet implemented for this algorithm." << std::endl;
			break;
		}

	} while (false);

	// 释放密钥
	CXXCryptoPrivate::destroyKey(hKey);
	return success;
}

bool CXXCrypto::handleStreamCipher(BCRYPT_ALG_HANDLE hAlgorithm, const BYTE* data, __int64 dataSize, CXXByteArray* output)
{
	BCRYPT_KEY_HANDLE hKey = nullptr;
	bool success = false;
	DWORD encryptedSize = 0;

	do
	{
		// RC4 不使用块操作，直接生成密钥并进行加密
		CXXVector<BYTE> key = d_ptr->getKey();

		if (!CXXCryptoPrivate::generateSymmetricKey(hAlgorithm, &hKey, key))
		{
			std::wcerr << L"Failed to generate symmetric key for RC4." << std::endl;
			break;
		}

		// 进行流加密（RC4）
		if (!CXXCryptoPrivate::encryptData(hKey, data, (ULONG)dataSize, nullptr, 0, encryptedSize, cxx::CryptAlgorithm::RC4))
		{
			std::wcerr << L"Failed to encrypt data for RC4." << std::endl;
			break;
		}

		output->resize(encryptedSize);
		success = true;

	} while (false);

	CXXCryptoPrivate::destroyKey(hKey);
	return success;
}

// 私有方法，使用 BCrypt API 进行加密处理，返回加密后的字节集
bool CXXCrypto::encryptData(const BYTE* data, __int64 dataSize, CXXByteArray* output, cxx::CryptAlgorithm algorithm)
{
	BCRYPT_ALG_HANDLE hAlgorithm = nullptr;
	BCRYPT_KEY_HANDLE hKey = nullptr;
	bool success = false;

	do
	{
		// 获取算法名称
		std::wstring algorithmName = CXXCryptoPrivate::getAlgorithmName(algorithm);
		if (algorithmName.empty())
		{
			std::wcerr << L"Unsupported algorithm." << std::endl;
			break;
		}

		// 打开算法提供者
		if (!CXXCryptoPrivate::openAlgorithmProvider(&hAlgorithm, algorithmName))
		{
			std::wcerr << L"Failed to open algorithm provider for: " << algorithmName << std::endl;
			break;
		}

		// 处理非对称加密（RSA, DH, DSA, ECDSA, ECDH等）
		if (algorithm == cxx::CryptAlgorithm::RSA || algorithm == cxx::CryptAlgorithm::RSA_SIGN ||
			algorithm == cxx::CryptAlgorithm::DH || algorithm == cxx::CryptAlgorithm::DSA ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P256 || algorithm == cxx::CryptAlgorithm::ECDSA_P384 ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P521 || algorithm == cxx::CryptAlgorithm::ECDH_P256 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P384 || algorithm == cxx::CryptAlgorithm::ECDH_P521)
		{
			// 非对称加密需要使用不同的加密逻辑（通常是 RSA 或 DH 等加密逻辑）
			success = handleAsymmetricEncryption(hAlgorithm, data, dataSize, output, algorithm);
			break;
		}

		// 流加密（RC4 等不需要分块的加密）
		if (algorithm == cxx::CryptAlgorithm::RC4)
		{
			success = handleStreamCipher(hAlgorithm, data, dataSize, output);
			break;
		}

		// 处理对称加密算法（AES, DES, TripleDES 等）
		DWORD blockSize = 0;
		if (!CXXCryptoPrivate::getBlockSize(hAlgorithm, &blockSize))
		{
			std::wcerr << L"Failed to get block size." << std::endl;
			break;
		}

		CXXVector<BYTE> key = d_ptr->getKey();
		key = CXXCryptoPrivate::applyPKCS7Padding(key.data(), static_cast<size_t>(key.size()), blockSize);

		if (!CXXCryptoPrivate::generateSymmetricKey(hAlgorithm, &hKey, key))
		{
			std::wcerr << L"Failed to generate symmetric key." << std::endl;
			break;
		}

		// 确定是否需要跳过 ECB 模式
		if (!CXXCryptoPrivate::shouldSkipECB(algorithm))
		{
			if (!CXXCryptoPrivate::setECBMode(hAlgorithm))
			{
				std::wcerr << L"Failed to set ECB mode." << std::endl;
				break;
			}
		}

		// PKCS#7 填充
		CXXVector<BYTE> paddedData = CXXCryptoPrivate::applyPKCS7Padding(data, static_cast<size_t>(dataSize), static_cast<size_t>(blockSize));

		// 获取所需的输出缓冲区大小
		DWORD requiredSize = 0;
		if (!CXXCryptoPrivate::encryptData(hKey, paddedData.data(), (ULONG)paddedData.size(), nullptr, 0, requiredSize, algorithm))
		{
			std::wcerr << L"Failed to get required size for encryption." << std::endl;
			break;
		}

		// 分配加密后的数据存储空间
		output->resize(requiredSize);

		// 执行加密操作
		DWORD encryptedSize = 0;
		if (!CXXCryptoPrivate::encryptData(hKey, paddedData.data(), (ULONG)paddedData.size(), output->data(), (ULONG)output->size(), encryptedSize, algorithm))
		{
			std::wcerr << L"Failed to encrypt data." << std::endl;
			break;
		}

		output->resize(encryptedSize);
		success = true;

	} while (false);

	CXXCryptoPrivate::destroyKey(hKey);
	CXXCryptoPrivate::closeAlgorithmProvider(hAlgorithm);

	return success;
}

// 私有方法，使用 BCrypt API 进行解密处理，返回解密后的字节集
bool CXXCrypto::decryptData(const BYTE* data, __int64 dataSize, CXXByteArray* output, cxx::CryptAlgorithm algorithm)
{
	BCRYPT_ALG_HANDLE hAlgorithm = nullptr;
	BCRYPT_KEY_HANDLE hKey = nullptr;
	bool success = false;
	const CXXVector<BYTE>& key = d_ptr->getKey(); // 获取密钥

	do
	{
		// 获取算法名称
		std::wstring algorithmName = CXXCryptoPrivate::getAlgorithmName(algorithm);
		if (algorithmName.empty())
		{
			std::wcerr << L"Unsupported algorithm." << std::endl;
			break;
		}

		// 打开算法提供者
		if (!CXXCryptoPrivate::openAlgorithmProvider(&hAlgorithm, algorithmName))
		{
			std::wcerr << L"Failed to open algorithm provider for: " << algorithmName << std::endl;
			break;
		}

		// 处理非对称解密（RSA, DH, DSA, ECDSA, ECDH等）
		if (algorithm == cxx::CryptAlgorithm::RSA || algorithm == cxx::CryptAlgorithm::RSA_SIGN ||
			algorithm == cxx::CryptAlgorithm::DH || algorithm == cxx::CryptAlgorithm::DSA ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P256 || algorithm == cxx::CryptAlgorithm::ECDSA_P384 ||
			algorithm == cxx::CryptAlgorithm::ECDSA_P521 || algorithm == cxx::CryptAlgorithm::ECDH_P256 ||
			algorithm == cxx::CryptAlgorithm::ECDH_P384 || algorithm == cxx::CryptAlgorithm::ECDH_P521)
		{
			// 使用封装的 importKeyPair 导入 RSA 私钥或其他非对称密钥
			if (!CXXCryptoPrivate::importKeyPair(hAlgorithm, &hKey, key, BCRYPT_RSAPRIVATE_BLOB))
			{
				std::wcerr << L"Failed to import key pair for asymmetric decryption." << std::endl;
				break;
			}

			// 获取解密后的所需空间
			if (!CXXCryptoPrivate::decrypt(hKey, data, (ULONG)dataSize, nullptr, BCRYPT_PAD_PKCS1))
			{
				std::wcerr << L"Failed to get required size for asymmetric decryption." << std::endl;
				break;
			}

			output->resize(dataSize);  // 假设解密后的数据大小不超过输入大小

			// 执行非对称解密
			if (!CXXCryptoPrivate::decrypt(hKey, data, (ULONG)dataSize, output, BCRYPT_PAD_PKCS1))
			{
				std::wcerr << L"Failed to decrypt data for asymmetric decryption." << std::endl;
				break;
			}

			success = true;
		}
		else if (algorithm == cxx::CryptAlgorithm::RC4)
		{
			// 流加密解密逻辑
			if (!CXXCryptoPrivate::generateSymmetricKey(hAlgorithm, &hKey, key))
			{
				std::wcerr << L"Failed to generate symmetric key for stream cipher." << std::endl;
				break;
			}

			// 执行流解密
			if (!CXXCryptoPrivate::decrypt(hKey, data, (ULONG)dataSize, output))
			{
				std::wcerr << L"Failed to decrypt data for stream cipher." << std::endl;
				break;
			}

			success = true;
		}
		else
		{
			// 处理对称解密算法（AES, DES, TripleDES等）
			DWORD blockSize = 0;
			if (!CXXCryptoPrivate::getBlockSize(hAlgorithm, &blockSize))
			{
				std::wcerr << L"Failed to get block size for symmetric decryption." << std::endl;
				break;
			}

			// PKCS#7 填充
			CXXVector<BYTE> paddedKey = CXXCryptoPrivate::applyPKCS7Padding(key.data(), static_cast<size_t>(key.size()), blockSize);

			if (!CXXCryptoPrivate::generateSymmetricKey(hAlgorithm, &hKey, paddedKey))
			{
				std::wcerr << L"Failed to generate symmetric key for symmetric decryption." << std::endl;
				break;
			}

			// 确定是否需要跳过 ECB 模式
			if (!CXXCryptoPrivate::shouldSkipECB(algorithm))
			{
				if (!CXXCryptoPrivate::setECBMode(hAlgorithm))
				{
					std::wcerr << L"Failed to set ECB mode for decryption." << std::endl;
					break;
				}
			}

			// 分配解密后的数据存储空间
			output->resize(dataSize);  // 假设解密后的数据大小不超过输入大小

			// 执行对称解密操作
			if (!CXXCryptoPrivate::decrypt(hKey, data, (ULONG)dataSize, output))
			{
				std::wcerr << L"Failed to decrypt data for symmetric decryption." << std::endl;
				break;
			}

			success = true;
		}

	} while (false);

	// 清理资源
	CXXCryptoPrivate::destroyKey(hKey);
	CXXCryptoPrivate::closeAlgorithmProvider(hAlgorithm);

	return success;
}

bool CXXCrypto::encryptDataCBC(const BYTE* data, __int64 dataSize, CXXByteArray* output)
{
	BCRYPT_ALG_HANDLE hAlgorithm = nullptr;
	BCRYPT_KEY_HANDLE hKey = nullptr;
	bool success = false;

	do
	{
		// Open AES algorithm provider
		if (!CXXCryptoPrivate::openAlgorithmProvider(&hAlgorithm, BCRYPT_AES_ALGORITHM))
		{
			break;
		}

		// Set CBC mode
		if (!CXXCryptoPrivate::setCBCMode(hAlgorithm))
		{
			break;
		}

		// Get block size
		DWORD blockSize = 0;
		if (!CXXCryptoPrivate::getBlockSize(hAlgorithm, &blockSize))
		{
			break;
		}

		// Pad key and data
		CXXVector<BYTE> key = d_ptr->getKey();
		key = CXXCryptoPrivate::applyPKCS7Padding(key.data(), key.size(), blockSize);

		// Generate symmetric key
		if (!CXXCryptoPrivate::generateSymmetricKey(hAlgorithm, &hKey, key))
		{
			break;
		}

		// Pad input data
		CXXVector<BYTE> paddedData = CXXCryptoPrivate::applyPKCS7Padding(data, dataSize, blockSize);

		// Get required size for encrypted data
		DWORD requiredSize = 0;
		if (!CXXCryptoPrivate::encrypt(hKey, paddedData.data(), paddedData.size(),
			nullptr, 0, requiredSize, 0))
		{
			break;
		}

		// Resize output buffer
		output->resize(requiredSize);

		// Encrypt data with IV
		BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
		BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
		authInfo.pbNonce = const_cast<BYTE*>(d_ptr->getIV().data());
		authInfo.cbNonce = d_ptr->getIV().size();

		DWORD encryptedSize = 0;
		NTSTATUS status = BCryptEncrypt(hKey, paddedData.data(), paddedData.size(),
			&authInfo, nullptr, 0, output->data(), output->size(),
			&encryptedSize, 0);

		if (!BCRYPT_SUCCESS(status))
		{
			break;
		}

		output->resize(encryptedSize);
		success = true;

	} while (false);

	// Cleanup
	CXXCryptoPrivate::destroyKey(hKey);
	CXXCryptoPrivate::closeAlgorithmProvider(hAlgorithm);

	return success;
}

bool CXXCrypto::decryptDataCBC(const BYTE* data, __int64 dataSize, CXXByteArray* output)
{
	BCRYPT_ALG_HANDLE hAlgorithm = nullptr;
	BCRYPT_KEY_HANDLE hKey = nullptr;
	bool success = false;

	do
	{
		// Open AES algorithm provider
		if (!CXXCryptoPrivate::openAlgorithmProvider(&hAlgorithm, BCRYPT_AES_ALGORITHM))
		{
			break;
		}

		// Set CBC mode
		if (!CXXCryptoPrivate::setCBCMode(hAlgorithm))
		{
			break;
		}

		// Get block size
		DWORD blockSize = 0;
		if (!CXXCryptoPrivate::getBlockSize(hAlgorithm, &blockSize))
		{
			break;
		}

		// Pad key
		CXXVector<BYTE> key = d_ptr->getKey();
		key = CXXCryptoPrivate::applyPKCS7Padding(key.data(), key.size(), blockSize);

		// Generate symmetric key
		if (!CXXCryptoPrivate::generateSymmetricKey(hAlgorithm, &hKey, key))
		{
			break;
		}

		// Prepare output buffer
		output->resize(dataSize);

		// Decrypt data with IV
		BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
		BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
		authInfo.pbNonce = const_cast<BYTE*>(d_ptr->getIV().data());
		authInfo.cbNonce = d_ptr->getIV().size();

		DWORD decryptedSize = 0;
		NTSTATUS status = BCryptDecrypt(hKey, const_cast<BYTE*>(data), dataSize,
			&authInfo, nullptr, 0, output->data(), output->size(),
			&decryptedSize, 0);

		if (!BCRYPT_SUCCESS(status))
		{
			break;
		}

		output->resize(decryptedSize);
		success = true;

	} while (false);

	// Cleanup
	CXXCryptoPrivate::destroyKey(hKey);
	CXXCryptoPrivate::closeAlgorithmProvider(hAlgorithm);

	return success;
}

#pragma region CXXCryptoHash

CXXCryptoHash::CXXCryptoHash(cxx::HashAlgorithm algorithm)
{
	algorithmId_ = CXXCryptoPrivate::getAlgorithmName(algorithm);

	if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&provider_, algorithmId_.c_str(), MS_PRIMITIVE_PROVIDER, BCRYPT_HASH_REUSABLE_FLAG)))
	{
		std::wcerr << "Failed to open algorithm provider for " << algorithmId_.data() << std::endl;
		provider_ = CXX_NULLPTR;
		return;
	}

	ULONG result = 0;
	if (!BCRYPT_SUCCESS(BCryptGetProperty(provider_, BCRYPT_HASH_LENGTH, reinterpret_cast<BYTE*>(&hashLength_), sizeof(ULONG), &result, 0)))
	{
		std::wcerr << "Failed to get hash length for " << algorithmId_.data() << std::endl;
		BCryptCloseAlgorithmProvider(provider_, 0);
		provider_ = CXX_NULLPTR;
		return;
	}

	if (!BCRYPT_SUCCESS(BCryptCreateHash(provider_, &hashObject_, CXX_NULLPTR, 0, CXX_NULLPTR, 0, BCRYPT_HASH_REUSABLE_FLAG)))
	{
		std::wcerr << "Failed to create hash object for " << algorithmId_.data() << std::endl;
		BCryptCloseAlgorithmProvider(provider_, 0);
		provider_ = CXX_NULLPTR;
		return;
	}
}

CXXCryptoHash::~CXXCryptoHash()
{
	if (hashObject_ != CXX_NULLPTR)
	{
		BCryptDestroyHash(hashObject_);
	}

	if (provider_ != CXX_NULLPTR)
	{
		BCryptCloseAlgorithmProvider(provider_, 0);
	}
}

CXXVector<BYTE> CXXCryptoHash::hash(BYTE* src, __int64 size)
{
	if (hashObject_ == CXX_NULLPTR)
	{
		std::wcerr << "Hash object is not initialized." << std::endl;
		return CXXVector<BYTE>();
	}

	if (!BCRYPT_SUCCESS(BCryptHashData(hashObject_, src, (ULONG)size, 0)))
	{
		std::wcerr << "Failed to hash data for " << algorithmId_.data() << std::endl;
		return CXXVector<BYTE>();
	}

	CXXVector<BYTE> readableHash(hashLength_, '\0');
	if (!BCRYPT_SUCCESS(BCryptFinishHash(hashObject_, readableHash.data(), hashLength_, 0)))
	{
		std::wcerr << "Failed to finish hashing for " << algorithmId_.data() << std::endl;
		return CXXVector<BYTE>();
	}

	// The reusable hash object is automatically reset after finishing the hash.

	return readableHash;
}

CXXString CXXCryptoHash::hash(const CXXString& input)
{
	std::string str = input.toUtf8();
	BYTE* src = const_cast<BYTE*>(reinterpret_cast<const BYTE*>(str.c_str()));
	size_t size = static_cast<size_t>(input.size() * sizeof(wchar_t) - 1);
	CXXVector<BYTE> readableHash = hash(src, size);

	std::wstringstream ss;
	for (BYTE byte : readableHash)
	{
		ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<USHORT>(byte);
	}

	return ss.str().c_str();
}

CXXVector<BYTE> CXXCryptoHash::hash(BYTE* src, __int64 size, cxx::HashAlgorithm algorithm)
{
	CXXString algorithmId = CXXCryptoPrivate::getAlgorithmName(algorithm);
	CXXVector<BYTE> readableHash;

	BCRYPT_ALG_HANDLE provider = CXX_NULLPTR;
	BCRYPT_HASH_HANDLE hashObject = CXX_NULLPTR;

	do
	{
		if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&provider, algorithmId.c_str(), MS_PRIMITIVE_PROVIDER, BCRYPT_HASH_REUSABLE_FLAG)))
		{
			std::wcerr << "Failed to open algorithm provider for " << algorithmId.data() << std::endl;
			break;
		}

		ULONG hashLength = 0, result = 0;
		if (!BCRYPT_SUCCESS(BCryptGetProperty(provider, BCRYPT_HASH_LENGTH, reinterpret_cast<BYTE*>(&hashLength), sizeof(ULONG), &result, 0)))
		{
			std::wcerr << "Failed to get hash length for " << algorithmId.data() << std::endl;
			break;
		}

		readableHash = CXXVector<BYTE>(hashLength, '\0');
		if (!BCRYPT_SUCCESS(BCryptCreateHash(provider, &hashObject, CXX_NULLPTR, 0, CXX_NULLPTR, 0, BCRYPT_HASH_REUSABLE_FLAG)))
		{
			std::wcerr << "Failed to create hash object for " << algorithmId.data() << std::endl;
			break;
		}

		if (!BCRYPT_SUCCESS(BCryptHashData(hashObject, src, (ULONG)size, 0)))
		{
			std::wcerr << "Failed to hash data for " << algorithmId.data() << std::endl;
			break;
		}

		if (!BCRYPT_SUCCESS(BCryptFinishHash(hashObject, readableHash.data(), hashLength, 0)))
		{
			std::wcerr << "Failed to finish hashing for " << algorithmId.data() << std::endl;
			break;
		}

	} while (false);

	if (hashObject != CXX_NULLPTR)
	{
		BCryptDestroyHash(hashObject);
	}

	if (provider != CXX_NULLPTR)
	{
		BCryptCloseAlgorithmProvider(provider, 0);
	}

	return readableHash;
}

CXXString CXXCryptoHash::hash(const CXXString& input, cxx::HashAlgorithm algorithm)
{
	BYTE* src = const_cast<BYTE*>(reinterpret_cast<const BYTE*>(input.data()));
	size_t size = static_cast<size_t>(input.size() * sizeof(wchar_t) - 1);
	CXXVector<BYTE> readableHash = hash(src, size, algorithm);

	std::wstringstream ss;
	for (BYTE byte : readableHash)
	{
		ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<USHORT>(byte);
	}

	return ss.str().c_str();
}
#pragma endregion