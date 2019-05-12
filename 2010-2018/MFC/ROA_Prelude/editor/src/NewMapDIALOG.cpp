// NewMapDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "NewMapDIALOG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewMapDIALOG dialog


CNewMapDIALOG::CNewMapDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMapDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewMapDIALOG)
	m_SizeHeight = 0;
	m_SizeWidth = 0;
	m_FileName = _T("Map000");
	//}}AFX_DATA_INIT
}


void CNewMapDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewMapDIALOG)
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_SizeHeight);
	DDV_MinMaxInt(pDX, m_SizeHeight, 32, 512);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_SizeWidth);
	DDV_MinMaxInt(pDX, m_SizeWidth, 32, 512);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_FileName);
	DDV_MaxChars(pDX, m_FileName, 12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewMapDIALOG, CDialog)
	//{{AFX_MSG_MAP(CNewMapDIALOG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewMapDIALOG message handlers

BOOL CNewMapDIALOG::OnInitDialog() // *다이얼로그 초기화
{
	CDialog::OnInitDialog();
	
	// 변수에서 에딧박스 등으로 데이터 전송
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
