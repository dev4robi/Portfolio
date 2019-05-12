/************************************************************************/
/* [SUDOKU game using C++ language]                                     */
/*                                                                      */
/*  # Produced by "2010305049 Lee_Tae_Hoon"                             */
/*																		*/
/*	# 2011/5/31 : 1.00a	(alpha test)	                                */
/*  # 2011/6/1 : 1.01b (beta)                                           */
/*	# 2011/6/3 : 1.11c (update setup)									*/
/*	# 2011/6/7 : 1.22d (save setup data)   								*/
/*	# 2011/6/9 : 1.32e (save play data)    								*/
/*	# The last update: 2011/6/10 (1.43f - bring map data)				*/
/*									     	     ┏┓  ┏━━┓┏┓┏┓	*/
/*												 ┃┗┓┗┓┏┛┃┣┫┃	*/
/*												 ┗━┛  ┗┛  ┗┛┗┛	*/
/************************************************************************/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <Turboc.h>
#include <time.h>
using namespace std;

// save global text
const char GLOBALTEXT_0[] = "                             ";//
const char GLOBALTEXT_1[] = "[NOTICE] 변경할 수 없습니다."; //
const char GLOBALTEXT_2[] = "[NOTICE] 성공하셨습니다!!";	//
const char Version[] = "1.43f";
// version: x.yz(a) x: main version, y: improve function, z: fix bug, (a): number of update

// save key value
const int Left = 75;
const int Right = 77;
const int Up = 72;
const int Down = 80;
const int Enter = 13;
const int ESC = 27;

// Tip :: color value
//	BLACK = 0 / BLUE = 1 / GREEN = 2 / CYAN = 3 / RED = 4 / MAGENTA = 5 / PURPLE = 5 / BROWN = 6
//	LIGHTGRAY = 7 / GRAY = 8 /LIGHTBLUE = 9 / LIGHTGREEN = 10 / LIGHTCYAN = 11 / LIGHTRED = 12 / LIGHTMAGENTA = 13
//	LIGHTPURPLE = 13 / YELLOW = 14 WHITE = 15

// Sudoku class
class Sudoku
{
	private:
		BOOL WinGame;			// TRUE: Win and Gameover & FALSE: During Game
		int MainBoard[9][9];	// Value of Board by Square
		BOOL ConstBoard[9][9];	// TRUE: Const & FALSE: Changeable
		int CposX, CposY;		// CurrentPosition X, Y
		int MaxStage;			// Number of maps
		int NumberCounter[10];	// Count Number
		
		// use for timer
		int SaveTime;			// Save time
		int TimeCheck;			// Time Check

		// use for test
		BOOL TestModeON;		// TestMode

		// use for setup
		int SetUp_GameLevel;		// 0: Easy, 1: Normal, 2: Extreme, 3: Random
		int SetUp_PosColor;			// PosBackground color
		int SetUp_ConstColor;		// Const Number color
		BOOL SetUp_Sound;			// sound use TRUE: use, FALSE: unused

	public:
		Sudoku();					 // Init MemberValue
		~Sudoku();					 // Clear
		void InitializeData();		 // Initialize Data
		void ShowMainMenu();		 // Draw MainMenu
		int SelectMenu();			 // Select MenuAction
		BOOL Playing();				 // During Game
		void Mapping(int CheckSave); // Load Map data to MainBoard & ConstBoard
		BOOL Win();					 // Check logical end of game
		void ShowStatus();			 // Show coordinate, count of number etc...
		void ShowTimer();			 // Show timer
		void Setup();				 // DrawSetup
		void HowTo();				 // Show How to play
		void FormatNumber(int FN_CposX, int FN_CposY);		// Change number to special character
		void DrawBoard(int DB_CposX, int DB_CposY, color Font, color BackGround);	// Change Mark of Board
		BOOL CheckSetupDat();	// Check SaveData value
		BOOL CheckPlayDat();	// Check PlayData value
};

Sudoku::Sudoku()
{
	// basic initialize
	MaxStage = 4;			// Note : If you add or sub map. you must change member variable :: MaxStage
	BOOL WinGame = FALSE;
	TestModeON = FALSE;
	CposX = 0;
	CposY = 0;
	SaveTime = TimeCheck = 0;

	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
		{
			MainBoard[i][j] = 0;
			ConstBoard[i][j] = FALSE;
			NumberCounter[i] = 0;
		}

	NumberCounter[9] = 0;

	// setup initialize
	SetUp_GameLevel = 3;	// random
	SetUp_PosColor = 12;	// lightRed
	SetUp_ConstColor = 8;	// gray
	SetUp_Sound = TRUE;		// use

	// bring save file
	FILE *SetupData = fopen("SetupDat.sdk", "r");
	FILE *MapData = fopen("MapDat.sdk", "r");
	
	// check file (have)
	if(SetupData != NULL)
		fscanf(SetupData, "%d %d %d %d", &SetUp_ConstColor, &SetUp_Sound, &SetUp_PosColor, &SetUp_GameLevel);

	// not have file
	else
	{
		SetupData = fopen("SetupDat.sdk", "w");
		fprintf(SetupData, "%d %d %d %d", SetUp_ConstColor, SetUp_Sound, SetUp_PosColor, SetUp_GameLevel);
	}

	// no have mapdat.sdk
	if(MapData == NULL)
	{
		MapData = fopen("MapDat.sdk", "w+");
		
		// default "max stage"
		fprintf(MapData, "(4)\n");

		// default "MapID/level/mapping" data
		fprintf(MapData, "[1] <0> 0 4 0 6 0 0 0 2 0 5 0 0 0 0 2 4 0 0 0 0 7 0 3 0 0 8 0 ");
		fprintf(MapData, "4 0 0 0 8 0 0 5 0 0 0 9 5 0 4 2 0 0 0 3 0 0 1 0 0 0 9 0 9 0 0 7 0 8 0 0 ");
		fprintf(MapData, "0 0 2 8 0 0 0 0 3 0 1 0 0 0 6 0 9 0 \n"); // Map1 (Easy)

		fprintf(MapData, "[2] <1> 5 8 0 0 2 7 6 0 0 0 1 0 4 0 0 0 0 0 4 0 0 0 0 0 1 0 0 ");
		fprintf(MapData, "1 0 5 0 0 0 0 4 0 0 9 0 0 0 0 0 2 0 0 4 0 0 0 0 7 0 1 0 0 6 0 0 0 0 0 8 ");
		fprintf(MapData, "0 0 0 0 0 9 0 7 0 0 0 1 2 5 0 0 3 9 \n"); // Map2 (Normal)
		
		fprintf(MapData, "[3] <1> 6 5 3 0 0 0 7 0 0 0 0 0 0 0 9 0 0 0 8 0 4 0 5 0 0 0 3 ");
		fprintf(MapData, "9 0 0 0 1 7 3 0 0 0 0 5 0 3 0 8 0 0 0 0 7 2 9 0 0 0 5 4 0 0 0 7 0 2 0 6 ");
		fprintf(MapData, "0 0 0 8 0 0 0 0 0 0 0 6 0 0 0 1 5 7 \n"); // Map3 (Normal)

		fprintf(MapData, "[4] <2> 7 0 0 0 0 0 0 1 2 9 0 2 0 0 6 4 0 0 0 0 0 0 8 0 0 0 0 ");
		fprintf(MapData, "0 0 0 0 3 9 2 0 0 4 0 0 0 0 0 0 0 8 0 0 8 7 5 0 0 0 0 0 0 0 0 4 0 0 0 0 ");
		fprintf(MapData, "0 0 6 5 0 0 8 0 1 1 3 0 0 0 0 0 0 5 \n"); // Map4 (Extreme)
	}

	else
		fscanf(MapData, "(%d)", &MaxStage);
		
	// close all file
	fclose(SetupData);
	fclose(MapData);
}

