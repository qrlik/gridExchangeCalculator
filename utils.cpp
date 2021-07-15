#include "utils.h"
#include "cmath"

int64_t utils::myCeil(double aValue)
{
	return static_cast<int64_t>(std::ceil(aValue));
}

int64_t utils::myTrunc(double aValue)
{
	return static_cast<int64_t>(std::trunc(aValue));
}

double utils::getTenFactor(int aFactor)
{
	return static_cast<double>(std::pow(10, aFactor));
}
