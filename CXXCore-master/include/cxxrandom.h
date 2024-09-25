/*
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

#ifndef CXXRANDOM_H_
//@隐藏{
#define CXXRANDOM_H_
//@隐藏}



#include <random>
#include <cxxglobal.h>

//@别名 炫
namespace cxx
{
	//@分组{［工具］.炫　随机数引擎类

	//@别名 随机数生成引擎类型
	enum RandomGeneratorsType
	{
		//@备注 普通随机数生成引擎 std::default_random_engine
		//优点:
		// 通用性好，适用于大多数需要随机数的场景。
		// 实现依赖于编译器，可能会自动选择最适合当前平台的实现。
		//缺点 :
		// 因为是默认引擎，其具体实现可能在不同的编译器和平台间有所不同，导致难以在不同环境下保持结果的一致性。
		// 可能不是对于所有用途都有最佳的性能或随机性。
		Regular = 0, //@别名 普通

		//@备注 MinSTD 最小标准(Minimum standard)随机数生成引擎 std::minstd_rand 或 std::minstd_rand0
		//优点:
		// 算法简单，速度快，占用资源少。对于基本的随机数需求足够好
		//缺点 :
		// 随机性较差，周期较短，不适合用于需要高质量随机数的场景。
		MinSTD = 1,//@别名 最小标准

		//@备注 基于 Mersenne Twister(梅森旋转算法) 的随机数生成引擎 std::mt19937 或 std::mt19937_64
		// 19937 是指它的周期长度为 2^19937 - 1
		//优点:
		// 高质量随机数，周期长，适用于大多数需要高质量随机数的场景。
		//缺点 :
		// 占用资源多，速度慢，不适合用于对性能要求较高的场景。
		Mt19937 = 2,//@别名 梅森旋转
	};

	//@别名 随机数分布类型
	enum RandomDistributionType
	{
		//@备注 均匀分布 std::uniform_int_distribution 或 std::uniform_real_distribution
		Uniform = 0,//@别名 均匀

		//@备注 std::bernoulli_distribution 伯努利分布
		Bernoulli = 1,//@别名 伯努利

		//@备注 std::binomial_distribution 二项分布
		Binomial = 2,//@别名 二项

		//@备注 std::negative_binomial_distribution 负二项分布
		NegativeBinomial = 3, //@别名 负二项

		//@备注 std::geometric_distribution 几何分布
		Geometric = 4, //@别名 几何

		//@备注 std::poisson_distribution 泊松分布
		Poisson = 5, //@别名 泊松

		//@备注 std::exponential_distribution 指数分布
		Exponential = 6, //@别名 指数

		//@备注 std::gamma_distribution 伽马分布
		Gamma = 7, //@别名 伽马

		//@备注 std::weibull_distribution 威布尔分布
		Weibull = 8, //@别名 威布尔

		//@备注 std::extreme_value_distribution 极值分布
		ExtremeValue = 9, //@别名 极值

		//@备注 std::normal_distribution 正态分布
		Normal = 10, //@别名 正态

		//@备注 std::lognormal_distribution 对数正态分布
		Lognormal = 11, //@别名 对数正态

		//@备注 std::chi_squared_distribution 卡方分布
		ChiSquared = 12, //@别名 卡方

		//@备注 std::cauchy_distribution 柯西分布
		Cauchy = 13, //@别名 柯西

		//@备注 std::fisher_f_distribution F分布
		FisherF = 14, //@别名 F

		//@备注 std::student_t_distribution t分布
		StudentT = 15, //@别名 t

		//@备注 std::discrete_distribution 离散分布
		Discrete = 16, //@别名 离散

		//@备注 std::piecewise_constant_distribution 分段常数分布
		PiecewiseConstant = 17, //@别名 分段常数

		//@备注 std::piecewise_linear_distribution 分段线性分布
		PiecewiseLinear = 18 //@别名 分段线性
	};

	//@分组}
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
	//@隐藏{
	CXXRandom();
	//@隐藏}
	CXXRandom(cxx::RandomGeneratorsType deviceType);

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
	//@别名 到双浮点型(最小值, 最大值)
	double toDouble(double min, double max);

	//@备注 取指定最大到 0 之间的随机数双精度浮点数
	//@参数 max 最大值
	//@别名 到双浮点型(最大值)
	double toDouble(double max);

	//@备注 取浮点数最小到最大值之间的随机数
	//@参数 无
	//@别名 到双浮点型()
	double toDouble();

	//@备注 取浮点数最小到最大值之间的随机数
	//@参数 min 最小值
	//@参数 max 最大值
	//@别名 到浮点型(最小值, 最大值)
	float toFloat(float min, float max);

	//@备注 取浮点数最大到 0 之间的随机数
	//@参数 max 最大值
	//@别名 到浮点型(最大值)
	float toFloat(float max);

	//@备注 取浮点数最小到最大值之间的随机数
	//@参数 无
	//@别名 到浮点型()
	float toFloat();

	//@备注 取概率各为 N% 得真 或 假 逻辑型
	//@参数 概率(可选) 默认为 0.5
	//@别名 到逻辑型()
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