#define DOWN 0x00
#define LEFT 0x01
#define UP 0x02
#define RIGHT 0x03

#define STAND 0x00
#define MOVE 0x01
#define ATTACK 0x02
#define SPELL 0x03
#define DEAD 0x04

#define MELEE 0x00
#define SWEEP 0x01
#define SHOCK 0x02

#define BLUE 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define PINK 4

// *구역(Location) 자료형 정희
typedef struct Locate
{
	int LocationX1, LocationY1;
	int LocationX2, LocationY2;
	CString LocationName;
	int LocationColor;
	int LocationID;

	BOOL LocationSelected;

	Locate() {}
	Locate(int LocX1, int LocY1, int LocX2, int LocY2, CString LocName, int LocID, int LocColor)
		  {LocationX1 = LocX1; LocationY1 = LocY1; LocationX2 = LocX2; LocationY2 = LocY2; LocationName = LocName;
	       LocationID = LocID, LocationColor = LocColor;}
} Location;

// *지형(Terrain) 자료형 정의
typedef struct TerrainDat
{
	int TrinID;
	int TrinNextID;
	int TrinHigh;
	BOOL TrinMove;
} Terrain;

// *유닛(Unit) 자료형 정의

typedef struct UnitDat
{
	int UnitID;
	char UnitName[20];
	int UnitX;	int UnitY;
	int UnitDirection;
	int UnitMotion;
	int UnitLv;
	int UnitType;
	int UnitHigh;

	int UnitHpMax;		int UnitMpMax;
	int UnitHp;			int UnitMp;
	int UnitEXPMax;		int UnitEXP;
	int UnitAtkType;	int UnitAtkRange;	int UnitAtkScale;
	int UnitSpeedAtk;	int UnitSpeedMove;
	int UnitDelayAtk;	int UnitDelayMove;
	int UnitDmgMax;		int UnitDmgMin;
	int UnitDef;
	int UnitResFire;	int UnitResCold;
	int UnitResNature;	int UnitResLight;
	int UnitResDark;
	int UnitRateCri;	int UnitRateEvade;

	int UnitState;
	BOOL UnitAbleAtk;
	BOOL UnitAbleMove;

	UnitDat() {}
	UnitDat(int uID, int uX, int uY, int uDirection, int uMotion) {UnitID = uID; UnitX = uX; UnitY = uY; UnitDirection = uDirection; UnitMotion = UnitMotion;}
} Unit;

