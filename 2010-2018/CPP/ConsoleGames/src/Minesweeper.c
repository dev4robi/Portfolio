/************************************************************************/
/* [Minesweeper game using C language]                                  */
/*                                                                      */
/*  # Produced by "2010305049 Lee Tae Hoon"                             */
/*																		*/
/*	# 2011/12/3 : 1.00						                            */
/*	# The last update: 2011/12/3        								*/
/*									     	     ����  ����������������	*/
/*												 ����������������������	*/
/*												 ������  ����  ��������	*/
/************************************************************************/

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>

// ��ũ��
#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define GS_MAINMENU 0
#define GS_STARTGAME 1
#define GS_LOADGAME 2
#define GS_OPTION 3
#define GS_EXIT 4
#define GS_WIN 5
#define GS_LOSE 6
#define GS_RESTRAT 7

#define BT_HIDE 0
#define BT_UNCOVER 1
#define BT_FLAG 2
#define BT_QUESTION 3
#define BT_MISS 4

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

struct GameBoard
{
	int BoardData;
	int VisualType;
};

// ���� ����
boolean gb_Testmode = FALSE; // �׽�Ʈ ��� (�ʱ�ȭ: ���)
boolean gb_Colormode = TRUE; // ���� ��� ��� (�ʱ�ȭ: ���)
boolean gb_Soundmode = TRUE; // �Ҹ� ��� (�ʱ�ȭ: ���)
int gb_GameState = GS_MAINMENU; // ���� ���� (�ʱ�ȭ: ���θ޴�)
char gb_GameVersion[] = "1.00"; // ���� ����
int gb_SelectLevel = 1; // ���õ� ���̵� (�ʱ�ȭ: �ʺ�)
int gb_SelectUI = 1; // ���õ� UI (�ʱ�ȭ: �⺻)
struct GameBoard gb_MainBoard[20][30]; // (���� ����)
int gb_SizeX = 9, gb_SizeY = 9; // ������ ������ (�ʱ�ȭ: �ʺ�)
int gb_Mines = 10; // ���ڼ� (�ʱ�ȭ: �ʺ�)
int gb_UIMines = 10; // �������̽��� ǥ�õǴ� ���ڼ�
int gb_UIrenewtimer = 10000; // UI ������ ���� Ÿ�̸Ӻ���
int gb_ClickCount = 0; // ��ư Ŭ�� Ƚ�� (�ʱ�ȭ: 0)
int gb_Time = 0; // ��� �ð��� �����ϱ� ���� ����

//
extern void Cprintf(char *String, int FontCR, int BackCR);
extern void ChangeCR(int FontCR, int BackCR);
extern void ConClr();
extern void MoveCur(int x, int y);
extern int Random_Int(int Min, int Max);
extern void BipSound(int Pitch, int Duration, boolean UseSound);
extern void SetCurType(int CurType);

// ���� ã�� �Ϲ� �Լ� ������Ÿ��
void ms_Initialization(); // �ʱ�ȭ �Լ�
void ms_DrawMainMenu(); // ���θ޴� �׸��� �Լ�
int ms_OnMainMenu(); // ���θ޴� ���� ���� �Լ�
void ms_DrawOptionMenu(); // �ɼǸ޴� �׸��� �Լ�
void ms_OnOptionMenu(); // �ɼǸ޴� ���� ���� �Լ�
void ms_DrawGameBoard(); // ���Ӻ��� �׸��� �Լ�
void ms_FormatBoardData(int i, int j, int BackColor); // ������ �����͸� UI/���� ���� ����� ���ڷ� ��½�Ű�� �Լ�
int ms_OnGameBoard(); // ������ ���� ���� �Լ�
int ms_LeftClick(int x, int y, boolean Recursive); // ���� Ŭ�� �׼�
void ms_RightClick(int x, int y); // ������ Ŭ�� �׼�
int ms_DualClick(int x, int y); // ���� Ŭ�� �׼�
void ms_CreateMine(int x, int y); // ���� ���� �Լ� (ù Ŭ���� ����. �Ű������� ùŬ�� ���� ����)
void ms_PlusBoardNumber(int x, int y); // �Էµ� ��ǥ�� ���� �����͸� 1���� (�����Ͱ� 8�̸��� ��쿡�� ����)
void ms_DrawGameOver(); // ���� ������ ȭ�� ����� �Է� ���
void ms_DrawWin(); // �¸��ϸ� ȭ�� ����� �Է� ���
int ms_CheckWin(); // �¸� ���� �Ǵ� �Լ�

// �ڽ� �Լ� ������Ÿ��
void TypeBox(char String[76], int ColorFont, int ColorBack);
void EraseBox();

// ���� �Լ� ������Ÿ��
void BCprintf(char *String, int FontCR, int BackCR);

// �Լ�
int ms_main()
{	
	srand(GetTickCount());

	ms_Initialization();

	while(1)
	{
		switch(gb_GameState)
		{
			case GS_MAINMENU:
				SetCurType(CT_HIDE);
				ms_DrawMainMenu();	
				gb_GameState = ms_OnMainMenu();
				break;

			case GS_STARTGAME:
				ms_DrawGameBoard();
				gb_GameState = ms_OnGameBoard();
				break;

			case GS_LOADGAME:
				gb_GameState = GS_MAINMENU;
				break;

			case GS_OPTION:
				ms_DrawOptionMenu();
				ms_OnOptionMenu();
				break;

			case GS_EXIT:
				MoveCur(2, 22);
				return 0;
				break;

			case GS_WIN:
				ms_DrawWin();
				gb_GameState = GS_MAINMENU;
				break;

			case GS_LOSE:
				ms_DrawGameOver();
				gb_GameState = GS_MAINMENU;
				break;

			case GS_RESTRAT:
				ms_DrawGameBoard();
				gb_GameState = ms_OnGameBoard();
				break;

			default:
				return 0;
		}
	}

	return 0;
}

