#include "pch.h"
#include "cppBinarization.h"

void cpp_binarization(int* arr, int threshold ,int start, int end)
{
	int sum = 0; 
	int tmp = start; 

	arr += start * 3; 

	while (tmp < end) 
	{
		sum += *arr;
		arr++;
		sum += *arr;
		arr++;
		sum += *arr;
		arr++;
		if (sum < threshold * 3) //white
		{
			arr -= 3;
			*arr = 0;
			arr++;
			*arr = 0;
			arr++;
			*arr = 0;
			arr++;
			sum = 0;
		}
		else //black
		{
			arr -= 3;
			*arr = 255;
			arr++;
			*arr = 255;
			arr++;
			*arr = 255;
			arr++;
			sum = 0;
		}
		tmp++;
	}
}