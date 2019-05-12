// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RuneStone.h"

#include "MainFrm.h"

// *추가된 해더 파일
#include "RuneStoneDoc.h"
#include "RuneStoneView.h"
#include "TerrainDIALOG.h"
#include "NpcDIALOG.h"
#include "MonsterDIALOG.h"
#include "ItemDIALOG.h"
#include "LocDialog.h"
#include "NewMapDIALOG.h"
#include "MinimapDialog.h"
#include "SetupMapDialog.h"
#include "FileOpenDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TRIN_DIALOG, OnTrinDialog)
	ON_COMMAND(ID_NPC_DIALOG, OnNpcDialog)
	ON_COMMAND(ID_ITEM_DIALOG, OnItemDialog)
	ON_COMMAND(ID_MON_DIALOG, OnMonDialog)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_SHOWGRID, OnShowgrid)
	ON_UPDATE_COMMAND_UI(ID_SHOWGRID, OnUpdateShowgrid)
	ON_COMMAND(ID_SHOWCOLL, OnShowcoll)
	ON_UPDATE_COMMAND_UI(ID_SHOWCOLL, OnUpdateShowcoll)
	ON_COMMAND(ID_SHOWHIGH, OnShowhigh)
	ON_UPDATE_COMMAND_UI(ID_SHOWHIGH, OnUpdateShowhigh)
	ON_COMMAND(ID_MINIMAP_DIALOG, OnMinimapDialog)
	ON_COMMAND(ID_SAVEFILE, OnSavefile)
	ON_COMMAND(ID_MAP_SETUP, OnMapSetup)
	ON_COMMAND(ID_OPENFILE, OnOpenfile)
	ON_COMMAND(ID_MAP_EDGE, OnMapEdge)
	ON_COMMAND(ID_TRIN_COPY, OnTrinCopy)
	ON_COMMAND(ID_TRIN_PASTE, OnTrinPaste)
	ON_COMMAND(ID_TRIN_CUT, OnTrinCut)
	ON_COMMAND(ID_LOCATE_DIALOG, OnLocateDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
	// *팔레트 다이얼로그 포인터 초기화
	m_pTrinDlg = NULL;
	m_pNpcDlg = NULL;
	m_pItemDlg = NULL;
	m_pMonDlg = NULL;
	m_pLocDlg = NULL;
	m_pMiniDlg = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 툴바 생성
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// 상태바 생성
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	HICON icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(icon, TRUE);   // 큰 아이콘을 설정합니다.
	SetIcon(icon, FALSE);  // 작은 아이콘을 설정합니다. 

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnTrinDialog() // * 지형 브러시 툴바 선택시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		if(m_pTrinDlg != NULL)
		{
			m_pTrinDlg->SetFocus();
		}

		else
		{
			m_pTrinDlg = new CTerrainDIALOG();
			m_pTrinDlg->m_pTrinMainFrame = this;
			
			// 이 아래에 초기값 설정
			//
			//
			//

			m_pTrinDlg->Create(IDD_TRINDIALOG);
			m_pTrinDlg->ShowWindow(SW_SHOW);		
		}
	}

	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnNpcDialog() // *NPC 브러시 툴바 선택시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		if(m_pNpcDlg != NULL)
		{
			m_pNpcDlg->SetFocus();
		}

		else
		{
			m_pNpcDlg = new CNpcDIALOG();
			m_pNpcDlg->m_pNpcMainFrame = this;

			// 이 아래에 초기값 설정
			//
			//
			//

			m_pNpcDlg->Create(IDD_NPCDIALOG);
			m_pNpcDlg->ShowWindow(SW_SHOW);
		}
	}

	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnItemDialog() // *아이템 브러시 툴바 선택시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		if(m_pItemDlg != NULL)
		{
			m_pItemDlg->SetFocus();
		}
		
		else
		{
			m_pItemDlg = new CItemDIALOG();
			m_pItemDlg->m_pItemMainFrame = this;
			
			// 이 아래에 초기값 설정
			//
			//
			//
			
			m_pItemDlg->Create(IDD_ITEMDIALOG);
			m_pItemDlg->ShowWindow(SW_SHOW);
		}
	}

	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnMonDialog() // *몬스터 브러시 툴바 선택시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		if(m_pMonDlg != NULL)
		{
			m_pMonDlg->SetFocus();
		}
		
		else
		{
			m_pMonDlg = new CMonsterDIALOG();
			m_pMonDlg->m_pMonMainFrame = this;
			
			// 이 아래에 초기값 설정
			//
			//
			//
			
			m_pMonDlg->Create(IDD_MONDIALOG);
			m_pMonDlg->ShowWindow(SW_SHOW);
		}
	}

	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnLocateDialog() // *구역 브러시 툴바 선택시
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		if(m_pLocDlg != NULL)
		{
			m_pLocDlg->SetFocus();
		}
		
		else
		{
			m_pLocDlg = new LocDialog();
			m_pLocDlg->m_pLocMainFrame = this;
			
			// 이 아래에 초기값 설정
			//
			//
			//
			
			m_pLocDlg->Create(IDD_LOCATEDIALOG);
			m_pLocDlg->ShowWindow(SW_SHOW);

			for(int i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
			{
				CString str;
				str = pDoc->m_LocationDoc[i].LocationName; // 너 나가
				m_pLocDlg->m_LocListBox.AddString(str);
			}
		}

		pView->m_ShowLoc = TRUE;
		pView->m_SelectDlgLoc = NONE;

		// *구역 팔레트 띄울시 다른 팔레트 종료
		if(m_pTrinDlg != NULL)
		{
			// *선택한 브러시 / 설정 없음으로 초기화
			pView->m_SelectDlgBrush = -1;
			pView->m_SelectDlgOpSize = -1;
			pView->m_SelectDlgOpType = -1;
			
			// *포인터 재설정
			m_pTrinDlg->m_pTrinMainFrame = NULL;
			m_pTrinDlg->DestroyWindow();
			m_pTrinDlg = NULL;
		}

		if(m_pNpcDlg != NULL)
		{
			// *포인터 재설정
			m_pNpcDlg->m_pNpcMainFrame = NULL;
			m_pNpcDlg->DestroyWindow();
			m_pNpcDlg = NULL;
		}

		if(m_pItemDlg != NULL)
		{
			// *포인터 재설정	
			m_pItemDlg->m_pItemMainFrame = NULL;
			m_pItemDlg->DestroyWindow();
			m_pItemDlg = NULL;
		}

		if(m_pMonDlg != NULL)
		{
			// *포인터 재설정	
			m_pMonDlg->m_pMonMainFrame = NULL;
			m_pMonDlg->DestroyWindow();
			m_pMonDlg = NULL;
		}

		Invalidate(FALSE);
	}
	
	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
	
}

