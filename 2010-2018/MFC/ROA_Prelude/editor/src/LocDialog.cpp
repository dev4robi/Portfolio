// LocDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "LocDialog.h"

// *�߰��� ��� ����
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

void LocDialog::OnClose() // *���̾�α� �ı���
{
	// *�޸� ��ȯ �� ������ �缳�� �� ���� �����
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

BOOL LocDialog::PreTranslateMessage(MSG* pMsg) // *������ �޽��� �������� �˻�
{
	// *�ش� Ű �޽��� ��ȿȭ
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void LocDialog::OnSelloc() // *���� ���� ���� ��ư Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	pView->m_SelectDlgLoc = SELLOC;
}

void LocDialog::OnCreloc() // *���� ���� ���� ��ư Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgLoc = CRELOC;
}

void LocDialog::OnDelloc() // *���� ���� ��ư Ŭ����
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
		LocName += " ��(��) ������ �����Ͻðڽ��ϱ�?";
		
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
		AfxMessageBox("���õ� ������ �����ϴ�.");
}
