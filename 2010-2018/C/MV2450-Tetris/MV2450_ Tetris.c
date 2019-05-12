/************************************************************************/
/* [ MV2450 - Device API ]                                              */
/*                                                                      */
/*  # Code by "2010305049 Lee Tae Hoon"                                 */
/*																		*/
/*	# Last update: 2014/6/28       			                            */
/*                                      								*/
/*									     	     ����  ����������������	*/
/*												 ����������������������	*/
/*												 ������  ����  ��������	*/
/************************************************************************/

#include "2450test.h"
#include "Image.h"

// Preprocessor
#define ON 0x01  // LED Control
#define OFF 0x00

#define BTN1 0x4 // BTN Control
#define BTN2 0x5
#define BTN3 0x6
#define BTN4 0x7
#define BTN5 0x8
#define BTN6 0x9

#define PCLK (266000000/4) // BUZ Reference
#define C1      523     // ��
#define D1      587     // ��
#define E1      659     // ��
#define F1      699     // ��
#define G1      784     // ��
#define A1      880     // ��
#define B1      988     // ��
#define C2      C1*2    // ���� ��
#define DLY_128	 156
#define DLY_64	 312
#define DLY_32	 625
#define DLY_24   937   
#define DLY_16   1250   // 1/16 musical note
#define DLY_8	 2500   // 1/8  musical note
#define DLY_4    5000   // 1/4  musical note
#define DLY_2    10000  // 1/2  musical note
#define DO1 131
#define DO1_S 139
#define RE1 147
#define RE1_S 156
#define ME1 165
#define PA1 175
#define PA1_S 185
#define SO1 196
#define SO1_S 208
#define RA1 220
#define RA1_S 233
#define SI1 247
#define DO2 (DO1*2)
#define RE2 (RE1*2)
#define ME2 (ME1*2)
#define PA2 (PA1*2)
#define SO2 (SO1*2)
#define SO2_S (SO1_S *2)
#define RA2 (RA1*2)
#define NO 0

#define CR_WHITE	0xFFFFFFFF // LCD Color Reference
#define CR_RED		0xF800F800
#define	CR_ORANGE	0xFA80FA80
#define CR_YELLOW	0xFFE0FFE0
#define CR_GREEN	0x07E007E0
#define CR_BLUE		0x001F001F
#define CR_DBLUE	0x441F441F
#define CR_PURPLE	0x480C480C
#define CR_BLACK	0x00000000
#define CR_TRANS	0xF81FF81F

#define SCR_LEFT  0 // LCD Scroll Direction
#define SCR_RIGHT 1
#define SCR_UP	  2
#define SCR_DOWN  3

#define LCD_RIGHT_BOUNDARY 	480 // TCH
#define LCD_LEFT_BOUNDARY  	0
#define LCD_UP_BOUNDARY    	0
#define LCD_DOWN_BOUNDARY  	272
#define GRID  		   		5 // TCH(calibrate)
#define GRID_CENTER    		GRID/2
#define TABLE_X     		LCD_RIGHT_BOUNDARY/GRID
#define TABLE_Y     		LCD_DOWN_BOUNDARY/GRID

// *Preprocessor (Extra API)
#define IMAGE_MAX_SPRITE 50	// ��������Ʈ ���� �ִ� ����


// Function Prototype
// LED
void LED_Init(void);
void LED_SetOnoff(int ledNum, int ledState);
void LED_ClrLED();
void LED_Segment(int i);
// BTN
void BTN_Init();
bool BTN_isPushed(int btnID);
// BUZ
void BUZ_Init();
void BUZ_play(unsigned long scale, unsigned short length);
void BUZ_stop();
// RTC
void Tick_Count_Set(U32 count);
void RTC_Tick_Time(void);
void __irq RTC_Tick_ISR(void);
void RTC_setTime(int year,int month,int date,int weekday,int hour,int min,int sec);
void RTC_getTime(int *year, int *month, int *date, int *weekday, int *hour, int *min, int *sec);
void RTC_printTime(void);
void RTC_tick_routine();
// LCD
void MyLCD_Clear(uint32 c);
void LCD_PutPixel(uint32 x, uint32 y, uint32 color);
void LCD_DrawLine(int32 x1, int32 y1, int32 x2, int32 y2, int32 color);
void LCD_DrawRect(int32 x1, int32 y1, int32 x2, int32 y2, int32 color);
void LCD_FillRect(int32 x1, int32 y1, int32 x2, int32 y2, int32 color);
void LCD_DrawCircle(int32 x, int32 y, int32 radius, uint32 color);
void LCD_DrawEllipse(int32 x, int32 y, int32 radius, uint32 color);
void LCD_FillEllipse(int32 nCenterX, int32 nCenterY, int32 radius, int32 nColor);
void LCD_DrawImage(int32 x, int32 y, int32 sizeX, int32 sizeY, const int image[]);
void LCD_DrawImageEx(int32 bufX, int32 bufY, int32 imgX, int32 imgY, int32 sizeX, int32 sizeY, const int image[], int32 transColor);
void LCD_ScrollImage(int SCR_DIRECTION, int scrPixel);
void LCD_Invalidate(bool doClr);
// TCH
void MakeTable(void);
void calibrate(void);
void Init_AdcTs(void);
void __irq AdcTs_Auto(void);
// BTH
void Init_Bluetooth(void);
void __irq Receive_Uart(void);
unsigned int Bluetooth_Putc(unsigned int c);
unsigned int Bluetooth_Getc(void);
char Bluetooth_GetKey_t(void);

// *Function Prototype (Extra API)
void LCD_Sprite(int x, int y, int spriteID, int milsec, int alphaVal); // ��������Ʈ �̹��� �����ϴ� �Լ�
void LCD_DrawSpriteImg(); // ��������Ʈ �̹��� ����ϴ� �Լ�
void LCD_DrawAlphaImage(int32 bufX, int32 bufY, int32 imgX, int32 imgY, int32 sizeX, int32 sizeY, const int image[], int32 transColor, int32 alphaRate);

// Global Variable
bool g_btnFlag[6] = { FALSE }; // BTN
unsigned int BackBuffer[240*272]; // LCD
volatile U32 Tick = 0; // RTC
char *day[8] = {" ","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"}; // RTC
int tx[6], ty[6]; // TCH
int fx[3] = {1, 0, -1}; // TCH
int fy[3] = {0, -1, -1}; // TCH
int cal_x[TABLE_X+1]; // TCH
int cal_y[TABLE_Y+1]; // TCH
unsigned int ADC_x, ADC_y; // TCH
int tmp_xdata, tmp_ydata; // TCH
extern int adcpsr; // TCH
extern int adcclock; // TCH
extern int xtal; // TCH
extern volatile int xdata, ydata; // TCH
extern int count; // TCH
int TP_X1 = TABLE_X/10; // TCH
int TP_Y1 = TABLE_Y/10; // TCH
int TP_X2 = TABLE_X-TABLE_X/10; // TCH
int TP_Y2 = TABLE_Y-TABLE_Y/10; // TCH
int TP_CX = TABLE_X/2; // TCH
int TP_CY = TABLE_Y/2; // TCH
int cal_cnt = -1; // TCH
unsigned int bt_data; // BTH

// *Global Variable (Extra API)
typedef struct tag_sprite {
	int spriteID;		 // ��������Ʈ ID
	int bufX, bufY;		 // ��������Ʈ ȭ����� ��ǥ
	int imgX, imgY;		 // ��������Ʈ �̹��� ���� ����
	int sizeX, sizeY;	 // ��������Ʈ �̹��� ������
	int spCnt;			 // ��������Ʈ �̹��� ����
	int currentTime;	 // ��������Ʈ�� ���� �ð�
	int givenTime;		 // ��������Ʈ�� �־����� �ð�
	const int *targetImg;// ��������Ʈ Ÿ�� �̹�������
	int alpha;			 // ��������Ʈ ���İ� ����
} sprite;
int g_spriteGroupCounter = 0;			// ��������Ʈ �׷� ī����
sprite spriteGroup[IMAGE_MAX_SPRITE];	// ��������Ʈ �׷�
sprite spriteData[27] = { {0,  0, 0, 0,   0,  56, 148, 3, 0, 0,EffectImage, 0}, // Combo
						  {1,  0, 0, 124, 292,56, 24, 3, 0, 0, EffectImage, 0}, // 0
						  {2,  0, 0, 124, 238,54, 18, 3, 0, 0, EffectImage, 0}, // 1
						  {3,  0, 0, 124, 168,54, 22, 3, 0, 0, EffectImage, 0}, // 2
  						  {4,  0, 0, 124, 90, 54, 26, 3, 0, 0, EffectImage, 0}, // 3
   						  {5,  0, 0, 124, 0,  56, 30, 3, 0, 0, EffectImage, 0}, // 4
   						  {6,  0, 0, 66, 294, 54, 24, 3, 0, 0, EffectImage, 0}, // 5
   						  {7,  0, 0, 66, 222, 56, 24, 3, 0, 0, EffectImage, 0}, // 6
   						  {8,  0, 0, 66, 150, 54, 24, 3, 0, 0, EffectImage, 0}, // 7
   						  {9,  0, 0, 66, 72,  56, 26, 3, 0, 0, EffectImage, 0}, // 8
   						  {10, 0, 0, 66, 0,   56, 24, 3, 0, 0, EffectImage, 0}, // 9
   						  {11, 0, 0, 0,  148, 18, 18, 9, 0, 0, EffectImage, 0}, // Block_Break
   						  {12, 0, 0, 420,0,   60, 68, 7, 0, 0, EffectImage, 0},	// Missile_Bomb Effect
   						  {13, 0, 0, 0,  0,   158,260,1, 0, 0, Dialog, 0},		// Debuff Diagram
   						  {14, 0, 0, 158,0,   158,260,1, 0, 0, Dialog, 0},		// Buff Diagram
   						  {15, 0, 0, 0,  42,  50, 50, 1, 0, 0, MiscImage, 0},	// Item Cloude Icon
   						  {16, 0, 0, 50, 42,  50, 50, 1, 0, 0, MiscImage, 0},	// Item Missile Icon
   						  {17, 0, 0, 100,42,  50, 50, 1, 0, 0, MiscImage, 0},	// Item Ghost Icon
   						  {18, 0, 0, 0,  92,  50, 50, 1, 0, 0, MiscImage, 0},	// Item Reverse Icon
   						  {19, 0, 0, 50, 92,  50, 50, 1, 0, 0, MiscImage, 0},	// Item Hardblock Icon
   						  {20, 0, 0, 100,92,  50, 50, 1, 0, 0, MiscImage, 0},	// Item Timestop Icon
   						  {21, 0, 0, 0,  142, 50, 50, 1, 0, 0, MiscImage, 0},	// Item Timeback Icon
						  {22, 0, 0, 50, 142, 50, 50, 1, 0, 0, MiscImage, 0},	// Item Shield Icon
						  {23, 0, 0, 100,142, 50, 50, 1, 0, 0, MiscImage, 0},	// Item Removeline Icon
						  {24, 0, 0, 0,  192, 50, 50, 1, 0, 0, MiscImage, 0},	// Item Lineblock Icon
						  {25, 0, 0, 50, 192, 50, 50, 1, 0, 0, MiscImage, 0},	// Item Changeline Icon
						  {26, 0, 0, 100,192, 50, 50, 1, 0, 0, MiscImage, 0},	// Item Ironblock Icon
						}; // ��������Ʈ ������ ����

const int SPRITE_COMBO = 0;
const int SPRITE_COMBO_NUM0 = 1;		const int SPRITE_COMBO_NUM1 = 2;		const int SPRITE_COMBO_NUM2 = 3;
const int SPRITE_COMBO_NUM3 = 4;		const int SPRITE_COMBO_NUM4 = 5;		const int SPRITE_COMBO_NUM5 = 6;
const int SPRITE_COMBO_NUM6 = 7;		const int SPRITE_COMBO_NUM7 = 8;		const int SPRITE_COMBO_NUM8 = 9;
const int SPRITE_COMBO_NUM9 = 10;		const int SPRITE_BLOCK_BREAK = 11;
const int SPRITE_EFFECT_MISSILE = 12;	const int SPRITE_DIAGRAM_DEBUFF = 13;	const int SPRITE_DIAGRAM_BUFF = 14;
const int SPRITE_ITEM_CLOUDE = 15;		const int SPRITE_ITEM_MISSILE = 16;		const int SPRITE_ITEM_GHOST = 17;
const int SPRITE_ITEM_REVERSE = 18;		const int SPRITE_ITEM_HARDBLOCK = 19;	const int SPRITE_ITEM_TIMESTOP = 20;
const int SPRITE_ITEM_TIMEBACK = 21;	const int SPRITE_ITEM_SHIELD = 22;		const int SPRITE_ITEM_REMOVELINE = 23;
const int SPRITE_ITEM_LINEBLOCK = 24;	const int SPRITE_ITEM_CHANGELINE = 25;	const int SPRITE_ITEM_IRONBLOCK = 26;

// ��Ʈ���� ���� ����
typedef struct tag_note {
	int scale;
	int delay;
} note;


