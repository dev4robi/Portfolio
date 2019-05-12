// RuneStoneView.cpp : implementation of the CRuneStoneView class
//

#include "stdafx.h"
#include "RuneStone.h"

#include "RuneStoneDoc.h"
#include "RuneStoneView.h"

// *추가된 해더 파일
#include "MainFrm.h"
#include "DataDialog.h"
#include "MinimapDialog.h"
#include "InputMapData.h"
#include "LocDialog.h"
#include "LocDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneView

IMPLEMENT_DYNCREATE(CRuneStoneView, CScrollView)

BEGIN_MESSAGE_MAP(CRuneStoneView, CScrollView)
	//{{AFX_MSG_MAP(CRuneStoneView)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneView construction/destruction

CRuneStoneView::CRuneStoneView()
{
	// *복사 붙여넣기 변수 초기화
	m_CopyWidth = m_CopyHeight = 0;

	// *드래그 초기화
	m_IsMouseClicked = FALSE;
	m_MouseDragX = m_MouseDragY = -1;

	// *초기 마우스 좌표
	m_MouseX = m_MouseY = 0;

	// *초기 선택한 타일
	m_SelectX = m_SelectY = 0;

	// *다이얼로그 뷰에서 선택된 지형ID
	m_SelectDlgTrinID = 0;

	// *다이얼로그 뷰에서 선택된 브러시 종류 / 브러시 설정 (초기 선택 브러시/설정 없음)
	m_SelectDlgBrush = -1;
	m_SelectDlgOpSize = -1;
	m_SelectDlgOpType = -1;

	// *구역 다이얼로그 뷰에서 선택된 구역 버튼
	m_SelectDlgLoc = -1;

	// *맵 존재여부
	m_IsMapExist = FALSE;

	// *격자와 충돌영역과 높낮이와 구역 보이기
	m_ShowGrid = FALSE;
	m_ShowColl = FALSE;
	m_ShowHigh = FALSE;
	m_ShowLoc = FALSE;
}

CRuneStoneView::~CRuneStoneView()
{
}

void CRuneStoneView::FillBrushDraw(int ClickTrinID, int SelectTrinID, int x, int y, int Count) // *채우기 브러시 재귀함수
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)GetDocument();

	if(Count < 4500)
	{
		if(x != -1 && y != -1 && x != pDoc->m_SizeWidthDoc && y != pDoc->m_SizeHeightDoc)
		{
			if(SelectTrinID != ClickTrinID)
			{
				if(pDoc->m_MapDataDoc[x+y*(pDoc->m_SizeWidthDoc)].TrinID == ClickTrinID)
				{
					InputMapData(x,y,SelectTrinID);
					FillBrushDraw(ClickTrinID, SelectTrinID, x, y+1, Count++);
					FillBrushDraw(ClickTrinID, SelectTrinID, x+1, y, Count++);
					FillBrushDraw(ClickTrinID, SelectTrinID, x, y-1, Count++);
					FillBrushDraw(ClickTrinID, SelectTrinID, x-1, y, Count++);
				}
			}
		}
	}
}

void CRuneStoneView::LineBrushDraw(int SelectOpType, int SelectTrinID, int x, int y, int SelectOpSize) // *선 그리기 브러시 함수
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)GetDocument();

	if(SelectOpType == 1) // *(모양: ─)
	{
		for(int i=0; i < SelectOpSize; i++)
		{
			if(((x-(SelectOpSize/2))+i) >= 0 && ((x-(SelectOpSize/2))+i) < pDoc->m_SizeWidthDoc && SelectOpSize != 0)
			{
				InputMapData((x-(SelectOpSize/2))+i,y,SelectTrinID);
			}
		}
	}
	
	else if(SelectOpType == 2) // *(모양: │)
	{
		for(int i=0; i < SelectOpSize; i++)
		{
			if(((y-(SelectOpSize/2))+i) >= 0 && ((y-(SelectOpSize/2))+i) < pDoc->m_SizeHeightDoc && SelectOpSize != 0)
			{
				InputMapData(x,(y-(SelectOpSize/2))+i,SelectTrinID);
			}
		}
	}
	
	else if(SelectOpType == 3) // *(모양: ＼)
	{
		for(int i=0; i < SelectOpSize; i++)
		{
			if(((x-(SelectOpSize/2))+i) >= 0 && ((y-(SelectOpSize/2))+i) >= 0 && ((x-(SelectOpSize/2))+i) < pDoc->m_SizeWidthDoc && ((y-(SelectOpSize/2))+i) < pDoc->m_SizeHeightDoc && SelectOpSize != 0)
			{
				InputMapData((x-(SelectOpSize/2))+i,(y-(SelectOpSize/2))+i,SelectTrinID);
			}
		}
	}
	
	else if(SelectOpType == 4) // *(모양: ／)
	{
		for(int i=0; i < SelectOpSize; i++)
		{
			if(((x-(SelectOpSize/2))+i) >= 0 && ((y+(SelectOpSize/2))-i) >= 0 && ((x-(SelectOpSize/2))+i) < pDoc->m_SizeWidthDoc && ((y-(SelectOpSize/2))+i) < pDoc->m_SizeHeightDoc && SelectOpSize != 0)
			{
				InputMapData((x-(SelectOpSize/2))+i,(y+(SelectOpSize/2))-i,SelectTrinID);
			}
		}
	}
}

void CRuneStoneView::PlusBrushDraw(int SelectDlgOpSize, int SelectTrinID, int x, int y) // *십자 브러시 재귀함수
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)GetDocument();
	
	InputMapData(x,y,SelectTrinID);

	if(x != 0 && SelectDlgOpSize != 0)
		PlusBrushDraw(SelectDlgOpSize-1, SelectTrinID, x-1, y);

	if(x != (pDoc->m_SizeWidthDoc-1) && SelectDlgOpSize != 0)
		PlusBrushDraw(SelectDlgOpSize-1, SelectTrinID, x+1, y);

	if(y != 0 && SelectDlgOpSize != 0)
		PlusBrushDraw(SelectDlgOpSize-1, SelectTrinID, x, y-1);

	if(y != (pDoc->m_SizeHeightDoc-1) && SelectDlgOpSize != 0)
		PlusBrushDraw(SelectDlgOpSize-1, SelectTrinID, x, y+1);
}

BOOL CRuneStoneView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// *뷰 영역 초기 색상 진한회색으로 변경 (마우스 포인트, 아이콘도 변경 가능할듯.)
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, LoadCursor(NULL, IDC_ARROW),
				   (HBRUSH) GetStockObject(GRAY_BRUSH), LoadIcon(NULL, IDI_APPLICATION));

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneView printing



void CRuneStoneView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
}

BOOL CRuneStoneView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRuneStoneView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRuneStoneView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneView diagnostics

#ifdef _DEBUG
void CRuneStoneView::AssertValid() const
{
	CView::AssertValid();
}

