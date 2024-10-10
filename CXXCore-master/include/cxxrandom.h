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

#ifndef CXXRANDOM_H_
//@隐藏{
#define CXXRANDOM_H_
//@隐藏}



#include <random>
#include <cxxglobal.h>

//@别名 炫
namespace cxx
{

}

//@分组{［工具］.炫　随机数引擎类

//@备注 相较于 C 语言的 rand()[伪随机数]，C++11 的 random 类库提供了更好的随机数生成器，更多的分布和更好的性能。
// 只允许 整型, 长整型, 浮点数, 双浮点型 或 逻辑型 作为模板参数
// 浮点数或双精度浮点数默认只使用梅森旋转算法
// 布尔值默认使用伯努利分布
// 其他类型默认使用均匀分布
//@别名 炫随机
class CXXRandom
{
public:
	CXXRandom();
	CXXRandom(cxx::RandomGeneratorsType deviceType);

	virtual ~CXXRandom() = default;

	//@备注 取指定类型指定最大到最小值之间的随机数
	//@参数 min 最小值
	//@参数 max 最大值
	//@别名 到整型(最小值, 最大值)
	intptr_t toInt(intptr_t min, intptr_t max);

	//@备注 取指定类型指定最大到最小值之间的随机数
	//@参数 无
	//@别名 到整型()
	intptr_t toInt();

	//@备注 取指定类型指定最大到 0 之间的随机数
	//@参数 max 最大值
	//@别名 到整型(最大值)
	intptr_t toInt(intptr_t max);

	//@备注 取指定最大到最小值之间的随机数
	//@参数 min 最小值
	//@参数 max 最大值
	//@别名 到正整型(最小值, 最大值)
	size_t toUInt(size_t min, size_t max);

	//@备注 取指定最大到 0 之间的随机数
	//@参数 max 最大值
	//@别名 到正整型(最大值)
	size_t toUInt(size_t max);

	//@备注 取指定最大到最小值之间的随机数
	//@参数 无
	//@别名 到正整型()
	size_t toUInt();

	//@备注 取指定最大到最小值之间的随机数双精度浮点数
	//@参数 min 最小值
	//@参数 max 最大值
	//@别名 到双浮点(最小值, 最大值)
	double toDouble(double min, double max);

	//@备注 取指定最大到 0 之间的随机数双精度浮点数
	//@参数 max 最大值
	//@别名 到双浮点(最大值)
	double toDouble(double max);

	//@备注 取浮点数最小到最大值之间的随机数
	//@参数 无
	//@别名 到双浮点()
	double toDouble();

	//@备注 取浮点数最小到最大值之间的随机数
	//@参数 min 最小值
	//@参数 max 最大值
	//@别名 到浮点(最小值, 最大值)
	float toFloat(float min, float max);

	//@备注 取浮点数最大到 0 之间的随机数
	//@参数 max 最大值
	//@别名 到浮点型(最大值)
	float toFloat(float max);

	//@备注 取浮点数最小到最大值之间的随机数
	//@参数 无
	//@别名 到浮点()
	float toFloat();

	//@备注 取概率各为 N% 得真 或 假 逻辑型
	//@参数 概率(可选) 默认为 0.5
	//@别名 到逻辑()
	bool toBool(double probability = 0.5);

	//@备注 取随机数
	//@别名 生成()
	static intptr_t generate();

	//@备注 取指定最大到最小值之间的随机数
	//@参数 min 最小值
	//@参数 max 最大值
	//@别名 生成范围(最小值, 最大值)
	static intptr_t generateRange(intptr_t min, intptr_t max);

	//@备注 取指定最大到 0 之间的随机数
	//@参数 max 最大值
	//@别名 生成至(最大值)
	static intptr_t generateUntil(intptr_t max);

private:
	//@隐藏{
	cxx::RandomDistributionType distributionType_ = cxx::RandomDistributionType::Uniform;
	cxx::RandomGeneratorsType deviceType_ = cxx::RandomGeneratorsType::Mt19937;
	//@隐藏}
};

//@分组}


#endif // CXXRANDOM_H_