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
#include <pch.h>

#include <cxxrandom.h>

#include <random>
#include <cxxglobal.h>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif


CXXRandom::CXXRandom()
{
}

CXXRandom::CXXRandom(cxx::RandomGeneratorsType deviceType)
	: deviceType_(deviceType) {}


intptr_t CXXRandom::toInt(intptr_t min, intptr_t max)
{
	if (min > max)
	{
		std::swap(min, max);
	}
	else if (min == max)
	{
		return min;
	}

	std::random_device rd;
	switch (deviceType_)
	{
	case cxx::RandomGeneratorsType::Regular:
	{
		std::default_random_engine engine(rd());
		std::uniform_int_distribution<intptr_t> dist(min, max);
		return dist(engine);
	}
	case cxx::RandomGeneratorsType::MinSTD:
	{
		std::minstd_rand engine(rd());
		std::uniform_int_distribution<intptr_t> dist(min, max);
		return dist(engine);
	}
	case cxx::RandomGeneratorsType::Mt19937:
	{
		std::mt19937 engine(rd());
		std::uniform_int_distribution<intptr_t> dist(min, max);
		return dist(engine);
	}
	default:
		throw std::runtime_error("Unsupported random generator type.");
	}
}

intptr_t CXXRandom::toInt()
{
	return toInt(std::numeric_limits<intptr_t>::min(), std::numeric_limits<intptr_t>::max());
}

intptr_t CXXRandom::toInt(intptr_t max)
{
	return toInt(0, max);
}

size_t CXXRandom::toUInt(size_t min, size_t max)
{
	if (min > max)
	{
		std::swap(min, max);
	}
	else if (min == max)
	{
		return min;
	}

	std::random_device rd;
	switch (deviceType_)
	{
	case cxx::RandomGeneratorsType::Regular:
	{
		std::default_random_engine engine(rd());
		std::uniform_int_distribution<size_t> dist(min, max);
		return dist(engine);
	}
	case cxx::RandomGeneratorsType::MinSTD:
	{
		std::minstd_rand engine(rd());
		std::uniform_int_distribution<size_t> dist(min, max);
		return dist(engine);
	}
	case cxx::RandomGeneratorsType::Mt19937:
	{
		std::mt19937 engine(rd());
		std::uniform_int_distribution<size_t> dist(min, max);
		return dist(engine);
	}
	default:
		throw std::runtime_error("Unsupported random generator type.");
	}
}

size_t CXXRandom::toUInt(size_t max)
{
	return toUInt(0, max);
}

size_t CXXRandom::toUInt()
{
	return toUInt(std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::max());
}

double CXXRandom::toDouble(double min, double max)
{
	if (min > max)
	{
		std::swap(min, max);
	}
	else if (min == max)
	{
		return min;
	}

	std::random_device rd;
	std::mt19937_64 engine(rd());
	std::uniform_real_distribution<double> dist(min, max);
	return dist(engine);
}

double CXXRandom::toDouble(double max)
{
	return toDouble(double(0), max);
}

double CXXRandom::toDouble()
{
	return toDouble(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
}

float CXXRandom::toFloat(float min, float max)
{
	if (min > max)
	{
		std::swap(min, max);
	}
	else if (min == max)
	{
		return min;
	}

	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(engine);
}

float CXXRandom::toFloat(float max)
{
	return toFloat(float(0), max);
}

float CXXRandom::toFloat()
{
	return toFloat(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
}

bool CXXRandom::toBool(double probability)
{
	std::random_device rd;
	std::bernoulli_distribution dist(probability);
	return dist(rd);
}

intptr_t CXXRandom::generate()
{
	return CXXRandom().toInt();
}

intptr_t CXXRandom::generateRange(intptr_t min, intptr_t max)
{
	return CXXRandom().toInt(min, max);
}

intptr_t CXXRandom::generateUntil(intptr_t max)
{
	return CXXRandom().toInt(max);
}
