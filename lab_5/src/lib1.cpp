#include <stdio.h>
#include <iostream>
#include "../src/types.h"

using namespace std;

int gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

extern "C" os_int GCD(os_int A, os_int B)
{
	cout << "\nEuclidean algorithm: ";
	return gcd(A, B);
}


extern "C" os_int *Sort(os_int *array, os_int size)
{
  int i, j;
  for (i = 0; i < size - 1; i++)
    {
      for (j = 0; j < size - 1 - i; j++)
	{
	  if (array[j] > array[j + 1])
	    {
	      os_int temp = array[j + 1];
	      array[j + 1] = array[j];
	      array[j] = temp;
	    }
	}
    }
  cout << "\nSorting an array with a bubble: ";
  return array;
}