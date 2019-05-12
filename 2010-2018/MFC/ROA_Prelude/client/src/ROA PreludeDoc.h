// ROA PreludeDoc.h : interface of the CROAPreludeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROAPRELUDEDOC_H__5BEFCC41_E293_4DDA_852D_AFE507E73375__INCLUDED_)
#define AFX_ROAPRELUDEDOC_H__5BEFCC41_E293_4DDA_852D_AFE507E73375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CROAPreludeDoc : public CDocument
{
protected: // create from serialization only
	CROAPreludeDoc();
	DECLARE_DYNCREATE(CROAPreludeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROAPreludeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CROAPreludeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CROAPreludeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROAPRELUDEDOC_H__5BEFCC41_E293_4DDA_852D_AFE507E73375__INCLUDED_)
