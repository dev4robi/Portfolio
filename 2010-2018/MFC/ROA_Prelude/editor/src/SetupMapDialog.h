#if !defined(AFX_SETUPMAPDIALOG_H__9B6CC43E_A470_48D4_84A9_97A22790F2EA__INCLUDED_)
#define AFX_SETUPMAPDIALOG_H__9B6CC43E_A470_48D4_84A9_97A22790F2EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupMapDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupMapDialog dialog

class CSetupMapDialog : public CDialog
{
// Construction
public:
	CSetupMapDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetupMapDialog)
	enum { IDD = IDD_SETUPMAP };
	CString	m_FileName;
	int		m_SizeHeight;
	int		m_SizeWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupMapDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupMapDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPMAPDIALOG_H__9B6CC43E_A470_48D4_84A9_97A22790F2EA__INCLUDED_)