void ms_Initialization()
{
	int i, j;

	ConClr();

	if(!gb_Testmode) system("mode con cols=80 lines=25"); // �ܼ� ������ ����
	else system("mode con cols=80 lines=30");

	if(!gb_Testmode) system("title ConsoleGames - Minesweeper"); // �ܼ� ���� ����
	else system("title ConsoleGames - Minesweeper Testmode Applied");

	gb_GameState = GS_MAINMENU; // ���� ���� (�ʱ�ȭ: ���θ޴�)
	// strcpy(gb_GameVersion, "1.00"); // ���� ����
	// gb_SelectLevel = 1; // ���õ� ���̵� (�ʱ�ȭ: �ʺ�)
	// gb_SelectUI = 1; // ���õ� UI (�ʱ�ȭ: �⺻)
	// gb_Colormode = TRUE; // ���� ��� ���(�ʱ�ȭ: ���)
	// gb_Soundmode = TRUE; // �Ҹ� ��� (�ʱ�ȭ: ���)
	gb_UIMines = gb_Mines; // �������̽� ���� �� = ���� ���� �� (����ȭ)
	gb_ClickCount = 0; // ��ư Ŭ�� Ƚ�� (�ʱ�ȭ: 0)
	gb_Time = 0; // �ð� �ʱ�ȭ

	for(i=0; i<20; i++) // ���� ���� �ʱ�ȭ
	{
		for(j=0; j<30; j++)
		{
			gb_MainBoard[i][j].BoardData = 0;
			gb_MainBoard[i][j].VisualType = BT_HIDE;
		}
	}
	
	// gb_SizeX = 9; // ������ ������ (�ʱ�ȭ: �ʺ�)
	// gb_SizeY = 9;

	SetCurType(CT_HIDE); // Ŀ�� ����
}

void ms_DrawMainMenu() // GS_MAINMENU
{
	ConClr();
	BCprintf("��������������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                              [Made By : 2010305049 ������] ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("�Ʀ�����������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	MoveCur(2, 2); BCprintf("    �� �� ã �� ver ", CR_WHITE, CR_BLACK); MoveCur(0, 8);
	BCprintf("                          ����������������������������                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��                        ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��   �� [ �����ϱ� ] ��   ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��                        ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��   �� [ �̾��ϱ� ] ��   ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��                        ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��   ��  [ ��  �� ]  ��   ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��                        ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��   �� [ �������� ] ��   ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ��                        ��                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          �Ʀ�������������������������                          ", CR_WHITE, CR_BLACK);
	MoveCur(0, 21);
	BCprintf("��������������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("�Ʀ�����������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	MoveCur(34, 12); BCprintf("[ �̾��ϱ� ]", CR_DGRAY, CR_BLACK);
	TypeBox("������ ���� �����մϴ�.", CR_WHITE, CR_BLACK);
	
	if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	MoveCur(22, 2); printf("%s", gb_GameVersion); ChangeCR(CR_GRAY, CR_BLACK);
	
	if(gb_Testmode) { MoveCur(61, 2); Cprintf("Testmode Applied", CR_RED, CR_BLACK); } // ���� ���� / �׽�Ʈ��� Ȯ��

	MoveCur(2, 12);
}

void ms_DrawOptionMenu() // GS_OPTION
{
	ConClr();
	BCprintf("��������������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("�Ʀ�����������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	MoveCur(2, 2); BCprintf("    ��  ��  ��  ��  (Option)", CR_WHITE, CR_BLACK);
	MoveCur(0, 7);
	BCprintf("��������������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��   �� [ ���̵� ] :: �ʱ� (����: 9 / ����: 9 / ���� : 10)                    ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��   �� [ UI���� ] :: ���� (�ܡۨ��������٨ݢ�)                       ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��   �� [ ��  �� ] :: ���                                                    ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��   �� [ ��  �� ] :: ���                                                    ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("��   �� [ ���� �޴��� ]                                                       ��", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("�Ʀ�����������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	MoveCur(0, 21);
	BCprintf("��������������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	BCprintf("��                                                                            ��", CR_WHITE, CR_BLACK);
	BCprintf("�Ʀ�����������������������������������������������������������������������������", CR_WHITE, CR_BLACK);
	TypeBox("���� ���̵��� �����մϴ�. �ʱ�, �߱�, ��� ���̵��� �ֽ��ϴ�.               ", CR_WHITE, CR_BLACK);

	// ���̵�
	if(gb_SelectLevel == 1) { MoveCur(22, 9); BCprintf("�ʱ�", CR_YELLOW, CR_BLACK); BCprintf(" (����: 9 / ����: 9 / ���� : 10)          ", CR_DGRAY, CR_BLACK); } 
	else if(gb_SelectLevel == 2) { MoveCur(22, 9); BCprintf("�߱�", CR_DGREEN, CR_BLACK); BCprintf(" (����: 16 / ����: 16 / ���� : 40)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectLevel == 3) { MoveCur(22, 9); BCprintf("���", CR_RED, CR_BLACK); BCprintf(" (����: 30 / ����: 18 / ���� : 112)          ", CR_DGRAY, CR_BLACK); }
	
	// UI����
	if(gb_SelectUI == 1) { MoveCur(22, 11); BCprintf("����", CR_WHITE, CR_BLACK); BCprintf(" (�ܡۨ��������٨ݢ�)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectUI == 2) { MoveCur(22, 11); BCprintf("�׸�", CR_WHITE, CR_BLACK); BCprintf(" (�����������٩ݢ�)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectUI == 3) { MoveCur(22, 11); BCprintf("ȥ��", CR_WHITE, CR_BLACK); BCprintf(" (�ߡ����̻�����ĥ�ȡ�??��)          ", CR_DGRAY, CR_BLACK); }

	// ����
	if(gb_Colormode) { MoveCur(22, 13); Cprintf("���          ", CR_GOLD, CR_BLACK); }
	else { MoveCur(22, 13); printf("��� ����          "); }

	// �Ҹ�
	if(gb_Soundmode) { MoveCur(22, 15); BCprintf("���          ", CR_DCYAN, CR_BLACK); }
	else { MoveCur(22, 15); BCprintf("��� ����          ", CR_DCYAN, CR_BLACK); }

	MoveCur(2, 12);
}

void ms_DrawGameBoard() // GS_GAMESTRAT
{
	int i, j;

	ConClr();

	// Į�� ����̸� ��� �� ���� ������� �׸���
	if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	
	// �ֺ� ���
	printf("��������������������������������������������������������������������������������");
	for(i=0; i<20; i++)
	printf("��                                                                ��          ��");
	printf("��������������������������������������������������������������������������������");
	printf("��                                                                            ��");
	printf("�Ʀ�����������������������������������������������������������������������������");
	
	// ���� �׵θ� �׸���(����)
	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("��");
		else if(i==gb_SizeX) printf("��");
		else printf("��");
	}

	// ���� �׵θ� �׸���(����)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("��"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("��"); }
		}
	}

	// ���� �׵θ� �׸���(�ϴ�)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("��");
		else if(i==gb_SizeX) printf("��");
		else printf("��");
	}

	// ���� �޴�â & ������ �ð�
	MoveCur(68, 2); (gb_Colormode) ? Cprintf(" ���޴��� ", CR_DGREEN, CR_BLACK) : printf(" ���޴��� ");
	MoveCur(68, 5); (gb_Colormode) ? Cprintf("[�����ϱ�]", CR_WHITE, CR_BLACK) : printf("[�����ϱ�]");
	MoveCur(68, 7); (gb_Colormode) ? Cprintf("[�����ϱ�]", CR_DGRAY, CR_BLACK) : printf("[�����ϱ�]");
	MoveCur(68, 9); (gb_Colormode) ? Cprintf("[���θ޴�]", CR_WHITE, CR_BLACK) : printf("[���θ޴�]");
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("��������������", CR_WHITE, CR_BLACK) : printf("��������������");
	MoveCur(68, 13); (gb_Colormode) ? Cprintf("�����ڼ���", CR_RED, CR_BLACK) : printf("�����ڼ���");
	MoveCur(68, 15); (gb_Colormode) ? Cprintf("    00    ", CR_WHITE, CR_BLACK) : printf("    00    ");
	MoveCur(68, 17); (gb_Colormode) ? Cprintf(" ���ð��� ", CR_RED, CR_BLACK) : printf(" ���ð��� ");
	MoveCur(68, 19); (gb_Colormode) ? Cprintf("[00:00:00]", CR_WHITE, CR_BLACK) : printf("[00:00:00]");
	
	// ���� �׸���
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=0; j<gb_SizeX; j++)
		{
			MoveCur(j*2+4, i+2);
			ms_FormatBoardData(i, j, CR_GRAY);
		}
	}

	// ���� ���� �׸���
	if(gb_ClickCount == 0) { MoveCur(4, 2); ms_FormatBoardData(0, 0, CR_WHITE); }

	// �ʱ� ����
	TypeBox("[A]:���� Ŭ��  [S]:���� Ŭ��  [D]:������ Ŭ��  [�����]:�̵�  [ESC]:�޴�", CR_WHITE, CR_BLACK);

	// �Ǻ� ���� (���� ��� ���ҽ�)
	if(!(gb_Colormode)) { SetCurType(CT_SOLID); MoveCur(4, 2); }
	else MoveCur(2, 12);
}

void ms_FormatBoardData(int y, int x, int BackColor)
{
	// [UI] �ܡۨ��������٨ݢ� / �����������٩ݢ� / �ߡ����̻�����ĥ�ȡ�??��
	// [CR] 0: ��ȸ�� 1:û�� 2:����� 3:���� 4:���� 5:���� 6:û�� 7:��� 8:ȸ�� ?:���� m:��� f:����

	if(gb_MainBoard[y][x].VisualType == BT_HIDE) // ���� Ȯ�ε��� ���� ����.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_UNCOVER) // Ŭ���Ǿ� Ȯ�ε� ����.
	{
		switch(gb_MainBoard[y][x].BoardData)
		{
			case 0:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��"); break;
			case 1:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLUE, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLUE, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLUE, BackColor) : printf("��"); break;
			case 2:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGREEN, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGREEN, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGREEN, BackColor) : printf("��"); break;
			case 3:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_RED, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_RED, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_RED, BackColor) : printf("��"); break;
			case 4:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DBLUE, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DBLUE, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_DBLUE, BackColor) : printf("��"); break;
			case 5:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DRED, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DRED, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_DRED, BackColor) : printf("��"); break;
			case 6:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DCYAN, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DCYAN, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_DCYAN, BackColor) : printf("��"); break;
			case 7:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLACK, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLACK, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("ĥ", CR_BLACK, BackColor) : printf("ĥ"); break;
			case 8:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_DGRAY, BackColor) : printf("��"); break;
			case 9:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLACK, BackColor) : printf("��");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLACK, BackColor) : printf("��");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_BLACK, BackColor) : printf("��"); break;
			default:
				(gb_Colormode == TRUE) ? Cprintf("??", CR_GOLD, BackColor) : printf("??");
		}
	}

	else if(gb_MainBoard[y][x].VisualType == BT_FLAG) // ��� ǥ�ø� �� ���� ����.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_RED, BackColor) : printf("��");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_RED, BackColor) : printf("��");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("��", CR_RED, BackColor) : printf("��");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_QUESTION) // ����ǥ ǥ�ø� �� ���� ����.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("��", CR_DBLUE, BackColor) : printf("��");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("��", CR_DBLUE, BackColor) : printf("��");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("??", CR_DBLUE, BackColor) : printf("??");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_MISS) // ���ڷ� �߸� ǥ���� ����.
	{
		BCprintf("��", CR_DRED, CR_GRAY);
	}
}

