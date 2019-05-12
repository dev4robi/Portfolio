#ifndef __TABLE_H__
#define __TABLE_H__

typedef struct tag_table {
	int tableRow;			// ���̺��� ��
	int tableCol;			// ���̺��� ��
	int* ary;				// ���̺��� ���� �ּ� (������ ���� �迭)
} table;

extern table g_IP;			// Initial Permutation : �ʱ� ���� (8x8 / 64bit -> 64bit)
extern table g_InverseIP;	// Inverse Initial Permutation : �� �ʱ���� (8x8 / 64bit -> 64bit)
extern table g_P;			// Permutation : ���� �Լ� (4x8 / 32bit -> 32bit)
extern table g_E;			// Expanshion : Ȯ�� ���� (8x6 / 32bit -> 48bit)
extern table g_PC1;			// Permutation Choice 1 : �и�Ƽ ��Ʈ�� ������ ���� (8x7 / 64bit -> 56bit)
extern table g_PC2;			// Permutation Choice 2 : ����Ű ���� ���� (6x8 / 48bit -> 48bit)
extern table g_SboxAry[8];	// S-Box Array
extern table g_S1;			// Substitution Box 1 : ��ġ(ġȯ) ���� ���� 1 (4x16 / 6bit -> 4bit)
extern table g_S2;			// Substitution Box 2 : ��ġ(ġȯ) ���� ���� 2 (4x16 / 6bit -> 4bit)
extern table g_S3;			// Substitution Box 3 : ��ġ(ġȯ) ���� ���� 3 (4x16 / 6bit -> 4bit)
extern table g_S4;			// Substitution Box 4 : ��ġ(ġȯ) ���� ���� 4 (4x16 / 6bit -> 4bit)
extern table g_S5;			// Substitution Box 5 : ��ġ(ġȯ) ���� ���� 5 (4x16 / 6bit -> 4bit)
extern table g_S6;			// Substitution Box 6 : ��ġ(ġȯ) ���� ���� 6 (4x16 / 6bit -> 4bit)
extern table g_S7;			// Substitution Box 7 : ��ġ(ġȯ) ���� ���� 7 (4x16 / 6bit -> 4bit)
extern table g_S8;			// Substitution Box 8 : ��ġ(ġȯ) ���� ���� 8 (4x16 / 6bit -> 4bit)
extern table g_ShiftLeft;	// Shift Left Scedule Table : Ű�� ���� �̵� ���̺� (2x16)

void MakeTable(table* _tb, int _row, int _col, int ary[]);	// Ư�� ���̺� Ư�� ���� �ִ� �Լ�
void InitTable();											// ��� ���̺� ���� �ʱ�ȭ��Ű�� �Լ�

#endif