Sudoku::~Sudoku()
{
}

void Sudoku::InitializeData()
{
	// basic initialize
	WinGame = FALSE;
	CposX = 0;
	CposY = 0;
	SaveTime = TimeCheck = 0;

	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
		{
			MainBoard[i][j] = 0;
			ConstBoard[i][j] = FALSE;
			NumberCounter[i] = 0;
		}
	
	NumberCounter[9] = 0;
	
	// note: do not init setup
}

void Sudoku::ShowMainMenu()
{
	system("cls");
	cout << "\n  */*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*" << endl;
	cout << "  /                                                               /" << endl;
	cout << "  *                        스도쿠(Sudoku)                         *" << endl;
	cout << "  /                                      ver " << Version << "                /" << endl;
	cout << "  *                                                               *" << endl;
	cout << "  /     ☞ 1. 새로 하기                                           /" << endl;
	cout << "  *        2. 이어 하기                                           *" << endl;
	cout << "  /        3. 하는법                                              /" << endl;
	cout << "  *        4. 설정                                                *" << endl;
	cout << "  /        5. 게임 종료                                           /" << endl;
	cout << "  *                                                               *" << endl;
	cout << "  /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/" << endl;

	SetColor(LIGHTGRAY, NULL);
}

int Sudoku::SelectMenu()
{
	int CurrentSelect = 1;
	char KeyValue = '\0';
	FILE *PlaySaveData = fopen("PlayDat.sdk", "r");
	
	// have play data
	if(PlaySaveData != NULL)
	{

		CurrentSelect = 2;
		gotoxy(8, 6); cout << "  ";
		gotoxy(8, 7); cout << "☞";
		fclose(PlaySaveData);
	}

	while(KeyValue != Enter)
	{
		if(kbhit())
		{
			KeyValue = getch();

			switch(KeyValue)
			{
				case Up:
					if(CurrentSelect > 1)
						CurrentSelect--;
					break;

				case Down:
					if(CurrentSelect <5)
						CurrentSelect++;
					break;
			}			

			gotoxy(8, CurrentSelect+4);
			cout << "   ";
			gotoxy(8, CurrentSelect+6);
			cout << "   ";
			gotoxy(8, CurrentSelect+5);
			cout << "☞";
		}
	}

	return CurrentSelect;
}

