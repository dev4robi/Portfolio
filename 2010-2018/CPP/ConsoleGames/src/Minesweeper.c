/************************************************************************/
/* [Minesweeper game using C language]                                  */
/*                                                                      */
/*  # Produced by "2010305049 Lee Tae Hoon"                             */
/*																		*/
/*	# 2011/12/3 : 1.00						                            */
/*	# The last update: 2011/12/3        								*/
/*									     	     ┏┓  ┏━━┓┏┓┏┓	*/
/*												 ┃┗┓┗┓┏┛┃┣┫┃	*/
/*												 ┗━┛  ┗┛  ┗┛┗┛	*/
/************************************************************************/

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>

// 메크로
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

// 전역 변수
boolean gb_Testmode = FALSE; // 테스트 모드 (초기화: 사용)
boolean gb_Colormode = TRUE; // 색상 사용 모드 (초기화: 사용)
boolean gb_Soundmode = TRUE; // 소리 사용 (초기화: 사용)
int gb_GameState = GS_MAINMENU; // 게임 상태 (초기화: 메인메뉴)
char gb_GameVersion[] = "1.00"; // 게임 버전
int gb_SelectLevel = 1; // 선택된 난이도 (초기화: 초보)
int gb_SelectUI = 1; // 선택된 UI (초기화: 기본)
struct GameBoard gb_MainBoard[20][30]; // (게임 보드)
int gb_SizeX = 9, gb_SizeY = 9; // 게임판 사이즈 (초기화: 초보)
int gb_Mines = 10; // 지뢰수 (초기화: 초보)
int gb_UIMines = 10; // 인터페이스에 표시되는 지뢰수
int gb_UIrenewtimer = 10000; // UI 갱신을 위한 타이머변수
int gb_ClickCount = 0; // 버튼 클릭 횟수 (초기화: 0)
int gb_Time = 0; // 경과 시간을 저장하기 위한 변수

//
extern void Cprintf(char *String, int FontCR, int BackCR);
extern void ChangeCR(int FontCR, int BackCR);
extern void ConClr();
extern void MoveCur(int x, int y);
extern int Random_Int(int Min, int Max);
extern void BipSound(int Pitch, int Duration, boolean UseSound);
extern void SetCurType(int CurType);

// 지뢰 찾기 일반 함수 프로토타입
void ms_Initialization(); // 초기화 함수
void ms_DrawMainMenu(); // 메인메뉴 그리는 함수
int ms_OnMainMenu(); // 메인메뉴 선택 관련 함수
void ms_DrawOptionMenu(); // 옵션메뉴 그리는 함수
void ms_OnOptionMenu(); // 옵션메뉴 선택 관련 함수
void ms_DrawGameBoard(); // 게임보드 그리는 함수
void ms_FormatBoardData(int i, int j, int BackColor); // 보드의 데이터를 UI/색에 따라 저장된 문자로 출력시키는 함수
int ms_OnGameBoard(); // 게임중 선택 관련 함수
int ms_LeftClick(int x, int y, boolean Recursive); // 왼쪽 클릭 액션
void ms_RightClick(int x, int y); // 오른쪽 클릭 액션
int ms_DualClick(int x, int y); // 양쪽 클릭 액션
void ms_CreateMine(int x, int y); // 지뢰 생성 함수 (첫 클릭시 생성. 매개변수로 첫클릭 지점 넣음)
void ms_PlusBoardNumber(int x, int y); // 입력된 좌표의 보드 데이터를 1증가 (데이터가 8미만인 경우에만 증가)
void ms_DrawGameOver(); // 게임 오버시 화면 출력후 입력 대기
void ms_DrawWin(); // 승리하면 화면 출력후 입력 대기
int ms_CheckWin(); // 승리 조건 판단 함수

// 박스 함수 프로토타입
void TypeBox(char String[76], int ColorFont, int ColorBack);
void EraseBox();

// 개조 함수 프로토타입
void BCprintf(char *String, int FontCR, int BackCR);

// 함수
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

	if(!gb_Testmode) system("mode con cols=80 lines=25"); // 콘솔 사이즈 변경
	else system("mode con cols=80 lines=30");

	if(!gb_Testmode) system("title ConsoleGames - Minesweeper"); // 콘솔 제목 변경
	else system("title ConsoleGames - Minesweeper Testmode Applied");

	gb_GameState = GS_MAINMENU; // 게임 상태 (초기화: 메인메뉴)
	// strcpy(gb_GameVersion, "1.00"); // 게임 버전
	// gb_SelectLevel = 1; // 선택된 난이도 (초기화: 초보)
	// gb_SelectUI = 1; // 선택된 UI (초기화: 기본)
	// gb_Colormode = TRUE; // 색상 사용 모드(초기화: 사용)
	// gb_Soundmode = TRUE; // 소리 사용 (초기화: 사용)
	gb_UIMines = gb_Mines; // 인터페이스 지뢰 수 = 실제 지뢰 수 (동기화)
	gb_ClickCount = 0; // 버튼 클릭 횟수 (초기화: 0)
	gb_Time = 0; // 시간 초기화

	for(i=0; i<20; i++) // 메인 보드 초기화
	{
		for(j=0; j<30; j++)
		{
			gb_MainBoard[i][j].BoardData = 0;
			gb_MainBoard[i][j].VisualType = BT_HIDE;
		}
	}
	
	// gb_SizeX = 9; // 게임판 사이즈 (초기화: 초보)
	// gb_SizeY = 9;

	SetCurType(CT_HIDE); // 커서 숨김
}

