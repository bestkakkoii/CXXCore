#ifndef CXXMATH_H_
//@隐藏{
#define CXXMATH_H_
//@隐藏}

#include <cmath>
#include <algorithm>
#include <type_traits>


//@别名 炫
namespace cxx
{
	//@分组{［工具］.炫　数学类

	template <class _Ty>
	double acos(_Ty _Left) { return std::acos(_Left); }

	template <class _Ty>
	double asin(_Ty _Left) { return std::asin(_Left); }

	template <class _Ty>
	double atan(_Ty _Left) { return std::atan(_Left); }

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> atan2(_Ty1 _Left, _Ty2 _Right)
	{
		return std::atan2(_Left, _Right);
	}

	template <class _Ty>
	double cos(_Ty _Left) { return std::cos(_Left); }

	template <class _Ty>
	double sin(_Ty _Left) { return std::sin(_Left); }

	template <class _Ty>
	double tan(_Ty _Left) { return std::tan(_Left); }

	template <class _Ty>
	double acosh(_Ty _Left) { return std::acosh(_Left); }

	template <class _Ty>
	double asinh(_Ty _Left) { return std::asinh(_Left); }

	template <class _Ty>
	double atanh(_Ty _Left) { return std::atanh(_Left); }

	template <class _Ty>
	double cosh(_Ty _Left) { return std::cosh(_Left); }

	template <class _Ty>
	double sinh(_Ty _Left) { return std::sinh(_Left); }

	template <class _Ty>
	double tanh(_Ty _Left) { return std::tanh(_Left); }

	template <class _Ty>
	double exp(_Ty _Left) { return std::exp(_Left); }

	template <class _Ty>
	double exp2(_Ty _Left) { return std::exp2(_Left); }

	template <class _Ty>
	double expm1(_Ty _Left) { return std::expm1(_Left); }

	template <class _Ty>
	int ilogb(_Ty _Left) { return std::ilogb(_Left); }

	template <class _Ty>
	double ldexp(_Ty _Left, int _Arg2) { return std::ldexp(_Left, _Arg2); }

	template <class _Ty>
	double log(_Ty _Left) { return std::log(_Left); }

	template <class _Ty>
	double log10(_Ty _Left) { return std::log10(_Left); }

	template <class _Ty>
	double log1p(_Ty _Left) { return std::log1p(_Left); }

	template <class _Ty>
	double log2(_Ty _Left) { return std::log2(_Left); }

	template <class _Ty>
	double logb(_Ty _Left) { return std::logb(_Left); }

	template <class _Ty>
	double scalbn(_Ty _Left, int _Arg2) { return std::scalbn(_Left, _Arg2); }

	template <class _Ty>
	double scalbln(_Ty _Left, long _Arg2) { return std::scalbln(_Left, _Arg2); }

	template <class _Ty>
	double cbrt(_Ty _Left) { return std::cbrt(_Left); }

	template <class _Ty>
	double fabs(_Ty _Left) { return std::fabs(_Left); }

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> hypot(_Ty1 _Left, _Ty2 _Right)
	{
		return std::hypot(_Left, _Right);
	}

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> pow(_Ty1 _Left, _Ty2 _Right)
	{
		return std::pow(_Left, _Right);
	}

	template <class _Ty>
	double sqrt(_Ty _Left) { return std::sqrt(_Left); }

	template <class _Ty>
	double erf(_Ty _Left) { return std::erf(_Left); }

	template <class _Ty>
	double erfc(_Ty _Left) { return std::erfc(_Left); }

	template <class _Ty>
	double lgamma(_Ty _Left) { return std::lgamma(_Left); }

	template <class _Ty>
	double tgamma(_Ty _Left) { return std::tgamma(_Left); }

	template <class _Ty>
	double ceil(_Ty _Left) { return std::ceil(_Left); }

	template <class _Ty>
	double floor(_Ty _Left) { return std::floor(_Left); }

	template <class _Ty>
	double nearbyint(_Ty _Left) { return std::nearbyint(_Left); }

	template <class _Ty>
	double rint(_Ty _Left) { return std::rint(_Left); }

	template <class _Ty>
	long lrint(_Ty _Left) { return std::lrint(_Left); }

	template <class _Ty>
	__int64 llrint(_Ty _Left) { return std::llrint(_Left); }

	template <class _Ty>
	double round(_Ty _Left) { return std::round(_Left); }

	template <class _Ty>
	long lround(_Ty _Left) { return std::lround(_Left); }

	template <class _Ty>
	__int64 llround(_Ty _Left) { return std::llround(_Left); }

	template <class _Ty>
	double trunc(_Ty _Left) { return std::trunc(_Left); }

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> fmod(_Ty1 _Left, _Ty2 _Right)
	{
		return std::fmod(_Left, _Right);
	}

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> remainder(_Ty1 _Left, _Ty2 _Right)
	{
		return std::remainder(_Left, _Right);
	}

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> copysign(_Ty1 _Left, _Ty2 _Right)
	{
		return std::copysign(_Left, _Right);
	}

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> nextafter(_Ty1 _Left, _Ty2 _Right)
	{
		return std::nextafter(_Left, _Right);
	}