void CMainFrame::OnFileNew() // *새 맵 만들기 메뉴바/툴바 클릭시
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	// *만들어진 맵이 없으면
	if(pView->m_IsMapExist == FALSE)
	{
		CNewMapDIALOG NewMapDlg;
		int DlgReturn = NewMapDlg.DoModal();
		
		if(DlgReturn == IDOK)
		{
			UpdateData(TRUE);

			pDoc->m_FileNameDoc = NewMapDlg.m_FileName;
			pDoc->m_SizeWidthDoc = NewMapDlg.m_SizeWidth;
			pDoc->m_SizeHeightDoc = NewMapDlg.m_SizeHeight;
			
			int MapSize = (pDoc->m_SizeWidthDoc) * (pDoc->m_SizeHeightDoc);
			pDoc->m_MapDataDoc.SetSize(MapSize);
			
			for(int i=0; i<MapSize; i++)
			{
				// *Default value
				pDoc->m_MapDataDoc[i].TrinID = 0;
				pDoc->m_MapDataDoc[i].TrinNextID = 0;
				pDoc->m_MapDataDoc[i].TrinMove = FALSE;
				pDoc->m_MapDataDoc[i].TrinHigh = 0;
			}

			pView->m_IsMapExist = TRUE;
			Invalidate();
		}
	}

	// *만들어진 맵이 있으면
	else
	{
		if(IDOK == AfxMessageBox("새로 만들게 되면 기존 맵 정보가 사라집니다. 새로 만들겠습니까?", MB_ICONEXCLAMATION | MB_OKCANCEL))
		{
			// *지형 다이얼로그 닫기
			if(m_pTrinDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				pView->m_SelectDlgBrush = -1;
				pView->m_SelectDlgOpSize = -1;
				pView->m_SelectDlgOpType = -1;
				
				m_pTrinDlg->CloseWindow();
				delete m_pTrinDlg;
				m_pTrinDlg = NULL;
			}
			// *end 지형 다이얼로그 닫기

			// *엔피씨 다이얼로그 닫기
			if(m_pNpcDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pNpcDlg->CloseWindow();
				delete m_pNpcDlg;
				m_pNpcDlg = NULL;
			}
			// *end 엔피씨 다이얼로그 닫기

			// *아이템 다이얼로그 닫기
			if(m_pItemDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pItemDlg->CloseWindow();
				delete m_pItemDlg;
				m_pItemDlg = NULL;
			}
			// *end 아이템 다이얼로그 닫기

			// *몬스터 다이얼로그 닫기
			if(m_pMonDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pMonDlg->CloseWindow();
				delete m_pMonDlg;
				m_pMonDlg = NULL;
			}
			// *end 몬스터 다이얼로그 닫기	

			// *미니맵 다이얼로그 닫기
			if(m_pMiniDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pMiniDlg->CloseWindow();
				delete m_pMiniDlg;
				m_pMiniDlg = NULL;
			}
			// *end 미니맵 다이얼로그 닫기	



			//for(int i=0; i<(pDoc->m_SizeWidthDoc) * (pDoc->m_SizeHeightDoc); i++)
			//pDoc->m_MapDataDoc.RemoveAt(i); // *CHECK 이 부분에서 가끔씩 에러가 발생하는 듯 하다.
			
			CNewMapDIALOG NewMapDlg;
			int DlgReturn = NewMapDlg.DoModal();

			if(DlgReturn == IDOK)
			{
				// *기존 데이터 삭제후 새 맵 만들기 다이얼로그 띄우기
				pView->m_IsMapExist = FALSE;
				pDoc->m_MapDataDoc.RemoveAll();
				UpdateData(TRUE);
			
				// *뉴맵 생성 다이얼로그 생성 후 취소 선택시 스크롤바 제거
				pDoc->m_SizeWidthDoc = 0;
				pDoc->m_SizeHeightDoc = 0;
				pDoc->m_MapDataDoc.SetSize(0);
				Invalidate(FALSE);

				pDoc->m_FileNameDoc = NewMapDlg.m_FileName;
				pDoc->m_SizeWidthDoc = NewMapDlg.m_SizeWidth;
				pDoc->m_SizeHeightDoc = NewMapDlg.m_SizeHeight;

				int MapSize = (pDoc->m_SizeWidthDoc) * (pDoc->m_SizeHeightDoc);
				pDoc->m_MapDataDoc.SetSize(MapSize);
				
				for(int i=0; i<MapSize; i++)
				{
					// *Default value
					pDoc->m_MapDataDoc[i].TrinID = 0;
					pDoc->m_MapDataDoc[i].TrinNextID = 0;
					pDoc->m_MapDataDoc[i].TrinMove = FALSE;
					pDoc->m_MapDataDoc[i].TrinHigh = 0;
				}
				
				pView->m_IsMapExist = TRUE;
			}

			Invalidate(FALSE);
		}
	}
}