BOOL Sudoku::Playing()
{
	int NoticeSelect = 0; // 0: Standby, 1~2: Quitgame
	char KeyValue = '\0';
	time_t GetTime;
	struct tm *CurrentTime;
	int TimeerSleep = 0;

	// initialization
	CposX = 0;
	CposY = 0;
	gotoxy(2, 1);
	ShowStatus();
	time(&GetTime);
	CurrentTime = localtime(&GetTime);

	// Draw Board
	system("cls");	
	cout << "┏───┳───┳───┓" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "┣───╋───╋───┫" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "┣───╋───╋───┫" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "│○○○│○○○│○○○│" << endl;
	cout << "┗───┻───┻───┛" << endl;
	
	SetColor(WHITE, NULL);
	
	// DrawBoard::Current Position
	gotoxy(28, 1);
	cout << "┌─────┐";
	gotoxy(28, 2);
	cout << "│X: " << CposX+1 << "  Y: " << CposY+1 << "│";
	gotoxy(28, 3);
	cout << "└─────┘";
	
	// DrawBoard::Timer
	gotoxy(42, 1);
	cout << "┌──────────┐";
	gotoxy(42, 2);
	cout << "│                    │";
	gotoxy(42, 3);
	cout << "└──────────┘";
	
	// DrawBoard::Counter
	gotoxy(28, 4);
	cout << "┌─────────────────┐";
	gotoxy(28, 5);
	printf("│○: %.2d ①: %.2d ②: %.2d ③: %.2d ④: %.2d│", NumberCounter[0], NumberCounter[1], NumberCounter[2],
		NumberCounter[3], NumberCounter[4]);
	gotoxy(28, 6);
	printf("│⑤: %.2d ⑥: %.2d ⑦: %.2d ⑧: %.2d ⑨: %.2d│", NumberCounter[5], NumberCounter[6], NumberCounter[7],
		NumberCounter[8], NumberCounter[9]);
	gotoxy(28, 7);
	cout << "└─────────────────┘";
	
	// DrawBoard::Notice
	gotoxy(28, 9);
	cout << "┌─────────────────┐";
	gotoxy(28, 10);
	cout << "│                                  │";
	gotoxy(28, 11);
	cout << "└─────────────────┘";
	
	SetColor(LIGHTGRAY, NULL);

	// uncover all place
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
			DrawBoard(i, j, LIGHTGRAY, NULL);
	
	// begin playing (main)
	while(WinGame != TRUE)
	{
		TimeerSleep++;

		if(TimeerSleep > 5000)
		{
			ShowTimer();
			TimeerSleep = 0;
		}

		if(kbhit())
		{
			SetColor(LIGHTGRAY, NULL);
			
			KeyValue = getch();
			
			// normal state
			if(NoticeSelect == 0)
			{
				// init notice area
				gotoxy(28, 10);
				SetColor(WHITE, NULL);
				cout << "│                                  │";
				SetColor(LIGHTGRAY, NULL);

				switch(KeyValue)
				{				
					case ESC:
					{
						NoticeSelect = 1;

						if(SetUp_Sound == TRUE)
							cout << "\a";

						gotoxy(30, 10); cout << "저장후종료? ☞ 1. 아니오    2. 예 ";
					} break;

					case Up:
						if(CposY>0)
						{
							DrawBoard(CposX, CposY, LIGHTGRAY, NULL);
							CposY--;
						}	
						break;
					
					case Down:
						if(CposY<8)
						{
							DrawBoard(CposX, CposY, LIGHTGRAY, NULL);
							CposY++;
						}
						break;

					case Right:
						if(CposX<8)
						{
							DrawBoard(CposX, CposY, LIGHTGRAY, NULL);
							CposX++;
						}
						break;

					case Left:
						if(CposX>0)
						{
							DrawBoard(CposX, CposY, LIGHTGRAY, NULL);
							CposX--;
						}
						break;

					case Enter:
						if(ConstBoard[CposY][CposX] == FALSE)
						{
							if(MainBoard[CposY][CposX] >= 9)
							{
									MainBoard[CposY][CposX] = 0;
									NumberCounter[9]--;
									NumberCounter[0]++;
							}

							else
							{
								NumberCounter[MainBoard[CposY][CposX]]--;
								MainBoard[CposY][CposX]++;
								NumberCounter[MainBoard[CposY][CposX]]++;
							}

							WinGame = Win();
						}

						else
						{
							if(SetUp_Sound == TRUE)
								cout << "\a";

							gotoxy(30, 10);
							cout << GLOBALTEXT_1;
						}
				}
			}

			// save&quit select state
			else
			{
				switch(KeyValue)
				{
					case Right:
						if(NoticeSelect == 1 || NoticeSelect == 0)
						{
							NoticeSelect = 2;
							gotoxy(42, 10); cout << "  ";
							gotoxy(55, 10); cout << "☞";
						} break;

					case Left:
						if(NoticeSelect == 2 || NoticeSelect == 0)
						{
							NoticeSelect = 1;
							gotoxy(55, 10); cout << "  ";
							gotoxy(42, 10); cout << "☞";
						} break;

					case Enter:
						if(NoticeSelect == 1) // cancel
						{
							NoticeSelect = 0;
							gotoxy(30, 10); cout << "                                  ";
						}

						else if(NoticeSelect == 2) // ok
						{
							// save data to file
							FILE *PlayData = fopen("PlayDat.sdk", "w+");

							// board data
							for(int i=0; i<9; i++)
							{
								for(int j=0; j<9; j++)
								{
									fprintf(PlayData, "%d ", MainBoard[i][j]);
									fprintf(PlayData, "%d ", ConstBoard[i][j]);
								}

								fprintf(PlayData, "%d ", NumberCounter[i]);
							}

							fprintf(PlayData, "%d ", NumberCounter[9]);

							// time data
							fprintf(PlayData, "%d ", SaveTime);

							fclose(PlayData);

							NoticeSelect = 0;
							return FALSE;
						}
					}
			}

		ShowStatus();
		DrawBoard(CposX, CposY, NULL, SetUp_PosColor);
		}
	}

	return TRUE;
}

void Sudoku::DrawBoard(int DB_CposX, int DB_CposY, color Font, color BackGround)
{
	int DB_VisualPosX = DB_CposX, DB_VisualPosY = DB_CposY;

	if(DB_CposX == 0) DB_VisualPosX = 2;	   else if(DB_CposX == 1) DB_VisualPosX = 4;
	else if(DB_CposX == 2) DB_VisualPosX = 6;  else if(DB_CposX == 3) DB_VisualPosX = 10;
	else if(DB_CposX == 4) DB_VisualPosX = 12; else if(DB_CposX == 5) DB_VisualPosX = 14;
	else if(DB_CposX == 6) DB_VisualPosX = 18; else if(DB_CposX == 7) DB_VisualPosX = 20;
	else if(DB_CposX == 8) DB_VisualPosX = 22; else DB_VisualPosX = 2;

	if(DB_CposY == 0) DB_VisualPosY = 1;	   else if(DB_CposY == 1) DB_VisualPosY = 2;
	else if(DB_CposY == 2) DB_VisualPosY = 3;  else if(DB_CposY == 3) DB_VisualPosY = 5;
	else if(DB_CposY == 4) DB_VisualPosY = 6;  else if(DB_CposY == 5) DB_VisualPosY = 7;
	else if(DB_CposY == 6) DB_VisualPosY = 9;  else if(DB_CposY == 7) DB_VisualPosY = 10;
	else if(DB_CposY == 8) DB_VisualPosY = 11; else DB_VisualPosY = 1;

	gotoxy(DB_VisualPosX, DB_VisualPosY);

	if(ConstBoard[DB_CposY][DB_CposX] != TRUE)
		SetColor(Font, BackGround);

	else
		SetColor(SetUp_ConstColor, BackGround);

		FormatNumber(DB_CposX, DB_CposY);
		SetColor(LIGHTGRAY, NULL);
}

