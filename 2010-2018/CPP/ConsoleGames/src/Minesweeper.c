/************************************************************************/
/* [Minesweeper game using C language]                                  */
/*                                                                      */
/*  # Produced by "2010305049 Lee Tae Hoon"                             */
/*																		*/
/*	# 2011/12/3 : 1.00						                            */
/*	# The last update: 2011/12/3        								*/
/*									     	     旨旬  旨收收旬旨旬旨旬	*/
/*												 早曲旬曲旬旨旭早曳朽早	*/
/*												 曲收旭  曲旭  曲旭曲旭	*/
/************************************************************************/

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>

// 詭觼煎
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

// 瞪羲 滲熱
boolean gb_Testmode = FALSE; // 纔蝶お 賅萄 (蟾晦��: 餌辨)
boolean gb_Colormode = TRUE; // 儀鼻 餌辨 賅萄 (蟾晦��: 餌辨)
boolean gb_Soundmode = TRUE; // 模葬 餌辨 (蟾晦��: 餌辨)
int gb_GameState = GS_MAINMENU; // 啪歜 鼻鷓 (蟾晦��: 詭檣詭景)
char gb_GameVersion[] = "1.00"; // 啪歜 幗瞪
int gb_SelectLevel = 1; // 摹鷗脹 陪檜紫 (蟾晦��: 蟾爾)
int gb_SelectUI = 1; // 摹鷗脹 UI (蟾晦��: 晦獄)
struct GameBoard gb_MainBoard[20][30]; // (啪歜 爾萄)
int gb_SizeX = 9, gb_SizeY = 9; // 啪歜っ 餌檜鍔 (蟾晦��: 蟾爾)
int gb_Mines = 10; // 雖煆熱 (蟾晦��: 蟾爾)
int gb_UIMines = 10; // 檣攪む檜蝶縑 ル衛腎朝 雖煆熱
int gb_UIrenewtimer = 10000; // UI 偵褐擊 嬪и 顫檜該滲熱
int gb_ClickCount = 0; // 幗が 贗葛 �蝦� (蟾晦��: 0)
int gb_Time = 0; // 唳婁 衛除擊 盪濰ж晦 嬪и 滲熱

//
extern void Cprintf(char *String, int FontCR, int BackCR);
extern void ChangeCR(int FontCR, int BackCR);
extern void ConClr();
extern void MoveCur(int x, int y);
extern int Random_Int(int Min, int Max);
extern void BipSound(int Pitch, int Duration, boolean UseSound);
extern void SetCurType(int CurType);

// 雖煆 瓊晦 橾奩 л熱 Щ煎饜顫殮
void ms_Initialization(); // 蟾晦�� л熱
void ms_DrawMainMenu(); // 詭檣詭景 斜葬朝 л熱
int ms_OnMainMenu(); // 詭檣詭景 摹鷗 婦溼 л熱
void ms_DrawOptionMenu(); // 褫暮詭景 斜葬朝 л熱
void ms_OnOptionMenu(); // 褫暮詭景 摹鷗 婦溼 л熱
void ms_DrawGameBoard(); // 啪歜爾萄 斜葬朝 л熱
void ms_FormatBoardData(int i, int j, int BackColor); // 爾萄曖 等檜攪蒂 UI/儀縑 評塭 盪濰脹 僥濠煎 轎溘衛酈朝 л熱
int ms_OnGameBoard(); // 啪歜醞 摹鷗 婦溼 л熱
int ms_LeftClick(int x, int y, boolean Recursive); // 豭薹 贗葛 擋暮
void ms_RightClick(int x, int y); // 螃艇薹 贗葛 擋暮
int ms_DualClick(int x, int y); // 曄薹 贗葛 擋暮
void ms_CreateMine(int x, int y); // 雖煆 儅撩 л熱 (羅 贗葛衛 儅撩. 衙偃滲熱煎 羅贗葛 雖薄 厥擠)
void ms_PlusBoardNumber(int x, int y); // 殮溘脹 謝ル曖 爾萄 等檜攪蒂 1隸陛 (等檜攪陛 8嘐虜檣 唳辦縑虜 隸陛)
void ms_DrawGameOver(); // 啪歜 螃幗衛 �飛� 轎溘�� 殮溘 渠晦
void ms_DrawWin(); // 蝓葬ж賊 �飛� 轎溘�� 殮溘 渠晦
int ms_CheckWin(); // 蝓葬 褻勒 っ欽 л熱

// 夢蝶 л熱 Щ煎饜顫殮
void TypeBox(char String[76], int ColorFont, int ColorBack);
void EraseBox();

// 偃褻 л熱 Щ煎饜顫殮
void BCprintf(char *String, int FontCR, int BackCR);

// л熱
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

	if(!gb_Testmode) system("mode con cols=80 lines=25"); // 夔樂 餌檜鍔 滲唳
	else system("mode con cols=80 lines=30");

	if(!gb_Testmode) system("title ConsoleGames - Minesweeper"); // 夔樂 薯跡 滲唳
	else system("title ConsoleGames - Minesweeper Testmode Applied");

	gb_GameState = GS_MAINMENU; // 啪歜 鼻鷓 (蟾晦��: 詭檣詭景)
	// strcpy(gb_GameVersion, "1.00"); // 啪歜 幗瞪
	// gb_SelectLevel = 1; // 摹鷗脹 陪檜紫 (蟾晦��: 蟾爾)
	// gb_SelectUI = 1; // 摹鷗脹 UI (蟾晦��: 晦獄)
	// gb_Colormode = TRUE; // 儀鼻 餌辨 賅萄(蟾晦��: 餌辨)
	// gb_Soundmode = TRUE; // 模葬 餌辨 (蟾晦��: 餌辨)
	gb_UIMines = gb_Mines; // 檣攪む檜蝶 雖煆 熱 = 褒薯 雖煆 熱 (翕晦��)
	gb_ClickCount = 0; // 幗が 贗葛 �蝦� (蟾晦��: 0)
	gb_Time = 0; // 衛除 蟾晦��

	for(i=0; i<20; i++) // 詭檣 爾萄 蟾晦��
	{
		for(j=0; j<30; j++)
		{
			gb_MainBoard[i][j].BoardData = 0;
			gb_MainBoard[i][j].VisualType = BT_HIDE;
		}
	}
	
	// gb_SizeX = 9; // 啪歜っ 餌檜鍔 (蟾晦��: 蟾爾)
	// gb_SizeY = 9;

	SetCurType(CT_HIDE); // 醴憮 獗梯
}

