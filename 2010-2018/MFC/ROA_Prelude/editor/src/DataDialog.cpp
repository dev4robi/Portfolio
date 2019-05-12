// DataDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "DataDialog.h"

// *추가된 해더 파일
#include "MainFrm.h"
#include "RuneStoneDoc.h"
#include "RuneStoneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DataDialog dialog


DataDialog::DataDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DataDialog)
	m_TrinNextIDDataDlg = 0;
	m_TrinHighDataDlg = 0;
	m_TrinIDDataDlg = 0;
	m_TrinMoveDataDlg = FALSE;
	//}}AFX_DATA_INIT
}


void DataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DataDialog)
	DDX_Text(pDX, IDC_NEXTTRINID_EDIT, m_TrinNextIDDataDlg);
	DDV_MinMaxInt(pDX, m_TrinNextIDDataDlg, 0, 671);
	DDX_Text(pDX, IDC_TRINHIGH_EDIT, m_TrinHighDataDlg);
	DDV_MinMaxInt(pDX, m_TrinHighDataDlg, 0, 5);
	DDX_Text(pDX, IDC_TRINID_EDIT, m_TrinIDDataDlg);
	DDV_MinMaxInt(pDX, m_TrinIDDataDlg, 0, 671);
	DDX_Check(pDX, IDC_IDC_TRINMOVE_CHECK, m_TrinMoveDataDlg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DataDialog, CDialog)
	//{{AFX_MSG_MAP(DataDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DataDialog message handlers

BOOL DataDialog::OnInitDialog() // *다이얼로그 생성(초기화)시
{
	CDialog::OnInitDialog();
	
	// *전역 함수로 프레임을 선택후 프레임에서 다큐먼트와 뷰로 포인터를 이어서 데이터를 가져옴
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrame->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrame->GetActiveView();

	m_TrinIDDataDlg = pDoc->m_MapDataDoc[pView->m_SelectX+(pView->m_SelectY*(pDoc->m_SizeWidthDoc))].TrinID;
	m_TrinNextIDDataDlg = pDoc->m_MapDataDoc[pView->m_SelectX+(pView->m_SelectY*(pDoc->m_SizeWidthDoc))].TrinNextID;
	m_TrinHighDataDlg = pDoc->m_MapDataDoc[pView->m_SelectX+(pView->m_SelectY*(pDoc->m_SizeWidthDoc))].TrinHigh;
	m_TrinMoveDataDlg = pDoc->m_MapDataDoc[pView->m_SelectX+(pView->m_SelectY*(pDoc->m_SizeWidthDoc))].TrinMove;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