void ms_DrawGameOver()
{	
	int i, j;
	char KeyValue;

	// �ֺ� ��� �׸���
	MoveCur(0, 0); BCprintf("��������������������������������������������������������������������������������", CR_RED, CR_BLACK);
	for(i=1; i<21; i++)
	{
		MoveCur(0, i); BCprintf("��", CR_RED, CR_BLACK); MoveCur(66, i); BCprintf("��", CR_RED, CR_BLACK); MoveCur(78, i); BCprintf("��", CR_RED, CR_BLACK);
	}
	MoveCur(0, 21); BCprintf("��������������������������������������������������������������������������������", CR_RED, CR_BLACK);
	MoveCur(0, 22); BCprintf("��                                                                            ��", CR_RED, CR_BLACK);
	MoveCur(0, 23); BCprintf("�Ʀ�����������������������������������������������������������������������������", CR_RED, CR_BLACK);
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("��������������", CR_RED, CR_BLACK) : printf("��������������");

	// ���� �׵θ� �׸���(����)
	if(gb_Colormode) ChangeCR(CR_RED, CR_BLACK);

	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("��");
		else if(i==gb_SizeX) printf("��");
		else printf("��");
	}
	
	// ���� �׵θ� �׸���(����)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("��"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("��"); }
		}
	}
	
	// ���� �׵θ� �׸���(�ϴ�)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("��");
		else if(i==gb_SizeX) printf("��");
		else printf("��");
	}

	// �����
	BipSound(1000, 500, gb_Soundmode); Sleep(250);
	TypeBox("���ڰ� ���õǾ����ϴ�... Game Over �������� ���ư����� [ESC]�� �����ʽÿ�.", CR_RED, CR_BLACK);

	// ��� ���� ���̱�, �߸� ��� ������ ���̱�
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=0; j<gb_SizeX; j++)
		{
			MoveCur(j*2+4, i+2);

			if(gb_MainBoard[i][j].BoardData == 9 && gb_MainBoard[i][j].VisualType != BT_UNCOVER)
			{
				if(gb_MainBoard[i][j].VisualType != BT_FLAG)
				{
					gb_MainBoard[i][j].VisualType = BT_UNCOVER;
					ms_FormatBoardData(i, j, CR_GRAY);
				}
			}

			else if(gb_MainBoard[i][j].BoardData != 9)
			{
				if(gb_MainBoard[i][j].VisualType == BT_FLAG)
				{
					gb_MainBoard[i][j].VisualType = BT_MISS;
					ms_FormatBoardData(i, j, CR_GRAY);
				}
			}
		}
	}
	
	// Ű �Է� ���
	while(1)
	{
		if(kbhit())
		{
			KeyValue = getch();
			
			if(KeyValue == KEY_ESC) break;
		}
	}

	ChangeCR(CR_GRAY, CR_BLACK);
	ms_Initialization();
}