/*
const int MUSIC_MAX_NOTE = 51;
note g_music[MUSIC_MAX_NOTE] = { {ME2,DLY_16},{NO,DLY_32},{SI1,DLY_24},{DO2,DLY_24},{RE2,DLY_16},{NO,DLY_32},{DO2,DLY_24},{SI1,DLY_24},{RA1,DLY_16},{NO,DLY_32}, {RA1,DLY_24},{DO2,DLY_24}, // 10
     							 {ME2,DLY_16},{NO,DLY_32},{RE2,DLY_24},{DO2,DLY_24},{SI1,DLY_16},{NO,DLY_32},{SI1,DLY_16},{DO2,DLY_24},{RE2,DLY_24},{ME2,DLY_16},{DO2,DLY_24},{RA1,DLY_16}, // 20
     							 {RA1,DLY_16},{NO,DLY_32}, {RE2,DLY_24},{PA2,DLY_24},{RA2,DLY_16},{SO2,DLY_24},{PA2,DLY_24},{NO,DLY_32},{ME2,DLY_16},{NO,DLY_32},{DO2,DLY_24},{ME2,DLY_24}, // 30
     							 {NO,DLY_32},{RE2,DLY_24},{DO2,DLY_24},{NO,DLY_32},{SI1,DLY_32},{NO,DLY_32},{SI1,DLY_32},{DO2,DLY_24},{RE2,DLY_16},{NO,DLY_32},{ME2,DLY_16},{DO2,DLY_32},{RA1,DLY_16},{RA1,DLY_16}, // 40
     							 {NO,DLY_16} }; // 43
*/
const int MUSIC_MAX_NOTE = 182;
note g_music[MUSIC_MAX_NOTE] =
{ {ME2,DLY_16},{NO,DLY_32},{SI1,DLY_24},{DO2,DLY_24},{RE2,DLY_16},{NO,DLY_32},{DO2,DLY_24},{SI1,DLY_24},{RA1,DLY_16},{RA1,DLY_16},{NO,DLY_32},{DO2,DLY_24},{ME2,DLY_24},{NO,DLY_32},{RE2,DLY_24},{DO2,DLY_24},{SI1,DLY_16},{NO,DLY_32},{SI1,DLY_24},{DO2,DLY_24},{RE2,DLY_24},{NO,DLY_32},{ME2,DLY_24},{NO,DLY_32},{DO2,DLY_24},{NO,DLY_32},{RA1,DLY_24},{NO,DLY_32},{RA1,DLY_16},{NO,DLY_32},{NO,DLY_32},
  {RE2,DLY_24},{PA2,DLY_24},{RA2,DLY_16},{NO,DLY_32},{SO2,DLY_24},{PA2,DLY_24},{ME2,DLY_16},{NO,DLY_32},{NO,DLY_32},{DO2,DLY_24},{ME2,DLY_24},{NO,DLY_32},{RE2,DLY_24},{DO2,DLY_24},{SI1,DLY_24},{NO,DLY_32},{SI1,DLY_24},{DO2,DLY_24},{RE2,DLY_24},{NO,DLY_32},{ME2,DLY_24},{NO,DLY_32},{DO2,DLY_24},{NO,DLY_32},{RA1,DLY_24},{NO,DLY_32},{RA1,DLY_16},{NO,DLY_32},{NO,DLY_32}, // x1
  {ME2,DLY_16},{NO,DLY_32},{SI1,DLY_24},{DO2,DLY_24},{RE2,DLY_16},{NO,DLY_32},{DO2,DLY_24},{SI1,DLY_24},{RA1,DLY_16},{RA1,DLY_16},{NO,DLY_32},{DO2,DLY_24},{ME2,DLY_24},{NO,DLY_32},{RE2,DLY_24},{DO2,DLY_24},{SI1,DLY_16},{NO,DLY_32},{SI1,DLY_24},{DO2,DLY_24},{RE2,DLY_24},{NO,DLY_32},{ME2,DLY_24},{NO,DLY_32},{DO2,DLY_24},{NO,DLY_32},{RA1,DLY_24},{NO,DLY_32},{RA1,DLY_16},{NO,DLY_32},{NO,DLY_32},
  {RE2,DLY_24},{PA2,DLY_24},{RA2,DLY_16},{NO,DLY_32},{SO2,DLY_24},{PA2,DLY_24},{ME2,DLY_16},{NO,DLY_32},{NO,DLY_32},{DO2,DLY_24},{ME2,DLY_24},{NO,DLY_32},{RE2,DLY_24},{DO2,DLY_24},{SI1,DLY_24},{NO,DLY_32},{SI1,DLY_24},{DO2,DLY_24},{RE2,DLY_24},{NO,DLY_32},{ME2,DLY_24},{NO,DLY_32},{DO2,DLY_24},{NO,DLY_32},{RA1,DLY_24},{NO,DLY_32},{RA1,DLY_16},{NO,DLY_32},{NO,DLY_32}, // x2
  {ME2,DLY_32},{ME2,DLY_32},{ME2,DLY_32},{ME2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{RE2,DLY_32},{RE2,DLY_32},{RE2,DLY_32},{RE2,DLY_32},{SI1,DLY_32},{SI1,DLY_32},{SI1,DLY_32},{SI1,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{RA1,DLY_32},{RA1,DLY_32},{RA1,DLY_32},{RA1,DLY_32},{SO1_S,DLY_32},{SO1_S,DLY_32},{SO1_S,DLY_32},{SO1_S,DLY_32},{SO1_S,DLY_32},{SO1_S,DLY_32},{SO1_S,DLY_32},{SO1_S,DLY_32},
  {ME2,DLY_32},{ME2,DLY_32},{ME2,DLY_32},{ME2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{DO2,DLY_32},{RE2,DLY_32},{RE2,DLY_32},{RE2,DLY_32},{RE2,DLY_32},{SI1,DLY_32},{SI1,DLY_32},{SI1,DLY_32},{SI1,DLY_32},{RA1,DLY_16},{NO,DLY_32},{DO2,DLY_24},{ME2,DLY_24},{RA2,DLY_24},{NO,DLY_32},{RA2,DLY_24},{NO,DLY_32},{SO2_S,DLY_16},{NO,DLY_32},{NO,DLY_32},{NO,DLY_32},{NO,DLY_32} // �ķ� ��
};
int g_musicCurrent = 0;


// API �ʱ�ȭ ����
// LED �ʱ�ȭ
void LED_Init(void)
{
	rGPHCON = (rGPHCON & ~(0x3<<26))  | (1<<26);	// nLED1 (GPH13)
	rGPGCON	= (rGPGCON & ~(0x3<<4)) | (0x1<<4);		// nLED2 (GPG2)
	rGPHCON = (rGPHCON & ~(0x3<<28))  | (1<<28);	// nLED3 (GPH14)
	rGPGCON	= (rGPGCON & ~(0x3<<6)) | (0x1<<6);		// nLED4 (GPG3)
	// LED Output Setting
	
	rGPHUDP = (rGPHUDP & ~(0x3<<26)) | (0x0<<26);	
	rGPGUDP	= (rGPGUDP & ~(0x3<<4))  | (0x0<<4);		
	rGPHUDP = (rGPHUDP & ~(0x3<<28)) | (0x0<<28);	
	rGPGUDP	= (rGPGUDP & ~(0x3<<6))  | (0x0<<6);	
	// Pull up/down Disable 	
	
	LED_ClrLED();
	// All LED off
}

// LED���� ON, OFF����
void LED_SetOnoff(int ledNum, int ledState)
{
	switch(ledNum) {
		case 1:
			if(ledState == ON)
				rGPHDAT = (rGPHDAT & ~(ledState << 13));	// LED 1 ON
			else
				rGPHDAT = (rGPHDAT | (!(ledState) << 13));	// LED 1 OFF
			break;
		case 2:
			if(ledState == ON)
				rGPGDAT = (rGPGDAT & ~(ledState << 2));		// LED 2 ON
			else
				rGPGDAT = (rGPGDAT | (!(ledState) << 2));	// LED 2 OFF
			break;
		case 3:
			if(ledState == ON)
				rGPHDAT = (rGPHDAT & ~(ledState << 14));	// LED 3 ON
			else
				rGPHDAT = (rGPHDAT | (!(ledState) << 14));	// LED 3 OFF
			break;
		case 4:
			if(ledState == ON)
				rGPGDAT = (rGPGDAT & ~(ledState << 3));		// LED 4 ON
			else
				rGPGDAT = (rGPGDAT | (!(ledState) << 3));	// LED 4 OFF
			break;
		default: break;	
	}
}

// ��� LED �ҵ�
void LED_ClrLED()
{
	LED_SetOnoff(1, OFF);
	LED_SetOnoff(2, OFF);
	LED_SetOnoff(3, OFF);
	LED_SetOnoff(4, OFF);
}

// i���� 2������ LED���
void LED_Segment(int i)
{
	int bit[4] = { 0 };
	
	bit[0] = (i & (0x0001));
	bit[1] = ((i & (0x0002)) >> 1);
	bit[2] = ((i & (0x0004)) >> 2);
	bit[3] = ((i & (0x0008)) >> 3);

	LED_SetOnoff(1, bit[0]);
	LED_SetOnoff(2, bit[1]);
	LED_SetOnoff(3, bit[2]);
	LED_SetOnoff(4, bit[3]);
}

void BTN_Init() 
{
	// 	GPGCON(Push Button) Input Setting
	rGPGCON = (rGPGCON & ~(0xfff << 8)); // GPG4 ~ GPG9 

	// 	Pull up/down Disable
	rGPGUDP	= (rGPGUDP & ~(0xfff << 8));	
}

// �� �Լ��� ����� �� ��ư�� ���� ������ TRUE, �ƴϸ� FALSE��ȯ
bool BTN_isPushed(int btnID)
{
	bool isPushed = FALSE;
	
	isPushed = !(rGPGDAT & (0x1 << btnID));	// BTN Pushed
	
	return isPushed;
}

void BUZ_Init() 
{
	rGPBCON = (rGPBCON & ~(0x3<<2)) | (0x2<<2);	// Config TOUT1
	rGPBUDP = (rGPBUDP & ~(0x3<<2)) | (0x1<<2);	// GPBUDP1
}

void BUZ_play(unsigned long scale, unsigned short length)
{
	rTCON=(rTCON & ~(0xf<<8));
	
	if(scale != NO) {
		// Ŭ�� ���� ���(Hz ������ ����)
		rTCNTB1  = (PCLK/64) / (scale*2) - 1;
		rTCMPB1  = (rTCNTB1) / 2;
		rTCON   |= (0x2<<8);
		rTCON    = (rTCON & ~(0xf<<8)) | (0x09<<8);
		// Interval Mode, Inverter Off, Start
	}
	
	rWTCON = (0x27<<8) + (0x3<<3) + (0x0<<0);	// resolution=0.1ms
    rWTDAT = length + 10;				// Using WDT
    rWTCNT = length + 10;
    	
    rWTCON |= (1<<5);

    while(rWTCNT>10);
    	rWTCON = 0;
}

void BUZ_stop()
{
	int save_BUP, save_B;
	save_BUP = rGPBUDP;
	save_B  = rGPBCON;

	rTCON=(rTCON& ~(0xf<<8))|(0x0<<8);          // Stop   
	rGPBUDP = save_BUP;
	rGPBCON = save_B;
}

void Tick_Count_Set(U32 count) {
	
	U32 temp;

	temp = (count-1)%32768;
	rTICNT2 = (count-1)/32768;
	rTICNT0 = temp / 256;
	rTICNT1 = temp % 256;
}

void RTC_Tick_Time(void) {
	rRTCCON = (rRTCCON & ~(0x1<<4)) | (0x1<<4);
	// Uart_Printf(" rtcon = %08x\n", rRTCCON);
 	Tick_Count_Set(32768);	

  	pISR_TICK = (U32)RTC_Tick_ISR; // ��������������������������������
   	rINTMSK &= ~(BIT_TICK);					    				 // ��
								  								 // ��
   	rTICNT0 |= (1<<7);	// Tick Time Interrupt Enable	     	 // ��
}															     // ��
															     // ��
															     // ��
void __irq RTC_Tick_ISR(void) { // <����������������������������������

	// Add "tick-action" here... (�ֱ������� ���ͷ�Ʈ �����Ͽ� �Լ� ȣ��: �ݹ� �Լ�?)
	// ...
	// RTC_printTime();
	// ...
	RTC_tick_routine();	// �� �Լ����� 1�ʸ��� ����

	rSRCPND = BIT_TICK;
   	rINTPND = BIT_TICK;
}

// �Ű������� ���޹��� �ð��� ������ �������Ϳ� BTC�ð����� �����Ͽ� ����
void RTC_setTime(int year,int month,int date,int weekday,int hour,int min,int sec) {

	rRTCCON  = rRTCCON  | 0x1;					
   	//RTC Control enable

   	rBCDYEAR = (rBCDYEAR & ~0xff)  | (( ((year / 10) << 4) + (year  % 10) ));
   	rBCDMON  = (rBCDMON  & ~0x1f)  | (( ((month/ 10) << 4) + (month % 10) ));
   	rBCDDATE = (rBCDDATE & ~0x3f)  | (( ((date / 10) << 4) + (date  % 10) ));     	
   	rBCDDAY  = (rBCDDAY  & ~(0x7)) | weekday;		
   	//SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
   	
   	rBCDHOUR =(rBCDHOUR & ~0x3f) | (( ((hour / 10) << 4) + (hour % 10) ));
   	rBCDMIN  =(rBCDMIN & ~0x7f)  | (( ((min  / 10) << 4) + (min  % 10) ));
   	rBCDSEC  =(rBCDSEC & ~0x7f)  | (( ((sec  / 10) << 4) + (sec  % 10) ));     	
}

// ������ �������Ϳ� ����� BTC�ð��� ���� �ð����� ��ȯ�Ͽ� �Ű������� ����
void RTC_getTime(int *year, int *month, int *date, int *weekday, int *hour, int *min, int *sec) {
	
   	*year 	= 2000 + (((rBCDYEAR & 0xf0) >> 4) * 10) + (rBCDYEAR & 0xf);
	*month   = (((rBCDMON & 0x10)  >> 4) * 10) + (rBCDMON  & 0xf);
	*date    = (((rBCDDATE & 0x70) >> 4) * 10) + (rBCDDATE & 0xf);
	*weekday = rBCDDAY;
  	
  	*hour    = (((rBCDHOUR & 0x30) >> 4) * 10) + (rBCDHOUR & 0xf);
  	*min     = (((rBCDMIN & 0x70)  >> 4) * 10) + (rBCDMIN  & 0xf);
	*sec     = (((rBCDSEC & 0x70)  >> 4) * 10) + (rBCDSEC  & 0xf);
}

// ���� �������Ϳ� ����� �ð��� �����ͼ� ���
void RTC_printTime(void) {
	
	int year,month,date,hour,min,sec,weekday;

   	RTC_getTime(&year, &month, &date, &weekday, &hour, &min, &sec);

	//          �ð�   ��    ��   ����   ��  ��  �⵵
	Uart_Printf("%2d : %2d : %2d  %10s,  %2d/%2d/%4d\n",hour,min,sec,day[weekday],month,date,year);
}

// LCD�� �ش� ������ ü��� �Լ�
void MyLCD_Clear(uint32 color) {
	int32 i, j;

	for(i = 0; i < 240*272; i++)
		BackBuffer[i] = color;		
}

// �ȼ��� ���� �����ϴ� �Լ�
void LCD_PutPixel(uint32 x, uint32 y, uint32 color) {
	if(x<screen_width && y<screen_height)
		*(uint32 *)(BackBuffer+y*screen_width/2+x/2) = 
			(*(uint32 *)(BackBuffer+y*screen_width/2+x/2) & ~(0xffff0000>>(x%2)*16))|((color&0x0000ffff)<<((2-1-(x%2))*16));
}

// ������ �׸��� �Լ�
void LCD_DrawLine(int32 x1, int32 y1, int32 x2, int32 y2, int32 color) {
	int32 dx, dy, e;
	dx = x2-x1; 
	dy = y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					LCD_PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}

// ���۶ո� �׸� �ش� ������ �׸��� �Լ�
void LCD_DrawRect(int32 x1, int32 y1, int32 x2, int32 y2, int32 color) {
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x2, y1, x2, y2 ,color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
}

// ���� ä���� �׸� �ش� ������ �׸��� �Լ�
void LCD_FillRect(int32 x1, int32 y1, int32 x2, int32 y2, int32 color) {
	int32 i;

	for(i = y1; i <= y2; i++)
		LCD_DrawLine(x1, i, x2, i, color);
}

// �� �׸���
void LCD_DrawCircle(int32 x, int32 y, int32 radius, uint32 color) {

	int nRadius = radius;
  	int nCenterX = x, nCenterY = y;
	int nStartX, nStartY;
  	int nFinder;
 
   	nStartX = 0;
  	nStartY = nRadius;
  	nFinder = 3 - 2 * nRadius;
 
  	while(nStartX <= nStartY) {
	    LCD_PutPixel(nCenterX + nStartX, nCenterY + nStartY, color);
	    LCD_PutPixel(nCenterX + nStartX, nCenterY - nStartY, color);
	    LCD_PutPixel(nCenterX - nStartX, nCenterY + nStartY, color);
	    LCD_PutPixel(nCenterX - nStartX, nCenterY - nStartY, color);
	    LCD_PutPixel(nCenterX + nStartY, nCenterY + nStartX, color);
	    LCD_PutPixel(nCenterX + nStartY, nCenterY - nStartX, color);
	    LCD_PutPixel(nCenterX - nStartY, nCenterY + nStartX, color);
	    LCD_PutPixel(nCenterX - nStartY, nCenterY - nStartX, color);
    	if(nFinder < 0) {
		    nFinder += 4 * nStartX + 6;
		    nStartX++;
	    }
	    else {
		    nFinder += 4 * (nStartX - nStartY) + 10;
		    nStartX++;
		    nStartY--;
	    }
	}  
}

// ä���� �� �׸���(��ä�� ����)
void LCD_DrawEllipse(int32 x, int32 y, int32 radius, uint32 color) {
	

}

// ä���� �� �׸���
void LCD_FillEllipse(int32 nCenterX, int32 nCenterY, int32 radius, int32 nColor) {

	int x, y;
	int nRadius = radius;
	
	for(x = nCenterX - nRadius; x < nCenterX + nRadius; x++)
		for(y = nCenterY - nRadius; y < nCenterY + nRadius; y++)
			if( ((nCenterX-x) * (nCenterX-x)) + ((nCenterY - y) * (nCenterY - y)) <= nRadius * nRadius)
				LCD_PutPixel(x, y, nColor);
					
	// if�� ���� ��ȣ(==)�� �����Ͽ� FillEllipse ���� ����
	// == : ���� �׵θ�(err)
	// <= : ���� �׵θ��� �� ���� ä���
	// >  : x, y������������ �� ���θ� �����Ͽ� ä���
	// ���� �������� ������ ����
	// (x-r) * (x-r) + (y-r) * (y-r) = r*r;
}

// ���� ȭ���� Ư�� �������� srcPixel��ŭ ��ũ�� ��Ŵ
void LCD_ScrollImage(int SCR_DIRECTION, int scrPixel) {
	int i = 0, j = 0;
	int lcdTemp[272] = { 0xFFFFFFFF };
	
	// ���� ���� ��ũ��
	if(SCR_DIRECTION == SCR_LEFT) {
		// �������� 2�ȼ����� �޸𸮿� ����
		for(i = 0; i < 272; i++)
			lcdTemp[i] = BackBuffer[(i*240)+0];
		// ���� ���ۿ� �׷��� �̹����� �������� 2�ȼ� �̵�
		for(i = 1; i < 240; i++)
			for(j = 0; j < 272; j++)
				BackBuffer[(j*240)+(i-1)] = BackBuffer[(j*240)+i];
		// ����� �ȼ��� ���� �� ������ ���
		for(i = 0; i < 272; i++)
			BackBuffer[(i*240)+239] = lcdTemp[i];
	}
	
	// ������ ���� ��ũ��
	else if(SCR_DIRECTION == SCR_RIGHT) {
		// �������� 2�ȼ����� �޸𸮿� ����
		for(i = 0; i < 272; i++)
			lcdTemp[i] = BackBuffer[(i*240)+239];
		// ���� ���ۿ� �׷��� �̹����� �������� 2�ȼ� �̵�
		for(i = 238; i >= 0; i--)
			for(j = 0; j < 272; j++)
				BackBuffer[(j*240)+(i+1)] = BackBuffer[(j*240)+i];
		// ����� �ȼ��� ���� �� ������ ���
		for(i = 0; i < 272; i++)
			BackBuffer[(i*240)+0] = lcdTemp[i];
	}
	
	// ���� ���� ��ũ��
	else if(SCR_DIRECTION == SCR_UP) {
		// ����� 1�ȼ��� �޸𸮿� ����
		for(i = 0; i < 240; i++)
			lcdTemp[i] = BackBuffer[i];
		// ���� ���ۿ� �׷��� �̹����� �������� 1�ȼ� �̵�
		for(i = 0; i < 240; i++)
			for(j = 0; j < 271; j++)
				BackBuffer[(j*240)+i] = BackBuffer[((j+1)*240)+i];
		// ����� �ʼ��� �ϴ� �� ������ ���
		for(i = 0; i < 240; i++)
			BackBuffer[i+271*240] = lcdTemp[i];
	}
	
	// �Ʒ��� ���� ��ũ��
	else if(SCR_DIRECTION == SCR_DOWN) {
		// �ϴ��� 1�ȼ��� �޸𸮿� ����
		for(i = 0; i < 240; i++)
			lcdTemp[i] = BackBuffer[i+271*240];
		// ���� ���ۿ� �׷��� �̹����� �Ʒ��� 1�ȼ� �̵�
		for(i = 0; i < 240; i++)
			for(j = 270; j >= 0; j--)
				BackBuffer[(j+1)*240+i] = BackBuffer[(j*240)+i];
		// ����� �ȼ��� ��� �� ������ ���
		for(i = 0; i < 240; i++)
			BackBuffer[i] = lcdTemp[i];
	}
}

// �̹����� �׸��� �Լ�
void LCD_DrawImage(int32 x, int32 y, int32 sizeX, int32 sizeY, const int image[]) {

	int i = 0, j = 0;
	
	if(sizeX > 480) sizeX = 480; // Size Limit
	if(sizeY > 272) sizeY = 272;
	
	for(i = x/2; i < sizeX/2; i++)
		for(j = y; j < sizeY; j++)
			if((i/2 < 240) && (i/2 >= 0) && (j < 272) && (j >= 0))
				BackBuffer[i+j*240] = image[i+j*240];
}

// �̹����� �׸��� �Լ� (����� ����)
void LCD_DrawImageEx(int32 bufX, int32 bufY, int32 imgX, int32 imgY, int32 sizeX, int32 sizeY, const int image[], int32 transColor) {

	int i = 0, j = 0;

	if(sizeX > 480) sizeX = 480; // Size Limit
	if(sizeY > 272) sizeY = 272;
	
	for(i = 0; i < sizeX/2; i++)
		for(j = 0; j < sizeY; j++)
			if((image[(i+imgX/2) + ((imgY+j)*240)] != transColor)) { // 32��Ʈ ��ü�� transColor���̸� �׸��� �۾� ����
				if((i+bufX/2 < 240) && (i+bufX/2 >= 0) && (j+bufY < 272) && (j+bufY >= 0) && // ���÷��� ���� ������ Ȯ��
				   (i+imgX/2 < IMAGE_X/2) && (i+imgX/2 >= 0) && (j+imgY < IMAGE_Y) && (j+imgY >= 0)) {
					// ���� 16��Ʈ�� ������̸� ���� 16��Ʈ�� ���
					if((image[(i+imgX/2) + ((imgY+j)*240)] & 0x0000FFFF) == (transColor & 0x0000FFFF)) {
						BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = ((BackBuffer[(i+bufX/2) + ((bufY+j)*240)] & 0x0000FFFF) | (image[(i+imgX/2) + ((imgY+j)*240)] & 0xFFFF0000));
					}
					// ���� 16��Ʈ�� ������̸� ���� 16��Ʈ�� ���
					else if((image[(i+imgX/2) + ((imgY+j)*240)] & 0xFFFF0000) == (transColor & 0xFFFF0000))
						BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = ((BackBuffer[(i+bufX/2) + ((bufY+j)*240)] & 0xFFFF0000) | (image[(i+imgX/2) + ((imgY+j)*240)] & 0x0000FFFF));
					// �� �ܴ� �� �ȼ� �� ���
					else
						BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = image[(i+imgX/2) + ((imgY+j)*240)];
				}
		    }
}

void LCD_Invalidate(bool doClr) {

	int i = 0;
	
	if(doClr)
		for(i = 0; i < 240*272; i++)
			FrameBuffer[i] = CR_WHITE;
			
	memcpy(FrameBuffer, BackBuffer, 240*272*sizeof(int));
	MyLCD_Clear(CR_WHITE);
}

// ��ġ��ũ�� �ʱ�ȭ �Լ�
void Init_AdcTs(void)
{ 
	adcpsr=(int)((PCLK/adcclock)-1);
	
	rADCDLY=(int)(xtal/1000)*2;	// 2ms delay(filter effect)

	rADCCON = rADCCON & ~(0xffff) | 		(1<<14) 	  		// ADC prescaler enabel
								  |			(adcpsr <<6) 		// ADC prescaler value
								  |			(1<<3)		   		// ADC resolution selection
								  |			(0<<2)				// Normal operation mode
								  |			(1<<1);				// read start

	rADCTSC = 0xd3;  //Wfint,XP_PU_dis,XP_Dis,XM_Dis,YP_Dis,YM_En
	// 	   0	| 	  1  	|  	  1	 	| 	  0		|	  1		|      0	 |     0 	  |	  1		  1	 |
	// 	UD_SEN	| 	YM_SEN	|	YP_SEN	|	XM_SEN	|	XP_SEN	|   PULL_UP	 |	AUTO_PST  |		XY_PST	 |
	pISR_ADC = (unsigned)AdcTs_Auto;	// Auto measurement

	rSUBSRCPND |= BIT_SUB_ADC;// Clear previous pending bit
	
	rINTMSK &=~BIT_ADC;       // ADC Touch Screen Mask bit clear
	rINTSUBMSK &=~(BIT_SUB_ADC | BIT_SUB_TC);

	Uart_Printf("\nWating Stylus Down... \n\n");

	/*
	Uart_Getc();
	count=0;
		
	rINTSUBMSK |= (BIT_SUB_ADC|BIT_SUB_TC);
	rINTMSK |= (U32)BIT_ADC;
	rADCCON &= ~(1<<14);// disable prescaler
	rADCCON |= (1<<2);	// Standby
	Uart_Printf("[Touch Screen Test.]\n");
	*/
}

// Ķ���극�̼� ��� ����
void MakeTable(void)
{

	Uart_Printf("\n\n---calibration mode----------------------------\n");
	Uart_Printf("---stylus down on red pixel point TWICE\n");	

	cal_cnt = 1;
	for(int i=0;i<=TABLE_X;i++)						// table reset
		cal_x[i] = GRID*i + GRID_CENTER;
	for(int i=0;i<=TABLE_Y;i++)
		cal_y[i] = GRID*i + GRID_CENTER;
		
	for(int i=0;i<LCD_RIGHT_BOUNDARY;i++)			// lcd clear
		for(int j=0;j<LCD_DOWN_BOUNDARY;j++)
			PutPixel(i, j, 0xFFFF);
	
	LCD_FilledRectangle(cal_x[TP_X1]-GRID_CENTER, cal_y[TP_Y1]-GRID_CENTER, cal_x[TP_X1]+GRID_CENTER, cal_y[TP_Y1]+GRID_CENTER, 0xf800);	
	// stylus point 1
}

// Ķ���극�̼�
void calibrate(void)
{
	tx[cal_cnt-1] = ADC_x;
	ty[cal_cnt-1] = ADC_y;
	Uart_Printf("TX : %d TY : %d\n",ADC_x,ADC_y);
	
	cal_cnt = (cal_cnt == 7) ? cal_cnt : cal_cnt + 1;
	
	if((cal_cnt == 1) || (cal_cnt == 2))			// stylus point 1
		LCD_FilledRectangle(cal_x[TP_X1]-GRID_CENTER, cal_y[TP_Y1]-GRID_CENTER, cal_x[TP_X1]+GRID_CENTER, cal_y[TP_Y1]+GRID_CENTER, 0xf800);
	else if((cal_cnt == 3) || (cal_cnt == 4))		// stylus point 2	
	   	LCD_FilledRectangle(cal_x[TP_CX]-GRID_CENTER, cal_y[TP_CY]-GRID_CENTER, cal_x[TP_CX]+GRID_CENTER, cal_y[TP_CY]+GRID_CENTER, 0xf800);
	else if((cal_cnt == 5) || (cal_cnt == 6))		// stylus point 3   	
   		LCD_FilledRectangle(cal_x[TP_X2]-GRID_CENTER, cal_y[TP_Y2]-GRID_CENTER, cal_x[TP_X2]+GRID_CENTER, cal_y[TP_Y2]+GRID_CENTER, 0xf800);
	
	if(cal_cnt == 7) 
	{
		LCD_FilledRectangle(cal_x[TP_X1], cal_y[TP_Y1], cal_x[TP_X2], cal_y[TP_Y2], 0x1111);	
		fx[0] = TP_X1 - (tx[0] + tx[1])/2;			// calculation 
		fy[0] = TP_Y1 - (ty[0] + ty[1])/2;	
		fx[1] = TP_CX - (tx[2] + tx[3])/2;
		fy[1] = TP_CY - (ty[2] + ty[3])/2;	
		fx[2] = TP_X2 - (tx[4] + tx[5])/2;
		fy[2] = TP_Y2 - (ty[4] + ty[5])/2;

		for(int i=1;i<4;i++)							// calibrate table
		{
			Uart_Printf("calibrated coordinates X%d : %d Y%d : %d\n",i,fx[i-1],i,fy[i-1]);
			for(int j = 0;j<TABLE_X*i/3;j++)
				cal_x[j] += (fx[i-1]*GRID);
			for(int k = 0;k<TABLE_Y*i/3;k++)
				cal_y[k] += (fy[i-1]*GRID);
		}
		Uart_Printf("\nplz correct your calibration value\n");		
		cal_cnt = 8;					
	}
}

unsigned int Bluetooth_Putc(unsigned int c) 
{
 	volatile UART_REGS *pUartRegs;

	pUartRegs = (UART_REGS *)(UART_REG_BASE+UART_REG_OFFSET*1);	
	// UART Channel 1 Address 

	while(!(pUartRegs->rUtrStat & 0x2));  //Wait until THR is empty.
	pUartRegs->rUtxh = c;
	return c;
}

unsigned int Bluetooth_Getc(void)
{
	unsigned int chRx; 
 	volatile UART_REGS *pUartRegs;

	pUartRegs = (UART_REGS *)(UART_REG_BASE+UART_REG_OFFSET*1);	
	// UART Channel 1 Address 
	
	while(!(pUartRegs->rUtrStat & 0x1));  //wait Receive data
	chRx = pUartRegs->rUrxh;

	return chRx; 
}

char Bluetooth_GetKey_t(void) 
{
 	volatile UART_REGS *pUartRegs;

	pUartRegs = (UART_REGS *)(UART_REG_BASE+UART_REG_OFFSET*1);	
	// UART Channel 1 Address 

	if(pUartRegs->rUtrStat & 0x1) return pUartRegs->rUrxh;
	else return 0;
}

void Init_Bluetooth(void)
{
	float DIV_VAL;
	int UDIV_SLOT;
 	volatile UART_REGS *pUartRegs;
	unsigned int temp;
	
	rUCON1 &= ~(0x3);//receive disable before gpio set to prevent dummy data input
	temp = (rGPHCON & ~0xffff ) | 0xaaaa;
	rGPHCON = temp;
	//set uart mode, keep uart mode if it was to prevent gpio set to input, uart
	//it will cause cause rx fifo dummy data input.
				     	
	pUartRegs = (UART_REGS*)(UART_REG_BASE+UART_REG_OFFSET*1); // UART Channel 1 Address 
	//    					  0x5000_0000     0x4000       1
	pUartRegs->rUlCon = 0x3; 
	//Line control register : Normal,No parity,1 stop,8 bits
	DIV_VAL = (int)(PCLK/16/9600)+0.5-1;  
	pUartRegs->rUbrDiv = (int)DIV_VAL;   //Baud rate divisior register 0 
	// DIV_VAL = 35.17
	// 35.17 - 35 = 0.17
	/*
	if(0.125 <= floating point < 0.1875)
	 	UDIVSLOT = 0x0808;
	*/
	pUartRegs->rUdivSlot = 0x5555;	
			
	pUartRegs->rUCon = 0x845;
	// 845 -> 1000 0100 0101
	//  [11:10]     [9]     [8]      [7]        [6]        [5]        [4]         [3:2]          [1:0]
	// Clock Sel, Tx Int, Rx Int, Rx Time Out, Rx err, Loop-back, Send break,  Transmit Mode, Receive Mode
	//     10        0       0        0          1          0          0           01             01
	//   PCLK2      Pulse  Pulse    Disable    Generate  Normal      Normal      Interrupt or Polling
	
	pUartRegs->rUfCon = 0x6; 			 //UART FIFO control register, FIFO disable, tx, rx fifo reset
	pUartRegs->rUmCon = 0x0; 			 //UART MODEM control register, AFC disable
	
	pISR_UART1 	 = (unsigned)Receive_Uart;
	rINTMSK 	&= ~(BIT_UART1);
	rINTSUBMSK 	&= ~(BIT_SUB_RXD1);
}

// Fucntion Body
// ���ĺ����� �����ϴ� DrawImageEx�Լ�. (������ ����� ������)
// * Code by 2010305049 Lee Tae Hoon - ver.2014.06.10 */
void LCD_DrawAlphaImage(int32 bufX, int32 bufY, int32 imgX, int32 imgY, int32 sizeX, int32 sizeY, const int image[], int32 transColor, int32 alphaRate)
{
	int i = 0, j = 0;
	uint32 bBuf = 0;
	uint32 iBuf = 0;
	uint32 cpybBuf = 0; // copy buffer
	uint32 cpyiBuf = 0; // copy buffer
	int bufRval = 0;	int imgRval = 0;
	int bufGval = 0;	int imgGval = 0;
	int bufBval = 0;	int imgBval = 0;
	
	if(sizeX > 480) sizeX = 480; // Size Limit
	if(sizeY > 272) sizeY = 272;
	
	for(i = 0; i < sizeX/2; i++)
		for(j = 0; j < sizeY; j++)
			if((image[(i+imgX/2) + ((imgY+j)*240)] != transColor)) { // 32��Ʈ ��ü�� transColor���̸� �׸��� �۾� ����
				if((i+bufX/2 < 240) && (i+bufX/2 >= 0) && (j+bufY < 272) && (j+bufY >= 0) && // ���÷��� ���� ������ Ȯ��
				   (i+imgX/2 < IMAGE_X/2) && (i+imgX/2 >= 0) && (j+imgY < IMAGE_Y) && (j+imgY >= 0)) {
					// ���� 16��Ʈ�� ������̸� ���� 16��Ʈ�� ���
					if((image[(i+imgX/2) + ((imgY+j)*240)] & 0x0000FFFF) == (transColor & 0x0000FFFF)) {
						if(alphaRate == 0) // ������ 0%
							BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = ((BackBuffer[(i+bufX/2) + ((bufY+j)*240)] & 0x0000FFFF) | (image[(i+imgX/2) + ((imgY+j)*240)] & 0xFFFF0000));
						else { // ������ 0%�� �ƴϸ�
							bBuf = BackBuffer[(i+bufX/2) + ((bufY+j)*240)];
							iBuf = image[(i+imgX/2) + ((imgY+j)*240)];							

							cpybBuf = (bBuf >> 16); // ù��° �ȼ����� �۾�(���� 16bit)
							cpyiBuf = (iBuf >> 16);
							//RED Value1
							bufRval = cpybBuf >> 11;	imgRval = cpyiBuf >> 11;
							bufRval = ((bufRval * (alphaRate) / 100) + (imgRval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0x07FFFFFF) | (bufRval << 27));
							// GREEN Value1
							bufGval = ((cpybBuf & 0xFFFF07EF) >> 5);	imgGval = ((cpyiBuf & 0xFFFF07EF) >> 5);
							bufGval = ((bufGval * (alphaRate) / 100) + (imgGval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xF81FFFFF) | (bufGval << 21));
							// BLUE Value1
							bufBval = (cpybBuf & 0xFFFF001F);	imgBval = (cpyiBuf & 0xFFFF001F);
							bufBval = ((bufBval * (alphaRate) / 100) + (imgBval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFE0FFFF) | (bufBval << 16));
							BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = bBuf;
						}
					}
					// ���� 16��Ʈ�� ������̸� ���� 16��Ʈ�� ���
					else if((image[(i+imgX/2) + ((imgY+j)*240)] & 0xFFFF0000) == (transColor & 0xFFFF0000))
						if(alphaRate == 0) // ������ 0%
							BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = ((BackBuffer[(i+bufX/2) + ((bufY+j)*240)] & 0xFFFF0000) | (image[(i+imgX/2) + ((imgY+j)*240)] & 0x0000FFFF));
						else { // ������ 0%�� �ƴϸ�							
							bBuf = BackBuffer[(i+bufX/2) + ((bufY+j)*240)];
							iBuf = image[(i+imgX/2) + ((imgY+j)*240)];
							cpybBuf = (bBuf & 0x0000FFFF); // �ι�° �ȼ� �۾�(���� 16bit)
							cpyiBuf = (iBuf & 0x0000FFFF);
							// RED Value2
							bufRval = cpybBuf >> 11;	imgRval = cpyiBuf >> 11;
							bufRval = ((bufRval * (alphaRate) / 100) + (imgRval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFFF07FF) | (bufRval << 11));
							// GREEN Value2
							bufGval = ((cpybBuf & 0xFFFF07EF) >> 5);	imgGval = ((cpyiBuf & 0xFFFF07EF) >> 5);
							bufGval = ((bufGval * (alphaRate) / 100) + (imgGval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFFFF81F) | (bufGval << 5));
							// BLUE Value2
							bufBval = (cpybBuf & 0xFFFF001F);	imgBval = (cpyiBuf & 0xFFFF001F);
							bufBval = ((bufBval * (alphaRate) / 100) + (imgBval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFFFFFE0) | bufBval);
							BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = bBuf;
						}
					// �� �ܴ� �� �ȼ� �� ���
					else {
						if(alphaRate == 0) // ������ 0%
							BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = image[(i+imgX/2) + ((imgY+j)*240)];
						else { // ������ 0%�� �ƴϸ�							
							bBuf = BackBuffer[(i+bufX/2) + ((bufY+j)*240)];
							iBuf = image[(i+imgX/2) + ((imgY+j)*240)];							

							cpybBuf = (bBuf >> 16); // ù��° �ȼ����� �۾�(���� 16bit)
							cpyiBuf = (iBuf >> 16);
							//RED Value1
							bufRval = cpybBuf >> 11;	imgRval = cpyiBuf >> 11;
							bufRval = ((bufRval * (alphaRate) / 100) + (imgRval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0x07FFFFFF) | (bufRval << 27));
							// GREEN Value1
							bufGval = ((cpybBuf & 0xFFFF07EF) >> 5);	imgGval = ((cpyiBuf & 0xFFFF07EF) >> 5);
							bufGval = ((bufGval * (alphaRate) / 100) + (imgGval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xF81FFFFF) | (bufGval << 21));
							// BLUE Value1
							bufBval = (cpybBuf & 0xFFFF001F);	imgBval = (cpyiBuf & 0xFFFF001F);
							bufBval = ((bufBval * (alphaRate) / 100) + (imgBval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFE0FFFF) | (bufBval << 16));
							
							cpybBuf = (bBuf & 0x0000FFFF); // �ι�° �ȼ� �۾�(���� 16bit)
							cpyiBuf = (iBuf & 0x0000FFFF);
							// RED Value2
							bufRval = cpybBuf >> 11;	imgRval = cpyiBuf >> 11;
							bufRval = ((bufRval * (alphaRate) / 100) + (imgRval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFFF07FF) | (bufRval << 11));
							// GREEN Value2
							bufGval = ((cpybBuf & 0xFFFF07EF) >> 5);	imgGval = ((cpyiBuf & 0xFFFF07EF) >> 5);
							bufGval = ((bufGval * (alphaRate) / 100) + (imgGval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFFFF81F) | (bufGval << 5));
							// BLUE Value2
							bufBval = (cpybBuf & 0xFFFF001F);	imgBval = (cpyiBuf & 0xFFFF001F);
							bufBval = ((bufBval * (alphaRate) / 100) + (imgBval * (100 - alphaRate) / 100));
							bBuf = ((bBuf & 0xFFFFFFE0) | bufBval);
							
							// Final
							BackBuffer[(i+bufX/2) + ((bufY+j)*240)] = bBuf;
						}
					}
				}
		    }
}

// ��������Ʈ �̹��� �����ϴ� �Լ�
void LCD_Sprite(int x, int y, int spriteID, int milsec, int alphaVal) {
	
	int i = 0;

	for(i = 0; i < IMAGE_MAX_SPRITE; i++) {
		if(spriteGroup[i].currentTime <= 0) {
			spriteGroup[i].spriteID = spriteID;
			spriteGroup[i].bufX = x;
			spriteGroup[i].bufY = y;
			spriteGroup[i].givenTime = milsec;
			spriteGroup[i].currentTime = milsec;
			spriteGroup[i].alpha = alphaVal;

			//Uart_Printf("Sprite[%d] insert at [%d] success...\n", g_spriteGroupCounter, i);
			g_spriteGroupCounter++;
			break;
		}
	}
}

// ��������Ʈ �̹��� ����ϴ� �Լ�
void LCD_DrawSpriteImg() {
	
	// *notice: �Լ� ��� �Ѱ�� ���� ��������Ʈ ���� ���� �̹����� �̹����� ��� �ξ�� �Ѵ�...
	int i = 0;
	int bufX = 0;
	int bufY = 0;
	int imgX = 0;
	int imgY = 0;
	int sizeX = 0;
	int sizeY = 0;
	float interval = 0;
	float spCnt = 0;
	float timeDiscount = 5;
	
	for(i = 0; i < IMAGE_MAX_SPRITE; i++) {
		if(spriteGroup[i].currentTime > 0) {
			bufX = spriteGroup[i].bufX;
			bufY = spriteGroup[i].bufY;
			sizeX = spriteData[spriteGroup[i].spriteID].sizeX;
			sizeY = spriteData[spriteGroup[i].spriteID].sizeY;
			interval = (spriteGroup[i].givenTime) / (spriteData[spriteGroup[i].spriteID].spCnt);
			if(interval <= 0) interval = 0.1;
			if(timeDiscount > interval) timeDiscount = interval; // �̹��� ����� �ǳʶ��� ���ϰ� ��
			spCnt = (spriteData[spriteGroup[i].spriteID].spCnt) - (((spriteGroup[i].currentTime-1) / (interval)));
			if(spCnt >= spriteData[spriteGroup[i].spriteID].spCnt) spCnt = spriteData[spriteGroup[i].spriteID].spCnt - 1;
			
			//imgX = ((int)spCnt * sizeX) + spriteData[spriteGroup[i].spriteID].imgX;
			//imgY = (int)spriteData[spriteGroup[i].spriteID].imgY;
			
			imgX = spriteData[spriteGroup[i].spriteID].imgX;
			imgY = ((int)spCnt * sizeY) + spriteData[spriteGroup[i].spriteID].imgY;
			
			//Uart_Printf("spG[%d]::imgX:%d, imgY:%d, sizeX:%d, sizeY:%d, interval:%.2f, spCnt:%.2f, currentTime:%.2f\n",
			//			 i, imgX, imgY, sizeX, sizeY, interval, spCnt, spriteGroup[i].currentTime);

			if(spriteGroup[i].alpha == 0)
				LCD_DrawImageEx(bufX, bufY, imgX, imgY, sizeX, sizeY, spriteData[spriteGroup[i].spriteID].targetImg, CR_TRANS);
			else
				LCD_DrawAlphaImage(bufX, bufY, imgX, imgY, sizeX, sizeY,  spriteData[spriteGroup[i].spriteID].targetImg, CR_TRANS, spriteGroup[i].alpha); // spriteData[spriteGroup[i].spriteID].alpha

			spriteGroup[i].currentTime -= timeDiscount; // Ŭ���� �����ٸ� �̹��� ��� milsec�κп� ���� ó���� �ȵǹǷ� �� ������ ����
			
			if(spriteGroup[i].currentTime <= 0) { // �̹��� ����� ������
				g_spriteGroupCounter--;			  // ī���� ����
				spriteGroup[i].currentTime -= timeDiscount;  // �ش� ��ġ�� �ð��� -1�� (��� �ϷḦ ����)
				// Uart_Printf("Sprite [%d] Removed...\n", i);
			}
		}
	}
}

/************************************************************************/
/* [ Tetris game on MV2450 board using CodeWarrior with C language ]    */
/*                                                                      */
/*  # Code by "2010305049 Lee Tae Hoon"                                 */
/*																		*/
/*	# Last update: 2014/7/16 : 1.00			                            */
/*                                      								*/
/*									     	     ����  ����������������	*/
/*												 ����������������������	*/
/*												 ������  ����  ��������	*/
/************************************************************************/

// Preprocessor (game)
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_BACKSPACE 8
#define KEY_SPACEBAR 32
#define KEY_SHIFT 16
#define KEY_CTRL 17
#define KEY_ALT 18
#define KEY_Z 90
#define KEY_X 88

#define NUM_FPS 10 		// 1�ʿ� �� 100�� ȭ�� �̹��� ��½õ�
#define NUM_MAINDLY 1	// ���α׷� ���� �����̴� 0.001��

#define SIZE_MAX_X 12			// ���� X������(����)
#define SIZE_MAX_Y 22			// ���� Y������(����)
#define NUM_MAX_BLOCK_TYPE 14	// �� ����
#define BLOCK_DESTROY (NUM_MAX_BLOCK_TYPE+4)	// �ı��Ǵ� ��

#define GS_MAIN 3	// Game State
#define GS_OPTION 2
#define GS_SINGLE 0
#define GS_MULTI 1

#define OP_SOUND 0	// On Option
#define OP_GHOST 1
#define OP_EXIT  2
#define OP_HELP  3
#define OP_HELP1 4
#define OP_HELP2 5

#define OGS_READY 0		// On Game State
#define OGS_SET 1
#define OGS_START 2
#define OGS_PLAYING 3
#define OGS_PAUSE 4
#define OGS_WIN 5
#define OGS_LOSE 6
#define OGS_GAMEOVER 7
#define OGS_WAITING 8	// Waiting other player

#define MAIN_SINGLE 0
#define MAIN_MULTI 1
#define MAIN_OPTION 2

#define NUM_MAX_FEVER 170	// �ƽ� �ǹ� ������

#define MB_LEFT	0	// Move Block
#define MB_RIGHT 1
#define MB_DOWN 2
#define TB_LEFT 0	// Turn Block
#define TB_RIGHT 1

#define NUM_OF_ITEM 12		// Item type
#define ITEM_NULL 0			// Item Slot
#define ITEM_CLOUDE  100
#define ITEM_MISSILE 101
#define ITEM_GHOST 102
#define ITEM_REVERSE 103
#define ITEM_HARDBLOCK 104
#define ITEM_TIMESTOP 105
#define ITEM_TIMEBACK 106
#define ITEM_SHIELD 107
#define ITEM_REMOVELINE 108
#define ITEM_LINEBLOCK 109
#define ITEM_CHANGELINE 110
#define ITEM_IRONBLOCK 111

#define EFFECT_PUSHLINE_1 201 // (�� �޺��� �ı� �� ȿ����. ������ ȿ�� �ƴ�)
#define EFFECT_PUSHLINE_2 202
#define EFFECT_PUSHLINE_3 203
#define EFFECT_PUSHLINE_4 204

// Global Variable (game)
int g_timeElapsed;												// ���α׷� �������κ��� ����� �ð� ����(��)
int g_year, g_month, g_date, g_weekday, g_hour, g_min, g_sec;	// �ð����� ����� ����
bool g_testMode = TRUE;							// *�׽�Ʈ ��� Ȱ��ȭ ����
bool g_useSound = TRUE;							// ���� �¿���
bool g_useMusic = TRUE;							// ���� ���
bool g_useGhostBlock = TRUE;					// ��Ʈ�� ��� ����
int g_blockData[NUM_MAX_BLOCK_TYPE][4][4][4];	// ���� ������ �̰��� ��Ƽ� ���� [�����][ȸ�����][y][x]
int g_gameState = GS_MAIN;						// ���� ���¸� �����ϴ� ����
int g_onGameState = OGS_READY;					// ���� �������� ���¸� �����ϴ� ����
int g_boardData[SIZE_MAX_Y][SIZE_MAX_X];		// ���� ��¿� ����
int g_Xpos = 0, g_Ypos = 0;						// �÷��̾� ����� (1,1)��ǥ
int g_curShape = 0;								// ���� ���� ���
int g_curPhase = 0;								// ���� ���� ������
int g_nextShape = 0;							// ���� ������ ���� ���
int g_holdShape = -1;							// Ȧ��Ǿ� �ִ� ���� ���
bool g_holdAble = TRUE;							// Ȧ�� ���� ���� (TRUE:Ȧ�� ���� / FALSE:Ȧ�� �Ұ���)
int g_score = 0;
int g_level = 1;
int g_fever = 0;
int g_comboCounter = 0;							// �޺� ī����
int g_lineRemoved = 0;							// ���ŵ� ���� ��
int g_itemSlot[2];								// ������ ���� 2��
bool g_buttonRight = FALSE;						// ������ ��ư
bool g_buttonLeft = FALSE;						// ���� ��ư

int g_itemCloudeTime;
bool g_itemCloude = FALSE;						// Ŭ���� ������ ȿ��
int g_itemGhostTime;
bool g_itemGhost = FALSE;						// ��Ʈ ������ ȿ��
int g_itemReverseTime;
bool g_itemReverse = FALSE;						// �¿� ����Ű ���� ȿ��
int g_itemTimestopTime;
bool g_itemTimestop = FALSE;					// �ð� ���� ȿ��
int g_itemTimeBackBoard[SIZE_MAX_Y][SIZE_MAX_X];// �ð��� �ǵ����� �����ۿ� ����� ���� ����
bool g_itemLineBlock = FALSE;					// ���� ���� ������ ������
int g_itemLineBlockTime;
bool g_itemIronBlock = FALSE;					// ���� �ı����� �ʴ� ȿ��
int g_itemIronBlockTime;

// Fuction Prototype (game)
void initalizeTetris();													// ������ �ʱ�ȭ�ϴ� �Լ�
int onMainmenu();														// ���� �޴� ����
int onOption();															// �ɼ� ����
int onGame(int singleOrMulti);											// ������ ����(�Ű������� �̱��÷��� ��Ƽ�÷��� ���� ����)
void onGame_pauseGame(int gameMode, int onGameState);					// ������ �Ͻ����� ��Ű�� �Լ�(�̱��÷��� ����)
void onGame_drawButton();												// ����Ű ��ư�� �׸��� �Լ�
void onGame_drawScore();												// ������ �������� �׷��ִ� �Լ�
void onGame_drawFever();												// �ǹ� �������� �׷��ִ� �Լ�
void onGame_drawBoardBlock();											// �������� �׷��ִ� �Լ�
void onGame_drawCurBlock();												// ���� ���������ִ� ���� �׷��ִ� �Լ�
void onGame_drawSideBlock(int type);									// ���̵� ��(Ȧ��/������)�� �׷��ִ� �Լ�
void onGame_drawBlock(int x, int y, int color, int type, bool isGhost);	// �� �ϳ��� �׷��ִ� �Լ�
void onGame_drawPreviewBlock();											// �̸����� ��(���� ��)�� �׷��ִ� �Լ�
bool onGame_checkBlockCrash(int x, int y, int shape, int phase);		// ���� �浹�ϴ��� Ȯ���ϴ� �Լ�
void onGame_downImmediately();											// ���� ��� �浹��ġ���� �̵���Ű�� �Լ�
bool onGame_moveBlock(int direction);									// ���� ��, ��, �� �̵��� �����ϴ� �Լ�
bool onGame_turnBlock(int direction);									// ���� ��, �� ȸ����Ű�� �Լ�
bool onGame_holdBlock();												// ��Ȧ�带 ó���ϴ� �Լ�
bool onGame_createBlock();												// �� ���� �����ϴ� �Լ�
bool onGame_blockCrashed();												// ���� �浹�Ͽ��� ��� �̸� ó���ϴ� �Լ�
bool onGame_removeLine(bool useRemove);									// ��ü ������ Ȯ���Ͽ� ���� ������ ó���ϴ� �Լ�
bool onGame_checkLine(int lineNumber);									// �ش� ������ ���� ���ִ��� Ȯ���ϴ� �Լ�
void onGame_addScoreAndFeverAndItem(int comboCnt);						// �޺��� �Ű������� �޾Ƽ� ������, �ǹ�, ������ ����/�߰� ��Ű�� �Լ�
void onGame_drawItemSlot();												// ������ ������ �׸��� �Լ�
void onGame_Item_UseItem(int itemID);									// ������ ���� ȣ���ϴ� �Լ�
void onGame_readItem(int itemID);										// ������ ȿ���� �д� �Լ�(����������� ȿ���� �а� �׿� �´� �ൿ�� ����)
void onGame_Item_SetEffectTime(int itemID);								// ������ ���ӽð��� �������ִ� �Լ�
void onGame_Item_CheckLeftTime();										// ������ ���� �ð��� Ȯ���Ͽ� �ð� ���� �������� ���������ִ� �Լ�
void onGame_Item_Effect(int itemID);									// ��� ȿ�� �������� ȿ���� ó���ϴ� �Լ�
void onGame_Item_Diagram(bool isBuf, int itemID);						// ������ ��� �� ��ǳ�� ����Ʈ�� �׷��ִ� �Լ�

// On main()
int main(void)
{
	// test code start setting in option.h
	SetCLKDIV( Startup_ARMCLKdiv, Startup_PREdiv, Startup_HCLKdiv, Startup_PCLKdiv);
	SetMPLL( Startup_MDIV, Startup_PDIV, Startup_SDIV);
	rCLKSRC	=	(rCLKSRC) | (1<<4);
	SystemCLK(0);
	Delay(0); //calibrate Delay()
 	
	//--------------------------------------------------------------------------------------------
	LCDControl_Init();		// LCD Initialize
	LED_Init();				// LED ``
	BTN_Init();				// BTN ``
	BUZ_Init();				// BUZ ``
	Isr_Init();				// Interrupt ``
	Init_Uart();			// UART ``
	RTC_Tick_Time();		// For 1sec Interrupt
	RTC_setTime(14, 4, 23, 4, 0, 0, 0); // �ʱ� �ð�
	Init_AdcTs();			// TCH Init
	Init_Bluetooth();		// BTH Init
	//MakeTable(); // ( If use calibreateion remove this Comment... )
	
	// *Local Variable(Main)
	int inputKey = '\0'; // Ű���� �Է°� ����� ����
	int FPS_Counter = 0;
		
	// **���� ���� ����
	while(1) { 
		// ���� �ʱ�ȭ
		initalizeTetris();
		
		// ���� �޴� ����
		if(g_gameState == GS_MAIN) {
			g_gameState = onMainmenu();
		}
		// �̱��÷��� Ȥ�� ��Ƽ�÷���
		if(g_gameState == GS_SINGLE || g_gameState == GS_MULTI) {
			g_gameState = onGame(g_gameState);
		}
		// �ɼ�
		if(g_gameState == GS_OPTION) {
			g_gameState = onOption();
		}
	}
	// **���� ���� ����
	
	Delay(NUM_MAINDLY);
}

// ���� �޴� ����
int onMainmenu() {
	int gameState = GS_MAIN;
	static int mainSelected = MAIN_SINGLE;
	int inputKey = '/0';
	int mainFPS_Counter = 0;
	int backgroundX = 0;
	
	// **���θ޴� ���� ����
	while(1) {
		// *���� Ű���� �Է� ����
		inputKey = '\0';
	    inputKey = Uart_GetKey_t();
	    if(inputKey != 0) { // Ű���� �Է��� ���� ��
			if(inputKey == '5' || inputKey == '1') { // �Է��� Ű�� 5Ȥ�� 1
				(mainSelected == MAIN_SINGLE) ? (mainSelected = MAIN_OPTION) : (mainSelected--);
				if(g_useSound) { BUZ_play(C1, DLY_64);	BUZ_play(D1, DLY_64);	BUZ_stop(); }
				if(g_testMode) { Uart_Printf("UP_KEY Pressed.\n"); }
			}
			else if(inputKey == '2' || inputKey == '3') { // �Է��� Ű�� 2Ȥ�� 3
				(mainSelected == MAIN_OPTION) ? (mainSelected = MAIN_SINGLE) : (mainSelected++);
				if(g_useSound) { BUZ_play(C1, DLY_64);	BUZ_play(D1, DLY_64);	BUZ_stop(); }
				if(g_testMode) { Uart_Printf("DOWN_KEY Pressed.\n"); }
			}
			else if(inputKey == KEY_ENTER) { // �Է��� Ű�� ����
				gameState = mainSelected;
				if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(D1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
				if(g_testMode) { Uart_Printf("ENTER_KEY Pressed. (gameState:%d)\n", gameState); }
				return gameState;
			}
		}
		else { } // Ű���� �Է��� ���� ��
		// *���� Ű���� �Է� ����
		
		// *���� ��ư �Է� ����
		// BTN4 (�޴� ����)
		if(BTN_isPushed(BTN4)) { // ������ ��
			if(g_btnFlag[3] == FALSE) {
				g_btnFlag[3] = TRUE;
				gameState = mainSelected;
				if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(D1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
				if(g_testMode) { Uart_Printf("ENTER_KEY Pressed. (gameState:%d)\n", gameState); }
				return gameState;
			}
		}
		else { 
			if(g_btnFlag[3] == TRUE) { // �� ��
				g_btnFlag[3] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN4' Released(g_yPos:%d, g_curShape:%d, g_holdShape:%d).\n", g_Ypos, g_curShape, g_holdShape); }
			}
		}
		// BTN5 (�޴� ���� �̵�)
		if(BTN_isPushed(BTN5)) { // ������ ��
			if(g_btnFlag[4] == FALSE) {
				g_btnFlag[4] = TRUE;
				(mainSelected == MAIN_SINGLE) ? (mainSelected = MAIN_OPTION) : (mainSelected--);
				if(g_useSound) { BUZ_play(C1, DLY_64);	BUZ_play(D1, DLY_64);	BUZ_stop(); }
				if(g_testMode) { Uart_Printf("UP_KEY Pressed.\n"); }
			}
		}
		else { 
			if(g_btnFlag[4] == TRUE) { // �� ��
				g_btnFlag[4] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN5' Released.\n"); }
			}
		}
		// BTN6 (�޴� �Ʒ��� �̵�)
		if(BTN_isPushed(BTN6)) { // ������ ��
			if(g_btnFlag[5] == FALSE) {
				g_btnFlag[5] = TRUE;
				(mainSelected == MAIN_OPTION) ? (mainSelected = MAIN_SINGLE) : (mainSelected++);
				if(g_useSound) { BUZ_play(C1, DLY_64);	BUZ_play(D1, DLY_64);	BUZ_stop(); }
				if(g_testMode) { Uart_Printf("DOWN_KEY(BTN6) Pressed.\n"); }
			}
		}
		else { 
			if(g_btnFlag[5] == TRUE) { // �� ��
				g_btnFlag[5] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN6' Released.\n"); }
			}
		}
		// *���� ��ư �Է� ����

		// *���θ޴� ȭ�� �׸��� ����
		if(mainFPS_Counter++ == NUM_FPS) {
			mainFPS_Counter = 0;
			
			// ��� ȭ��
			LCD_DrawImageEx(backgroundX, 0, 0, 0, 480-backgroundX, 272, OptionImage, CR_TRANS);
			LCD_DrawImageEx(0, 0, 480-backgroundX, 0, backgroundX, 272, OptionImage, CR_TRANS);
			((backgroundX < 480) ? (backgroundX += 2) : (backgroundX = 0));
			LCD_DrawImageEx(0, 0, 0, 0, 480, 272, MainMenu, CR_TRANS);
			
			if(mainSelected == MAIN_SINGLE)	{ // �̱��÷��� ���ý�
				LCD_DrawRect(199, 146, 199+92, 146+35, CR_RED); // (92x35)�簢��
				LCD_DrawRect(198, 145, 198+94, 145+37, CR_RED); // (94x37)�簢��
			}
			else if(mainSelected == MAIN_MULTI) { // ��Ƽ�÷��� ���ý�
				LCD_DrawRect(199, 186, 199+92, 186+35, CR_RED); // (92x35)�簢��
				LCD_DrawRect(198, 185, 198+94, 185+37, CR_RED); // (94x37)�簢��
			}
			else if(mainSelected == MAIN_OPTION) { // �ɼ� ���ý�
				LCD_DrawRect(199, 224, 199+92, 224+35, CR_RED); // (92x35)�簢��
				LCD_DrawRect(198, 223, 198+94, 223+37, CR_RED); // (94x37)�簢��
			}

			LCD_DrawSpriteImg();
			LCD_Invalidate(FALSE);
		}
		// *���θ޴� ȭ�� �׸��� ����
		
		Delay(NUM_MAINDLY);
	}
	// **���θ޴� ���� ����

	return -1;
}

// �ɼ� ����
int onOption() {
	int gameState = GS_OPTION;
	int optionSelected = OP_SOUND;
	int inputKey = '/0';
	int optionFPS_Counter = 0;
	int backgroundX = 0;
	
	// **�ɼ� ���� ����
	while(1) {
		// *�ɼ� Ű���� �Է� ����
		inputKey = '\0';
		inputKey = Uart_GetKey_t();
		if(inputKey != 0) { // Ű���� �Է��� ���� ��
			if(inputKey == '2' || inputKey == '3') { // '2 or 3'�� ������ ��
				(optionSelected < OP_HELP) ? (optionSelected++) : (optionSelected = OP_SOUND);
				if(g_useSound) { BUZ_play(C1, DLY_64);	BUZ_play(D1, DLY_64);	BUZ_stop(); }
			}
			else if(inputKey == '5' || inputKey == '1') { // '1 or 5'�� ������ ��
				(optionSelected > OP_SOUND) ? (optionSelected--) : (optionSelected = OP_HELP);
				if(g_useSound) { BUZ_play(C1, DLY_64);	BUZ_play(D1, DLY_64);	BUZ_stop(); }
			}
			else if(inputKey == KEY_ENTER) { // '���� Ű'�� ������ ��
				if(optionSelected == OP_SOUND) {
					if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(D1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
					(g_useSound) = !(g_useSound);
				}
				else if(optionSelected == OP_GHOST) {
					(g_useGhostBlock) = !(g_useGhostBlock);
					if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(D1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
				}
				else if(optionSelected == OP_EXIT) {
					if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(D1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
					return GS_MAIN;
				}
				else if(optionSelected == OP_HELP) {
					optionSelected = OP_HELP1;
					if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(D1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
				}
				else if(optionSelected == OP_HELP1) {
					optionSelected = OP_HELP2;
					if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
				}
				else if(optionSelected == OP_HELP2) {
					optionSelected = OP_HELP;
					if(g_useSound) { BUZ_play(C1, DLY_32);	BUZ_play(E1, DLY_32);	BUZ_stop(); }
				}
			}
		}		
		// *�ɼ� Ű���� �Է� ����
		
		// *�ɼ� ��ư �Է� ����
		
		// *�ɼ� ��ư �Է� ����
	
		// *�ɼ� ȭ�� �׸��� ����
		if(optionFPS_Counter++ == NUM_FPS) {
			optionFPS_Counter = 0;
			// ��� ȭ��
			LCD_DrawImageEx(backgroundX, 0, 0, 0, 480-backgroundX, 272, OptionImage, CR_TRANS);
			LCD_DrawImageEx(0, 0, 480-backgroundX, 0, backgroundX, 272, OptionImage, CR_TRANS);
			((backgroundX < 480) ? (backgroundX += 2) : (backgroundX = 0));
			//LCD_DrawImage(0, 0, 480, 272, OptionImage);
			// ���� �ɼ�
			LCD_DrawImageEx(34, 36, 328, 173, 62, 40, MiscImage, CR_TRANS);  // ����
			LCD_DrawImageEx(120, 36, 392, 134, 46, 39, MiscImage, CR_TRANS); // ���
			LCD_DrawImageEx(188, 36, 392, 175, 78, 37, MiscImage, CR_TRANS); // �̻��
			if(optionSelected == OP_SOUND) { LCD_DrawRect(32, 34, 32+66, 34+44, CR_BLUE); LCD_DrawRect(31, 33, 31+68, 33+46, CR_BLUE); }
			if(g_useSound == TRUE) { LCD_DrawRect(118, 34, 118+48, 34+44, CR_RED); LCD_DrawRect(117, 33, 117+50, 33+46, CR_RED); }
			else { LCD_DrawRect(186, 34, 186+84, 34+44, CR_RED); LCD_DrawRect(185, 33, 185+86, 33+46, CR_RED); }
			// ��Ʈ �ɼ�
			LCD_DrawImageEx(34, 90, 205, 173, 122, 40, MiscImage, CR_TRANS); // ���� ��
			LCD_DrawImageEx(176, 90, 392, 134, 46, 39, MiscImage, CR_TRANS); // ���
			LCD_DrawImageEx(236, 90, 392, 175, 78, 37, MiscImage, CR_TRANS); // �̻��
			if(optionSelected == OP_GHOST) { LCD_DrawRect(32, 88, 32+126, 88+44, CR_BLUE); LCD_DrawRect(31, 87, 31+128, 87+46, CR_BLUE); }
			if(g_useGhostBlock == TRUE) { LCD_DrawRect(174, 88, 174+50, 88+44, CR_RED); LCD_DrawRect(173, 87, 173+52, 87+46, CR_RED); }
			else { LCD_DrawRect(234, 88, 234+84, 88+44, CR_RED); LCD_DrawRect(233, 87, 233+86, 87+46, CR_RED); }
			// �ɼ� ������
			LCD_DrawImageEx(36, 152, 266, 128, 124, 44, MiscImage, CR_TRANS); // �ɼ� ������
			if(optionSelected == OP_EXIT) { LCD_DrawRect(34, 150, 34+128, 150+48, CR_BLUE);  LCD_DrawRect(33, 149, 33+130, 149+50, CR_BLUE); }
			// ������
			LCD_DrawImageEx(156, 224, 154, 214, 320, 46, MiscImage, CR_TRANS);
			// ����
			LCD_DrawImageEx(392, 152, 264, 42, 76, 48, MiscImage, CR_TRANS); // ���� ����
			if(optionSelected == OP_HELP) { LCD_DrawRect(390, 150, 390+76, 150+48, CR_BLUE);  LCD_DrawRect(389, 149, 389+76, 149+48, CR_BLUE); }
			// ���� 1/2
			if(optionSelected == OP_HELP1) LCD_DrawImageEx(0, 0, 0, 0, 480, 272, HelpScroll1, CR_TRANS);
			else if(optionSelected == OP_HELP2) LCD_DrawImageEx(0, 0, 0, 0, 480, 272, HelpScroll2, CR_TRANS);
			
			LCD_DrawSpriteImg();
			LCD_Invalidate(FALSE);
		}
		// *�ɼ� ȭ�� �׸��� ����
		
		Delay(NUM_MAINDLY);
	}
	// **�ɼ� ���� ����
}

// ������ ����
int onGame(int singleOrMulti)
{
	int i = 0, j = 0;
	int gameMode = singleOrMulti;			// �̱��÷��� Ȥ�� ��Ƽ�÷��̸� �Ű������� �޾ƿ���
	int onGameState = GS_SINGLE;
	int timerBeforePlaying = g_timeElapsed;	// ���� ���� �� OGS_READY~OGS_START �κ��� ó���ϱ� ����
	int inputKey = '/0';
	int gameFPS_Counter = 0;
	int blockDownDelay = 300;				// �ʱ� ���� �������� �ӵ�
	int blockDownDelayCnt = blockDownDelay;	// �� �ٿ� ������ ī����
	int timeElapsed = g_timeElapsed;		// 1�� Ÿ�̸ӿ� �̿��ϱ� ���� ����
	int blockBreakCnt = 0;					// �� �ı���� ���� ī����
	bool breakAble = FALSE;					// �� �ı���� ���� ����
	bool gameoverSound = TRUE;				// ���� ���� ���带 �ѹ� ��½����ֱ� ����
	
	int itemCloudeX = 40;				// Ŭ���� ������ ����
	bool itemCloudeXUpCnt = TRUE;		// Ŭ���� ������ ����(TRUE�� X��ǥ ����, FALSE�� ����)
	int itemCloudeSpriteCnt = 0;		// Ŭ���� ������ ����(Ŭ���� ��������Ʈ 2��)
	
	int musicCnt = 0; // ���� ī���� ����
	
	((gameMode == GS_SINGLE) ? (g_onGameState = OGS_READY) : (g_onGameState = OGS_WAITING)); // ó�� ���۽� �غ����(OGS_READY) ��Ƽ ����� ������(OGS_WAITING)
	if(gameMode == GS_MULTI) { Bluetooth_Putc(OGS_WAITING); } // ���� ���� ���� ���� ��뿡�� ������̶�� ��ȣ�� �ѹ� ����.
															  // ���� ��밡 OGS_WAITING���¶�� OGS_WAITING��ȣ�� �����Ե� �����ϰ�
															  // �ڽ��� ���¸� OGS_READY�� �����Ѵ�.
	// **���� ���� ����
	while(1) {
		timeElapsed = g_timeElapsed; // �׻� ��� �ð��� �����Ͽ� ����
		onGameState = g_onGameState; // �׻� ���Ӿ��� ���¸� �����Ͽ� ����	
		
		// *���� ���� ����
		if(onGameState == OGS_PLAYING) { // ���� ���°� OGS_PLAYING�� ��
			// �� �ٿ� ����
			if(g_itemTimestop == FALSE) { // ������ �ð������� FALSE ������ �� �� ������
				if(blockDownDelayCnt-- <= 0) {
					blockDownDelay = 300 - (g_level * 3);	// ���� �������� �ӵ��� ������ �����ϸ� ������
					blockDownDelayCnt = blockDownDelay;
					onGame_moveBlock(MB_DOWN);				// ���� �Ʒ��� ������ �浹 ������ �׿� �ش��ϴ� ��ƾ�� ����
				}
			}
			
			// ���� ����
			if(g_lineRemoved != 0) {	// ���ŵ� �� ���� 0�� �ƴ� ��
				g_level = (g_lineRemoved / 4) + 1;	// (������ ���ŵ� �� �� / 4) + 1
			}
			
			// �� �ı� ����
			if(blockBreakCnt++ >= ((g_useMusic==TRUE)?(4):(10))) {
					blockBreakCnt = 0;
					breakAble = FALSE;
					for(i = 1; i < SIZE_MAX_X-1; i++) {
						if(breakAble == TRUE) break;
						for(j = 1; j < SIZE_MAX_Y-1; j++) {
							if(g_boardData[j][i] >= BLOCK_DESTROY) if(g_itemIronBlock == FALSE) { g_boardData[j][i]++; } // ��ö �� �������� ���� ���� ��
							if(g_boardData[j][i] >= BLOCK_DESTROY + 9) { onGame_removeLine(TRUE); breakAble = TRUE; break; } // �ϳ��� �ı����� ���̸� ���� �ΰ� Ż��
						}
					}
			}
			// *���� ��� ����
			if(g_useMusic == TRUE) {
				if(musicCnt++ == 15) {			
					musicCnt = 0;
					BUZ_play(g_music[g_musicCurrent].scale, g_music[g_musicCurrent].delay);
					BUZ_stop();
					(g_musicCurrent < MUSIC_MAX_NOTE) ? (g_musicCurrent++) : (g_musicCurrent = 0);
				}
			}
			// *���� ��� ����
			
			// ������ ���ӽð� ���� (���ӽð��� �� �� ȿ���� �������� �ֱ� ����)
			onGame_Item_CheckLeftTime();
			
			// *���� Ű���� �Է� ����
			inputKey = '\0';
		    inputKey = Uart_GetKey_t();
		    if(inputKey != 0) { // Ű���� �Է��� ���� ��
				/////////////////// TEST CODE /////////////////// 
				if(inputKey == 'r') { // r�� ������ �� (�׽�Ʈ)
					g_Ypos = 2;
					Uart_Printf("g_Xpos:%d, g_Ypos:%d, g_curShape:%d, g_curPhase:%d\n", g_Xpos, g_Ypos, g_curShape, g_curPhase);
				}
				if(inputKey == '+') { // +�� �������� (�׽�Ʈ)
					g_itemSlot[0] = ((rand() + g_timeElapsed) % 12) + 100;
					g_itemSlot[1] = ((rand() + g_timeElapsed) % 12) + 100;
				}
				/////////////////// TEST CODE /////////////////// 
				if(inputKey == '1') { // �Է��� Ű�� 1�� �� (�� �·� �̵�)
					onGame_moveBlock(MB_LEFT);
					if(g_testMode) { Uart_Printf("'1' Pressed(g_Xpos:%d, g_Ypos:%d, g_curShape:%d, g_curPhase:%d).\n",
												 g_Xpos, g_Ypos, g_curShape, g_curPhase); }
				}
				else if(inputKey == '3') { // �Է��� Ű�� 3�� �� (�� ��� �̵�)
					onGame_moveBlock(MB_RIGHT);
					if(g_testMode) { Uart_Printf("'3' Pressed(g_Xpos:%d, g_Ypos:%d, g_curShape:%d, g_curPhase:%d).\n",
												 g_Xpos, g_Ypos, g_curShape, g_curPhase); }
				}
				else if(inputKey == '2') { // �Է��� Ű�� 2�� �� (�� �Ʒ��� �̵�)
					onGame_moveBlock(MB_DOWN);
					if(g_testMode) { Uart_Printf("'2' Pressed(g_Xpos:%d, g_Ypos:%d, g_curShape:%d, g_curPhase:%d).\n",
												 g_Xpos, g_Ypos, g_curShape, g_curPhase); }
				}
				else if(inputKey == 'z' || inputKey == 'Z')	{ // �Է��� Ű�� 'Z or z'�� �� (�� �·� ȸ��)
					onGame_turnBlock(TB_LEFT);
					if(g_testMode) { Uart_Printf("'z' Pressed(g_curShape:%d, g_curPhase:%d).\n", g_curShape, g_curPhase); }
				}
				else if(inputKey == 'x' || inputKey == 'X' || inputKey == '5')	{ // �Է��� Ű�� 'x or X or 5'�� �� (�� ��� ȸ��)
					onGame_turnBlock(TB_RIGHT);
					if(g_testMode) { Uart_Printf("'x' Pressed(g_curShape:%d, g_curPhase:%d).\n", g_curShape, g_curPhase); }
				}
				else if(inputKey == 'd' || inputKey == 'D')	{ // �Է��� Ű�� 'd or D'�� �� (�� Ȧ��)
					onGame_holdBlock();
					if(g_testMode) { Uart_Printf("'a' Pressed(g_curShape:%d, g_holdShape:%d).\n", g_curShape, g_holdShape); }
				}
				else if(inputKey == ' ') { // �Է��� Ű�� ' '�� �� (�� ��� ������)
					onGame_downImmediately();
					if(g_testMode) { Uart_Printf("' ' Pressed(g_yPos:%d, g_curShape:%d, g_holdShape:%d).\n", g_Ypos, g_curShape, g_holdShape); }
				}
				else if(inputKey == 'a' || inputKey == 'A') { // �Է��� Ű�� 'A or a'�� �� (������1 ���)
					if(g_testMode) { Uart_Printf("'q' Pressed(g_itemSlot[0]:%d\n", g_itemSlot[0]); }
					onGame_Item_UseItem(g_itemSlot[0]);
					// onGame_readItem(g_itemSlot[0]); // ONLY FOR TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
					g_itemSlot[0] = ITEM_NULL;
				}
				else if(inputKey == 's' || inputKey == 'S') { // �Է��� Ű�� 'S or s'�� �� (������2 ���)
					if(g_testMode) { Uart_Printf("'q' Pressed(g_itemSlot[1]:%d\n", g_itemSlot[1]); }
					onGame_Item_UseItem(g_itemSlot[1]);
					// onGame_readItem(g_itemSlot[1]); // ONLY FOR TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
					g_itemSlot[1] = ITEM_NULL;
				}
				else if(inputKey == 'p' || inputKey == 'P') { // �Է��� Ű�� 'P or p'�� �� (�Ͻ� ����)
					onGame_pauseGame(gameMode, onGameState);
					if(g_testMode) { Uart_Printf("'p' Pressed(gameMode:%d, onGameState:%d\n", gameMode, onGameState); }
				}
				else if(inputKey == 'm' || inputKey == 'M') { // �Է��� Ű�� 'M or m'�� �� (���� ��� ����)
					g_useMusic = !(g_useMusic);
					if(g_testMode) { Uart_Printf("'m' Pressed(g_useMusic:%d)\n", g_useMusic); }
				}
			}
			else { } // Ű���� �Է��� ���� ��
			// *���� Ű���� �Է� ����
		}
		else if(onGameState == OGS_READY || onGameState == OGS_SET || onGameState == OGS_START) { // ����, ��, ��ŸƮ ��
			if(onGameState == OGS_READY) {
				if(timerBeforePlaying + 1 <= timeElapsed) {
					timerBeforePlaying = timeElapsed;
					if(g_useSound) { BUZ_play(E1, DLY_16); BUZ_stop(); }
					if(g_testMode) { Uart_Printf("onGameState:OGS_READY\n"); }
					g_onGameState = OGS_SET;
				}
			}
			else if(onGameState == OGS_SET) {
				if(timerBeforePlaying + 1 <= timeElapsed) {
					timerBeforePlaying = timeElapsed;
					if(g_useSound) { BUZ_play(F1, DLY_16); BUZ_stop(); }
					if(g_testMode) { Uart_Printf("onGameState:OGS_SET\n"); }
					g_onGameState = OGS_START;
				}
			}
			else if(onGameState == OGS_START) {
				if(timerBeforePlaying + 1 <= timeElapsed) {
					timerBeforePlaying = timeElapsed;
					if(g_useSound) { BUZ_play(G1, DLY_16); BUZ_stop(); }
					if(g_testMode) { Uart_Printf("onGameState:OGS_START\n"); }
					g_onGameState = OGS_PLAYING;
				}
			}	
		}
		else if(onGameState == OGS_GAMEOVER) {	// ���� ���� ��
			for(i = 1; i < SIZE_MAX_Y-1; i++) {
				for(j = 1; j < SIZE_MAX_X-1; j++) {	
					if(g_boardData[i][j] != 0) g_boardData[i][j] = 16;
				}
			}
			if(g_useSound && gameoverSound) { BUZ_play(C2, DLY_8); BUZ_play(B1, DLY_4); BUZ_play(A1, DLY_8); BUZ_play(G1, DLY_4); BUZ_play(F1, DLY_8);
   	 						 				  BUZ_play(E1, DLY_4); BUZ_play(D1, DLY_8); BUZ_play(C1, DLY_4); BUZ_play(B1, DLY_8); BUZ_play(C2, DLY_4); BUZ_stop(); }
   	 		gameoverSound = FALSE;	
   	 		// ���� �޴��� ���ư��� ���� Ű �Է� �޴� ��ƾ
   	 		inputKey = '\0';
		    inputKey = Uart_GetKey_t();
		    if(inputKey != 0) { // Ű���� �Է��� ���� ��
				if(inputKey == KEY_ENTER) // '���� Ű'�� ������ ��
					return GS_MAIN;
			}
		}
		else if(onGameState == OGS_WIN) { // �¸� ��
			if(g_useSound && gameoverSound) { BUZ_play(C1, DLY_4); BUZ_play(D1, DLY_8); BUZ_play(E1, DLY_4); BUZ_play(F1, DLY_8); BUZ_play(G1, DLY_4);
											  BUZ_play(A1, DLY_8); BUZ_play(B1, DLY_4); BUZ_play(C2, DLY_8); BUZ_play(D1, DLY_8); BUZ_play(C1, DLY_4); BUZ_stop(); }
			gameoverSound = FALSE;
   	 		// ���� �޴��� ���ư��� ���� Ű �Է� �޴� ��ƾ
   	 		inputKey = '\0';
		    inputKey = Uart_GetKey_t();
		    if(inputKey != 0) { // Ű���� �Է��� ���� ��
				if(inputKey == KEY_ENTER) // '���� Ű'�� ������ ��
					return GS_MAIN;
			}
		}
		else if(onGameState == OGS_PAUSE) { // �Ͻ������� (��ư�� Ű���� �Է� �߰��� ���)
			// �Ͻ����� �� Ű���� �Է�
			inputKey = '\0';
		    inputKey = Uart_GetKey_t();
		    if(inputKey != 0) { // Ű���� �Է��� ���� ��
				if(inputKey == 'p' || inputKey == 'P') { // �Է��� Ű�� 'P or p'�� �� (�Ͻ� ����)
					onGame_pauseGame(gameMode, onGameState);
					if(g_testMode) { Uart_Printf("'p' Pressed(gameMode:%d, onGameState:%d\n", gameMode, onGameState); }
				}
			}
			// �Ͻ����� �� ��ư �Է�
			if(BTN_isPushed(BTN1)) { // ������ ��
				if(g_btnFlag[0] == FALSE) {
					g_btnFlag[0] = TRUE;
					onGame_pauseGame(gameMode, onGameState);
					if(g_testMode) { Uart_Printf("'BTN1' Pressed(gameMode:%d, onGameState:%d\n", gameMode, onGameState); }
				}
			}
			else { 
				if(g_btnFlag[0] == TRUE) { // �� ��
					g_btnFlag[0] = FALSE;
					if(g_testMode) { Uart_Printf("'BTN1' Released(gameMode:%d, onGameState:%d\n", gameMode, onGameState); }
				}
			}
		}
		else if(onGameState == OGS_WAITING) { // ��Ƽ�÷��� �����
		   	// ���� �޴��� ���ư��� ���� Ű �Է� �޴� ��ƾ
   	 		inputKey = '\0';
		    inputKey = Uart_GetKey_t();
		    if(inputKey != 0) { // Ű���� �Է��� ���� ��
				if(inputKey == KEY_ENTER) // '���� Ű'�� ������ ��
					return GS_MAIN;
			}
		}
		// *���� ���� ��
		
		// *���� ��ư �Է� ����
		// BTN1 (�Ͻ� ����)
		if(BTN_isPushed(BTN1)) { // ������ ��
			if(g_btnFlag[0] == FALSE) {
				g_btnFlag[0] = TRUE;
				onGame_pauseGame(gameMode, onGameState);
				if(g_testMode) { Uart_Printf("'BTN1' Pressed(gameMode:%d, onGameState:%d\n", gameMode, onGameState); }
				Delay(1);
				g_btnFlag[0] = FALSE; // (��ư�� ������ �Ͻ������� ���� �κ��� ��ƾ�� �ٲ�� ������� ���ϹǷ� ���⼭ �÷��� ����)
			}
		}
		else { 
			if(g_btnFlag[0] == TRUE) { // �� ��
				g_btnFlag[0] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN1' Released(gameMode:%d, onGameState:%d\n", gameMode, onGameState); }
			}
		}
		// BTN3 (Ȧ�� ��)
		if(BTN_isPushed(BTN3)) { // ������ ��
			if(g_btnFlag[2] == FALSE) {
				g_btnFlag[2] = TRUE;
				onGame_holdBlock();
				if(g_testMode) { Uart_Printf("'BTN3' Pressed(g_curShape:%d, g_holdShape:%d).\n", g_curShape, g_holdShape); }
			}
		}
		else { 
			if(g_btnFlag[2] == TRUE) { // �� ��
				g_btnFlag[2] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN3' Released(g_curShape:%d, g_holdShape:%d).\n", g_curShape, g_holdShape); }
			}
		}
		// BTN4 (�� �ٷ� ������)
		if(BTN_isPushed(BTN4)) { // ������ ��
			if(g_btnFlag[3] == FALSE) {
				g_btnFlag[3] = TRUE;
				onGame_downImmediately();
				if(g_testMode) { Uart_Printf("'BTN4' Pressed(g_yPos:%d, g_curShape:%d, g_holdShape:%d).\n", g_Ypos, g_curShape, g_holdShape); }
			}
		}
		else { 
			if(g_btnFlag[3] == TRUE) { // �� ��
				g_btnFlag[3] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN4' Released(g_yPos:%d, g_curShape:%d, g_holdShape:%d).\n", g_Ypos, g_curShape, g_holdShape); }
			}
		}
		// BTN5 (�� ��� ȸ��)
		if(BTN_isPushed(BTN5)) { // ������ ��
			if(g_btnFlag[4] == FALSE) {
				g_btnFlag[4] = TRUE;
				onGame_turnBlock(TB_RIGHT);
				if(g_testMode) { Uart_Printf("'BTN5' Pressed(g_curShape:%d, g_curPhase:%d).\n", g_curShape, g_curPhase); }
			}
		}
		else { 
			if(g_btnFlag[4] == TRUE) { // �� ��
				g_btnFlag[4] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN5' Released(g_curShape:%d, g_curPhase:%d).\n", g_curShape, g_curPhase); }
			}
		}
		// BTN6 (�� �Ʒ��� �̵�)
		if(BTN_isPushed(BTN6)) { // ������ ��
			if(g_btnFlag[5] == FALSE) {
				g_btnFlag[5] = TRUE;
				onGame_moveBlock(MB_DOWN);
				if(g_testMode) { Uart_Printf("'BTN6' Pressed(g_curShape:%d, g_curPhase:%d).\n", g_curShape, g_curPhase); }
			}
		}
		else { 
			if(g_btnFlag[5] == TRUE) { // �� ��
				g_btnFlag[5] = FALSE;
				if(g_testMode) { Uart_Printf("'BTN6' Released(g_curShape:%d, g_curPhase:%d).\n", g_curShape, g_curPhase); }
			}
		}
		// *���� ��ư �Է� ��
	
		// *���� ȭ�� �׸��� ����
		if(gameFPS_Counter++ == NUM_FPS) {
			gameFPS_Counter = 0;
			// ��Ʈ���� ������ �׷���
			LCD_DrawImage(0, 0, 480, 272, GameBoard);
			// ��Ʈ���� ��ü �� �׷���
			onGame_drawBoardBlock();
			// ��Ʈ���� �̸����� ���ɺ� �׷���
			if(g_useGhostBlock) onGame_drawPreviewBlock();
			// ��Ʈ���� ���� �� �׷���
			onGame_drawCurBlock();
			// ��Ʈ���� ���� �� �׷���
			onGame_drawSideBlock(0);
			// ��Ʈ���� Ȧ�� �� �׷���
			onGame_drawSideBlock(1);
			// ��Ʈ���� ������ ���� �׷���
			onGame_drawScore();
			// ��Ʈ���� �ǹ� ������ �׷���
			onGame_drawFever();
			// ��Ʈ���� ��/�� ����Ű �׷���
			onGame_drawButton();
			// ��Ʈ���� ������ �׷���
			onGame_drawItemSlot();
			// ��Ʈ���� ��������Ʈ ����Ʈ �׷���
			LCD_DrawSpriteImg();
			// ��Ʈ���� ������ ȿ�� �׷���
			if(g_itemCloude == TRUE) {
				LCD_DrawImageEx(itemCloudeX, 78, 200, 0+(188*itemCloudeSpriteCnt), 164, 188, EffectImage, CR_TRANS);
				(itemCloudeSpriteCnt < 1) ? (itemCloudeSpriteCnt++) : (itemCloudeSpriteCnt = 0);
				if(itemCloudeXUpCnt) { // ��ǥ ���� Ÿ�̹�
					if(itemCloudeX < 280) itemCloudeX += 12;
					else itemCloudeXUpCnt = FALSE;
				}
				else { // ��ǥ ���� Ÿ�̹�
					if(itemCloudeX > 92) itemCloudeX -= 12;
					else itemCloudeXUpCnt = TRUE;
				}
			}
			// ��Ʈ���� ���� ���� �׷���(onGameState: READY, SET, START, PAUSE)
			if(onGameState == OGS_READY) LCD_DrawAlphaImage(180, 12, 0, 240, 149, 239, OnGameState, CR_TRANS, 64);
			else if(onGameState == OGS_SET) LCD_DrawAlphaImage(180, 12, 0, 0, 149, 239, OnGameState, CR_TRANS, 64);
			else if(onGameState == OGS_START) LCD_DrawAlphaImage(180, 12, 150, 240, 149, 239, OnGameState, CR_TRANS, 64);
			else if(onGameState == OGS_WAITING) {
				LCD_DrawAlphaImage(0, 0, 0, 0, 480, 272, BlackScreen, CR_TRANS, 32);
				LCD_DrawImageEx(110, 22, 300, 241, 84, 206, OnGameState, CR_TRANS);
			}
			if (onGameState == OGS_PAUSE) {
				LCD_DrawAlphaImage(0, 0, 0, 0, 480, 272, BlackScreen, CR_TRANS, 32);
				LCD_DrawImageEx(140, 22, 150, 0, 94, 228, OnGameState, CR_TRANS);
			}
			if (onGameState == OGS_GAMEOVER) { 
				LCD_DrawAlphaImage(0, 0, 0, 0, 480, 272, BlackScreen, CR_TRANS, 32);
				LCD_DrawImageEx(110, 32, 328, 0, 82, 200, OnGameState, CR_TRANS);
			}
			if (onGameState == OGS_WIN) {
				LCD_DrawAlphaImage(0, 0, 0, 0, 480, 272, BlackScreen, CR_TRANS, 32);
				LCD_DrawImageEx(110, 32, 245, 0, 82, 200, OnGameState, CR_TRANS);
			}
			else if(onGameState == OGS_LOSE) { }
			// ȭ�� ����
			LCD_Invalidate(FALSE);
		}
		// *���� ȭ�� �׸��� ����
		
		Delay(NUM_MAINDLY);
	}
	
	return -1;
}

// ������ �Ͻ����� ��Ű�� �Լ�
void onGame_pauseGame(int gameMode, int onGameState) {
	// ���� �Ͻ�����
	if(gameMode == GS_SINGLE && onGameState == OGS_PLAYING) { // ������ �̱��÷��� ���� �� �϶�
		g_onGameState = OGS_PAUSE;
		if(g_useSound) { BUZ_play(E1, DLY_64); BUZ_play(F1, DLY_64); BUZ_play(E1, DLY_64); BUZ_play(F1, DLY_64); BUZ_stop(); }
	}
	// �Ͻ����� ����
	else if(onGameState == OGS_PAUSE) {
		g_onGameState = OGS_PLAYING;
		if(g_useSound) { BUZ_play(F1, DLY_64); BUZ_play(E1, DLY_64); BUZ_play(F1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
	}	
}

// ��ư�� �׸��� �Լ�
void onGame_drawButton() {
	// ������ ��ư
	if(g_buttonRight == FALSE) // ����
		LCD_DrawImageEx(335, 13, 170, 89, 46, 46, MiscImage, CR_TRANS);
	else // �ȴ���
		LCD_DrawImageEx(335, 13, 216, 89, 46, 46, MiscImage, CR_TRANS);
	// ���� ��ư
	if(g_buttonLeft == FALSE) // ����
		LCD_DrawImageEx(408, 13, 170, 43, 46, 46, MiscImage, CR_TRANS);
	else // �ȴ���
		LCD_DrawImageEx(408, 13, 216, 43, 46, 46, MiscImage, CR_TRANS);
}

// �������� ���� �׸��� �Լ�
void onGame_drawBoardBlock()
{
	int i = 0, j = 0;
	
	// �������� ������ �о�ͼ� ���� �׸�(�������� �����¿� 1���� ���� ������ -> �׸��� ����)
	for(i = 1; i <= SIZE_MAX_Y-2; i++) {
		for(j = 1; j <= SIZE_MAX_X-2; j++) {
			if(g_boardData[i][j] != 0 && g_boardData[i][j] != 1) {	// �ش� ��ġ�� �׸� ���� ������ �׸�
				if(g_itemGhost == FALSE) { // ��Ʈ ������ ȿ���� ���� ���� ��� ���� �׸�
					onGame_drawBlock(j, i, g_boardData[i][j], 0, FALSE);// �����ǿ��� ���� ���� ����
				}
				else { // ��Ʈ ������ ȿ���� ���� ��� �ı��Ǵ� ���� �׸�
					if(g_boardData[i][j] >= BLOCK_DESTROY) {
						onGame_drawBlock(j, i, g_boardData[i][j], 0, FALSE);// �����ǿ��� ���� ���� ����
					}
				}
			}
		}
	}
}

// ���� ���� �׷��ִ� �Լ�
void onGame_drawCurBlock()
{
	int i = 0, j = 0;
	
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			onGame_drawBlock(g_Xpos-1+j, g_Ypos-1+i, g_blockData[g_curShape][g_curPhase][i][j], 0, FALSE);	// ���� ���� �ƴ�
}

// �̸����� ��(���� ��)�� �׷��ִ� �Լ�
void onGame_drawPreviewBlock()
{	
	int i = 0, j = 0, k = 0;

	for(i = g_Ypos; i < SIZE_MAX_Y-1; i++) {
		if(onGame_checkBlockCrash(g_Xpos, i+1, g_curShape, g_curPhase) == TRUE) {	// ���� ��ġ���� �浹�� �Ͼ��
			for(j = 0; j < 4; j ++) {
				for(k = 0; k < 4; k++) {
					onGame_drawBlock(g_Xpos+k-1, i+j-1, g_blockData[g_curShape][g_curPhase][j][k], 0, TRUE);
				}
			}
			break;
		}
	}
}

// ���̵� ��(Ȧ��/������)�� �׷��ִ� �Լ�
void onGame_drawSideBlock(int type)
{
	int i = 0, j = 0;
	
	// ���� ���� �׷���
	if(type == 0) {
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++)
				onGame_drawBlock(i+1, j+1, g_blockData[g_nextShape][1][j][i], 1, FALSE);
	}
	
	// Ȧ�� ���� �׷���
		else if(type == 1) { 
			if(g_holdShape != -1)	// Ȧ��� ���� ������ (-1�� �ʱⰪ���� Ȧ��� ���� ������ ����)
				for(i = 0; i < 4; i++)
					for(j = 0; j < 4; j++)
						onGame_drawBlock(i+1, j+6+1, g_blockData[g_holdShape][1][j][i], 1, FALSE);
	}
}

// ������ ������ �׷���
void onGame_drawScore()
{
	int i = 0;
	int number[6];
	int tempValue = g_level;
	
	// LEVEL ����(01~99)
	number[1] = tempValue / 10;
	number[0] = tempValue % 10;
	LCD_DrawImageEx(12, 78, 186, 148, 16, 50, MiscImage, CR_TRANS);
	for(i = 0; i <= 1; i++)
		LCD_DrawImageEx(13, 48+(i*12), 240+(number[i]*16), 20, 16, 12, MiscImage, CR_TRANS);
	
	// SCORE ����(000000~999999)
	tempValue = g_score;
	number[5] = tempValue / 100000;		tempValue -= (number[5]*100000);
	number[4] = tempValue / 10000;		tempValue -= (number[4]*10000);
	number[3] = tempValue / 1000;		tempValue -= (number[3]*1000);
	number[2] = tempValue / 100;		tempValue -= (number[2]*100);
	number[1] = tempValue / 10;			tempValue -= (number[1]*10);
	number[0] = tempValue % 10;
	LCD_DrawImageEx(34, 78, 170, 148, 16, 50, MiscImage, CR_TRANS);
	for(i = 0; i <= 5; i++)
		LCD_DrawImageEx(35, 5+(i*12), 240+number[i]*16, 20, 16, 12, MiscImage, CR_TRANS);
}

// �ǹ� �������� �׷���
void onGame_drawFever()
{
	int i = 0;
	int tempFever = g_fever;
	const int imgFeverSizeX = 15;	// (��� �ǹ� �̹��� ũ�� ���: 15x170)
	const int imgFeverSizeY = 170;
	
	// Uart_Printf("fev1Pix:%d, fevImgStartY:%d\n", fev1Pix, fevImgStartY);
	LCD_DrawImageEx(66, 78+(imgFeverSizeY-tempFever), 153, 43+(imgFeverSizeY-tempFever), 15, tempFever, MiscImage, CR_TRANS);
}

// �� �ϳ��ϳ��� �׸��� �Լ�
void onGame_drawBlock(int x, int y, int color, int type, bool isGhost)
{
	color -= 2;
	
	// �����ǿ� �׸� ��	��ǥ
	if(type == 0) {
		if(x > 0 && x < SIZE_MAX_X-1 && y > 0 && y < SIZE_MAX_Y-1) {	// ������ �ȿ� ���� �������̸� �׸�
			if(isGhost == FALSE) {	// ���ɺ� ��尡 �ƴ� ��
				LCD_DrawImageEx(94-19+(y*19), 246+19+(x*-19), (18*color), 0, 18, 18, MiscImage, CR_TRANS);
			}	
			else {	// ���ɺ� ����� ��
				LCD_DrawAlphaImage(94-19+(y*19), 246+19+(x*-19), (18*color), 0, 18, 18, MiscImage, CR_TRANS, 64);
			}
		}
	}
	// ���̵���(Ȧ��/������)�� �׸� �� ��ǥ
	else if(type == 1) {
				if(isGhost == FALSE) {
				LCD_DrawImageEx(115-19+(y*19), 44+19+(x*-19), (18*color), 0, 18, 18, MiscImage, CR_TRANS);
			}	
			else {
				LCD_DrawAlphaImage(115-19+(y*19), 44+19+(x*-19), (18*color), 0, 18, 18, MiscImage, CR_TRANS, 64);
			}
	}
}

// �� �浹�� �����ϴ� �Լ�
bool onGame_checkBlockCrash(int x, int y, int shape, int phase)
{
	int i = 0, j = 0;
	int k = 0, l = 0;
	bool isCrashed = FALSE;
	
	for(i = y-1; i < ((y == SIZE_MAX_Y-2)?(y+2):(y+3)); i++) {
		l = 0;
		for(j = x-1; j < ((x == SIZE_MAX_Y-2)?(x+2):(x+3)); j++) {
			if(g_boardData[i][j] != 0 && g_blockData[shape][phase][k][l] != 0) {
				if(g_testMode) Uart_Printf("Block Crash Detected. (i=%d, j=%d, k=%d, l=%d, shape:%d, phase:%d)\n", i, j, k, l, shape, phase);
				isCrashed = TRUE;
				return isCrashed;
			}
			l++;
		}
		k++;
	}
	
	return isCrashed;
}

// ���� ��� �浹��ġ���� �̵���Ű�� �Լ�
void onGame_downImmediately()
{
	int i = 0;
	
	for(i = g_Ypos; i < SIZE_MAX_Y-1; i++) { // (i = 0; i < SIZE_MAX_Y+12-g_Ypos; i++)
		if(onGame_moveBlock(MB_DOWN) == TRUE) { }
		else break;
	 }
}

// ���� �̵���Ű�� �Լ�
bool onGame_moveBlock(int direction)
{
	bool isMoved = TRUE;

	if(direction == MB_LEFT) {	// ���� �̵��� ���
		if(g_itemReverse == FALSE) { // ������ ������ ȿ���� ���� ���
			if(onGame_checkBlockCrash(g_Xpos-1, g_Ypos, g_curShape, g_curPhase) == FALSE)	// ���� �̵��ص� �� �浹�� ���� ���
				g_Xpos--;
			else isMoved = FALSE;
		}
		else { // ������ ������ ȿ���� �ִ� ���
			if(onGame_checkBlockCrash(g_Xpos+1, g_Ypos, g_curShape, g_curPhase) == FALSE)	// ������ �̵��ص� �� �浹�� ���� ���
				g_Xpos++;
			else isMoved = FALSE;
		}
	}
	else if(direction == MB_RIGHT) { // ������ �̵��� ���
		if(g_itemReverse == FALSE) { // ������ ������ ȿ���� ���� ���
			if(onGame_checkBlockCrash(g_Xpos+1, g_Ypos, g_curShape, g_curPhase) == FALSE)	// ������ �̵��ص� �� �浹�� ���� ���
				g_Xpos++;
			else isMoved = FALSE;
		}
		else { // ������ ������ ȿ���� �ִ� ���
			if(onGame_checkBlockCrash(g_Xpos-1, g_Ypos, g_curShape, g_curPhase) == FALSE)	// ���� �̵��ص� �� �浹�� ���� ���
				g_Xpos--;
			else isMoved = FALSE;
		}
	}
	else if(direction == MB_DOWN) { // �Ʒ��� �̵��ϴ� ���
		if(onGame_checkBlockCrash(g_Xpos, g_Ypos+1, g_curShape, g_curPhase) == FALSE)	// �Ʒ��� �̵��ص� �� �浹�� ���� ���
			g_Ypos++;
		else {
			isMoved = FALSE;
			onGame_blockCrashed();	// �Ʒ��� �̵��� �źεǴ� ��쿡�� �浹 ó��
		}
	}
	
	return isMoved;	// �̵��� �㰡�Ǿ������� ��ȯ (�浹�� ���� ��� �㰡�Ǿ�:TRUE �浹�� �ִ� ��� �źεǾ�:FALSE ��ȯ)
}

// ���� ȸ����Ű�� �Լ�
bool onGame_turnBlock(int direction)
{
	bool isTurned = TRUE;
	int tempPhase = g_curPhase;
	
	if(direction == TB_LEFT) { // ���� ȸ���� ���
		if(onGame_checkBlockCrash(g_Xpos, g_Ypos, g_curShape, ((tempPhase == 0)?(tempPhase = 3):(--tempPhase))) == FALSE) // �·� ȸ���ص� �浹�� ���� ���
			g_curPhase = tempPhase;
		else isTurned = FALSE;
	}
	else if(direction == TB_RIGHT) { // ������ ȸ���� ���
		if(onGame_checkBlockCrash(g_Xpos, g_Ypos, g_curShape, ((tempPhase == 3)?(tempPhase = 0):(++tempPhase))) == FALSE) // ��� ȸ���ص� �浹�� ���� ���
			g_curPhase = tempPhase;
		else isTurned = FALSE;
	}
	
	return isTurned;
}

// ���� Ȧ���Ű�� Ȧ��� ���� �������� �Լ�
bool onGame_holdBlock()
{
	int tempBlock = -1;
	bool holdAccept = TRUE;

	if(g_holdAble == TRUE) { // Ȧ�尡 ������ ��(�̹� Ȧ�带 ���� �ʾ��� ��)
		if(g_holdShape == -1) { // �̹� Ȧ��� ���� ���� ��� ���� ���� Ȧ���Ű�� ���� �ٽ� ����
			g_holdShape = g_curShape;
			onGame_createBlock();
			g_holdAble = FALSE;	// �ѹ� Ȧ�带 �ϸ� ���� ���� ������ ������ Ȧ�� �Ұ���
			if(g_useSound) { BUZ_play(C1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
		}
		else {	// �̹� Ȧ��� ���� �ִ� ��� ��ü
			if(onGame_checkBlockCrash(g_Xpos, g_Ypos, g_holdShape, 1) == FALSE) { // Ȧ��� ���� �����͵� �浹�� ���� ��
				tempBlock = g_curShape;
				g_curShape = g_holdShape;
				g_curPhase = 1;
				g_holdShape = tempBlock;
				g_holdAble = FALSE;	// �ѹ� Ȧ�带 �ϸ� ���� ���� ������ ������ Ȧ�� �Ұ���
				if(g_useSound) { BUZ_play(C1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			}
		}
	}
	else { // Ȧ�尡 �Ұ��� �� ��
		holdAccept = FALSE;
		if(g_useSound) { BUZ_play(A1, DLY_32); BUZ_stop(); BUZ_play(B1, DLY_32); BUZ_stop(); }
	}
	
	return holdAccept;	// Ȧ�� ����/���� ���θ� ��ȯ
}

// �浹 ������ �Ͽ� ���� �����ϰ� ���� ���� �������� �����ϴ� �Լ�
bool onGame_createBlock()
{
	bool createAccept = TRUE;
	
	if(onGame_checkBlockCrash(g_Xpos, g_Ypos, g_nextShape, 1) == FALSE) { // ���� �� ������ �浹�� ����
		g_curShape = g_nextShape;
		g_curPhase = 1;
		
		if(g_itemLineBlock == FALSE) // ���� �� ������ ȿ�� ���� ��
			g_nextShape = (rand() + g_timeElapsed) % 7;
		else // ���� �� ������ ȿ�� ���� ��
			g_nextShape = 1;
	}
	else createAccept = FALSE;	// �浹 �������� ���� �� ���� ����
	
	return createAccept;	// ���� ���� ��뿩�θ� ��ȯ
}

// ���� ���� �浹�Ͽ������ ó���ϴ� ��ƾ
bool onGame_blockCrashed()
{
	int i = 0, j = 0;
	int gameState = OGS_PLAYING;
	
	// �����ǿ� �浹�� �� ������ �߰�				
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
	   		if(g_blockData[g_curShape][g_curPhase][i][j] != 0) {
		 		g_boardData[g_Ypos+i-1][g_Xpos+j-1] = g_blockData[g_curShape][g_curPhase][i][j];
			}
		}				
	}
	// ���� �÷���
	if(g_useSound) { BUZ_play(E1, DLY_64); BUZ_stop(); }
	// ������ ������ �ִ°� Ȯ��
	onGame_removeLine(FALSE);
	// ��ǥ�� �ٽ� ���� �÷��ְ� ���� ���� �������� ���� ����
	g_Xpos = (SIZE_MAX_X/2)-1;		g_Ypos = 2;
	g_curShape = g_nextShape;		g_curPhase = 1;
	if(onGame_createBlock() == FALSE) {  // �浹�� ���� �� �� ���� ���� �� ���� ������ ��ȯ�ϵ���
		g_onGameState = OGS_GAMEOVER;
		if(g_gameState == GS_MULTI) // ��Ƽ�÷����� ��� ���濡�� �ڽ��� �й��ߴٴ� ����� ����
			Bluetooth_Putc(OGS_LOSE);
	}
	// Ȧ�尡 �ٽ� �����ϰ� ��
	g_holdAble = TRUE;

	return gameState;	// ���� ���� ��ȯ
}

// ��ü ���带 Ȯ���Ͽ� ���ִ� ���� ó���ϴ� �Լ�
bool onGame_removeLine(bool useRemove)
{
	int comboCnt = 0;
	int pushLevel = 0;
	bool isLineCleared = FALSE;
	int i = 0, j = 0, k = 0, l = 0; 
	
 	//for(i=((g_Ypos-1>0)?(g_Ypos-1):(1)); i<=((g_Ypos+2<SIZE_MAX_Y-1)?(g_Ypos+2):(SIZE_MAX_Y-2)); i++) { // (�� ��ǥ �������� 4ĭ�� �˻��ų �� ���)
 	for(i = 1; i <= SIZE_MAX_Y-2; i++) {
		if(onGame_checkLine(i)) { // ����� �� �ִ� �κ��� ���� ä���� �ִ��� ���� -> ä���� ������ ����
  			if(useRemove == FALSE) { // ���� �ı����� ������ ������� �ش�
	  			for(j=1; j<SIZE_MAX_X-1; j++) { // �ش� ������ �� ������� ä�� ���� 
					g_boardData[1][j] = 0; // �� �� ������ ������ �������Ƿ� 0���� ä�� �ִ´�
					g_boardData[i][j] = BLOCK_DESTROY;
					
					isLineCleared = TRUE;
			 	}
		 	}
		 	else if(useRemove == TRUE) { // ���� �ı��ϰ� ������ �޺� ���� ������ �Ѵ�
				for(j=i-1; j>1; j--) { // ���� ������ �� ĭ ������ �ö󰡸�
		 			for(k=1; k<SIZE_MAX_X-1; k++)
						g_boardData[j+1][k] = g_boardData[j][k]; // ���� ������ �� ĭ ���� ������ ��ü
					}
					// �Ҹ� ���
					if(g_useSound) { BUZ_play(C1, DLY_64); BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_play(F1, DLY_64); BUZ_stop(); }

					// ����� ���μ��� ����
					g_lineRemoved++;
				
					// ���κ��� �޺� ī���� ���� (�߽� �޺�: 99)
					comboCnt++; // (�̹��� ����� �� �� ��)
					(g_comboCounter < 99) ? (g_comboCounter++) : (g_comboCounter = 99);

					// ������ ��������� ����
					isLineCleared = TRUE;
			}
		}
	}
	
	if(isLineCleared == FALSE) g_comboCounter = 0;	// ������ ������ ������ �޺��� ��Ʈ��
	if(useRemove == TRUE) {// ���� �ı��ϰ� ������ �޺� ���� ������ �Ѵ�
		if(g_testMode) { Uart_Printf("Removed Line:%d\n", g_lineRemoved); Uart_Printf("Current Combo:%d\n", g_comboCounter); }
		if(g_comboCounter >= 3) {
			int number[2];
			int tempValue = g_comboCounter;
			
			// Combo ���� (00~99)
			number[1] = tempValue / 10;
			number[0] = tempValue % 10;
			
			for(i = 0; i < 2; i ++) {
				if(number[i] == 0) 		number[i] = SPRITE_COMBO_NUM0;
				else if(number[i] == 1) number[i] = SPRITE_COMBO_NUM1;
				else if(number[i] == 2) number[i] = SPRITE_COMBO_NUM2;
				else if(number[i] == 3) number[i] = SPRITE_COMBO_NUM3;
				else if(number[i] == 4) number[i] = SPRITE_COMBO_NUM4;
				else if(number[i] == 5) number[i] = SPRITE_COMBO_NUM5;
				else if(number[i] == 6) number[i] = SPRITE_COMBO_NUM6;
				else if(number[i] == 7) number[i] = SPRITE_COMBO_NUM7;
				else if(number[i] == 8) number[i] = SPRITE_COMBO_NUM8;
				else					number[i] = SPRITE_COMBO_NUM9;
			}
			
			LCD_Sprite(276, 143, number[0], 16, 32);	// �޺� ���� ���ڸ�
			LCD_Sprite(276, 174, number[1], 16, 32);	// �޺� ���� ���ڸ�
			LCD_Sprite(327, 98, SPRITE_COMBO, 16, 32);	// �޺� ����
		}
		
		// ��Ƽ ���� ���濡�� ȸ�� �� ���� �õ�
		if(g_gameState == GS_MULTI) { 
			if(comboCnt >= 2) { // �ѹ��� �� �� �̻� ���� ���� ��
				pushLevel = (comboCnt/2) + (g_level/33);
				if(pushLevel > 4) pushLevel = 4;
				switch(pushLevel) {
					case 1:
						Bluetooth_Putc(EFFECT_PUSHLINE_1);
						break;
					case 2:
						Bluetooth_Putc(EFFECT_PUSHLINE_2);
						break;
					case 3:
						Bluetooth_Putc(EFFECT_PUSHLINE_3);
						break;
					case 4:
						Bluetooth_Putc(EFFECT_PUSHLINE_4);
						break;
					default: break;
				}		
			}
		}
		
		// �ǹ��� ���� ����
		onGame_addScoreAndFeverAndItem(g_comboCounter);
	}
	
	return isLineCleared;	// ������ ����� ���θ� ��ȯ
}

// �Ű������� ���� ���� �� ���ִ��� Ȯ���ϴ� �Լ�
bool onGame_checkLine(int lineNumber)
{
 	 int j = 0;

 	 for(j = 1; j < SIZE_MAX_X-1; j++) {
	  	 if(g_boardData[lineNumber][j] == 0) // �����߿� �� ĭ�� ������ 
			 return FALSE;					 // �˻��� ������ �� ä���� ���� ������ ��ȯ 
     }
     
     return TRUE; // �˻��� ������ �� ä���� ������ ��ȯ 
}

// ������ �ǹ� �������� ������Ű�� �������� �߰���Ű�� �Լ�
void onGame_addScoreAndFeverAndItem(int comboCnt)
{
	int ledLight = 0;
	
	g_score += ((g_level*10) * comboCnt);
	g_fever += (10 + (comboCnt * 10) + (g_level / 3));
	
	if(g_score > 999999) g_score = 999999;	// �Ѱ�ġ
	if(g_fever >= NUM_MAX_FEVER) { // �ǹ��� NUM_MAX_FEVER(170)���� ����
		if(g_itemSlot[0] == ITEM_NULL || g_itemSlot[1] == ITEM_NULL) { // ������ ������ �ϳ��� ��� ������
			g_fever = 0;
			
			// �� ���Կ� ������ �߰�
			if(g_itemSlot[0] == ITEM_NULL) {
				if(g_gameState == GS_MULTI) { g_itemSlot[0] = ((rand() + g_timeElapsed) % NUM_OF_ITEM) + 100; }
				else if(g_gameState == GS_SINGLE) { g_itemSlot[0] = ((rand() + g_timeElapsed) % 4) + 105; }
			}	
			else if(g_itemSlot[1] == ITEM_NULL) {
				if(g_gameState == GS_MULTI) { g_itemSlot[1] = ((rand() + g_timeElapsed) % NUM_OF_ITEM) + 100; }
				else if(g_gameState == GS_SINGLE) { g_itemSlot[1] = ((rand() + g_timeElapsed) % 4) + 105; }
			}
		}
		else { // ������ ������ �� �� �� ������
			g_fever = NUM_MAX_FEVER;
		}
	}
	
	// LED�Һ��� Fever�������� �°� ä����
	if(g_fever < 0) ledLight = 0;
	else if(g_fever < (NUM_MAX_FEVER / 4)    ) ledLight = 8;
	else if(g_fever < (NUM_MAX_FEVER / 4) * 2) ledLight = 12;
	else if(g_fever < (NUM_MAX_FEVER / 4) * 3) ledLight = 14;
	else if(g_fever >= NUM_MAX_FEVER) ledLight = 15;
	LED_Segment(ledLight);
}

// ������ ������ �׸��� �Լ�
void onGame_drawItemSlot()
{
	int itemID = g_itemSlot[0];

	// ������ 1
	if(itemID != ITEM_NULL)
		LCD_DrawImageEx(6, 217, 0+((itemID-100)%3)*50, 42+((itemID-100)/3)*50, 50, 50, MiscImage, CR_TRANS);

	// ������ 2
	itemID = g_itemSlot[1];
	if(itemID != ITEM_NULL)
		LCD_DrawImageEx(6, 157, 0+((itemID-100)%3)*50, 42+((itemID-100)/3)*50, 50, 50, MiscImage, CR_TRANS);
}

// ������ ���� ȣ���ϴ� �Լ�
void onGame_Item_UseItem(int itemID)
{	
	switch(itemID) {
		case ITEM_NULL:
			if(g_useSound) { BUZ_play(F1, DLY_64); BUZ_play(F1, DLY_64); BUZ_stop(); }
			break;
		case ITEM_CLOUDE:
			if(g_useSound) { BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			Bluetooth_Putc(ITEM_CLOUDE); // ���� ������ ȿ���� ��뿡�� ����
			break;
		case ITEM_MISSILE:
			if(g_useSound) { BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			Bluetooth_Putc(ITEM_MISSILE); // �̻��� ������ ȿ���� ��뿡�� ����
			break;
		case ITEM_GHOST:
			if(g_useSound) { BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			Bluetooth_Putc(ITEM_GHOST); // ���� ������ ȿ���� ��뿡�� ����
			break;
		case ITEM_REVERSE:
			if(g_useSound) { BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			Bluetooth_Putc(ITEM_REVERSE); // ���� ������ ȿ���� ��뿡�� ����
			break;
		case ITEM_HARDBLOCK:
			if(g_useSound) { BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			Bluetooth_Putc(ITEM_HARDBLOCK); // �ϵ��� ������ ȿ���� ��뿡�� ����
			break;
		case ITEM_TIMESTOP:
			if(g_useSound) {  BUZ_play(E1, DLY_64); BUZ_play(D1, DLY_64); BUZ_stop(); }
			onGame_Item_SetEffectTime(ITEM_TIMESTOP); // �ð����� �������� ���ӽð��� Set
			break;
		case ITEM_TIMEBACK:
			if(g_useSound) {  BUZ_play(E1, DLY_64); BUZ_play(D1, DLY_64); BUZ_stop(); }
			onGame_Item_Effect(ITEM_TIMEBACK); // �ð� �ǵ����� �������� ȿ���� �ߵ���Ŵ
			break;
		case ITEM_SHIELD:
			if(g_useSound) {  BUZ_play(E1, DLY_64); BUZ_play(D1, DLY_64); BUZ_stop(); }
			onGame_Item_Effect(ITEM_SHIELD); // ���� �������� ȿ���� �ߵ���Ŵ
			break;
		case ITEM_REMOVELINE:
			if(g_useSound) {  BUZ_play(E1, DLY_64); BUZ_play(D1, DLY_64); BUZ_stop(); }
			onGame_Item_Effect(ITEM_REMOVELINE); // ���� ���� �������� ȿ���� �ߵ���Ŵ
			break;
		case ITEM_LINEBLOCK:
			if(g_useSound) {  BUZ_play(E1, DLY_64); BUZ_play(D1, DLY_64); BUZ_stop(); }
			onGame_Item_SetEffectTime(ITEM_LINEBLOCK); // ���� ���� ������ ȿ���� �ߵ���Ŵ
			break;
		case ITEM_CHANGELINE:
			if(g_useSound) { BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			Bluetooth_Putc(ITEM_CHANGELINE); // ü���� ���� ������ ȿ���� ���濡�� ����
			break;
		case ITEM_IRONBLOCK:
			if(g_useSound) { BUZ_play(D1, DLY_64); BUZ_play(E1, DLY_64); BUZ_stop(); }
			Bluetooth_Putc(ITEM_IRONBLOCK); // ��ö �� ȿ���� ���濡�� ���޽�Ŵ
			break;
		default: break;
	}
}

// ������ ȿ���� ���� �� ȣ���ϴ� �Լ� (������� ������ �����͸� ���⼭ �а� �׿� �����ϴ� ȿ���� �ο� + �������� ���� ����� ����)
void onGame_readItem(int itemID)
{
	switch(itemID) {
		case OGS_WAITING: // ��뿡�� �� ��ȣ�� ���� ���� �� �ڽ��� ���°� ������̸� ��뿡�Ե� �� ��ȣ�� ������ �ڽ��� OGS_READY���·� ��ȯ.
			if(g_onGameState == OGS_WAITING) {
				Bluetooth_Putc(OGS_WAITING);
				g_onGameState = (OGS_READY);
			}
			break;
		case OGS_LOSE:	// ��뿡�� �� ��ȣ�� ���� ���� ��
			g_onGameState = OGS_WIN; // �ڽ��� �¸� ���·� ����
			break;
		case ITEM_NULL:
			break;
		case ITEM_CLOUDE:
			onGame_Item_SetEffectTime(ITEM_CLOUDE); // ���� ȿ���� ���ӽð��� Set
			break;
		case ITEM_MISSILE:
			onGame_Item_Effect(ITEM_MISSILE); // �̻��� ȿ���� �ߵ���Ŵ
			break;
		case ITEM_GHOST:
			onGame_Item_SetEffectTime(ITEM_GHOST); // ���� ȿ���� ���ӽð��� Set
			break;
		case ITEM_REVERSE:
			onGame_Item_SetEffectTime(ITEM_REVERSE); // ���� ȿ���� ���ӽð��� Set
			break;
		case ITEM_HARDBLOCK:
			onGame_Item_Effect(ITEM_HARDBLOCK); // �ϵ��� ȿ���� �ߵ���Ŵ
			break;
		case ITEM_CHANGELINE:
			onGame_Item_Effect(ITEM_CHANGELINE); // ü���� ���� ȿ���� �ߵ���Ŵ
			break;
		case ITEM_IRONBLOCK:
			onGame_Item_SetEffectTime(ITEM_IRONBLOCK); // ��ö ����� ���ӽð��� Set
			break;
		case EFFECT_PUSHLINE_4:
			onGame_Item_Effect(EFFECT_PUSHLINE_1); // ���� ��ĭ ���� �����
		case EFFECT_PUSHLINE_3:
			onGame_Item_Effect(EFFECT_PUSHLINE_1); // ���� ��ĭ ���� �����
		case EFFECT_PUSHLINE_2:
			onGame_Item_Effect(EFFECT_PUSHLINE_1); // ���� ��ĭ ���� �����
		case EFFECT_PUSHLINE_1:
			onGame_Item_Effect(EFFECT_PUSHLINE_1); // ���� ��ĭ ���� �����
			break;
		default: break;
	}
}

// ������ ���� �ð��� üũ�ϴ� �Լ�
void onGame_Item_CheckLeftTime()
{
	if((g_timeElapsed > g_itemCloudeTime) && g_itemCloude) { // Ŭ���� ������
		g_itemCloude = FALSE;
		onGame_Item_Diagram(TRUE, ITEM_CLOUDE);
	}
	if((g_timeElapsed > g_itemGhostTime) && g_itemGhost) { // ��Ʈ ������
		g_itemGhost = FALSE;
		onGame_Item_Diagram(TRUE, ITEM_GHOST);
	}
	if((g_timeElapsed > g_itemReverseTime) && g_itemReverse) { // ������ ������
		g_itemReverse = FALSE;
		onGame_Item_Diagram(TRUE, ITEM_REVERSE);
	}
	if((g_timeElapsed > g_itemTimestopTime) && g_itemTimestop) { // �ð����� ������
		g_itemTimestop = FALSE;
		onGame_Item_Diagram(TRUE, ITEM_TIMESTOP);
	}
	if((g_timeElapsed > g_itemLineBlockTime) && g_itemLineBlock) { // ���� �� ������
		g_itemLineBlock = FALSE;
		onGame_Item_Diagram(TRUE, ITEM_LINEBLOCK);
	}
	if((g_timeElapsed > g_itemIronBlockTime) && g_itemIronBlock) { // ��ö �� ������
		g_itemIronBlock = FALSE;
		onGame_Item_Diagram(TRUE, ITEM_IRONBLOCK);
	}
}

// ������ ���ӽð��� ������Ű�� �Լ�
void onGame_Item_SetEffectTime(int itemID)
{
	switch(itemID) {
		case ITEM_CLOUDE:
			g_itemCloudeTime = g_timeElapsed + 15;	// ���� �ð� 15��
			g_itemCloude = TRUE;					// ���� ����
			onGame_Item_Diagram(FALSE, ITEM_CLOUDE);
			break;
		case ITEM_GHOST:
			g_itemGhostTime = g_timeElapsed + 10;	// ���� �ð� 10��
			g_itemGhost = TRUE;						// ���ɺ� ���
			onGame_Item_Diagram(FALSE, ITEM_GHOST);
			break;
		case ITEM_REVERSE:
			g_itemReverseTime = g_timeElapsed + 12; // ���� �ð� 12��
			g_itemReverse = TRUE;					// Ű ���� ���
			onGame_Item_Diagram(FALSE, ITEM_REVERSE);
			break;
		case ITEM_TIMESTOP:
			g_itemTimestopTime = g_timeElapsed + 10;// ���� �ð� 10��
			g_itemTimestop = TRUE;					// �� ���� ����
			break;
		case ITEM_LINEBLOCK:
			g_itemLineBlockTime = g_timeElapsed + 6; // ���� �� ���ӽð� 6��
			g_itemLineBlock = TRUE;					 // ���ں��� ������ ����
			g_nextShape = 1;						 // ���� ���� ���� ������ ����
			break;
		case ITEM_IRONBLOCK:
			g_itemIronBlockTime = g_timeElapsed + 8; // ��ö �� ���ӽð� 8��
			g_itemIronBlock = TRUE;					 // �� �ı��� �ź��ϱ� ����
			onGame_Item_Diagram(FALSE, ITEM_IRONBLOCK);
			break;
	}
}

void onGame_Item_Effect(int itemID)
{
	int i = 0, j = 0;
	int x = 0, y = 0;
	int boardColData[SIZE_MAX_Y];
	
	switch(itemID) {
		case ITEM_MISSILE: // �̻��� ������(������ ���)
			if(g_testMode) { Uart_Printf("[ITEM_MISSILE] Avoked!\n"); }
			for(i = 0; i < 8; i++) { // 8���� ������ ���� (�ַ� �Ʒ��� �븲)
				x = ((rand() + g_timeElapsed) % (SIZE_MAX_X-2)) + 1;
				y = ((rand() + g_timeElapsed) % (SIZE_MAX_Y-10)) + 9;
				if(g_boardData[y][x] != 0 && g_boardData[y][x] < BLOCK_DESTROY && g_boardData[y][x] != 1) // ������ ��ǥ�� ������� ������ ���� ����
					g_boardData[y][x] = 0;
				LCD_Sprite(94-19+(y*19)-30, 246+19+(x*-19)-32, SPRITE_EFFECT_MISSILE, 14, 0); // ���� ����Ʈ(-30, -32�� ������ ����Ʈ�� �߾ӿ� ���� ��ġ�ϱ� ����)
				if(g_testMode) { Uart_Printf("[ITEM_MISSILE] %dst / x:%d, y:%d\n", i, x, y); }
			}
			onGame_Item_Diagram(TRUE, ITEM_MISSILE);
			break;
		case ITEM_HARDBLOCK: // �ϵ��� ������(������ ���)
			if(g_testMode) { Uart_Printf("[ITEM_HARDBLOCK] Avoked!\n"); }
			g_nextShape = ((rand() + g_timeElapsed) % 7) + 7; // ���� ���� ����� ����� ������ ���� (7~13�� Ÿ��)
			onGame_Item_Diagram(FALSE, ITEM_HARDBLOCK);
			break;
		case ITEM_TIMEBACK:
			if(g_testMode) { Uart_Printf("[ITEM_TIMEBACK] Avoked!\n"); }
			g_Xpos = (SIZE_MAX_X/2)-1;
			g_Ypos = 2;
			for(i = 1; i <= SIZE_MAX_Y-1; i++)
				for(j = 1; j <= SIZE_MAX_X-1; j++)
					g_boardData[i][j] = g_itemTimeBackBoard[i][j]; // 1���� ����� ������ ����� ���� ���� �����͸� ����
			break;
		case ITEM_SHIELD: // ���� ������(������ ���)
			if(g_testMode) { Uart_Printf("[ITEM_SHIELD] Avoked!\n"); } // ���� �����̻��� ������
			g_itemCloudeTime = 0;	g_itemGhostTime = 0;	g_itemReverseTime = 0; g_itemIronBlockTime = 0;
			g_itemCloude = FALSE;	g_itemGhost = FALSE;	g_itemReverse = FALSE; g_itemIronBlock = FALSE;
			break;
		case ITEM_REMOVELINE: // ���� ���� ������(������ ���)
			for(i = SIZE_MAX_Y-5; i <= SIZE_MAX_Y-2; i++) // �Ʒ����� 3ĭ�� �ı���
				for(j = 1; j <= SIZE_MAX_X-2; j++)
					if(g_boardData[i][j] < BLOCK_DESTROY)
						g_boardData[i][j] = BLOCK_DESTROY;	// �ı��Ǵ� ������ ��ü
			break;
		case ITEM_CHANGELINE: // ���� Ȧ¦�� ���� ������
			for(i = 1; i <= SIZE_MAX_X-2; i++) {
				if(i % 2 != 0) { // Ȧ�� ���� �� Ȧ�� �� �����͸� ���� �� ¦������ �����͸� Ȧ������ �׷��ְ� ¦������ �����͸� ����� ���� ��ü
					for(j = 1; j <= SIZE_MAX_Y-2; j++)
						boardColData[j-1] = g_boardData[j][i];
					for(j = 1; j <= SIZE_MAX_Y-2; j++)
						g_boardData[j][i] = g_boardData[j][i+1];
					for(j = 1; j <= SIZE_MAX_Y-2; j++)
						g_boardData[j][i+1] = boardColData[j-1];
			onGame_Item_Diagram(FALSE, ITEM_CHANGELINE);
				}
			}
			break;
		case EFFECT_PUSHLINE_1: // ���� ��ĭ ���� ����ϰ� ��ĭ ���� �ϳ� ����
			// �� ���� ���
			for(i = 2; i <= SIZE_MAX_Y-2; i++) {
				for(j = 1; j <= SIZE_MAX_X-2; j++) {
					g_boardData[i-1][j] = g_boardData[i][j];
				}
			}
			// �� �Ʒ� ��ĭ ���
			j = ((rand() + g_timeElapsed) % (SIZE_MAX_X-1)) + 1; // ��ĭ�� ���� ���� ��ǥ
			for(i = 1; i <= SIZE_MAX_X-2; i++) {
				if(i != j) { g_boardData[SIZE_MAX_Y-2][i] = 15; } // �� ��ǥ�� �����ϰ� �Ϸķ� ä�� ����
				else { g_boardData[SIZE_MAX_Y-2][i] = 0; } // �� �� ��ǥ�� 0���� ä������
			}
			break;
		default: break;
	}
}

// ������ ȿ�� �����ó� ������ ��ǳ���� �׸��� �Լ�
void onGame_Item_Diagram(bool isBuf, int itemID)
{
	if(isBuf == TRUE) { // ���� ȿ��(����� ���ŵ�)
		LCD_Sprite(14, 0, SPRITE_DIAGRAM_BUFF, 48, 0);
		LCD_Sprite(14+80, 0+150, itemID-85, 48, 0);
		if(g_useSound) { BUZ_play(F1, DLY_64); BUZ_play(G1, DLY_64); BUZ_stop(); }
	}
	else { // ���� ȿ��(����� �ɸ�)
		LCD_Sprite(14, 0, SPRITE_DIAGRAM_DEBUFF, 32, 0);
		LCD_Sprite(14+80, 0+150, itemID-85, 32, 0);
		if(g_useSound) { BUZ_play(G1, DLY_64); BUZ_play(F1, DLY_64); BUZ_stop(); }
	}
}

// ������ �ʱ�ȭ�ϴ� �Լ�
void initalizeTetris()
{
	int i = 0, j = 0;
	
	// ���� ���� �ʱ�ȭ(���� ȭ��)
	g_gameState = GS_MAIN;
	// ���� ���� �ʱ�ȭ(���� �ȿ����� �߰����� ���� = OGS_READY)
	g_onGameState = OGS_READY;
	// �ð� ���� �ʱ�ȭ
	g_timeElapsed = 0;
	// LED �ʱ�ȭ
	LED_Segment(0);
	// ���� �ʱ�ȭ
	BUZ_stop();
	
	// ������ �ʱ�ȭ
	for(i = 0; i < SIZE_MAX_Y; i++) {
		for(j=0; j<SIZE_MAX_X; j++) {
			if(i == 0 || i == SIZE_MAX_Y-1 || j == 0 || j == SIZE_MAX_X-1) {
				g_boardData[i][j] = 1; // �����¿� 1�������� ��
				g_itemTimeBackBoard[i][j] = 1; // ������ ȿ���� ����� ���� ���� ���� �ʱ�ȭ
			}
			else {
				g_boardData[i][j] = 0; // �� �� ������ �������
				g_itemTimeBackBoard[i][j] = 0; // ������ ȿ���� ����� ���� ���� ���� �ʱ�ȭ
			}
		}
    }

	// �����÷��� ���� �ʱ�ȭ
	g_Xpos = (SIZE_MAX_X/2)-1;	// X��ǥ�� ������ ũ�� �߾��� -1
	g_Ypos = 2;					// Y��ǥ�� ���κ��� 2��° ��ġ
	g_curShape  = rand() % 7;	// �ʱ� ���� ������ ���
	g_nextShape = rand() % 7;	// ���� ���� ������ ���
	g_curPhase  = 1;			// �ʱ� ���� ������� 1
	g_holdShape = -1;			// �ʱ� Ȧ����� ����(-1)
	g_level = 1;				// �ʱ� ������ 1
	g_score = 0;				// �ʱ� ������ 0
	g_fever = 0;				// �ʱ� �ǹ��� 0
	g_comboCounter = 0;			// �ʱ� �޺� ī���ʹ� 0
	g_lineRemoved = 0;			// ���ŵ� ���� ��
	g_itemSlot[0] = ITEM_NULL;	// ������ ������ ����ְ� ��
	g_itemSlot[1] = ITEM_NULL;
	
	// ��ư ���� �ʱ�ȭ
	g_buttonRight = FALSE;						// ������ ��ư
	g_buttonLeft = FALSE;						// ���� ��ư
	
	// ������ ȿ�� �ʱ�ȭ
	g_itemCloude = FALSE;						// Ŭ���� ������ ȿ��
	g_itemGhost = FALSE;						// ��Ʈ ������ ȿ��
	g_itemReverse = FALSE;						// �¿� ����Ű ���� ȿ��
	g_itemTimestop = FALSE;						// �ð� ���� ȿ��
	g_itemLineBlock = FALSE;					// ���� ���� ������ ������
	g_itemIronBlock = FALSE;					// ���� �ı����� �ʴ� ȿ��
	g_itemCloudeTime = 0;
	g_itemGhostTime = 0;
	g_itemReverseTime = 0;
	g_itemTimestopTime = 0;
	g_itemLineBlock = 0;
	g_itemIronBlockTime = 0;
			
	// �� ������ 2���� �迭�� ����
	int blockData[NUM_MAX_BLOCK_TYPE*4][16] = { { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0}, // 14 ������ ��
											    { 0, 2, 2, 0,   0, 2, 2, 0,   0, 2, 2, 0,   0, 2, 2, 0}, // ������ 4���� ���
			                                    { 0, 2, 2, 0,   0, 2, 2, 0,   0, 2, 2, 0,   0, 2, 2, 0},
											    { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 { 0, 0, 0, 0,   0, 3, 0, 0,   0, 0, 0, 0,   0, 3, 0, 0},
								 	 { 3, 3, 3, 3,   0, 3, 0, 0,   3, 3, 3, 3,   0, 3, 0, 0},
								 	 { 0, 0, 0, 0,   0, 3, 0, 0,   0, 0, 0, 0,   0, 3, 0, 0},
								 	 { 0, 0, 0, 0,   0, 3, 0, 0,   0, 0, 0, 0,   0, 3, 0, 0},
  										 { 0, 4, 0, 0,   0, 0, 0, 0,   4, 0, 0, 0,   0, 4, 4, 0},
										 { 0, 4, 4, 0,   0, 4, 4, 0,   4, 4, 0, 0,   4, 4, 0, 0},
										 { 0, 0, 4, 0,   4, 4, 0, 0,   0, 4, 0, 0,   0, 0, 0, 0},
										 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},					
									 { 0, 0, 5, 0,   0, 0, 0, 0,   0, 5, 0, 0,   5, 5, 0, 0},
									 { 0, 5, 5, 0,   5, 5, 0, 0,   5, 5, 0, 0,   0, 5, 5, 0},
									 { 0, 5, 0, 0,   0, 5, 5, 0,   5, 0, 0, 0,   0, 0, 0, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
										 { 0, 6, 0, 0,   0, 6, 0, 0,   0, 0, 0, 0,   0, 6, 0, 0},
										 { 6, 6, 6, 0,   0, 6, 6, 0,   6, 6, 6, 0,   6, 6, 0, 0},
										 { 0, 0, 0, 0,   0, 6, 0, 0,   0, 6, 0, 0,   0, 6, 0, 0},
										 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 { 7, 0, 0, 0,   0, 7, 7, 0,   0, 0, 0, 0,   0, 7, 0, 0},
									 { 7, 7, 7, 0,   0, 7, 0, 0,   7, 7, 7, 0,   0, 7, 0, 0},
									 { 0, 0, 0, 0,   0, 7, 0, 0,   0, 0, 7, 0,   7, 7, 0, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
										 { 0, 0, 8, 0,   0, 8, 0, 0,   0, 0, 0, 0,   8, 8, 0, 0},
										 { 8, 8, 8, 0,   0, 8, 0, 0,   8, 8, 8, 0,   0, 8, 0, 0},
										 { 0, 0, 0, 0,   0, 8, 8, 0,   8, 0, 0, 0,   0, 8, 0, 0},
										 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 { 0, 9, 0, 0,   0, 9, 0, 0,   0, 9, 0, 0,   0, 9, 0, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
										 { 0,10, 0, 0,   0,10, 0, 0,   0,10, 0, 0,   0,10, 0, 0},
										 {10,10,10, 0,  10,10,10, 0,  10,10,10, 0,  10,10,10, 0},
										 { 0,10, 0, 0,   0,10, 0, 0,   0,10, 0, 0,   0,10, 0, 0},
										 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 { 0,11,11, 0,  11, 0, 0, 0,   0,11,11, 0,  11, 0, 0, 0},
									 { 0,11, 0, 0,  11,11,11, 0,   0,11, 0, 0,  11,11,11, 0},
									 {11,11, 0, 0,   0, 0,11, 0,  11,11, 0, 0,   0, 0,11, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
										 {12,12, 0, 0,   0, 0,12, 0,  12,12, 0, 0,   0, 0,12, 0},
										 { 0,12, 0, 0,  12,12,12, 0,   0,12, 0, 0,  12,12,12, 0},
										 { 0,12,12, 0,  12, 0, 0, 0,   0,12,12, 0,  12, 0, 0, 0},
										 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 {13, 0, 0, 0,  13,13,13, 0,  13,13,13, 0,   0, 0,13, 0},
									 {13, 0, 0, 0,  13, 0, 0, 0,   0, 0,13, 0,   0, 0,13, 0},
									 {13,13,13, 0,  13, 0, 0, 0,   0, 0,13, 0,  13,13,13, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
										 {14, 0,14, 0,  14,14,14, 0,  14, 0,14, 0,  14,14,14, 0},
										 {14,14,14, 0,   0,14, 0, 0,  14,14,14, 0,   0,14, 0, 0},
										 {14, 0,14, 0,  14,14,14, 0,  14, 0,14, 0,  14,14,14, 0},
										 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0},
									 {15,15, 0, 0,   0,15,15, 0,  15,15, 0, 0,   0,15,15, 0},
									 {15,15,15, 0,  15,15,15, 0,  15,15,15, 0,  15,15,15, 0},
									 { 0,15,15, 0,  15,15, 0, 0,   0,15,15, 0,  15,15, 0, 0},
									 { 0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0} };
				   
	// 2���� �迭�� �� ������ 4���� �迭�� ��ȯ (sh: shape(���) / ph: phase(����))
	int sh = 0, ph = 0;
    for(sh = 0; sh < NUM_MAX_BLOCK_TYPE; sh++)
		for(ph = 0; ph < 4; ph++)
			for(i = (sh*4); i < (sh*4+4); i++)
	   			for(j = (ph*4); j < (ph*4+4); j++)
					g_blockData[sh][ph][(i+4)%4][(j+4)%4] = blockData[i][j];
}

//*** ���ͷ�Ʈ ��ƾ ***//
// RTC 1�� �ֱ� ���ͷ�Ʈ ��ƾ
void RTC_tick_routine() {
	int i = 0, j = 0;
	
	// ���α׷� �������κ��� ����� �ð� ����
	if(g_timeElapsed < 2147483647) g_timeElapsed++;
	else g_timeElapsed = 0;
	
	// �� 1�и��� ���� ������ ����(Ÿ�ӹ� ������ ȿ���� ����)
	if(g_timeElapsed % 60 == 0)
		for(i = 1; i <= SIZE_MAX_Y-1; i++)
			for(j = 1; j <= SIZE_MAX_X-1; j++)
				g_itemTimeBackBoard[i][j] = g_boardData[i][j];
}

// ������� ���ͷ�Ʈ ��ƾ
void __irq Receive_Uart(void) {
	rINTSUBMSK |= (BIT_SUB_RXD1);
		
	// ******* USER CODE ********		
	bt_data = Bluetooth_Getc();
	Uart_Printf("Bluetooth Input Data : %c \r\n", bt_data);	
	
	onGame_readItem(bt_data); // ������ ȿ���� ����
	// **************************
		
	rSUBSRCPND |= BIT_SUB_RXD1;
	rINTSUBMSK &= ~(BIT_SUB_RXD1);
	rSRCPND |= (BIT_UART1);
	rINTPND |= (BIT_UART1);
}

// ��ġ ���ͷ�Ʈ ��ƾ
void __irq AdcTs_Auto(void)
{
	int i = 0, j = 0;
	int transXdata = 0, transYdata = 0;	// Save LCD x,y conversion coordinate

	rINTMSK |= (U32)BIT_ADC;			//ADC Touch Screen Mask bit clear
	rINTSUBMSK |= (BIT_SUB_ADC | BIT_SUB_TC);	// Mask sub interrupt (ADC and TC)
	
	// ��ġ�� ����
	if(rSUBSRCPND & (0x1<<10))			// ADC interrupt
	{
		rSUBSRCPND |= BIT_SUB_ADC;

		rADCCON &= ~(1<<1);				// Disable read_start

		xdata=(rADCDAT0 & 0xfff);		// Read X-position Conversion data
		ydata=(rADCDAT1 & 0xfff);		// Read Y-position Conversion data
		
		ADC_x = ((xdata - 845) * 480) / 2475;
		ADC_y = 272 - (((ydata - 1300) * 272) / 1500);
		
		ADC_x /= GRID;
		ADC_y /= GRID;
		
		// Conversion board coordinate (my board only)
		transXdata = (int)((xdata - 780)  / 5.56);
		transYdata = (int)((ydata - 2550) / -3.86);
		
		// Calibration Source *****************************************************************************************
		/* ( If use calibreateion remove this Comment... )
		if(cal_cnt > 0 && cal_cnt <= 7) {
			calibrate();
		} else if(cal_cnt == 8) {
			for(int i = 0; i < TABLE_X; i++) {
				Uart_Printf(" Table X[%2d] : %3d ", i, cal_x[i]);
				if(i % 3 == 2)
					Uart_Printf("\n");
			}
			
			Uart_Printf("\n");			
			
			for(int i = 0; i < TABLE_Y; i++) {
				Uart_Printf("Table Y[%2d] : %3d ", i, cal_y[i]);
				if(i % 3 == 2)
					Uart_Printf("\n");
			}
			
			cal_cnt += 1;
			
			for(int i = 0; i < 3; i++)
				Uart_Printf("fx[%d] = %d, fy[%d] = %d\n", i, fx[i], i, fy[i]);
				
				
			//Frame Buffer => image
			for(int i = 0; i <480; i++)
			  for(int j = 0; j <= 272; j++)
				 FrameBuffer[(j*240)+i/2] = Image[(j*240)+i/2];
						
			
		} else {
			LCD_FilledRectangle(transXdata - 2, transYdata - 2,transXdata + 2,transYdata + 2, 0x11111111);
			LCD_FilledRectangle(cal_x[ADC_x] - 2, cal_y[ADC_y] - 2, cal_x[ADC_x] + 2, cal_y[ADC_y] + 2, 0xF800F800);
			Uart_Printf("xdata : %d, ydata : %d\n", xdata, ydata);
			Uart_Printf("transXdata : %d, transYdata : %d\n", transXdata, transYdata);
			Uart_Printf("cal_x[%2d] : %3d, cal_y[%2d] : %3d\n", ADC_x, cal_x[ADC_x], ADC_y, cal_y[ADC_y]);
		
		}
		
		// ************************************************************************************************************
		*/
		// (cal_x[ADC_x], cal_y[ADC_y] = transXdata, transYdata) // (�޾ƿ��� ��ǥ�� �ΰ����� ��� �����ϴ�.)
		
		// ** Add LCD touch action here...
		// ���� �÷����� ������ ��
		if(g_onGameState == OGS_PLAYING) {
			if(transXdata >= 335 && transXdata <= 335+46 && transYdata >= 13 && transYdata <= 13+46) { // ������ ��ư
				g_buttonRight = TRUE;
				onGame_moveBlock(MB_RIGHT);
				if(g_testMode) { Uart_Printf("transXdata:%d, transYdata:%d 'TURN_RIGHT' (g_curShape:%d, g_curPhase:%d).\n", transXdata, transYdata, g_curShape, g_curPhase); }
			}
			else if(transXdata >= 408 && transXdata <= 408+46 && transYdata >= 13 && transYdata <= 13+46) { // ���� ��ư
				g_buttonLeft = TRUE;
				onGame_moveBlock(MB_LEFT);
				if(g_testMode) { Uart_Printf("transXdata:%d, transYdata:%d 'TURN_LEFT' (g_curShape:%d, g_curPhase:%d).\n", transXdata, transYdata, g_curShape, g_curPhase); }
			}
			
			else if(transXdata >= 6 && transXdata <= 6+50 && transYdata >= 217 && transYdata <= 217+50) { // 1�� ������
					onGame_Item_UseItem(g_itemSlot[0]);
					//onGame_readItem(g_itemSlot[0]); // ONLY FOR TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
					g_itemSlot[0] = ITEM_NULL;
			}
			else if(transXdata >= 6 && transXdata <= 6+50 && transYdata >= 157 && transYdata <= 157+50) { // 2�� ������
					onGame_Item_UseItem(g_itemSlot[1]);
					//onGame_readItem(g_itemSlot[1]); // ONLY FOR TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
					g_itemSlot[1] = ITEM_NULL;
			}
		}	
		// ���� �޴� ������ ��
		else if(g_gameState == GS_MAIN) { }
		// �ɼ� ������ ��
		else if(g_gameState == GS_OPTION) { }
		// ** LCD touch action end.
		
		rADCTSC = 0x1d3;				// Up detect, Waiting for interrupt
		// 	  1		[ 	  1  	|  	  1	 	| 	  0		|	  1		][      0	 |     0 	  |	  1		  1	 ]
		// 	UD_SEN	[ 	YM_SEN	|	YP_SEN	|	XM_SEN	|	XP_SEN	][   PULL_UP |	AUTO_PST  |		XY_PST	 ]
	}

	// �� �� ����(?)
	else if(rSUBSRCPND & (0x1<<9))		// TC interrupt
	{
		rSUBSRCPND |= BIT_SUB_TC;
		
		// Conversion board coordinate (my board only)
		transXdata = (int)((xdata - 780)  / 5.56);
		transYdata = (int)((ydata - 2550) / -3.86);

		if(rADCUPDN == 0x1)				// Stylus Down
		{
			rADCUPDN &= ~(0x1);			// Clear Down state
			//Uart_Printf("Stylus Down\n");
				
			rADCDLY=(PCLK/1000000)*500;	// 500us delay(input stable)
			rADCTSC = 1<<2;				// Auto conversion mode
			
			rADCCON |= (1<<1);			// Enable read_start
			rADCDAT0;					// Read_start
		}
		else if(rADCUPDN == 0x2)		// Stylus Up
		{
			rADCUPDN &= ~(0x1<<1);		// Clear Up state
			
			//Uart_Printf("Stylus Up\n\n");
			// ���� �÷����� ������ ��
			if(g_onGameState == OGS_PLAYING) {
				if(transXdata >= 335 && transXdata <= 335+46 && transYdata >= 13 && transYdata <= 13+46) { // ������ ��ư
					g_buttonRight = FALSE;
					if(g_testMode) { Uart_Printf("transXdata:%d, transYdata:%d 'BUTTON_UP'.\n", transXdata, transYdata); }
				}
				else if(transXdata >= 408 && transXdata <= 408+46 && transYdata >= 13 && transYdata <= 13+46) { // ���� ��ư
					g_buttonLeft = FALSE;
					if(g_testMode) { Uart_Printf("transXdata:%d, transYdata:%d 'BUTTON_UP'.\n", transXdata, transYdata); }
				}
		}
						
			rADCTSC = 0xd3;    			// Down detect, Waiting for interrupt	
		}
	}
	
	rINTMSK &= ~BIT_ADC;       			//ADC Touch Screen Mask bit clear
	
	rINTSUBMSK &= ~(BIT_SUB_ADC | BIT_SUB_TC);
	rSRCPND = BIT_ADC;
	rINTPND = BIT_ADC;
}