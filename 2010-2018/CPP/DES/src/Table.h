#ifndef __TABLE_H__
#define __TABLE_H__

typedef struct tag_table {
	int tableRow;			// 테이블의 행
	int tableCol;			// 테이블의 열
	int* ary;				// 테이블의 시작 주소 (일차원 동적 배열)
} table;

extern table g_IP;			// Initial Permutation : 초기 순열 (8x8 / 64bit -> 64bit)
extern table g_InverseIP;	// Inverse Initial Permutation : 역 초기순열 (8x8 / 64bit -> 64bit)
extern table g_P;			// Permutation : 순열 함수 (4x8 / 32bit -> 32bit)
extern table g_E;			// Expanshion : 확장 순열 (8x6 / 32bit -> 48bit)
extern table g_PC1;			// Permutation Choice 1 : 패리티 비트를 제거한 순열 (8x7 / 64bit -> 56bit)
extern table g_PC2;			// Permutation Choice 2 : 서브키 생성 순열 (6x8 / 48bit -> 48bit)
extern table g_SboxAry[8];	// S-Box Array
extern table g_S1;			// Substitution Box 1 : 대치(치환) 상자 순열 1 (4x16 / 6bit -> 4bit)
extern table g_S2;			// Substitution Box 2 : 대치(치환) 상자 순열 2 (4x16 / 6bit -> 4bit)
extern table g_S3;			// Substitution Box 3 : 대치(치환) 상자 순열 3 (4x16 / 6bit -> 4bit)
extern table g_S4;			// Substitution Box 4 : 대치(치환) 상자 순열 4 (4x16 / 6bit -> 4bit)
extern table g_S5;			// Substitution Box 5 : 대치(치환) 상자 순열 5 (4x16 / 6bit -> 4bit)
extern table g_S6;			// Substitution Box 6 : 대치(치환) 상자 순열 6 (4x16 / 6bit -> 4bit)
extern table g_S7;			// Substitution Box 7 : 대치(치환) 상자 순열 7 (4x16 / 6bit -> 4bit)
extern table g_S8;			// Substitution Box 8 : 대치(치환) 상자 순열 8 (4x16 / 6bit -> 4bit)
extern table g_ShiftLeft;	// Shift Left Scedule Table : 키의 좌측 이동 테이블 (2x16)

void MakeTable(table* _tb, int _row, int _col, int ary[]);	// 특정 테이블에 특정 값을 넣는 함수
void InitTable();											// 모든 테이블 값을 초기화시키는 함수

#endif