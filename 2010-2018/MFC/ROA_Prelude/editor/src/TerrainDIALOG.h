#if !defined(AFX_TERRAINDIALOG_H__4632C8AE_C4C5_45BD_887E_29398318F8DE__INCLUDED_)
#define AFX_TERRAINDIALOG_H__4632C8AE_C4C5_45BD_887E_29398318F8DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerrainDIALOG.h : header file
//

// *참조할 클래스 선언
class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CTerrainDIALOG dialog

class CTerrainDIALOG : public CDialog
{
// Construction
public:
	CTerrainDIALOG(CWnd* pParent = NULL);   // standard constructor

	// *MainFrame클래스를 가리키는 포인터
	CMainFrame *m_pTrinMainFrame;

// Dialog Data
	//{{AFX_DATA(CTerrainDIALOG)
	enum { IDD = IDD_TRINDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerrainDIALOG)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTerrainDIALOG)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBrushSelect();
	afx_msg void OnBrushDot();
	afx_msg void OnBrushPlus();
	afx_msg void OnBrushLine();
	afx_msg void OnBrushFill();
	afx_msg void OnBrushErase();
	afx_msg void OnBrushFind();
	afx_msg void OnBrushOpSize1();
	afx_msg void OnBrushOpSize3();
	afx_msg void OnBrushOpSize5();
	afx_msg void OnBrushOpSize7();
	afx_msg void OnBrushOpType1();
	afx_msg void OnBrushOpType2();
	afx_msg void OnBrushOpType3();
	afx_msg void OnBrushOpType4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAINDIALOG_H__4632C8AE_C4C5_45BD_887E_29398318F8DE__INCLUDED_)
