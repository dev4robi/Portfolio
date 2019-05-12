// LocDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "LocDataDlg.h"

// *추가된 해더 파일
#include "MainFrm.h"
#include "RuneStoneDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocDataDlg dialog


CLocDataDlg::CLocDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLocDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocDataDlg)
	m_LocName = _T("");
	m_LocColor = 0;
	m_LocID = 0;
	m_LocX1 = 0;
	m_LocX2 = 0;
	m_LocY2 = 0;
	m_LocY1 = 0;
	//}}AFX_DATA_INIT
}


void CLocDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocDataDlg)
	DDX_Text(pDX, IDC_LOCNAME_EDIT, m_LocName);
	DDV_MaxChars(pDX, m_LocName, 20);
	DDX_Text(pDX, IDC_LOCCOLOR_EDIT, m_LocColor);
	DDV_MinMaxInt(pDX, m_LocColor, 0, 4);
	DDX_Text(pDX, IDC_LOCID_EDIT, m_LocID);
	DDX_Text(pDX, IDC_LOCX1_EDIT, m_LocX1);
	DDX_Text(pDX, IDC_LOCX2_EDIT, m_LocX2);
	DDX_Text(pDX, IDC_LOCY2_EDIT, m_LocY2);
	DDX_Text(pDX, IDC_LOCY1_EDIT, m_LocY1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocDataDlg, CDialog)
	//{{AFX_MSG_MAP(CLocDataDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocDataDlg message handlers

BOOL CLocDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();
	
	for(int i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
	{
		if(pDoc->m_LocationDoc[i].LocationSelected == TRUE)
		{
			m_LocID = pDoc->m_LocationDoc[i].LocationID;
			m_LocColor = pDoc->m_LocationDoc[i].LocationColor;
			m_LocName = pDoc->m_LocationDoc[i].LocationName;
			m_LocX1 = pDoc->m_LocationDoc[i].LocationX1;
			m_LocY1 = pDoc->m_LocationDoc[i].LocationY1;
			m_LocX2 = pDoc->m_LocationDoc[i].LocationX2;
			m_LocY2 = pDoc->m_LocationDoc[i].LocationY2;
			UpdateData(FALSE);
			break;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
