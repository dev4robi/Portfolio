#if !defined(AFX_DATADIALOG_H__ED9A75D5_1601_4FB7_83C6_1ED4DF8600E3__INCLUDED_)
#define AFX_DATADIALOG_H__ED9A75D5_1601_4FB7_83C6_1ED4DF8600E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DataDialog dialog

class DataDialog : public CDialog
{
// Construction
public:
	DataDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DataDialog)
	enum { IDD = IDD_DATADIALOG };
	int		m_TrinNextIDDataDlg;
	int		m_TrinHighDataDlg;
	int		m_TrinIDDataDlg;
	BOOL	m_TrinMoveDataDlg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DataDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DataDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADIALOG_H__ED9A75D5_1601_4FB7_83C6_1ED4DF8600E3__INCLUDED_)
