// TileSetView.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "TileSetView.h"

// *�߰��� �ش� ����
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
	// *��ü�� ������ �� ��ũ���� ����
	CSize sizeTotal;
	sizeTotal.cx = 390;
	sizeTotal.cy = 4115;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// *�ʱ� ������ Ÿ��
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
	
	// *���̾�α� �� ������ Ÿ�� ��Ʈ �׸��� (���� ���۸� ���)
	for(int i=0; i<672; i++)
	{
		pDC->BitBlt((((i%8)*48)+(i%8)), (((i/8)*48)+(i/8)), 48, 48, &MemDC, ((i%16)*48), ((i/16)*48), SRCCOPY);
	}

	// *���̾�α� �� ������ ���õǾ� ������
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

int CTileSetView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) // *���콺 ��� ������ Ȱ��ȭ ��
{
	// *������� Ŭ������ �Ϻ� �����Ͱ� �����Ƿ� Ŭ������ CWnd�� ������ ��� (���� �������� ��.)
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CTileSetView::PreCreateWindow(CREATESTRUCT& cs) // *DlgView���� ����� ����
{
	// *�� ���� �ʱ� ���� ���������� ����
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, LoadCursor(NULL, IDC_ARROW),
				   (HBRUSH) GetStockObject(BLACK_BRUSH), LoadIcon(NULL, IDI_APPLICATION));
	
	return CScrollView::PreCreateWindow(cs);
}

int CTileSetView::OnCreate(LPCREATESTRUCT lpCreateStruct) // *�� ������ ������ ��
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// *�޸𸮿� Ÿ�ϼ� ��Ʈ�� ����
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

void CTileSetView::OnLButtonDown(UINT nFlags, CPoint point) // *���̾�α� �信 ���� ���콺 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	// *���̾�α� �� ������ ���� Ŭ����
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