void ms_DrawMainMenu() // GS_MAINMENU
{
	ConClr();
	BCprintf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式汐", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                              [Made By : 2010305049 檜鷓�釁 早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭", CR_WHITE, CR_BLACK);
	MoveCur(2, 2); BCprintf("    雖 煆 瓊 晦 ver ", CR_WHITE, CR_BLACK); MoveCur(0, 8);
	BCprintf("                          忙式式式式式式式式式式式式汐                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛                        早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛   ≒ [ 億煎ж晦 ] ≒   早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛                        早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛   ∞ [ 檜橫ж晦 ] ∞   早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛                        早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛   ∞  [ 撲  薑 ]  ∞   早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛                        早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛   ∞ [ 啪歜謙猿 ] ∞   早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          弛                        早                          ", CR_WHITE, CR_BLACK);
	BCprintf("                          汎收收收收收收收收收收收收旭                          ", CR_WHITE, CR_BLACK);
	MoveCur(0, 21);
	BCprintf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式汐", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭", CR_WHITE, CR_BLACK);
	MoveCur(34, 12); BCprintf("[ 檜橫ж晦 ]", CR_DGRAY, CR_BLACK);
	TypeBox("啪歜擊 億煎 衛濛м棲棻.", CR_WHITE, CR_BLACK);
	
	if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	MoveCur(22, 2); printf("%s", gb_GameVersion); ChangeCR(CR_GRAY, CR_BLACK);
	
	if(gb_Testmode) { MoveCur(61, 2); Cprintf("Testmode Applied", CR_RED, CR_BLACK); } // 啪歜 幗瞪 / 纔蝶お賅萄 �挫�

	MoveCur(2, 12);
}

void ms_DrawOptionMenu() // GS_OPTION
{
	ConClr();
	BCprintf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式汐", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭", CR_WHITE, CR_BLACK);
	MoveCur(2, 2); BCprintf("    啪  歜  撲  薑  (Option)", CR_WHITE, CR_BLACK);
	MoveCur(0, 7);
	BCprintf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式汐", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛   ≒ [ 陪檜紫 ] :: 蟾晝 (陛煎: 9 / 撮煎: 9 / 雖煆 : 10)                    早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛   ∞ [ UI摹鷗 ] :: 翕旋 (≒∞函刻券刷刺到刮制佻侑Ｄ)                       早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛   ∞ [ 儀  鼻 ] :: 餌辨                                                    早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛   ∞ [ 模  葬 ] :: 餌辨                                                    早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("弛   ∞ [ 詭檣 詭景煎 ]                                                       早", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭", CR_WHITE, CR_BLACK);
	MoveCur(0, 21);
	BCprintf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式汐", CR_WHITE, CR_BLACK);
	BCprintf("弛                                                                            早", CR_WHITE, CR_BLACK);
	BCprintf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭", CR_WHITE, CR_BLACK);
	TypeBox("啪歜 陪檜紫蒂 滲唳м棲棻. 蟾晝, 醞晝, 堅晝 陪檜紫陛 氈蝗棲棻.               ", CR_WHITE, CR_BLACK);

	// 陪檜紫
	if(gb_SelectLevel == 1) { MoveCur(22, 9); BCprintf("蟾晝", CR_YELLOW, CR_BLACK); BCprintf(" (陛煎: 9 / 撮煎: 9 / 雖煆 : 10)          ", CR_DGRAY, CR_BLACK); } 
	else if(gb_SelectLevel == 2) { MoveCur(22, 9); BCprintf("醞晝", CR_DGREEN, CR_BLACK); BCprintf(" (陛煎: 16 / 撮煎: 16 / 雖煆 : 40)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectLevel == 3) { MoveCur(22, 9); BCprintf("堅晝", CR_RED, CR_BLACK); BCprintf(" (陛煎: 30 / 撮煎: 18 / 雖煆 : 112)          ", CR_DGRAY, CR_BLACK); }
	
	// UI摹鷗
	if(gb_SelectUI == 1) { MoveCur(22, 11); BCprintf("翕旋", CR_WHITE, CR_BLACK); BCprintf(" (≒∞函刻券刷刺到刮制佻侑Ｄ)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectUI == 2) { MoveCur(22, 11); BCprintf("啻賅", CR_WHITE, CR_BLACK); BCprintf(" (﹥﹤拍抵拚抱拘拖拗拆抹拓Ｄ)          ", CR_DGRAY, CR_BLACK); }
	else if(gb_SelectUI == 3) { MoveCur(22, 11); BCprintf("�母�", CR_WHITE, CR_BLACK); BCprintf(" (﹣﹢橾檜鳴餌螃幫艦つ≠??Ｄ)          ", CR_DGRAY, CR_BLACK); }

	// 儀鼻
	if(gb_Colormode) { MoveCur(22, 13); Cprintf("餌辨          ", CR_GOLD, CR_BLACK); }
	else { MoveCur(22, 13); printf("餌辨 寰л          "); }

	// 模葬
	if(gb_Soundmode) { MoveCur(22, 15); BCprintf("餌辨          ", CR_DCYAN, CR_BLACK); }
	else { MoveCur(22, 15); BCprintf("餌辨 寰л          ", CR_DCYAN, CR_BLACK); }

	MoveCur(2, 12);
}

void ms_DrawGameBoard() // GS_GAMESTRAT
{
	int i, j;

	ConClr();

	// 蠔塭 賅萄檜賊 �羃� 摹 匐擎 寡唳戲煎 斜萼棻
	if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	
	// 輿滲 唳啗
	printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式成式式式式式汐");
	for(i=0; i<20; i++)
	printf("弛                                                                弛          早");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扛式式式式式此");
	printf("弛                                                                            早");
	printf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
	
	// 爾萄 纔舒葬 斜葬晦(嶺賊)
	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("旨");
		else if(i==gb_SizeX) printf("旬");
		else printf("收");
	}

	// 爾萄 纔舒葬 斜葬晦(難賊)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("早"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("早"); }
		}
	}

	// 爾萄 纔舒葬 斜葬晦(ж欽)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("曲");
		else if(i==gb_SizeX) printf("旭");
		else printf("收");
	}

	// 爾萄 詭景璽 & 薄熱諦 衛除
	MoveCur(68, 2); (gb_Colormode) ? Cprintf(" □詭景■ ", CR_DGREEN, CR_BLACK) : printf(" □詭景■ ");
	MoveCur(68, 5); (gb_Colormode) ? Cprintf("[億煎ж晦]", CR_WHITE, CR_BLACK) : printf("[億煎ж晦]");
	MoveCur(68, 7); (gb_Colormode) ? Cprintf("[盪濰ж晦]", CR_DGRAY, CR_BLACK) : printf("[盪濰ж晦]");
	MoveCur(68, 9); (gb_Colormode) ? Cprintf("[詭檣詭景]", CR_WHITE, CR_BLACK) : printf("[詭檣詭景]");
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("戍式式式式式此", CR_WHITE, CR_BLACK) : printf("戍式式式式式此");
	MoveCur(68, 13); (gb_Colormode) ? Cprintf("□雖煆熱■", CR_RED, CR_BLACK) : printf("□雖煆熱■");
	MoveCur(68, 15); (gb_Colormode) ? Cprintf("    00    ", CR_WHITE, CR_BLACK) : printf("    00    ");
	MoveCur(68, 17); (gb_Colormode) ? Cprintf(" □衛除■ ", CR_RED, CR_BLACK) : printf(" □衛除■ ");
	MoveCur(68, 19); (gb_Colormode) ? Cprintf("[00:00:00]", CR_WHITE, CR_BLACK) : printf("[00:00:00]");
	
	// 爾萄 斜葬晦
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=0; j<gb_SizeX; j++)
		{
			MoveCur(j*2+4, i+2);
			ms_FormatBoardData(i, j, CR_GRAY);
		}
	}

	// 衛濛 雖薄 斜葬晦
	if(gb_ClickCount == 0) { MoveCur(4, 2); ms_FormatBoardData(0, 0, CR_WHITE); }

	// 蟾晦 紫遺蜓
	TypeBox("[A]:豭薹 贗葛  [S]:曄薹 贗葛  [D]:螃艇薹 贗葛  [∟⊥⊿∠]:檜翕  [ESC]:詭景", CR_WHITE, CR_BLACK);

	// Я瑪 罹睡 (儀鼻 餌辨 寰й衛)
	if(!(gb_Colormode)) { SetCurType(CT_SOLID); MoveCur(4, 2); }
	else MoveCur(2, 12);
}

