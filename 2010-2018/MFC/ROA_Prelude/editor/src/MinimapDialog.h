#if !defined(AFX_MINIMAPDIALOG_H__31F901D3_9527_4CA2_9672_F924DA7E3615__INCLUDED_)
#define AFX_MINIMAPDIALOG_H__31F901D3_9527_4CA2_9672_F924DA7E3615__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MinimapDialog.h : header file
//

class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CMinimapDialog dialog

class CMinimapDialog : public CDialog
{
// Construction
public:
	CMinimapDialog(CWnd* pParent = NULL);   // standard constructor
	
	// *MainFrame클래스를 가리키는 포인터
	CMainFrame *m_pMiniMainFrame;

// Dialog Data
	//{{AFX_DATA(CMinimapDialog)
	enum { IDD = IDD_MINIDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinimapDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMinimapDialog)
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIMAPDIALOG_H__31F901D3_9527_4CA2_9672_F924DA7E3615__INCLUDED_)
