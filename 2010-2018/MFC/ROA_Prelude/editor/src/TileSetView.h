#if !defined(AFX_TILESETVIEW_H__EFDA7DF6_99D8_4E12_8881_99F977B5AB20__INCLUDED_)
#define AFX_TILESETVIEW_H__EFDA7DF6_99D8_4E12_8881_99F977B5AB20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TileSetView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTileSetView view

class CTileSetView : public CScrollView
{
protected:
	CTileSetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTileSetView)

// Attributes
public:
	// *더블 버퍼링 사용을 위한 변수
	CDC MemDC;
	CBitmap MemBit;
	CDC MapDC;
	CBitmap MapBit;

	// *현재 선택한 다이얼로그 뷰 영역을 저장하는 변수
	int m_DlgSelectX, m_DlgSelectY;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTileSetView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTileSetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTileSetView)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILESETVIEW_H__EFDA7DF6_99D8_4E12_8881_99F977B5AB20__INCLUDED_)
