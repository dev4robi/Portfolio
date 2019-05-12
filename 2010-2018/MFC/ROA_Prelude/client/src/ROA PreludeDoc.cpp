// ROA PreludeDoc.cpp : implementation of the CROAPreludeDoc class
//

#include "stdafx.h"
#include "ROA Prelude.h"

#include "ROA PreludeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeDoc

IMPLEMENT_DYNCREATE(CROAPreludeDoc, CDocument)

BEGIN_MESSAGE_MAP(CROAPreludeDoc, CDocument)
	//{{AFX_MSG_MAP(CROAPreludeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeDoc construction/destruction

CROAPreludeDoc::CROAPreludeDoc()
{
	// TODO: add one-time construction code here

}

CROAPreludeDoc::~CROAPreludeDoc()
{
}

BOOL CROAPreludeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CROAPreludeDoc serialization

void CROAPreludeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeDoc diagnostics

#ifdef _DEBUG
void CROAPreludeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CROAPreludeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeDoc commands
