// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__31D4144C_1BE1_43FD_BB4A_C10C7DC0DBC6__INCLUDED_)
#define AFX_MAINFRM_H__31D4144C_1BE1_43FD_BB4A_C10C7DC0DBC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// *참조하는 클래스
class CTerrainDIALOG; 
class CNpcDIALOG;
class CItemDIALOG;
class CMonsterDIALOG;
class CMinimapDialog;
class LocDialog;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	// *지형 팔레트(Terrain Dialog)
	CTerrainDIALOG *m_pTrinDlg;

	// *NPC 팔레트(Npc Dialog)
	CNpcDIALOG *m_pNpcDlg;

	// *아이템 팔레트(Item Dialog)
	CItemDIALOG *m_pItemDlg;

	// *몬스터 팔레트(Monster Dialog)
	CMonsterDIALOG *m_pMonDlg;

	// *지형 팔레트(Location Dialog)
	LocDialog *m_pLocDlg;

	// *미니맵
	CMinimapDialog *m_pMiniDlg;

// Operations
public:
	CStatusBar  m_wndStatusBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTrinDialog();
	afx_msg void OnNpcDialog();
	afx_msg void OnItemDialog();
	afx_msg void OnMonDialog();
	afx_msg void OnFileNew();
	afx_msg void OnShowgrid();
	afx_msg void OnUpdateShowgrid(CCmdUI* pCmdUI);
	afx_msg void OnShowcoll();
	afx_msg void OnUpdateShowcoll(CCmdUI* pCmdUI);
	afx_msg void OnShowhigh();
	afx_msg void OnUpdateShowhigh(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnMinimap();
	afx_msg void OnMinimapDialog();
	afx_msg void OnSavefile();
	afx_msg void OnMapSetup();
	afx_msg void OnOpenfile();
	afx_msg void OnMapEdge();
	afx_msg void OnTrinCopy();
	afx_msg void OnTrinPaste();
	afx_msg void OnTrinCut();
	afx_msg void OnLocateDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__31D4144C_1BE1_43FD_BB4A_C10C7DC0DBC6__INCLUDED_)
