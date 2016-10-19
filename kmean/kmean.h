#include <stdio.h>
#include <math.h>
#include <string>
#ifdef DLL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

bool DLLAPI kmean(int *x, int *y, int num, int lastnum);