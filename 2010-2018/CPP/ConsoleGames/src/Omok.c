// 2010305042 �����

#include <stdio.h>
#include <windows.h>
#include <conio.h>

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

int gv_StoneArr[50][50];		// �������� �����͸� �����ϴ� 2���� �迭
int gv_TurnChk;					// �÷��� ������ �����ϴ� ������ ��������
BOOL gv_BlackWin;				// �浹 �¸��� üũ�ϴ� BOOL����
BOOL gv_WhiteWin;				// �鵹 �¸��� üũ�ϴ� BOOL����
BOOL gv_BlackChk;				// �浹�� ����(��ǥ ����, �̹� ���� ���� �ڸ�, �ֻ�)�� üũ
BOOL gv_WhiteChk;				// �鵹�� ����(��ǥ ����, �̹� ���� ���� �ڸ�, �ֻ�)�� üũ

//
extern void Cprintf(char *String, int FontCR, int BackCR);
extern void ChangeCR(int FontCR, int BackCR);
extern void ConClr();
extern void MoveCur(int x, int y);
extern int Random_Int(int Min, int Max);
extern void BipSound(int Pitch, int Duration, boolean UseSound);
extern void SetCurType(int CurType);

//
void om_Run();					// ������ �����ϴ� �Լ�
void om_Initialize();			// ���� �����͸� �ʱ�ȭ�ϴ� �Լ�
void om_Beep();					// ���� ����/����� ���� ���
void om_DrawPlate();			// �ٵ����� ����ϴ� �Լ�
void om_Turn();					// �÷��� ������ �����ϴ� �Լ�
void om_Insert(int *i,int *j);	// ���� ��ġ�� �Է¹޴� �Լ�
void om_BlackStone();			// �浹�� ���� �˻�(�¸�, ����, �ֻ�, ����)�� �����ϴ� �Լ�
void om_WhiteStone();			// �鵹�� ���� �˻�(�¸�, ����, �ֻ�, ����)�� �����ϴ� �Լ�
void om_ErrorChk(int i,int j);	// �� ��ġ�� ���� ������ �˻��ϴ� �Լ�
void om_SamSamChk(int i,int j);	// �ֻ� �ݼ��� �˻��ϴ� �Լ�
void om_WinCheck(int i,int j);	// �¸� ������ �˻��ϴ� �Լ�
void om_SixStnChk(int i,int j);	// ���� �ݼ��� �˻��ϴ� �Լ�
void om_Gameover();				// ���� ����� �¸��ڸ� ����ϴ� �Լ�

void om_main()
{
	system("mode con:lines=35");
	system("mode con:cols=61");	    // �ܼ�â�� ����/���� ������ ����
	system("title ConsoleGames - Omok"); // �ܼ� ���� ����
	SetCurType(CT_HIDE);			// Ŀ�� ����
	om_Run();						// ���� ����
}

void om_Run()						// ������ �����ϴ� �Լ�
{
	om_Initialize();				// ���� ���� �� ������ �ʱ�ȭ
	om_Beep();						// ���� ���� ���� ȣ��
	while (gv_BlackWin==FALSE && gv_WhiteWin==FALSE) // �¸��Ҷ����� �ݺ�
	{
		printf("\n");
		om_DrawPlate();				// ������ �׸��� �Լ� ȣ��
		ConClr();				// ȭ�� �����
	}	
	om_Gameover();					// �¸��� �÷��̾� ���
}

void om_Initialize()				// ���� �����͸� �ʱ�ȭ�ϴ� �Լ�
{
	int i,j;
	gv_TurnChk=1;
	gv_BlackWin=FALSE;
	gv_WhiteWin=FALSE;
	gv_BlackChk=TRUE;
	gv_WhiteChk=TRUE;
	for (i=0;i<50;i++)
	{
		for (j=0;j<50;j++)
			gv_StoneArr[i][j]=0;
	}								// ���� ���������� �迭 ������ �ʱ�ȭ
}

