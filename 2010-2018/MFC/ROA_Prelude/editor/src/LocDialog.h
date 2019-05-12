#if !defined(AFX_LOCDIALOG_H__7B65CFF9_0BC5_4C2E_9BD6_DD8EFD5D4A7B__INCLUDED_)
#define AFX_LOCDIALOG_H__7B65CFF9_0BC5_4C2E_9BD6_DD8EFD5D4A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LocDialog dialog

// *참조하는 클래스
class CMainFrame;

class LocDialog : public CDialog
{
// Construction
public:
	LocDialog(CWnd* pParent = NULL);   // standard constructor

	// *MainFrame클래스를 가리키는 포인터
	CMainFrame *m_pLocMainFrame;

// Dialog Data
	//{{AFX_DATA(LocDialog)
	enum { IDD = IDD_LOCATEDIALOG };
	CListBox	m_LocListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LocDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LocDialog)
	afx_msg void OnClose();
	afx_msg void OnSelloc();
	afx_msg void OnCreloc();
	afx_msg void OnDelloc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCDIALOG_H__7B65CFF9_0BC5_4C2E_9BD6_DD8EFD5D4A7B__INCLUDED_)
