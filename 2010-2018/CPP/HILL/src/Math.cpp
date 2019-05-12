#include "Math.h"
#include "GlobalVar.h"
#include <stdlib.h>

int power(int x, int y) {
	int pow = 1;

	for (int i = 0; i < y; ++i) {
		pow *= x;
	}

	return pow;
}

int modular(int x, int div) {
	int mod = 0;

	if ((mod = (x % div)) < 0)
		mod += 26;

	return mod;
}

int getRandomInt(int min, int max) {
	return (rand() % (abs(min - max) + 1)) + min;
}

int getEuclidGCD(int a, int b) {
	int r = (a % b);

	if (r == 0) {
		return b;
	}

	getEuclidGCD(b, r);
}

int* getZstar(int z, int* zStarCnt) {
	// 임시 공간 생성
	int* tStar = (int*)malloc(sizeof(int)* z);
	int k = 0;

	// zStarCnt 초기화
	*zStarCnt = 0;

	// 임시로 생성된 동적 배열 tStar에 zStar값 혹은 -1을 채워 넣음
	// zStar = 0과 크거나 같고 z보다 작은 수 중, 최대공약수가 1인 정수
	for (int i = 1; i < z; ++i) {
		tStar[i - 1] = -1;
		if (getEuclidGCD(z, i) == 1) {
			tStar[k++] = i;
			++(*zStarCnt);
		}
	}

	// zStar의 크기만큼 동적 배열 생성
	int* zStar = (int*)malloc(sizeof(int) * (*zStarCnt));
	
	// tStar에 있던 값 중 -1을 제외한 값을 zStar으로 옮김
	for (int i = 0; i < (*zStarCnt); ++i) {
		zStar[i] = tStar[i];
	}

	// tStar 삭제
	delete []tStar;

	// zStar 반환
	return zStar;
}

int getZstarInverse(int det, int* zStar, int* zStarCnt) {
	// 인버스를 담을 공간 생성
	int zStarInv = 0;
	
	// zStar에서 ((det * zStar) mod 26) = 1 이 되는 zStar를 반환
	for (int i = 0; i < (*zStarCnt); ++i) {
		if (modular((det * zStar[i]), charCnt) == 1)
			return zStarInv = zStar[i];
	}

	// 여기에 도달하지 않음
	return -1;
}
