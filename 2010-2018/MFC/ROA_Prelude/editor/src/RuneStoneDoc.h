// RuneStoneDoc.h : interface of the CRuneStoneDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#include "ROAdat.h"

#if !defined(AFX_RUNESTONEDOC_H__EE0DC02C_C0FD_4D1D_AC61_2FEF621505B5__INCLUDED_)
#define AFX_RUNESTONEDOC_H__EE0DC02C_C0FD_4D1D_AC61_2FEF621505B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRuneStoneDoc : public CDocument
{
protected: // create from serialization only
	CRuneStoneDoc();
	DECLARE_DYNCREATE(CRuneStoneDoc)

// Attributes
public:
	// *복사 변수
	CArray <Terrain, Terrain&> m_Copy;
	
	// *맵 데이터를 다큐먼트에 저장하기 위한 변수들
	int m_SizeHeightDoc, m_SizeWidthDoc;
	CString m_FileNameDoc;
	CArray <Terrain, Terrain&> m_MapDataDoc;
	CArray <Location, Location&> m_LocationDoc;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuneStoneDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRuneStoneDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRuneStoneDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNESTONEDOC_H__EE0DC02C_C0FD_4D1D_AC61_2FEF621505B5__INCLUDED_)
