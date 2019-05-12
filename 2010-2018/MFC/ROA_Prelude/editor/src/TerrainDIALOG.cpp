// TerrainDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "TerrainDIALOG.h"

// *�߰��� ��� ����
#include "RuneStoneDoc.h"
#include "MainFrm.h"
#include "TileSetView.h"
#include "RuneStoneView.h"
#include "LocDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerrainDIALOG dialog


CTerrainDIALOG::CTerrainDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTerrainDIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTerrainDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerrainDIALOG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTerrainDIALOG, CDialog)
	//{{AFX_MSG_MAP(CTerrainDIALOG)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BRUSH_SELECT, OnBrushSelect)
	ON_BN_CLICKED(IDC_BRUSH_DOT, OnBrushDot)
	ON_BN_CLICKED(IDC_BRUSH_PLUS, OnBrushPlus)
	ON_BN_CLICKED(IDC_BRUSH_LINE, OnBrushLine)
	ON_BN_CLICKED(IDC_BRUSH_FILL, OnBrushFill)
	ON_BN_CLICKED(IDC_BRUSH_ERASE, OnBrushErase)
	ON_BN_CLICKED(IDC_BRUSH_FIND, OnBrushFind)
	ON_BN_CLICKED(IDC_BRUSH_OP_SIZE1, OnBrushOpSize1)
	ON_BN_CLICKED(IDC_BRUSH_OP_SIZE3, OnBrushOpSize3)
	ON_BN_CLICKED(IDC_BRUSH_OP_SIZE5, OnBrushOpSize5)
	ON_BN_CLICKED(IDC_BRUSH_OP_SIZE7, OnBrushOpSize7)
	ON_BN_CLICKED(IDC_BRUSH_OP_TYPE1, OnBrushOpType1)
	ON_BN_CLICKED(IDC_BRUSH_OP_TYPE2, OnBrushOpType2)
	ON_BN_CLICKED(IDC_BRUSH_OP_TYPE3, OnBrushOpType3)
	ON_BN_CLICKED(IDC_BRUSH_OP_TYPE4, OnBrushOpType4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainDIALOG message handlers

void CTerrainDIALOG::OnClose() // *���̾�α� ������
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	// *������ �귯�� / ���� �������� �ʱ�ȭ
	pView->m_SelectDlgBrush = -1;
	pView->m_SelectDlgOpSize = -1;
	pView->m_SelectDlgOpType = -1;
	pView->m_MouseDragX = -1;
	pView->m_MouseDragY = -1;
	pView->m_SelectX = 0;
	pView->m_SelectY = 0;

	// *�޸� ��ȯ �� ������ �缳��(�� �۵��� �� �������� �־�� ��)
	m_pTrinMainFrame->m_pTrinDlg = NULL;
	DestroyWindow();

	delete this;
}

