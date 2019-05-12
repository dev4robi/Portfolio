#ifndef __GLOBAL_VAR_H__
#define __GLOBAL_VAR_H__

// [Text Setting]
extern const char PADDING_CHAR;	// �е��� ����
extern const int TEXT_MAX;		// ��Ʈ���� �ִ� ����
extern char** planeTextMat;		// ���� �ؽ�Ʈ ��Ʈ����
extern int textMatCol;			// �ؽ�Ʈ ��Ʈ���� ��
extern int textMatRow;			// �ؽ�Ʈ ��Ʈ���� ��

// [Key Matrix Setting]
extern int lowBound;			// Ű�� �ּҰ�
extern int highBound;			// Ű�� �ִ밪
extern int keyMatSize;			// Ű ��Ʈ������ ������(�⺻ 3x3)

// [Charset and zStar Setting]
extern const int charCnt;		// ������ ����
extern int zStarCnt;			// zStar Count
extern int* zStar;				// zStar
extern int* zStarInv;			// zStar^-1 (Inverse)

#endif