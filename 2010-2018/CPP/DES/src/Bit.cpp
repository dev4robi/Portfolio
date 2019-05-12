#include "Bit.h"
#include "Math.h"
#include <iostream>
#include <string>
using namespace std;

Bit::Bit() {
	// ������ �⺻ �ʱ�ȭ
	size = 0;
	pBit = NULL;
}

Bit::Bit(const int _size) {
	// �ùٸ� ũ������ Ȯ��
	if (_size <= 0) {
		cout << "[����](Bit::Bit(const int _size)): �ùٸ� ����� �Է��Ͻʽÿ�." << endl;
		exit(0);
	}

	// ���޹��� ũ�⸸ŭ bool������ ���� ����
	if ((pBit = (bool*)malloc(sizeof(bool) * _size)) == NULL) {
		cout << "[����](Bit::Bit(const int _size)): �޸� �Ҵ� ����." << endl;
		exit(0);
	}

	// ����� ����
	size = _size;

	// ��Ʈ ��Ʈ���� 0���� �ʱ�ȭ
	for (int i = 0; i < size; ++i) {
		pBit[i] = 0;
	}
}

Bit::Bit(const char* string) {
	// ���̸� ����
	size = 64; /*(strlen(string) * 8);*/

	// �ùٸ� ũ������ Ȯ��
	if (size <= 0) {
		cout << "[����](Bit::Bit(const int _size)): �ùٸ� ����� �Է��Ͻʽÿ�." << endl;
		exit(0);
	}

	// ���޹��� ũ�⸸ŭ bool������ ���� ����
	if ((pBit = (bool*)malloc(sizeof(bool) * size)) == NULL) {
		cout << "[����](Bit::Bit(const int _size)): �޸� �Ҵ� ����." << endl;
		exit(0);
	}

	// ���ڿ��� ��Ʈ�� ��ȯ�Ͽ� ����
	for (int i = 0; i < (size / 8); ++i) {
		char c = string[i];

		// �� ����(8bit)�� �������� j�� ����Ʈ�ϰ� 0x01�� ����ŷ => j��° ��Ʈ�� ���� �� ����
		for (int j = 0; j < 8; ++j) {
			pBit[i * 8 + j] = ((c >> (7 - j)) & 0x01);
		}
	}
}

Bit::Bit(const Bit& _bit) {
	// ������(L-Value)�� ��Ʈ�� ����
	if (size > 0 && pBit != NULL) {
		free(pBit);
		pBit = NULL;
	}

	// �����׿� ���ο� ������ �� ����
	size = _bit.size;

	// �����׿� ���� ���� ����
	if ((pBit = (bool*)malloc(sizeof(bool) * size)) == NULL) {
		cout << "[����](Bit::Bit(const Bit& _bit)): �޸� �Ҵ� ����." << endl;
		exit(0);
	}

	// �����׿� ������(R-Value)�� �� ����
	for (int i = 0; i < size; ++i) {
		pBit[i] = _bit.pBit[i];
	}
}

Bit::~Bit() {
	// ����� 0 ���� ũ�� ��Ʈ �����Ͱ� NULL�� �ƴϸ� ���� ���� ����
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
			if (tempChar == 7 || tempChar == 8 || tempChar == 10 || tempChar == 13 || tempChar == 27 || tempChar == 127) // ��ü ����� �ʿ��� ����
				printf("?");
			else
				printf("%c", tempChar);
		}
	}
}

