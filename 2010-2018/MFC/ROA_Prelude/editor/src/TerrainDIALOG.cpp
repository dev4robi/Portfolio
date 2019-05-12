// TerrainDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "RuneStone.h"
#include "TerrainDIALOG.h"

// *추가된 헤더 파일
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

void CTerrainDIALOG::OnClose() // *다이얼로그 닫을시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	// *선택한 브러시 / 설정 없음으로 초기화
	pView->m_SelectDlgBrush = -1;
	pView->m_SelectDlgOpSize = -1;
	pView->m_SelectDlgOpType = -1;
	pView->m_MouseDragX = -1;
	pView->m_MouseDragY = -1;
	pView->m_SelectX = 0;
	pView->m_SelectY = 0;

	// *메모리 반환 및 포인터 재설정(이 작동은 맨 마지막에 있어야 함)
	m_pTrinMainFrame->m_pTrinDlg = NULL;
	DestroyWindow();

	delete this;
}

BOOL CTerrainDIALOG::OnInitDialog() // *다이얼로그 생성전 초기화시
{	
	// *버튼형 라디오버튼 이미지 변경
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

BOOL CTerrainDIALOG::PreTranslateMessage(MSG* pMsg) // *윈도우 메시지 전달전에 검사
{
	// *해당 키 메시지 무효화
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CTerrainDIALOG::OnCreate(LPCREATESTRUCT lpCreateStruct) // *다이얼로그 생성시
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	// *지형 다이얼로그의 스크롤 뷰 영역 생성
	CRuntimeClass *pObject;
	pObject = RUNTIME_CLASS(CTileSetView);
	
	CTileSetView *pDlgView = (CTileSetView*)pObject->CreateObject();

	if(!pDlgView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(15, 180, 427, 445), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Faile to crate dlgview window\n");
		return -1;
	}

	// *구역 다이얼로그 파괴
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

void CTerrainDIALOG::OnBrushSelect() // *지형선택 브러시 선택
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

void CTerrainDIALOG::OnBrushDot() // *점 브러시 선택
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

void CTerrainDIALOG::OnBrushPlus() // *십자 브러시 선택
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

void CTerrainDIALOG::OnBrushLine() // *선 브러시 선택
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

void CTerrainDIALOG::OnBrushFill() // *채우기 브러시 선택
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

void CTerrainDIALOG::OnBrushErase() // *지우기 브러시 선택
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

void CTerrainDIALOG::OnBrushFind() // *지형찾기 브러시 선택
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

void CTerrainDIALOG::OnBrushOpSize1() // *브러시 사이즈1 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();

	pView->m_SelectDlgOpSize = 1;
}

void CTerrainDIALOG::OnBrushOpSize3() // *브러시 사이즈3 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpSize = 3;
}


void CTerrainDIALOG::OnBrushOpSize5() // *브러시 사이즈5 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpSize = 5;
}

void CTerrainDIALOG::OnBrushOpSize7() // *브러시 사이즈7 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpSize = 7;
}

void CTerrainDIALOG::OnBrushOpType1() // *대각선 타입1 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 1;
}

void CTerrainDIALOG::OnBrushOpType2()  // *대각선 타입2 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 2;
}

void CTerrainDIALOG::OnBrushOpType3()  // *대각선 타입3 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 3;
}

void CTerrainDIALOG::OnBrushOpType4()  // *대각선 타입4 클릭시
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneView *pView = (CRuneStoneView*)pMainFrm->GetActiveView();
	
	pView->m_SelectDlgOpType = 4;
}
