#ifndef UTILS_H
#define UTILS_H

#include <tonc.h>

/// @brief floating point absolute value
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

/// @brief Copy char array contents to another
void copyText(char target[], char source[], size_t length);


#endif // UTILS_H