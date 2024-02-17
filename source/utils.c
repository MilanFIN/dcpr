#include <tonc.h>


void copyText(char target[], char source[], size_t length)
{

	for (size_t i = 0; i < length; i++)
	{
		target[i] = source[i];
	}
}