void ms_DrawWin()
{	
	int i, j;
	char KeyValue;
	
	// �ֺ� ��� �׸���
	MoveCur(0, 0); BCprintf("��������������������������������������������������������������������������������", CR_GREEN, CR_BLACK);
	for(i=1; i<21; i++)
	{
		MoveCur(0, i); BCprintf("��", CR_GREEN, CR_BLACK); MoveCur(66, i); BCprintf("��", CR_GREEN, CR_BLACK); MoveCur(78, i); BCprintf("��", CR_GREEN, CR_BLACK);
	}
	MoveCur(0, 21); BCprintf("��������������������������������������������������������������������������������", CR_GREEN, CR_BLACK);
	MoveCur(0, 22); BCprintf("��                                                                            ��", CR_GREEN, CR_BLACK);
	MoveCur(0, 23); BCprintf("�Ʀ�����������������������������������������������������������������������������", CR_GREEN, CR_BLACK);
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("��������������", CR_GREEN, CR_BLACK) : printf("��������������");
	
	// ���� �׵θ� �׸���(����)
	if(gb_Colormode) ChangeCR(CR_GREEN, CR_BLACK);
	
	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("��");
		else if(i==gb_SizeX) printf("��");
		else printf("��");
	}
	
	// ���� �׵θ� �׸���(����)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("��"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("��"); }
		}
	}
	
	// ���� �׵θ� �׸���(�ϴ�)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("��");
		else if(i==gb_SizeX) printf("��");
		else printf("��");
	}
	
	// �¸� BGM
	BipSound(261, 333, gb_Soundmode); BipSound(329, 333, gb_Soundmode); BipSound(391, 333, gb_Soundmode); Sleep(250);
	TypeBox("�����Ͽ����ϴ�! ���� �ð��� ", CR_GREEN, CR_BLACK);
	ChangeCR(CR_GREEN, CR_BLACK); printf("[%.2d:%.2d:%.2d]�Դϴ�. �������� ���ư��� : [ESC]Ű", (gb_Time/3600), ((gb_Time-(gb_Time/3600))/60), (gb_Time%60));
	ChangeCR(CR_GRAY, CR_BLACK);
	
	// ��� ���ڿ� ��� ����
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=0; j<gb_SizeX; j++)
		{
			MoveCur(j*2+4, i+2);
			
			if(gb_MainBoard[i][j].VisualType == BT_HIDE)
			{
				gb_MainBoard[i][j].VisualType = BT_FLAG;
				ms_FormatBoardData(i, j, CR_GRAY);
			}
		}
	}

	// ���ڼ� 0���� ����
	gb_UIMines = 0;
	MoveCur(71, 15); if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	printf("%3d", gb_UIMines); ChangeCR(CR_GRAY, CR_BLACK);
	
	// Ű �Է� ���
	while(1)
	{
		if(kbhit())
		{
			KeyValue = getch();
			
			if(KeyValue == KEY_ESC) break;
		}
	}
	
	ChangeCR(CR_GRAY, CR_BLACK);
	ms_Initialization();
}

int ms_OnMainMenu()
{	
	char KeyValue;
	int CurrentSelect = 1;

	while(1)
	{
		if(kbhit())
		{
			KeyValue = getch();
		
			if(KeyValue == KEY_ENTER || KeyValue == 'A' || KeyValue == 'a') break;
			else if(KeyValue == KEY_DOWN || KeyValue == KEY_RIGHT)
			{
				if(CurrentSelect < 4) CurrentSelect++;
				else CurrentSelect = 1;
			}
			else if(KeyValue == KEY_UP || KeyValue == KEY_LEFT)
			{
				if(CurrentSelect > 1) CurrentSelect--;
				else CurrentSelect = 4;
			}

			if(gb_Testmode) { MoveCur(0, 24); printf("CurrentSelect: %d\n", CurrentSelect); } // ONLY FOR TEST

			if(CurrentSelect == 1)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("��"); MoveCur(47, 10);  printf("��"); MoveCur(31, 12); printf("��"); MoveCur(47, 12); printf("��");
				MoveCur(31, 14); printf("��"); MoveCur(47, 14); printf("��"); MoveCur(31, 16); printf("��"); MoveCur(47, 16); printf("��");
				TypeBox("������ ���� �����մϴ�.", CR_WHITE, CR_BLACK);
			}
			
			else if(CurrentSelect == 2)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("��"); MoveCur(47, 10);  printf("��"); MoveCur(31, 12); printf("��"); MoveCur(47, 12); printf("��");
				MoveCur(31, 14); printf("��"); MoveCur(47, 14); printf("��"); MoveCur(31, 16); printf("��"); MoveCur(47, 16); printf("��");
				TypeBox("����� ������ �ҷ��ɴϴ�. (�������Դϴ�)", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 3)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("��"); MoveCur(47, 10);  printf("��"); MoveCur(31, 12); printf("��"); MoveCur(47, 12); printf("��");
				MoveCur(31, 14); printf("��"); MoveCur(47, 14); printf("��"); MoveCur(31, 16); printf("��"); MoveCur(47, 16); printf("��");
				TypeBox("���̵�, UI, ����, �Ҹ� ���� �����մϴ�.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 4)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("��"); MoveCur(47, 10);  printf("��"); MoveCur(31, 12); printf("��"); MoveCur(47, 12); printf("��");
				MoveCur(31, 14); printf("��"); MoveCur(47, 14); printf("��"); MoveCur(31, 16); printf("��"); MoveCur(47, 16); printf("��");
				TypeBox("������ �����մϴ�.", CR_WHITE, CR_BLACK);
			}

			ChangeCR(CR_GRAY, CR_BLACK);
		}
	}

	return CurrentSelect;
}

