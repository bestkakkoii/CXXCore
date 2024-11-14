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

#ifndef CXXCRYPTO_H_
//@隐藏{
#define CXXCRYPTO_H_
//@隐藏}



#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>

#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxvector.h>
#include <cxxscopedpointer.h>

//@别名 炫
namespace cxx
{

}

//@分组{［文本］.炫　加密类

//@隐藏{
using BCRYPT_HANDLE = PVOID;
using BCRYPT_ALG_HANDLE = PVOID;
using BCRYPT_KEY_HANDLE = PVOID;
using BCRYPT_HASH_HANDLE = PVOID;
using BCRYPT_SECRET_HANDLE = PVOID;
//@隐藏}

//@别名 炫哈希算法器
//@备注 基于微软 BCrypt API 的哈希算法器，用于将数据转换为固定长度的散列值
class CXXCryptoHash
{
public:
	//@别名 构造哈希器(哈希算法类型枚举)
	CXXCryptoHash(cxx::HashAlgorithm algorithm);
	//@别名 析构哈希器
	virtual ~CXXCryptoHash();

	//@返回 哈希值字节动数组
	//@别名 哈希(字节集指针, 字节集大小)
	CXXVector<BYTE> hash(BYTE* src, __int64 size);

	//@返回 十六进制哈希值文本
	//@别名 哈希(来源文本)
	CXXString hash(const CXXString& input);

	//@返回 哈希值字节动数组
	//@别名 哈希(字节集指针, 字节集大小, 哈希算法类型枚举)
	static CXXVector<BYTE> hash(BYTE* src, __int64 size, cxx::HashAlgorithm algorithm);

	//@返回 十六进制哈希值文本
	//@别名 哈希(来源文本, 哈希算法类型枚举)
	static CXXString hash(const CXXString& input, cxx::HashAlgorithm algorithm);

private:
	//@隐藏{

	BCRYPT_ALG_HANDLE provider_ = CXX_NULLPTR;
	BCRYPT_HASH_HANDLE hashObject_ = CXX_NULLPTR;
	ULONG hashLength_ = 0;
	CXXString algorithmId_;
	//@隐藏}
};

//@隐藏{
class CXXCryptoPrivate;
//@隐藏}

//@备注 基于微软 BCrypt API 文字或字节集(二进制数据)加解密
// AES的密钥和数据 必须是16字节的倍数，假设不是则内部会自动使用 PKCS7 填充
// DES的密钥和数据 必须是8字节的倍数，假设不是则内部会自动使用 PKCS7 填充
//@别名 炫加密
class CXXCrypto
{
public:
	CXXCrypto();
	virtual ~CXXCrypto();

	//@备注 设置密钥，输入为文本
	//@参数 key 密钥文本
	//@返回 无
	//@别名 置密钥(文本)
	void setKey(const CXXString& key);

	//@备注 设置密钥，输入为字节集
	//@参数 key 密钥字节集
	//@返回 无
	//@别名 置密钥(字节集)
	void setKey(const CXXVector<BYTE>& key);

	//@备註 设置初始化向量，输入为文本
	//@参数 iv 初始化向量文本
	//@返回 无
	//@别名 置初始化向量(文本)
	void setIV(const CXXString& iv);

	//@备註 设置初始化向量，输入为字节集
	//@参数 iv 初始化向量字节集
	//@返回 无
	//@别名 置初始化向量(字节集)
	void setIV(const CXXVector<BYTE>& iv);

	//@备註 使用AES-CBC模式加密，输入为文本
	//@参数 input 输入文本
	//@返回 是否加密成功
	//@别名 CBC加密(文本)
	bool encryptCBC(const CXXString& input);

	//@备註 使用AES-CBC模式加密，输入为字节集
	//@参数 input 输入字节集
	//@返回 是否加密成功
	//@别名 CBC加密(字节集)
	bool encryptCBC(const CXXVector<BYTE>& input);

	//@备註 使用AES-CBC模式解密，输入为加密后的文本
	//@参数 input 输入文本
	//@返回 是否解密成功
	//@别名 CBC解密(文本)
	bool decryptCBC(const CXXString& input);

