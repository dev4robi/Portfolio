#if !defined(AFX_ITEMDIALOG_H__C8EB05BC_BFF1_4581_9D46_031C65DF38A9__INCLUDED_)
#define AFX_ITEMDIALOG_H__C8EB05BC_BFF1_4581_9D46_031C65DF38A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemDIALOG.h : header file
//

// *참조하는 클래스
class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CItemDIALOG dialog

class CItemDIALOG : public CDialog
{
// Construction
public:
	CItemDIALOG(CWnd* pParent = NULL);   // standard constructor

	// *MainFrame클래스를 가리키는 포인터
	CMainFrame *m_pItemMainFrame;

// Dialog Data
	//{{AFX_DATA(CItemDIALOG)
	enum { IDD = IDD_ITEMDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDIALOG)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemDIALOG)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMDIALOG_H__C8EB05BC_BFF1_4581_9D46_031C65DF38A9__INCLUDED_)
