#ifndef __MATH_H__
#define __MATH_H__

int power(int x, int y);
int modular(int x, int div);
int getRandomInt(int min, int max);
int getEuclidGCD(int a, int b);
int* getZstar(int z, int* zStarCnt);
int getZstarInverse(int det, int* zStar, int* zStarCnt);

#endif