void ms_OnOptionMenu()
{	
	char KeyValue;
	int CurrentSelect = 1;
	
	while(1)
	{
		if(kbhit())
		{
			KeyValue = getch();

			if(KeyValue == KEY_ENTER || KeyValue == 'A' || KeyValue == 'a')
			{
				if(CurrentSelect == 1)
				{
					(gb_SelectLevel < 3) ? (gb_SelectLevel++) : (gb_SelectLevel = 1);
					
					if(gb_SelectLevel == 1)
					{
						MoveCur(22, 9); BCprintf("�ʱ�", CR_YELLOW, CR_BLACK); BCprintf(" (����: 9 / ����: 9 / ���� : 10)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 9; gb_SizeY = 9; gb_Mines = 10;
					}
					
					else if(gb_SelectLevel == 2)
					{
						MoveCur(22, 9); BCprintf("�߱�", CR_DGREEN, CR_BLACK); BCprintf(" (����: 16 / ����: 16 / ���� : 40)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 16; gb_SizeY = 16; gb_Mines = 40;
					}
					
					else if(gb_SelectLevel == 3)
					{
						MoveCur(22, 9); BCprintf("���", CR_RED, CR_BLACK); BCprintf(" (����: 30 / ����: 18 / ���� : 112)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 30; gb_SizeY = 18; gb_Mines = 112;
					}
				}

				else if(CurrentSelect == 2)
				{
					(gb_SelectUI < 3) ? (gb_SelectUI++) : (gb_SelectUI = 1);
					if(gb_SelectUI == 1) { MoveCur(22, 11); BCprintf("����", CR_WHITE, CR_BLACK); BCprintf(" (�ܡۨ��������٨ݢ�)          ", CR_DGRAY, CR_BLACK); }
					else if(gb_SelectUI == 2) { MoveCur(22, 11); BCprintf("�׸�", CR_WHITE, CR_BLACK); BCprintf(" (�����������٩ݢ�)          ", CR_DGRAY, CR_BLACK); }
					else if(gb_SelectUI == 3) { MoveCur(22, 11); BCprintf("ȥ��", CR_WHITE, CR_BLACK); BCprintf(" (�ߡ����̻�����ĥ�ȡ�??��)          ", CR_DGRAY, CR_BLACK); }
				}

				else if(CurrentSelect == 3)
				{
					gb_Colormode = !(gb_Colormode);
					if(gb_Colormode) { MoveCur(22, 13); Cprintf("���          ", CR_GOLD, CR_BLACK); }
					else { MoveCur(22, 13); printf("��� ����          "); }
					ms_DrawOptionMenu();
				}

				else if(CurrentSelect == 4)
				{
					gb_Soundmode = !(gb_Soundmode);
					if(gb_Soundmode) { MoveCur(22, 15); BCprintf("���          ", CR_DCYAN, CR_BLACK); BipSound(1000, 100, TRUE); }
					else { MoveCur(22, 15); BCprintf("��� ����         ", CR_DCYAN, CR_BLACK); }
				}

				else if(CurrentSelect == 5) { gb_GameState = GS_MAINMENU; break; }
			}

			else if(KeyValue == KEY_DOWN || KeyValue == KEY_RIGHT)
			{
				if(CurrentSelect < 5) CurrentSelect++;
				else CurrentSelect = 1;
			}

			else if(KeyValue == KEY_UP || KeyValue == KEY_LEFT)
			{
				if(CurrentSelect > 1) CurrentSelect--;
				else CurrentSelect = 5;
			}

			if(CurrentSelect == 1)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("��"); MoveCur(5, 11); printf("��"); MoveCur(5, 13); printf("��");
				MoveCur(5, 15); printf("��"); MoveCur(5, 17); printf("��");
				TypeBox("���� ���̵��� �����մϴ�. �ʱ�, �߱�, ��� ���̵��� �ֽ��ϴ�.", CR_WHITE, CR_BLACK);
			} 

			else if(CurrentSelect == 2)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("��"); MoveCur(5, 11); printf("��"); MoveCur(5, 13); printf("��");
				MoveCur(5, 15); printf("��"); MoveCur(5, 17); printf("��");
				TypeBox("�������� ������ �����մϴ�. �⺻, �׸�, ȥ�� ������ �ֽ��ϴ�.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 3)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("��"); MoveCur(5, 11); printf("��"); MoveCur(5, 13); printf("��");
				MoveCur(5, 15); printf("��"); MoveCur(5, 17); printf("��");
				TypeBox("���ӿ� ���� ��� ���θ� �����մϴ�.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 4)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("��"); MoveCur(5, 11); printf("��"); MoveCur(5, 13); printf("��");
				MoveCur(5, 15); printf("��"); MoveCur(5, 17); printf("��");
				TypeBox("�Ҹ� ��� ���θ� �����մϴ�.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 5)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("��"); MoveCur(5, 11); printf("��"); MoveCur(5, 13); printf("��");
				MoveCur(5, 15); printf("��"); MoveCur(5, 17); printf("��");
				TypeBox("������ �����ϰ� ���� �޴��� ���ư��ϴ�.", CR_WHITE, CR_BLACK);
			}

			ChangeCR(CR_GRAY, CR_BLACK);

			if(gb_Testmode) { MoveCur(0, 24); printf("CurrentSelect: %d\ngb_SelectLevel: %d\ngb_SelectUI: %d\ngb_Colormode: %d\ngb_Soundmode: %d\n", // ONLY FOR TEST
													  CurrentSelect, gb_SelectLevel, gb_SelectUI, gb_Colormode, gb_Soundmode); } // ONLY FOR TEST
		}
	}
}

int ms_OnGameBoard()
{
	char KeyValue;
	int CurrentX = 0, CurrentY = 0;
	int SavedX = 0, SavedY = 0; //���޴��������� �� CurrentX/Y�� �����ϱ� ���� ����
	boolean OnMenu = FALSE; // ���޴������������� �����ϴ� ����
	int MenuCounter = 0; // ���޴����� ����� ����Ű�� �ִ��� �����ϴ� ����
	int ClickReturnValue; // ���� ���ý� �ൿ�� �޾ƿ��� ���� (���ڿ���, �׳� �ٴ��̿��� ���)
	int RenewCounter = 0; // Ű���� �Է��� �� ���� �� �޴��� ���� �ӵ��� �����ϱ� ���� ���� (while���� ������ �ʹ� ���� ��Ʈ�� ���� �׾����� ������ ���)

	// Ÿ�̸� ���� ����
	time_t CurrentT; // �ð� ���� ���� (�ش�����: <time.h>)
	struct tm *Today;	// ����ü tm������
	int BeforeTime; // ���� �ʴ��� �ð� ������ ���� ����1
	int CurrentTime; // ���� �ʴ��� �ð� ������ ���� ����2

	time(&CurrentT); // CurrentTime ������ ���� �ð� ��������
	Today = localtime(&CurrentT); // Today ����ü�� CurrentTime�� �����͵��� ��������
	BeforeTime = Today->tm_hour*3600 + Today->tm_min*60 + Today->tm_sec; // ���� ������ �ð��� �ʷ� �����Ͽ� ����

	while(1)
	{
		RenewCounter++;

		time(&CurrentT);
		Today = localtime(&CurrentT);
		CurrentTime = Today->tm_hour*3600 + Today->tm_min*60 + Today->tm_sec;

		// Ű���尡 ������ ��
		if(kbhit())
		{
			KeyValue = getch();
			EraseBox();

			if(OnMenu) // Ű���尡 �޴��� �ֵ��϶�
			{
				if(KeyValue == KEY_DOWN || KeyValue == KEY_RIGHT)
				{
					MenuCounter = 0;
					if(CurrentY < 2) CurrentY++;
					else CurrentY = 0;
				}
				
				else if(KeyValue == KEY_UP || KeyValue == KEY_LEFT)
				{
					MenuCounter = 0;
					if(CurrentY > 0) CurrentY--;
					else CurrentY = 2;
				}
				
				else if(KeyValue == KEY_ESC)
				{
					MenuCounter = 0;
					OnMenu = !(OnMenu);
					CurrentX = SavedX;
					CurrentY = SavedY;
					MoveCur(68, 5); BCprintf("[�����ϱ�]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[�����ϱ�]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[���θ޴�]", CR_WHITE, CR_BLACK);
					MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE); MoveCur((CurrentX)*2+4, CurrentY+2);
				}
				
				else if(KeyValue == KEY_ENTER)
				{
					if(CurrentY == 0)
					{
						if(MenuCounter == 0)
						{
							MenuCounter++;
							TypeBox("������ ���� �ϰڽ��ϱ�? ���� �Ϸ��� �ѹ� �� ���͸� �����ʽÿ�.", CR_WHITE, CR_BLACK);
							BipSound(300, 500, gb_Soundmode);
						}
						
						else if (MenuCounter >= 1) { ms_Initialization(); return GS_RESTRAT; }
					}
					
					else if(CurrentY == 1)
					{
						TypeBox("���� �������� ���� ����Դϴ�.", CR_WHITE, CR_BLACK);
					}
					
					else if(CurrentY == 2)
					{
						if(MenuCounter == 0)
						{
							MenuCounter++;
							TypeBox("������ �����մϱ�? �����Ϸ��� �ѹ� �� ���͸� �����ʽÿ�.", CR_WHITE, CR_BLACK);
							BipSound(300, 500, gb_Soundmode);
						}
						
						else if(MenuCounter >= 1) { ms_Initialization(); return GS_MAINMENU; }
					}
				}

				if(CurrentY == 0 && OnMenu) // OnMenu ������ �ణ�� ���� �߻�
				{
					MoveCur(68, 5); BCprintf("[�����ϱ�]", CR_WHITE, CR_PURPLE); MoveCur(68, 7); BCprintf("[�����ϱ�]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[���θ޴�]", CR_WHITE, CR_BLACK);
					if(MenuCounter==0) TypeBox("������ ���� �����մϴ�. �������� ������ ��ϵ��� �ʽ��ϴ�.", CR_WHITE, CR_BLACK);
					MoveCur(69, 5); // ���� ��带 ���� �ǹ� �̵�
				}

				else if(CurrentY == 1 && OnMenu)
				{
					MoveCur(68, 5); BCprintf("[�����ϱ�]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[�����ϱ�]", CR_DGRAY, CR_PURPLE);
					MoveCur(68, 9); BCprintf("[���θ޴�]", CR_WHITE, CR_BLACK);
					TypeBox("�������� ������ �����մϴ�. (�������Դϴ�)", CR_WHITE, CR_BLACK);
					MoveCur(69, 7);
				}

				else if(CurrentY == 2 && OnMenu)
				{
					MoveCur(68, 5); BCprintf("[�����ϱ�]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[�����ϱ�]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[���θ޴�]", CR_WHITE, CR_PURPLE); MoveCur(69, 9);
					if(MenuCounter==0) TypeBox("���θ޴��� ���ư��ϴ�. �������� ������ ��ϵ��� �ʽ��ϴ�.", CR_WHITE, CR_BLACK);
					MoveCur(69, 9);
				}
			}
			
			else // Ű���尡 ������ �ֵ��϶�
			{
				if(KeyValue == KEY_LEFT)
				{
					if(CurrentX > 0)
					{
						CurrentX--;
						MoveCur((CurrentX+1)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX+1, CR_GRAY); // �̵��� ��ǥ�� ��� ����
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE); // �̵��� ��ǥ�� ����ġ ǥ��
						MoveCur((CurrentX)*2+4, CurrentY+2); // ���� ��带 ���� �ǹ� �̵�
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentX = gb_SizeX-1; ms_FormatBoardData(CurrentY, 0, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // ���� ��带 ���� �ǹ� �̵�
					}
				}

				else if(KeyValue == KEY_RIGHT)
				{
					if(CurrentX < gb_SizeX-1)
					{
						CurrentX++;
						MoveCur((CurrentX-1)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX-1, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // ���� ��带 ���� �ǹ� �̵�
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentX = 0; ms_FormatBoardData(CurrentY, gb_SizeX-1, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // ���� ��带 ���� �ǹ� �̵�
					}
				}

				else if(KeyValue == KEY_UP)
				{
					if(CurrentY > 0)
					{
						CurrentY--;
						MoveCur((CurrentX)*2+4, CurrentY+2+1); ms_FormatBoardData(CurrentY+1, CurrentX, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2);
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentY = gb_SizeY-1; ms_FormatBoardData(0, CurrentX, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // ���� ��带 ���� �ǹ� �̵�
					}
				}

				else if(KeyValue == KEY_DOWN)
				{
					if(CurrentY < gb_SizeY-1)
					{
						CurrentY++;
						MoveCur((CurrentX)*2+4, CurrentY+2-1); ms_FormatBoardData(CurrentY-1, CurrentX, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2);
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentY = 0; ms_FormatBoardData(gb_SizeY-1, CurrentX, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // ���� ��带 ���� �ǹ� �̵�
					}
				}

				else if(KeyValue == KEY_ESC)
				{
					OnMenu = !(OnMenu);
					SavedX = CurrentX;
					SavedY = CurrentY;
					MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_GRAY);
					CurrentY = 0;
					TypeBox("�޴��� �̵��մϴ�. �������� ���ư����� [ESC]Ű�� �����ʽÿ�.", CR_WHITE, CR_BLACK);
					MoveCur(68, 5); BCprintf("[�����ϱ�]", CR_WHITE, CR_PURPLE); MoveCur(69, 5); // ���� ��带 ���� �ǹ� �̵�
					BipSound(200, 100, gb_Soundmode);
				}

				else if(KeyValue == 'A' || KeyValue == 'a') // ���� Ŭ��
				{
					ClickReturnValue = ms_LeftClick(CurrentX, CurrentY, FALSE);

					if(ClickReturnValue != GS_LOSE) ClickReturnValue = ms_CheckWin();
				}

				else if(KeyValue == 'S' || KeyValue == 's') // ���� Ŭ��
				{
					ClickReturnValue = ms_DualClick(CurrentX, CurrentY);

					if(ClickReturnValue != GS_LOSE) ClickReturnValue = ms_CheckWin();
				}

				else if(KeyValue == 'D' || KeyValue == 'd') // ������ Ŭ��
				{
					ms_RightClick(CurrentX, CurrentY);
				}
			}
		}

		// Ű���尡 �� ������ ��
		else
		{
			if(RenewCounter >= gb_UIrenewtimer) // Ÿ�̸� �ʰ��ϸ�
			{
				RenewCounter = 0;

				// �����ڼ����������̽�
				MoveCur(71, 15); if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				printf("%3d", gb_UIMines); ChangeCR(CR_GRAY, CR_BLACK);

				// ��Ÿ�̸ӡ��������̽�
				if(CurrentTime > BeforeTime) // 1�� �̻� ����Ͽ�����
				{
					gb_Time += (CurrentTime - BeforeTime); // �ð� ����ŭ ������Ŵ
					BeforeTime = CurrentTime; // �ʼ�
					if(gb_Time > 359999) gb_Time = 359999; // 99�ð� 59�� 59�� �ʰ��� 99�ð� 59�� 59�ʷ� ����
					if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
					MoveCur(68, 19); printf("[%.2d:%.2d:%.2d]", (gb_Time/3600), ((gb_Time-(gb_Time/3600))/60), (gb_Time%60)); // �������̽��� ���
					MoveCur((CurrentX)*2+4, CurrentY+2);
				}

				else // 23�� -> 0�÷� �Ѿ�� �κ��� ��� ��... (�� 1�� ������ ������ ���� �� �ִ�)
				{
					BeforeTime = CurrentTime;
				}
				
			}
		}

		// ������ ���¸� ��ȯ (��� ���� or �й� or �¸�)
		if(ClickReturnValue == GS_LOSE) return GS_LOSE; // ���� ��Ƽ� GS_LOSE ���Ͻ� �ݺ��� Ż��
		else if(ClickReturnValue == GS_WIN) return GS_WIN; // �¸����� ������ GS_WIN ȣ��� �ݺ��� Ż��
	}
	
	return GS_STARTGAME; // ���⿡ GS_LOSE, GS_WIN ȣ�� ����
}

int ms_LeftClick(int x, int y, boolean Recursive)
{
	int LeftClickResult = GS_STARTGAME;

	// ��ǥ�� ���带 ����� �ʴ� �ѵ� ���̸�
	if(x >= 0 && y >= 0 && x < gb_SizeX && y < gb_SizeY)
	{
		// ������ ���� ���õ��� �ʰų� ?��ũ�� ���
		if(gb_MainBoard[y][x].VisualType == BT_HIDE || gb_MainBoard[y][x].VisualType == BT_QUESTION)
		{
			// ù Ŭ���̸� ���� ����
			if(gb_ClickCount == 0)
				ms_CreateMine(x, y);

			gb_ClickCount++;
			
			// Ŭ���� ���� ���� ������ ���� ����
			gb_MainBoard[y][x].VisualType = BT_UNCOVER;
			MoveCur((x)*2+4, y+2);
			if(Recursive) ms_FormatBoardData(y, x, CR_GRAY);
			else ms_FormatBoardData(y, x, CR_WHITE);
			MoveCur((x)*2+4, y+2);
			
			// Ŭ���� ���� 0�� ��� ����Լ�
			if(gb_MainBoard[y][x].BoardData == 0)
			{
					ms_LeftClick(x-1, y-1, TRUE); ms_LeftClick(x, y-1, TRUE); ms_LeftClick(x+1, y-1, TRUE); ms_LeftClick(x-1, y, TRUE);
					ms_LeftClick(x+1, y, TRUE); ms_LeftClick(x-1, y+1, TRUE); ms_LeftClick(x, y+1, TRUE); ms_LeftClick(x+1, y+1, TRUE);
			}

			// Ŭ���� ���� ������ ��� ���� ����
			else if(gb_MainBoard[y][x].BoardData == 9)
			{
				LeftClickResult = GS_LOSE;
			}
		}
	}

	return LeftClickResult; // ���⿡ GS_LOSE ���� ���� ����
}

void ms_RightClick(int x, int y)
{
	if(gb_MainBoard[y][x].VisualType != BT_UNCOVER)	
	{
		if(gb_MainBoard[y][x].VisualType == BT_HIDE) { gb_MainBoard[y][x].VisualType = BT_FLAG; gb_UIMines--; }
		else if(gb_MainBoard[y][x].VisualType == BT_FLAG) { gb_MainBoard[y][x].VisualType = BT_QUESTION; gb_UIMines++;}
		else if(gb_MainBoard[y][x].VisualType == BT_QUESTION) gb_MainBoard[y][x].VisualType = BT_HIDE;

		MoveCur((x)*2+4, y+2); ms_FormatBoardData(y, x, CR_WHITE); MoveCur((x)*2+4, y+2);
	}	
}

int ms_DualClick(int x, int y)
{
	int i, j, k, l;
	int FlagCounter = 0;
	int DualClickResult = GS_STARTGAME;
	boolean ReturnLose = FALSE;

	if(gb_MainBoard[y][x].VisualType == BT_UNCOVER && gb_MainBoard[y][x].BoardData != 0)
	{
		// �ֺ� ����� ���� ī��Ʈ
		k = y+1; l = x+1;

		for(i=y-1; i<=k; i++)
		{
			if(i<0) i=0; if(k>=gb_SizeY) k = y;
			for(j=x-1; j<=l; j++)
			{
				if(j<0) j=0; if(l>=gb_SizeX) l = x;
				if(gb_MainBoard[i][j].VisualType == BT_FLAG)
					FlagCounter++;
			}
		}

		if(gb_Testmode) { TypeBox("", CR_WHITE, CR_BLACK); printf("%d���� ��� ������.", FlagCounter); } // ONLY FOR TEST

		// �ֺ� ����� ���� ���õ� ��ǥ�� ��ȣ�� ������ �ֺ��� BT_HIDE �� ���� Ŭ��
		if(gb_MainBoard[y][x].BoardData == FlagCounter)
		{
			k = y+1; l = x+1;
			if(gb_Testmode) { TypeBox("", CR_PURPLE, CR_WHITE); printf("x:%d y:%d k:%d l:%d", x, y, k, l); } // ONLY FOR TEST
			for(i=y-1; i<=k; i++)
			{
				if(i<0) i=0; if(k>=gb_SizeY) k = y;
				for(j=x-1; j<=l; j++)
				{
					if(j<0) j=0; if(l>=gb_SizeX) l = x;
					if(gb_MainBoard[i][j].VisualType == BT_HIDE || gb_MainBoard[i][j].VisualType == BT_QUESTION)
						DualClickResult = ms_LeftClick(j, i, TRUE);

					if(DualClickResult == GS_LOSE) // �ѹ��̶� GS_LOSE���� �������� ���Ŭ���� ���� ���ϰ��� GS_LOSE
						ReturnLose = TRUE;
					
					// MoveCur((j)*2+4, i+2); ms_FormatBoardData(j, i, CR_GOLD); MoveCur((j)*2+4, y+i); // ONLY FOR TEST
				}
			}
		}

		else { TypeBox("�ֺ� ����� ���� �ش� ��ġ�� ���� ���� �ʽ��ϴ�.", CR_WHITE, CR_BLACK); BipSound(100, 100, gb_Soundmode); }
	}

	if(ReturnLose) return GS_LOSE;
	else return DualClickResult; // ���⿡ GS_LOSE ���� ���� ����
}

void ms_CreateMine(int x, int y)
{
	int i, j, k;
	int MineLeft = gb_Mines;

	// �� ũ�Ⱑ ���ڼ����� ������ ���ڼ� ������
	if((gb_SizeX * gb_SizeY) < gb_Mines)
		gb_Mines = (gb_SizeX * gb_SizeY) - 1;
	
	// ���� �ɱ�
	while(MineLeft != 0)
	{
		j = Random_Int(0, gb_SizeX-1);
		k = Random_Int(0, gb_SizeY-1);
		
		if((gb_MainBoard[k][j].BoardData == 0) && (j != x || k != y)) // �� �� ���� �����̰� ù���� ������ �ƴϸ�
		{
			gb_MainBoard[k][j].BoardData = 9; // ���� ����
			MineLeft--;
		}
	}

	//gb_MainBoard[0][0].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[0][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][0].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[0][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[(gb_SizeY-1)/2][0].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[(gb_SizeY-1)/2][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[(gb_SizeY-1)/2][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST


	// ���� �ֺ� ���� ����
	for(i=0; i<=gb_SizeY-1; i++)
	{
		for(j=0; j<=gb_SizeX-1; j++)
		{
			if(gb_MainBoard[i][j].BoardData == 9) // ���� �߰��ϸ�
			{
				if(i==0 && j==0) { ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // ���� ����̸�
				else if(i==0 && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); } // ���� ����̸�
				else if(i==gb_SizeY-1 && j==0) { ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j+1, i); } // ���� �ϴ��̸�
				else if(i==gb_SizeY-1 && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j-1, i); } // ���� �ϴ��̸�
				else if(i==0 && (j>0 && j<gb_SizeX-1)) { ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j-1, i+1);
														 ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // �𼭸� ���� ���
				else if((i>0 && i<gb_SizeY-1) && j==0) { ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j+1, i);
														 ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // �𼭸� ���� ����
				else if((i>0 && i<gb_SizeY-1) && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j-1, i);
																  ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); } // �𼭸� ���� ����
				else if(i==gb_SizeY-1 && (j>0 && j<gb_SizeX-1)) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1);
																  ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j+1, i); } // �𼭸� ���� �ϴ�
				else { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j-1, i);
					   ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // �׿� �߾�
			}
		}
	}

	// ms_DrawGameBoard(); ONLY FOR TEST
}

