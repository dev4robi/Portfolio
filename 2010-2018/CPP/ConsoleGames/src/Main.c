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
void SetCurType(int CurType);

// ���� ã�� �Ϲ� �Լ� ������Ÿ��
extern int ms_main();
extern void om_main();

//
int main()
{
	int ChoiceGame;

	while(1)
	{
		system("mode con cols=80 lines=25"); // �ܼ� ������ ����
		system("title ConsoleGames"); // �ܼ� ���� ����
		ConClr();

		printf("��������������������������������������������������������������������������������");
		printf("��                                                                            ��");
		printf("��    Console Games  ver 0.1                                                  ��");
		printf("��                                       [Minesweeper By : 2010305049 ������] ��");
		printf("��                                       [   Omok   By   : 2010305042 �����] ��");
		printf("�Ʀ�����������������������������������������������������������������������������");
		printf("\n");
		printf(" 1. ����\n");
		printf(" 2. ���� ã��\n");
		printf(" 3. ���α׷� ����\n");
		printf("\n  �� ���Ͻô� �׸��� �����ϼ��� : ");
		scanf("%d", &ChoiceGame);

		switch(ChoiceGame)
		{
			case 1: // ����
				om_main();
				break;

			case 2: // ����ã��
				ms_main();
				break;

			case 3: // ���α׷� ����
				exit(0);
				break;

			default:
				break;
		}
	}

	return 0;
}

// �ش� ���ڿ��� ��Ʈ���� ������ �����Ͽ� ����ϰ� ����ȸ��, ���� ��� �Ӽ����� �ǵ��� ���´�.
void Cprintf(char *String, int FontCR, int BackCR)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackCR << 4) | FontCR));

	printf("%s", String);

	FontCR = CR_GRAY;
	BackCR = CR_BLACK;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackCR << 4) | FontCR));
}

// ���ķ� ����ϴ� ������ ��Ʈ���� ������ �����Ų��.
void ChangeCR(int FontCR, int BackCR)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackCR << 4) | FontCR));
}

// �ܼ� ȭ���� �����Ѵ�.
void ConClr()
{
	system("cls");
}

// �ܼ� â���� Ŀ���� ��ġ�� �ش� x, y��ǥ�� �̵���Ų��.
void MoveCur(int x, int y)
{
	COORD Cursor;

	Cursor.X = x;
	Cursor.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cursor);
}

// �ּҰ� �ִ밪 �������� ������ ���� ��ȯ�ϴ� �Լ�.
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

// Ŀ���� ����ų� ���̴� �Լ�.
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

// Ŀ���� X��ǥ Ȥ�� Y��ǥ�� ��ȯ�Ѵ�
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

// �������� �������� �Լ�.
void BipSound(int Pitch, int Duration, boolean UseSound)
{
	if(UseSound)
		Beep(Pitch, Duration);
}