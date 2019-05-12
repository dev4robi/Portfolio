// ROA PreludeView.h : interface of the CROAPreludeView class
//
/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

#if !defined(AFX_ROAPRELUDEVIEW_H__216E3D62_6418_405D_B7F8_38B10BD2DFEC__INCLUDED_)
#define AFX_ROAPRELUDEVIEW_H__216E3D62_6418_405D_B7F8_38B10BD2DFEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000


class CROAPreludeView : public CView
{
protected: // create from serialization only
	CROAPreludeView();
	DECLARE_DYNCREATE(CROAPreludeView)

// Attributes
public:
	CROAPreludeDoc* GetDocument();

	int m_MapWidth, m_MapHeight;
	CArray <Terrain, Terrain&> m_TrinDat;
	CList <UnitObject, UnitObject&> m_UnitList; // 모든 유닛을 담아두는 리스트.
	CArray <BOOL, BOOL&> m_UnitMap; // 모든 유닛의 좌표 상태를 담아두는 리스트.
	CArray <Location, Location&> m_LocationDat;

	// 플레이어
	PlayerObject HostPlayer;
	
	// 게임 상태
	BOOL m_CamLockHero;
	

	// 이미지를 위한 메모리
	CDC memDC; // 최종 화면출력을 위한 dc.
	CBitmap memBitmap; // 최종 화면출력을 위한 비트맵.
	
	BITMAP bmpinfo; // bmp정보를 가지고 있을 변수.
	CDC TrinDC; // 타일셋을 가지고 있는 dc.
	CBitmap TrinBitmap; // 타일셋 비트맵.
	CDC UnitDC; // 유닛을 가지고 있는 dc.
	CArray <CBitmap, CBitmap&> UnitBitmap; //유닛셋 비트맵.
	CBitmap InterfaceBitmap; // 인터페이스 비트맵.
	CDC InterfaceDC; // 인터페이스를 가지고 있는 비트맵.
	CBitmap BarBitmap; // HP, MP, EXP를 가지고 있는 비트맵.
	CDC BarDC; // HP, MP, EXP를 가지고 있는 dc.
	CBitmap CharWndBitmap; // 캐릭터창 가지고 있는 비트맵.
	CDC CharWndDC; // 캐릭터창 가지고 있는 dc.

// Operations
public:

	void TransDraw(CDC * pDC, int x, int y,int width,int height,CDC* srcDC,int sx,int sy,COLORREF crColour); // 테스트

	void DrawMemBlt(); // 메모리에서 view영역으로 비트맵 출력
	void DrawScreenMem(int RgnX, int RgnY, int PixX, int PixY); // 픽셀 혹은 칸을 받아서 상하좌우 5칸씩 추가로 메모리에 이미지 출력(내부에서 Draw함수 돌아감).
	void AnimationDelay(); // 모션번호를 다음 필요한 모션번호로 변경

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROAPreludeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CROAPreludeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CROAPreludeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ROA PreludeView.cpp
inline CROAPreludeDoc* CROAPreludeView::GetDocument()
   { return (CROAPreludeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROAPRELUDEVIEW_H__216E3D62_6418_405D_B7F8_38B10BD2DFEC__INCLUDED_)
