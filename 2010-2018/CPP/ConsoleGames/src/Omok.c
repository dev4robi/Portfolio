// 2010305042 윤상수

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

int gv_StoneArr[50][50];		// 오목판의 데이터를 관리하는 2차원 배열
int gv_TurnChk;					// 플레이 순서를 관리하는 정수형 전역변수
BOOL gv_BlackWin;				// 흑돌 승리를 체크하는 BOOL변수
BOOL gv_WhiteWin;				// 백돌 승리를 체크하는 BOOL변수
BOOL gv_BlackChk;				// 흑돌의 오류(좌표 범위, 이미 돌이 놓인 자리, 쌍삼)를 체크
BOOL gv_WhiteChk;				// 백돌의 오류(좌표 범위, 이미 돌이 놓인 자리, 쌍삼)를 체크

//
extern void Cprintf(char *String, int FontCR, int BackCR);
extern void ChangeCR(int FontCR, int BackCR);
extern void ConClr();
extern void MoveCur(int x, int y);
extern int Random_Int(int Min, int Max);
extern void BipSound(int Pitch, int Duration, boolean UseSound);
extern void SetCurType(int CurType);

//
void om_Run();					// 게임을 시작하는 함수
void om_Initialize();			// 게임 데이터를 초기화하는 함수
void om_Beep();					// 게임 시작/종료시 사운드 출력
void om_DrawPlate();			// 바둑판을 출력하는 함수
void om_Turn();					// 플레이 순서를 관리하는 함수
void om_Insert(int *i,int *j);	// 돌의 위치를 입력받는 함수
void om_BlackStone();			// 흑돌의 각종 검사(승리, 오류, 쌍삼, 육목)를 수행하는 함수
void om_WhiteStone();			// 백돌의 각종 검사(승리, 오류, 쌍삼, 육목)를 수행하는 함수
void om_ErrorChk(int i,int j);	// 돌 위치에 따른 오류를 검사하는 함수
void om_SamSamChk(int i,int j);	// 쌍삼 금수를 검사하는 함수
void om_WinCheck(int i,int j);	// 승리 조건을 검사하는 함수
void om_SixStnChk(int i,int j);	// 육목 금수를 검사하는 함수
void om_Gameover();				// 게임 종료시 승리자를 출력하는 함수

void om_main()
{
	system("mode con:lines=35");
	system("mode con:cols=61");	    // 콘솔창의 가로/세로 사이즈 조정
	system("title ConsoleGames - Omok"); // 콘솔 제목 변경
	SetCurType(CT_HIDE);			// 커서 숨김
	om_Run();						// 오목 실행
}

void om_Run()						// 게임을 시작하는 함수
{
	om_Initialize();				// 게임 시작 전 데이터 초기화
	om_Beep();						// 게임 시작 사운드 호출
	while (gv_BlackWin==FALSE && gv_WhiteWin==FALSE) // 승리할때까지 반복
	{
		printf("\n");
		om_DrawPlate();				// 오목판 그리는 함수 호출
		ConClr();				// 화면 지우기
	}	
	om_Gameover();					// 승리한 플레이어 출력
}

void om_Initialize()				// 게임 데이터를 초기화하는 함수
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
	}								// 각종 전역변수와 배열 변수를 초기화
}

