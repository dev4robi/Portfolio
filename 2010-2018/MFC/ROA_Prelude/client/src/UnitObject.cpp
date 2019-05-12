#include "stdafx.h"
#include "ROAdat.h"
#include "UnitObject.h"
#include "PlayerObject.h"

#include "MainFrm.h"
#include "ROA PreludeDoc.h"
#include "ROA PreludeView.h"

UnitObject::UnitObject(int uID, int uX, int uY, int uDirection, int uMotion)
{
	Object.UnitName = "루크";

	Object.UnitLv = 1;
	Object.UnitRemainStat = 0;

	Object.UnitHpMax = 100;
	Object.UnitHp = Object.UnitHpMax;
	Object.UnitMpMax = 100;
	Object.UnitMp = Object.UnitMpMax;

	Object.UnitStatSTR	= Object.UnitBonusSTR = 0;
	Object.UnitStatDEX	= Object.UnitBonusDEX = 0;
	Object.UnitStatINT	= Object.UnitBonusINT = 0;	
	Object.UnitStatSPR	= Object.UnitBonusSPR = 0;
	Object.UnitStatHEL	= Object.UnitBonusHEL = 0;
	Object.UnitStatCON	= Object.UnitBonusCON = 0;

	Object.UnitID = uID;
	Object.UnitX = uX;
	Object.UnitY = uY;
	Object.UnitDirection = uDirection;
	Object.UnitMotion = uMotion;

	Object.UnitRevive = 100;
	Object.UnitBkRevive = Object.UnitRevive;

	Object.UnitState = STAND;
	Object.UnitAbleAtk = TRUE;
	Object.UnitAbleMove = TRUE;

	Object.UnitAtkType = MELEE;
	Object.UnitAtkScale = 1;
	Object.UnitAtkRange = 1;
	Object.UnitAgroRange = 2;
	Object.UnitDmgMax = 21;
	Object.UnitDmgMin = 20;

	Object.UnitDelayAtk = 100;
	Object.UnitDelayMove = 100;

	Object.UnitBkDelayAtk = Object.UnitDelayAtk;
	Object.UnitBkDelayMove = Object.UnitDelayMove;
}

UnitObject::~UnitObject()
{

}

