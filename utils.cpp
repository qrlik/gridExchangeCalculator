#include "utils.h"
#include "cmath"

double utils::myCeil(double aValue, int aPrecision)
{
	const auto tenFactor = std::pow(10, aPrecision);
	return std::ceil(aValue * tenFactor) / tenFactor;
}