void ms_DrawMainMenu() // GS_MAINMENU
{
	ConClr();
	BCprintf("┌──────────────────────────────────────┒", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                              [Made By : 2010305049 이태훈] ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", CR_WHITE, CR_BLACK);
	MoveCur(2, 2); BCprintf("    지 뢰 찾 기 ver ", CR_WHITE, CR_BLACK); MoveCur(0, 8);
	BCprintf("                          ┌────────────┒                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │                        ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │   ● [ 새로하기 ] ●   ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │                        ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │   ○ [ 이어하기 ] ○   ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │                        ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │   ○  [ 설  정 ]  ○   ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │                        ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │   ○ [ 게임종료 ] ○   ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          │                        ┃                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          ┕━━━━━━━━━━━━┛                          ", CR_WHITE, CR_BLACK);
	MoveCur(0, 21);
	BCprintf("┌──────────────────────────────────────┒", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", CR_WHITE, CR_BLACK);
	MoveCur(34, 12); BCprintf("[ 이어하기 ]", CR_DGRAY, CR_BLACK);
	TypeBox("게임을 새로 시작합니다.", CR_WHITE, CR_BLACK);
	
	if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	MoveCur(22, 2); printf("%s", gb_GameVersion); ChangeCR(CR_GRAY, CR_BLACK);
	
	if(gb_Testmode) { MoveCur(61, 2); Cprintf("Testmode Applied", CR_RED, CR_BLACK); } // 게임 버전 / 테스트모드 확인

	MoveCur(2, 12);
}

void ms_DrawOptionMenu() // GS_OPTION
{
	ConClr();
	BCprintf("┌──────────────────────────────────────┒", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", CR_WHITE, CR_BLACK);
	MoveCur(2, 2); BCprintf("    게  임  설  정  (Option)", CR_WHITE, CR_BLACK);
	MoveCur(0, 7);
	BCprintf("┌──────────────────────────────────────┒", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│   ● [ 난이도 ] :: 초급 (가로: 9 / 세로: 9 / 지뢰 : 10)                    ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│   ○ [ UI선택 ] :: 동글 (●○①②③④⑤⑥⑦⑧ⓜⓠ¶)                       ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│   ○ [ 색  상 ] :: 사용                                                    ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│   ○ [ 소  리 ] :: 사용                                                    ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("│   ○ [ 메인 메뉴로 ]                                                       ┃", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", CR_WHITE, CR_BLACK);
	MoveCur(0, 21);
	BCprintf("┌──────────────────────────────────────┒", CR_WHITE, CR_BLACK);
	BCprintf("│                                                                            ┃", CR_WHITE, CR_BLACK);
	BCprintf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", CR_WHITE, CR_BLACK);
	TypeBox("게임 난이도를 변경합니다. 초급, 중급, 고급 난이도가 있습니다.               ", CR_WHITE, CR_BLACK);

	// 난이도
	if(gb_SelectLevel == 1) { MoveCur(22, 9); BCprintf("초급", CR_YELLOW, CR_BLACK); BCprintf(" (가로: 9 / 세로: 9 / 지뢰 : 10)          ", CR_DGRAY, CR_BLACK); } 
	else if(gb_SelectLevel == 2) { MoveCur(22, 9); BCprintf("중급", CR_DGREEN, CR_BLACK); BCprintf(" (가로: 16 / 세로: 16 / 지뢰 : 40)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectLevel == 3) { MoveCur(22, 9); BCprintf("고급", CR_RED, CR_BLACK); BCprintf(" (가로: 30 / 세로: 18 / 지뢰 : 112)          ", CR_DGRAY, CR_BLACK); }
	
	// UI선택
	if(gb_SelectUI == 1) { MoveCur(22, 11); BCprintf("동글", CR_WHITE, CR_BLACK); BCprintf(" (●○①②③④⑤⑥⑦⑧ⓜⓠ¶)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectUI == 2) { MoveCur(22, 11); BCprintf("네모", CR_WHITE, CR_BLACK); BCprintf(" (■□⑴⑵⑶⑷⑸⑹⑺⑻⒨⒬¶)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectUI == 3) { MoveCur(22, 11); BCprintf("혼잡", CR_WHITE, CR_BLACK); BCprintf(" (◆◇일이삼사오육칠팔★??¶)          ", CR_DGRAY, CR_BLACK); }

	// 색상
	if(gb_Colormode) { MoveCur(22, 13); Cprintf("사용          ", CR_GOLD, CR_BLACK); }
	else { MoveCur(22, 13); printf("사용 안함          "); }

	// 소리
	if(gb_Soundmode) { MoveCur(22, 15); BCprintf("사용          ", CR_DCYAN, CR_BLACK); }
	else { MoveCur(22, 15); BCprintf("사용 안함          ", CR_DCYAN, CR_BLACK); }

	MoveCur(2, 12);
}

void ms_DrawGameBoard() // GS_GAMESTRAT
{
	int i, j;

	ConClr();

	// 칼라 모드이면 흰색 선 검은 배경으로 그린다
	if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	
	// 주변 경계
	printf("┌────────────────────────────────┬─────┒");
	for(i=0; i<20; i++)
	printf("│                                                                │          ┃");
	printf("├────────────────────────────────┴─────┨");
	printf("│                                                                            ┃");
	printf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	
	// 보드 테두리 그리기(윗면)
	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("┏");
		else if(i==gb_SizeX) printf("┓");
		else printf("━");
	}

	// 보드 테두리 그리기(측면)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("┃"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("┃"); }
		}
	}

	// 보드 테두리 그리기(하단)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("┗");
		else if(i==gb_SizeX) printf("┛");
		else printf("━");
	}

	// 보드 메뉴창 & 점수와 시간
	MoveCur(68, 2); (gb_Colormode) ? Cprintf(" 【메뉴】 ", CR_DGREEN, CR_BLACK) : printf(" 【메뉴】 ");
	MoveCur(68, 5); (gb_Colormode) ? Cprintf("[새로하기]", CR_WHITE, CR_BLACK) : printf("[새로하기]");
	MoveCur(68, 7); (gb_Colormode) ? Cprintf("[저장하기]", CR_DGRAY, CR_BLACK) : printf("[저장하기]");
	MoveCur(68, 9); (gb_Colormode) ? Cprintf("[메인메뉴]", CR_WHITE, CR_BLACK) : printf("[메인메뉴]");
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("├─────┨", CR_WHITE, CR_BLACK) : printf("├─────┨");
	MoveCur(68, 13); (gb_Colormode) ? Cprintf("【지뢰수】", CR_RED, CR_BLACK) : printf("【지뢰수】");
	MoveCur(68, 15); (gb_Colormode) ? Cprintf("    00    ", CR_WHITE, CR_BLACK) : printf("    00    ");
	MoveCur(68, 17); (gb_Colormode) ? Cprintf(" 【시간】 ", CR_RED, CR_BLACK) : printf(" 【시간】 ");
	MoveCur(68, 19); (gb_Colormode) ? Cprintf("[00:00:00]", CR_WHITE, CR_BLACK) : printf("[00:00:00]");
	
	// 보드 그리기
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=0; j<gb_SizeX; j++)
		{
			MoveCur(j*2+4, i+2);
			ms_FormatBoardData(i, j, CR_GRAY);
		}
	}

	// 시작 지점 그리기
	if(gb_ClickCount == 0) { MoveCur(4, 2); ms_FormatBoardData(0, 0, CR_WHITE); }

	// 초기 도움말
	TypeBox("[A]:왼쪽 클릭  [S]:양쪽 클릭  [D]:오른쪽 클릭  [↑→↓←]:이동  [ESC]:메뉴", CR_WHITE, CR_BLACK);

	// 피봇 여부 (색상 사용 안할시)
	if(!(gb_Colormode)) { SetCurType(CT_SOLID); MoveCur(4, 2); }
	else MoveCur(2, 12);
}

void ms_FormatBoardData(int y, int x, int BackColor)
{
	// [UI] ●○①②③④⑤⑥⑦⑧ⓜⓠ¶ / ■□⑴⑵⑶⑷⑸⑹⑺⑻⒨⒬¶ / ◆◇일이삼사오육칠팔★??¶
	// [CR] 0: 진회색 1:청색 2:진녹색 3:적색 4:남색 5:갈색 6:청록 7:흑색 8:회색 ?:남색 m:흑색 f:적색

	if(gb_MainBoard[y][x].VisualType == BT_HIDE) // 아직 확인되지 않은 보드.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("●", CR_DGRAY, BackColor) : printf("●");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("■", CR_DGRAY, BackColor) : printf("■");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("◆", CR_DGRAY, BackColor) : printf("◆");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_UNCOVER) // 클릭되어 확인된 보드.
	{
		switch(gb_MainBoard[y][x].BoardData)
		{
			case 0:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("○", CR_DGRAY, BackColor) : printf("○");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("□", CR_DGRAY, BackColor) : printf("□");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("◇", CR_DGRAY, BackColor) : printf("◇"); break;
			case 1:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("①", CR_BLUE, BackColor) : printf("①");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑴", CR_BLUE, BackColor) : printf("⑴");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("일", CR_BLUE, BackColor) : printf("일"); break;
			case 2:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("②", CR_DGREEN, BackColor) : printf("②");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑵", CR_DGREEN, BackColor) : printf("⑵");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("이", CR_DGREEN, BackColor) : printf("이"); break;
			case 3:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("③", CR_RED, BackColor) : printf("③");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑶", CR_RED, BackColor) : printf("⑶");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("삼", CR_RED, BackColor) : printf("삼"); break;
			case 4:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("④", CR_DBLUE, BackColor) : printf("④");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑷", CR_DBLUE, BackColor) : printf("⑷");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("사", CR_DBLUE, BackColor) : printf("사"); break;
			case 5:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("⑤", CR_DRED, BackColor) : printf("⑤");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑸", CR_DRED, BackColor) : printf("⑸");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("오", CR_DRED, BackColor) : printf("오"); break;
			case 6:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("⑥", CR_DCYAN, BackColor) : printf("⑥");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑹", CR_DCYAN, BackColor) : printf("⑹");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("육", CR_DCYAN, BackColor) : printf("육"); break;
			case 7:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("⑦", CR_BLACK, BackColor) : printf("⑦");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑺", CR_BLACK, BackColor) : printf("⑺");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("칠", CR_BLACK, BackColor) : printf("칠"); break;
			case 8:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("⑧", CR_DGRAY, BackColor) : printf("⑧");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⑻", CR_DGRAY, BackColor) : printf("⑻");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("팔", CR_DGRAY, BackColor) : printf("팔"); break;
			case 9:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("ⓜ", CR_BLACK, BackColor) : printf("ⓜ");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⒨", CR_BLACK, BackColor) : printf("⒨");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("★", CR_BLACK, BackColor) : printf("★"); break;
			default:
				(gb_Colormode == TRUE) ? Cprintf("??", CR_GOLD, BackColor) : printf("??");
		}
	}

	else if(gb_MainBoard[y][x].VisualType == BT_FLAG) // 깃발 표시를 해 놓은 보드.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("¶", CR_RED, BackColor) : printf("¶");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("¶", CR_RED, BackColor) : printf("¶");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("¶", CR_RED, BackColor) : printf("¶");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_QUESTION) // 물음표 표시를 해 놓은 보드.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("ⓠ", CR_DBLUE, BackColor) : printf("ⓠ");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("⒬", CR_DBLUE, BackColor) : printf("⒬");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("??", CR_DBLUE, BackColor) : printf("??");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_MISS) // 지뢰로 잘못 표시한 보드.
	{
		BCprintf("※", CR_DRED, CR_GRAY);
	}
}

void ms_DrawGameOver()
{	
	int i, j;
	char KeyValue;

	// 주변 경계 그리기
	MoveCur(0, 0); BCprintf("┌────────────────────────────────┬─────┒", CR_RED, CR_BLACK);
	for(i=1; i<21; i++)
	{
		MoveCur(0, i); BCprintf("│", CR_RED, CR_BLACK); MoveCur(66, i); BCprintf("│", CR_RED, CR_BLACK); MoveCur(78, i); BCprintf("┃", CR_RED, CR_BLACK);
	}
	MoveCur(0, 21); BCprintf("├────────────────────────────────┴─────┨", CR_RED, CR_BLACK);
	MoveCur(0, 22); BCprintf("│                                                                            ┃", CR_RED, CR_BLACK);
	MoveCur(0, 23); BCprintf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", CR_RED, CR_BLACK);
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("├─────┨", CR_RED, CR_BLACK) : printf("├─────┨");

	// 보드 테두리 그리기(윗면)
	if(gb_Colormode) ChangeCR(CR_RED, CR_BLACK);

	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("┏");
		else if(i==gb_SizeX) printf("┓");
		else printf("━");
	}
	
	// 보드 테두리 그리기(측면)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("┃"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("┃"); }
		}
	}
	
	// 보드 테두리 그리기(하단)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("┗");
		else if(i==gb_SizeX) printf("┛");
		else printf("━");
	}

	// 경고음
	BipSound(1000, 500, gb_Soundmode); Sleep(250);
	TypeBox("지뢰가 선택되었습니다... Game Over 메인으로 돌아가려면 [ESC]를 누르십시오.", CR_RED, CR_BLACK);

	// 모든 지뢰 보이기, 잘못 깃발 꼽은곳 보이기
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
	
	// 키 입력 대기
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
	
	// 주변 경계 그리기
	MoveCur(0, 0); BCprintf("┌────────────────────────────────┬─────┒", CR_GREEN, CR_BLACK);
	for(i=1; i<21; i++)
	{
		MoveCur(0, i); BCprintf("│", CR_GREEN, CR_BLACK); MoveCur(66, i); BCprintf("│", CR_GREEN, CR_BLACK); MoveCur(78, i); BCprintf("┃", CR_GREEN, CR_BLACK);
	}
	MoveCur(0, 21); BCprintf("├────────────────────────────────┴─────┨", CR_GREEN, CR_BLACK);
	MoveCur(0, 22); BCprintf("│                                                                            ┃", CR_GREEN, CR_BLACK);
	MoveCur(0, 23); BCprintf("┕━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", CR_GREEN, CR_BLACK);
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("├─────┨", CR_GREEN, CR_BLACK) : printf("├─────┨");
	
	// 보드 테두리 그리기(윗면)
	if(gb_Colormode) ChangeCR(CR_GREEN, CR_BLACK);
	
	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("┏");
		else if(i==gb_SizeX) printf("┓");
		else printf("━");
	}
	
	// 보드 테두리 그리기(측면)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("┃"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("┃"); }
		}
	}
	
	// 보드 테두리 그리기(하단)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("┗");
		else if(i==gb_SizeX) printf("┛");
		else printf("━");
	}
	
	// 승리 BGM
	BipSound(261, 333, gb_Soundmode); BipSound(329, 333, gb_Soundmode); BipSound(391, 333, gb_Soundmode); Sleep(250);
	TypeBox("성공하였습니다! 최종 시간은 ", CR_GREEN, CR_BLACK);
	ChangeCR(CR_GREEN, CR_BLACK); printf("[%.2d:%.2d:%.2d]입니다. 메인으로 돌아가기 : [ESC]키", (gb_Time/3600), ((gb_Time-(gb_Time/3600))/60), (gb_Time%60));
	ChangeCR(CR_GRAY, CR_BLACK);
	
	// 모든 지뢰에 깃발 곱기
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

	// 지뢰수 0개로 변경
	gb_UIMines = 0;
	MoveCur(71, 15); if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	printf("%3d", gb_UIMines); ChangeCR(CR_GRAY, CR_BLACK);
	
	// 키 입력 대기
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
				MoveCur(31, 10);  printf("●"); MoveCur(47, 10);  printf("●"); MoveCur(31, 12); printf("○"); MoveCur(47, 12); printf("○");
				MoveCur(31, 14); printf("○"); MoveCur(47, 14); printf("○"); MoveCur(31, 16); printf("○"); MoveCur(47, 16); printf("○");
				TypeBox("게임을 새로 시작합니다.", CR_WHITE, CR_BLACK);
			}
			
			else if(CurrentSelect == 2)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("○"); MoveCur(47, 10);  printf("○"); MoveCur(31, 12); printf("●"); MoveCur(47, 12); printf("●");
				MoveCur(31, 14); printf("○"); MoveCur(47, 14); printf("○"); MoveCur(31, 16); printf("○"); MoveCur(47, 16); printf("○");
				TypeBox("저장된 게임을 불러옵니다. (구현중입니다)", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 3)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("○"); MoveCur(47, 10);  printf("○"); MoveCur(31, 12); printf("○"); MoveCur(47, 12); printf("○");
				MoveCur(31, 14); printf("●"); MoveCur(47, 14); printf("●"); MoveCur(31, 16); printf("○"); MoveCur(47, 16); printf("○");
				TypeBox("난이도, UI, 색상, 소리 등을 설정합니다.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 4)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("○"); MoveCur(47, 10);  printf("○"); MoveCur(31, 12); printf("○"); MoveCur(47, 12); printf("○");
				MoveCur(31, 14); printf("○"); MoveCur(47, 14); printf("○"); MoveCur(31, 16); printf("●"); MoveCur(47, 16); printf("●");
				TypeBox("게임을 종료합니다.", CR_WHITE, CR_BLACK);
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
						MoveCur(22, 9); BCprintf("초급", CR_YELLOW, CR_BLACK); BCprintf(" (가로: 9 / 세로: 9 / 지뢰 : 10)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 9; gb_SizeY = 9; gb_Mines = 10;
					}
					
					else if(gb_SelectLevel == 2)
					{
						MoveCur(22, 9); BCprintf("중급", CR_DGREEN, CR_BLACK); BCprintf(" (가로: 16 / 세로: 16 / 지뢰 : 40)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 16; gb_SizeY = 16; gb_Mines = 40;
					}
					
					else if(gb_SelectLevel == 3)
					{
						MoveCur(22, 9); BCprintf("고급", CR_RED, CR_BLACK); BCprintf(" (가로: 30 / 세로: 18 / 지뢰 : 112)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 30; gb_SizeY = 18; gb_Mines = 112;
					}
				}

				else if(CurrentSelect == 2)
				{
					(gb_SelectUI < 3) ? (gb_SelectUI++) : (gb_SelectUI = 1);
					if(gb_SelectUI == 1) { MoveCur(22, 11); BCprintf("동글", CR_WHITE, CR_BLACK); BCprintf(" (●○①②③④⑤⑥⑦⑧ⓜⓠ¶)          ", CR_DGRAY, CR_BLACK); }
					else if(gb_SelectUI == 2) { MoveCur(22, 11); BCprintf("네모", CR_WHITE, CR_BLACK); BCprintf(" (■□⑴⑵⑶⑷⑸⑹⑺⑻⒨⒬¶)          ", CR_DGRAY, CR_BLACK); }
					else if(gb_SelectUI == 3) { MoveCur(22, 11); BCprintf("혼잡", CR_WHITE, CR_BLACK); BCprintf(" (◆◇일이삼사오육칠팔★??¶)          ", CR_DGRAY, CR_BLACK); }
				}

				else if(CurrentSelect == 3)
				{
					gb_Colormode = !(gb_Colormode);
					if(gb_Colormode) { MoveCur(22, 13); Cprintf("사용          ", CR_GOLD, CR_BLACK); }
					else { MoveCur(22, 13); printf("사용 안함          "); }
					ms_DrawOptionMenu();
				}

				else if(CurrentSelect == 4)
				{
					gb_Soundmode = !(gb_Soundmode);
					if(gb_Soundmode) { MoveCur(22, 15); BCprintf("사용          ", CR_DCYAN, CR_BLACK); BipSound(1000, 100, TRUE); }
					else { MoveCur(22, 15); BCprintf("사용 안함         ", CR_DCYAN, CR_BLACK); }
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
				MoveCur(5, 9);  printf("●"); MoveCur(5, 11); printf("○"); MoveCur(5, 13); printf("○");
				MoveCur(5, 15); printf("○"); MoveCur(5, 17); printf("○");
				TypeBox("게임 난이도를 변경합니다. 초급, 중급, 고급 난이도가 있습니다.", CR_WHITE, CR_BLACK);
			} 

			else if(CurrentSelect == 2)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("○"); MoveCur(5, 11); printf("●"); MoveCur(5, 13); printf("○");
				MoveCur(5, 15); printf("○"); MoveCur(5, 17); printf("○");
				TypeBox("게임판의 문양을 변경합니다. 기본, 네모, 혼잡 종류가 있습니다.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 3)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("○"); MoveCur(5, 11); printf("○"); MoveCur(5, 13); printf("●");
				MoveCur(5, 15); printf("○"); MoveCur(5, 17); printf("○");
				TypeBox("게임에 색상 사용 여부를 결정합니다.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 4)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("○"); MoveCur(5, 11); printf("○"); MoveCur(5, 13); printf("○");
				MoveCur(5, 15); printf("●"); MoveCur(5, 17); printf("○");
				TypeBox("소리 사용 여부를 결정합니다.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 5)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("○"); MoveCur(5, 11); printf("○"); MoveCur(5, 13); printf("○");
				MoveCur(5, 15); printf("○"); MoveCur(5, 17); printf("●");
				TypeBox("설정을 저장하고 메인 메뉴로 돌아갑니다.", CR_WHITE, CR_BLACK);
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
	int SavedX = 0, SavedY = 0; //【메뉴】상태일 때 CurrentX/Y를 저장하기 위한 변수
	boolean OnMenu = FALSE; // 【메뉴】상태인지를 저장하는 변수
	int MenuCounter = 0; // 【메뉴】가 몇번을 가리키고 있는지 저장하는 변수
	int ClickReturnValue; // 지점 선택시 행동을 받아오는 변수 (지뢰였나, 그냥 바닥이였나 등등)
	int RenewCounter = 0; // 키보드 입력을 안 받을 때 메뉴판 갱신 속도를 관리하기 위한 변수 (while문의 루프가 너무 빨라서 폰트에 선이 그어지기 때문에 사용)

	// 타이머 관련 변수
	time_t CurrentT; // 시간 관련 변수 (해더파일: <time.h>)
	struct tm *Today;	// 구조체 tm포인터
	int BeforeTime; // 현재 초단위 시간 저장을 위한 변수1
	int CurrentTime; // 현재 초단위 시간 저장을 위한 변수2

	time(&CurrentT); // CurrentTime 변수에 현재 시간 내려받음
	Today = localtime(&CurrentT); // Today 구조체에 CurrentTime의 데이터들을 내려받음
	BeforeTime = Today->tm_hour*3600 + Today->tm_min*60 + Today->tm_sec; // 전역 변수에 시간을 초로 변경하여 저장

	while(1)
	{
		RenewCounter++;

		time(&CurrentT);
		Today = localtime(&CurrentT);
		CurrentTime = Today->tm_hour*3600 + Today->tm_min*60 + Today->tm_sec;

		// 키보드가 눌렸을 때
		if(kbhit())
		{
			KeyValue = getch();
			EraseBox();

			if(OnMenu) // 키보드가 메뉴판 주도일때
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
					MoveCur(68, 5); BCprintf("[새로하기]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[저장하기]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[메인메뉴]", CR_WHITE, CR_BLACK);
					MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE); MoveCur((CurrentX)*2+4, CurrentY+2);
				}
				
				else if(KeyValue == KEY_ENTER)
				{
					if(CurrentY == 0)
					{
						if(MenuCounter == 0)
						{
							MenuCounter++;
							TypeBox("정말로 새로 하겠습니까? 새로 하려면 한번 더 엔터를 누르십시오.", CR_WHITE, CR_BLACK);
							BipSound(300, 500, gb_Soundmode);
						}
						
						else if (MenuCounter >= 1) { ms_Initialization(); return GS_RESTRAT; }
					}
					
					else if(CurrentY == 1)
					{
						TypeBox("아직 구현되지 않은 기능입니다.", CR_WHITE, CR_BLACK);
					}
					
					else if(CurrentY == 2)
					{
						if(MenuCounter == 0)
						{
							MenuCounter++;
							TypeBox("정말로 종료합니까? 종료하려면 한번 더 엔터를 누르십시오.", CR_WHITE, CR_BLACK);
							BipSound(300, 500, gb_Soundmode);
						}
						
						else if(MenuCounter >= 1) { ms_Initialization(); return GS_MAINMENU; }
					}
				}

				if(CurrentY == 0 && OnMenu) // OnMenu 삭제시 약간의 버그 발생
				{
					MoveCur(68, 5); BCprintf("[새로하기]", CR_WHITE, CR_PURPLE); MoveCur(68, 7); BCprintf("[저장하기]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[메인메뉴]", CR_WHITE, CR_BLACK);
					if(MenuCounter==0) TypeBox("게임을 새로 시작합니다. 진행중인 게임은 기록되지 않습니다.", CR_WHITE, CR_BLACK);
					MoveCur(69, 5); // 무색 모드를 위한 피벗 이동
				}

				else if(CurrentY == 1 && OnMenu)
				{
					MoveCur(68, 5); BCprintf("[새로하기]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[저장하기]", CR_DGRAY, CR_PURPLE);
					MoveCur(68, 9); BCprintf("[메인메뉴]", CR_WHITE, CR_BLACK);
					TypeBox("진행중인 게임을 저장합니다. (구현중입니다)", CR_WHITE, CR_BLACK);
					MoveCur(69, 7);
				}

				else if(CurrentY == 2 && OnMenu)
				{
					MoveCur(68, 5); BCprintf("[새로하기]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[저장하기]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[메인메뉴]", CR_WHITE, CR_PURPLE); MoveCur(69, 9);
					if(MenuCounter==0) TypeBox("메인메뉴로 돌아갑니다. 진행중인 게임은 기록되지 않습니다.", CR_WHITE, CR_BLACK);
					MoveCur(69, 9);
				}
			}
			
			else // 키보드가 게임판 주도일때
			{
				if(KeyValue == KEY_LEFT)
				{
					if(CurrentX > 0)
					{
						CurrentX--;
						MoveCur((CurrentX+1)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX+1, CR_GRAY); // 이동전 좌표의 모양 복구
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE); // 이동후 좌표의 현위치 표시
						MoveCur((CurrentX)*2+4, CurrentY+2); // 무색 모드를 위한 피벗 이동
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentX = gb_SizeX-1; ms_FormatBoardData(CurrentY, 0, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // 무색 모드를 위한 피벗 이동
					}
				}

				else if(KeyValue == KEY_RIGHT)
				{
					if(CurrentX < gb_SizeX-1)
					{
						CurrentX++;
						MoveCur((CurrentX-1)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX-1, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // 무색 모드를 위한 피벗 이동
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentX = 0; ms_FormatBoardData(CurrentY, gb_SizeX-1, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // 무색 모드를 위한 피벗 이동
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
						MoveCur((CurrentX)*2+4, CurrentY+2); // 무색 모드를 위한 피벗 이동
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
						MoveCur((CurrentX)*2+4, CurrentY+2); // 무색 모드를 위한 피벗 이동
					}
				}

				else if(KeyValue == KEY_ESC)
				{
					OnMenu = !(OnMenu);
					SavedX = CurrentX;
					SavedY = CurrentY;
					MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_GRAY);
					CurrentY = 0;
					TypeBox("메뉴로 이동합니다. 게임으로 돌아가려면 [ESC]키를 누르십시오.", CR_WHITE, CR_BLACK);
					MoveCur(68, 5); BCprintf("[새로하기]", CR_WHITE, CR_PURPLE); MoveCur(69, 5); // 무색 모드를 위한 피벗 이동
					BipSound(200, 100, gb_Soundmode);
				}

				else if(KeyValue == 'A' || KeyValue == 'a') // 왼쪽 클릭
				{
					ClickReturnValue = ms_LeftClick(CurrentX, CurrentY, FALSE);

					if(ClickReturnValue != GS_LOSE) ClickReturnValue = ms_CheckWin();
				}

				else if(KeyValue == 'S' || KeyValue == 's') // 양쪽 클릭
				{
					ClickReturnValue = ms_DualClick(CurrentX, CurrentY);

					if(ClickReturnValue != GS_LOSE) ClickReturnValue = ms_CheckWin();
				}

				else if(KeyValue == 'D' || KeyValue == 'd') // 오른쪽 클릭
				{
					ms_RightClick(CurrentX, CurrentY);
				}
			}
		}

		// 키보드가 안 눌렸을 때
		else
		{
			if(RenewCounter >= gb_UIrenewtimer) // 타이머 초과하면
			{
				RenewCounter = 0;

				// 【지뢰수】인터페이스
				MoveCur(71, 15); if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				printf("%3d", gb_UIMines); ChangeCR(CR_GRAY, CR_BLACK);

				// 【타이머】인터페이스
				if(CurrentTime > BeforeTime) // 1초 이상 경과하였으면
				{
					gb_Time += (CurrentTime - BeforeTime); // 시간 차만큼 증가시킴
					BeforeTime = CurrentTime; // 필수
					if(gb_Time > 359999) gb_Time = 359999; // 99시간 59분 59초 초과시 99시간 59분 59초로 고정
					if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
					MoveCur(68, 19); printf("[%.2d:%.2d:%.2d]", (gb_Time/3600), ((gb_Time-(gb_Time/3600))/60), (gb_Time%60)); // 인터페이스에 출력
					MoveCur((CurrentX)*2+4, CurrentY+2);
				}

				else // 23시 -> 0시로 넘어가는 부분일 경우 등... (약 1초 정도의 멈춤이 있을 수 있다)
				{
					BeforeTime = CurrentTime;
				}
				
			}
		}

		// 게임의 상태를 반환 (계속 진행 or 패배 or 승리)
		if(ClickReturnValue == GS_LOSE) return GS_LOSE; // 지뢰 밟아서 GS_LOSE 리턴시 반복문 탈출
		else if(ClickReturnValue == GS_WIN) return GS_WIN; // 승리조건 충족시 GS_WIN 호출로 반복문 탈출
	}
	
	return GS_STARTGAME; // 여기에 GS_LOSE, GS_WIN 호출 금지
}

int ms_LeftClick(int x, int y, boolean Recursive)
{
	int LeftClickResult = GS_STARTGAME;

	// 좌표가 보드를 벗어나지 않는 한도 내이면
	if(x >= 0 && y >= 0 && x < gb_SizeX && y < gb_SizeY)
	{
		// 선택한 곳이 선택되지 않거나 ?마크일 경우
		if(gb_MainBoard[y][x].VisualType == BT_HIDE || gb_MainBoard[y][x].VisualType == BT_QUESTION)
		{
			// 첫 클릭이면 지뢰 심음
			if(gb_ClickCount == 0)
				ms_CreateMine(x, y);

			gb_ClickCount++;
			
			// 클릭한 곳의 정보 밝히고 보드 갱신
			gb_MainBoard[y][x].VisualType = BT_UNCOVER;
			MoveCur((x)*2+4, y+2);
			if(Recursive) ms_FormatBoardData(y, x, CR_GRAY);
			else ms_FormatBoardData(y, x, CR_WHITE);
			MoveCur((x)*2+4, y+2);
			
			// 클릭한 곳이 0일 경우 재귀함수
			if(gb_MainBoard[y][x].BoardData == 0)
			{
					ms_LeftClick(x-1, y-1, TRUE); ms_LeftClick(x, y-1, TRUE); ms_LeftClick(x+1, y-1, TRUE); ms_LeftClick(x-1, y, TRUE);
					ms_LeftClick(x+1, y, TRUE); ms_LeftClick(x-1, y+1, TRUE); ms_LeftClick(x, y+1, TRUE); ms_LeftClick(x+1, y+1, TRUE);
			}

			// 클릭한 곳이 지뢰일 경우 게임 오버
			else if(gb_MainBoard[y][x].BoardData == 9)
			{
				LeftClickResult = GS_LOSE;
			}
		}
	}

	return LeftClickResult; // 여기에 GS_LOSE 직접 리턴 금지
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
		// 주변 깃발의 수를 카운트
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

		if(gb_Testmode) { TypeBox("", CR_WHITE, CR_BLACK); printf("%d개의 깃발 감지됨.", FlagCounter); } // ONLY FOR TEST

		// 주변 깃발의 수와 선택된 좌표의 번호가 같으면 주변의 BT_HIDE 를 왼쪽 클릭
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

					if(DualClickResult == GS_LOSE) // 한번이라도 GS_LOSE값이 나왔으면 듀얼클릭의 최종 리턴값은 GS_LOSE
						ReturnLose = TRUE;
					
					// MoveCur((j)*2+4, i+2); ms_FormatBoardData(j, i, CR_GOLD); MoveCur((j)*2+4, y+i); // ONLY FOR TEST
				}
			}
		}

		else { TypeBox("주변 깃발의 수가 해당 위치의 수와 같지 않습니다.", CR_WHITE, CR_BLACK); BipSound(100, 100, gb_Soundmode); }
	}

	if(ReturnLose) return GS_LOSE;
	else return DualClickResult; // 여기에 GS_LOSE 직접 리턴 금지
}