void CMainFrame::OnShowgrid() // *격자 보이기 툴바 클릭시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	if(pView->m_IsMapExist == TRUE)
	{
		pView->m_ShowGrid = !(pView->m_ShowGrid);

		Invalidate(FALSE);
	}

	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnUpdateShowgrid(CCmdUI* pCmdUI) // *격자 보이기 상태일 때
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	pCmdUI->SetCheck(pView->m_ShowGrid == TRUE);
}

void CMainFrame::OnShowcoll() // *충돌 보이기 툴바 클릭시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	if(pView->m_IsMapExist == TRUE)
	{	
		pView->m_ShowColl = !(pView->m_ShowColl);
		
		Invalidate(FALSE);
	}

	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnUpdateShowcoll(CCmdUI* pCmdUI) // *충돌 보이기 상태일 때
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	pCmdUI->SetCheck(pView->m_ShowColl == TRUE);
}

void CMainFrame::OnShowhigh() // *높낮이 보이기 툴바 클릭시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		pView->m_ShowHigh = !(pView->m_ShowHigh);
	
		Invalidate(FALSE);
	}

	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnUpdateShowhigh(CCmdUI* pCmdUI) // *높낮이 보이기 상태일 때
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	pCmdUI->SetCheck(pView->m_ShowHigh == TRUE);
}

