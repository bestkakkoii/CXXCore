// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef CXXPOINT_H_
//@隐藏{
#define CXXPOINT_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxmath.h>
#include <windows.h>
#include <cmath>
#include <type_traits>

//@隐藏{
static constexpr double cxxRound(double value) noexcept
{
	// 四舍五入算法，负数时需要减去0.5，正数时加上0.5
	return (value < 0.0) ? (static_cast<double>(static_cast<intptr_t>(value - 0.5))) :
		(static_cast<double>(static_cast<intptr_t>(value + 0.5)));
}

static constexpr intptr_t cxxRoundToInt(double value) noexcept
{
	// 调用cxxRound函数并将结果转换为整数
	return static_cast<intptr_t>(cxxRound(value));
}

static constexpr intptr_t cxxAbs(intptr_t value) noexcept
{
	return (value < 0) ? -value : value;
}

static constexpr double cxxAbs(double value) noexcept
{
	return (value < 0.0) ? -value : value;
}

// constexpr min 函数，返回较小的值
template<typename T>
static constexpr T cxxMin(const T& a, const T& b) noexcept
{
	return (a < b) ? a : b;
}

// constexpr max 函数，返回较大的值
template<typename T>
static constexpr T cxxMax(const T& a, const T& b) noexcept
{
	return (a > b) ? a : b;
}

static [[nodiscard]] inline bool cxxFuzzyIsNull(double f) noexcept
{
	return cxxAbs(f) < 0.00976; // 1/102.5 to 3 significant digits; see qFuzzyCompare()
}

static [[nodiscard]] inline bool cxxFuzzyCompare(double  p1, double  p2) noexcept
{
	double  f1 = static_cast<double>(p1);
	double  f2 = static_cast<double>(p2);
	// The significand precision for IEEE754 half precision is
	// 11 bits (10 explicitly stored), or approximately 3 decimal
	// digits.  In selecting the fuzzy comparison factor of 102.5f
	// (that is, (2^10+1)/10) below, we effectively select a
	// window of about 1 (least significant) decimal digit about
	// which the two operands can vary and still return true.
	return (cxxAbs(f1 - f2) * 102.5 <= cxxMin(cxxAbs(f1), cxxAbs(f2)));
}
//@隐藏}

//@分组{ ［容器］.炫　坐标类

//@隐藏{
class CXXPointF;
//@隐藏}

//@别名 炫坐标
class CXXPoint
{
public:
	constexpr CXXPoint() noexcept;

	constexpr CXXPoint(intptr_t xpos, intptr_t ypos) noexcept;

	constexpr CXXPoint(const POINT& p) noexcept;

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 检查坐标是否为空(0)
		//@参数 无
		//@返回 逻辑型 是否为空
		//@别名 是否空()
		inline bool isNull() const noexcept;

	//@隐藏{
	constexpr
		//@隐藏}
		inline intptr_t x() const noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		inline intptr_t y() const noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 设置坐标的X值
		//@参数 x 整型 X值
		//@返回 无
		//@别名 置X()
		inline void setX(intptr_t x) noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 设置坐标的Y值
		//@参数 y 整型 Y值
		//@返回 无
		//@别名 置Y()
		inline void setY(intptr_t y) noexcept;

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 设置坐标的X和Y值
		//@参数 x 整型 X值
		//@参数 y 整型 Y值
		//@返回 无
		//@别名 置XY()
		inline void setXY(intptr_t x, intptr_t y) noexcept;

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 计算曼哈顿长度
		//@参数 无
		//@返回 整型 曼哈顿长度
		//@别名 曼哈顿长度()
		inline intptr_t manhattanLength() const;

