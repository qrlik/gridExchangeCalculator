#ifndef UTILS_H
#define UTILS_H
#include "cstdint"
class utils
{
public:
	static int64_t myCeil(double aValue);
	static int64_t myTrunc(double aValue);
	static double getTenFactor(int aFactor);
};

#endif // UTILS_H
