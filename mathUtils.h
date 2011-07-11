#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

#define PI 3.14159265
#define TO_RAD(degrees) (((degrees)*PI)/180.0)
#define TO_DEG(radians) (((radians)*180.0)/PI)
#define MAX(X, Y) ((X)>(Y)?(X):(Y))
#define MIN(X, Y) ((X)<(Y)?(X):(Y))

/*
void swap(float* a, float* b) {
	float tmp = *a;
	*a = *b;
	*b = tmp;
}
*/

#endif