void Bit::swap() {
	// ��Ʈ��Ʈ�� ��ü�� ¦�� ���������� Ȯ��
	if (size % 2 != 0) {
		cout << "([����](void Bit::swap()): ����� ¦������ ������ �����մϴ�. (" << size << ")" << endl;
		exit(0);
	}

	// ���� ��Ʈ�� ���� ��Ʈ ������
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
	// ��ġ�� ũ�Ⱑ 48bit�� ��Ʈ��Ʈ���� ���� ����
	if (size != 48) {
		cout << "([����]Bit Bit::substitute(const table* table): ����� 48�� �ƴմϴ�. (" << size << ")" << endl;
		exit(0);
	}

	// ��ȯ�� ���� ��ü ����
	Bit rtBit(32);

	// �� ���(�� 8���)�� S-�ڽ��� �̿��Ͽ� ��ġ
	for (int i = 0; i < 8; ++i) {
		// S-�ڽ��� ���� Ri -> EP -> ^Key / 8 �� ���� �����Ų ��, �� �� �� �� ���� ��Ʈ�� �������� �����ȴ�
		// S-�ڽ��� ���� Ri -> EP -> ^Key / 8 �� ���� �����Ų ��, �� �� �� �� ���� ��Ʈ�� ������ ��Ʈ�� �������� �����ȴ�
		int row = 0;	int col = 0;
		row = ((pBit[i * 6] ? 1 : 0) * 2) + ((pBit[i * 6 + 5] ? 1 : 0));
		col = ((pBit[i * 6 + 1] ? 1 : 0) * 8) + ((pBit[i * 6 + 2] ? 1 : 0) * 4) + ((pBit[i * 6 + 3] ? 1 : 0) * 2) + ((pBit[i * 6 + 4] ? 1 : 0));
		// ���̺� ����
		table* table = &(g_SboxAry[i]);

		// �ش� ���̺��� ���� ������
		int tbBinary = table->ary[row * table->tableCol + col];

		// ������ ���� �������Ͽ� ��ȯ�� ��Ʈ��Ʈ���� ����
		for (int k = 3; k >= 0; --k) {
			rtBit[i * 4 + k] = (tbBinary % 2);
			tbBinary /= 2;
		}
	}

	return rtBit;
}

Bit Bit::subBit(const int _low, const int _high) {
	// �� ������ �ϳ��� �����̰ų� 0�̸� ����
	if (_low <= 0 || _high <= 0) {
		cout << "[����](Bit::Bit(const Bit& _bit, const int _low, const int _high)): �ùٸ��� ���� ���� ����. (" <<
			    _low << ", " << _high << ")" << endl;
		exit(0);
	}

	// �ʰ��� �����̸� ����
	if (_low > size || _high > size) {
		cout << "[����](Bit::Bit(const Bit& _bit, const int _low, const int _high)): �ʰ��� ���� ����. (" <<
			    _low << ", " << _high << ")" << endl;
		exit(0);
	}

	// ��ȯ�� �����Ʈ��Ʈ�� ����
	int subSize = abs(_high - _low + 1);
	Bit rtBit(subSize);

	// _low�� _high ������ �� ����
	int j = 0;
	for (int i = min(_high, _low); i <= max(_high, _low); ++i) {
		rtBit.pBit[j++] = pBit[i-1];
	}

	// ������ �����Ʈ��Ʈ�� ��ȯ
	return rtBit;
}

Bit Bit::append(const Bit& _bit) {
	Bit rtBit(size + _bit.size);

	// ���� ��Ʈ
	for (int i = 0; i < size; ++i) {
		rtBit.pBit[i] = pBit[i];
	}

	// ���� ��Ʈ
	for (int i = 0; i < _bit.size; ++i) {
		rtBit.pBit[i + size] = _bit.pBit[i];
	}

	// ������ ��Ʈ ��ȯ
	return rtBit;
}

Bit Bit::operator^(const Bit& _bit) {
	// �� ��Ʈ��ü�� ����� ������ Ȯ��
	if (size != _bit.size) {
		cout << "([����]Bit& Bit::operator^(const Bit& _bit)): xor�� �� ��ü�� ũ�Ⱑ ���ƾ� �մϴ�. (" <<
			    size << "/" << _bit.size << ")" << endl;
		exit(0);
	}

	// ��Ʈ���� xor������ ���� �� rtBit�� ����
	Bit rtBit(size);	
	for (int i = 0; i < size; ++i) {
		rtBit.pBit[i] = (pBit[i] ^ _bit.pBit[i]);
	}

	// �ӽ� ��ü ��ȯ
	return rtBit;
}

