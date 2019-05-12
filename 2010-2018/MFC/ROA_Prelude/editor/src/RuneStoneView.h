// RuneStoneView.h : interface of the CRuneStoneView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNESTONEVIEW_H__F86AB5E8_071E_423C_98B4_B4F48864189D__INCLUDED_)
#define AFX_RUNESTONEVIEW_H__F86AB5E8_071E_423C_98B4_B4F48864189D__INCLUDED_

// *���� ���� ��ư
#define NONE -1
#define SELLOC 0
#define CRELOC 1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// *�����ϴ� Ŭ����
class CRuneStoneDoc;

class CRuneStoneView : public CScrollView
{
protected: // create from serialization only
	CRuneStoneView();
	DECLARE_DYNCREATE(CRuneStoneView)

// Attributes
public:
	CRuneStoneDoc* GetDocument();

	// *�ɹ� ����************************************************ //
	// *���� �ٿ��ֱ� ����
	int m_CopyWidth,m_CopyHeight;

	// *�巡�׺���
	BOOL m_IsMouseClicked;
	int m_MouseDragX, m_MouseDragY;

	// *���� ���콺 ��ǥ�� �����ϴ� ����
	int m_MouseX,m_MouseY;

	// *���� ������ �� ������ �����ϴ� ����
	int m_SelectX, m_SelectY;

	// *���̾�α� �信�� ���õ� Ÿ��ID
	int m_SelectDlgTrinID;

	// *���̾�α� �信�� ���õ� �귯�� / ũ�� / �밢�� Ÿ��
	int m_SelectDlgBrush;
	int m_SelectDlgOpSize;
	int m_SelectDlgOpType;

	// *���� ���̾�α� �信�� ���õ� ���� ��ư
	int m_SelectDlgLoc;

	// *���� �귯�ø� ���� ����
	int m_LocStartX, m_LocStartY;
	int m_LocEndX, m_LocEndY;

	// *�̹� ������� ���� �ִ��� üũ�ϴ� ����
	BOOL m_IsMapExist;

	// *���ڿ� �浹 ������ �����̿� ���� ���̱� ���θ� ���� ����
	BOOL m_ShowGrid;
	BOOL m_ShowColl;
	BOOL m_ShowHigh;
	BOOL m_ShowLoc;

	// *�ɹ� �Լ�************************************************ //
	void FillBrushDraw(int ClickTrinID, int SelectTrinID, int x, int y, int Count); // *ä��� ��
	void LineBrushDraw(int SelectDlgOpType, int SelectTrinID, int x, int y, int SelectOpSize); // *�� ��
	void PlusBrushDraw(int SelectDlgOpSize, int SelectTrinID, int x, int y); // *���� ��
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuneStoneView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRuneStoneView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRuneStoneView)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RuneStoneView.cpp
inline CRuneStoneDoc* CRuneStoneView::GetDocument()
   { return (CRuneStoneDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNESTONEVIEW_H__F86AB5E8_071E_423C_98B4_B4F48864189D__INCLUDED_)