void Sudoku::FormatNumber(int FN_CposX, int FN_CposY)
{	
	switch(MainBoard[FN_CposY][FN_CposX])
	{
		case 0:
			cout << "○"; break;
		
		case 1:
			cout << "①"; break;
		  
		case 2:
			cout << "②"; break;
		
		case 3:
			cout << "③"; break;
			  
		case 4:
			cout << "④"; break;
				
		case 5:
			cout << "⑤"; break;
				  
		case 6:
			cout << "⑥"; break;
				
		case 7:
			cout << "⑦"; break;
					  
		case 8:
			cout << "⑧"; break;
						
		case 9:
			cout << "⑨"; break;
						  
		default:
			cout << "??";
	}
}

void Sudoku::Mapping(int CheckSave)
{
	int Index = 0;
	int Counter = 0;
	int SelectedMap = 0;
	FILE *PlayData = fopen("PlayDat.sdk", "r");

	int Map[ ][81] = {{0,4,0,6,0,0,0,2,0,
			     	   5,0,0,0,0,2,4,0,0,
					   0,0,7,0,3,0,0,8,0,
					   4,0,0,0,8,0,0,5,0,
					   0,0,9,5,0,4,2,0,0,
					   0,3,0,0,1,0,0,0,9,
					   0,9,0,0,7,0,8,0,0,
					   0,0,2,8,0,0,0,0,3,
					   0,1,0,0,0,6,0,9,0 }, // Map0 (Easy)

					{  5,8,0,0,2,7,6,0,0,
					   0,1,0,4,0,0,0,0,0,
				       4,0,0,0,0,0,1,0,0,
					   1,0,5,0,0,0,0,4,0,
					   0,9,0,0,0,0,0,2,0,
					   0,4,0,0,0,0,7,0,1,
					   0,0,6,0,0,0,0,0,8,
					   0,0,0,0,0,9,0,7,0,
					   0,0,1,2,5,0,0,3,9 }, // Map1 (Normal)

					{  7,0,0,0,0,0,0,1,2,
					   9,0,2,0,0,6,4,0,0,
					   0,0,0,0,8,0,0,0,0,
					   0,0,0,0,3,9,2,0,0,
					   4,0,0,0,0,0,0,0,8,
					   0,0,8,7,5,0,0,0,0,
					   0,0,0,0,4,0,0,0,0,
					   0,0,6,5,0,0,8,0,1,
					   1,3,0,0,0,0,0,0,5 }, // Map2 (Extreme)

					{  6,5,3,0,0,0,7,0,0,
					   0,0,0,0,0,9,0,0,0,
					   8,0,4,0,5,0,0,0,3,
					   9,0,0,0,1,7,3,0,0,
					   0,0,5,0,3,0,8,0,0,
					   0,0,7,2,9,0,0,0,5,
					   4,0,0,0,7,0,2,0,6,
					   0,0,0,8,0,0,0,0,0,
					   0,0,6,0,0,0,1,5,7 } // Map3 (Normal)

					// You can add "default map" here to as like as upper style.
					// * Caution : If you add or sub map. you must change member variable :: MaxStage
					//			   using 생성자.
							 			 
										 };

	srand(GetTickCount());

	// no have playdat.sdk
	if(PlayData == NULL && CheckSave == 2)
	{
		CheckSave = 1;

		system("cls");
		SetColor(YELLOW, NULL);
		gotoxy(10, 6); cout << "저장된 데이터가 없으므로 게임을 새로 시작합니다.";
		gotoxy(10, 7); cout << "시작하려면 엔터키를 눌러주십시오.";
		getch();
	}

	// select "new game"
	if(CheckSave == 1)
	{	
		remove("PlayDat.sdk"); // Tip : If file was open, It can't remove

		if(TestModeON == FALSE && SetUp_GameLevel == 3)
			SelectedMap = rand() % (MaxStage);

		else
			SelectedMap = 3; // only for test;
		
		for(int i=0; i<81; i++)
		{
			MainBoard[Index][Counter] = Map[SelectedMap][i];

			switch(Map[SelectedMap][i])
			{
				case 0:
					NumberCounter[0]++; break;

				case 1:
					NumberCounter[1]++; break;

				case 2:
					NumberCounter[2]++; break;

				case 3:
					NumberCounter[3]++; break;

				case 4:
					NumberCounter[4]++; break;
					
				case 5:
					NumberCounter[5]++; break;
					
				case 6:
					NumberCounter[6]++; break;
					
				case 7:
					NumberCounter[7]++; break;
					
				case 8:
					NumberCounter[8]++; break;
					
				case 9:
					NumberCounter[9]++; break;
			}			
				
			if(MainBoard[Index][Counter] != 0)
				ConstBoard[Index][Counter] = TRUE;

			Counter++;

			if(Counter == 9)
			{
				Index++;
				Counter = 0;
			}
		}
	}

	// select "continue past game"
	if(CheckSave == 2) 
	{
		fclose(PlayData);

		if(!(CheckPlayDat()))
		{
			system("cls");
			SetColor(LIGHTRED, NULL);
			gotoxy(20, 6); cout << "세이브 데이터가 손상되었습니다.";
			gotoxy(20, 7); cout << "게임을 다시 실행시켜 주십시오.";
			gotoxy(20, 8); cout << "손실된 데이터는 자동으로 삭제됩니다.";
			getch();
			
			exit(0);
		}
		
		FILE *PlayData = fopen("PlayDat.sdk", "r");

		for(int i=0; i<9; i++)
		{
			for(int j=0; j<9; j++)
			{
				fscanf(PlayData, "%d ", &MainBoard[i][j]);
				fscanf(PlayData, "%d ", &ConstBoard[i][j]);
			}

			fscanf(PlayData, "%d ", &NumberCounter[i]);
		}

		fscanf(PlayData, "%d ", &NumberCounter[9]);
		fscanf(PlayData, "%d ", &SaveTime);

		fclose(PlayData);
	}
}