void om_Beep()						// ���� ����/����� ���� ���
{
	if (gv_BlackWin==TRUE || gv_WhiteWin==TRUE)	// ���� ����� ����
	{
		Beep(392,250);
		Beep(392,250);
		Beep(392,250);
		Beep(524,1000);
	}
	else										// ���� ����
	{
		Beep(150,50);//do
		Beep(200,50);
		Beep(250,50);//re
		Beep(300,50);
		Beep(350,50);//me
		Beep(400,50);//fa
		Beep(450,50);
		Beep(500,50);//sol
		Beep(550,50);
		Beep(600,50);//la
		Beep(650,50);
		Beep(700,50);//ti
		Beep(750,50);//do
	}
}

void om_DrawPlate()					// �ٵ����� ����ϴ� �Լ�
{
	int i,j;
	printf("\n      ");
	for (i=0;i<25;i++)
		printf("%2d",i+1);			// �ٵ��� ��� x�� ��ȣ�� ���
	printf("\n    ");	
	for (i=0;i<25;i++)
	{
		if (i<9)
			printf(" %d",i+1);
		else
			printf("%d",i+1);		// �ٵ��� ���� y�� ��ȣ�� ���

		for (j=0;j<25;j++)
		{
			if (gv_StoneArr[i+5][j+5]==0)
			{
				if (i==0 && j==0)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else if (i==0 && j!=24)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else if (i==0 && j==24)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else if (i!=24 && j==0 && j!=24)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else if (i!=24 && j==24)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else if (i==24 && j==0)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else if (i==24 && j==24)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else if (i==24 && j!=0)
					Cprintf("��",CR_BLACK,CR_GOLD);
				else
					Cprintf("��",CR_BLACK,CR_GOLD);
			}						// �ٵ��� �𼭸� ó��
				
			else if (gv_StoneArr[i+5][j+5]==1)
				Cprintf("��",CR_BLACK,CR_GOLD);	
			else if (gv_StoneArr[i+5][j+5]==2)
				Cprintf("��",CR_WHITE,CR_GOLD);
		}							// �迭 �����Ϳ� ���� �ٵ��ǰ� �ٵϵ� ���
		printf("\n    ");
	}	
	printf("\n");
	if (gv_WhiteWin==FALSE && gv_BlackWin==FALSE)	// �¸��ڰ� ���� ��� ���� ������ �̵�
		om_Turn();
}

void om_Turn()						// �÷��� ������ �����ϴ� �Լ�
{
	if (gv_TurnChk==1)
		om_BlackStone();
	else if (gv_TurnChk==2)
		om_WhiteStone();
}									// ���� ���� ���������� ���� �Լ��� ȣ��

void om_Insert(int *i,int *j)		// ���� ��ġ�� �Է¹޴� �Լ�
{
	int ti,tj;
	if (gv_TurnChk==1)
	{
		MoveCur(0,29);
		printf("������������������������������������������������������������\n");
		printf("�� ������ �����Դϴ�. X��ǥ�� �Է��� �ּ��� :             ��\n");
		printf("������������������������������������������������������������\n");
		MoveCur(47,30);
		scanf("%d",&ti);		
		MoveCur(0,29);
		printf("������������������������������������������������������������\n");
		printf("�� ������ �����Դϴ�. Y��ǥ�� �Է��� �ּ��� :             ��\n");
		printf("������������������������������������������������������������\n");
		MoveCur(47,30);
		scanf("%d",&tj);
	}

	if (gv_TurnChk==2)
	{
		MoveCur(0,29);
		printf("������������������������������������������������������������\n");
		printf("�� �� �����Դϴ�. X��ǥ�� �Է��� �ּ��� :               ��\n");
		printf("������������������������������������������������������������\n");
		MoveCur(47,30);
		scanf("%d",&ti);		
		MoveCur(0,29);
		printf("������������������������������������������������������������\n");
		printf("�� �� �����Դϴ�. Y��ǥ�� �Է��� �ּ��� :               ��\n");
		printf("������������������������������������������������������������\n");
		MoveCur(47,30);
		scanf("%d",&tj);
	}								// om_Insert() �Լ� �� ���������� ���� ��ǥ���� �Է¹޴´�
	*i=ti;
	*j=tj;							// �� �Ŀ� ���������� ��ǥ���� �����ͷ� �̵���Ų��
}

