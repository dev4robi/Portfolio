// RuneStoneDoc.cpp : implementation of the CRuneStoneDoc class
//

#include "stdafx.h"
#include "RuneStone.h"

#include "RuneStoneDoc.h"

// *추가된 해더 파일
#include "RuneStoneView.h"
#include "MainFrm.h"
#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneDoc

IMPLEMENT_DYNCREATE(CRuneStoneDoc, CDocument)

BEGIN_MESSAGE_MAP(CRuneStoneDoc, CDocument)
	//{{AFX_MSG_MAP(CRuneStoneDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneDoc construction/destruction

CRuneStoneDoc::CRuneStoneDoc()
{
	//m_pTrinData = NULL;

	// * 도큐먼트 데이터 초기화
	m_FileNameDoc = "Map000";
	m_SizeHeightDoc = 0;
	m_SizeWidthDoc = 0;
}

CRuneStoneDoc::~CRuneStoneDoc()
{
}

BOOL CRuneStoneDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRuneStoneDoc serialization

void CRuneStoneDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}

	else
	{
		// TODO: add storing code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneDoc diagnostics

#ifdef _DEBUG
void CRuneStoneDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRuneStoneDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRuneStoneDoc commands
