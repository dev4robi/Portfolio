#if !defined(AFX_NEWMAPDIALOG_H__FDE35BE9_1691_4BF7_A175_06A463240CD7__INCLUDED_)
#define AFX_NEWMAPDIALOG_H__FDE35BE9_1691_4BF7_A175_06A463240CD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewMapDIALOG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewMapDIALOG dialog

class CNewMapDIALOG : public CDialog
{
// Construction
public:
	CNewMapDIALOG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewMapDIALOG)
	enum { IDD = IDD_NEWMAP };
	int		m_SizeHeight;
	int		m_SizeWidth;
	CString	m_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewMapDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewMapDIALOG)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWMAPDIALOG_H__FDE35BE9_1691_4BF7_A175_06A463240CD7__INCLUDED_)