void om_BlackStone()				// �浹�� ���� �˻�(�¸�, ����, �ֻ�, ����)�� �����ϴ� �Լ�
{
	int i,j;	
	while (gv_BlackChk==TRUE)		// ������ ���������� �ݺ�
	{
		om_Insert(&i,&j);			// ��ǥ �Է� �Լ��� �ּҰ��� �Ѱ��ش�
		i--;						// �迭�� 0���� ���������� ����ڴ� �ּ� 1�� �Է��ϱ� ������
		j--;						// ���� ���� �� 1�� ����		
		om_ErrorChk(j+5,i+5);		// ���� Ư���� ���� �ٵ����� (5,5)���� �����Ѵ�
	}
	gv_StoneArr[j+5][i+5]=1;		// ���� �迭 ������ �浹��(1)�� �Է�
	gv_TurnChk=2;					// ������ �鵹 ������ �ٲ��ش�
	gv_BlackChk=TRUE;				// ���� ������ ���� BOOL���� ���� �ʱ�ȭ�Ѵ�
	om_WinCheck(j+5,i+5);			// ��ǥ ���ؿ��� �¸��ߴ��� �˻��ϴ� �Լ��� ȣ���Ѵ�
}

void om_WhiteStone()				// �鵹�� ���� �˻�(�¸�, ����, �ֻ�, ����)�� �����ϴ� �Լ�
{
	int i,j;	
	while (gv_WhiteChk==TRUE)		// ������ ���������� �ݺ�
	{
		om_Insert(&i,&j);			// ��ǥ �Է� �Լ��� �ּҰ��� �Ѱ��ش�
		i--;						// �迭�� 0���� ���������� ����ڴ� �ּ� 1�� �Է��ϱ� ������
		j--;						// ���� ���� �� 1�� ����		
		om_ErrorChk(j+5,i+5);		// ���� Ư���� ���� �ٵ����� (5,5)���� �����Ѵ�
	}	
	gv_StoneArr[j+5][i+5]=2;		// ���� �迭 ������ �浹��(1)�� �Է�
	gv_TurnChk=1;					// ������ �鵹 ������ �ٲ��ش�
	gv_WhiteChk=TRUE;				// ���� ������ ���� BOOL���� ���� �ʱ�ȭ�Ѵ�
	om_WinCheck(j+5,i+5);			// ��ǥ ���ؿ��� �¸��ߴ��� �˻��ϴ� �Լ��� ȣ���Ѵ�
}

void om_ErrorChk(int i,int j)		// �� ��ġ�� ���� ������ �˻��ϴ� �Լ�
{
	MoveCur(4,32);
	if (gv_TurnChk==1)				// �浹 ������ ���
	{
		if (gv_StoneArr[i][j]!=0)	// �������� ��ǥ�� ���� 0�� �ƴ� ��� �ٸ� ���� ��������
		{
			gv_BlackChk=TRUE;		// while���� �۵��� �� �ֵ��� BOOL������ �ʱ�ȭ
			Cprintf("�� �ڸ��� �̹� ���� �ֽ��ϴ�. �ٸ� ��ǥ�� �Է��ϼ���.\n",CR_RED,CR_GREEN);	// �޽��� ���
		}		
		else if(i>29 || j>29 || i<5 || j<5)	// 25x25 ���� ���� ��ǥ�� �Է����� ���
		{
			gv_BlackChk=TRUE;			
			Cprintf("��ǥ�� ������ ������ϴ�. �ٸ� ��ǥ�� �Է��ϼ���.\n",CR_RED,CR_GREEN);	// �޽��� ���
		}		
		else
			gv_BlackChk=FALSE;		// ������ ���� ��� while���� ���Ḧ ���� ������ ����
	}
	else if (gv_TurnChk==2)				// �鵹 ������ ���
	{
		if (gv_StoneArr[i][j]!=0)		// �������� ��ǥ�� ���� 0�� �ƴ� ��� �ٸ� ���� ��������
		{
			gv_WhiteChk=TRUE;			// while���� �۵��� �� �ֵ��� BOOL������ �ʱ�ȭ					
			Cprintf("�� �ڸ��� �̹� ���� �ֽ��ϴ�. �ٸ� ��ǥ�� �Է��ϼ���.\n",CR_RED,CR_GREEN);	// �޽��� ���
		}		
		else if(i>29 || j>29 || i<5 || j<5)	// 25x25 ���� ���� ��ǥ�� �Է����� ���
		{
			gv_WhiteChk=TRUE;			
			Cprintf("��ǥ�� ������ ������ϴ�. �ٸ� ��ǥ�� �Է��ϼ���.\n",CR_RED,CR_GREEN);	// �޽��� ���
		}		
		else
			gv_WhiteChk=FALSE;		// ������ ���� ��� while���� ���Ḧ ���� ������ ����
	}
	om_SamSamChk(i,j);				// 3x3�� ��� �ƿ� ���� ���� �� ���� ������ om_ErrorChk()���� �˻��� ��,
}									// ������ üũ�ϰ� ���� ���� ���� �����ؾ� ��

void om_SamSamChk(int i,int j)		// 3x3 �ݼ��� �˻��ϴ� �Լ�
{	// 3x3 �˻� ���� ����
	int temp;
	MoveCur(4,32);
	if (gv_TurnChk==1)
	{
		temp=1;
		if ((temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i-2][j+2])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i+1][j+1] && temp==gv_StoneArr[i+2][j+2])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i+1][j-1] && temp==gv_StoneArr[i+2][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i-2][j-2])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2] && temp==gv_StoneArr[i+1][j-1] && temp==gv_StoneArr[i+2][j-2])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i-2][j-2])
			|| (temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2] && temp==gv_StoneArr[i+1][j+1] && temp==gv_StoneArr[i+2][j+2])
			|| (temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i-2][j+2])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i+1][j+1])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2] && temp==gv_StoneArr[i+1][j-1] && temp==gv_StoneArr[i-1][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i+1][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i+1][j-1])
			|| (temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i+1][j+1] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i+1][j-1])
			)	// 3x3 �˻� ���� ��ħ. �� 22���� ���ǹ�
		{
			gv_BlackChk=TRUE;			// while���� �۵��� ���� BOOL���� �ʱ�ȭ
			Cprintf("�ֻ�!  �ٸ� ��ǥ�� �Է��ϼ���.\n\a\a",CR_RED,CR_GREEN);	// �޽��� ���
			Sleep(1000);				// �޽����� ����� �� 1000ms(1sec) ���� ������
		}
	}

	else if (gv_TurnChk==2)
	{
		temp=2;
		if ((temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i-2][j+2])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i+1][j+1] && temp==gv_StoneArr[i+2][j+2])
			|| (temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i+2][j] && temp==gv_StoneArr[i+1][j-1] && temp==gv_StoneArr[i+2][j-2])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i-2][j] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i-2][j-2])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2] && temp==gv_StoneArr[i+1][j-1] && temp==gv_StoneArr[i+2][j-2])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i-2][j-2])
			|| (temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2] && temp==gv_StoneArr[i+1][j+1] && temp==gv_StoneArr[i+2][j+2])
			|| (temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i][j+2] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i-2][j+2])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j+1] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i+1][j+1])
			|| (temp==gv_StoneArr[i][j-1] && temp==gv_StoneArr[i][j-2] && temp==gv_StoneArr[i+1][j-1] && temp==gv_StoneArr[i-1][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i+1][j+1])
			|| (temp==gv_StoneArr[i-1][j] && temp==gv_StoneArr[i+1][j] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i+1][j-1])
			|| (temp==gv_StoneArr[i-1][j-1] && temp==gv_StoneArr[i+1][j+1] && temp==gv_StoneArr[i-1][j+1] && temp==gv_StoneArr[i+1][j-1])
			)	// 3x3 �˻� ���� ��ħ. �� 22���� ���ǹ�
		{
			gv_WhiteChk=TRUE;			// while���� �۵��� ���� BOOL���� �ʱ�ȭ
			Cprintf("�ֻ�! �ٸ� ��ǥ�� �Է��ϼ���.\n\a\a",CR_RED,CR_GREEN);	// �޽��� ���
			Sleep(1000);				// �޽����� ����� �� 1000ms(1sec) ���� ������
		}
	}
}