void CRuneStoneView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRuneStoneDoc* CRuneStoneView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRuneStoneDoc)));
	return (CRuneStoneDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneView message handlers

void CRuneStoneView::OnInitialUpdate() // *뷰 영역 생성할 때
{
	CScrollView::OnInitialUpdate();


	// TODO: Add your specialized code here and/or call the base class
}

void CRuneStoneView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// *다큐먼트를 가리키는 포인터 생성 (다큐먼트의 데이터에 접근)
	CRuneStoneDoc *pDoc = GetDocument();
	int MapSize = (pDoc->m_SizeWidthDoc) * (pDoc->m_SizeHeightDoc);

	// *사용자 정의 펜
	CPen GridPen(PS_DOT, 1, RGB(0, 0, 0));
	CPen CollGreenPen(PS_SOLID, 1, RGB(0, 255, 0));
	CPen CollRedPen(PS_SOLID, 1, RGB(255, 0, 0));

	// *사용자 정의 브러시
	CBrush RedBrush(RGB(255, 0, 0));
	CBrush GreenBrush(RGB(0, 255, 0));
	
	// *스크롤 바 생성
	CSize sizeTotal;
	sizeTotal.cx = (pDoc->m_SizeWidthDoc)*48;
	sizeTotal.cy = (pDoc->m_SizeHeightDoc)*48;
	CScrollView::SetScrollSizes(MM_TEXT, sizeTotal);

	// *맵이 존재하면
	if(m_IsMapExist == TRUE)
	{
		// *실제 좌표와 가상 좌표의 칸수 차이를 저장
		CPoint RealPos = GetScrollPosition();
		RealPos.x = RealPos.x / 48;
		RealPos.y = RealPos.y / 48;

		// *클라이언트 뷰 영역의 크기 저장
		CRect ViewArea;
		GetClientRect(&ViewArea);

		// *맵 타일 그리기
		if(m_ShowColl == FALSE)
		{
			OnPrepareDC(&dc);

			CBitmap MapDraw;
			MapDraw.LoadBitmap(IDB_TILESET);

			BITMAP bmpinfo;
			MapDraw.GetBitmap(&bmpinfo);

			CDC dcmem;
			dcmem.CreateCompatibleDC(&dc);
			dcmem.SelectObject(&MapDraw);

			for(int i=RealPos.y; i<RealPos.y + (ViewArea.Height() / 48) + (pDoc->m_SizeHeightDoc-1 > RealPos.y + ((ViewArea.Height() / 48)) ? 2:1); i++)
			{
				for(int j=RealPos.x; j<RealPos.x + (ViewArea.Width() / 48) + (pDoc->m_SizeWidthDoc-1 > RealPos.x + ((ViewArea.Width() / 48)) ? 2:1); j++)
				{
					dc.BitBlt((j*48), (i*48), 48, 48, &dcmem, (pDoc->m_MapDataDoc[(i*(pDoc->m_SizeWidthDoc))+j].TrinID)
					%16*48, (pDoc->m_MapDataDoc[(i*(pDoc->m_SizeWidthDoc))+j].TrinID)/16*48, SRCCOPY);
				}
			}
		}

		// *충돌여부 그리기
		else
		{
			OnPrepareDC(&dc);
			
			for(int i=RealPos.y; i<RealPos.y + (ViewArea.Height() / 48) + (pDoc->m_SizeHeightDoc-1 > RealPos.y + ((ViewArea.Height() / 48)) ? 2:1); i++)
			{
				for(int j=RealPos.x; j<RealPos.x + (ViewArea.Width() / 48) + (pDoc->m_SizeWidthDoc-1 > RealPos.x + ((ViewArea.Width() / 48)) ? 2:1); j++)
				{
					// *이동 가능시 녹색 칠함
					if(pDoc->m_MapDataDoc[(i*(pDoc->m_SizeWidthDoc))+j].TrinMove == TRUE)
					{
						dc.SelectObject(&GreenBrush);
						dc.SelectObject(&CollGreenPen);
						
						dc.Rectangle(j*48, i*48, j*48+48, i*48+48);
					}

					// *이동 불가능시 적색 칠함
					else
					{
						dc.SelectObject(&RedBrush);
						dc.SelectObject(&CollRedPen);

						dc.Rectangle(j*48, i*48, j*48+48, i*48+48);
					}
				}
			}
		}

		// *구역 그리기
		if(m_ShowLoc == TRUE)
		{
			// *구역 리스트에서 데이터를 하나하나 받아와서 뷰 영역에 그려준다.
			for(int i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
			{
				CBrush BlueBrush(RGB(0, 0, 255));
				CBrush RedBrush(RGB(255, 0, 0));
				CBrush GreenBrush(RGB(0, 255, 0));
				CBrush YellowBrush(RGB(255, 255, 0));
				CBrush PinkBrush(RGB(255, 0, 255));

				if(pDoc->m_LocationDoc.GetAt(i).LocationColor == BLUE) dc.SelectObject(&BlueBrush);
				else if(pDoc->m_LocationDoc.GetAt(i).LocationColor == RED) dc.SelectObject(&RedBrush);
				else if(pDoc->m_LocationDoc.GetAt(i).LocationColor == GREEN) dc.SelectObject(&GreenBrush);
				else if(pDoc->m_LocationDoc.GetAt(i).LocationColor == YELLOW) dc.SelectObject(&YellowBrush);
				else if(pDoc->m_LocationDoc.GetAt(i).LocationColor == PINK) dc.SelectObject(&PinkBrush);

				dc.SelectStockObject(NULL_PEN);
				dc.Rectangle(pDoc->m_LocationDoc.GetAt(i).LocationX1*48, pDoc->m_LocationDoc.GetAt(i).LocationY1*48, (pDoc->m_LocationDoc.GetAt(i).LocationX2*48)+48, (pDoc->m_LocationDoc.GetAt(i).LocationY2*48)+48);
				dc.SelectStockObject(BLACK_PEN);
				dc.TextOut(pDoc->m_LocationDoc.GetAt(i).LocationX1*48+5, pDoc->m_LocationDoc.GetAt(i).LocationY1*48+5, pDoc->m_LocationDoc.GetAt(i).LocationName);

				if(pDoc->m_LocationDoc[i].LocationSelected == TRUE)
				{
					CPen SelLocPen(PS_INSIDEFRAME, 4, RGB(255, 255, 255));
				
					dc.SelectObject(&SelLocPen);
					dc.SelectStockObject(NULL_BRUSH);
					dc.Rectangle(pDoc->m_LocationDoc.GetAt(i).LocationX1*48, pDoc->m_LocationDoc.GetAt(i).LocationY1*48, pDoc->m_LocationDoc.GetAt(i).LocationX2*48+48, pDoc->m_LocationDoc.GetAt(i).LocationY2*48+48);
				}
			}
		}

		// *격자 그리기
		if(m_ShowGrid == TRUE)
		{
			dc.SelectObject(&GridPen);

			for(int i=RealPos.y; i<RealPos.y + (ViewArea.Height() / 48) + (pDoc->m_SizeHeightDoc-1 > RealPos.y + ((ViewArea.Height() / 48)) ? 2:1); i++)
			{
				dc.MoveTo(0, i*48);
				dc.LineTo((pDoc->m_SizeWidthDoc)*48, i*48);
			}

			for(int j=RealPos.x; j<RealPos.x + (ViewArea.Width() / 48) + (pDoc->m_SizeWidthDoc-1 > RealPos.x + ((ViewArea.Width() / 48)) ? 2:1); j++)
			{
				dc.MoveTo(j*48, 0);
				dc.LineTo(j*48, (pDoc->m_SizeHeightDoc)*48);
			}
		}

		// *높낮이 그리기
		if(m_ShowHigh == TRUE)
		{
			if(MapSize >= 1024)
			{
				OnPrepareDC(&dc);
				dc.SetBkColor(RGB(255, 255, 0));
				
				for(int i=RealPos.y; i<RealPos.y + (ViewArea.Height() / 48) + (pDoc->m_SizeHeightDoc-1 > RealPos.y + ((ViewArea.Height() / 48)) ? 2:1); i++)
				{
					for(int j=RealPos.x; j<RealPos.x + (ViewArea.Width() / 48) + (pDoc->m_SizeWidthDoc-1 > RealPos.x + ((ViewArea.Width() / 48)) ? 2:1); j++)
					{
						CRect TrinRect(j*48, i*48, j*48+48, i*48+48);
						CString TrinHigh;
						TrinHigh.Format(" %d ", (pDoc->m_MapDataDoc[(i*(pDoc->m_SizeWidthDoc))+j].TrinHigh));

						dc.DrawText(TrinHigh, &TrinRect, DT_LEFT | DT_TOP);
					}
				}
			}
		}
		
		CClientDC dc(this);
		CPen SelectPen(PS_INSIDEFRAME, 4, RGB(255, 255, 0));
		CBrush BlueBrush(RGB(0, 0, 255));
		dc.SelectStockObject(NULL_BRUSH);
		dc.SelectObject(&SelectPen);
		
		OnPrepareDC(&dc);

		// *선택 브러시 그리기
		if (m_SelectDlgBrush == 0)
		{
			// *선택 영역 그리기
			if(m_SelectX <= m_MouseDragX && m_SelectY <= m_MouseDragY)
			{		
				dc.Rectangle(m_SelectX*48, m_SelectY*48, (m_MouseDragX)*48+49,(m_MouseDragY)*48+49);
			}
			else if(m_SelectX <= m_MouseDragX && m_SelectY >= m_MouseDragY)
			{
				dc.Rectangle(m_SelectX*48, (m_MouseDragY)*48, (m_MouseDragX)*48+49,m_SelectY*48+49);
			}
			else if(m_SelectX >= m_MouseDragX && m_SelectY <= m_MouseDragY)
			{
				dc.Rectangle((m_MouseDragX)*48, m_SelectY*48, m_SelectX*48+49,(m_MouseDragY)*48+49);
			}
			else if(m_SelectX >= m_MouseDragX && m_SelectY >= m_MouseDragY)
			{
				dc.Rectangle((m_MouseDragX)*48, (m_MouseDragY)*48, m_SelectX*48+49,m_SelectY*48+49);
			}
		}

		// *도트 브러시 그리기
		else if(m_SelectDlgBrush == 1)
		{
			
			if(m_SelectDlgOpSize > 0 && m_SelectDlgOpSize < 8)
				dc.Rectangle((m_MouseX-m_SelectDlgOpSize/2)*48, (m_MouseY-m_SelectDlgOpSize/2)*48, (m_MouseX+m_SelectDlgOpSize/2)*48+49, (m_MouseY+m_SelectDlgOpSize/2)*48+49);
		}

		// *플러스 브러시 그리기
		else if(m_SelectDlgBrush == 2)
		{

			if(m_SelectDlgOpSize == 1)
			{
				POINT PlusPoint[]={{(m_MouseX)*48, (m_MouseY)*48}, {(m_MouseX)*48, (m_MouseY-1)*48}, {(m_MouseX+1)*48, (m_MouseY-1)*48}, {(m_MouseX+1)*48, (m_MouseY)*48}, 
				{(m_MouseX+2)*48, (m_MouseY)*48}, {(m_MouseX+2)*48, (m_MouseY+1)*48}, {(m_MouseX+1)*48, (m_MouseY+1)*48}, {(m_MouseX+1)*48, (m_MouseY+2)*48}, 
				{(m_MouseX)*48, (m_MouseY+2)*48}, {(m_MouseX)*48, (m_MouseY+1)*48}, {(m_MouseX-1)*48, (m_MouseY+1)*48}, {(m_MouseX-1)*48, (m_MouseY)*48}, {(m_MouseX)*48, (m_MouseY)*48}};
				dc.Polygon(PlusPoint, 13);
			}
			
			else if(m_SelectDlgOpSize == 3)
			{
				POINT PlusPoint[]={{(m_MouseX*48), (m_MouseY-2)*48}, {(m_MouseX+1)*48, (m_MouseY-2)*48}, {(m_MouseX+1)*48, (m_MouseY-1)*48}, {(m_MouseX+2)*48, (m_MouseY-1)*48}
				, {(m_MouseX+2)*48, (m_MouseY)*48}, {(m_MouseX+3)*48, (m_MouseY)*48}, {(m_MouseX+3)*48, (m_MouseY+1)*48}, {(m_MouseX+2)*48, (m_MouseY+1)*48}, {(m_MouseX+2)*48, (m_MouseY+2)*48}, {(m_MouseX+1)*48, (m_MouseY+2)*48}
				, {(m_MouseX+1)*48, (m_MouseY+3)*48}, {(m_MouseX)*48, (m_MouseY+3)*48}, {(m_MouseX)*48, (m_MouseY+2)*48}, {(m_MouseX-1)*48, (m_MouseY+2)*48}, {(m_MouseX-1)*48, (m_MouseY+1)*48}
				, {(m_MouseX-2)*48, (m_MouseY+1)*48}, {(m_MouseX-2)*48, (m_MouseY)*48}, {(m_MouseX-1)*48, (m_MouseY)*48}, {(m_MouseX-1)*48, (m_MouseY-1)*48}, {(m_MouseX)*48, (m_MouseY-1)*48}, {(m_MouseX)*48, (m_MouseY-2)*48}};
				dc.Polygon(PlusPoint, 21);
			}
			
			else if(m_SelectDlgOpSize == 5)
			{
				POINT PlusPoint[]={{(m_MouseX*48), (m_MouseY-3)*48}, {(m_MouseX+1)*48, (m_MouseY-3)*48}, {(m_MouseX+1)*48, (m_MouseY-2)*48}, {(m_MouseX+2)*48, (m_MouseY-2)*48}, {(m_MouseX+2)*48, (m_MouseY-1)*48}, {(m_MouseX+3)*48, (m_MouseY-1)*48}
				, {(m_MouseX+3)*48, (m_MouseY)*48}, {(m_MouseX+4)*48, (m_MouseY)*48}, {(m_MouseX+4)*48, (m_MouseY+1)*48}, {(m_MouseX+3)*48, (m_MouseY+1)*48}, {(m_MouseX+3)*48, (m_MouseY+2)*48}, {(m_MouseX+2)*48, (m_MouseY+2)*48}, {(m_MouseX+2)*48, (m_MouseY+3)*48}
				, {(m_MouseX+1)*48, (m_MouseY+3)*48}, {(m_MouseX+1)*48, (m_MouseY+4)*48}, {(m_MouseX)*48, (m_MouseY+4)*48}, {(m_MouseX)*48, (m_MouseY+3)*48}, {(m_MouseX-1)*48, (m_MouseY+3)*48}, {(m_MouseX-1)*48, (m_MouseY+2)*48}, {(m_MouseX-2)*48, (m_MouseY+2)*48}
				, {(m_MouseX-2)*48, (m_MouseY+1)*48}, {(m_MouseX-3)*48, (m_MouseY+1)*48}, {(m_MouseX-3)*48, (m_MouseY)*48}, {(m_MouseX-2)*48, (m_MouseY)*48}, {(m_MouseX-2)*48, (m_MouseY-1)*48}, {(m_MouseX-1)*48, (m_MouseY-1)*48}, {(m_MouseX-1)*48, (m_MouseY-2)*48}
				, {(m_MouseX)*48, (m_MouseY-2)*48}, {(m_MouseX)*48, (m_MouseY-3)*48}};
				dc.Polygon(PlusPoint, 29);
			}
			
			else if(m_SelectDlgOpSize == 7)
			{
				POINT PlusPoint[]={{(m_MouseX*48), (m_MouseY-4)*48}, {(m_MouseX+1)*48, (m_MouseY-4)*48}, {(m_MouseX+1)*48, (m_MouseY-3)*48}, {(m_MouseX+2)*48, (m_MouseY-3)*48}, {(m_MouseX+2)*48, (m_MouseY-2)*48}, {(m_MouseX+3)*48, (m_MouseY-2)*48}, {(m_MouseX+3)*48, (m_MouseY-1)*48}
				, {(m_MouseX+4)*48, (m_MouseY-1)*48}, {(m_MouseX+4)*48, (m_MouseY*48)}, {(m_MouseX+5)*48, m_MouseY*48}, {(m_MouseX+5)*48, (m_MouseY+1)*48}, {(m_MouseX+4)*48, (m_MouseY+1)*48}, {(m_MouseX+4)*48, (m_MouseY+2)*48}, {(m_MouseX+3)*48, (m_MouseY+2)*48}, {(m_MouseX+3)*48, (m_MouseY+3)*48}
				, {(m_MouseX+2)*48, (m_MouseY+3)*48}, {(m_MouseX+2)*48, (m_MouseY+4)*48}, {(m_MouseX+1)*48, (m_MouseY+4)*48}, {(m_MouseX+1)*48, (m_MouseY+5)*48}, {(m_MouseX*48), (m_MouseY+5)*48}, {m_MouseX*48, (m_MouseY+4)*48}, {(m_MouseX-1)*48, (m_MouseY+4)*48}, {(m_MouseX-1)*48, (m_MouseY+3)*48}
				, {(m_MouseX-2)*48, (m_MouseY+3)*48}, {(m_MouseX-2)*48, (m_MouseY+2)*48}, {(m_MouseX-3)*48, (m_MouseY+2)*48}, {(m_MouseX-3)*48, (m_MouseY+1)*48}, {(m_MouseX-4)*48, (m_MouseY+1)*48}, {(m_MouseX-4)*48, (m_MouseY*48)}, {(m_MouseX-3)*48, (m_MouseY*48)}, {(m_MouseX-3)*48, (m_MouseY-1)*48}
				, {(m_MouseX-2)*48, (m_MouseY-1)*48}, {(m_MouseX-2)*48, (m_MouseY-2)*48}, {(m_MouseX-1)*48, (m_MouseY-2)*48}, {(m_MouseX-1)*48, (m_MouseY-3)*48}, {(m_MouseX*48), (m_MouseY-3)*48}, {(m_MouseX*48), (m_MouseY-4)*48}};
				dc.Polygon(PlusPoint, 37);
			}
		}

		// *라인 브러시 그리기
		else if (m_SelectDlgBrush == 3)
		{
			if(m_SelectDlgOpType == 1 && m_SelectDlgOpSize > 0)
				dc.Rectangle((m_MouseX-m_SelectDlgOpSize/2)*48, m_MouseY*48, (m_MouseX+m_SelectDlgOpSize/2)*48+49, m_MouseY*48+49);
			else if(m_SelectDlgOpType == 2 && m_SelectDlgOpSize > 0)
				dc.Rectangle(m_MouseX*48, (m_MouseY-m_SelectDlgOpSize/2)*48, m_MouseX*48+49, (m_MouseY+m_SelectDlgOpSize/2)*48+49);
			else if(m_SelectDlgOpType == 3 && m_SelectDlgOpSize > 0)
				for(int i=(-1)*(m_SelectDlgOpSize/2); i<=m_SelectDlgOpSize/2;i++)
					dc.Rectangle((m_MouseX+i)*48, (m_MouseY+i)*48, (m_MouseX+i)*48+49, (m_MouseY+i)*48+49);
			else if(m_SelectDlgOpType == 4 && m_SelectDlgOpSize > 0)
				for(int i=(-1)*(m_SelectDlgOpSize/2); i<=m_SelectDlgOpSize/2;i++)
					dc.Rectangle((m_MouseX+i)*48, (m_MouseY-i)*48, (m_MouseX+i)*48+49, (m_MouseY-i)*48+49);
		}	
		
		// *채우기 브러시 그리기
		else if(m_SelectDlgBrush == 4)
		{
			dc.Rectangle(m_MouseX*48, m_MouseY*48, m_MouseX*48+49, m_MouseY*48+49);
		}
		
		// *지우개 브러시 그리기
		else if(m_SelectDlgBrush == 5)
		{
			if(m_SelectDlgOpSize > 0 && m_SelectDlgOpSize < 8)
				dc.Rectangle((m_MouseX-m_SelectDlgOpSize/2)*48, (m_MouseY-m_SelectDlgOpSize/2)*48, (m_MouseX+m_SelectDlgOpSize/2)*48+49, (m_MouseY+m_SelectDlgOpSize/2)*48+49);
		}

		// *지형찾기 브러시 그리기
		else if(m_SelectDlgBrush == 6)
		{
			dc.Rectangle(m_MouseX*48, m_MouseY*48, m_MouseX*48+49, m_MouseY*48+49);
		}

		// * 구역생성 브러시 그리기
		else if(m_SelectDlgLoc == CRELOC)
		{
			if(m_IsMouseClicked == TRUE)
			{
				CPen BlackSelectPen(PS_INSIDEFRAME, 1, RGB(0, 0, 0));
				
				dc.SelectObject(&BlueBrush);
				dc.SelectObject(&BlackSelectPen);

				if(m_LocStartX <= m_MouseDragX && m_LocStartY <= m_MouseDragY)
				{
					dc.Rectangle(m_LocStartX*48, m_LocStartY*48, (m_MouseDragX)*48+48,(m_MouseDragY)*48+48);
				}

				else if(m_LocStartX <= m_MouseDragX && m_LocStartY >= m_MouseDragY)
				{
					dc.Rectangle(m_LocStartX*48, (m_MouseDragY)*48, (m_MouseDragX)*48+48,m_LocStartY*48+48);
				}

				else if(m_LocStartX >= m_MouseDragX && m_LocStartY <= m_MouseDragY)
				{
					dc.Rectangle((m_MouseDragX)*48, m_LocStartY*48, m_LocStartX*48+48,(m_MouseDragY)*48+48);
				}

				else if(m_LocStartX >= m_MouseDragX && m_LocStartY >= m_MouseDragY)
				{
					dc.Rectangle((m_MouseDragX)*48, (m_MouseDragY)*48, m_LocStartX*48+48,m_LocStartY*48+48);
				}
			}
		}
	} // *맵이 존재하면 end

	// *맵이 존재하지 않으면
	else
	{
	}
}

void CRuneStoneView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) // *스크롤 가로 이동시
{
	Invalidate();
	
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRuneStoneView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) // *스크롤 세로 이동시
{
	Invalidate();
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CRuneStoneView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)  // *휠 스크롤로 이동시
{
	Invalidate();
	
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CRuneStoneView::OnLButtonDown(UINT nFlags, CPoint point) // *왼쪽 버튼 클릭시
{
	CClientDC dc(this);
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)GetDocument();

	// *맵이 존재하면
	if(m_IsMapExist == TRUE)
	{
		// *클라이언트 뷰 영역의 크기 저장
		CRect ViewArea;
		GetClientRect(&ViewArea);

		// *실제 좌표와 가상 좌표의 칸수 차이를 저장
		CPoint RealPos = GetScrollPosition();
		RealPos.x += point.x;
		RealPos.y += point.y;
		RealPos.x = RealPos.x / 48;
		RealPos.y = RealPos.y / 48;

		// *클릭한 칸 저장
		m_SelectX = RealPos.x;
		m_SelectY = RealPos.y;

		// *지형 브러시가 선택되어 있으면
		if(m_SelectDlgBrush != -1)
		{
			// *선택 브러시
			if(m_SelectDlgBrush == 0)
			{
				m_MouseDragX = m_SelectX;
				m_MouseDragY = m_SelectY;
				m_IsMouseClicked = TRUE;
			}

			// *점 브러시
			if(m_SelectDlgBrush == 1)
			{
				if(m_SelectDlgOpSize != -1)
				{
					for(int y = (m_SelectY-m_SelectDlgOpSize/2); y <= (m_SelectY+m_SelectDlgOpSize/2); y++)
					{
						for(int x = (m_SelectX-m_SelectDlgOpSize/2); x <= (m_SelectX+m_SelectDlgOpSize/2); x++)
						{
							if(x >= 0 && y >= 0 && x < pDoc->m_SizeWidthDoc && y < pDoc->m_SizeHeightDoc)
								InputMapData(x,y,m_SelectDlgTrinID);
						}
					}

					m_IsMouseClicked = TRUE;
				}

				else
					AfxMessageBox("붓 크기를 선택해 주십시오.");
			}

			// *십자 브러시
			else if(m_SelectDlgBrush == 2)
			{
				if(m_SelectDlgOpSize == -1)	
					AfxMessageBox("붓 크기를 선택해 주십시오.");
				
				else
				{
					PlusBrushDraw(m_SelectDlgOpSize/2+1, m_SelectDlgTrinID, m_SelectX, m_SelectY);
							      m_IsMouseClicked = TRUE;
				}
			}

			// *라인 브러시
			else if(m_SelectDlgBrush == 3)
			{
				if(m_SelectDlgOpSize == -1)
					AfxMessageBox("붓 크기를 선택해 주십시오.");

				else if(m_SelectDlgOpType == -1)
					AfxMessageBox("붓 타입을 선택해 주십시오.");
					
				else
				{
					LineBrushDraw(m_SelectDlgOpType,m_SelectDlgTrinID,m_SelectX,m_SelectY,m_SelectDlgOpSize);
								  m_IsMouseClicked = TRUE;
				}
			}
			
			// *채우기 브러시
			else if(m_SelectDlgBrush == 4)
			{
				FillBrushDraw(pDoc->m_MapDataDoc[m_SelectX+m_SelectY*(pDoc->m_SizeWidthDoc)].TrinID,
						      m_SelectDlgTrinID, m_SelectX, m_SelectY, 0);
			}

			// *지우기 브러시
			else if(m_SelectDlgBrush == 5)
			{
				if(m_SelectDlgOpSize != -1)
				{
					for(int y = (m_SelectY-m_SelectDlgOpSize/2); y <= (m_SelectY+m_SelectDlgOpSize/2); y++)
					{
						for(int x = (m_SelectX-m_SelectDlgOpSize/2); x <= (m_SelectX+m_SelectDlgOpSize/2); x++)
						{
							if(x >= 0 && y >= 0 && x < pDoc->m_SizeWidthDoc && y < pDoc->m_SizeHeightDoc)
							{
								InputMapData(x,y,0);
							}
						}
					}

					m_IsMouseClicked = TRUE;
				}

				else
					AfxMessageBox("붓 크기를 선택해 주십시오.");
			}

			// *지형 찾기 브러시
			else if(m_SelectDlgBrush == 6)
			{
				m_SelectDlgTrinID = pDoc->m_MapDataDoc[m_SelectX+m_SelectY*(pDoc->m_SizeWidthDoc)].TrinID;
			}
		}// end 지형 브러시가 선택되어 있으면
		
		// *구역 창이 선택되어 있으면
		if(m_SelectDlgLoc != NONE)
		{	
			// *구역 선택
			if(m_SelectDlgLoc == SELLOC)
			{
				// 구역 선택 버튼 클릭되어 있을때 왼쪽 버튼 클릭시.
				for(int j=0; j<pDoc->m_LocationDoc.GetSize(); j++)
					pDoc->m_LocationDoc[j].LocationSelected = FALSE;

				for(int i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
				{
					if(m_SelectX >= pDoc->m_LocationDoc.GetAt(i).LocationX1 && m_SelectX <= pDoc->m_LocationDoc.GetAt(i).LocationX2 &&
					   m_SelectY >= pDoc->m_LocationDoc.GetAt(i).LocationY1 && m_SelectY <= pDoc->m_LocationDoc.GetAt(i).LocationY2)
					{
						pDoc->m_LocationDoc[i].LocationSelected = TRUE;
						break;
					}

				}

				m_IsMouseClicked = TRUE;
			}

			// *구역 생성
			else if(m_SelectDlgLoc == CRELOC)
			{
				m_MouseDragX = m_LocStartX = m_SelectX;
				m_MouseDragY = m_LocStartY = m_SelectY;
			}
			
			m_IsMouseClicked = TRUE;
		}
	} // end 맵이 존재하면

	// *맵이 존재하지 않으면
	else
	{
	}

	//Invalidate(FALSE);
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CRuneStoneView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) // *키 입력시
{
	// *맵이 존재하면
	if(m_IsMapExist == TRUE)
	{
		if(m_SelectDlgBrush == 0)
		{
			// *입력키가 "엔터"
			if(nChar == VK_RETURN)
			{
				// *다큐먼트를 가리키는 포인터 생성 (다큐먼트의 데이터에 접근)
				CRuneStoneDoc *pDoc = GetDocument();
				
				// *맵이 존재하면
				DataDialog DataDlg;
				int DlgReturn = DataDlg.DoModal();
				
				if(DlgReturn == IDOK)
				{
					// *선택된 영역이 한개일때
					if(m_SelectX == m_MouseDragX && m_SelectY == m_MouseDragY)
					{
						pDoc->m_MapDataDoc[m_SelectX+(m_SelectY*(pDoc->m_SizeWidthDoc))].TrinID = DataDlg.m_TrinIDDataDlg;
						pDoc->m_MapDataDoc[m_SelectX+(m_SelectY*(pDoc->m_SizeWidthDoc))].TrinNextID = DataDlg.m_TrinNextIDDataDlg;
						pDoc->m_MapDataDoc[m_SelectX+(m_SelectY*(pDoc->m_SizeWidthDoc))].TrinHigh = DataDlg.m_TrinHighDataDlg;
						pDoc->m_MapDataDoc[m_SelectX+(m_SelectY*(pDoc->m_SizeWidthDoc))].TrinMove = DataDlg.m_TrinMoveDataDlg;
					}
					
					// *드래그를 했을때
					else
					{
						if(m_SelectX <= m_MouseDragX)
						{
							if(m_SelectY <= m_MouseDragY)
							{
								for(int i = m_SelectY; i <= m_MouseDragY; i++)
								{
									for(int j = m_SelectX; j <= m_MouseDragX; j++)
									{
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinID = DataDlg.m_TrinIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinNextID = DataDlg.m_TrinNextIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinHigh = DataDlg.m_TrinHighDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinMove = DataDlg.m_TrinMoveDataDlg;
									}
								}
							}
							else
							{
								for(int i = m_MouseDragY; i <= m_SelectY; i++)
								{
									for(int j = m_SelectX; j <= m_MouseDragX; j++)
									{
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinID = DataDlg.m_TrinIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinNextID = DataDlg.m_TrinNextIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinHigh = DataDlg.m_TrinHighDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinMove = DataDlg.m_TrinMoveDataDlg;
									}
								}
							}
						}
						else
						{
							if(m_SelectY <= m_MouseDragY)
							{
								for(int i = m_SelectY; i <= m_MouseDragY; i++)
								{
									for(int j = m_MouseDragX; j <= m_SelectX; j++)
									{
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinID = DataDlg.m_TrinIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinNextID = DataDlg.m_TrinNextIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinHigh = DataDlg.m_TrinHighDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinMove = DataDlg.m_TrinMoveDataDlg;
									}
								}
							}
							else
							{
								for(int i = m_MouseDragY; i <= m_SelectY; i++)
								{
									for(int j = m_MouseDragX; j <= m_SelectX; j++)
									{
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinID = DataDlg.m_TrinIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinNextID = DataDlg.m_TrinNextIDDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinHigh = DataDlg.m_TrinHighDataDlg;
										pDoc->m_MapDataDoc[ j + ( i * (pDoc->m_SizeWidthDoc) )].TrinMove = DataDlg.m_TrinMoveDataDlg;
									}
								}
							}
						}
					}
					
					CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
					
					if(pMainFrm->m_pMiniDlg != NULL)
						pMainFrm->m_pMiniDlg->CMinimapDialog::Invalidate(FALSE);
				}

				Invalidate(FALSE);	
			}
		}

		if(m_SelectDlgLoc == SELLOC)
		{
			if(nChar == VK_RETURN)
			{
				// *다큐먼트를 가리키는 포인터 생성 (다큐먼트의 데이터에 접근)
				CRuneStoneDoc *pDoc = (CRuneStoneDoc*)GetDocument();
				CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
				
				// *맵이 존재하면
				CLocDataDlg DataDlg;
				int DlgReturn = DataDlg.DoModal();
				
				if(DlgReturn == IDOK)
				{
					if(DataDlg.m_LocX1 > DataDlg.m_LocX2)
					{
						int Temp;
						Temp = DataDlg.m_LocX2;
						DataDlg.m_LocX2 = DataDlg.m_LocX1;
						DataDlg.m_LocX1 = DataDlg.m_LocX2;
					}	
					
					if(DataDlg.m_LocY1 > DataDlg.m_LocY2)
					{
						int Temp;
						Temp = DataDlg.m_LocY2;
						DataDlg.m_LocY2 = DataDlg.m_LocY1;
						DataDlg.m_LocY1 = DataDlg.m_LocY2;
					}	
					
					pDoc->m_LocationDoc[DataDlg.m_LocID].LocationColor = DataDlg.m_LocColor;
					pDoc->m_LocationDoc[DataDlg.m_LocID].LocationName = DataDlg.m_LocName;
					pDoc->m_LocationDoc[DataDlg.m_LocID].LocationX1 = DataDlg.m_LocX1;
					pDoc->m_LocationDoc[DataDlg.m_LocID].LocationY1 = DataDlg.m_LocY1;
					pDoc->m_LocationDoc[DataDlg.m_LocID].LocationX2 = DataDlg.m_LocX2;
					pDoc->m_LocationDoc[DataDlg.m_LocID].LocationY2 = DataDlg.m_LocY2;
					UpdateData(TRUE);
					pMainFrm->m_pLocDlg->m_LocListBox.Invalidate(FALSE);
					pMainFrm->m_pLocDlg->m_LocListBox.Invalidate();
				}
			}
		}
	}
	
	// *맵이 존재하지 않으면
	else
	{
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRuneStoneView::OnMouseMove(UINT nFlags,  CPoint point) // *마우스가 이동할 때
{    

	CClientDC dc(this);
	CPen SelectPen(PS_INSIDEFRAME, 4, RGB(255, 255, 0));
	CBrush BlueBrush(RGB(0, 0, 255));

	dc.SelectStockObject(NULL_BRUSH);
	dc.SelectObject(&SelectPen);

	OnPrepareDC(&dc);

	CPoint RealPos = GetScrollPosition();
	RealPos.x += point.x;
	RealPos.y += point.y;
	RealPos.x = RealPos.x/48;
	RealPos.y = RealPos.y/48;

	// *이전 좌표와 현재 좌표가 다를시
	if(RealPos.x != m_MouseX || RealPos.y != m_MouseY)
	{
		m_MouseX = RealPos.x;
		m_MouseY = RealPos.y;

		// *지형 브러시 선택영역을 표시하기 위한 부분
		if(m_SelectDlgBrush != -1)
		{
			// *선택 브러시 그리기
			if(m_SelectDlgBrush == 0)
			{
				if(m_IsMouseClicked)
				{
					m_MouseDragX = m_MouseX;
					m_MouseDragY = m_MouseY;
				}
			}

			// *도트 브러시 그리기
			else if(m_SelectDlgBrush == 1)
			{	
				if(m_SelectDlgOpSize > 0 && m_SelectDlgOpSize <8)
					dc.Rectangle((m_MouseX-m_SelectDlgOpSize/2)*48, (m_MouseY-m_SelectDlgOpSize/2)*48, (m_MouseX+m_SelectDlgOpSize/2)*48+49, (m_MouseY+m_SelectDlgOpSize/2)*48+49);

				if(m_IsMouseClicked)
				{
					CRuneStoneDoc *pDoc = (CRuneStoneDoc*)GetDocument();
					for(int y = (m_MouseY-m_SelectDlgOpSize/2); y <= (m_MouseY+m_SelectDlgOpSize/2); y++)
					{
						for(int x = (m_MouseX-m_SelectDlgOpSize/2); x <= (m_MouseX+m_SelectDlgOpSize/2); x++)
						{
							if(x >= 0 && y >= 0 && x < pDoc->m_SizeWidthDoc && y < pDoc->m_SizeHeightDoc)
								InputMapData(x,y,m_SelectDlgTrinID);
						}
					}
				}
			}

			// *플러스 브러시 그리기
			else if(m_SelectDlgBrush == 2)
			{
				if(m_SelectDlgOpSize == 1)
				{
					POINT PlusPoint[]={{(RealPos.x)*48, (RealPos.y)*48}, {(RealPos.x)*48, (RealPos.y-1)*48}, {(RealPos.x+1)*48, (RealPos.y-1)*48}, {(RealPos.x+1)*48, (RealPos.y)*48}, 
					{(RealPos.x+2)*48, (RealPos.y)*48}, {(RealPos.x+2)*48, (RealPos.y+1)*48}, {(RealPos.x+1)*48, (RealPos.y+1)*48}, {(RealPos.x+1)*48, (RealPos.y+2)*48}, 
					{(RealPos.x)*48, (RealPos.y+2)*48}, {(RealPos.x)*48, (RealPos.y+1)*48}, {(RealPos.x-1)*48, (RealPos.y+1)*48}, {(RealPos.x-1)*48, (RealPos.y)*48}, {(RealPos.x)*48, (RealPos.y)*48}};
					dc.Polygon(PlusPoint, 13);
				}
				
				else if(m_SelectDlgOpSize == 3)
				{
					POINT PlusPoint[]={{(RealPos.x*48), (RealPos.y-2)*48}, {(RealPos.x+1)*48, (RealPos.y-2)*48}, {(RealPos.x+1)*48, (RealPos.y-1)*48}, {(RealPos.x+2)*48, (RealPos.y-1)*48}
					, {(RealPos.x+2)*48, (RealPos.y)*48}, {(RealPos.x+3)*48, (RealPos.y)*48}, {(RealPos.x+3)*48, (RealPos.y+1)*48}, {(RealPos.x+2)*48, (RealPos.y+1)*48}, {(RealPos.x+2)*48, (RealPos.y+2)*48}, {(RealPos.x+1)*48, (RealPos.y+2)*48}
					, {(RealPos.x+1)*48, (RealPos.y+3)*48}, {(RealPos.x)*48, (RealPos.y+3)*48}, {(RealPos.x)*48, (RealPos.y+2)*48}, {(RealPos.x-1)*48, (RealPos.y+2)*48}, {(RealPos.x-1)*48, (RealPos.y+1)*48}
					, {(RealPos.x-2)*48, (RealPos.y+1)*48}, {(RealPos.x-2)*48, (RealPos.y)*48}, {(RealPos.x-1)*48, (RealPos.y)*48}, {(RealPos.x-1)*48, (RealPos.y-1)*48}, {(RealPos.x)*48, (RealPos.y-1)*48}, {(RealPos.x)*48, (RealPos.y-2)*48}};
					dc.Polygon(PlusPoint, 21);
				}
				
				else if(m_SelectDlgOpSize == 5)
				{
					POINT PlusPoint[]={{(RealPos.x*48), (RealPos.y-3)*48}, {(RealPos.x+1)*48, (RealPos.y-3)*48}, {(RealPos.x+1)*48, (RealPos.y-2)*48}, {(RealPos.x+2)*48, (RealPos.y-2)*48}, {(RealPos.x+2)*48, (RealPos.y-1)*48}, {(RealPos.x+3)*48, (RealPos.y-1)*48}
					, {(RealPos.x+3)*48, (RealPos.y)*48}, {(RealPos.x+4)*48, (RealPos.y)*48}, {(RealPos.x+4)*48, (RealPos.y+1)*48}, {(RealPos.x+3)*48, (RealPos.y+1)*48}, {(RealPos.x+3)*48, (RealPos.y+2)*48}, {(RealPos.x+2)*48, (RealPos.y+2)*48}, {(RealPos.x+2)*48, (RealPos.y+3)*48}
					, {(RealPos.x+1)*48, (RealPos.y+3)*48}, {(RealPos.x+1)*48, (RealPos.y+4)*48}, {(RealPos.x)*48, (RealPos.y+4)*48}, {(RealPos.x)*48, (RealPos.y+3)*48}, {(RealPos.x-1)*48, (RealPos.y+3)*48}, {(RealPos.x-1)*48, (RealPos.y+2)*48}, {(RealPos.x-2)*48, (RealPos.y+2)*48}
					, {(RealPos.x-2)*48, (RealPos.y+1)*48}, {(RealPos.x-3)*48, (RealPos.y+1)*48}, {(RealPos.x-3)*48, (RealPos.y)*48}, {(RealPos.x-2)*48, (RealPos.y)*48}, {(RealPos.x-2)*48, (RealPos.y-1)*48}, {(RealPos.x-1)*48, (RealPos.y-1)*48}, {(RealPos.x-1)*48, (RealPos.y-2)*48}
					, {(RealPos.x)*48, (RealPos.y-2)*48}, {(RealPos.x)*48, (RealPos.y-3)*48}};
					dc.Polygon(PlusPoint, 29);
				}
				
				else if(m_SelectDlgOpSize == 7)
				{
					POINT PlusPoint[]={{(RealPos.x*48), (RealPos.y-4)*48}, {(RealPos.x+1)*48, (RealPos.y-4)*48}, {(RealPos.x+1)*48, (RealPos.y-3)*48}, {(RealPos.x+2)*48, (RealPos.y-3)*48}, {(RealPos.x+2)*48, (RealPos.y-2)*48}, {(RealPos.x+3)*48, (RealPos.y-2)*48}, {(RealPos.x+3)*48, (RealPos.y-1)*48}
					, {(RealPos.x+4)*48, (RealPos.y-1)*48}, {(RealPos.x+4)*48, (RealPos.y*48)}, {(RealPos.x+5)*48, RealPos.y*48}, {(RealPos.x+5)*48, (RealPos.y+1)*48}, {(RealPos.x+4)*48, (RealPos.y+1)*48}, {(RealPos.x+4)*48, (RealPos.y+2)*48}, {(RealPos.x+3)*48, (RealPos.y+2)*48}, {(RealPos.x+3)*48, (RealPos.y+3)*48}
					, {(RealPos.x+2)*48, (RealPos.y+3)*48}, {(RealPos.x+2)*48, (RealPos.y+4)*48}, {(RealPos.x+1)*48, (RealPos.y+4)*48}, {(RealPos.x+1)*48, (RealPos.y+5)*48}, {(RealPos.x*48), (RealPos.y+5)*48}, {RealPos.x*48, (RealPos.y+4)*48}, {(RealPos.x-1)*48, (RealPos.y+4)*48}, {(RealPos.x-1)*48, (RealPos.y+3)*48}
					, {(RealPos.x-2)*48, (RealPos.y+3)*48}, {(RealPos.x-2)*48, (RealPos.y+2)*48}, {(RealPos.x-3)*48, (RealPos.y+2)*48}, {(RealPos.x-3)*48, (RealPos.y+1)*48}, {(RealPos.x-4)*48, (RealPos.y+1)*48}, {(RealPos.x-4)*48, (RealPos.y*48)}, {(RealPos.x-3)*48, (RealPos.y*48)}, {(RealPos.x-3)*48, (RealPos.y-1)*48}
					, {(RealPos.x-2)*48, (RealPos.y-1)*48}, {(RealPos.x-2)*48, (RealPos.y-2)*48}, {(RealPos.x-1)*48, (RealPos.y-2)*48}, {(RealPos.x-1)*48, (RealPos.y-3)*48}, {(RealPos.x*48), (RealPos.y-3)*48}, {(RealPos.x*48), (RealPos.y-4)*48}};
					dc.Polygon(PlusPoint, 37);
				}
				if(m_IsMouseClicked)
				{
					PlusBrushDraw(m_SelectDlgOpSize/2+1, m_SelectDlgTrinID, m_MouseX, m_MouseY);
				}
			}

			// *라인 브러시 그리기
			else if(m_SelectDlgBrush == 3)
			{
				if(m_SelectDlgOpType == 1 && m_SelectDlgOpSize > 0)
					dc.Rectangle((m_MouseX-m_SelectDlgOpSize/2)*48, m_MouseY*48, (m_MouseX+m_SelectDlgOpSize/2)*48+49, m_MouseY*48+49);
				else if(m_SelectDlgOpType == 2 && m_SelectDlgOpSize > 0)
					dc.Rectangle(m_MouseX*48, (m_MouseY-m_SelectDlgOpSize/2)*48, m_MouseX*48+49, (m_MouseY+m_SelectDlgOpSize/2)*48+49);
				else if(m_SelectDlgOpType == 3 && m_SelectDlgOpSize > 0)
					for(int i=(-1)*(m_SelectDlgOpSize/2); i<=m_SelectDlgOpSize/2;i++)
						dc.Rectangle((m_MouseX+i)*48, (m_MouseY+i)*48, (m_MouseX+i)*48+49, (m_MouseY+i)*48+49);
				else if(m_SelectDlgOpType == 4 && m_SelectDlgOpSize > 0)
					for(int i=(-1)*(m_SelectDlgOpSize/2); i<=m_SelectDlgOpSize/2;i++)
						dc.Rectangle((m_MouseX+i)*48, (m_MouseY-i)*48, (m_MouseX+i)*48+49, (m_MouseY-i)*48+49);

				if(m_IsMouseClicked)
				{
					LineBrushDraw(m_SelectDlgOpType,m_SelectDlgTrinID,m_MouseX,m_MouseY,m_SelectDlgOpSize);
				}
			}
			
			// *채우기 브러시 그리기
			else if(m_SelectDlgBrush == 4)
			{
				dc.Rectangle(m_MouseX*48, m_MouseY*48, m_MouseX*48+49, m_MouseY*48+49);
			}

			// *지우개 브러시 그리기
			else if(m_SelectDlgBrush == 5)
			{
				if(m_SelectDlgOpSize > 0 && m_SelectDlgOpSize < 8)
					dc.Rectangle((m_MouseX-m_SelectDlgOpSize/2)*48, (m_MouseY-m_SelectDlgOpSize/2)*48, (m_MouseX+m_SelectDlgOpSize/2)*48+49, (m_MouseY+m_SelectDlgOpSize/2)*48+49);
				
				if(m_IsMouseClicked)
				{
					CRuneStoneDoc *pDoc = (CRuneStoneDoc*)GetDocument();
					for(int y = (m_MouseY-m_SelectDlgOpSize/2); y <= (m_MouseY+m_SelectDlgOpSize/2); y++)
					{
						for(int x = (m_MouseX-m_SelectDlgOpSize/2); x <= (m_MouseX+m_SelectDlgOpSize/2); x++)
						{
							if(x >= 0 && y >= 0 && x < pDoc->m_SizeWidthDoc && y < pDoc->m_SizeHeightDoc)
							{
								InputMapData(x,y,0);
							}
						}
					}
				}
			}
			
			// *지형찾기 브러시 그리기
			else if(m_SelectDlgBrush == 6)
			{
				dc.Rectangle(m_MouseX*48, m_MouseY*48, m_MouseX*48+49, m_MouseY*48+49);
			}			
		} // end 지형 브러시 선택영역을 표시하기 위한 부분

		// *구역 생성도중을 그리는 부분
		if(m_SelectDlgLoc == CRELOC)
		{
			if(m_IsMouseClicked == TRUE)
			{
				m_MouseDragX = m_MouseX;
				m_MouseDragY = m_MouseY;
			}
		}

		Invalidate(FALSE);
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CRuneStoneView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();

	m_IsMouseClicked = FALSE;
	
	if(m_SelectDlgLoc == CRELOC)
	{
		m_LocEndX=0;
		m_LocEndY=0;
		
		if(m_SelectX >= m_MouseDragX)
		{
			m_LocEndX = m_SelectX;
			m_LocStartX = m_MouseDragX;
		}

		else
		{
			m_LocStartX = m_SelectX;
			m_LocEndX = m_MouseDragX;
		}
		
		if(m_SelectY >= m_MouseDragY)
		{
			m_LocEndY = m_SelectY;
			m_LocStartY = m_MouseDragY;
		}

		else
		{
			m_LocStartY = m_SelectY;
			m_LocEndY = m_MouseDragY;
		}
		
		if(pMainFrm->m_pMiniDlg != NULL)
			pMainFrm->m_pMiniDlg->CMinimapDialog::Invalidate(FALSE);

		if(m_SelectDlgLoc == CRELOC)
		{
			int IndexID = 0;

			if(pDoc->m_LocationDoc.GetSize() == 0)
			{
				CString str;
				str.Format("구역 %d", pDoc->m_LocationDoc.GetSize());
				pDoc->m_LocationDoc.InsertAt(pDoc->m_LocationDoc.GetSize(), Location(m_LocStartX, m_LocStartY, m_LocEndX, m_LocEndY,
											 str, pDoc->m_LocationDoc.GetSize(), BLUE));

				pMainFrm->m_pLocDlg->m_LocListBox.AddString(str);
			}

			else
			{
				int ArraySize = pDoc->m_LocationDoc.GetSize();
				
				for(int i=0; i<ArraySize; i++)
				{
					if(IndexID != pDoc->m_LocationDoc[i].LocationID)
					{
						CString str;
						str.Format("구역 %d", IndexID);
						pDoc->m_LocationDoc.InsertAt(IndexID, Location(m_LocStartX, m_LocStartY, m_LocEndX, m_LocEndY,
													 str, IndexID, BLUE));
						
						pMainFrm->m_pLocDlg->m_LocListBox.AddString(str);
						break;
					}

					else if(IndexID == pDoc->m_LocationDoc.GetSize()-1)
					{
						CString str;
						str.Format("구역 %d", pDoc->m_LocationDoc.GetSize());
						pDoc->m_LocationDoc.InsertAt(pDoc->m_LocationDoc.GetSize(), Location(m_LocStartX, m_LocStartY, m_LocEndX, m_LocEndY,
							str, pDoc->m_LocationDoc.GetSize(), BLUE));
						
						pMainFrm->m_pLocDlg->m_LocListBox.AddString(str);
						break;
					}

					IndexID++;
				}
			}
		}
	}
	
	Invalidate(FALSE);
	
	CScrollView::OnLButtonUp(nFlags, point);
}
