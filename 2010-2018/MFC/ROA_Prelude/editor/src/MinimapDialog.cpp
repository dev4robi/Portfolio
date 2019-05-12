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
	GetWindowRect(&MiniMapDlgRect); // ���̾�α� ��ġ ���� ���ϱ�

	for(int i=0; i<pDoc->m_SizeHeightDoc; i++)
	{
		for(int j=0; j<pDoc->m_SizeWidthDoc; j++)
		{
			// *�̵� ���ɽ� ��� ĥ��
			if(pDoc->m_MapDataDoc[(i*(pDoc->m_SizeWidthDoc))+j].TrinMove == TRUE)
			{
				dc.SetPixelV(((MiniMapDlgRect.Width()-18)/2-pDoc->m_SizeWidthDoc/2)+5+j,((MiniMapDlgRect.Height()-42)/2-pDoc->m_SizeHeightDoc/2)+5+i,RGB(255, 255, 255));
			}
			
			// *�̵� �Ұ��ɽ� ������ ĥ��
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
	
	// *�޸� ��ȯ �� ������ �缳��(�� �۵��� �� �������� �־�� ��)
	m_pMiniMainFrame->m_pMiniDlg = NULL;
	DestroyWindow();
	
	delete this;
}

BOOL CMinimapDialog::PreTranslateMessage(MSG* pMsg) 
{
	// *�ش� Ű �޽��� ��ȿȭ
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
	
	// *���� �� ���̾�α� ũ�� ����
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();
	
	CRect rect;
	GetWindowRect(&rect); // ���̾�α� ��ġ ���� ���ϱ�
	if(pDoc->m_SizeWidthDoc > 256 || pDoc->m_SizeHeightDoc > 256)
		MoveWindow(rect.left, rect.top, 512+18, 512+42, TRUE);
	else
		MoveWindow(rect.left, rect.top, 256+18, 256+42, TRUE);
	
	return 0;
}