void om_Beep()						// 게임 시작/종료시 사운드 출력
{
	if (gv_BlackWin==TRUE || gv_WhiteWin==TRUE)	// 게임 종료시 사운드
	{
		Beep(392,250);
		Beep(392,250);
		Beep(392,250);
		Beep(524,1000);
	}
	else										// 시작 사운드
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

void om_DrawPlate()					// 바둑판을 출력하는 함수
{
	int i,j;
	printf("\n      ");
	for (i=0;i<25;i++)
		printf("%2d",i+1);			// 바둑판 상단 x축 번호를 출력
	printf("\n    ");	
	for (i=0;i<25;i++)
	{
		if (i<9)
			printf(" %d",i+1);
		else
			printf("%d",i+1);		// 바둑판 좌측 y축 번호를 출력

		for (j=0;j<25;j++)
		{
			if (gv_StoneArr[i+5][j+5]==0)
			{
				if (i==0 && j==0)
					Cprintf("┌",CR_BLACK,CR_GOLD);
				else if (i==0 && j!=24)
					Cprintf("┬",CR_BLACK,CR_GOLD);
				else if (i==0 && j==24)
					Cprintf("┐",CR_BLACK,CR_GOLD);
				else if (i!=24 && j==0 && j!=24)
					Cprintf("├",CR_BLACK,CR_GOLD);
				else if (i!=24 && j==24)
					Cprintf("┤",CR_BLACK,CR_GOLD);
				else if (i==24 && j==0)
					Cprintf("└",CR_BLACK,CR_GOLD);
				else if (i==24 && j==24)
					Cprintf("┘",CR_BLACK,CR_GOLD);
				else if (i==24 && j!=0)
					Cprintf("┴",CR_BLACK,CR_GOLD);
				else
					Cprintf("┼",CR_BLACK,CR_GOLD);
			}						// 바둑판 모서리 처리
				
			else if (gv_StoneArr[i+5][j+5]==1)
				Cprintf("●",CR_BLACK,CR_GOLD);	
			else if (gv_StoneArr[i+5][j+5]==2)
				Cprintf("●",CR_WHITE,CR_GOLD);
		}							// 배열 데이터에 따른 바둑판과 바둑돌 출력
		printf("\n    ");
	}	
	printf("\n");
	if (gv_WhiteWin==FALSE && gv_BlackWin==FALSE)	// 승리자가 없을 경우 다음 순서로 이동
		om_Turn();
}

void om_Turn()						// 플레이 순서를 관리하는 함수
{
	if (gv_TurnChk==1)
		om_BlackStone();
	else if (gv_TurnChk==2)
		om_WhiteStone();
}									// 순서 관리 전역변수에 따라 함수를 호출

void om_Insert(int *i,int *j)		// 돌의 위치를 입력받는 함수
{
	int ti,tj;
	if (gv_TurnChk==1)
	{
		MoveCur(0,29);
		printf("┌────────────────────────────┐\n");
		printf("│ 검은돌 차례입니다. X좌표를 입력해 주세요 :             │\n");
		printf("└────────────────────────────┘\n");
		MoveCur(47,30);
		scanf("%d",&ti);		
		MoveCur(0,29);
		printf("┌────────────────────────────┐\n");
		printf("│ 검은돌 차례입니다. Y좌표를 입력해 주세요 :             │\n");
		printf("└────────────────────────────┘\n");
		MoveCur(47,30);
		scanf("%d",&tj);
	}

	if (gv_TurnChk==2)
	{
		MoveCur(0,29);
		printf("┌────────────────────────────┐\n");
		printf("│ 흰돌 차례입니다. X좌표를 입력해 주세요 :               │\n");
		printf("└────────────────────────────┘\n");
		MoveCur(47,30);
		scanf("%d",&ti);		
		MoveCur(0,29);
		printf("┌────────────────────────────┐\n");
		printf("│ 흰돌 차례입니다. Y좌표를 입력해 주세요 :               │\n");
		printf("└────────────────────────────┘\n");
		MoveCur(47,30);
		scanf("%d",&tj);
	}								// om_Insert() 함수 내 지역변수에 먼저 좌표값을 입력받는다
	*i=ti;
	*j=tj;							// 그 후에 지역변수의 좌표값을 포인터로 이동시킨다
}

void om_BlackStone()				// 흑돌의 각종 검사(승리, 오류, 쌍삼, 육목)를 수행하는 함수
{
	int i,j;	
	while (gv_BlackChk==TRUE)		// 오류가 없을때까지 반복
	{
		om_Insert(&i,&j);			// 좌표 입력 함수에 주소값을 넘겨준다
		i--;						// 배열은 0부터 시작이지만 사용자는 최소 1을 입력하기 때문에
		j--;						// 값을 받은 뒤 1을 뺀다		
		om_ErrorChk(j+5,i+5);		// 제작 특성상 실제 바둑판은 (5,5)부터 시작한다
	}
	gv_StoneArr[j+5][i+5]=1;		// 전역 배열 변수에 흑돌값(1)을 입력
	gv_TurnChk=2;					// 순서를 백돌 순서로 바꿔준다
	gv_BlackChk=TRUE;				// 다음 실행을 위해 BOOL변수 값을 초기화한다
	om_WinCheck(j+5,i+5);			// 좌표 기준에서 승리했는지 검사하는 함수를 호출한다
}

void om_WhiteStone()				// 백돌의 각종 검사(승리, 오류, 쌍삼, 육목)를 수행하는 함수
{
	int i,j;	
	while (gv_WhiteChk==TRUE)		// 오류가 없을때까지 반복
	{
		om_Insert(&i,&j);			// 좌표 입력 함수에 주소값을 넘겨준다
		i--;						// 배열은 0부터 시작이지만 사용자는 최소 1을 입력하기 때문에
		j--;						// 값을 받은 뒤 1을 뺀다		
		om_ErrorChk(j+5,i+5);		// 제작 특성상 실제 바둑판은 (5,5)부터 시작한다
	}	
	gv_StoneArr[j+5][i+5]=2;		// 전역 배열 변수에 흑돌값(1)을 입력
	gv_TurnChk=1;					// 순서를 백돌 순서로 바꿔준다
	gv_WhiteChk=TRUE;				// 다음 실행을 위해 BOOL변수 값을 초기화한다
	om_WinCheck(j+5,i+5);			// 좌표 기준에서 승리했는지 검사하는 함수를 호출한다
}

void om_ErrorChk(int i,int j)		// 돌 위치에 따른 오류를 검사하는 함수
{
	MoveCur(4,32);
	if (gv_TurnChk==1)				// 흑돌 순서일 경우
	{
		if (gv_StoneArr[i][j]!=0)	// 놓으려는 좌표의 값이 0이 아닐 경우 다른 돌이 놓여있음
		{
			gv_BlackChk=TRUE;		// while문이 작동할 수 있도록 BOOL변수를 초기화
			Cprintf("그 자리에 이미 돌이 있습니다. 다른 좌표를 입력하세요.\n",CR_RED,CR_GREEN);	// 메시지 출력
		}		
		else if(i>29 || j>29 || i<5 || j<5)	// 25x25 구역 밖의 좌표를 입력했을 경우
		{
			gv_BlackChk=TRUE;			
			Cprintf("좌표가 영역을 벗어났습니다. 다른 좌표를 입력하세요.\n",CR_RED,CR_GREEN);	// 메시지 출력
		}		
		else
			gv_BlackChk=FALSE;		// 오류가 없을 경우 while문의 종료를 위한 변수값 변경
	}
	else if (gv_TurnChk==2)				// 백돌 순서일 경우
	{
		if (gv_StoneArr[i][j]!=0)		// 놓으려는 좌표의 값이 0이 아닐 경우 다른 돌이 놓여있음
		{
			gv_WhiteChk=TRUE;			// while문이 작동할 수 있도록 BOOL변수를 초기화					
			Cprintf("그 자리에 이미 돌이 있습니다. 다른 좌표를 입력하세요.\n",CR_RED,CR_GREEN);	// 메시지 출력
		}		
		else if(i>29 || j>29 || i<5 || j<5)	// 25x25 구역 밖의 좌표를 입력했을 경우
		{
			gv_WhiteChk=TRUE;			
			Cprintf("좌표가 영역을 벗어났습니다. 다른 좌표를 입력하세요.\n",CR_RED,CR_GREEN);	// 메시지 출력
		}		
		else
			gv_WhiteChk=FALSE;		// 오류가 없을 경우 while문의 종료를 위한 변수값 변경
	}
	om_SamSamChk(i,j);				// 3x3의 경우 아예 돌을 놓을 수 없기 때문에 om_ErrorChk()에서 검사한 뒤,
}									// 조건을 체크하고 돌을 둘지 말지 결정해야 함

void om_SamSamChk(int i,int j)		// 3x3 금수를 검사하는 함수
{	// 3x3 검사 조건 시작
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
			)	// 3x3 검사 조건 마침. 총 22개의 조건문
		{
			gv_BlackChk=TRUE;			// while문의 작동을 위한 BOOL변수 초기화
			Cprintf("쌍삼!  다른 좌표를 입력하세요.\n\a\a",CR_RED,CR_GREEN);	// 메시지 출력
			Sleep(1000);				// 메시지를 출력한 뒤 1000ms(1sec) 동안 유지함
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
			)	// 3x3 검사 조건 마침. 총 22개의 조건문
		{
			gv_WhiteChk=TRUE;			// while문의 작동을 위한 BOOL변수 초기화
			Cprintf("쌍삼! 다른 좌표를 입력하세요.\n\a\a",CR_RED,CR_GREEN);	// 메시지 출력
			Sleep(1000);				// 메시지를 출력한 뒤 1000ms(1sec) 동안 유지함
		}
	}
}