void CMainFrame::OnMinimapDialog() // *미니맵 보기 클릭시
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		if(m_pMiniDlg != NULL)
		{
			m_pMiniDlg->SetFocus();
		}
		
		else
		{
			m_pMiniDlg = new CMinimapDialog();
			m_pMiniDlg->m_pMiniMainFrame = this;
			
			// 이 아래에 초기값 설정
			//
			//
			//
			
			m_pMiniDlg->Create(IDD_MINIDIALOG);
			m_pMiniDlg->ShowWindow(SW_SHOW);
		}
	}
	
	else
		AfxMessageBox("먼저 새 맵을 만들어야 합니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnMapSetup() // *맵 설정 메뉴바 클릭시
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	// *새 맵이 있을 경우
	if(pView->m_IsMapExist == TRUE)
	{
		CSetupMapDialog NewMapDlg;
		int DlgReturn = NewMapDlg.DoModal();
		
		if(DlgReturn == IDOK)
		{
			UpdateData(TRUE);

			if(pDoc->m_SizeWidthDoc != NewMapDlg.m_SizeWidth || pDoc->m_SizeHeightDoc != NewMapDlg.m_SizeHeight)
			{
				int i,j;
				TerrainDat **TrinData = new TerrainDat*[pDoc->m_SizeWidthDoc];

				for(i=0; i<pDoc->m_SizeWidthDoc; i++)
					TrinData[i] = new TerrainDat[pDoc->m_SizeHeightDoc];

				for(i=0; i<pDoc->m_SizeHeightDoc; i++)
				{
					for(j=0; j<pDoc->m_SizeWidthDoc; j++)
					{
						TrinData[i][j] = pDoc->m_MapDataDoc[j + (i * pDoc->m_SizeWidthDoc)];
					}
				}

				pDoc->m_MapDataDoc.SetSize(NewMapDlg.m_SizeWidth*NewMapDlg.m_SizeHeight);

				for(i=0; i<NewMapDlg.m_SizeHeight; i++)
				{
					for(j=0; j<NewMapDlg.m_SizeWidth; j++)
					{
						pDoc->m_MapDataDoc[j+(i*NewMapDlg.m_SizeWidth)].TrinID = 0;
						pDoc->m_MapDataDoc[j+(i*NewMapDlg.m_SizeWidth)].TrinNextID = 0;
						pDoc->m_MapDataDoc[j+(i*NewMapDlg.m_SizeWidth)].TrinHigh = 0;
						pDoc->m_MapDataDoc[j+(i*NewMapDlg.m_SizeWidth)].TrinMove = FALSE;
					}
				}

				for(i=0; i<((NewMapDlg.m_SizeHeight > pDoc->m_SizeHeightDoc)?pDoc->m_SizeHeightDoc:NewMapDlg.m_SizeHeight); i++)
				{
					for(j=0; j<((NewMapDlg.m_SizeWidth > pDoc->m_SizeWidthDoc)?pDoc->m_SizeWidthDoc:NewMapDlg.m_SizeWidth); j++)
					{
						pDoc->m_MapDataDoc[j + (i * NewMapDlg.m_SizeWidth)] = TrinData[i][j];
					}
				}
				
				for(i=0; i<pDoc->m_SizeWidthDoc; i++)
					delete [] TrinData[i];
				delete [] TrinData;

				pDoc->m_SizeWidthDoc = NewMapDlg.m_SizeWidth;
				pDoc->m_SizeHeightDoc = NewMapDlg.m_SizeHeight;
			}
			
			pDoc->m_FileNameDoc = NewMapDlg.m_FileName;

			Invalidate(FALSE);
		}
	}

	//* 새 맵이 없을 경우
	else
		AfxMessageBox("설정을 변경할 맵이 없습니다.");
}