void ms_FormatBoardData(int y, int x, int BackColor)
{
	// [UI] ≒∞函刻券刷刺到刮制佻侑Ｄ / ﹥﹤拍抵拚抱拘拖拗拆抹拓Ｄ / ﹣﹢橾檜鳴餌螃幫艦つ≠??Ｄ
	// [CR] 0: 霞�蜓� 1:羶儀 2:霞喬儀 3:瞳儀 4:陴儀 5:陞儀 6:羶煙 7:�皛� 8:�蜓� ?:陴儀 m:�皛� f:瞳儀

	if(gb_MainBoard[y][x].VisualType == BT_HIDE) // 嬴霜 �挫庰Щ� 彊擎 爾萄.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("≒", CR_DGRAY, BackColor) : printf("≒");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("﹥", CR_DGRAY, BackColor) : printf("﹥");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("﹣", CR_DGRAY, BackColor) : printf("﹣");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_UNCOVER) // 贗葛腎橫 �挫庰� 爾萄.
	{
		switch(gb_MainBoard[y][x].BoardData)
		{
			case 0:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("∞", CR_DGRAY, BackColor) : printf("∞");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("﹤", CR_DGRAY, BackColor) : printf("﹤");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("﹢", CR_DGRAY, BackColor) : printf("﹢"); break;
			case 1:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("函", CR_BLUE, BackColor) : printf("函");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("拍", CR_BLUE, BackColor) : printf("拍");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("橾", CR_BLUE, BackColor) : printf("橾"); break;
			case 2:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("刻", CR_DGREEN, BackColor) : printf("刻");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("抵", CR_DGREEN, BackColor) : printf("抵");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("檜", CR_DGREEN, BackColor) : printf("檜"); break;
			case 3:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("券", CR_RED, BackColor) : printf("券");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("拚", CR_RED, BackColor) : printf("拚");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("鳴", CR_RED, BackColor) : printf("鳴"); break;
			case 4:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("刷", CR_DBLUE, BackColor) : printf("刷");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("抱", CR_DBLUE, BackColor) : printf("抱");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("餌", CR_DBLUE, BackColor) : printf("餌"); break;
			case 5:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("刺", CR_DRED, BackColor) : printf("刺");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("拘", CR_DRED, BackColor) : printf("拘");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("螃", CR_DRED, BackColor) : printf("螃"); break;
			case 6:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("到", CR_DCYAN, BackColor) : printf("到");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("拖", CR_DCYAN, BackColor) : printf("拖");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("幫", CR_DCYAN, BackColor) : printf("幫"); break;
			case 7:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("刮", CR_BLACK, BackColor) : printf("刮");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("拗", CR_BLACK, BackColor) : printf("拗");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("艦", CR_BLACK, BackColor) : printf("艦"); break;
			case 8:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("制", CR_DGRAY, BackColor) : printf("制");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("拆", CR_DGRAY, BackColor) : printf("拆");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("つ", CR_DGRAY, BackColor) : printf("つ"); break;
			case 9:
				if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("佻", CR_BLACK, BackColor) : printf("佻");
				else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("抹", CR_BLACK, BackColor) : printf("抹");
				else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("≠", CR_BLACK, BackColor) : printf("≠"); break;
			default:
				(gb_Colormode == TRUE) ? Cprintf("??", CR_GOLD, BackColor) : printf("??");
		}
	}

	else if(gb_MainBoard[y][x].VisualType == BT_FLAG) // 梓嫦 ル衛蒂 п 場擎 爾萄.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("Ｄ", CR_RED, BackColor) : printf("Ｄ");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("Ｄ", CR_RED, BackColor) : printf("Ｄ");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("Ｄ", CR_RED, BackColor) : printf("Ｄ");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_QUESTION) // 僭擠ル ル衛蒂 п 場擎 爾萄.
	{
		if(gb_SelectUI == 1) (gb_Colormode == TRUE) ? Cprintf("侑", CR_DBLUE, BackColor) : printf("侑");
		else if(gb_SelectUI == 2) (gb_Colormode == TRUE) ? Cprintf("拓", CR_DBLUE, BackColor) : printf("拓");
		else if(gb_SelectUI == 3) (gb_Colormode == TRUE) ? Cprintf("??", CR_DBLUE, BackColor) : printf("??");
	}

	else if(gb_MainBoard[y][x].VisualType == BT_MISS) // 雖煆煎 澀跤 ル衛и 爾萄.
	{
		BCprintf("≦", CR_DRED, CR_GRAY);
	}
}