void om_WinCheck(int i,int j)			// �¸� ������ �˻��ϴ� �Լ�
{
	if ((gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i][j+4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j+4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i][j-4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j-4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j-4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j+4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i][j-1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i][j+1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1]))
	{
		if (gv_StoneArr[i][j]==1)			// �¸� ������ �����ϸ� ��� �� ���� �浹�� ��쿡
			gv_BlackWin=TRUE;				// �浹 �¸��� �÷�����
		else if (gv_StoneArr[i][j]==2)		// �¸� ������ �����ϸ� ��� �� ���� �鵹�� ��쿡
			gv_WhiteWin=TRUE;				// �鵹 �¸��� �÷�����
	}
	om_SixStnChk(i,j);						// ������ ��쿡�� �¸����� �ʱ� ������ ���� �˻� �Լ� ȣ��
}

void om_SixStnChk(int i,int j)				// ���� �ݼ��� �˻��ϴ� �Լ�
{
	if ((gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j] && gv_StoneArr[i][j]==gv_StoneArr[i+5][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i][j+4] && gv_StoneArr[i][j]==gv_StoneArr[i][j+5])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j+4] && gv_StoneArr[i][j]==gv_StoneArr[i+5][j+5])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j] && gv_StoneArr[i][j]==gv_StoneArr[i-5][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i][j-4] && gv_StoneArr[i][j]==gv_StoneArr[i][j-5])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j-4] && gv_StoneArr[i][j]==gv_StoneArr[i-5][j-5])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j-4] && gv_StoneArr[i][j]==gv_StoneArr[i+5][j-5])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j+4] && gv_StoneArr[i][j]==gv_StoneArr[i-5][j+5])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j+4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j-4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j-3] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+4][j-4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j+3] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-4][j+4])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j+3])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j+3])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i][j-3])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j-3])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i+3][j-3])
		|| (gv_StoneArr[i][j]==gv_StoneArr[i-1][j+1] && gv_StoneArr[i][j]==gv_StoneArr[i-2][j+2] && gv_StoneArr[i][j]==gv_StoneArr[i+2][j-2] && gv_StoneArr[i][j]==gv_StoneArr[i+1][j-1] && gv_StoneArr[i][j]==gv_StoneArr[i-3][j+3]))
	{
		if (gv_BlackWin==TRUE)
		{
			gv_BlackWin=FALSE;				// �浹�� ������ ��� ���� �¸� �÷��׸� ���
			Cprintf("����! �¸����� ���߽��ϴ�.\n\a",CR_RED,CR_GREEN);	// �޽��� ���
			Sleep(1000);					// �޽����� ����� �� 1�ʰ� ����
		}
		else if (gv_WhiteWin==TRUE)
		{
			gv_WhiteWin=FALSE;				// �鵹�� ������ ��� ���� �¸� �÷��׸� ���
			Cprintf("����! �¸����� ���߽��ϴ�.\n\a",CR_RED,CR_GREEN);	// �޽��� ���
			Sleep(1000);					// �޽����� ����� �� 1�ʰ� ����
		}
	}
}

void om_Gameover()							// ���� ����� �¸��ڸ� ����ϴ� �Լ�
{
	char retry;
	om_DrawPlate();	// ������ ���� �ٵ����� ���
	MoveCur(17,8);
	if (gv_WhiteWin==TRUE)					// �¸��ڰ� �鵹�� ��� �޽��� ���
		Cprintf("��� ���� �¸��Ͽ����ϴ�!",CR_WHITE,CR_BLACK);	
	else if (gv_BlackWin==TRUE)				// �¸��ڰ� �浹�� ��� �޽��� ���
		Cprintf("������ ���� �¸��Ͽ����ϴ�!",CR_WHITE,CR_BLACK);
	om_Beep();								// �¸� ���� ȣ��
	fflush(stdin);
	MoveCur(16,30);
	Cprintf("�ٽ� ���� �Ͻðڽ��ϱ�?(y/n) : ",CR_WHITE,CR_BLACK);
	retry=getch();							// ���۸� ������� �ʰ� �� ���ڸ� �Է�
	if (retry=='y')							// ���� y�� �Էµ� ��� ������ �����
	{
		ConClr();
		om_Run();
	}										// y �̿��� ���� �Է��� ��� ���� ����
	else
		ConClr();
}
