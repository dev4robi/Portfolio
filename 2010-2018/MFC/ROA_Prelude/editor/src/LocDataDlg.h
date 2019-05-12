#if !defined(AFX_LOCDATADLG_H__52231181_E50E_4633_A285_0DD5019E4E26__INCLUDED_)
#define AFX_LOCDATADLG_H__52231181_E50E_4633_A285_0DD5019E4E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLocDataDlg dialog

class CLocDataDlg : public CDialog
{
// Construction
public:
	CLocDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLocDataDlg)
	enum { IDD = IDD_LOCDATADIG };
	CString	m_LocName;
	int		m_LocColor;
	int		m_LocID;
	int		m_LocX1;
	int		m_LocX2;
	int		m_LocY2;
	int		m_LocY1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLocDataDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCDATADLG_H__52231181_E50E_4633_A285_0DD5019E4E26__INCLUDED_)
