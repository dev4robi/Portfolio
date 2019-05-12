// NpcDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "NpcDIALOG.h"

// *추가된 헤더 파일
#include "RuneStoneDoc.h"
#include "MainFrm.h"
#include "RuneStoneView.h"
#include "LocDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNpcDIALOG dialog


CNpcDIALOG::CNpcDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNpcDIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNpcDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNpcDIALOG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcDIALOG, CDialog)
	//{{AFX_MSG_MAP(CNpcDIALOG)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNpcDIALOG message handlers

void CNpcDIALOG::OnClose() // *다이얼로그 파괴시
{
	// *메모리 반환 및 포인터 재설정
	m_pNpcMainFrame->m_pNpcDlg = NULL;
	DestroyWindow();
		
	delete this;
}

BOOL CNpcDIALOG::PreTranslateMessage(MSG* pMsg) // *윈도우 메시지 전달전에 검사
{
	// *해당 키 메시지 무효화
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CNpcDIALOG::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	// *구역 다이얼로그 파괴
	if(pMainFrm->m_pLocDlg != NULL)
	{
		pView->m_ShowLoc = FALSE;
		pView->m_SelectDlgLoc = -1;	
		pMainFrm->m_pLocDlg->m_pLocMainFrame = NULL;
		pMainFrm->m_pLocDlg->DestroyWindow();
		pMainFrm->m_pLocDlg = NULL;
	}
	
	return 0;
}
