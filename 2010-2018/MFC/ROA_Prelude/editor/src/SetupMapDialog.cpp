// SetupMapDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "SetupMapDialog.h"

// *추가된 해더 파일
#include "MainFrm.h"
#include "RuneStoneDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupMapDialog dialog


CSetupMapDialog::CSetupMapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupMapDialog::IDD, pParent)
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();

	//{{AFX_DATA_INIT(CSetupMapDialog)
	m_FileName = pDoc->m_FileNameDoc;
	m_SizeHeight = pDoc->m_SizeHeightDoc;
	m_SizeWidth = pDoc->m_SizeWidthDoc;
	//}}AFX_DATA_INIT
}


void CSetupMapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupMapDialog)
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_FileName);
	DDV_MaxChars(pDX, m_FileName, 12);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_SizeHeight);
	DDV_MinMaxInt(pDX, m_SizeHeight, 32, 512);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_SizeWidth);
	DDV_MinMaxInt(pDX, m_SizeWidth, 32, 512);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupMapDialog, CDialog)
	//{{AFX_MSG_MAP(CSetupMapDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupMapDialog message handlers

BOOL CSetupMapDialog::OnInitDialog() // *다이얼로그 초기화
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
