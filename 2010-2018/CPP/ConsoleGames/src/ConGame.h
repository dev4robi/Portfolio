//////////////////////////////////////////////////////////////////////////
// Extra_Console_Ability_Function(ECAF)
//
// ver 1.00 (오후 1:20 2011-11-21)
//

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

// Color Ref
#define CR_BLACK 0
#define CR_DBLUE 1
#define	CR_DGREEN 2
#define CR_DCYAN 3
#define CR_DRED 4
#define CR_DPURPLE 5
#define CR_GOLD 6
#define CR_GRAY 7
#define CR_DGRAY 8
#define CR_BLUE 9
#define CR_GREEN 10
#define CR_CYAN 11
#define CR_RED 12
#define CR_PURPLE 13
#define CR_YELLOW 14
#define CR_WHITE 15

// Cursor Type
#define CT_HIDE 0
#define CT_SOLID 1
#define CT_NORMAL 2

void Cprintf(char *String, int FontCR, int BackCR);
void ChangeCR(int FontCR, int BackCR);
void ConClr();
void MoveCur(int x, int y);
int Random_Int(int Min, int Max);
void BipSound(int Pitch, int Duration, boolean UseSound);

// 해당 문자열의 폰트색과 배경색을 변경하여 출력하고 밝은회색, 검은 배경 속성으로 되돌려 놓는다.
void Cprintf(char *String, int FontCR, int BackCR)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackCR << 4) | FontCR));

	printf("%s", String);

	FontCR = CR_GRAY;
	BackCR = CR_BLACK;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackCR << 4) | FontCR));
}

// 이후로 출력하는 문자의 폰트색과 배경색을 변경시킨다.
void ChangeCR(int FontCR, int BackCR)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackCR << 4) | FontCR));
}

// 콘솔 화면을 정리한다.
void ConClr()
{
	system("cls");
}

// 콘솔 창에서 커서의 위치를 해당 x, y좌표로 이동시킨다.
void MoveCur(int x, int y)
{
	COORD Cursor;

	Cursor.X = x;
	Cursor.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cursor);
}

// 최소값 최대값 범위내의 무작위 수를 반환하는 함수.
int Random_Int(int Min, int Max)
{
	int Random, Temp;

	if(Min > Max)
	{
		Temp = Min;
		Min = Max;
		Max = Temp;
	}

	Random = (rand() % ((Max - Min) + 1)) + Min;

	return Random;
}

// 커서를 숨기거나 보이는 함수.
void SetCurType(int CurType)
{
	CONSOLE_CURSOR_INFO CursorInfo;
	
	switch(CurType)
	{
		case CT_HIDE:
			CursorInfo.dwSize = 1;
			CursorInfo.bVisible = FALSE;
			break;
	
		case CT_SOLID:
			CursorInfo.dwSize = 100;
			CursorInfo.bVisible = TRUE;
			break;
	
		case CT_NORMAL:
			CursorInfo.dwSize = 20;
			CursorInfo.bVisible = TRUE;
			break;
	}
	
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}

// 커서의 X좌표 혹은 Y좌표를 반환한다
int CursorXY(char CurXorY)
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;
	int CursorX_Y = -1;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);

	if(CurXorY == 'X' || CurXorY == 'x' || CurXorY == '0')
		CursorX_Y = BufInfo.dwCursorPosition.X;

	else if(CurXorY == 'Y' || CurXorY == 'y' || CurXorY == '1')
		CursorX_Y = BufInfo.dwCursorPosition.Y;

	else
		CursorX_Y = -1;
	
	return CursorX_Y;
}

// 비프음을 내보내는 함수.
void BipSound(int Pitch, int Duration, boolean UseSound)
{
	if(UseSound)
		Beep(Pitch, Duration);
}