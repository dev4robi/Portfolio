#ifndef __BIT_H__
#define __BIT_H__

#include "Table.h"

enum Option {
	OPTION_BINARY, OPTION_HEXA, OPTION_CHAR
};

class Bit {
private:
	int size;		// ��Ʈ ������
	bool* pBit;		// ù ��° ��Ʈ�� ������

public:
	Bit();											// ������
	Bit(const int _size);							// ������ (ũ��)
	Bit(const char* string);						// ������ (���ڿ�)
	Bit(const Bit& _bit);							// ���� ������
	~Bit();											// �Ҹ���

	int getSize() const;							// ������ ��ȯ
	void print(const Option option) const;			// ��Ʈ ��Ʈ�� ���
	void swap();									// ���� (size/n) ��Ʈ�� ���� (size/n) ��Ʈ�� ������
	Bit permutate(const table* _table);				// ���̺��� �Է¹޾Ƽ� ��ġ�� ������ ��Ʈ ��Ʈ���� ��ȯ
	Bit substitute();								// S-BOX ġȯ�� ����� ��Ʈ ��Ʈ���� ��ȯ (48bit -> 32bit)
	Bit subBit(const int _low, const int _high);	// ���� ��Ʈ�� ��ȯ�ϴ� �Լ�
	Bit append(const Bit& _bit);					// BitŬ�������� ��ħ

	Bit operator^(const Bit& _bit);					// BitŬ�������� xor���� ����
	void operator=(const Bit& _bit);				// BitŬ�������� �Ҵ� ���� ����
	void operator=(const bool oneOrZero);			// ���� ��Ʈ�迭�� �� ����
	bool& operator[](const int index);				// ���� ��Ʈ�迭�� �ش� ��ġ ������ ��ȯ
	Bit operator<<(const int shiftLeft);			// ���� ���� ����Ʈ
	Bit operator>>(const int shiftRight);			// ���� ���� ����Ʈ
};

#endif