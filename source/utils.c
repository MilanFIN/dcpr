#include <tonc.h>
#include "utils.h"

void copyText(char target[], char source[], size_t length)
{

	for (size_t i = 0; i < length; i++)
	{
		target[i] = source[i];
	}
}

/// @brief helper for quicksort, swap places of two items
/// @param a
/// @param b
void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/// @brief randomize order of an array in place
/// @param array array pointer
/// @param size size of array
void shuffleArray(int *array, int size)
{
	for (int i = 0; i < size; i++)
	{
		int j = qran_range(0, size);
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

int simpleDistance(int x1, int y1, int x2, int y2)
{
	return intAbs(x2 - x1) + intAbs(y2 - y1);
}
