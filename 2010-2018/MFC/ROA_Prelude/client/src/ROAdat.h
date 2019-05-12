#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

#define STAND 0
#define MOVE 1
#define ATTACK 2
#define SPELL 3
#define DEAD 4

#define MELEE 0
#define SWEEP 1
#define SHOCK 2

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
	CString UnitName;
	int UnitX;	int UnitY;
	int UnitDirection;
	int UnitMotion;
	int UnitLv;
	int UnitType;
	int UnitHigh;

	int UnitHpMax;		int UnitMpMax;
	int UnitHp;			int UnitMp;
	int UnitRegenHp;	int UnitRegenMp;
	int UnitEXPMax;		int UnitEXP;
	int UnitAtkType;	int UnitAtkRange;	int UnitAtkScale;
	int UnitDelayAtk;	int UnitDelayMove;
	int UnitBkDelayAtk;	int UnitBkDelayMove;
	int UnitDmgMax;		int UnitDmgMin;
	int UnitDef;
	int UnitResFire;	int UnitResCold;
	int UnitResNature;	int UnitResLight;
	int UnitResDark;
	int UnitRateCri;	int UnitRateEvade;
	
	int UnitStatSTR;	int UnitBonusSTR;
	int UnitStatDEX;	int UnitBonusDEX;
	int UnitStatINT;	int UnitBonusINT;	
	int UnitStatSPR;	int UnitBonusSPR;
	int UnitStatHEL;	int UnitBonusHEL;
	int UnitStatCON;	int UnitBonusCON;

	int UnitRemainStat;


	int UnitRevive;		int UnitBkRevive;
	int UnitAgroRange;

	int UnitState;
	BOOL UnitAbleAtk;
	BOOL UnitAbleMove;

	UnitDat() {}
	UnitDat(int uID, int uX, int uY, int uDirection, int uMotion) {UnitID = uID; UnitX = uX; UnitY = uY; UnitDirection = uDirection; UnitMotion = UnitMotion;}
} Unit;