BOOL Sudoku::Win()
{
	BOOL CheckWin[3]= {FALSE, FALSE, FALSE};	// Horizontal, Vertical, Square
	BOOL CheckNumber[9] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}; // 1,2,3,4,5,6,7,8,9
	BOOL CheckLine[9] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}; // ─,│,□

	if(NumberCounter[0] == 0 && NumberCounter[1] == 9 && NumberCounter[2] == 9 && NumberCounter[3] == 9 && NumberCounter[4] == 9 &&
	   NumberCounter[5] == 9 && NumberCounter[6] == 9 && NumberCounter[7] == 9 && NumberCounter[8] == 9 && NumberCounter[9] == 9)
	{
		// Horizontal Check
		for(int i=0; i<9; i++)
		{
			for(int z=0; z<9; z++)
				CheckNumber[z] = FALSE;

			for(int j=0; j<9; j++)
			{
				switch(MainBoard[i][j])
				{
					case 1:
						if(CheckNumber[0] != TRUE)
							CheckNumber[0] = TRUE;

						else return FALSE; break;

					case 2:
						if(CheckNumber[1] != TRUE)
							CheckNumber[1] = TRUE;

						else return FALSE; break;

					case 3:
						if(CheckNumber[2] != TRUE)
							CheckNumber[2] = TRUE;

						else return FALSE; break;

					case 4:
						if(CheckNumber[3] != TRUE)
							CheckNumber[3] = TRUE;

						else return FALSE; break;

					case 5:
						if(CheckNumber[4] != TRUE)
							CheckNumber[4] = TRUE;

						else return FALSE; break;

					case 6:
						if(CheckNumber[5] != TRUE)
							CheckNumber[5] = TRUE;

						else return FALSE; break;

					case 7:
						if(CheckNumber[6] != TRUE)
							CheckNumber[6] = TRUE;

						else return FALSE; break;

					case 8:
						if(CheckNumber[7] != TRUE)
							CheckNumber[7] = TRUE;

						else return FALSE; break;

					case 9:
						if(CheckNumber[8] != TRUE)
							CheckNumber[8] = TRUE;

						else return FALSE; break;
				}
			}
	
			if(CheckNumber[0] && CheckNumber[1] && CheckNumber[2] && CheckNumber[3] && CheckNumber[4] &&
			   CheckNumber[5] && CheckNumber[6] && CheckNumber[7] && CheckNumber[8])
			{
				CheckLine[i] = TRUE;
			}
			
			for(z=0; z<9; z++)
				CheckNumber[z] = FALSE;
		} // end of double for syntax(Horizontal)

		if(CheckLine[0] && CheckLine[1] && CheckLine[2] && CheckLine[3] && CheckLine[4] &&
		   CheckLine[5] && CheckLine[6] && CheckLine[7] && CheckLine[8])
		{
			CheckWin[0] = TRUE;
		}

		else
			CheckWin[0] = FALSE;

		for(int z=0; z<9; z++)
			CheckLine[z] = FALSE;

		// Vertical Check
		for(i=0; i<9; i++)
		{
			for(int z=0; z<9; z++)
				CheckNumber[z] = FALSE;

			for(int j=0; j<9; j++)
			{
				switch(MainBoard[j][i])
				{
					case 1:
						if(CheckNumber[0] != TRUE)
							CheckNumber[0] = TRUE;
						
						else return FALSE; break;
						
					case 2:
						if(CheckNumber[1] != TRUE)
							CheckNumber[1] = TRUE;
						
						else return FALSE; break;
						
					case 3:
						if(CheckNumber[2] != TRUE)
							CheckNumber[2] = TRUE;
						
						else return FALSE; break;
						
					case 4:
						if(CheckNumber[3] != TRUE)
							CheckNumber[3] = TRUE;
						
						else return FALSE; break;
						
					case 5:
						if(CheckNumber[4] != TRUE)
							CheckNumber[4] = TRUE;
						
						else return FALSE; break;
						
					case 6:
						if(CheckNumber[5] != TRUE)
							CheckNumber[5] = TRUE;
						
						else return FALSE; break;
						
					case 7:
						if(CheckNumber[6] != TRUE)
							CheckNumber[6] = TRUE;
						
						else return FALSE; break;
						
					case 8:
						if(CheckNumber[7] != TRUE)
							CheckNumber[7] = TRUE;
						
						else return FALSE; break;
						
					case 9:
						if(CheckNumber[8] != TRUE)
							CheckNumber[8] = TRUE;
						
							else return FALSE; break;
				}
			}
		
			if(CheckNumber[0] && CheckNumber[1] && CheckNumber[2] && CheckNumber[3] && CheckNumber[4] &&
			   CheckNumber[5] && CheckNumber[6] && CheckNumber[7] && CheckNumber[8])
			{
				CheckLine[i] = TRUE;
			}

			for(z=0; z<9; z++)
				CheckNumber[z] = FALSE;
		} // end of double for syntax(Vertical)

		if(CheckLine[0] && CheckLine[1] && CheckLine[2] && CheckLine[3] && CheckLine[4] &&
		   CheckLine[5] && CheckLine[6] && CheckLine[7] && CheckLine[8])
		{
			CheckWin[1] = TRUE;
		}

		else
			CheckWin[1] = FALSE;

		for(z=0; z<9; z++)
			CheckLine[z] = FALSE;

		// Square Check
		int AdderX = 0;
		int AdderY = 0;

		for(int k=0; k<9; k++)
		{
			for(int z=0; z<9; z++)
				CheckNumber[z] = FALSE;

			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
				{
					switch(MainBoard[i+AdderY][j+AdderX])
					{
					case 1:
						if(CheckNumber[0] != TRUE)
							CheckNumber[0] = TRUE;
						
						else return FALSE; break;
						
					case 2:
						if(CheckNumber[1] != TRUE)
							CheckNumber[1] = TRUE;
						
						else return FALSE; break;
						
					case 3:
						if(CheckNumber[2] != TRUE)
							CheckNumber[2] = TRUE;
						
						else return FALSE; break;
						
					case 4:
						if(CheckNumber[3] != TRUE)
							CheckNumber[3] = TRUE;
						
						else return FALSE; break;
						
					case 5:
						if(CheckNumber[4] != TRUE)
							CheckNumber[4] = TRUE;
						
						else return FALSE; break;
						
					case 6:
						if(CheckNumber[5] != TRUE)
							CheckNumber[5] = TRUE;
						
						else return FALSE; break;
						
					case 7:
						if(CheckNumber[6] != TRUE)
							CheckNumber[6] = TRUE;
						
						else return FALSE; break;
						
					case 8:
						if(CheckNumber[7] != TRUE)
							CheckNumber[7] = TRUE;
						
						else return FALSE; break;
						
					case 9:
						if(CheckNumber[8] != TRUE)
							CheckNumber[8] = TRUE;
						
							else return FALSE; break;	
					}
				}
			} // end of double for syntax(Square)

			AdderX += 3;

			if(AdderX == 9)
			{
				AdderX = 0;
				AdderY += 3;
			}

			if(CheckNumber[0] && CheckNumber[1] && CheckNumber[2] && CheckNumber[3] && CheckNumber[4] &&
				CheckNumber[5] && CheckNumber[6] && CheckNumber[7] && CheckNumber[8])
			{
				CheckLine[k] = TRUE;
			}

			for(z=0; z<9; z++)
				CheckNumber[z] = FALSE;
		} // end of triple for syntax(Square)

		if(CheckLine[0] && CheckLine[1] && CheckLine[2] && CheckLine[3] && CheckLine[4] &&
			CheckLine[5] && CheckLine[6] && CheckLine[7] && CheckLine[8])
		{
			CheckWin[2] = TRUE;
		}
		
		else
			CheckWin[2] = FALSE;
		
		for(z=0; z<9; z++)
			CheckLine[z] = FALSE;
	} // end of main if
	
	else
		return FALSE;

	return (CheckWin[0] && CheckWin[1] && CheckWin[2]);
}