void Bit::operator=(const Bit& _bit) {
	// �������� ũ�Ⱑ 0���� Ȯ��
	if (_bit.size <= 0) {
		cout << "([����]void Bit::operator=(const Bit& _bit)): R-���� ũ�Ⱑ 0�����Դϴ�. (" <<
			    size << "/" << _bit.size << ")" << endl;
		exit(0);
	}
	// 0�� �ƴ� �� �����׿� �������� ũ�� ����
	else {
		size = _bit.size;
	}

	// �������� ���� ��Ʈ���� �ı�
	if (pBit != NULL) {
		free(pBit);
		pBit = NULL;
	}

	// �����׿� ���� ���� ����
	if ((pBit = (bool*)malloc(sizeof(bool)* size)) == NULL) {
		cout << "[����](void Bit::operator=(const Bit& _bit)): �޸� �Ҵ� ����." << endl;
		exit(0);
	}

	// �����׿� �������� �� ����
	for (int i = 0; i < size; ++i) {
		pBit[i] = _bit.pBit[i];
	}
}

void Bit::operator=(const bool oneOrZero) {
	*pBit = oneOrZero;
}

bool& Bit::operator[](const int index) {
	// �ε��� ���� ���� Ȯ��
	if (index < 0 || index >= size) {
		cout << "[����](bool* Bit::operator[](const int index)): �ε��� ���� ����. (" <<
			    size << "/" << index << ")" << endl;
		exit(0);
	}

	return pBit[index];
}

Bit Bit::operator<<(const int shiftLeft) {
	// ��Ʈ ũ�� �˻�
	if (size <= 0) {
		cout << "[����](Bit Bit::operator<<(const int shiftLeft)): ũ�Ⱑ 0������ ��Ʈ�� ����Ʈ �� �� �����ϴ�. (size: " <<
			size << endl;
		exit(0);
	}

	// ��ȯ�� ���� �ӽ� ��ü ������ ���� ��Ʈ ��Ʈ�� ����
	Bit rtBit(*this);

	// �Ű����� ��ŭ 1ȸ ���� ������ �ݺ�
	for (int shift = 0; shift < shiftLeft; ++shift) {
		// ù ��° ��Ʈ�� �ӽ� ����
		bool tempBit = rtBit.pBit[0];
		// ������ ��Ʈ���� �������� 1ȸ ������
		for (int i = 0; i < (size - 1); ++i) {
			rtBit.pBit[i] = rtBit.pBit[i + 1];
		}
		// �ӽ� ������ ���� ���� ������ ��Ʈ�� ����
		rtBit.pBit[size - 1] = tempBit;
	}

	return rtBit;
}

Bit Bit::operator>>(const int shiftRight) {
	// ��Ʈ ũ�� �˻�
	if (size <= 0) {
		cout << "[����](Bit Bit::operator>>(const int shiftLeft)): ũ�Ⱑ 0������ ��Ʈ�� ����Ʈ �� �� �����ϴ�. (size: " <<
			size << endl;
		exit(0);
	}

	// ��ȯ�� ���� �ӽ� ��ü ������ ���� ��Ʈ ��Ʈ�� ����
	Bit rtBit(*this);

	// �Ű����� ��ŭ 1ȸ ���� ������ �ݺ�
	for (int shift = 0; shift < shiftRight; ++shift) {
		// ù ��° ��Ʈ�� �ӽ� ����
		bool tempBit = rtBit.pBit[size - 1];
		// ������ ��Ʈ���� �������� 1ȸ ������
		for (int i = (size - 1); i > 0; --i) {
			rtBit.pBit[i] = rtBit.pBit[i - 1];
		}
		// �ӽ� ������ ���� ���� ������ ��Ʈ�� ����
		rtBit.pBit[0] = tempBit;
	}

	return rtBit;
}