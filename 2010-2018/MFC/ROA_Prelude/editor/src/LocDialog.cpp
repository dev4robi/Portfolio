// LocDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "LocDialog.h"

// *추가된 헤더 파일
#include "RuneStoneDoc.h"
#include "MainFrm.h"
#include "RuneStoneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LocDialog dialog


LocDialog::LocDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LocDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(LocDialog)
	//}}AFX_DATA_INIT
}


void LocDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LocDialog)
	DDX_Control(pDX, IDC_LOCLIST, m_LocListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LocDialog, CDialog)
	//{{AFX_MSG_MAP(LocDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SELLOC, OnSelloc)
	ON_BN_CLICKED(IDC_CRELOC, OnCreloc)
	ON_BN_CLICKED(IDC_DELLOC, OnDelloc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LocDialog message handlers

void LocDialog::OnClose() // *다이얼로그 파괴시
{
	// *메모리 반환 및 포인터 재설정 및 구역 숨기기
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();
	
	for(int i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
		m_LocListBox.DeleteString(i);

	pView->m_ShowLoc = FALSE;
	pView->m_SelectDlgLoc = NONE;

	pView->m_LocStartX = -1;
	pView->m_LocStartY = -1;
	pView->m_SelectX = 0;
	pView->m_SelectY = 0;
	pView->m_MouseDragX = -1;
	pView->m_MouseDragY = -1;

	m_pLocMainFrame->m_pLocDlg = NULL;
	pView->Invalidate(FALSE);
	DestroyWindow();

	delete this;
}

BOOL LocDialog::PreTranslateMessage(MSG* pMsg) // *윈도우 메시지 전달전에 검사
{
	// *해당 키 메시지 무효화
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void LocDialog::OnSelloc() // *구역 선택 라디오 버튼 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	pView->m_SelectDlgLoc = SELLOC;
}

void LocDialog::OnCreloc() // *구역 생성 라디오 버튼 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgLoc = CRELOC;
}

void LocDialog::OnDelloc() // *구역 삭제 버튼 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();

	pView->m_SelectDlgLoc = NONE;
	
	int ClickLoc = m_LocListBox.GetCurSel();
	
	if(ClickLoc != LB_ERR)
	{
		CString LocName;
		m_LocListBox.GetText(ClickLoc, LocName);
		LocName += " 을(를) 정말로 삭제하시겠습니까?";
		
		if(IDOK == AfxMessageBox(LocName, MB_ICONEXCLAMATION | MB_OKCANCEL))
		{
			m_LocListBox.GetText(ClickLoc, LocName);

			for(int i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
			{
				if(pDoc->m_LocationDoc.GetAt(i).LocationName == LocName)
					pDoc->m_LocationDoc.RemoveAt(i);
			}

			m_LocListBox.DeleteString(ClickLoc);
			m_LocListBox.SetCurSel(ClickLoc);

			pView->Invalidate(FALSE);
		}
	}
	
	else
		AfxMessageBox("선택된 구역이 없습니다.");
}