void Sudoku::ShowStatus()
{
	SetColor(WHITE, NULL);
	
	// Current Position
	gotoxy(28, 2);
	cout << "│X: " << CposX+1 << "  Y: " << CposY+1 << "│";

	// Counter
	gotoxy(28, 5);
	printf("│○: %.2d ①: %.2d ②: %.2d ③: %.2d ④: %.2d│", NumberCounter[0], NumberCounter[1], NumberCounter[2],
															   NumberCounter[3], NumberCounter[4]);
	gotoxy(28, 6);
	printf("│⑤: %.2d ⑥: %.2d ⑦: %.2d ⑧: %.2d ⑨: %.2d│", NumberCounter[5], NumberCounter[6], NumberCounter[7],
															   NumberCounter[8], NumberCounter[9]);
	SetColor(LIGHTGRAY, NULL);
}

void Sudoku::ShowTimer()
{
	time_t GetTime;
	struct tm *CurrentTime;

	// initialization
	time(&GetTime);
	CurrentTime = localtime(&GetTime);
	int C_TimeHour = CurrentTime->tm_hour;
	int C_TimeMin = CurrentTime->tm_min;
	int C_TimeSec = CurrentTime->tm_sec;
	int EncodedTime = C_TimeHour * 3600 + C_TimeMin * 60 + C_TimeSec;

	// check time flow
	if(TimeCheck < EncodedTime && SaveTime < 356400)
	{
		SaveTime++;
		TimeCheck = EncodedTime;
	}

	else
		TimeCheck = EncodedTime;

	// decode time
	C_TimeHour = SaveTime / 3600;
	C_TimeMin = (SaveTime - C_TimeHour * 3600) / 60;
	C_TimeSec = (SaveTime - (C_TimeHour * 3600 + C_TimeMin * 60));

	// set maximum
	if(SaveTime > 356400) SaveTime = 356400;

	// draw
	SetColor(WHITE, NULL);
	gotoxy(44, 2);
	printf("    %.2d : %.2d : %.2d    ", C_TimeHour, C_TimeMin, C_TimeSec);

	SetColor(LIGHTGRAY, NULL);
}

