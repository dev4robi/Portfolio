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
	// �ӽ� ���� ����
	int* tStar = (int*)malloc(sizeof(int)* z);
	int k = 0;

	// zStarCnt �ʱ�ȭ
	*zStarCnt = 0;

	// �ӽ÷� ������ ���� �迭 tStar�� zStar�� Ȥ�� -1�� ä�� ����
	// zStar = 0�� ũ�ų� ���� z���� ���� �� ��, �ִ������� 1�� ����
	for (int i = 1; i < z; ++i) {
		tStar[i - 1] = -1;
		if (getEuclidGCD(z, i) == 1) {
			tStar[k++] = i;
			++(*zStarCnt);
		}
	}

	// zStar�� ũ�⸸ŭ ���� �迭 ����
	int* zStar = (int*)malloc(sizeof(int) * (*zStarCnt));
	
	// tStar�� �ִ� �� �� -1�� ������ ���� zStar���� �ű�
	for (int i = 0; i < (*zStarCnt); ++i) {
		zStar[i] = tStar[i];
	}

	// tStar ����
	delete []tStar;

	// zStar ��ȯ
	return zStar;
}

int getZstarInverse(int det, int* zStar, int* zStarCnt) {
	// �ι����� ���� ���� ����
	int zStarInv = 0;
	
	// zStar���� ((det * zStar) mod 26) = 1 �� �Ǵ� zStar�� ��ȯ
	for (int i = 0; i < (*zStarCnt); ++i) {
		if (modular((det * zStar[i]), charCnt) == 1)
			return zStarInv = zStar[i];
	}

	// ���⿡ �������� ����
	return -1;
}