void ms_DrawGameOver()
{	
	int i, j;
	char KeyValue;

	// 輿滲 唳啗 斜葬晦
	MoveCur(0, 0); BCprintf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式成式式式式式汐", CR_RED, CR_BLACK);
	for(i=1; i<21; i++)
	{
		MoveCur(0, i); BCprintf("弛", CR_RED, CR_BLACK); MoveCur(66, i); BCprintf("弛", CR_RED, CR_BLACK); MoveCur(78, i); BCprintf("早", CR_RED, CR_BLACK);
	}
	MoveCur(0, 21); BCprintf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扛式式式式式此", CR_RED, CR_BLACK);
	MoveCur(0, 22); BCprintf("弛                                                                            早", CR_RED, CR_BLACK);
	MoveCur(0, 23); BCprintf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭", CR_RED, CR_BLACK);
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("戍式式式式式此", CR_RED, CR_BLACK) : printf("戍式式式式式此");

	// 爾萄 纔舒葬 斜葬晦(嶺賊)
	if(gb_Colormode) ChangeCR(CR_RED, CR_BLACK);

	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("旨");
		else if(i==gb_SizeX) printf("旬");
		else printf("收");
	}
	
	// 爾萄 纔舒葬 斜葬晦(難賊)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("早"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("早"); }
		}
	}
	
	// 爾萄 纔舒葬 斜葬晦(ж欽)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("曲");
		else if(i==gb_SizeX) printf("旭");
		else printf("收");
	}

	// 唳堅擠
	BipSound(1000, 500, gb_Soundmode); Sleep(250);
	TypeBox("雖煆陛 摹鷗腎歷蝗棲棻... Game Over 詭檣戲煎 給嬴陛溥賊 [ESC]蒂 援腦褊衛螃.", CR_RED, CR_BLACK);

	// 賅萇 雖煆 爾檜晦, 澀跤 梓嫦 符擎夠 爾檜晦
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
	
	// 酈 殮溘 渠晦
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
	
	// 輿滲 唳啗 斜葬晦
	MoveCur(0, 0); BCprintf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式成式式式式式汐", CR_GREEN, CR_BLACK);
	for(i=1; i<21; i++)
	{
		MoveCur(0, i); BCprintf("弛", CR_GREEN, CR_BLACK); MoveCur(66, i); BCprintf("弛", CR_GREEN, CR_BLACK); MoveCur(78, i); BCprintf("早", CR_GREEN, CR_BLACK);
	}
	MoveCur(0, 21); BCprintf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扛式式式式式此", CR_GREEN, CR_BLACK);
	MoveCur(0, 22); BCprintf("弛                                                                            早", CR_GREEN, CR_BLACK);
	MoveCur(0, 23); BCprintf("汎收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭", CR_GREEN, CR_BLACK);
	MoveCur(66, 11); (gb_Colormode) ? Cprintf("戍式式式式式此", CR_GREEN, CR_BLACK) : printf("戍式式式式式此");
	
	// 爾萄 纔舒葬 斜葬晦(嶺賊)
	if(gb_Colormode) ChangeCR(CR_GREEN, CR_BLACK);
	
	MoveCur(2, 1);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("旨");
		else if(i==gb_SizeX) printf("旬");
		else printf("收");
	}
	
	// 爾萄 纔舒葬 斜葬晦(難賊)
	MoveCur(2, 2);
	for(i=0; i<gb_SizeY; i++)
	{
		for(j=-1; j<=gb_SizeX; j++)
		{
			if(j==-1) { MoveCur(2, i+2); printf("早"); }
			else if (j==gb_SizeX) { MoveCur(gb_SizeX*2+4, i+2); printf("早"); }
		}
	}
	
	// 爾萄 纔舒葬 斜葬晦(ж欽)
	MoveCur(2, gb_SizeY+2);
	for(i=-1; i<=gb_SizeX; i++)
	{
		if(i==-1) printf("曲");
		else if(i==gb_SizeX) printf("旭");
		else printf("收");
	}
	
	// 蝓葬 BGM
	BipSound(261, 333, gb_Soundmode); BipSound(329, 333, gb_Soundmode); BipSound(391, 333, gb_Soundmode); Sleep(250);
	TypeBox("撩奢ж艘蝗棲棻! 譆謙 衛除擎 ", CR_GREEN, CR_BLACK);
	ChangeCR(CR_GREEN, CR_BLACK); printf("[%.2d:%.2d:%.2d]殮棲棻. 詭檣戲煎 給嬴陛晦 : [ESC]酈", (gb_Time/3600), ((gb_Time-(gb_Time/3600))/60), (gb_Time%60));
	ChangeCR(CR_GRAY, CR_BLACK);
	
	// 賅萇 雖煆縑 梓嫦 培晦
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

	// 雖煆熱 0偃煎 滲唳
	gb_UIMines = 0;
	MoveCur(71, 15); if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
	printf("%3d", gb_UIMines); ChangeCR(CR_GRAY, CR_BLACK);
	
	// 酈 殮溘 渠晦
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
				MoveCur(31, 10);  printf("≒"); MoveCur(47, 10);  printf("≒"); MoveCur(31, 12); printf("∞"); MoveCur(47, 12); printf("∞");
				MoveCur(31, 14); printf("∞"); MoveCur(47, 14); printf("∞"); MoveCur(31, 16); printf("∞"); MoveCur(47, 16); printf("∞");
				TypeBox("啪歜擊 億煎 衛濛м棲棻.", CR_WHITE, CR_BLACK);
			}
			
			else if(CurrentSelect == 2)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("∞"); MoveCur(47, 10);  printf("∞"); MoveCur(31, 12); printf("≒"); MoveCur(47, 12); printf("≒");
				MoveCur(31, 14); printf("∞"); MoveCur(47, 14); printf("∞"); MoveCur(31, 16); printf("∞"); MoveCur(47, 16); printf("∞");
				TypeBox("盪濰脹 啪歜擊 碳楝褫棲棻. (掘⑷醞殮棲棻)", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 3)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("∞"); MoveCur(47, 10);  printf("∞"); MoveCur(31, 12); printf("∞"); MoveCur(47, 12); printf("∞");
				MoveCur(31, 14); printf("≒"); MoveCur(47, 14); printf("≒"); MoveCur(31, 16); printf("∞"); MoveCur(47, 16); printf("∞");
				TypeBox("陪檜紫, UI, 儀鼻, 模葬 蛔擊 撲薑м棲棻.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 4)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(31, 10);  printf("∞"); MoveCur(47, 10);  printf("∞"); MoveCur(31, 12); printf("∞"); MoveCur(47, 12); printf("∞");
				MoveCur(31, 14); printf("∞"); MoveCur(47, 14); printf("∞"); MoveCur(31, 16); printf("≒"); MoveCur(47, 16); printf("≒");
				TypeBox("啪歜擊 謙猿м棲棻.", CR_WHITE, CR_BLACK);
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
						MoveCur(22, 9); BCprintf("蟾晝", CR_YELLOW, CR_BLACK); BCprintf(" (陛煎: 9 / 撮煎: 9 / 雖煆 : 10)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 9; gb_SizeY = 9; gb_Mines = 10;
					}
					
					else if(gb_SelectLevel == 2)
					{
						MoveCur(22, 9); BCprintf("醞晝", CR_DGREEN, CR_BLACK); BCprintf(" (陛煎: 16 / 撮煎: 16 / 雖煆 : 40)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 16; gb_SizeY = 16; gb_Mines = 40;
					}
					
					else if(gb_SelectLevel == 3)
					{
						MoveCur(22, 9); BCprintf("堅晝", CR_RED, CR_BLACK); BCprintf(" (陛煎: 30 / 撮煎: 18 / 雖煆 : 112)          ", CR_DGRAY, CR_BLACK);
						gb_SizeX = 30; gb_SizeY = 18; gb_Mines = 112;
					}
				}

				else if(CurrentSelect == 2)
				{
					(gb_SelectUI < 3) ? (gb_SelectUI++) : (gb_SelectUI = 1);
					if(gb_SelectUI == 1) { MoveCur(22, 11); BCprintf("翕旋", CR_WHITE, CR_BLACK); BCprintf(" (≒∞函刻券刷刺到刮制佻侑Ｄ)          ", CR_DGRAY, CR_BLACK); }
					else if(gb_SelectUI == 2) { MoveCur(22, 11); BCprintf("啻賅", CR_WHITE, CR_BLACK); BCprintf(" (﹥﹤拍抵拚抱拘拖拗拆抹拓Ｄ)          ", CR_DGRAY, CR_BLACK); }
					else if(gb_SelectUI == 3) { MoveCur(22, 11); BCprintf("�母�", CR_WHITE, CR_BLACK); BCprintf(" (﹣﹢橾檜鳴餌螃幫艦つ≠??Ｄ)          ", CR_DGRAY, CR_BLACK); }
				}

				else if(CurrentSelect == 3)
				{
					gb_Colormode = !(gb_Colormode);
					if(gb_Colormode) { MoveCur(22, 13); Cprintf("餌辨          ", CR_GOLD, CR_BLACK); }
					else { MoveCur(22, 13); printf("餌辨 寰л          "); }
					ms_DrawOptionMenu();
				}

				else if(CurrentSelect == 4)
				{
					gb_Soundmode = !(gb_Soundmode);
					if(gb_Soundmode) { MoveCur(22, 15); BCprintf("餌辨          ", CR_DCYAN, CR_BLACK); BipSound(1000, 100, TRUE); }
					else { MoveCur(22, 15); BCprintf("餌辨 寰л         ", CR_DCYAN, CR_BLACK); }
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
				MoveCur(5, 9);  printf("≒"); MoveCur(5, 11); printf("∞"); MoveCur(5, 13); printf("∞");
				MoveCur(5, 15); printf("∞"); MoveCur(5, 17); printf("∞");
				TypeBox("啪歜 陪檜紫蒂 滲唳м棲棻. 蟾晝, 醞晝, 堅晝 陪檜紫陛 氈蝗棲棻.", CR_WHITE, CR_BLACK);
			} 

			else if(CurrentSelect == 2)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("∞"); MoveCur(5, 11); printf("≒"); MoveCur(5, 13); printf("∞");
				MoveCur(5, 15); printf("∞"); MoveCur(5, 17); printf("∞");
				TypeBox("啪歜っ曖 僥曄擊 滲唳м棲棻. 晦獄, 啻賅, �母� 謙盟陛 氈蝗棲棻.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 3)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("∞"); MoveCur(5, 11); printf("∞"); MoveCur(5, 13); printf("≒");
				MoveCur(5, 15); printf("∞"); MoveCur(5, 17); printf("∞");
				TypeBox("啪歜縑 儀鼻 餌辨 罹睡蒂 唸薑м棲棻.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 4)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("∞"); MoveCur(5, 11); printf("∞"); MoveCur(5, 13); printf("∞");
				MoveCur(5, 15); printf("≒"); MoveCur(5, 17); printf("∞");
				TypeBox("模葬 餌辨 罹睡蒂 唸薑м棲棻.", CR_WHITE, CR_BLACK);
			}

			else if(CurrentSelect == 5)
			{
				if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				MoveCur(5, 9);  printf("∞"); MoveCur(5, 11); printf("∞"); MoveCur(5, 13); printf("∞");
				MoveCur(5, 15); printf("∞"); MoveCur(5, 17); printf("≒");
				TypeBox("撲薑擊 盪濰ж堅 詭檣 詭景煎 給嬴骨棲棻.", CR_WHITE, CR_BLACK);
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
	int SavedX = 0, SavedY = 0; //□詭景■鼻鷓橾 陽 CurrentX/Y蒂 盪濰ж晦 嬪и 滲熱
	boolean OnMenu = FALSE; // □詭景■鼻鷓檣雖蒂 盪濰ж朝 滲熱
	int MenuCounter = 0; // □詭景■陛 賃廓擊 陛葬酈堅 氈朝雖 盪濰ж朝 滲熱
	int ClickReturnValue; // 雖薄 摹鷗衛 ч翕擊 嫡嬴螃朝 滲熱 (雖煆艘釭, 斜傖 夥款檜艘釭 蛔蛔)
	int RenewCounter = 0; // 酈爾萄 殮溘擊 寰 嫡擊 陽 詭景っ 偵褐 樓紫蒂 婦葬ж晦 嬪и 滲熱 (while僥曖 瑞Щ陛 傘鼠 說塭憮 アお縑 摹檜 斜橫雖晦 陽僥縑 餌辨)

	// 顫檜該 婦溼 滲熱
	time_t CurrentT; // 衛除 婦溼 滲熱 (п渦だ橾: <time.h>)
	struct tm *Today;	// 掘褻羹 tmん檣攪
	int BeforeTime; // ⑷營 蟾欽嬪 衛除 盪濰擊 嬪и 滲熱1
	int CurrentTime; // ⑷營 蟾欽嬪 衛除 盪濰擊 嬪и 滲熱2

	time(&CurrentT); // CurrentTime 滲熱縑 ⑷營 衛除 頂溥嫡擠
	Today = localtime(&CurrentT); // Today 掘褻羹縑 CurrentTime曖 等檜攪菟擊 頂溥嫡擠
	BeforeTime = Today->tm_hour*3600 + Today->tm_min*60 + Today->tm_sec; // 瞪羲 滲熱縑 衛除擊 蟾煎 滲唳ж罹 盪濰

	while(1)
	{
		RenewCounter++;

		time(&CurrentT);
		Today = localtime(&CurrentT);
		CurrentTime = Today->tm_hour*3600 + Today->tm_min*60 + Today->tm_sec;

		// 酈爾萄陛 揚溜擊 陽
		if(kbhit())
		{
			KeyValue = getch();
			EraseBox();

			if(OnMenu) // 酈爾萄陛 詭景っ 輿紫橾陽
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
					MoveCur(68, 5); BCprintf("[億煎ж晦]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[盪濰ж晦]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[詭檣詭景]", CR_WHITE, CR_BLACK);
					MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE); MoveCur((CurrentX)*2+4, CurrentY+2);
				}
				
				else if(KeyValue == KEY_ENTER)
				{
					if(CurrentY == 0)
					{
						if(MenuCounter == 0)
						{
							MenuCounter++;
							TypeBox("薑蜓煎 億煎 ж啊蝗棲梱? 億煎 ж溥賊 и廓 渦 縛攪蒂 援腦褊衛螃.", CR_WHITE, CR_BLACK);
							BipSound(300, 500, gb_Soundmode);
						}
						
						else if (MenuCounter >= 1) { ms_Initialization(); return GS_RESTRAT; }
					}
					
					else if(CurrentY == 1)
					{
						TypeBox("嬴霜 掘⑷腎雖 彊擎 晦棟殮棲棻.", CR_WHITE, CR_BLACK);
					}
					
					else if(CurrentY == 2)
					{
						if(MenuCounter == 0)
						{
							MenuCounter++;
							TypeBox("薑蜓煎 謙猿м棲梱? 謙猿ж溥賊 и廓 渦 縛攪蒂 援腦褊衛螃.", CR_WHITE, CR_BLACK);
							BipSound(300, 500, gb_Soundmode);
						}
						
						else if(MenuCounter >= 1) { ms_Initialization(); return GS_MAINMENU; }
					}
				}

				if(CurrentY == 0 && OnMenu) // OnMenu 餉薯衛 擒除曖 幗斜 嫦儅
				{
					MoveCur(68, 5); BCprintf("[億煎ж晦]", CR_WHITE, CR_PURPLE); MoveCur(68, 7); BCprintf("[盪濰ж晦]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[詭檣詭景]", CR_WHITE, CR_BLACK);
					if(MenuCounter==0) TypeBox("啪歜擊 億煎 衛濛м棲棻. 霞ч醞檣 啪歜擎 晦煙腎雖 彊蝗棲棻.", CR_WHITE, CR_BLACK);
					MoveCur(69, 5); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
				}

				else if(CurrentY == 1 && OnMenu)
				{
					MoveCur(68, 5); BCprintf("[億煎ж晦]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[盪濰ж晦]", CR_DGRAY, CR_PURPLE);
					MoveCur(68, 9); BCprintf("[詭檣詭景]", CR_WHITE, CR_BLACK);
					TypeBox("霞ч醞檣 啪歜擊 盪濰м棲棻. (掘⑷醞殮棲棻)", CR_WHITE, CR_BLACK);
					MoveCur(69, 7);
				}

				else if(CurrentY == 2 && OnMenu)
				{
					MoveCur(68, 5); BCprintf("[億煎ж晦]", CR_WHITE, CR_BLACK); MoveCur(68, 7); BCprintf("[盪濰ж晦]", CR_DGRAY, CR_BLACK);
					MoveCur(68, 9); BCprintf("[詭檣詭景]", CR_WHITE, CR_PURPLE); MoveCur(69, 9);
					if(MenuCounter==0) TypeBox("詭檣詭景煎 給嬴骨棲棻. 霞ч醞檣 啪歜擎 晦煙腎雖 彊蝗棲棻.", CR_WHITE, CR_BLACK);
					MoveCur(69, 9);
				}
			}
			
			else // 酈爾萄陛 啪歜っ 輿紫橾陽
			{
				if(KeyValue == KEY_LEFT)
				{
					if(CurrentX > 0)
					{
						CurrentX--;
						MoveCur((CurrentX+1)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX+1, CR_GRAY); // 檜翕瞪 謝ル曖 賅曄 犒掘
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE); // 檜翕�� 謝ル曖 ⑷嬪纂 ル衛
						MoveCur((CurrentX)*2+4, CurrentY+2); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentX = gb_SizeX-1; ms_FormatBoardData(CurrentY, 0, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
					}
				}

				else if(KeyValue == KEY_RIGHT)
				{
					if(CurrentX < gb_SizeX-1)
					{
						CurrentX++;
						MoveCur((CurrentX-1)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX-1, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
					}

					else
					{
						MoveCur((CurrentX)*2+4, CurrentY+2); CurrentX = 0; ms_FormatBoardData(CurrentY, gb_SizeX-1, CR_GRAY);
						MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_WHITE);
						MoveCur((CurrentX)*2+4, CurrentY+2); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
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
						MoveCur((CurrentX)*2+4, CurrentY+2); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
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
						MoveCur((CurrentX)*2+4, CurrentY+2); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
					}
				}

				else if(KeyValue == KEY_ESC)
				{
					OnMenu = !(OnMenu);
					SavedX = CurrentX;
					SavedY = CurrentY;
					MoveCur((CurrentX)*2+4, CurrentY+2); ms_FormatBoardData(CurrentY, CurrentX, CR_GRAY);
					CurrentY = 0;
					TypeBox("詭景煎 檜翕м棲棻. 啪歜戲煎 給嬴陛溥賊 [ESC]酈蒂 援腦褊衛螃.", CR_WHITE, CR_BLACK);
					MoveCur(68, 5); BCprintf("[億煎ж晦]", CR_WHITE, CR_PURPLE); MoveCur(69, 5); // 鼠儀 賅萄蒂 嬪и Я慇 檜翕
					BipSound(200, 100, gb_Soundmode);
				}

				else if(KeyValue == 'A' || KeyValue == 'a') // 豭薹 贗葛
				{
					ClickReturnValue = ms_LeftClick(CurrentX, CurrentY, FALSE);

					if(ClickReturnValue != GS_LOSE) ClickReturnValue = ms_CheckWin();
				}

				else if(KeyValue == 'S' || KeyValue == 's') // 曄薹 贗葛
				{
					ClickReturnValue = ms_DualClick(CurrentX, CurrentY);

					if(ClickReturnValue != GS_LOSE) ClickReturnValue = ms_CheckWin();
				}

				else if(KeyValue == 'D' || KeyValue == 'd') // 螃艇薹 贗葛
				{
					ms_RightClick(CurrentX, CurrentY);
				}
			}
		}

		// 酈爾萄陛 寰 揚溜擊 陽
		else
		{
			if(RenewCounter >= gb_UIrenewtimer) // 顫檜該 蟾婁ж賊
			{
				RenewCounter = 0;

				// □雖煆熱■檣攪む檜蝶
				MoveCur(71, 15); if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
				printf("%3d", gb_UIMines); ChangeCR(CR_GRAY, CR_BLACK);

				// □顫檜該■檣攪む檜蝶
				if(CurrentTime > BeforeTime) // 1蟾 檜鼻 唳婁ж艘戲賊
				{
					gb_Time += (CurrentTime - BeforeTime); // 衛除 離虜躑 隸陛衛霽
					BeforeTime = CurrentTime; // в熱
					if(gb_Time > 359999) gb_Time = 359999; // 99衛除 59碟 59蟾 蟾婁衛 99衛除 59碟 59蟾煎 堅薑
					if(gb_Colormode) ChangeCR(CR_WHITE, CR_BLACK);
					MoveCur(68, 19); printf("[%.2d:%.2d:%.2d]", (gb_Time/3600), ((gb_Time-(gb_Time/3600))/60), (gb_Time%60)); // 檣攪む檜蝶縑 轎溘
					MoveCur((CurrentX)*2+4, CurrentY+2);
				}

				else // 23衛 -> 0衛煎 剩橫陛朝 睡碟橾 唳辦 蛔... (擒 1蟾 薑紫曖 誇辭檜 氈擊 熱 氈棻)
				{
					BeforeTime = CurrentTime;
				}
				
			}
		}

		// 啪歜曖 鼻鷓蒂 奩�� (啗樓 霞ч or ぬ寡 or 蝓葬)
		if(ClickReturnValue == GS_LOSE) return GS_LOSE; // 雖煆 嫖嬴憮 GS_LOSE 葬欐衛 奩犒僥 驍轎
		else if(ClickReturnValue == GS_WIN) return GS_WIN; // 蝓葬褻勒 醱褶衛 GS_WIN ��轎煎 奩犒僥 驍轎
	}
	
	return GS_STARTGAME; // 罹晦縑 GS_LOSE, GS_WIN ��轎 旎雖
}

