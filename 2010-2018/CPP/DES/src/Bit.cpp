#include "Bit.h"
#include "Math.h"
#include <iostream>
#include <string>
using namespace std;

Bit::Bit() {
	// 생성자 기본 초기화
	size = 0;
	pBit = NULL;
}

Bit::Bit(const int _size) {
	// 올바른 크기인지 확인
	if (_size <= 0) {
		cout << "[오류](Bit::Bit(const int _size)): 올바른 사이즈를 입력하십시오." << endl;
		exit(0);
	}

	// 전달받은 크기만큼 bool공간을 동적 생성
	if ((pBit = (bool*)malloc(sizeof(bool) * _size)) == NULL) {
		cout << "[오류](Bit::Bit(const int _size)): 메모리 할당 실패." << endl;
		exit(0);
	}

	// 사이즈를 저장
	size = _size;

	// 비트 스트림을 0으로 초기화
	for (int i = 0; i < size; ++i) {
		pBit[i] = 0;
	}
}

Bit::Bit(const char* string) {
	// 길이를 대입
	size = 64; /*(strlen(string) * 8);*/

	// 올바른 크기인지 확인
	if (size <= 0) {
		cout << "[오류](Bit::Bit(const int _size)): 올바른 사이즈를 입력하십시오." << endl;
		exit(0);
	}

	// 전달받은 크기만큼 bool공간을 동적 생성
	if ((pBit = (bool*)malloc(sizeof(bool) * size)) == NULL) {
		cout << "[오류](Bit::Bit(const int _size)): 메모리 할당 실패." << endl;
		exit(0);
	}

	// 문자열을 비트로 변환하여 대입
	for (int i = 0; i < (size / 8); ++i) {
		char c = string[i];

		// 한 문자(8bit)를 우측으로 j번 쉬프트하고 0x01과 마스킹 => j번째 비트를 추출 후 저장
		for (int j = 0; j < 8; ++j) {
			pBit[i * 8 + j] = ((c >> (7 - j)) & 0x01);
		}
	}
}

Bit::Bit(const Bit& _bit) {
	// 좌측항(L-Value)의 비트값 삭제
	if (size > 0 && pBit != NULL) {
		free(pBit);
		pBit = NULL;
	}

	// 좌측항에 새로운 사이즈 값 대입
	size = _bit.size;

	// 좌측항에 동적 공간 생성
	if ((pBit = (bool*)malloc(sizeof(bool) * size)) == NULL) {
		cout << "[오류](Bit::Bit(const Bit& _bit)): 메모리 할당 실패." << endl;
		exit(0);
	}

	// 좌측항에 우측항(R-Value)의 값 저장
	for (int i = 0; i < size; ++i) {
		pBit[i] = _bit.pBit[i];
	}
}

Bit::~Bit() {
	// 사이즈가 0 보다 크고 비트 포인터가 NULL이 아니면 동적 공간 해제
	if (size > 0 && pBit != NULL) {
		free(pBit);
		pBit = NULL;
	}
}

int Bit::getSize() const {
	return size;
}

void Bit::print(const Option option) const {
	if (option == OPTION_BINARY) {
		for (int i = 0; i < size; ++i) {
			printf("%d", pBit[i]);
		}
	}
	else if (option == OPTION_HEXA) {
		for (int i = 0; i < size / 4; ++i) {
			int j = (4 * i), tempHex;
			tempHex = 8 * pBit[j] + 4 * pBit[j + 1] + 2 * pBit[j + 2] + pBit[j + 3];
			printf("%X", tempHex);
		}
	}
	else if (option == OPTION_CHAR) {
		for (int i = 0; i < size / 8; ++i) {
			int j = (8 * i), tempChar;
			tempChar = 128 * pBit[j] + 64 * pBit[j + 1] + 32 * pBit[j + 2] + 16 * pBit[j + 3] + 
					   8 * pBit[j + 4] + 4 * pBit[j + 5] + 2 * pBit[j + 6] + pBit[j + 7];
			if (tempChar == 7 || tempChar == 8 || tempChar == 10 || tempChar == 13 || tempChar == 27 || tempChar == 127) // 대체 출력이 필요한 문자
				printf("?");
			else
				printf("%c", tempChar);
		}
	}
}

