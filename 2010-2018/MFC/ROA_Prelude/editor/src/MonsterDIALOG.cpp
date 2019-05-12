// MonsterDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "MonsterDIALOG.h"

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
// CMonsterDIALOG dialog


CMonsterDIALOG::CMonsterDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterDIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMonsterDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterDIALOG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterDIALOG, CDialog)
	//{{AFX_MSG_MAP(CMonsterDIALOG)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterDIALOG message handlers

void CMonsterDIALOG::OnClose() // *다이얼로그 파괴시
{
	// *메모리 반환 및 포인터 재설정
	m_pMonMainFrame->m_pMonDlg = NULL;
	DestroyWindow();
	
	delete this;
}

BOOL CMonsterDIALOG::PreTranslateMessage(MSG* pMsg) // *윈도우 메시지 전달전에 검사
{
	// *해당 키 메시지 무효화
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CMonsterDIALOG::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