int ms_LeftClick(int x, int y, boolean Recursive)
{
	int LeftClickResult = GS_STARTGAME;

	// 謝ル陛 爾萄蒂 慇橫釭雖 彊朝 и紫 頂檜賊
	if(x >= 0 && y >= 0 && x < gb_SizeX && y < gb_SizeY)
	{
		// 摹鷗и 夠檜 摹鷗腎雖 彊剪釭 ?葆觼橾 唳辦
		if(gb_MainBoard[y][x].VisualType == BT_HIDE || gb_MainBoard[y][x].VisualType == BT_QUESTION)
		{
			// 羅 贗葛檜賊 雖煆 褕擠
			if(gb_ClickCount == 0)
				ms_CreateMine(x, y);

			gb_ClickCount++;
			
			// 贗葛и 夠曖 薑爾 嫩�鱁� 爾萄 偵褐
			gb_MainBoard[y][x].VisualType = BT_UNCOVER;
			MoveCur((x)*2+4, y+2);
			if(Recursive) ms_FormatBoardData(y, x, CR_GRAY);
			else ms_FormatBoardData(y, x, CR_WHITE);
			MoveCur((x)*2+4, y+2);
			
			// 贗葛и 夠檜 0橾 唳辦 營敝л熱
			if(gb_MainBoard[y][x].BoardData == 0)
			{
					ms_LeftClick(x-1, y-1, TRUE); ms_LeftClick(x, y-1, TRUE); ms_LeftClick(x+1, y-1, TRUE); ms_LeftClick(x-1, y, TRUE);
					ms_LeftClick(x+1, y, TRUE); ms_LeftClick(x-1, y+1, TRUE); ms_LeftClick(x, y+1, TRUE); ms_LeftClick(x+1, y+1, TRUE);
			}

			// 贗葛и 夠檜 雖煆橾 唳辦 啪歜 螃幗
			else if(gb_MainBoard[y][x].BoardData == 9)
			{
				LeftClickResult = GS_LOSE;
			}
		}
	}

	return LeftClickResult; // 罹晦縑 GS_LOSE 霜蕾 葬欐 旎雖
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
		// 輿滲 梓嫦曖 熱蒂 蘋遴お
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

		if(gb_Testmode) { TypeBox("", CR_WHITE, CR_BLACK); printf("%d偃曖 梓嫦 馬雖脾.", FlagCounter); } // ONLY FOR TEST

		// 輿滲 梓嫦曖 熱諦 摹鷗脹 謝ル曖 廓�ㄟ� 偽戲賊 輿滲曖 BT_HIDE 蒂 豭薹 贗葛
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

					if(DualClickResult == GS_LOSE) // и廓檜塭紫 GS_LOSE高檜 釭諮戲賊 菽橡贗葛曖 譆謙 葬欐高擎 GS_LOSE
						ReturnLose = TRUE;
					
					// MoveCur((j)*2+4, i+2); ms_FormatBoardData(j, i, CR_GOLD); MoveCur((j)*2+4, y+i); // ONLY FOR TEST
				}
			}
		}

		else { TypeBox("輿滲 梓嫦曖 熱陛 п渡 嬪纂曖 熱諦 偽雖 彊蝗棲棻.", CR_WHITE, CR_BLACK); BipSound(100, 100, gb_Soundmode); }
	}

	if(ReturnLose) return GS_LOSE;
	else return DualClickResult; // 罹晦縑 GS_LOSE 霜蕾 葬欐 旎雖
}