void UnitObject::AtkUnit()
{
	if(Object.UnitAbleAtk == TRUE && Object.UnitState == STAND)
	{
		CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CROAPreludeView *pView = (CROAPreludeView*)pMainFrm->GetActiveView();

		switch(Object.UnitDirection)
		{
		case 0:
			Object.UnitMotion = 4;

			// 밀리 타입 DOWN
			if(Object.UnitAtkType == MELEE)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();
				POSITION Atkpos;
				
				Atkpos = NULL;
	
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY+Object.UnitAtkRange && pView->m_UnitList.GetAt(pos).Object.UnitY > Object.UnitY)
					{
						if(Atkpos == NULL)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}
						else if(pView->m_UnitList.GetAt(Atkpos).Object.UnitY > pView->m_UnitList.GetAt(pos).Object.UnitY)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}
					}

					pView->m_UnitList.GetNext(pos);
				}

				if(Atkpos != NULL)
				{
					// 데미지 주는 함수
					pView->m_UnitList.GetAt(Atkpos).GiveDamageUnit();
					if(pView->m_UnitList.GetAt(Atkpos).Object.UnitState == DEAD)
					{
						// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
						Object.UnitEXP += pView->m_UnitList.GetAt(Atkpos).Object.UnitLv*10;
						while(Object.UnitEXP >= Object.UnitEXPMax)
						{
							Object.UnitLv++;
							Object.UnitEXP = 0;
							Object.UnitRemainStat += 5;
							Object.UnitEXPMax = Object.UnitLv*300;
						}
					}
				}
			}

			// 휘두르기 타입 DOWN
			else if(Object.UnitAtkType == SWEEP)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();			
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX+Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY+Object.UnitAtkRange)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}
			
			// 쇼크 타입 DOWN
			else if(Object.UnitAtkType == SHOCK)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();		
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX+Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY+Object.UnitAtkRange-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY+Object.UnitAtkRange+Object.UnitAtkScale)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}

			break;
			
		case 1:
			Object.UnitMotion = 14;

			// 밀리 타입 LEFT
			if(Object.UnitAtkType == MELEE)
			{	
				POSITION pos = pView->m_UnitList.GetHeadPosition();
				POSITION Atkpos;
				
				Atkpos = NULL;
				
				while(pos != NULL)
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY && pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX-Object.UnitAtkRange && pView->m_UnitList.GetAt(pos).Object.UnitX < Object.UnitX)
					{
						if(Atkpos == NULL)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}

						else if(pView->m_UnitList.GetAt(Atkpos).Object.UnitX < pView->m_UnitList.GetAt(pos).Object.UnitX)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
				
				if(Atkpos != NULL)
				{
					// 데미지 주는 함수
					pView->m_UnitList.GetAt(Atkpos).GiveDamageUnit();
					if(pView->m_UnitList.GetAt(Atkpos).Object.UnitState == DEAD)
					{
						// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
						Object.UnitEXP += pView->m_UnitList.GetAt(Atkpos).Object.UnitLv*10;
						while(Object.UnitEXP >= Object.UnitEXPMax)
						{
							Object.UnitLv++;
							Object.UnitEXP = 0;
							Object.UnitRemainStat += 5;
							Object.UnitEXPMax = Object.UnitLv*300;
						}
					}
				}
			}

			// 휘두르기 타입 LEFT
			else if(Object.UnitAtkType == SWEEP)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();			
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX &&
						pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX-Object.UnitAtkRange &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY+Object.UnitAtkScale)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}
			
			// 쇼크 타입 LEFT
			else if(Object.UnitAtkType == SHOCK)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();		
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY+Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX-Object.UnitAtkRange-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX-Object.UnitAtkRange+Object.UnitAtkScale)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}

			break;
			
		case 2:
			Object.UnitMotion = 24;

			// 밀리 타입 UP
			if(Object.UnitAtkType == MELEE)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();
				POSITION Atkpos;
				
				Atkpos = NULL;
				
				while(pos != NULL)
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY-Object.UnitAtkRange && pView->m_UnitList.GetAt(pos).Object.UnitY < Object.UnitY)
					{
						if(Atkpos == NULL)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}

						else if(pView->m_UnitList.GetAt(Atkpos).Object.UnitY < pView->m_UnitList.GetAt(pos).Object.UnitY)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
				
				if(Atkpos != NULL)
				{
					// 데미지 주는 함수
					pView->m_UnitList.GetAt(Atkpos).GiveDamageUnit();
					if(pView->m_UnitList.GetAt(Atkpos).Object.UnitState == DEAD)
					{
						// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
						Object.UnitEXP += pView->m_UnitList.GetAt(Atkpos).Object.UnitLv*10;
						while(Object.UnitEXP >= Object.UnitEXPMax)
						{
							Object.UnitLv++;
							Object.UnitEXP = 0;
							Object.UnitRemainStat += 5;
							Object.UnitEXPMax = Object.UnitLv*300;
						}
					}
				}
			}
			
			// 휘두르기 타입 UP
			else if(Object.UnitAtkType == SWEEP)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();			
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY &&
						pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY-Object.UnitAtkRange &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX+Object.UnitAtkScale)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}
			
			// 쇼크 타입 UP
			else if(Object.UnitAtkType == SHOCK)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();		
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX+Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY-Object.UnitAtkRange-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY-Object.UnitAtkRange+Object.UnitAtkScale)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}
			break;
			
		case 3:
			Object.UnitMotion = 34;

			// 밀리 타입 RIGHT
			if(Object.UnitAtkType == MELEE)
			{			
				POSITION pos = pView->m_UnitList.GetHeadPosition();
				POSITION Atkpos;
				
				Atkpos = NULL;
				
				while(pos != NULL)
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY && pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX+Object.UnitAtkRange && pView->m_UnitList.GetAt(pos).Object.UnitX > Object.UnitX)
					{
						if(Atkpos == NULL)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}

						else if(pView->m_UnitList.GetAt(Atkpos).Object.UnitX > pView->m_UnitList.GetAt(pos).Object.UnitX)
						{
							if(pView->m_UnitList.GetAt(pos).Object.UnitState != DEAD)
								Atkpos = pos;
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
				
				if(Atkpos != NULL)
				{
					// 데미지 주는 함수
					pView->m_UnitList.GetAt(Atkpos).GiveDamageUnit();
					if(pView->m_UnitList.GetAt(Atkpos).Object.UnitState == DEAD)
					{
						// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
						Object.UnitEXP += pView->m_UnitList.GetAt(Atkpos).Object.UnitLv*10;
						while(Object.UnitEXP >= Object.UnitEXPMax)
						{
							Object.UnitLv++;
							Object.UnitEXP = 0;
							Object.UnitRemainStat += 5;
							Object.UnitEXPMax = Object.UnitLv*300;
						}
					}
				}
			}
			
			// 휘두르기 타입 RIGHT
			else if(Object.UnitAtkType == SWEEP)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();			
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX+Object.UnitAtkRange &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY+Object.UnitAtkScale)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}
			
			// 쇼크 타입 LEFT
			else if(Object.UnitAtkType == SHOCK)
			{
				POSITION pos = pView->m_UnitList.GetHeadPosition();		
				
				while(pos != NULL) // 타격받을 유닛 선택
				{
					if(pView->m_UnitList.GetAt(pos).Object.UnitY >= Object.UnitY-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitY <= Object.UnitY+Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX >= Object.UnitX+Object.UnitAtkRange-Object.UnitAtkScale &&
						pView->m_UnitList.GetAt(pos).Object.UnitX <= Object.UnitX+Object.UnitAtkRange+Object.UnitAtkScale)
					{
						// 데미지 주는 함수
						if(pView->m_UnitList.GetAt(pos).Object.UnitX == Object.UnitX && pView->m_UnitList.GetAt(pos).Object.UnitY == Object.UnitY)
						{
							pView->m_UnitList.GetAt(pos).GiveDamageUnit();
							if(pView->m_UnitList.GetAt(pos).Object.UnitState == DEAD)
							{
								// 어택 유닛이 죽엇을 경우 경험치 얻는 함수
								Object.UnitEXP += pView->m_UnitList.GetAt(pos).Object.UnitLv*10;
								while(Object.UnitEXP >= Object.UnitEXPMax)
								{
									Object.UnitLv++;
									Object.UnitEXP = 0;
									Object.UnitRemainStat += 5;
									Object.UnitEXPMax = Object.UnitLv*300;
								}
							}
						}
					}
					
					pView->m_UnitList.GetNext(pos);
				}
			}
			break;
		}
		
		Object.UnitState = ATTACK;
	}
}