void CMainFrame::OnSavefile() // *파일을 저장할 때
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	CString FileName = pDoc->m_FileNameDoc + ".roa";
	FILE *SaveMapFile;

	// *저장할 맵 파일이 있으면
	if(pView->m_IsMapExist == TRUE)
	{
		// *같은 이름의 파일이 있으면
		if(SaveMapFile = fopen(FileName, "r"))
		{
			if(IDOK == AfxMessageBox("이미 같은 이름의 파일이 있습니다. 정말 갱신 하겠습니까?", MB_ICONEXCLAMATION | MB_OKCANCEL))
			{
				fclose(SaveMapFile);
				SaveMapFile = fopen(FileName, "w");
				
				// *지형 사이즈
				fprintf(SaveMapFile, "%d %d ", pDoc->m_SizeWidthDoc, pDoc->m_SizeHeightDoc);
				
				// *지형 데이터
				for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
					fprintf(SaveMapFile, "%d %d %d %d ",(pDoc->m_MapDataDoc[i]).TrinID, (pDoc->m_MapDataDoc[i]).TrinNextID,
							(pDoc->m_MapDataDoc[i]).TrinHigh, (pDoc->m_MapDataDoc[i]).TrinMove);

				// *구역 사이즈
				fprintf(SaveMapFile, "%d ", pDoc->m_LocationDoc.GetSize());
				
				// *구역 데이터
				for(i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
					fprintf(SaveMapFile, "%d %s %d %d %d %d %d ", pDoc->m_LocationDoc[i].LocationID, pDoc->m_LocationDoc[i].LocationName, pDoc->m_LocationDoc[i].LocationColor,
							pDoc->m_LocationDoc[i].LocationX1, pDoc->m_LocationDoc[i].LocationY1, pDoc->m_LocationDoc[i].LocationX2, pDoc->m_LocationDoc[i].LocationY2);
				
				fclose(SaveMapFile);
				AfxMessageBox(FileName + " 가 저장 되었습니다.");
			}
		} // *end 같은 이름의 파일이 있으면
		
		// *같은 이름의 파일이 없으면
		else
		{
			SaveMapFile = fopen(FileName, "w");
			
			// *지형 사이즈
			fprintf(SaveMapFile, "%d %d ", pDoc->m_SizeWidthDoc, pDoc->m_SizeHeightDoc);
			
			// *지형 데이터
			for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
				fprintf(SaveMapFile, "%d %d %d %d ",(pDoc->m_MapDataDoc[i]).TrinID, (pDoc->m_MapDataDoc[i]).TrinNextID,
				(pDoc->m_MapDataDoc[i]).TrinHigh, (pDoc->m_MapDataDoc[i]).TrinMove);
			
			// *구역 사이즈
			fprintf(SaveMapFile, "%d ", pDoc->m_LocationDoc.GetSize());
			
			// *구역 데이터
			for(i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
				fprintf(SaveMapFile, "%d %s %d %d %d %d %d ", pDoc->m_LocationDoc[i].LocationID, pDoc->m_LocationDoc[i].LocationName, pDoc->m_LocationDoc[i].LocationColor,
						pDoc->m_LocationDoc[i].LocationX1, pDoc->m_LocationDoc[i].LocationY1, pDoc->m_LocationDoc[i].LocationX2, pDoc->m_LocationDoc[i].LocationY2);
			
			fclose(SaveMapFile);
			AfxMessageBox(FileName + " 가 저장 되었습니다.");
		} // *end 같은 이름의 파일이 없으면
	}// *end 저장할 맵 파일이 있으면

	// *저장할 맵 파일이 없으면
	else
		AfxMessageBox("저장할 맵이 없습니다.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnOpenfile() 
{	
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	CString FileName = "";
	FILE *OpenMapFile;

	// *현재 맵 파일이 없으면
	if(pView->m_IsMapExist == FALSE)
	{
		CFileOpenDialog OpenDlg;

		if(OpenDlg.DoModal() == IDOK)
		{
			FileName = OpenDlg.m_FileName + ".roa";
			if(OpenMapFile = fopen(FileName, "r"))
			{
				fscanf(OpenMapFile, "%d %d ",&(pDoc->m_SizeWidthDoc), &(pDoc->m_SizeHeightDoc));

				// *지형 사이즈
				pDoc->m_MapDataDoc.SetSize((pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc));
				
				// *지형 데이터
				for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
					fscanf(OpenMapFile, "%d %d %d %d ", &(pDoc->m_MapDataDoc[i]).TrinID, &(pDoc->m_MapDataDoc[i]).TrinNextID,
						   &(pDoc->m_MapDataDoc[i]).TrinHigh, &(pDoc->m_MapDataDoc[i]).TrinMove);

				// *구역 사이즈
				int LocSize;
				fscanf(OpenMapFile, "%d ", &LocSize);
				pDoc->m_LocationDoc.SetSize(LocSize);
				
				// *구역 데이터
				for(i=0; i<LocSize; i++)
				{
					char LocName[20];
					fscanf(OpenMapFile, "%d %s %d %d %d %d %d ", &(pDoc->m_LocationDoc[i].LocationID), LocName, 
						   &(pDoc->m_LocationDoc[i].LocationColor), &(pDoc->m_LocationDoc[i].LocationX1), &(pDoc->m_LocationDoc[i].LocationY1), 
						   &(pDoc->m_LocationDoc[i].LocationX2), &(pDoc->m_LocationDoc[i].LocationY2));
					pDoc->m_LocationDoc[i].LocationName = LocName;
				}
				fclose(OpenMapFile);
				pView->m_IsMapExist = TRUE;
				Invalidate(FALSE);
				AfxMessageBox(FileName + " 를 불러왔습니다.");
			}
			else
				AfxMessageBox(FileName + " 는 없는 파일입니다.");
		}
	} // *end 현재 맵 파일이 없으면
	
	// *현재 맵 파일이 있으면
	else
	{
		if(IDOK == AfxMessageBox("작업중인 맵이 있습니다. 정말 불러오시겠습니까?\n작업중인 맵은 저장되지 않습니다.", MB_ICONEXCLAMATION | MB_OKCANCEL))
		{
			CFileOpenDialog OpenDlg;

			if(OpenDlg.DoModal() == IDOK)
			{
				FileName = OpenDlg.m_FileName + ".roa";
				
				if(OpenMapFile = fopen(FileName, "r"))
				{
					fscanf(OpenMapFile, "%d %d ",&(pDoc->m_SizeWidthDoc), &(pDoc->m_SizeHeightDoc));

					// *지형 사이즈
					pDoc->m_MapDataDoc.SetSize((pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc));
					
					// *지형 데이터
					for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
						fscanf(OpenMapFile, "%d %d %d %d ", &(pDoc->m_MapDataDoc[i]).TrinID, &(pDoc->m_MapDataDoc[i]).TrinNextID,
						&(pDoc->m_MapDataDoc[i]).TrinHigh, &(pDoc->m_MapDataDoc[i]).TrinMove);
					
					// *구역 사이즈
					int LocSize;
					fscanf(OpenMapFile, "%d ", &LocSize);
					pDoc->m_LocationDoc.SetSize(LocSize);
					
					// *구역 데이터
					for(i=0; i<LocSize; i++)
					{
						char LocName[20];
						fscanf(OpenMapFile, "%d %s %d %d %d %d %d ", &(pDoc->m_LocationDoc[i].LocationID), LocName, 
							&(pDoc->m_LocationDoc[i].LocationColor), &(pDoc->m_LocationDoc[i].LocationX1), &(pDoc->m_LocationDoc[i].LocationY1), 
							&(pDoc->m_LocationDoc[i].LocationX2), &(pDoc->m_LocationDoc[i].LocationY2));
						pDoc->m_LocationDoc[i].LocationName = LocName;
					}				
					
					fclose(OpenMapFile);
					pView->m_IsMapExist = TRUE;
					Invalidate(FALSE);
					AfxMessageBox(FileName + " 를 불러왔습니다.");
				}

				else
					AfxMessageBox(FileName + " 는 없는 파일입니다.");
			}
		}
	} // *end 현재 맵 파일이 있으면
}

void CMainFrame::OnMapEdge() 
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	// *새 맵이 있을 경우
	if(pView->m_IsMapExist == TRUE)
	{
		for(int i=0; i<(pDoc->m_SizeWidthDoc)-1; i++)
			pDoc->m_MapDataDoc[i].TrinMove = FALSE;
		for(i=0; i<(pDoc->m_SizeHeightDoc)-1; i++)
			pDoc->m_MapDataDoc[i*pDoc->m_SizeHeightDoc].TrinMove = FALSE;
		for(i=1; i<(pDoc->m_SizeHeightDoc)+1; i++)
			pDoc->m_MapDataDoc[i*pDoc->m_SizeWidthDoc-1].TrinMove = FALSE;
		for(i=(pDoc->m_SizeHeightDoc)*(pDoc->m_SizeWidthDoc-1); i<(pDoc->m_SizeHeightDoc)*(pDoc->m_SizeWidthDoc); i++)
			pDoc->m_MapDataDoc[i].TrinMove = FALSE;

		Invalidate(FALSE);
	}

	//* 새 맵이 없을 경우
	else
		AfxMessageBox("설정을 변경할 맵이 없습니다.");
}

void CMainFrame::OnTrinCopy() 
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist)
	{
		if(pView->m_SelectDlgBrush == 0)
		{
			pDoc->m_Copy.RemoveAll();

			int o,p;
			pView->m_CopyWidth = ((pView->m_SelectX > pView->m_MouseDragX)?pView->m_SelectX-pView->m_MouseDragX:pView->m_MouseDragX-pView->m_SelectX)+1;

			pView->m_CopyHeight = ((pView->m_SelectY > pView->m_MouseDragY)?pView->m_SelectY-pView->m_MouseDragY:pView->m_MouseDragY-pView->m_SelectY)+1;
			
			pDoc->m_Copy.SetSize(pView->m_CopyHeight*pView->m_CopyWidth);
			
			o = 0;
			for(int i=((pView->m_SelectY > pView->m_MouseDragY)?pView->m_MouseDragY:pView->m_SelectY); i<=((pView->m_SelectY < pView->m_MouseDragY)?pView->m_MouseDragY:pView->m_SelectY); i++)
			{
				p = 0;
				for(int j=((pView->m_SelectX > pView->m_MouseDragX)?pView->m_MouseDragX:pView->m_SelectX); j<=((pView->m_SelectX < pView->m_MouseDragX)?pView->m_MouseDragX:pView->m_SelectX); j++)
				{
					pDoc->m_Copy[p+(o*pView->m_CopyWidth)] = pDoc->m_MapDataDoc[j+(i*pDoc->m_SizeWidthDoc)];
					p++;
				}
				o++;
			}
		}
		else
			AfxMessageBox("선택 브러시를 선택해주세요.");
	}
	else
		AfxMessageBox("맵이 없습니다.");
}

