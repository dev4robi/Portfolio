// FileOpenDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "FileOpenDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileOpenDialog dialog


CFileOpenDialog::CFileOpenDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFileOpenDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileOpenDialog)
	m_FileName = _T("");
	//}}AFX_DATA_INIT
}


void CFileOpenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileOpenDialog)
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_FileName);
	DDV_MaxChars(pDX, m_FileName, 12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileOpenDialog, CDialog)
	//{{AFX_MSG_MAP(CFileOpenDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileOpenDialog message handlers

void CFileOpenDialog::OnOK() // *열기 버튼 누를시
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}