BOOL UnitObject::MoveUnit()
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CROAPreludeView *pView = (CROAPreludeView*)pMainFrm->GetActiveView();

	if(Object.UnitAbleMove == TRUE && Object.UnitState == STAND)
	{
		switch(Object.UnitDirection)
		{
			case 0: // DOWN
				if(pView->m_UnitMap[Object.UnitX+(Object.UnitY+1)*pView->m_MapWidth] == FALSE)
				{
					if(pView->m_TrinDat[Object.UnitX+(Object.UnitY+1)*pView->m_MapWidth].TrinMove == TRUE)
					{
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = FALSE;
						Object.UnitY++;
						Object.UnitMotion = 1;
						Object.UnitState = MOVE;
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = TRUE;
					}
					else
						return FALSE;
				}
				else
					return FALSE;
				break;

			case 1: // LEFT
				if(pView->m_UnitMap[Object.UnitX-1+(Object.UnitY)*pView->m_MapWidth] == FALSE)
				{
					if(pView->m_TrinDat[Object.UnitX-1+(Object.UnitY)*pView->m_MapWidth].TrinMove == TRUE)
					{
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = FALSE;
						Object.UnitX--;
						Object.UnitMotion = 11;
						Object.UnitState = MOVE;
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = TRUE;
					}
					else
						return FALSE;
				}
				else
					return FALSE;
				break;
				
			case 2: // UP
				if(pView->m_UnitMap[Object.UnitX+(Object.UnitY-1)*pView->m_MapWidth] == FALSE)
				{
					if(pView->m_TrinDat[Object.UnitX+(Object.UnitY-1)*pView->m_MapWidth].TrinMove == TRUE)
					{
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = FALSE;
						Object.UnitY--;
						Object.UnitMotion = 21;
						Object.UnitState = MOVE;
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = TRUE;
					}
					else
						return FALSE;
				}
				else
					return FALSE;
				break;
				
			case 3: // RIGHT
				if(pView->m_UnitMap[Object.UnitX+1+(Object.UnitY)*pView->m_MapWidth] == FALSE)
				{
					if(pView->m_TrinDat[Object.UnitX+1+(Object.UnitY)*pView->m_MapWidth].TrinMove == TRUE)
					{
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = FALSE;
						Object.UnitX++;
						Object.UnitMotion = 31;
						Object.UnitState = MOVE;
						pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = TRUE;
					}
					else
						return FALSE;
				}
				else
					return FALSE;
				break;			
		}
	}
	return TRUE;
}

