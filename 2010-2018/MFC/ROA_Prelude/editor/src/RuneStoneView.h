// RuneStoneView.h : interface of the CRuneStoneView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNESTONEVIEW_H__F86AB5E8_071E_423C_98B4_B4F48864189D__INCLUDED_)
#define AFX_RUNESTONEVIEW_H__F86AB5E8_071E_423C_98B4_B4F48864189D__INCLUDED_

// *구역 선택 버튼
#define NONE -1
#define SELLOC 0
#define CRELOC 1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// *참조하는 클래스
class CRuneStoneDoc;

class CRuneStoneView : public CScrollView
{
protected: // create from serialization only
	CRuneStoneView();
	DECLARE_DYNCREATE(CRuneStoneView)

// Attributes
public:
	CRuneStoneDoc* GetDocument();

	// *맴버 변수************************************************ //
	// *복사 붙여넣기 변수
	int m_CopyWidth,m_CopyHeight;

	// *드래그변수
	BOOL m_IsMouseClicked;
	int m_MouseDragX, m_MouseDragY;

	// *현재 마우스 좌표를 저장하는 변수
	int m_MouseX,m_MouseY;

	// *현재 선택한 뷰 영역을 저장하는 변수
	int m_SelectX, m_SelectY;

	// *다이얼로그 뷰에서 선택된 타일ID
	int m_SelectDlgTrinID;

	// *다이얼로그 뷰에서 선택된 브러시 / 크기 / 대각선 타입
	int m_SelectDlgBrush;
	int m_SelectDlgOpSize;
	int m_SelectDlgOpType;

	// *구역 다이얼로그 뷰에서 선택된 구역 버튼
	int m_SelectDlgLoc;

	// *구역 브러시를 위한 변수
	int m_LocStartX, m_LocStartY;
	int m_LocEndX, m_LocEndY;

	// *이미 만들어진 맵이 있는지 체크하는 변수
	BOOL m_IsMapExist;

	// *격자와 충돌 영역과 높낮이와 구역 보이기 여부를 위한 변수
	BOOL m_ShowGrid;
	BOOL m_ShowColl;
	BOOL m_ShowHigh;
	BOOL m_ShowLoc;

	// *맴버 함수************************************************ //
	void FillBrushDraw(int ClickTrinID, int SelectTrinID, int x, int y, int Count); // *채우기 붓
	void LineBrushDraw(int SelectDlgOpType, int SelectTrinID, int x, int y, int SelectOpSize); // *선 붓
	void PlusBrushDraw(int SelectDlgOpSize, int SelectTrinID, int x, int y); // *십자 붓
	

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
