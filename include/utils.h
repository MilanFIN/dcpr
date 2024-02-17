#ifndef UTILS_H
#define UTILS_H

#include <tonc.h>

/// @brief floating point absolute value
/// @param a source value
/// @return absolute value of a
inline float floatAbs(float a)
{
	if (a < 0.0)
	{
		return -a;
	}
	else
	{
		return a;
	}
}

/// @brief libtonc builtin fixed point absolute value
/// @param a source value
/// @return absolute value
inline static FIXED fixedAbs(FIXED a)
{
	if (a < 0)
	{
		return fxsub(int2fx(0), a);
	}
	else
	{
		return a;
	}
}

/// @brief integer absolute value
/// @param a
/// @return
inline int intAbs(int a)
{
	if (a < 0)
	{
		return -a;
	}
	else
	{
		return a;
	}
}

void copyText(char target[], char source[], size_t length);
#endif // UTILS_H