void UnitObject::GiveDamageUnit()
{
	// 데미지 주는 수식
	if(Object.UnitState != DEAD)
		Object.UnitHp -= 10;

	// 죽음 여부 확인
	if(Object.UnitHp <= 0 && Object.UnitState != DEAD)
		DeathUnit();
}

void UnitObject::TeleportUnit(int x, int y, int Direction)
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CROAPreludeView *pView = (CROAPreludeView*)pMainFrm->GetActiveView();
	if(x < pView->m_MapWidth && x >=0 && y >= 0 && y < pView->m_MapHeight)
	{
		pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = FALSE;
		Object.UnitX = x;
		Object.UnitY = y;
		Object.UnitDirection = Direction;
		pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = TRUE;
	}
}

void UnitObject::DeathUnit()
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CROAPreludeView *pView = (CROAPreludeView*)pMainFrm->GetActiveView();

	Object.UnitState = DEAD;
	Object.UnitAbleAtk = FALSE;
	Object.UnitAbleMove = FALSE;
	Object.UnitMotion = 40;
	Object.UnitHp = 0;
	Object.UnitMp = 0;
	Object.UnitEXP = ((double)Object.UnitEXP-(double)Object.UnitEXPMax*0.1 < 0)?0:(double)Object.UnitEXP-(double)Object.UnitEXPMax*0.1;

	pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = FALSE;
}

void UnitObject::ReviveUnit()
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CROAPreludeView *pView = (CROAPreludeView*)pMainFrm->GetActiveView();

	if(Object.UnitState == DEAD)
	{
		if(Object.UnitRevive >= 0)
			Object.UnitRevive--;
		else
		{
			Object.UnitRevive = Object.UnitBkRevive;
			Object.UnitHp = Object.UnitHpMax;
			Object.UnitMp = Object.UnitMpMax;
			Object.UnitState = STAND;
			Object.UnitAbleAtk = TRUE;
			Object.UnitAbleMove = TRUE;
			Object.UnitMotion = Object.UnitDirection*10;

			pView->m_UnitMap[Object.UnitX+Object.UnitY*pView->m_MapWidth] = TRUE;
		}
	}
}