void Bit::swap() {
	// 비트스트림 객체가 짝수 길이인지를 확인
	if (size % 2 != 0) {
		cout << "([오류](void Bit::swap()): 사이즈가 짝수여야 스왑이 가능합니다. (" << size << ")" << endl;
		exit(0);
	}

	// 상위 비트와 하위 비트 스와핑
	int bitCenter = (size / 2);
	for (int i = 0; i < bitCenter; ++i) {
		bool tempBit = pBit[i];
		pBit[i] = pBit[bitCenter + i];
		pBit[bitCenter + i] = tempBit;
	}
}

Bit Bit::permutate(const table* table) {
	int bitSize = (table->tableRow * table->tableCol);
	Bit rtBit(bitSize);

	for (int i = 0; i < bitSize; ++i) {
		rtBit.pBit[i] = pBit[(table->ary[i]) - 1];
	}

	return rtBit;
}

Bit Bit::substitute() {
	// 대치는 크기가 48bit인 비트스트림만 수행 가능
	if (size != 48) {
		cout << "([오류]Bit Bit::substitute(const table* table): 사이즈가 48이 아닙니다. (" << size << ")" << endl;
		exit(0);
	}

	// 반환을 위한 객체 생성
	Bit rtBit(32);

	// 각 블록(총 8블록)을 S-박스를 이용하여 대치
	for (int i = 0; i < 8; ++i) {
		// S-박스의 행은 Ri -> EP -> ^Key / 8 한 값을 통과시킨 후, 블럭 맨 앞 맨 뒤의 비트의 이진수로 결정된다
		// S-박스의 열은 Ri -> EP -> ^Key / 8 한 값을 통과시킨 후, 블럭 맨 앞 맨 뒤의 비트를 제외한 비트의 이진수로 결정된다
		int row = 0;	int col = 0;
		row = ((pBit[i * 6] ? 1 : 0) * 2) + ((pBit[i * 6 + 5] ? 1 : 0));
		col = ((pBit[i * 6 + 1] ? 1 : 0) * 8) + ((pBit[i * 6 + 2] ? 1 : 0) * 4) + ((pBit[i * 6 + 3] ? 1 : 0) * 2) + ((pBit[i * 6 + 4] ? 1 : 0));
		// 테이블 선택
		table* table = &(g_SboxAry[i]);

		// 해당 테이블의 수를 가져옴
		int tbBinary = table->ary[row * table->tableCol + col];

		// 가져온 수를 이진수하여 반환할 비트스트림에 저장
		for (int k = 3; k >= 0; --k) {
			rtBit[i * 4 + k] = (tbBinary % 2);
			tbBinary /= 2;
		}
	}

	return rtBit;
}

Bit Bit::subBit(const int _low, const int _high) {
	// 두 범위중 하나라도 음수이거나 0이면 오류
	if (_low <= 0 || _high <= 0) {
		cout << "[오류](Bit::Bit(const Bit& _bit, const int _low, const int _high)): 올바르지 않은 범위 지정. (" <<
			    _low << ", " << _high << ")" << endl;
		exit(0);
	}

	// 초과한 범위이면 오류
	if (_low > size || _high > size) {
		cout << "[오류](Bit::Bit(const Bit& _bit, const int _low, const int _high)): 초과된 범위 오류. (" <<
			    _low << ", " << _high << ")" << endl;
		exit(0);
	}

	// 반환할 서브비트스트림 생성
	int subSize = abs(_high - _low + 1);
	Bit rtBit(subSize);

	// _low와 _high 사이의 값 대입
	int j = 0;
	for (int i = min(_high, _low); i <= max(_high, _low); ++i) {
		rtBit.pBit[j++] = pBit[i-1];
	}

	// 생성된 서브비트스트림 반환
	return rtBit;
}

Bit Bit::append(const Bit& _bit) {
	Bit rtBit(size + _bit.size);

	// 앞쪽 비트
	for (int i = 0; i < size; ++i) {
		rtBit.pBit[i] = pBit[i];
	}

	// 뒤쪽 비트
	for (int i = 0; i < _bit.size; ++i) {
		rtBit.pBit[i + size] = _bit.pBit[i];
	}

	// 생성된 비트 반환
	return rtBit;
}