	//@备注 计算欧几里得长度
	//@参数 无
	//@返回 整型 欧几里得长度
	//@别名 欧几里得长度()
	inline intptr_t euclideanLength() const
	{
		return cxxRoundToInt(std::sqrt(xp * xp + yp * yp));
	}

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 计算切比雪夫距离
		//@参数 无
		//@返回 整型 切比雪夫距离
		//@别名 切比雪夫长度()
		inline intptr_t chebyshevDistance() const noexcept
	{
		return cxxMax(cxxAbs(xp), cxxAbs(yp));
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 两个向量的代数运算
	//@参数 p 炫坐标 另一个向量
	//@返回 整型 点积
	//@别名 点积()
		inline intptr_t dotProduct(const CXXPoint& p) const noexcept
	{
		return xp * p.xp + yp * p.yp;
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 叉积，两个向量的向量积用来判断两个向量的相对方向（顺时针或逆时针）
	//@参数 p 炫坐标 另一个向量
	//@返回 整型 叉积
	//@别名 叉积()
		inline intptr_t crossProduct(const CXXPoint& p) const noexcept
	{
		return xp * p.yp - yp * p.xp;
	}

	//@备注 将点绕原点旋转给定角度
	//@参数 angle 双精度 旋转角度（弧度）
	//@返回 炫坐标 旋转后的坐标
	//@别名 旋转()
	CXXPoint rotate(double angle) const noexcept
	{
		double cos_a = std::cos(angle);
		double sin_a = std::sin(angle);
		return CXXPoint(
			cxxRoundToInt(xp * cos_a - yp * sin_a),
			cxxRoundToInt(xp * sin_a + yp * cos_a)
		);
	}

	//@备注 返回一个方向相同但长度为1的新点
	//@参数 无
	//@返回 炫坐标 标准化后的坐标
	//@别名 标准化()
	CXXPoint normalize() const noexcept
	{
		double length = static_cast<double>(euclideanLength());
		if (cxxFuzzyIsNull(length)) return *this;
		return CXXPoint(
			cxxRoundToInt(static_cast<double>(xp) / length),
			cxxRoundToInt(static_cast<double>(yp) / length)
		);
	}

	//@备注 在两点之间进行线性插值
	//@参数 a 炫坐标 起始点
	//@参数 b 炫坐标 终点
	//@参数 t 双精度 插值参数 (0.0 到 1.0)
	//@返回 炫坐标 插值结果
	//@别名 插值()
	static CXXPoint interpolate(const CXXPoint& a, const CXXPoint& b, double t) noexcept
	{
		return CXXPoint(
			cxxRoundToInt(a.xp + t * (b.xp - a.xp)),
			cxxRoundToInt(a.yp + t * (b.yp - a.yp))
		);
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 相对于x轴进行反射
	//@参数 无
	//@返回 炫坐标 反射后的坐标
	//@别名 X轴反射()
		CXXPoint reflectX() const noexcept
	{
		return CXXPoint(xp, -yp);
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 计算到原点距离的平方
	//@参数 无
	//@返回 整型 距离平方
	//@别名 距离平方()
		inline intptr_t distanceSquared() const noexcept
	{
		return xp * xp + yp * yp;
	}

	//@备注 计算方位角
	//@参数 无
	//@返回 双精度 方位角（弧度）
	//@别名 方位角()
	double azimuth() const noexcept
	{
		double angle = std::atan2(static_cast<double>(yp), static_cast<double>(xp));
		if (angle < 0) angle += 2 * cxx::_PI;
		return angle;
	}


	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 将X坐标转置为Y坐标，Y坐标转置为X坐标
		//@参数 无
		//@返回 炫坐标 转置后的坐标
		//@别名 交换()
		CXXPoint transposed() const noexcept { return { yp, xp }; }

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 获取X坐标的引用
		//@参数 无
		//@返回 整型 引用
		//@别名 引用X()
		inline intptr_t& rx() noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 获取Y坐标的引用
		//@参数 无
		//@返回 整型 引用
		//@别名 引用Y()
		inline intptr_t& ry() noexcept;


	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 两个向量的代数运算
		//@参数 p1 炫坐标 第一个向量
		//@参数 p2 炫坐标 第二个向量
		//@返回 整型 点积
		//@别名 点积()
		static inline intptr_t dotProduct(const CXXPoint& p1, const CXXPoint& p2)
	{
		return p1.xp * p2.xp + p1.yp * p2.yp;
	}

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 叉积，两个向量的向量积用来判断两个向量的相对方向（顺时针或逆时针）
		//@参数 p1 炫坐标 第一个向量
		//@参数 p2 炫坐标 第二个向量
		//@返回 整型 叉积
		//@别名 叉积()
		static inline intptr_t crossProduct(const CXXPoint& p1, const CXXPoint& p2)
	{
		return p1.xp * p2.yp - p1.yp * p2.xp;
	}

	//@隐藏{
	constexpr inline CXXPoint& operator+=(const CXXPoint& p);
	constexpr inline CXXPoint& operator-=(const CXXPoint& p);

	constexpr inline CXXPoint& operator*=(float factor);
	constexpr inline CXXPoint& operator*=(double factor);
	constexpr inline CXXPoint& operator*=(intptr_t factor);

	constexpr inline CXXPoint& operator/=(double divisor);



	friend constexpr inline bool operator==(const CXXPoint& p1, const CXXPoint& p2) noexcept
	{
		return p1.xp == p2.xp && p1.yp == p2.yp;
	}
	friend constexpr inline bool operator!=(const CXXPoint& p1, const CXXPoint& p2) noexcept
	{
		return p1.xp != p2.xp || p1.yp != p2.yp;
	}
	friend constexpr inline CXXPoint operator+(const CXXPoint& p1, const CXXPoint& p2) noexcept
	{
		return CXXPoint(p1.xp + p2.xp, p1.yp + p2.yp);
	}
	friend constexpr inline CXXPoint operator-(const CXXPoint& p1, const CXXPoint& p2) noexcept
	{
		return CXXPoint(p1.xp - p2.xp, p1.yp - p2.yp);
	}
	friend constexpr inline CXXPoint operator*(const CXXPoint& p, float factor)
	{
		return CXXPoint(cxxRoundToInt(p.xp * factor), cxxRoundToInt(p.yp * factor));
	}
	friend constexpr inline CXXPoint operator*(const CXXPoint& p, double factor)
	{
		return CXXPoint(cxxRoundToInt(p.xp * factor), cxxRoundToInt(p.yp * factor));
	}
	friend constexpr inline CXXPoint operator*(const CXXPoint& p, intptr_t factor) noexcept
	{
		return CXXPoint(p.xp * factor, p.yp * factor);
	}
	friend constexpr inline CXXPoint operator*(float factor, const CXXPoint& p)
	{
		return CXXPoint(cxxRoundToInt(p.xp * factor), cxxRoundToInt(p.yp * factor));
	}
	friend constexpr inline CXXPoint operator*(double factor, const CXXPoint& p)
	{
		return CXXPoint(cxxRoundToInt(p.xp * factor), cxxRoundToInt(p.yp * factor));
	}
	friend constexpr inline CXXPoint operator*(intptr_t factor, const CXXPoint& p) noexcept
	{
		return CXXPoint(p.xp * factor, p.yp * factor);
	}
	friend constexpr inline CXXPoint operator+(const CXXPoint& p) noexcept
	{
		return p;
	}
	friend constexpr inline CXXPoint operator-(const CXXPoint& p) noexcept
	{
		return CXXPoint(-p.xp, -p.yp);
	}
	friend constexpr inline CXXPoint operator/(const CXXPoint& p, double c)
	{
		return CXXPoint(cxxRoundToInt(p.xp / c), cxxRoundToInt(p.yp / c));
	}
	//@隐藏}

	 //@隐藏{
	[[nodiscard]] constexpr
		//@隐藏}
		//@备注 转换为炫双精度坐标
		//@参数 无
		//@返回 炫双精度坐标 双精度坐标
		//@别名 到双精度()
		inline CXXPointF toPointF() const noexcept;

	//@隐藏{
	[[nodiscard]] constexpr
		//@隐藏}
		//@备注 转换为系统坐标
		//@参数 无
		//@返回 系统坐标 系统坐标
		//@别名 到POINT()
		inline POINT toPoint() const noexcept
	{
		return { static_cast<LONG>(xp), static_cast<LONG>(yp) };
	}

private:
	//@隐藏{
	intptr_t xp = 0;
	intptr_t yp = 0;
	//@隐藏}
};

//@隐藏{

/*****************************************************************************
  CXXPoint stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM
std::wostream& operator<<(std::wostream&, const CXXPoint&);
std::wistream& operator>>(std::wistream&, CXXPoint&);
#endif

/*****************************************************************************
  CXXPoint inline functions
 *****************************************************************************/

constexpr inline CXXPoint::CXXPoint() noexcept : xp(0), yp(0) {}

constexpr inline CXXPoint::CXXPoint(intptr_t xpos, intptr_t ypos) noexcept : xp(xpos), yp(ypos) {}

constexpr inline CXXPoint::CXXPoint(const POINT& p) noexcept : xp(p.x), yp(p.y) {}

constexpr inline bool CXXPoint::isNull() const noexcept
{
	return xp == 0 && yp == 0;
}

constexpr inline intptr_t CXXPoint::x() const noexcept
{
	return xp;
}

constexpr inline intptr_t CXXPoint::y() const noexcept
{
	return yp;
}

constexpr inline void CXXPoint::setX(intptr_t xpos) noexcept
{
	xp = xpos;
}

constexpr inline void CXXPoint::setY(intptr_t ypos) noexcept
{
	yp = ypos;
}

constexpr inline void CXXPoint::setXY(intptr_t xpos, intptr_t ypos) noexcept
{
	xp = xpos;
	yp = ypos;
}

inline intptr_t constexpr CXXPoint::manhattanLength() const
{
	return cxxAbs(x()) + cxxAbs(y());
}

constexpr inline intptr_t& CXXPoint::rx() noexcept
{
	return xp;
}

constexpr inline intptr_t& CXXPoint::ry() noexcept
{
	return yp;
}

constexpr inline CXXPoint& CXXPoint::operator+=(const CXXPoint& p)
{
	xp += p.xp;
	yp += p.yp;
	return *this;
}

constexpr inline CXXPoint& CXXPoint::operator-=(const CXXPoint& p)
{
	xp -= p.xp;
	yp -= p.yp;
	return *this;
}

constexpr inline CXXPoint& CXXPoint::operator*=(float factor)
{
	xp = cxxRoundToInt(static_cast<double>(xp) * static_cast<double>(factor));
	yp = cxxRoundToInt(static_cast<double>(yp) * static_cast<double>(factor));
	return *this;
}

constexpr inline CXXPoint& CXXPoint::operator*=(double factor)
{
	xp = cxxRoundToInt(static_cast<double>(xp) * static_cast<double>(factor));
	yp = cxxRoundToInt(static_cast<double>(yp) * static_cast<double>(factor));
	return *this;
}

constexpr inline CXXPoint& CXXPoint::operator*=(intptr_t factor)
{
	xp = xp * factor;
	yp = yp * factor;
	return *this;
}

constexpr inline CXXPoint& CXXPoint::operator/=(double c)
{
	xp = cxxRoundToInt(static_cast<double>(xp) / static_cast<double>(c));
	yp = cxxRoundToInt(static_cast<double>(yp) / static_cast<double>(c));
	return *this;
}

namespace std
{
	template <>
	struct hash<CXXPoint>
	{
		intptr_t operator()(const CXXPoint& p) const
		{
			return hash<intptr_t>()(p.x()) ^ hash<intptr_t>()(p.y());
		}
	};
}

//@隐藏}

//@别名 炫双浮点坐标
class CXXPointF
{
public:
	constexpr CXXPointF() noexcept;
	constexpr CXXPointF(const CXXPoint& p) noexcept;
	constexpr CXXPointF(double xpos, double ypos) noexcept;

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 计算曼哈顿长度
		//@参数 无
		//@返回 双精度 曼哈顿长度
		//@别名 曼哈顿长度()
		inline double manhattanLength() const;

	//@备注 计算欧几里得长度
	//@参数 无
	//@返回 双精度 欧几里得长度
	//@别名 欧几里得长度()
	inline double euclideanLength() const
	{
		return std::sqrt(xp * xp + yp * yp);
	}

	//@备注 检查坐标是否为空(0)
	//@参数 无
	//@返回 逻辑型 是否为空
	//@别名 是否空()
	inline bool isNull() const noexcept;

	//@隐藏{
	constexpr
		//@隐藏}
		inline double x() const noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		inline double y() const noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 设置坐标的X值
		//@参数 x 双精度 X值
		//@返回 无
		//@别名 置X()
		inline void setX(double x) noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 设置坐标的Y值
		//@参数 y 双精度 Y值
		//@返回 无
		//@别名 置Y()
		inline void setY(double y) noexcept;

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 将X坐标转置为Y坐标，Y坐标转置为X坐标
		//@参数 无
		//@返回 炫双精度坐标 转置后的坐标
		//@别名 交换()
		CXXPointF transposed() const noexcept { return { yp, xp }; }

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 计算切比雪夫距离
		//@参数 无
		//@返回 整型 切比雪夫距离
		//@别名 切比雪夫长度()
		inline double chebyshevDistance() const noexcept
	{
		return cxxMax(cxxAbs(xp), cxxAbs(yp));
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 两个向量的代数运算
	//@参数 p 炫坐标 另一个向量
	//@返回 整型 点积
	//@别名 点积()
		inline double dotProduct(const CXXPointF& p) const noexcept
	{
		return xp * p.xp + yp * p.yp;
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 叉积，两个向量的向量积用来判断两个向量的相对方向（顺时针或逆时针）
	//@参数 p 炫坐标 另一个向量
	//@返回 整型 叉积
	//@别名 叉积()
		inline double crossProduct(const CXXPointF& p) const noexcept
	{
		return xp * p.yp - yp * p.xp;
	}

	//@备注 将点绕原点旋转给定角度
	//@参数 angle 双精度 旋转角度（弧度）
	//@返回 炫坐标 旋转后的坐标
	//@别名 旋转()
	CXXPointF rotate(double angle) const noexcept
	{
		double cos_a = std::cos(angle);
		double sin_a = std::sin(angle);
		return CXXPointF(
			static_cast<double>(cxxRoundToInt(static_cast<double>(xp) * cos_a - yp * sin_a)),
			static_cast<double>(cxxRoundToInt(static_cast<double>(xp) * sin_a + yp * cos_a))
		);
	}

	//@备注 返回一个方向相同但长度为1的新点
	//@参数 无
	//@返回 炫坐标 标准化后的坐标
	//@别名 标准化()
	CXXPointF normalize() const noexcept
	{
		double length = euclideanLength();
		if (cxxFuzzyIsNull(length)) return *this;
		return CXXPointF(
			static_cast<double>(cxxRoundToInt(static_cast<double>(xp) / length)),
			static_cast<double>(cxxRoundToInt(static_cast<double>(yp) / length))
		);
	}

	//@备注 在两点之间进行线性插值
	//@参数 a 炫坐标 起始点
	//@参数 b 炫坐标 终点
	//@参数 t 双精度 插值参数 (0.0 到 1.0)
	//@返回 炫坐标 插值结果
	//@别名 插值()
	static CXXPointF interpolate(const CXXPointF& a, const CXXPointF& b, double t) noexcept
	{
		return CXXPointF(
			static_cast<double>(cxxRoundToInt(a.xp + t * (b.xp - a.xp))),
			static_cast<double>(cxxRoundToInt(a.yp + t * (b.yp - a.yp)))
		);
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 相对于x轴进行反射
	//@参数 无
	//@返回 炫坐标 反射后的坐标
	//@别名 X轴反射()
		CXXPointF reflectX() const noexcept
	{
		return CXXPointF(xp, -yp);
	}

	//@隐藏{
	constexpr
		//@隐藏}
	//@备注 计算到原点距离的平方
	//@参数 无
	//@返回 整型 距离平方
	//@别名 距离平方()
		inline double distanceSquared() const noexcept
	{
		return xp * xp + yp * yp;
	}

	//@备注 计算方位角
	//@参数 无
	//@返回 双精度 方位角（弧度）
	//@别名 方位角()
	double azimuth() const noexcept
	{
		double angle = std::atan2(static_cast<double>(yp), static_cast<double>(xp));
		if (angle < 0) angle += 2 * cxx::_PI;
		return angle;
	}

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 获取X坐标的引用
		//@参数 无
		//@返回 双精度 引用
		//@别名 引用X()
		inline double& rx() noexcept;
	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 获取Y坐标的引用
		//@参数 无
		//@返回 双精度 引用
		//@别名 引用Y()
		inline double& ry() noexcept;

	//@隐藏{
	constexpr inline CXXPointF& operator+=(const CXXPointF& p);
	constexpr inline CXXPointF& operator-=(const CXXPointF& p);
	constexpr inline CXXPointF& operator*=(double c);
	constexpr inline CXXPointF& operator/=(double c);

	constexpr static inline double dotProduct(const CXXPointF& p1, const CXXPointF& p2)
	{
		return p1.xp * p2.xp + p1.yp * p2.yp;
	}

	friend constexpr inline bool operator==(const CXXPointF& p1, const CXXPointF& p2)
	{
		return ((!p1.xp || !p2.xp) ? cxxFuzzyIsNull(p1.xp - p2.xp) : cxxFuzzyCompare(p1.xp, p2.xp))
			&& ((!p1.yp || !p2.yp) ? cxxFuzzyIsNull(p1.yp - p2.yp) : cxxFuzzyCompare(p1.yp, p2.yp));
	}
	friend constexpr inline bool operator!=(const CXXPointF& p1, const CXXPointF& p2)
	{
		return !(p1 == p2);
	}

	friend constexpr inline CXXPointF operator+(const CXXPointF& p1, const CXXPointF& p2)
	{
		return CXXPointF(p1.xp + p2.xp, p1.yp + p2.yp);
	}
	friend constexpr inline CXXPointF operator-(const CXXPointF& p1, const CXXPointF& p2)
	{
		return CXXPointF(p1.xp - p2.xp, p1.yp - p2.yp);
	}
	friend constexpr inline CXXPointF operator*(const CXXPointF& p, double c)
	{
		return CXXPointF(p.xp * c, p.yp * c);
	}
	friend constexpr inline CXXPointF operator*(double c, const CXXPointF& p)
	{
		return CXXPointF(p.xp * c, p.yp * c);
	}
	friend constexpr inline CXXPointF operator+(const CXXPointF& p)
	{
		return p;
	}
	friend constexpr inline CXXPointF operator-(const CXXPointF& p)
	{
		return CXXPointF(-p.xp, -p.yp);
	}
	friend constexpr inline CXXPointF operator/(const CXXPointF& p, double divisor)
	{
		CXX_ASSERT(divisor < 0 || divisor > 0);
		return CXXPointF(p.xp / divisor, p.yp / divisor);
	}
	//@隐藏}

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 转换为炫整型坐标
		//@参数 无
		//@返回 炫整型坐标 整型坐标
		//@别名 到整型()
		CXXPoint toPoint() const;

	//@隐藏{
	constexpr
		//@隐藏}
		//@备注 转换为系统坐标
		//@参数 无
		//@返回 系统坐标 系统坐标
		//@别名 到POINT()
		inline POINT toPOINT() const noexcept
	{
		return { static_cast<LONG>(xp), static_cast<LONG>(yp) };
	}

private:
	//@隐藏{
	double xp = 0.0;
	double yp = 0.0;
	//@隐藏}
};

//@隐藏{
/*****************************************************************************
  CXXPointF stream functions
 *****************************************************************************/
std::wostream& operator<<(std::wostream&, const CXXPointF&);
std::wistream& operator>>(std::wistream&, CXXPointF&);

/*****************************************************************************
  CXXPointF inline functions
 *****************************************************************************/

constexpr inline CXXPointF::CXXPointF() noexcept
	: xp(0)
	, yp(0)
{
}

constexpr inline CXXPointF::CXXPointF(double xpos, double ypos) noexcept : xp(xpos), yp(ypos) { }

constexpr inline CXXPointF::CXXPointF(const CXXPoint& p) noexcept : xp(static_cast<double>(p.x())), yp(static_cast<double>(p.y())) { }

constexpr inline double CXXPointF::manhattanLength() const
{
	return cxxAbs(x()) + cxxAbs(y());
}

inline bool CXXPointF::isNull() const noexcept
{
	return cxxFuzzyIsNull(xp) && cxxFuzzyIsNull(yp);
}

constexpr inline double CXXPointF::x() const noexcept
{
	return xp;
}

constexpr inline double CXXPointF::y() const noexcept
{
	return yp;
}

constexpr inline void CXXPointF::setX(double xpos) noexcept
{
	xp = xpos;
}

constexpr inline void CXXPointF::setY(double ypos) noexcept
{
	yp = ypos;
}

constexpr inline double& CXXPointF::rx() noexcept
{
	return xp;
}

constexpr inline double& CXXPointF::ry() noexcept
{
	return yp;
}

constexpr inline CXXPointF& CXXPointF::operator+=(const CXXPointF& p)
{
	xp += p.xp;
	yp += p.yp;
	return *this;
}

constexpr inline CXXPointF& CXXPointF::operator-=(const CXXPointF& p)
{
	xp -= p.xp;
	yp -= p.yp;
	return *this;
}

constexpr inline CXXPointF& CXXPointF::operator*=(double c)
{
	xp *= c;
	yp *= c;
	return *this;
}

constexpr inline CXXPointF& CXXPointF::operator/=(double divisor)
{
	CXX_ASSERT(divisor > 0 || divisor < 0);
	xp /= divisor;
	yp /= divisor;
	return *this;
}

constexpr CXXPointF CXXPoint::toPointF() const noexcept { return *this; }

constexpr inline CXXPoint CXXPointF::toPoint() const
{
	return CXXPoint(cxxRoundToInt(xp), cxxRoundToInt(yp));
}

namespace std
{
	template <>
	struct hash<CXXPointF>
	{
		intptr_t operator()(const CXXPointF& p) const
		{
			return hash<double>()(p.x()) ^ hash<double>()(p.y());
		}
	};
}

//@隐藏}

//@分组}

/*****************************************************************************
  CXXPoint/CXXPointF tuple protocol
 *****************************************************************************/

#endif // CXXPOINT_H_