void om_WinCheck(int i,int j)			// 승리 조건을 검사하는 함수
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
		if (gv_StoneArr[i][j]==1)			// 승리 조건이 만족하며 방금 둔 돌이 흑돌일 경우에
			gv_BlackWin=TRUE;				// 흑돌 승리를 플래그함
		else if (gv_StoneArr[i][j]==2)		// 승리 조건이 만족하며 방금 둔 돌이 백돌일 경우에
			gv_WhiteWin=TRUE;				// 백돌 승리를 플래그함
	}
	om_SixStnChk(i,j);						// 육목일 경우에는 승리하지 않기 때문에 육목 검사 함수 호출
}

void om_SixStnChk(int i,int j)				// 육목 금수를 검사하는 함수
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
			gv_BlackWin=FALSE;				// 흑돌이 육목일 경우 윽돌 승리 플래그를 취소
			Cprintf("육목! 승리하지 못했습니다.\n\a",CR_RED,CR_GREEN);	// 메시지 출력
			Sleep(1000);					// 메시지를 출력한 뒤 1초간 유지
		}
		else if (gv_WhiteWin==TRUE)
		{
			gv_WhiteWin=FALSE;				// 백돌이 육목일 경우 윽돌 승리 플래그를 취소
			Cprintf("육목! 승리하지 못했습니다.\n\a",CR_RED,CR_GREEN);	// 메시지 출력
			Sleep(1000);					// 메시지를 출력한 뒤 1초간 유지
		}
	}
}

void om_Gameover()							// 게임 종료시 승리자를 출력하는 함수
{
	char retry;
	om_DrawPlate();	// 게임이 끝난 바둑판을 출력
	MoveCur(17,8);
	if (gv_WhiteWin==TRUE)					// 승리자가 백돌일 경우 메시지 출력
		Cprintf("흰색 돌이 승리하였습니다!",CR_WHITE,CR_BLACK);	
	else if (gv_BlackWin==TRUE)				// 승리자가 흑돌일 경우 메시지 출력
		Cprintf("검은색 돌이 승리하였습니다!",CR_WHITE,CR_BLACK);
	om_Beep();								// 승리 사운드 호출
	fflush(stdin);
	MoveCur(16,30);
	Cprintf("다시 시작 하시겠습니까?(y/n) : ",CR_WHITE,CR_BLACK);
	retry=getch();							// 버퍼를 사용하지 않고 한 글자만 입력
	if (retry=='y')							// 만약 y가 입력될 경우 게임을 재시작
	{
		ConClr();
		om_Run();
	}										// y 이외의 값을 입력할 경우 게임 종료
	else
		ConClr();
}
