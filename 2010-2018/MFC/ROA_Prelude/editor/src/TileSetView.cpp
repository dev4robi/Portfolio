// TileSetView.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "TileSetView.h"

// *추가된 해더 파일
#include "MainFrm.h"
#include "RuneStoneView.h"
#include "RuneStoneDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTileSetView

IMPLEMENT_DYNCREATE(CTileSetView, CScrollView)

CTileSetView::CTileSetView()
{
	// *객체가 생성될 때 스크롤을 만듬
	CSize sizeTotal;
	sizeTotal.cx = 390;
	sizeTotal.cy = 4115;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// *초기 선택한 타일
	m_DlgSelectX = m_DlgSelectY = 0;
}

CTileSetView::~CTileSetView()
{
}


BEGIN_MESSAGE_MAP(CTileSetView, CScrollView)
	//{{AFX_MSG_MAP(CTileSetView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTileSetView drawing

void CTileSetView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
}

void CTileSetView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();

	CClientDC dc(this);
	OnPrepareDC(&dc);
	
	// *다이얼로그 뷰 영역에 타일 세트 그리기 (더블 버퍼링 사용)
	for(int i=0; i<672; i++)
	{
		pDC->BitBlt((((i%8)*48)+(i%8)), (((i/8)*48)+(i/8)), 48, 48, &MemDC, ((i%16)*48), ((i/16)*48), SRCCOPY);
	}

	// *다이얼로그 뷰 영역이 선택되어 있으면
	CPen SelectPen(PS_INSIDEFRAME, 4, RGB(255, 255, 0));
	dc.SelectStockObject(NULL_BRUSH);
	dc.SelectObject(&SelectPen);
	dc.Rectangle(m_DlgSelectX*48+m_DlgSelectX, m_DlgSelectY*48+m_DlgSelectY,
				 m_DlgSelectX*48+48+m_DlgSelectX, m_DlgSelectY*48+48+m_DlgSelectY);
}

/////////////////////////////////////////////////////////////////////////////
// CTileSetView diagnostics

#ifdef _DEBUG
void CTileSetView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTileSetView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTileSetView message handlers

int CTileSetView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) // *마우스 어떠한 종류든 활성화 시
{
	// *만들어진 클래스에 일부 데이터가 없으므로 클래스를 CWnd로 돌려서 사용 (문제 있을지도 모름.)
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CTileSetView::PreCreateWindow(CREATESTRUCT& cs) // *DlgView영역 만들기 전에
{
	// *뷰 영역 초기 색상 검은색으로 변경
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, LoadCursor(NULL, IDC_ARROW),
				   (HBRUSH) GetStockObject(BLACK_BRUSH), LoadIcon(NULL, IDI_APPLICATION));
	
	return CScrollView::PreCreateWindow(cs);
}

int CTileSetView::OnCreate(LPCREATESTRUCT lpCreateStruct) // *뷰 영역을 생성할 때
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// *메모리에 타일셋 비트맵 연동
	CClientDC dc(this);

	MapDC.CreateCompatibleDC(&dc);
	MapBit.LoadBitmap(IDB_TILESET);
	MapDC.SelectObject(&MapBit);
	MemDC.CreateCompatibleDC(&dc);
	MemBit.CreateCompatibleBitmap(&dc, 768, 2016);
	MemDC.SelectObject(&MemBit);
	MemDC.BitBlt(0, 0, 768, 2016, &MapDC, 0, 0, SRCCOPY);
	
	return 0;
}

void CTileSetView::OnLButtonDown(UINT nFlags, CPoint point) // *다이얼로그 뷰에 왼쪽 마우스 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	// *다이얼로그 뷰 영역의 지형 클릭시
	CClientDC dc(this);

	CPoint RealPos = GetScrollPosition();
	RealPos.x += point.x;
	RealPos.y += point.y;
	RealPos.x = RealPos.x/48;
	RealPos.y = RealPos.y/48;
	m_DlgSelectX = RealPos.x;
	m_DlgSelectY = RealPos.y;

	RealPos = GetScrollPosition();
	RealPos.x = RealPos.x + point.x - m_DlgSelectX;
	RealPos.y = RealPos.y + point.y - m_DlgSelectY;
	RealPos.x = RealPos.x / 48;
	RealPos.y = RealPos.y / 48;

	m_DlgSelectX = RealPos.x;
	m_DlgSelectY = RealPos.y;
	
	pView->m_SelectDlgTrinID = m_DlgSelectX + m_DlgSelectY*8;

	Invalidate();

	CScrollView::OnLButtonDown(nFlags, point);
}