	//@备註 使用AES-CBC模式解密，输入为加密后的字节集
	//@参数 input 输入字节集
	//@返回 是否解密成功
	//@别名 CBC解密(字节集)
	bool decryptCBC(const CXXVector<BYTE>& input);

	//@备注 提供加密操作，输入为文本形式的 CXXString，返回加密后的 CXXString
	// 成功后数据会保存于缓存中，可通过 toString() 或 toByteArray() 获取
	//@别名 加密(文本, 加密算法类型枚举)
	//@备注 该方法使用 BCrypt API 进行加密处理
	//@参数 input 输入文本
	//@参数 algorithm 加密算法类型
	//@返回 是否加密成功
	//@别名 加密(文本, 文本, 加密算法类型枚举)
	bool encrypt(const CXXString& input, cxx::CryptAlgorithm algorithm);

	//@备注 提供解密操作，输入为加密后的 CXXString，返回解密后的 CXXString（文本）
	// 成功后数据会保存于缓存中，可通过 toString() 或 toByteArray() 获取
	//@别名 解密(文本, 解密算法类型枚举)
	//@备注 该方法使用 BCrypt API 进行解密处理
	//@参数 input 输入文本
	//@参数 algorithm 解密算法类型
	//@返回 是否解密成功
	//@别名 解密(文本, 文本, 解密算法类型枚举)
	bool decrypt(const CXXString& input, cxx::CryptAlgorithm algorithm);

	//@备注 提供加密操作，输入为字节集，返回加密后的字节集
	// 成功后数据会保存于缓存中，可通过 toString() 或 toByteArray() 获取
	//@参数 input 输入字节集
	//@参数 algorithm 加密算法类型
	//@别名 加密(字节集, 加密算法类型枚举)
	bool encrypt(const CXXVector<BYTE>& input, cxx::CryptAlgorithm algorithm);

	//@备注 提供解密操作，输入为加密后的字节集，返回解密后的字节集
	// 成功后数据会保存于缓存中，可通过 toString() 或 toByteArray() 获取
	//@参数 input 输入字节集
	//@参数 algorithm 解密算法类型
	//@别名 解密(字节集, 解密算法类型枚举)
	bool decrypt(const CXXVector<BYTE>& input, cxx::CryptAlgorithm algorithm);

	//@备注 加解密后的数据转换为文本，如果是不可见字符则会转换为十六进制表示
	//@别名 到文本()
	CXXString toString() const;

	//@备注 加解密后的数据转换为字节集
	//@别名 到字节集()
	CXXVector<BYTE> toByteArray() const;

	//@备注 加解密后的数据转换为 Base64 编码的文本
	//@别名 到BASE64()
	CXXString toBase64() const;

private:
	//@备注 私有方法，使用 BCrypt API 进行加密处理，返回加密后的字节集
	bool encryptData(const BYTE* data, __int64 dataSize, CXXByteArray* output, cxx::CryptAlgorithm algorithm);

	//@备注 私有方法，使用 BCrypt API 进行解密处理，返回解密后的字节集
	bool decryptData(const BYTE* data, __int64 dataSize, CXXByteArray* output, cxx::CryptAlgorithm algorithm);

	bool encryptDataCBC(const BYTE* data, __int64 dataSize, CXXByteArray* output);

	bool decryptDataCBC(const BYTE* data, __int64 dataSize, CXXByteArray* output);

	bool handleAsymmetricEncryption(BCRYPT_ALG_HANDLE hAlgorithm, const BYTE* data, __int64 dataSize, CXXByteArray* output, cxx::CryptAlgorithm algorithm) const;

	bool handleStreamCipher(BCRYPT_ALG_HANDLE hAlgorithm, const BYTE* data, __int64 dataSize, CXXByteArray* output);

private:
	// 禁止复制和移动
	CXX_DISABLE_COPY_MOVE(CXXCrypto);

	// 指向私有类的指针
	CXXScopedPointer<CXXCryptoPrivate> d_ptr;
};
//@分组}



#endif // CXXCRYPT_H_