void ms_CreateMine(int x, int y)
{
	int i, j, k;
	int MineLeft = gb_Mines;

	// っ 觼晦陛 雖煆熱爾棻 濛戲賊 雖煆熱 營褻薑
	if((gb_SizeX * gb_SizeY) < gb_Mines)
		gb_Mines = (gb_SizeX * gb_SizeY) - 1;
	
	// 雖煆 褕晦
	while(MineLeft != 0)
	{
		j = Random_Int(0, gb_SizeX-1);
		k = Random_Int(0, gb_SizeY-1);
		
		if((gb_MainBoard[k][j].BoardData == 0) && (j != x || k != y)) // 纖 綴 爾萄 奢除檜堅 羅摹鷗 雖薄檜 嬴棲賊
		{
			gb_MainBoard[k][j].BoardData = 9; // 雖煆 褕擠
			MineLeft--;
		}
	}

	//gb_MainBoard[0][0].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[0][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][0].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[0][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[(gb_SizeY-1)/2][0].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[(gb_SizeY-1)/2][gb_SizeX-1].BoardData = 9; // ONLY FOR TEST //gb_MainBoard[gb_SizeY-1][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST
	//gb_MainBoard[(gb_SizeY-1)/2][(gb_SizeX-1)/2].BoardData = 9; // ONLY FOR TEST


	// 雖煆 輿滲 璋濠 隸陛
	for(i=0; i<=gb_SizeY-1; i++)
	{
		for(j=0; j<=gb_SizeX-1; j++)
		{
			if(gb_MainBoard[i][j].BoardData == 9) // 雖煆 嫦唯ж賊
			{
				if(i==0 && j==0) { ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 謝難 鼻欽檜賊
				else if(i==0 && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); } // 辦難 鼻欽檜賊
				else if(i==gb_SizeY-1 && j==0) { ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j+1, i); } // 謝難 ж欽檜賊
				else if(i==gb_SizeY-1 && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j-1, i); } // 辦難 ж欽檜賊
				else if(i==0 && (j>0 && j<gb_SizeX-1)) { ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j-1, i+1);
														 ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 賅憮葬 薯諼 鼻欽
				else if((i>0 && i<gb_SizeY-1) && j==0) { ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j+1, i);
														 ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 賅憮葬 薯諼 謝難
				else if((i>0 && i<gb_SizeY-1) && j==gb_SizeX-1) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j-1, i);
																  ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); } // 賅憮葬 薯諼 辦難
				else if(i==gb_SizeY-1 && (j>0 && j<gb_SizeX-1)) { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1);
																  ms_PlusBoardNumber(j-1, i); ms_PlusBoardNumber(j+1, i); } // 賅憮葬 薯諼 ж欽
				else { ms_PlusBoardNumber(j-1, i-1); ms_PlusBoardNumber(j, i-1); ms_PlusBoardNumber(j+1, i-1); ms_PlusBoardNumber(j-1, i);
					   ms_PlusBoardNumber(j+1, i); ms_PlusBoardNumber(j-1, i+1); ms_PlusBoardNumber(j, i+1); ms_PlusBoardNumber(j+1, i+1); } // 斜諼 醞懈
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

int ms_CheckWin() // 蝓葬 褻勒 っ欽 л熱
{
	int i, j;
	int BlockCounter = 0;

	// 賅萇 爾萄蒂 匐餌ж罹 雖煆陛 嬴棋 睡碟檜 嫩⑵螺朝陛蒂 �挫�
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


// 紫遺蜓 夢蝶 艙羲縑 譆渠 76byte望檜 臢蝶お蒂 轎溘п輿朝 л熱
void TypeBox(char String[76], int ColorFont, int ColorBack)
{
	EraseBox();

	MoveCur(2, 22);	
	BCprintf(String, ColorFont, ColorBack);
}

// 紫遺蜓 夢蝶 艙羲擊 奢寥戲煎 雖辦朝 л熱
void EraseBox()
{
	MoveCur(2, 22);
	printf("                                                                            ");
}

// 儀鼻 薑爾蒂 嫡嬴 僥濠蒂 轎溘ж朝 л熱. 儀鼻 餌辨擊 寰й衛 晦獄 夔樂 儀戲煎 轎溘
void BCprintf(char *String, int FontCR, int BackCR)
{
	if(gb_Colormode)
		Cprintf(String, FontCR, BackCR);
	
	else
		printf("%s", String);
}