Bit Bit::operator^(const Bit& _bit) {
	// 두 비트객체의 사이즈가 같은지 확인
	if (size != _bit.size) {
		cout << "([오류]Bit& Bit::operator^(const Bit& _bit)): xor은 두 객체의 크기가 같아야 합니다. (" <<
			    size << "/" << _bit.size << ")" << endl;
		exit(0);
	}

	// 비트별로 xor연산을 수행 후 rtBit에 저장
	Bit rtBit(size);	
	for (int i = 0; i < size; ++i) {
		rtBit.pBit[i] = (pBit[i] ^ _bit.pBit[i]);
	}

	// 임시 객체 반환
	return rtBit;
}

void Bit::operator=(const Bit& _bit) {
	// 우측항의 크기가 0인지 확인
	if (_bit.size <= 0) {
		cout << "([오류]void Bit::operator=(const Bit& _bit)): R-값의 크기가 0이하입니다. (" <<
			    size << "/" << _bit.size << ")" << endl;
		exit(0);
	}
	// 0이 아닐 시 좌측항에 우측항의 크기 대입
	else {
		size = _bit.size;
	}

	// 좌측항의 동적 비트공간 파괴
	if (pBit != NULL) {
		free(pBit);
		pBit = NULL;
	}

	// 좌측항에 동적 공간 생성
	if ((pBit = (bool*)malloc(sizeof(bool)* size)) == NULL) {
		cout << "[오류](void Bit::operator=(const Bit& _bit)): 메모리 할당 실패." << endl;
		exit(0);
	}

	// 좌측항에 우측항의 값 대입
	for (int i = 0; i < size; ++i) {
		pBit[i] = _bit.pBit[i];
	}
}

void Bit::operator=(const bool oneOrZero) {
	*pBit = oneOrZero;
}

bool& Bit::operator[](const int index) {
	// 인덱스 범위 오류 확인
	if (index < 0 || index >= size) {
		cout << "[오류](bool* Bit::operator[](const int index)): 인덱스 범위 오류. (" <<
			    size << "/" << index << ")" << endl;
		exit(0);
	}

	return pBit[index];
}

Bit Bit::operator<<(const int shiftLeft) {
	// 비트 크기 검사
	if (size <= 0) {
		cout << "[오류](Bit Bit::operator<<(const int shiftLeft)): 크기가 0이하인 비트는 쉬프트 할 수 없습니다. (size: " <<
			size << endl;
		exit(0);
	}

	// 반환을 위한 임시 객체 생성후 원본 비트 스트림 복사
	Bit rtBit(*this);

	// 매개변수 만큼 1회 라운드 쉬프팅 반복
	for (int shift = 0; shift < shiftLeft; ++shift) {
		// 첫 번째 비트를 임시 저장
		bool tempBit = rtBit.pBit[0];
		// 마지막 비트까지 좌측으로 1회 쉬프팅
		for (int i = 0; i < (size - 1); ++i) {
			rtBit.pBit[i] = rtBit.pBit[i + 1];
		}
		// 임시 저장한 값을 가장 마지막 비트에 대입
		rtBit.pBit[size - 1] = tempBit;
	}

	return rtBit;
}

Bit Bit::operator>>(const int shiftRight) {
	// 비트 크기 검사
	if (size <= 0) {
		cout << "[오류](Bit Bit::operator>>(const int shiftLeft)): 크기가 0이하인 비트는 쉬프트 할 수 없습니다. (size: " <<
			size << endl;
		exit(0);
	}

	// 반환을 위한 임시 객체 생성후 원본 비트 스트림 복사
	Bit rtBit(*this);

	// 매개변수 만큼 1회 라운드 쉬프팅 반복
	for (int shift = 0; shift < shiftRight; ++shift) {
		// 첫 번째 비트를 임시 저장
		bool tempBit = rtBit.pBit[size - 1];
		// 마지막 비트까지 좌측으로 1회 쉬프팅
		for (int i = (size - 1); i > 0; --i) {
			rtBit.pBit[i] = rtBit.pBit[i - 1];
		}
		// 임시 저장한 값을 가장 마지막 비트에 대입
		rtBit.pBit[0] = tempBit;
	}

	return rtBit;
}