void Sudoku::Setup()
{
	int CurrentSelect = 1;
	char KeyValue = '\0';
	
	system("cls");
	cout << "\n  */*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*" << endl; // 1
	cout << "  /                                                               /" << endl;	 // 2
	cout << "  *                          설정(Set-up)                         *" << endl;	 // 3
	cout << "  /                                                               /" << endl;	 // 4
	cout << "  *                                                               *" << endl;	 // 5
	cout << "  /    ☞ 1. 고정수 색상 설정:              2. 소리 사용: ON      /" << endl;	 // 6
	cout << "  *                                                               *" << endl;	 // 7
	cout << "  /       3. 현위치 색상 설정:              4. 난이도: 무작위     /" << endl;	 // 8
	cout << "  *                                                               *" << endl;	 // 9
	cout << "  /       5. 저장후 메인 메뉴로.                                  /" << endl;	 // 10
	cout << "  *                                                               *" << endl;	 // 11
	cout << "  /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/" << endl;	 // 12

	// change setup value
	if(SetUp_Sound != TRUE){gotoxy(58, 6); cout << "OFF";}
	if(SetUp_GameLevel == 0){gotoxy(55, 8); cout << "쉬움  ";}
	else if(SetUp_GameLevel == 1){gotoxy(55, 8); cout << "보통  ";}
	else if(SetUp_GameLevel == 2){gotoxy(55, 8); cout << "어려움";}
	gotoxy(32, 6); SetColor(NULL, SetUp_ConstColor); cout << "  ";
	gotoxy(32, 8); SetColor(NULL, SetUp_PosColor); cout << "  ";	

	// setup main
	while(1)
	{
		if(kbhit())
		{
			KeyValue = getch();
			SetColor(LIGHTGRAY, NULL);
			
			switch(KeyValue)
			{
				case Up:
					if(CurrentSelect == 5)
					{
						CurrentSelect = 3;
						gotoxy(7, 10); cout << "  "; gotoxy(7, 8); cout << "☞";
					}

					else if(CurrentSelect == 3)
					{
						CurrentSelect = 1;
						gotoxy(7, 8); cout << "  "; gotoxy(7, 6); cout << "☞";
					}

					else if(CurrentSelect == 4)
					{
						CurrentSelect = 2;
						gotoxy(41, 8); cout << "  "; gotoxy(41, 6); cout << "☞";
					}
					break;
				
				case Down:
					if(CurrentSelect == 1)
					{
						CurrentSelect = 3;
						gotoxy(7, 6); cout << "  "; gotoxy(7, 8); cout << "☞";
					}

					else if(CurrentSelect == 3)
					{
						CurrentSelect = 5;
						gotoxy(7, 8); cout << "  "; gotoxy(7, 10); cout << "☞";
					}

					else if(CurrentSelect == 2)
					{
						CurrentSelect = 4;
						gotoxy(41, 6); cout << "  "; gotoxy(41, 8); cout << "☞";
					}
					break;

				case Right:
					if(CurrentSelect == 1)
					{
						CurrentSelect = 2;
						gotoxy(7, 6); cout << "  "; gotoxy(41, 6); cout << "☞";
					}

					else if(CurrentSelect == 3)
					{
						CurrentSelect = 4;
						gotoxy(7, 8); cout << "  "; gotoxy(41, 8); cout << "☞";
					}
					break;

				case Left:
					if(CurrentSelect == 2)
					{
						CurrentSelect = 1;
						gotoxy(41, 6); cout << "  "; gotoxy(7, 6); cout << "☞";
					}

					else if(CurrentSelect == 4)
					{
						CurrentSelect = 3;
						gotoxy(41, 8); cout << "  "; gotoxy(7, 8); cout << "☞";
					}
					break;

				case Enter:
					if(CurrentSelect == 1)
					{
						if(SetUp_ConstColor != 15)
							SetUp_ConstColor++;

						else
							SetUp_ConstColor = 0;
						
						gotoxy(32, 6); SetColor(NULL, SetUp_ConstColor); cout << "  ";
					}

					else if(CurrentSelect == 2)
					{
						gotoxy(58, 6); cout << "   "; gotoxy(58, 6);

						if(SetUp_Sound == TRUE)
							cout << "OFF";

						else
							cout << "ON\a";
						
						SetUp_Sound = (!SetUp_Sound);
					}

					else if(CurrentSelect == 3)
					{
						if(SetUp_PosColor != 15)
							SetUp_PosColor++;
						
						else
							SetUp_PosColor = 0;
						
						gotoxy(32, 8); SetColor(NULL, SetUp_PosColor); cout << "  ";
					}

					else if(CurrentSelect == 4)
					{
						gotoxy(55, 8); cout << "      "; gotoxy(55, 8);

						if(SetUp_GameLevel != 3)
							SetUp_GameLevel++;

						else
							SetUp_GameLevel = 0;

						switch(SetUp_GameLevel)
						{
							case 0:
								cout << "쉬움"; break;

							case 1:
								cout << "보통"; break;

							case 2:
								cout << "어려움"; break;

							case 3:
								cout << "무작위";
						}
					}
					
					else if(CurrentSelect == 5)
					{
						FILE *SetupData = fopen("SetupDat.sdk", "w+");
						fprintf(SetupData, "%d %d %d %d", SetUp_ConstColor, SetUp_Sound, SetUp_PosColor, SetUp_GameLevel);
						fclose(SetupData);
						return;
					}
			}					
		}
	}
}

