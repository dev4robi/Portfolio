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
	CList <UnitObject, UnitObject&> m_UnitList; // ��� ������ ��Ƶδ� ����Ʈ.
	CArray <BOOL, BOOL&> m_UnitMap; // ��� ������ ��ǥ ���¸� ��Ƶδ� ����Ʈ.
	CArray <Location, Location&> m_LocationDat;

	// �÷��̾�
	PlayerObject HostPlayer;
	
	// ���� ����
	BOOL m_CamLockHero;
	

	// �̹����� ���� �޸�
	CDC memDC; // ���� ȭ������� ���� dc.
	CBitmap memBitmap; // ���� ȭ������� ���� ��Ʈ��.
	
	BITMAP bmpinfo; // bmp������ ������ ���� ����.
	CDC TrinDC; // Ÿ�ϼ��� ������ �ִ� dc.
	CBitmap TrinBitmap; // Ÿ�ϼ� ��Ʈ��.
	CDC UnitDC; // ������ ������ �ִ� dc.
	CArray <CBitmap, CBitmap&> UnitBitmap; //���ּ� ��Ʈ��.
	CBitmap InterfaceBitmap; // �������̽� ��Ʈ��.
	CDC InterfaceDC; // �������̽��� ������ �ִ� ��Ʈ��.
	CBitmap BarBitmap; // HP, MP, EXP�� ������ �ִ� ��Ʈ��.
	CDC BarDC; // HP, MP, EXP�� ������ �ִ� dc.
	CBitmap CharWndBitmap; // ĳ����â ������ �ִ� ��Ʈ��.
	CDC CharWndDC; // ĳ����â ������ �ִ� dc.

// Operations
public:

	void TransDraw(CDC * pDC, int x, int y,int width,int height,CDC* srcDC,int sx,int sy,COLORREF crColour); // �׽�Ʈ

	void DrawMemBlt(); // �޸𸮿��� view�������� ��Ʈ�� ���
	void DrawScreenMem(int RgnX, int RgnY, int PixX, int PixY); // �ȼ� Ȥ�� ĭ�� �޾Ƽ� �����¿� 5ĭ�� �߰��� �޸𸮿� �̹��� ���(���ο��� Draw�Լ� ���ư�).
	void AnimationDelay(); // ��ǹ�ȣ�� ���� �ʿ��� ��ǹ�ȣ�� ����

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