	template <class _Ty>
	double nexttoward(_Ty _Left, double _Arg2) { return std::nexttoward(_Left, _Arg2); }

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> fdim(_Ty1 _Left, _Ty2 _Right)
	{
		return std::fdim(_Left, _Right);
	}

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> fmax(_Ty1 _Left, _Ty2 _Right)
	{
		return std::fmax(_Left, _Right);
	}

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> fmin(_Ty1 _Left, _Ty2 _Right)
	{
		return std::fmin(_Left, _Right);
	}

	template <class _Ty1, class _Ty2, class _Ty3>
	::std::_Common_float_type_t<_Ty1, ::std::_Common_float_type_t<_Ty2, _Ty3>> fma(_Ty1 _Left, _Ty2 _Middle, _Ty3 _Right)
	{
		return std::fma(_Left, _Middle, _Right);
	}

	template <class _Ty1, class _Ty2>
	::std::_Common_float_type_t<_Ty1, _Ty2> remquo(_Ty1 _Left, _Ty2 _Right, int* _Pquo)
	{
		return std::remquo(_Left, _Right, _Pquo);
	}

#ifdef min
#undef min
#endif
	//@隐藏{
	template<typename T1, typename T2>
	constexpr auto min(T1 a, T2 b) { return std::min(a, b); }
	//@隐藏}
	/*@声明
	template<typename T>
	T min(T a, T b);
	*/

#ifdef max
#undef max
#endif
	//@隐藏{
	template<typename T1, typename T2>
	constexpr auto max(T1 a, T2 b) { return std::max(a, b); }
	//@隐藏}
	/*@声明
	template<typename T>
	T max(T a, T b);
	*/

#ifdef abs
#undef abs
#endif
	//@隐藏{
	template< typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_pointer_v<T>>>
	constexpr auto abs(T a) { return std::abs(a); }
	//@隐藏}
	/*@声明
	template<typename T>
	T abs(T a);
	*/

	static inline double nan(const char* _Arg1) { return std::nan(_Arg1); }

	//@隐藏{
	static inline constexpr double _E = 2.71828182845904523536;   // e
	static inline constexpr double _LOG2E = 1.44269504088896340736;   // log2(e)
	static inline constexpr double _LOG10E = 0.434294481903251827651;  // log10(e)
	static inline constexpr double _LN2 = 0.693147180559945309417;  // ln(2)
	static inline constexpr double _LN10 = 2.30258509299404568402;  // ln(10)
	static inline constexpr double _PI = 3.14159265358979323846;  // pi
	static inline constexpr double _PI_2 = 1.57079632679489661923;   // pi/2
	static inline constexpr double _PI_4 = 0.785398163397448309616;  // pi/4
	static inline constexpr double _1_PI = 0.318309886183790671538;  // 1/pi
	static inline constexpr double _2_PI = 0.636619772367581343076; // 2/pi
	static inline constexpr double _2_SQRTPI = 1.12837916709551257390;  // 2/sqrt(pi)
	static inline constexpr double  _SQRT2 = 1.41421356237309504880;   // sqrt(2)
	static inline constexpr double _SQRT1_2 = 0.707106781186547524401;  // 1/sqrt(2)
	//@隐藏}

	/*@声明
	static const double E = 2.71828182845904523536; //@ 备注 自然对数的底e: 2.71828182845904523536
	static const double LOG2E = 1.44269504088896340736; //@ 备注 以2为底e的对数: 1.44269504088896340736
	static const double LOG10E = 0.434294481903251827651; //@ 备注 以10为底e的对数: 0.434294481903251827651
	static const double LN2 = 0.693147180559945309417; //@ 备注 以e为底2的对数: 0.693147180559945309417
	static const double LN10 = 2.30258509299404568402; //@ 备注 以e为底10的对数: 2.30258509299404568402
	static const double PI = 3.14159265358979323846; //@ 备注 圆周率π: 3.14159265358979323846
	static const double PI_2 = 1.57079632679489661923; //@ 备注 π/2: 1.57079632679489661923
	static const double PI_4 = 0.785398163397448309616; //@ 备注 π/4: 0.785398163397448309616
	static const double _1_PI = 0.318309886183790671538; //@ 备注 1/π: 0.318309886183790671538
	static const double _2_PI = 0.636619772367581343076; //@ 备注 2/π: 0.636619772367581343076
	static const double _2_SQRTPI = 1.12837916709551257390; //@ 备注 2/√π: 1.12837916709551257390
	static const double SQRT2 = 1.41421356237309504880; //@ 备注 √2: 1.41421356237309504880
	static const double SQRT1_2 = 0.707106781186547524401; //@ 备注 1/√2: 0.707106781186547524401

	*/


	//@分组}
}

#endif // CXXMATH_H_