void Sudoku::HowTo()
{
	system("cls");
	cout << "\n  */*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*" << endl; // 1
	cout << "  /                                                               /" << endl;	 // 2
	cout << "  *                       하는법(How to play)                     *" << endl;	 // 3
	cout << "  /                                                               /" << endl;	 // 4
	cout << "  *                                                               *" << endl;	 // 5
	cout << "  /                                                               /" << endl;	 // 6
	cout << "  *                                                               *" << endl;	 // 7
	cout << "  /                                                               /" << endl;	 // 8
	cout << "  *                                                               *" << endl;	 // 9
	cout << "  /                                                    Next(1/6)  /" << endl;	 // 10
	cout << "  *                                                               *" << endl;	 // 11
	cout << "  /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/" << endl;	 // 12

	gotoxy(7, 6); cout << "스도쿠는 가로 세로 9칸인 정사각형 모양의 빈 칸에 1부터";
	gotoxy(7, 7); cout << "9까지 아홉 개의 숫자를 적당히 넣어 다음 세 조건을 만족";
	gotoxy(7, 8); cout << "하는 것이다."; getch();

	gotoxy(3, 6); cout << "                                                               ";
	gotoxy(3, 7); cout << "                                                               ";
	gotoxy(3, 8); cout << "                                                               ";
	
	gotoxy(60, 10); cout << "2";
	gotoxy(7, 6); cout << "1. 어떤 가로줄에도 같은 숫자가 나타나지 않는다. 바꿔 말";
	gotoxy(7, 7); cout << "하면, 어떤 가로줄에도 1부터 9까지 아홉 개의 숫자가 모두";
	gotoxy(7, 8); cout << "나타난다."; getch();

	gotoxy(3, 6); cout << "                                                               ";
	gotoxy(3, 7); cout << "                                                               ";
	gotoxy(3, 8); cout << "                                                               ";

	gotoxy(60, 10); cout << "3";
	gotoxy(7, 6); cout << "2. 어떤 세로줄에도 같은 숫자가 나타나지 않는다. 즉, 어떤";
	gotoxy(7, 7); cout << "세로줄에도 아홉 개의 숫자가 모두 나타난다.";
	getch();

	gotoxy(3, 6); cout << "                                                               ";
	gotoxy(3, 7); cout << "                                                               ";

	gotoxy(60, 10); cout << "4";
	gotoxy(7, 6); cout << "3. 굵은 테두리를 두른, 가로 세로 3칸인 작은 정사각형에도";
	gotoxy(7, 7); cout << "같은 숫자가 나타나지 않는다. 즉, 아홉 개의 숫자가 모두 나";
	gotoxy(7, 8); cout << "타난다.";
	gotoxy(7, 10); cout << "＊출처: 네이버캐스트 - 오늘의 과학"; getch();

	gotoxy(3, 6); cout << "                                                               ";
	gotoxy(3, 7); cout << "                                                               ";
	gotoxy(3, 8); cout << "                                                               ";
	gotoxy(3, 10); cout << "                                                    ";

	gotoxy(60, 10); cout << "5";
	gotoxy(26, 3); cout << "정보(Information)  ";
	gotoxy(7, 6); cout << "＊새로 시작 : 게임을 새로 시작합니다. 만약, 저장되었던";
	gotoxy(7, 7); cout << "              게임이 있다면 저장된 게임을 삭제합니다.";
	gotoxy(7, 8); cout << "＊이어 하기 : 기존에 하던 게임을 이어합니다.";
	gotoxy(7, 9); cout << "＊설정 : 여러가지 설정을 할 수 있습니다."; getch();

	gotoxy(3, 6); cout << "                                                               ";
	gotoxy(3, 7); cout << "                                                               ";
	gotoxy(3, 8); cout << "                                                               ";
	gotoxy(3, 9); cout << "                                                               ";
	gotoxy(3, 10); cout << "                                                    ";

	gotoxy(60, 10); cout << "6";
	gotoxy(26, 3); cout << "정보(Information)  ";
	gotoxy(7, 6); cout << "＊사용키: [→], [↑], [↓], [←], [ENTER], [ESC]";
	gotoxy(7, 7); cout << "＊게임 버전: " << Version;
	gotoxy(7, 9); cout << "＊제작: 2010305049 이태훈"; getch();
}

BOOL Sudoku::CheckSetupDat()
{
	// check value
	if(SetUp_ConstColor < 0 || SetUp_ConstColor > 15)
	{
		remove("SetupDat.sdk");
		return FALSE;
	}

	if(SetUp_GameLevel < 0 || SetUp_GameLevel > 4)
	{
		remove("SetupDat.sdk");
		return FALSE;
	}
	
	if(SetUp_PosColor < 0 || SetUp_PosColor > 15)
	{
		remove("SetupDat.sdk");
		return FALSE;
	}

	return TRUE;
}

BOOL Sudoku::CheckPlayDat()
{
	int SavedBoard[9][9] = {0};
	int SavedCBoard[9][9] = {0};
	int SavedNumber[10] = {0};
	int SavedTime = 0;

	FILE *PlayData = fopen("PlayDat.sdk", "r");

	// bring data to test variable
	if(PlayData != NULL)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<9; j++)
			{
				fscanf(PlayData, "%d ", &SavedBoard[i][j]);
				fscanf(PlayData, "%d ", &SavedCBoard[i][j]);
			}
			
			fscanf(PlayData, "%d ", &SavedNumber[i]);
		}
		
		fscanf(PlayData, "%d ", &SavedNumber[9]);
		fscanf(PlayData, "%d ", &SavedTime);
		
		fclose(PlayData);
	}

	// check value
	for(int i=0; i<9; i++)
	{
		for(int j=0; j<9; j++)
		{
			if(SavedBoard[i][j] > 9 || SavedBoard[i][j] < 0)
			{
				remove("PlayDat.sdk");
				return FALSE;
			}

			if(SavedCBoard[i][j] != 0 && SavedCBoard[i][j] != 1)
			{
				remove("PlayDat.sdk");
				return FALSE;
			}
		}

		if(SavedNumber[i] > 81 || SavedNumber[i] < 0)
		{
			remove("PlayDat.sdk");
			return FALSE;
		}
	}

	if(SavedNumber[9] > 81 || SavedNumber[9] < 0)
	{
		remove("PlayDat.sdk");
		return FALSE;
	}
	
	if(SavedTime < 0 || SavedTime > 356405)
	{
		remove("PlayDat.sdk");
		return FALSE;
	}
	
	return TRUE;
}

void main()
{
	Sudoku Game1;
	int GameState = 0;
	BOOL Win = FALSE;
	BOOL SaveData = TRUE;

	// setup console
	setcursortype(NOCURSOR);
	system("mode con cols=70 lines=14");
	system("title Sudoku");

	// check save data value
	SaveData = Game1.CheckSetupDat();

	if(SaveData == FALSE)
	{
		system("cls");
		SetColor(LIGHTRED, NULL);
		gotoxy(20, 6); cout << "설정 데이터가 손상되었습니다.";
		gotoxy(20, 7); cout << "게임을 다시 실행시켜 주십시오.";
		gotoxy(20, 8); cout << "설정 데이터를 자동으로 초기화합니다.";
		getch();
		
		return;
	}

	// main
	while(1)
	{
		while(GameState != 1 && GameState != 2)
		{
			// draw main menu & select option
			Game1.ShowMainMenu();
			GameState = Game1.SelectMenu();
			
			// how to play
			if(GameState == 3)
				Game1.HowTo();

			// setup
			else if(GameState == 4)
				Game1.Setup();
			
			// exit game
			else if(GameState == 5)
			{
				system("cls");
				return;
			}
		}
		
		// game start
		Game1.Mapping(GameState);
		Win = Game1.Playing();
		
		// victory text
		if(Win == TRUE)
		{
			gotoxy(30, 10);
			cout << GLOBALTEXT_2;
			gotoxy(0, 15);
			getch();
		}
		
		// reinitialize
		GameState = 0;
		Game1.InitializeData();
	}
	
	return;
}