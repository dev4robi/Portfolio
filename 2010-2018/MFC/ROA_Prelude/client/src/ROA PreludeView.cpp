// ROA PreludeView.cpp : implementation of the CROAPreludeView class
//

#include "stdafx.h"
#include "ROA Prelude.h"
#include "ROAdat.h"
#include "UnitObject.h"
#include "PlayerObject.h"

#include "ROA PreludeDoc.h"
#include "ROA PreludeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeView

IMPLEMENT_DYNCREATE(CROAPreludeView, CView)

BEGIN_MESSAGE_MAP(CROAPreludeView, CView)
	//{{AFX_MSG_MAP(CROAPreludeView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeView construction/destruction

CROAPreludeView::CROAPreludeView()
{
	// TODO: add construction code here
	//m_CamLockHero = FALSE;
	m_CamLockHero = TRUE;
}

CROAPreludeView::~CROAPreludeView()
{
}

void CROAPreludeView::DrawScreenMem(int RgnX, int RgnY, int PixX, int PixY)
{
	RgnX += PixX/48;
	RgnY += PixY/48;
	
	int MinX, MinY, MaxX, MaxY;
	
	if(RgnX-8 > 0)
	{
		if(RgnX+10 < m_MapWidth)
		{
			MinX = RgnX-14;
			MaxX = RgnX+14;
		}

		else
		{
			MinX = m_MapWidth-24;
			MaxX = m_MapWidth+5;
			PixX = 0;
		}
	}

	else
	{
		MinX = -5;
		MaxX = 29;
		PixX = 0;
	}
	
	if(RgnY-6 > 0)
	{
		if(RgnY+6 < m_MapHeight)
		{
			MinY = RgnY-12;
			MaxY = RgnY+12;
		}
		
		else
		{
			MinY = m_MapHeight-18;
			MaxY = m_MapHeight+7;
			PixY = 0;
		}
	}

	else
	{
		MinY = -5;
		MaxY = 25;
		PixY = 0;
	}
	
	int x=0, y=0;

	// 지형
	for(int i=MinY; i<MaxY; i++)
	{
		for(int j=MinX; j<MaxX; j++)
		{
			if((i*m_MapWidth+j) >= 0 && (i*m_MapWidth+j) < (m_MapWidth*m_MapHeight))
			{
				memDC.BitBlt(x*48-PixX%48, y*48-PixY%48, 48, 48, &TrinDC, (m_TrinDat[i*m_MapWidth+j].TrinID)%16*48,
					         (m_TrinDat[i*m_MapWidth+j].TrinID)/16*48, SRCCOPY);
			}
			
			x++;
		}
		y++;
		x = 0;
	}

	x = 0; y = 0;

	// 유닛
	POSITION pos = m_UnitList.GetHeadPosition();
	CList <POSITION, POSITION&> UnitList; // 모든 유닛을 담아두는 리스트.
	
	while(pos != NULL)
	{
		if(m_UnitList.GetAt(pos).Object.UnitY < MaxY && m_UnitList.GetAt(pos).Object.UnitY >= MinY && m_UnitList.GetAt(pos).Object.UnitX < MaxX && m_UnitList.GetAt(pos).Object.UnitX >= MinX)
		{
			if(UnitList.IsEmpty())
				UnitList.AddHead(pos);
			else
			{
				POSITION post = UnitList.GetHeadPosition();
				while(post != NULL)
				{
					if(m_UnitList.GetAt(pos).Object.UnitState != DEAD)
					{			
						if(m_UnitList.GetAt(UnitList.GetAt(post)).Object.UnitY > m_UnitList.GetAt(pos).Object.UnitY && m_UnitList.GetAt(UnitList.GetAt(post)).Object.UnitState != DEAD)
						{
							UnitList.InsertBefore(post,pos);
							post = UnitList.GetTailPosition();
						}

						else if(post == UnitList.GetTailPosition())
						{
							UnitList.AddTail(pos);
							post = UnitList.GetTailPosition();
						}
					}
					else
					{
						UnitList.AddHead(pos);
						post = UnitList.GetTailPosition();
					}
					UnitList.GetNext(post);
				}
			}
		}
		m_UnitList.GetNext(pos);
	}

	pos = UnitList.GetHeadPosition();

	while(pos != NULL)
	{
		// 상태에 따른 유닛 모션 출력
		//
		//
		UnitDC.SelectObject(&UnitBitmap[m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitID]);
		UnitBitmap[m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitID].GetBitmap(&bmpinfo);

		// 최종적으로 그려줌(STAND)
		if(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitState == STAND)
		{
			TransDraw(&memDC,((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitX-MinX)*48-PixX%48)-(((bmpinfo.bmWidth/10)-48)/2), ((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitY-MinY)*48-24-PixY%48)-((bmpinfo.bmHeight/5)-72), bmpinfo.bmWidth/10, bmpinfo.bmHeight/5, &UnitDC, (bmpinfo.bmWidth/10)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion%10),
				(bmpinfo.bmHeight/5)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion/10),RGB(255,0,255));
		}
		
		// 최종적으로 그려줌(MOVE)
		else if(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitState == MOVE)
		{
			if(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitDirection % 2 == 0 && m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitState == MOVE) // 위 아래
				TransDraw(&memDC,((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitX-MinX)*48-PixX%48)-(((bmpinfo.bmWidth/10)-48)/2), ((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitY-MinY)*48-24-PixY%48)-((bmpinfo.bmHeight/5)-72)+((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitDirection -1)*(48-(((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion)%10)*12))),
				bmpinfo.bmWidth/10, bmpinfo.bmHeight/5, &UnitDC, (bmpinfo.bmWidth/10)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion%10), (bmpinfo.bmHeight/5)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion/10), RGB(255,0,255));
			
			else if(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitState == MOVE) // 좌 우
				TransDraw(&memDC,((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitX-MinX)*48-PixX%48)-(((bmpinfo.bmWidth/10)-48)/2)-((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitDirection -2)*(48-(((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion)%10)*12))), ((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitY-MinY)*48-24-PixY%48)-((bmpinfo.bmHeight/5)-72),
				bmpinfo.bmWidth/10, bmpinfo.bmHeight/5, &UnitDC, (bmpinfo.bmWidth/10)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion%10), (bmpinfo.bmHeight/5)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion/10), RGB(255,0,255));
			
			if(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion%10 == 0)
				TransDraw(&memDC,((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitX-MinX)*48-PixX%48)-(((bmpinfo.bmWidth/10)-48)/2), ((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitY-MinY)*48-24-PixY%48)-((bmpinfo.bmHeight/5)-72), bmpinfo.bmWidth/10, bmpinfo.bmHeight/5, &UnitDC, (bmpinfo.bmWidth/10)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion%10),
				(bmpinfo.bmHeight/5)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion/10), RGB(255,0,255));
		}
		
		// 최종적으로 그려줌(ATTACK)
		else if(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitState == ATTACK)
		{
			TransDraw(&memDC,((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitX-MinX)*48-PixX%48)-(((bmpinfo.bmWidth/10)-48)/2), ((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitY-MinY)*48-24-PixY%48)-((bmpinfo.bmHeight/5)-72), bmpinfo.bmWidth/10, bmpinfo.bmHeight/5, &UnitDC, (bmpinfo.bmWidth/10)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion%10),
				(bmpinfo.bmHeight/5)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion/10), RGB(255,0,255));
		}
		
		// 최종적으로 그려줌(DEAD)
		else if(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitState == DEAD)
		{
			TransDraw(&memDC,((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitX-MinX)*48-PixX%48)-(((bmpinfo.bmWidth/10)-48)/2), ((m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitY-MinY)*48-24-PixY%48)-((bmpinfo.bmHeight/5)-72), bmpinfo.bmWidth/10, bmpinfo.bmHeight/5, &UnitDC, (bmpinfo.bmWidth/10)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion%10),
				(bmpinfo.bmHeight/5)*(m_UnitList.GetAt(UnitList.GetAt(pos)).Object.UnitMotion/10), RGB(255,0,255));
		}

		UnitList.GetNext(pos);
	}

	UnitList.RemoveAll();

	// 유저 인터페이스
	TransDraw(&memDC, 240, 840, 912, 120, &InterfaceDC, 0, 0, RGB(255, 0, 255));
	TransDraw(&memDC, 240+273, 840+58, m_UnitList.GetHead().Object.UnitHp*268/m_UnitList.GetHead().Object.UnitHpMax, 12, &BarDC, 0, 28, RGB(255, 0, 255)); // HP 268Px/ 12
	TransDraw(&memDC, 240+273, 840+91, m_UnitList.GetHead().Object.UnitMp*268/m_UnitList.GetHead().Object.UnitMpMax, 12, &BarDC, 0, 15, RGB(255, 0, 255)); // MP 268Px / 12
	TransDraw(&memDC, 240+378, 840+21, m_UnitList.GetHead().Object.UnitEXP*268/m_UnitList.GetHead().Object.UnitEXPMax, 12, &BarDC, 0, 2, RGB(255, 0, 255)); // EXP 268Px / 12
	
	CString HP, MP, EXP, LV;
	HP.Format("[%d/%d] %d%%", m_UnitList.GetHead().Object.UnitHp, m_UnitList.GetHead().Object.UnitHpMax, m_UnitList.GetHead().Object.UnitHp*100/m_UnitList.GetHead().Object.UnitHpMax);
	MP.Format("[%d/%d] %d%%", m_UnitList.GetHead().Object.UnitMp, m_UnitList.GetHead().Object.UnitMpMax, m_UnitList.GetHead().Object.UnitMp*100/m_UnitList.GetHead().Object.UnitMpMax);
	EXP.Format("[%d/%d] %.2f%%", m_UnitList.GetHead().Object.UnitEXP, m_UnitList.GetHead().Object.UnitEXPMax, (float)((float)m_UnitList.GetHead().Object.UnitEXP*100/(float)m_UnitList.GetHead().Object.UnitEXPMax));
	LV.Format("%d", m_UnitList.GetHead().Object.UnitLv);

	CFont font; font.CreatePointFont(90, "Arial Black"); memDC.SelectObject(&font);
	COLORREF OldTextColor = memDC.GetTextColor();

	memDC.SetTextColor(RGB(255, 255, 255));
	memDC.SetBkMode(TRANSPARENT);

	memDC.DrawText(HP, CRect(240+549, 840+54, 240+715, 840+71), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	memDC.DrawText(MP, CRect(240+549, 840+86, 240+715, 840+103), DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	font.DeleteObject(); font.CreatePointFont(150, "Arial Black"); memDC.SelectObject(&font);
	memDC.DrawText(LV, CRect(242+268, 838+12, 242+388+268, 838+21+22), DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	memDC.SetTextColor(RGB(0, 0, 0));
	font.DeleteObject(); font.CreatePointFont(90, "Arial Black"); memDC.SelectObject(&font);
	memDC.DrawText(EXP, CRect(242+378, 838+22, 242+378+268, 838+21+12), DT_SINGLELINE | DT_CENTER | DT_VCENTER);


	// 캐릭터창 그리기
	if(HostPlayer.CharWndOn == TRUE)
	{
		CClientDC dc(this);
		CDC PortraitDC;
		CBitmap PortraitBitmap;

		PortraitBitmap.CreateCompatibleBitmap(&dc, 3*48, 3*48);
		PortraitDC.CreateCompatibleDC(&dc);
		PortraitDC.SelectObject(&PortraitBitmap);

		// 캐릭터 어디잇는지 찾아야되
		PortraitDC.BitBlt(0, 0, 3*48, 3*48, &memDC, 13*48, 10*48, SRCCOPY);

		CString Name, Atk, PDef, MDef, AtkS, MoveS, HitRate, EveRate, CriRate, STR, DEX, sINT, VIT, CON, SPR, RemainStat, FireRes, ColdRes, NatureRes, LightRes, DarkRes;

		Atk.Format("%d~%d", m_UnitList.GetHead().Object.UnitDmgMin, m_UnitList.GetHead().Object.UnitDmgMax);
		PDef.Format("%d", m_UnitList.GetHead().Object.UnitDef);
		MDef.Format("%d", m_UnitList.GetHead().Object.UnitDef);
		AtkS.Format("%d", m_UnitList.GetHead().Object.UnitBkDelayAtk);
		MoveS.Format("%d", m_UnitList.GetHead().Object.UnitBkDelayMove);
		HitRate.Format("%d", 0); // 바꿔임마
		EveRate.Format("%d", m_UnitList.GetHead().Object.UnitRateEvade);
		CriRate.Format("%d", m_UnitList.GetHead().Object.UnitRateCri);
		STR.Format("%d", m_UnitList.GetHead().Object.UnitStatSTR);
		DEX.Format("%d", m_UnitList.GetHead().Object.UnitStatDEX);
		sINT.Format("%d", m_UnitList.GetHead().Object.UnitStatINT);
		VIT.Format("%d", m_UnitList.GetHead().Object.UnitStatHEL);
		CON.Format("%d", m_UnitList.GetHead().Object.UnitStatCON);
		SPR.Format("%d", m_UnitList.GetHead().Object.UnitStatSPR);
		RemainStat.Format("%d", m_UnitList.GetHead().Object.UnitRemainStat);
		FireRes.Format("%d", m_UnitList.GetHead().Object.UnitResFire);
		ColdRes.Format("%d", m_UnitList.GetHead().Object.UnitResCold);
		NatureRes.Format("%d", m_UnitList.GetHead().Object.UnitResNature);
		LightRes.Format("%d", m_UnitList.GetHead().Object.UnitResLight);
		DarkRes.Format("%d", m_UnitList.GetHead().Object.UnitResDark);
		Name.Format("%s", m_UnitList.GetHead().Object.UnitName);
		
		memDC.BitBlt(240+4*48, 240+12, 528, 576, &CharWndDC, 0, 0, SRCCOPY);
		memDC.BitBlt(240+4*48+35, 240+12+35, 3*48+35, 3*48+35, &PortraitDC, 0, 0, SRCCOPY);
		memDC.SetTextColor(RGB(255, 255, 255));

		memDC.DrawText(Name, CRect(240+4*48+302, 240+12+31, 240+4*48+508, 240+12+45), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(LV, CRect(240+4*48+302, 240+12+61, 240+4*48+508, 240+12+75), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(EXP, CRect(240+4*48+318, 240+12+91, 240+4*48+508, 240+12+105), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(HP, CRect(240+4*48+318, 240+12+121, 240+4*48+508, 240+12+135), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(MP, CRect(240+4*48+302, 240+12+151, 240+4*48+508, 240+12+165), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(Atk, CRect(240+4*48+122, 240+12+218, 240+4*48+300, 240+12+232), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(PDef, CRect(240+4*48+160, 240+12+248, 240+4*48+294, 240+12+262), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(MDef, CRect(240+4*48+160, 240+12+278, 240+4*48+294, 240+12+292), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(AtkS, CRect(240+4*48+144, 240+12+308, 240+4*48+294, 240+12+322), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(MoveS, CRect(240+4*48+144, 240+12+338, 240+4*48+294, 240+12+352), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(HitRate, CRect(240+4*48+122, 240+12+368, 240+4*48+294, 240+12+382), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(EveRate, CRect(240+4*48+122, 240+12+398, 240+4*48+294, 240+12+412), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(CriRate, CRect(240+4*48+138, 240+12+428, 240+4*48+294, 240+12+442), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(STR, CRect(240+4*48+354, 240+12+218, 240+4*48+484, 240+12+232), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(DEX, CRect(240+4*48+364, 240+12+248, 240+4*48+484, 240+12+262), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(sINT, CRect(240+4*48+364, 240+12+278, 240+4*48+484, 240+12+292), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(VIT, CRect(240+4*48+364, 240+12+308, 240+4*48+484, 240+12+322), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(SPR, CRect(240+4*48+364, 240+12+338, 240+4*48+484, 240+12+352), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(CON, CRect(240+4*48+364, 240+12+368, 240+4*48+484, 240+12+382), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(RemainStat, CRect(240+4*48+418, 240+12+428, 240+4*48+508, 240+12+443), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(FireRes, CRect(240+4*48+117, 240+12+489, 240+4*48+181, 240+12+503), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(ColdRes, CRect(240+4*48+251, 240+12+489, 240+4*48+315, 240+12+503), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(NatureRes, CRect(240+4*48+385, 240+12+489, 240+4*48+508, 240+12+503), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(LightRes, CRect(240+4*48+108, 240+12+520, 240+4*48+181, 240+12+534), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		memDC.DrawText(DarkRes, CRect(240+4*48+251, 240+12+520, 240+4*48+315, 240+12+534), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		
		if(m_UnitList.GetHead().Object.UnitRemainStat > 0)
		{
			CDC ButtonDC;
			CBitmap ButtonBitmap;
			
			ButtonBitmap.LoadBitmap(IDB_BUTTON1);
			ButtonDC.CreateCompatibleDC(&dc);
			ButtonDC.SelectObject(&ButtonBitmap);
			
			memDC.BitBlt(240+4*48+485, 240+12+217, 16, 16, &ButtonDC, 16, 0, SRCCOPY);
			memDC.BitBlt(240+4*48+485, 240+12+247, 16, 16, &ButtonDC, 16, 0, SRCCOPY);
			memDC.BitBlt(240+4*48+485, 240+12+277, 16, 16, &ButtonDC, 16, 0, SRCCOPY);
			memDC.BitBlt(240+4*48+485, 240+12+307, 16, 16, &ButtonDC, 16, 0, SRCCOPY);
			memDC.BitBlt(240+4*48+485, 240+12+337, 16, 16, &ButtonDC, 16, 0, SRCCOPY);
			memDC.BitBlt(240+4*48+485, 240+12+367, 16, 16, &ButtonDC, 16, 0, SRCCOPY);
		}
	}
	
	memDC.SetTextColor(OldTextColor);
}

void CROAPreludeView::DrawMemBlt()
{
	CClientDC dc(this);

	dc.BitBlt(0, 0, 19*48, 15*48, &memDC, 48*5, 48*5, SRCCOPY);
	
	Invalidate(FALSE);
}

BOOL CROAPreludeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// 윈도우 뷰 초기화
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, LoadCursor(NULL, IDC_ARROW),
				   (HBRUSH)GetStockObject(BLACK_BRUSH), LoadIcon(NULL, IDI_APPLICATION));

	// 맵 데이터 가져오기
	FILE *MapData;
	MapData = fopen("Map000.roa", "r");

	if(MapData == NULL)
	{
		AfxMessageBox("맵을 불러올 수 없습니다.");
		fclose(MapData);
	}

	else
	{
		// 지형 사이즈
		fscanf(MapData, "%d %d ", &m_MapWidth, &m_MapHeight);

		m_TrinDat.SetSize(m_MapWidth*m_MapHeight);
		m_UnitMap.SetSize(m_MapWidth*m_MapHeight);
		
		// 지형 데이터
		for(int i=0; i<m_MapWidth*m_MapHeight; i++)
			fscanf(MapData, "%d %d %d %d ", &m_TrinDat[i].TrinID, &m_TrinDat[i].TrinNextID, &m_TrinDat[i].TrinHigh,&m_TrinDat[i].TrinMove);
		
		// *구역 사이즈
		int LocSize;
		fscanf(MapData, "%d ", &LocSize);
		m_LocationDat.SetSize(LocSize);
		

		// *구역 데이터
		for(i=0; i<LocSize; i++)
		{
			char LocName[20];
			fscanf(MapData, "%d %s %d %d %d %d %d ", &(m_LocationDat[i].LocationID), LocName, 
				&(m_LocationDat[i].LocationColor), &(m_LocationDat[i].LocationX1), &(m_LocationDat[i].LocationY1), 
				&(m_LocationDat[i].LocationX2), &(m_LocationDat[i].LocationY2));
			m_LocationDat[i].LocationName = LocName;
		}

		AfxMessageBox("맵을 정상적으로 불러왔습니다.");
		fclose(MapData);
	}

	// 몬스터 이미지 로드
	UnitBitmap.SetSize(8);
	UnitBitmap[0].LoadBitmap(134);
	UnitBitmap[1].LoadBitmap(135);
	UnitBitmap[2].LoadBitmap(136);
	UnitBitmap[3].LoadBitmap(137);
	UnitBitmap[4].LoadBitmap(138);
	UnitBitmap[5].LoadBitmap(139);
	UnitBitmap[6].LoadBitmap(140);
	UnitBitmap[7].LoadBitmap(141);
	// 몬스터 이미지 로드

	// only for test
	m_UnitList.AddTail(UnitObject(0, 18, 21, 0, 0));
	m_UnitList.GetHead().Object.UnitLv = 1;
	m_UnitList.GetHead().Object.UnitRemainStat = 0;
	m_UnitList.GetHead().Object.UnitState = STAND;
	m_UnitList.GetHead().Object.UnitAtkRange = 1;
	m_UnitList.GetHead().Object.UnitAtkScale = 1;
	m_UnitList.GetHead().Object.UnitDelayAtk = 1;
	m_UnitList.GetHead().Object.UnitDelayMove = 1;
	m_UnitList.GetHead().Object.UnitBkDelayAtk = m_UnitList.GetHead().Object.UnitDelayAtk;
	m_UnitList.GetHead().Object.UnitBkDelayMove = m_UnitList.GetHead().Object.UnitDelayMove;
	m_UnitList.GetHead().Object.UnitHpMax = 1000;
	m_UnitList.GetHead().Object.UnitHp = m_UnitList.GetHead().Object.UnitHpMax;
	m_UnitList.GetHead().Object.UnitMpMax = 1000;
	m_UnitList.GetHead().Object.UnitMp = m_UnitList.GetHead().Object.UnitMpMax;
	m_UnitList.GetHead().Object.UnitEXPMax = 300;
	m_UnitList.GetHead().Object.UnitEXP = 0;
	m_UnitList.GetHead().Object.UnitAtkType = MELEE;
	m_UnitList.GetHead().Object.UnitRevive = -1;
	m_UnitList.GetHead().Object.UnitBkRevive = m_UnitList.GetTail().Object.UnitRevive;

	m_UnitMap[18+21*m_MapWidth] = TRUE;

	//m_UnitList.AddTail(UnitObject(0, 15, 15, 0, 30));
	//m_UnitList.AddTail(UnitObject(0, 16, 15, 0, 10));
	//m_UnitList.AddTail(UnitObject(0, 15, 17, 0, 0));
	//m_UnitList.AddTail(UnitObject(0, 15, 19, 0, 20));

	srand(GetTickCount());
	for(int j=0; j<100; j++)
	{
		int test = rand()%4,testx = rand()%(m_MapWidth-2)+1,testy = rand()%(m_MapHeight-2)+1;

		if(m_TrinDat[testx+(testy*m_MapWidth)].TrinMove == TRUE)
		{
			if(m_UnitMap[testx+testy*m_MapWidth] == FALSE)
			{
				m_UnitList.AddTail(UnitObject(rand()%7+1, testx, testy, test, 0));
				m_UnitList.GetTail().Object.UnitLv = (rand()%8+1)*(rand()%4+1);
				m_UnitList.GetTail().Object.UnitHp = rand()%4+1;
				m_UnitList.GetTail().Object.UnitHpMax = m_UnitList.GetTail().Object.UnitHp;
				m_UnitList.GetTail().Object.UnitState = STAND;
				m_UnitList.GetTail().Object.UnitDelayAtk = rand()%10+1;
				m_UnitList.GetTail().Object.UnitDelayMove = rand()%10+10;
				m_UnitList.GetTail().Object.UnitRevive = rand()%50+50;
				m_UnitList.GetTail().Object.UnitBkRevive = m_UnitList.GetTail().Object.UnitRevive;
				m_UnitList.GetTail().Object.UnitBkDelayAtk = m_UnitList.GetTail().Object.UnitDelayAtk;
				m_UnitList.GetTail().Object.UnitBkDelayMove = m_UnitList.GetTail().Object.UnitDelayMove;
				m_UnitList.GetTail().Object.UnitAtkRange = 1;
				m_UnitList.GetTail().Object.UnitAtkScale = 1;
				m_UnitList.GetTail().Object.UnitAgroRange = rand()%5+1;
				m_UnitList.GetTail().Object.UnitAtkType = MELEE;

				m_UnitMap[testx+testy*m_MapWidth] = TRUE;
			}
		}
	}

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeView drawing

void CROAPreludeView::OnDraw(CDC* pDC)
{
	CROAPreludeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeView diagnostics

#ifdef _DEBUG
void CROAPreludeView::AssertValid() const
{
	CView::AssertValid();
}

void CROAPreludeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CROAPreludeDoc* CROAPreludeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CROAPreludeDoc)));
	return (CROAPreludeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeView message handlers

int CROAPreludeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CClientDC dc(this);

	// 이미지 불러오기
	TrinBitmap.LoadBitmap(IDB_TRINSET);
	InterfaceBitmap.LoadBitmap(IDB_INTERFACE0);
	BarBitmap.LoadBitmap(IDB_BAR);
	CharWndBitmap.LoadBitmap(IDB_CHARWND);
	
	TrinDC.CreateCompatibleDC(&dc);
	TrinDC.SelectObject(&TrinBitmap);
	UnitDC.CreateCompatibleDC(&dc);
	InterfaceDC.CreateCompatibleDC(&dc);
	InterfaceDC.SelectObject(&InterfaceBitmap);
	BarDC.CreateCompatibleDC(&dc);
	BarDC.SelectObject(&BarBitmap);
	CharWndDC.CreateCompatibleDC(&dc);
	CharWndDC.SelectObject(&CharWndBitmap);

	memBitmap.CreateCompatibleBitmap(&dc, 29*48, 25*48);
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&memBitmap);

	// 타이머 생성
	SetTimer(1, 10, NULL);
	SetTimer(2, 100, NULL); // only for test
	
	return 0;
}

void CROAPreludeView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
	// 이미지 해제
	TrinBitmap.Detach();
	//UnitBitmap.Detach();
	
	TrinDC.DeleteDC();
	memBitmap.Detach();
	memDC.DeleteDC();

	// 타이머 파괴
	KillTimer(1);
	KillTimer(2); // only for test
	
}

void CROAPreludeView::OnTimer(UINT nIDEvent) 
{
	POSITION pos;

	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		case 1: // 0.01sec
			AnimationDelay();
			
			if(m_CamLockHero == TRUE)
			{
				DrawScreenMem(0, 0, HostPlayer.CamX, HostPlayer.CamY+48);
			}			
			else
			{
				DrawScreenMem(0, 0, 16, 16);
			}

			DrawMemBlt();
			break;

		case 2: // 1sec only for test
			pos = m_UnitList.GetHeadPosition();
			
			m_UnitList.GetNext(pos);
			if(m_UnitList.GetHead().Object.UnitState != DEAD)
			{
				while(pos != NULL)
				{
					if(m_UnitList.GetAt(pos).Object.UnitState == STAND && m_UnitList.GetAt(pos).Object.UnitState != DEAD)
					{
						if(((m_UnitList.GetAt(pos).Object.UnitX == m_UnitList.GetHead().Object.UnitX) && (m_UnitList.GetAt(pos).Object.UnitY-1 == m_UnitList.GetHead().Object.UnitY)) ||
						   ((m_UnitList.GetAt(pos).Object.UnitX == m_UnitList.GetHead().Object.UnitX) && (m_UnitList.GetAt(pos).Object.UnitY+1 == m_UnitList.GetHead().Object.UnitY)) ||
						   ((m_UnitList.GetAt(pos).Object.UnitX-1 == m_UnitList.GetHead().Object.UnitX) && (m_UnitList.GetAt(pos).Object.UnitY == m_UnitList.GetHead().Object.UnitY)) ||
						   ((m_UnitList.GetAt(pos).Object.UnitX+1 == m_UnitList.GetHead().Object.UnitX) && (m_UnitList.GetAt(pos).Object.UnitY == m_UnitList.GetHead().Object.UnitY)))
						{
							if(m_UnitList.GetHead().Object.UnitX == m_UnitList.GetAt(pos).Object.UnitX+1)
							{
								m_UnitList.GetAt(pos).Object.UnitDirection = 3;
								m_UnitList.GetAt(pos).AtkUnit();
							}
							
							else if(m_UnitList.GetHead().Object.UnitX == m_UnitList.GetAt(pos).Object.UnitX-1)
							{
								m_UnitList.GetAt(pos).Object.UnitDirection = 1;
								m_UnitList.GetAt(pos).AtkUnit();
							}
							
							else if(m_UnitList.GetHead().Object.UnitY == m_UnitList.GetAt(pos).Object.UnitY+1)
							{
								m_UnitList.GetAt(pos).Object.UnitDirection = 0;
								m_UnitList.GetAt(pos).AtkUnit();
							}
							
							else if(m_UnitList.GetHead().Object.UnitY == m_UnitList.GetAt(pos).Object.UnitY-1)
							{
								m_UnitList.GetAt(pos).Object.UnitDirection = 2;
								m_UnitList.GetAt(pos).AtkUnit();
							}
						}
						else
						{
							if(m_UnitList.GetHead().Object.UnitY+m_UnitList.GetAt(pos).Object.UnitAgroRange > m_UnitList.GetAt(pos).Object.UnitY &&
							   m_UnitList.GetHead().Object.UnitX+m_UnitList.GetAt(pos).Object.UnitAgroRange > m_UnitList.GetAt(pos).Object.UnitX &&
							   m_UnitList.GetHead().Object.UnitY-m_UnitList.GetAt(pos).Object.UnitAgroRange < m_UnitList.GetAt(pos).Object.UnitY &&
							   m_UnitList.GetHead().Object.UnitX-m_UnitList.GetAt(pos).Object.UnitAgroRange < m_UnitList.GetAt(pos).Object.UnitX)
							{
								// 테스트 시작
								if(m_UnitList.GetHead().Object.UnitY > m_UnitList.GetAt(pos).Object.UnitY)
								{
									m_UnitList.GetAt(pos).Object.UnitDirection = 0;
									if(!m_UnitList.GetAt(pos).MoveUnit())
									{
										if(rand()%2)
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 1;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 3;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 2;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}
										else
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 3;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 1;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 2;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}
									}
								}
							
								else if(m_UnitList.GetHead().Object.UnitX < m_UnitList.GetAt(pos).Object.UnitX)
								{
									m_UnitList.GetAt(pos).Object.UnitDirection = 1;
									if(!m_UnitList.GetAt(pos).MoveUnit())
									{
										if(rand()%2)
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 2;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 0;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 3;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}
										else
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 0;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 2;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 3;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}
									}
								}
								
								else if(m_UnitList.GetHead().Object.UnitY < m_UnitList.GetAt(pos).Object.UnitY)
								{
									m_UnitList.GetAt(pos).Object.UnitDirection = 2;
									if(!m_UnitList.GetAt(pos).MoveUnit())
									{
										if(rand()%2)
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 1;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 3;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 0;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}
										else
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 3;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 1;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 0;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}
									}
								}
								
								else if(m_UnitList.GetHead().Object.UnitX > m_UnitList.GetAt(pos).Object.UnitX)
								{
									m_UnitList.GetAt(pos).Object.UnitDirection = 3;
									if(!m_UnitList.GetAt(pos).MoveUnit())
									{
										if(rand()%2)
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 2;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 0;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 1;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}

										else
										{
											m_UnitList.GetAt(pos).Object.UnitDirection = 0;
											if(!m_UnitList.GetAt(pos).MoveUnit())
											{
												m_UnitList.GetAt(pos).Object.UnitDirection = 2;
												if(!m_UnitList.GetAt(pos).MoveUnit())
												{
													m_UnitList.GetAt(pos).Object.UnitDirection = 1;
													m_UnitList.GetAt(pos).MoveUnit();
												}
											}
										}
									}
								}
							}
							//테스트 끝
							else
							{
								m_UnitList.GetAt(pos).Object.UnitDirection = rand()%4;
								m_UnitList.GetAt(pos).MoveUnit();
							}
						}
					}
					else if(m_UnitList.GetAt(pos).Object.UnitState == DEAD)
						m_UnitList.GetAt(pos).ReviveUnit();

					m_UnitList.GetNext(pos);
				}
			}
			else
			{
				POSITION pos = m_UnitList.GetHeadPosition();
				while(pos != NULL)
				{
					if(m_UnitList.GetAt(pos).Object.UnitState == STAND && m_UnitList.GetAt(pos).Object.UnitState != DEAD)
					{
						m_UnitList.GetAt(pos).Object.UnitDirection = rand()%4;
						m_UnitList.GetAt(pos).MoveUnit();
					}
					else if(m_UnitList.GetAt(pos).Object.UnitState == DEAD && pos != m_UnitList.GetHeadPosition())
						m_UnitList.GetAt(pos).ReviveUnit();

					m_UnitList.GetNext(pos);
				}
			}
			break;

		case 3:
			break;
	}
	
	CView::OnTimer(nIDEvent);
}

void CROAPreludeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_UnitList.GetHead().Object.UnitAbleMove == TRUE && m_UnitList.GetHead().Object.UnitState == STAND)
	{
		if(nChar == VK_DOWN)
		{
			if(m_UnitList.GetHead().Object.UnitDirection == 0)
				m_UnitList.GetHead().MoveUnit();
			
			else
			{
				m_UnitList.GetHead().Object.UnitDirection = 0;
				m_UnitList.GetHead().Object.UnitMotion = 0;
			}
		}

		else if(nChar == VK_UP)
		{
			if(m_UnitList.GetHead().Object.UnitDirection == 2)
				m_UnitList.GetHead().MoveUnit();
			
			else
			{
				m_UnitList.GetHead().Object.UnitDirection = 2;
				m_UnitList.GetHead().Object.UnitMotion = 20;
			}
		}

		else if(nChar == VK_LEFT)
		{
			if(m_UnitList.GetHead().Object.UnitDirection == 1)
				m_UnitList.GetHead().MoveUnit();
			
			else
			{
				m_UnitList.GetHead().Object.UnitDirection = 1;
				m_UnitList.GetHead().Object.UnitMotion = 10;
			}
		}

		else if(nChar == VK_RIGHT)
		{
			if(m_UnitList.GetHead().Object.UnitDirection == 3)
				m_UnitList.GetHead().MoveUnit();
			
			else
			{
				m_UnitList.GetHead().Object.UnitDirection = 3;
				m_UnitList.GetHead().Object.UnitMotion = 30;
			}
		}

		else if(nChar == VK_SPACE)
		{
			// only for test
			if(m_UnitList.GetHead().Object.UnitMp >= 10)
			{
				if(m_UnitList.GetHead().Object.UnitDirection == DOWN)
					m_UnitList.GetHead().TeleportUnit(m_UnitList.GetHead().Object.UnitX, m_UnitList.GetHead().Object.UnitY + 3, m_UnitList.GetHead().Object.UnitDirection);
				
				else if(m_UnitList.GetHead().Object.UnitDirection == LEFT)
					m_UnitList.GetHead().TeleportUnit(m_UnitList.GetHead().Object.UnitX - 3, m_UnitList.GetHead().Object.UnitY, m_UnitList.GetHead().Object.UnitDirection);
				
				else if(m_UnitList.GetHead().Object.UnitDirection == UP)
					m_UnitList.GetHead().TeleportUnit(m_UnitList.GetHead().Object.UnitX, m_UnitList.GetHead().Object.UnitY - 3, m_UnitList.GetHead().Object.UnitDirection);
				
				else if(m_UnitList.GetHead().Object.UnitDirection == RIGHT)
					m_UnitList.GetHead().TeleportUnit(m_UnitList.GetHead().Object.UnitX + 3, m_UnitList.GetHead().Object.UnitY, m_UnitList.GetHead().Object.UnitDirection);

				m_UnitList.GetHead().Object.UnitMp -= 10; // Only For Test
			}
		}

		else if(nChar == 'A')
			m_UnitList.GetHead().AtkUnit();

		else
		{

		}
	}

	if(nChar == VK_RETURN)
	{
		//m_CamLockHero = !m_CamLockHero;
		m_UnitList.GetHead().ReviveUnit();
	}

	if(nChar == 'U')
			HostPlayer.CharWndOn = !HostPlayer.CharWndOn;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CROAPreludeView::AnimationDelay()
{
	POSITION pos = m_UnitList.GetHeadPosition();

	while(pos != NULL)
	{
		// 이동 모션
		if(m_UnitList.GetAt(pos).Object.UnitState == MOVE)
		{
			if(m_UnitList.GetAt(pos).Object.UnitDelayMove <= 0) // 이동 딜레이 끝
			{
				m_UnitList.GetAt(pos).Object.UnitDelayMove = m_UnitList.GetAt(pos).Object.UnitBkDelayMove;
				
				if(m_UnitList.GetAt(pos).Object.UnitMotion%10 > 0 && m_UnitList.GetAt(pos).Object.UnitMotion%10 != 3)
					m_UnitList.GetAt(pos).Object.UnitMotion++;
				
				else
				{	
					m_UnitList.GetAt(pos).Object.UnitMotion -= 3;
					m_UnitList.GetAt(pos).Object.UnitState = STAND;
				}
			}
			
			else
				--m_UnitList.GetAt(pos).Object.UnitDelayMove;
		}
		
		// 공격 모션
		else if(m_UnitList.GetAt(pos).Object.UnitState == ATTACK)
		{
			if(m_UnitList.GetAt(pos).Object.UnitDelayAtk <= 0) // 공격 딜레이 끝
			{
				m_UnitList.GetAt(pos).Object.UnitDelayAtk = m_UnitList.GetAt(pos).Object.UnitBkDelayAtk;
				
				if(m_UnitList.GetAt(pos).Object.UnitMotion%10 == 0)
					m_UnitList.GetAt(pos).Object.UnitMotion += 4;

				else if(m_UnitList.GetAt(pos).Object.UnitMotion%10 < 6)
					m_UnitList.GetAt(pos).Object.UnitMotion++;

				else
				{	
					m_UnitList.GetAt(pos).Object.UnitMotion -= 6;
					m_UnitList.GetAt(pos).Object.UnitState = STAND;
				}
			}

			else
				--m_UnitList.GetAt(pos).Object.UnitDelayAtk;
		}

		m_UnitList.GetNext(pos);
	}

	if(m_CamLockHero == TRUE)
		HostPlayer.MoveCamera(m_UnitList.GetHead().Object.UnitX*48, m_UnitList.GetHead().Object.UnitY*48);
	
	if(m_UnitList.GetHead().Object.UnitDirection % 2 == 0 && m_UnitList.GetHead().Object.UnitState == MOVE) // 위 아래
		HostPlayer.CamY = m_UnitList.GetHead().Object.UnitY*48+((m_UnitList.GetHead().Object.UnitDirection -1)*(48-(((m_UnitList.GetHead().Object.UnitMotion)%10)*12)));
	
	else if(m_UnitList.GetHead().Object.UnitState == MOVE) // 좌 우
		HostPlayer.CamX = m_UnitList.GetHead().Object.UnitX*48-((m_UnitList.GetHead().Object.UnitDirection -2)*(48-(((m_UnitList.GetHead().Object.UnitMotion)%10)*12)));
}

void CROAPreludeView::TransDraw(CDC * pDC, int x, int y,int width,int height,CDC* srcDC,int sx,int sy,COLORREF crColour)
{
	CDC dcImage, dcTrans;

	dcImage.CreateCompatibleDC(pDC);
	dcTrans.CreateCompatibleDC(pDC);
	
	CBitmap bitmapTrans;

	bitmapTrans.CreateBitmap(width, height, 1, 1, NULL);
	
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);
	
	dcImage.SetBkColor(crColour);
	srcDC->SetBkColor(crColour);
	dcTrans.BitBlt(0, 0, width, height, srcDC, sx, sy, SRCCOPY);
	pDC->BitBlt(x, y, width, height, srcDC, sx, sy, SRCINVERT);
	pDC->BitBlt(x, y, width, height, &dcTrans, 0, 0, SRCAND);
	pDC->BitBlt(x, y, width, height, srcDC, sx, sy, SRCINVERT);
	dcTrans.SelectObject(pOldBitmapTrans);
}

void CROAPreludeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 캐릭터창 열었을 때
	if(HostPlayer.CharWndOn == TRUE)
	{
		// 스텟이 남아 있을 때
		if(m_UnitList.GetHead().Object.UnitRemainStat > 0)
		{
			if(point.x >= 678 && point.x <= 693 && point.y >= 230 && point.y <= 245) // 힘
			{
				m_UnitList.GetHead().Object.UnitStatSTR++;
				m_UnitList.GetHead().Object.UnitRemainStat--;
			}

			else if(point.x >= 678 && point.x <= 693 && point.y >= 260 && point.y <= 275) // 민첩
			{
				m_UnitList.GetHead().Object.UnitStatDEX++;
				m_UnitList.GetHead().Object.UnitRemainStat--;
			}

			else if(point.x >= 678 && point.x <= 693 && point.y >= 290 && point.y <= 305) // 지능
			{
				m_UnitList.GetHead().Object.UnitStatINT++;
				m_UnitList.GetHead().Object.UnitRemainStat--;
			}
			
			else if(point.x >= 678 && point.x <= 693 && point.y >= 320 && point.y <= 335) // 체력
			{
				m_UnitList.GetHead().Object.UnitStatHEL++;
				m_UnitList.GetHead().Object.UnitRemainStat--;
			}

			else if(point.x >= 678 && point.x <= 693 && point.y >= 350 && point.y <= 365) // 정신력
			{
				m_UnitList.GetHead().Object.UnitStatSPR++;
				m_UnitList.GetHead().Object.UnitRemainStat--;
			}

			else if(point.x >= 678 && point.x <= 693 && point.y >= 380 && point.y <= 395) // 집중
			{
				m_UnitList.GetHead().Object.UnitStatCON++;
				m_UnitList.GetHead().Object.UnitRemainStat--;
			}			
		}
	}
	
	CView::OnLButtonDown(nFlags, point);


}
