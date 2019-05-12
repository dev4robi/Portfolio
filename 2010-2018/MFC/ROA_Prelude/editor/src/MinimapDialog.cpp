// MinimapDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "MinimapDialog.h"
#include "MainFrm.h"
#include "RuneStoneDoc.h"
#include "RuneStoneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMinimapDialog dialog


CMinimapDialog::CMinimapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMinimapDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMinimapDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMinimapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMinimapDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMinimapDialog, CDialog)
	//{{AFX_MSG_MAP(CMinimapDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMinimapDialog message handlers

void CMinimapDialog::OnPaint() 
{ 
	CPaintDC dc(this); // device context for painting
	
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();
	
	CRect MiniMapDlgRect;
	GetWindowRect(&MiniMapDlgRect); // 다이얼로그 위치 정보 구하기

	for(int i=0; i<pDoc->m_SizeHeightDoc; i++)
	{
		for(int j=0; j<pDoc->m_SizeWidthDoc; j++)
		{
			// *이동 가능시 흰색 칠함
			if(pDoc->m_MapDataDoc[(i*(pDoc->m_SizeWidthDoc))+j].TrinMove == TRUE)
			{
				dc.SetPixelV(((MiniMapDlgRect.Width()-18)/2-pDoc->m_SizeWidthDoc/2)+5+j,((MiniMapDlgRect.Height()-42)/2-pDoc->m_SizeHeightDoc/2)+5+i,RGB(255, 255, 255));
			}
			
			// *이동 불가능시 검은색 칠함
			else
			{
				dc.SetPixelV(((MiniMapDlgRect.Width()-18)/2-pDoc->m_SizeWidthDoc/2)+5+j,((MiniMapDlgRect.Height()-42)/2-pDoc->m_SizeHeightDoc/2)+5+i,RGB(0, 0, 0));
			}
		}
	}
}

void CMinimapDialog::OnClose() 
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	
	// *메모리 반환 및 포인터 재설정(이 작동은 맨 마지막에 있어야 함)
	m_pMiniMainFrame->m_pMiniDlg = NULL;
	DestroyWindow();
	
	delete this;
}

BOOL CMinimapDialog::PreTranslateMessage(MSG* pMsg) 
{
	// *해당 키 메시지 무효화
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

int CMinimapDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// *생성 후 다이얼로그 크기 변경
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();
	
	CRect rect;
	GetWindowRect(&rect); // 다이얼로그 위치 정보 구하기
	if(pDoc->m_SizeWidthDoc > 256 || pDoc->m_SizeHeightDoc > 256)
		MoveWindow(rect.left, rect.top, 512+18, 512+42, TRUE);
	else
		MoveWindow(rect.left, rect.top, 256+18, 256+42, TRUE);
	
	return 0;
}
