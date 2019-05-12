#if !defined(AFX_NPCDIALOG_H__1AE298F9_F26B_4079_9064_4FED1D79CE50__INCLUDED_)
#define AFX_NPCDIALOG_H__1AE298F9_F26B_4079_9064_4FED1D79CE50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NpcDIALOG.h : header file
//

 // *참조하는 클래스
class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CNpcDIALOG dialog

class CNpcDIALOG : public CDialog
{
// Construction
public:
	CNpcDIALOG(CWnd* pParent = NULL);   // standard constructor

	// *MainFrame클래스를 가리키는 포인터
	CMainFrame *m_pNpcMainFrame;

// Dialog Data
	//{{AFX_DATA(CNpcDIALOG)
	enum { IDD = IDD_NPCDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNpcDIALOG)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNpcDIALOG)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NPCDIALOG_H__1AE298F9_F26B_4079_9064_4FED1D79CE50__INCLUDED_)