void ms_PlusBoardNumber(int x, int y)
{
	if(gb_MainBoard[y][x].BoardData < 8)
		gb_MainBoard[y][x].BoardData++;
}

int ms_CheckWin() // �¸� ���� �Ǵ� �Լ�
{
	int i, j;
	int BlockCounter = 0;

	// ��� ���带 �˻��Ͽ� ���ڰ� �ƴ� �κ��� �������°��� Ȯ��
	for(i=0; i<=gb_SizeY-1; i++)
	{
		for(j=0; j<=gb_SizeX-1; j++)
		{
			if(gb_MainBoard[i][j].BoardData != 9 && gb_MainBoard[i][j].VisualType == BT_UNCOVER)
				BlockCounter++;
		}
	}

	if((gb_SizeX*gb_SizeY)-gb_Mines == BlockCounter) return GS_WIN;
	else return GS_STARTGAME;
}


// ���� �ڽ� ������ �ִ� 76byte���� �ؽ�Ʈ�� ������ִ� �Լ�
void TypeBox(char String[76], int ColorFont, int ColorBack)
{
	EraseBox();

	MoveCur(2, 22);	
	BCprintf(String, ColorFont, ColorBack);
}

// ���� �ڽ� ������ �������� ����� �Լ�
void EraseBox()
{
	MoveCur(2, 22);
	printf("                                                                            ");
}

// ���� ������ �޾� ���ڸ� ����ϴ� �Լ�. ���� ����� ���ҽ� �⺻ �ܼ� ������ ���
void BCprintf(char *String, int FontCR, int BackCR)
{
	if(gb_Colormode)
		Cprintf(String, FontCR, BackCR);
	
	else
		printf("%s", String);
}