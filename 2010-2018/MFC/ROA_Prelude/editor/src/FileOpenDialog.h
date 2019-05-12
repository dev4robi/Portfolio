#if !defined(AFX_FILEOPENDIALOG_H__52A11B4B_DED9_4B58_B53B_F52BDDBDFFEC__INCLUDED_)
#define AFX_FILEOPENDIALOG_H__52A11B4B_DED9_4B58_B53B_F52BDDBDFFEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileOpenDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileOpenDialog dialog

class CFileOpenDialog : public CDialog
{
// Construction
public:
	CFileOpenDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileOpenDialog)
	enum { IDD = IDD_FILEOPEN };
	CString	m_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileOpenDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileOpenDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEOPENDIALOG_H__52A11B4B_DED9_4B58_B53B_F52BDDBDFFEC__INCLUDED_)
