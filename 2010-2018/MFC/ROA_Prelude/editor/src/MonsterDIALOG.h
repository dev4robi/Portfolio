#if !defined(AFX_MONSTERDIALOG_H__85755C87_463D_486C_B143_D7193BABE062__INCLUDED_)
#define AFX_MONSTERDIALOG_H__85755C87_463D_486C_B143_D7193BABE062__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterDIALOG.h : header file
//

 // *참조하는 클래스
class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CMonsterDIALOG dialog

class CMonsterDIALOG : public CDialog
{
// Construction
public:
	CMonsterDIALOG(CWnd* pParent = NULL);   // standard constructor

	// *MainFrame클래스를 가리키는 포인터
	CMainFrame *m_pMonMainFrame;

// Dialog Data
	//{{AFX_DATA(CMonsterDIALOG)
	enum { IDD = IDD_MONDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterDIALOG)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterDIALOG)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERDIALOG_H__85755C87_463D_486C_B143_D7193BABE062__INCLUDED_)
