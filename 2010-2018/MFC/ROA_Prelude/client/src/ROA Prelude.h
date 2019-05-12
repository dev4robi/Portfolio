// ROA Prelude.h : main header file for the ROA PRELUDE application
//

#if !defined(AFX_ROAPRELUDE_H__6F74BA49_33B4_4A2D_BEE3_940A091F3063__INCLUDED_)
#define AFX_ROAPRELUDE_H__6F74BA49_33B4_4A2D_BEE3_940A091F3063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CROAPreludeApp:
// See ROA Prelude.cpp for the implementation of this class
//

class CROAPreludeApp : public CWinApp
{
public:
	CROAPreludeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROAPreludeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CROAPreludeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROAPRELUDE_H__6F74BA49_33B4_4A2D_BEE3_940A091F3063__INCLUDED_)