void CMainFrame::OnTrinPaste() 
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist)
	{
		if(pView->m_SelectDlgBrush == 0)
		{
			if(pDoc->m_Copy.GetSize() > 0)
			{
				for(int i=0; i<pView->m_CopyHeight; i++)
				{
					for(int j=0; j<pView->m_CopyWidth; j++)
					{
						if((pDoc->m_SizeHeightDoc) > (pView->m_SelectY+i) && (pDoc->m_SizeWidthDoc) > (pView->m_SelectX+j))
							pDoc->m_MapDataDoc[(pView->m_SelectX+j)+((pView->m_SelectY+i)*pDoc->m_SizeWidthDoc)] = pDoc->m_Copy[j+(i*pView->m_CopyWidth)];
					}
				}

				pView->m_MouseDragX = pView->m_SelectX + pView->m_CopyWidth - 1;
				pView->m_MouseDragY = pView->m_SelectY + pView->m_CopyHeight - 1;
				Invalidate(FALSE);
			}
			else
				AfxMessageBox("복사된 데이터가 없습니다.");
		}
		else
			AfxMessageBox("선택 브러시를 선택해주세요.");
	}
	else
		AfxMessageBox("맵이 없습니다.");
}

void CMainFrame::OnTrinCut() 
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist)
	{
		if(pView->m_SelectDlgBrush == 0)
		{
			pDoc->m_Copy.RemoveAll();
			
			int o,p;
			pView->m_CopyWidth = ((pView->m_SelectX > pView->m_MouseDragX)?pView->m_SelectX-pView->m_MouseDragX:pView->m_MouseDragX-pView->m_SelectX)+1;
			
			pView->m_CopyHeight = ((pView->m_SelectY > pView->m_MouseDragY)?pView->m_SelectY-pView->m_MouseDragY:pView->m_MouseDragY-pView->m_SelectY)+1;
			
			pDoc->m_Copy.SetSize(pView->m_CopyHeight*pView->m_CopyWidth);
			
			o = 0;
			for(int i=((pView->m_SelectY > pView->m_MouseDragY)?pView->m_MouseDragY:pView->m_SelectY); i<=((pView->m_SelectY < pView->m_MouseDragY)?pView->m_MouseDragY:pView->m_SelectY); i++)
			{
				p = 0;
				for(int j=((pView->m_SelectX > pView->m_MouseDragX)?pView->m_MouseDragX:pView->m_SelectX); j<=((pView->m_SelectX < pView->m_MouseDragX)?pView->m_MouseDragX:pView->m_SelectX); j++)
				{
					pDoc->m_Copy[p+(o*pView->m_CopyWidth)] = pDoc->m_MapDataDoc[j+(i*pDoc->m_SizeWidthDoc)];
					p++;
				}
				o++;
			}
			
			for(i=((pView->m_SelectY > pView->m_MouseDragY)?pView->m_MouseDragY:pView->m_SelectY); i<=((pView->m_SelectY < pView->m_MouseDragY)?pView->m_MouseDragY:pView->m_SelectY); i++)
			{
				for(int j=((pView->m_SelectX > pView->m_MouseDragX)?pView->m_MouseDragX:pView->m_SelectX); j<=((pView->m_SelectX < pView->m_MouseDragX)?pView->m_MouseDragX:pView->m_SelectX); j++)
				{
					pDoc->m_MapDataDoc[j+(i*pDoc->m_SizeWidthDoc)].TrinID = 0;
					pDoc->m_MapDataDoc[j+(i*pDoc->m_SizeWidthDoc)].TrinNextID = 0;
					pDoc->m_MapDataDoc[j+(i*pDoc->m_SizeWidthDoc)].TrinHigh = 0;
					pDoc->m_MapDataDoc[j+(i*pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
				}
			}
		}
		else
			AfxMessageBox("선택 브러시를 선택해주세요.");
	}
	else
		AfxMessageBox("맵이 없습니다.");
}