void ms_CreateMine(int x, int y)
{
	int i, j, k;
	int MineLeft = gb_Mines;

	// 판 크기가 지뢰수보다 작으면 지뢰수 재조정
	if((gb_SizeX * gb_SizeY) < gb_Mines)
		gb_Mines = (gb_SizeX * gb_SizeY) - 1;
	
	// 지뢰 심기
	while(MineLeft != 0)
	{
		j = Random_Int(0, gb_SizeX-1);
		k = Random_Int(0, gb_SizeY-1);
		
		if((gb_MainBoard[k][j].BoardData == 0) && (j != x || k != y)) // 텅 빈 보드 공간이고 첫선택 지점이 아니면
		{
			gb_MainBoard[k][j].BoardData = 9; // 지뢰 심음
			MineLeft--;
		}
	}

	//gb_MainBoard[0][0].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[0][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][0].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[0][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[(gb_SizeY-1)/2][0].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[(gb_SizeY-1)/2][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[(gb_SizeY-1)/2][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST


	// 지뢰 주변 숫자 증가
	for(i=0; i<=gb_SizeY-1; i++)
	{
		for(j=0; j<=gb_SizeX-1; j++)
		{
			if(gb_MainBoard[i][j].BoardData == 9) // 지뢰 발견하면
			{
				if(i==0 && j==0) { ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 좌측 상단이면
				else if(i==0 && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); } // 우측 상단이면
				else if(i==gb_SizeY-1 && j==0) { ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j+1, i); } // 좌측 하단이면
				else if(i==gb_SizeY-1 && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j-1, i); } // 우측 하단이면
				else if(i==0 && (j>0 && j<gb_SizeX-1)) { ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j-1, i+1);
														 ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 모서리 제외 상단
				else if((i>0 && i<gb_SizeY-1) && j==0) { ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j+1, i);
														 ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 모서리 제외 좌측
				else if((i>0 && i<gb_SizeY-1) && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j-1, i);
																  ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); } // 모서리 제외 우측
				else if(i==gb_SizeY-1 && (j>0 && j<gb_SizeX-1)) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1);
																  ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j+1, i); } // 모서리 제외 하단
				else { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j-1, i);
					   ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 그외 중앙
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

int ms_CheckWin() // 승리 조건 판단 함수
{
	int i, j;
	int BlockCounter = 0;

	// 모든 보드를 검사하여 지뢰가 아닌 부분이 밝혀졌는가를 확인
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


// 도움말 박스 영역에 최대 76byte길이 텍스트를 출력해주는 함수
void TypeBox(char String[76], int ColorFont, int ColorBack)
{
	EraseBox();

	MoveCur(2, 22);	
	BCprintf(String, ColorFont, ColorBack);
}

// 도움말 박스 영역을 공백으로 지우는 함수
void EraseBox()
{
	MoveCur(2, 22);
	printf("                                                                            ");
}

// 색상 정보를 받아 문자를 출력하는 함수. 색상 사용을 안할시 기본 콘솔 색으로 출력
void BCprintf(char *String, int FontCR, int BackCR)
{
	if(gb_Colormode)
		Cprintf(String, FontCR, BackCR);
	
	else
		printf("%s", String);
}