BOOL CTerrainDIALOG::OnInitDialog() // *���̾�α� ������ �ʱ�ȭ��
{	
	// *��ư�� ������ư �̹��� ����
	CBitmap BrushSelect, BrushDot, BrushPlus, BrushLine, BrushFill, BrushErase, BrushFind;
	CBitmap BrushOPS1, BrushOPS3, BrushOPS5, BrushOPS7, BrushOPT1, BrushOPT2, BrushOPT3, BrushOPT4;

	BrushSelect.LoadBitmap(IDB_BRUSH_SELECT);	BrushDot.LoadBitmap(IDB_BRUSH_DOT);
	BrushPlus.LoadBitmap(IDB_BRUSH_PLUS);		BrushLine.LoadBitmap(IDB_BRUSH_LINE);
	BrushFill.LoadBitmap(IDB_BRUSH_FILL);		BrushErase.LoadBitmap(IDB_BRUSH_ERASE);
	BrushFind.LoadBitmap(IDB_BRUSH_FIND);		BrushOPS1.LoadBitmap(IDB_BRUSH_OP_S1);
	BrushOPS3.LoadBitmap(IDB_BRUSH_OP_S3);		BrushOPS5.LoadBitmap(IDB_BRUSH_OP_S5);
	BrushOPS7.LoadBitmap(IDB_BRUSH_OP_S7);		BrushOPT1.LoadBitmap(IDB_BRUSH_OP_T1);
	BrushOPT2.LoadBitmap(IDB_BRUSH_OP_T2);		BrushOPT3.LoadBitmap(IDB_BRUSH_OP_T3);
	BrushOPT4.LoadBitmap(IDB_BRUSH_OP_T4);

	((CButton*)GetDlgItem(IDC_BRUSH_SELECT))->SetBitmap(BrushSelect);
	((CButton*)GetDlgItem(IDC_BRUSH_DOT))->SetBitmap(BrushDot);
	((CButton*)GetDlgItem(IDC_BRUSH_PLUS))->SetBitmap(BrushPlus);
	((CButton*)GetDlgItem(IDC_BRUSH_LINE))->SetBitmap(BrushLine);
	((CButton*)GetDlgItem(IDC_BRUSH_FILL))->SetBitmap(BrushFill);
	((CButton*)GetDlgItem(IDC_BRUSH_ERASE))->SetBitmap(BrushErase);
	((CButton*)GetDlgItem(IDC_BRUSH_FIND))->SetBitmap(BrushFind);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_SIZE1))->SetBitmap(BrushOPS1);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_SIZE3))->SetBitmap(BrushOPS3);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_SIZE5))->SetBitmap(BrushOPS5);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_SIZE7))->SetBitmap(BrushOPS7);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_TYPE1))->SetBitmap(BrushOPT1);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_TYPE2))->SetBitmap(BrushOPT2);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_TYPE3))->SetBitmap(BrushOPT3);
	((CButton*)GetDlgItem(IDC_BRUSH_OP_TYPE4))->SetBitmap(BrushOPT4);

	BrushSelect.Detach();		BrushDot.Detach();		BrushPlus.Detach();
	BrushLine.Detach();			BrushFill.Detach();		BrushErase.Detach();
	BrushFind.Detach();			BrushOPS1.Detach();		BrushOPS3.Detach();
	BrushOPS5.Detach();			BrushOPS7.Detach();		BrushOPT1.Detach();
	BrushOPT2.Detach();			BrushOPT3.Detach();		BrushOPT4.Detach();

	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTerrainDIALOG::PreTranslateMessage(MSG* pMsg) // *������ �޽��� �������� �˻�
{
	// *�ش� Ű �޽��� ��ȿȭ
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CTerrainDIALOG::OnCreate(LPCREATESTRUCT lpCreateStruct) // *���̾�α� ������
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	// *���� ���̾�α��� ��ũ�� �� ���� ����
	CRuntimeClass *pObject;
	pObject = RUNTIME_CLASS(CTileSetView);
	
	CTileSetView *pDlgView = (CTileSetView*)pObject->CreateObject();

	if(!pDlgView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(15, 180, 427, 445), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Faile to crate dlgview window\n");
		return -1;
	}

	// *���� ���̾�α� �ı�
	if(pMainFrm->m_pLocDlg != NULL)
	{
		pView->m_ShowLoc = FALSE;
		pView->m_SelectDlgLoc = -1;	
		pMainFrm->m_pLocDlg->m_pLocMainFrame = NULL;
		pMainFrm->m_pLocDlg->DestroyWindow();
		pMainFrm->m_pLocDlg = NULL;
	}
	
	return 0;
}

void CTerrainDIALOG::OnBrushSelect() // *�������� �귯�� ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(FALSE);
	pView->m_SelectDlgBrush = 0;
}

void CTerrainDIALOG::OnBrushDot() // *�� �귯�� ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(FALSE);
	pView->m_SelectDlgBrush = 1;

}

void CTerrainDIALOG::OnBrushPlus() // *���� �귯�� ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(FALSE);
	pView->m_SelectDlgBrush = 2;
}

void CTerrainDIALOG::OnBrushLine() // *�� �귯�� ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(TRUE);

	pView->m_SelectDlgBrush = 3;
}

void CTerrainDIALOG::OnBrushFill() // *ä��� �귯�� ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(FALSE);
	pView->m_SelectDlgBrush = 4;
}

void CTerrainDIALOG::OnBrushErase() // *����� �귯�� ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(TRUE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(TRUE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(FALSE);
	pView->m_SelectDlgBrush = 5;

}

void CTerrainDIALOG::OnBrushFind() // *����ã�� �귯�� ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	GetDlgItem(IDC_BRUSH_OP_STATIC)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE3)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_SIZE5)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_SIZE7)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE2)->ShowWindow(FALSE);	GetDlgItem(IDC_BRUSH_OP_TYPE3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BRUSH_OP_TYPE4)->ShowWindow(FALSE);
	pView->m_SelectDlgBrush = 6;
}

void CTerrainDIALOG::OnBrushOpSize1() // *�귯�� ������1 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	pView->m_SelectDlgOpSize = 1;
}

void CTerrainDIALOG::OnBrushOpSize3() // *�귯�� ������3 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpSize = 3;
}


void CTerrainDIALOG::OnBrushOpSize5() // *�귯�� ������5 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpSize = 5;
}

void CTerrainDIALOG::OnBrushOpSize7() // *�귯�� ������7 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpSize = 7;
}

void CTerrainDIALOG::OnBrushOpType1() // *�밢�� Ÿ��1 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 1;
}

void CTerrainDIALOG::OnBrushOpType2()  // *�밢�� Ÿ��2 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 2;
}

void CTerrainDIALOG::OnBrushOpType3()  // *�밢�� Ÿ��3 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 3;
}

void CTerrainDIALOG::OnBrushOpType4()  // *�밢�� Ÿ��4 Ŭ����
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 4;
}
