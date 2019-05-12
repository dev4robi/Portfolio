// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RuneStone.h"

#include "MainFrm.h"

// *�߰��� �ش� ����
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
	
	// *�ȷ�Ʈ ���̾�α� ������ �ʱ�ȭ
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
	
	// ���� ����
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// ���¹� ����
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	HICON icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(icon, TRUE);   // ū �������� �����մϴ�.
	SetIcon(icon, FALSE);  // ���� �������� �����մϴ�. 

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


void CMainFrame::OnTrinDialog() // * ���� �귯�� ���� ���ý�
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
			
			// �� �Ʒ��� �ʱⰪ ����
			//
			//
			//

			m_pTrinDlg->Create(IDD_TRINDIALOG);
			m_pTrinDlg->ShowWindow(SW_SHOW);		
		}
	}

	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnNpcDialog() // *NPC �귯�� ���� ���ý�
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

			// �� �Ʒ��� �ʱⰪ ����
			//
			//
			//

			m_pNpcDlg->Create(IDD_NPCDIALOG);
			m_pNpcDlg->ShowWindow(SW_SHOW);
		}
	}

	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnItemDialog() // *������ �귯�� ���� ���ý�
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
			
			// �� �Ʒ��� �ʱⰪ ����
			//
			//
			//
			
			m_pItemDlg->Create(IDD_ITEMDIALOG);
			m_pItemDlg->ShowWindow(SW_SHOW);
		}
	}

	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnMonDialog() // *���� �귯�� ���� ���ý�
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
			
			// �� �Ʒ��� �ʱⰪ ����
			//
			//
			//
			
			m_pMonDlg->Create(IDD_MONDIALOG);
			m_pMonDlg->ShowWindow(SW_SHOW);
		}
	}

	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnLocateDialog() // *���� �귯�� ���� ���ý�
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
			
			// �� �Ʒ��� �ʱⰪ ����
			//
			//
			//
			
			m_pLocDlg->Create(IDD_LOCATEDIALOG);
			m_pLocDlg->ShowWindow(SW_SHOW);

			for(int i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
			{
				CString str;
				str = pDoc->m_LocationDoc[i].LocationName; // �� ����
				m_pLocDlg->m_LocListBox.AddString(str);
			}
		}

		pView->m_ShowLoc = TRUE;
		pView->m_SelectDlgLoc = NONE;

		// *���� �ȷ�Ʈ ���� �ٸ� �ȷ�Ʈ ����
		if(m_pTrinDlg != NULL)
		{
			// *������ �귯�� / ���� �������� �ʱ�ȭ
			pView->m_SelectDlgBrush = -1;
			pView->m_SelectDlgOpSize = -1;
			pView->m_SelectDlgOpType = -1;
			
			// *������ �缳��
			m_pTrinDlg->m_pTrinMainFrame = NULL;
			m_pTrinDlg->DestroyWindow();
			m_pTrinDlg = NULL;
		}

		if(m_pNpcDlg != NULL)
		{
			// *������ �缳��
			m_pNpcDlg->m_pNpcMainFrame = NULL;
			m_pNpcDlg->DestroyWindow();
			m_pNpcDlg = NULL;
		}

		if(m_pItemDlg != NULL)
		{
			// *������ �缳��	
			m_pItemDlg->m_pItemMainFrame = NULL;
			m_pItemDlg->DestroyWindow();
			m_pItemDlg = NULL;
		}

		if(m_pMonDlg != NULL)
		{
			// *������ �缳��	
			m_pMonDlg->m_pMonMainFrame = NULL;
			m_pMonDlg->DestroyWindow();
			m_pMonDlg = NULL;
		}

		Invalidate(FALSE);
	}
	
	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
	
}

void CMainFrame::OnFileNew() // *�� �� ����� �޴���/���� Ŭ����
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	// *������� ���� ������
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

	// *������� ���� ������
	else
	{
		if(IDOK == AfxMessageBox("���� ����� �Ǹ� ���� �� ������ ������ϴ�. ���� ����ڽ��ϱ�?", MB_ICONEXCLAMATION | MB_OKCANCEL))
		{
			// *���� ���̾�α� �ݱ�
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
			// *end ���� ���̾�α� �ݱ�

			// *���Ǿ� ���̾�α� �ݱ�
			if(m_pNpcDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pNpcDlg->CloseWindow();
				delete m_pNpcDlg;
				m_pNpcDlg = NULL;
			}
			// *end ���Ǿ� ���̾�α� �ݱ�

			// *������ ���̾�α� �ݱ�
			if(m_pItemDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pItemDlg->CloseWindow();
				delete m_pItemDlg;
				m_pItemDlg = NULL;
			}
			// *end ������ ���̾�α� �ݱ�

			// *���� ���̾�α� �ݱ�
			if(m_pMonDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pMonDlg->CloseWindow();
				delete m_pMonDlg;
				m_pMonDlg = NULL;
			}
			// *end ���� ���̾�α� �ݱ�	

			// *�̴ϸ� ���̾�α� �ݱ�
			if(m_pMiniDlg != NULL)
			{
				CRuneStoneView *pView = (CRuneStoneView*)GetActiveView();
				
				m_pMiniDlg->CloseWindow();
				delete m_pMiniDlg;
				m_pMiniDlg = NULL;
			}
			// *end �̴ϸ� ���̾�α� �ݱ�	



			//for(int i=0; i<(pDoc->m_SizeWidthDoc) * (pDoc->m_SizeHeightDoc); i++)
			//pDoc->m_MapDataDoc.RemoveAt(i); // *CHECK �� �κп��� ������ ������ �߻��ϴ� �� �ϴ�.
			
			CNewMapDIALOG NewMapDlg;
			int DlgReturn = NewMapDlg.DoModal();

			if(DlgReturn == IDOK)
			{
				// *���� ������ ������ �� �� ����� ���̾�α� ����
				pView->m_IsMapExist = FALSE;
				pDoc->m_MapDataDoc.RemoveAll();
				UpdateData(TRUE);
			
				// *���� ���� ���̾�α� ���� �� ��� ���ý� ��ũ�ѹ� ����
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

void CMainFrame::OnShowgrid() // *���� ���̱� ���� Ŭ����
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	if(pView->m_IsMapExist == TRUE)
	{
		pView->m_ShowGrid = !(pView->m_ShowGrid);

		Invalidate(FALSE);
	}

	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnUpdateShowgrid(CCmdUI* pCmdUI) // *���� ���̱� ������ ��
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	pCmdUI->SetCheck(pView->m_ShowGrid == TRUE);
}

void CMainFrame::OnShowcoll() // *�浹 ���̱� ���� Ŭ����
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	if(pView->m_IsMapExist == TRUE)
	{	
		pView->m_ShowColl = !(pView->m_ShowColl);
		
		Invalidate(FALSE);
	}

	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnUpdateShowcoll(CCmdUI* pCmdUI) // *�浹 ���̱� ������ ��
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	pCmdUI->SetCheck(pView->m_ShowColl == TRUE);
}

void CMainFrame::OnShowhigh() // *������ ���̱� ���� Ŭ����
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	if(pView->m_IsMapExist == TRUE)
	{
		pView->m_ShowHigh = !(pView->m_ShowHigh);
	
		Invalidate(FALSE);
	}

	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnUpdateShowhigh(CCmdUI* pCmdUI) // *������ ���̱� ������ ��
{
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	
	pCmdUI->SetCheck(pView->m_ShowHigh == TRUE);
}

void CMainFrame::OnMinimapDialog() // *�̴ϸ� ���� Ŭ����
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
			
			// �� �Ʒ��� �ʱⰪ ����
			//
			//
			//
			
			m_pMiniDlg->Create(IDD_MINIDIALOG);
			m_pMiniDlg->ShowWindow(SW_SHOW);
		}
	}
	
	else
		AfxMessageBox("���� �� ���� ������ �մϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnMapSetup() // *�� ���� �޴��� Ŭ����
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	// *�� ���� ���� ���
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

	//* �� ���� ���� ���
	else
		AfxMessageBox("������ ������ ���� �����ϴ�.");
}

void CMainFrame::OnSavefile() // *������ ������ ��
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	CString FileName = pDoc->m_FileNameDoc + ".roa";
	FILE *SaveMapFile;

	// *������ �� ������ ������
	if(pView->m_IsMapExist == TRUE)
	{
		// *���� �̸��� ������ ������
		if(SaveMapFile = fopen(FileName, "r"))
		{
			if(IDOK == AfxMessageBox("�̹� ���� �̸��� ������ �ֽ��ϴ�. ���� ���� �ϰڽ��ϱ�?", MB_ICONEXCLAMATION | MB_OKCANCEL))
			{
				fclose(SaveMapFile);
				SaveMapFile = fopen(FileName, "w");
				
				// *���� ������
				fprintf(SaveMapFile, "%d %d ", pDoc->m_SizeWidthDoc, pDoc->m_SizeHeightDoc);
				
				// *���� ������
				for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
					fprintf(SaveMapFile, "%d %d %d %d ",(pDoc->m_MapDataDoc[i]).TrinID, (pDoc->m_MapDataDoc[i]).TrinNextID,
							(pDoc->m_MapDataDoc[i]).TrinHigh, (pDoc->m_MapDataDoc[i]).TrinMove);

				// *���� ������
				fprintf(SaveMapFile, "%d ", pDoc->m_LocationDoc.GetSize());
				
				// *���� ������
				for(i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
					fprintf(SaveMapFile, "%d %s %d %d %d %d %d ", pDoc->m_LocationDoc[i].LocationID, pDoc->m_LocationDoc[i].LocationName, pDoc->m_LocationDoc[i].LocationColor,
							pDoc->m_LocationDoc[i].LocationX1, pDoc->m_LocationDoc[i].LocationY1, pDoc->m_LocationDoc[i].LocationX2, pDoc->m_LocationDoc[i].LocationY2);
				
				fclose(SaveMapFile);
				AfxMessageBox(FileName + " �� ���� �Ǿ����ϴ�.");
			}
		} // *end ���� �̸��� ������ ������
		
		// *���� �̸��� ������ ������
		else
		{
			SaveMapFile = fopen(FileName, "w");
			
			// *���� ������
			fprintf(SaveMapFile, "%d %d ", pDoc->m_SizeWidthDoc, pDoc->m_SizeHeightDoc);
			
			// *���� ������
			for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
				fprintf(SaveMapFile, "%d %d %d %d ",(pDoc->m_MapDataDoc[i]).TrinID, (pDoc->m_MapDataDoc[i]).TrinNextID,
				(pDoc->m_MapDataDoc[i]).TrinHigh, (pDoc->m_MapDataDoc[i]).TrinMove);
			
			// *���� ������
			fprintf(SaveMapFile, "%d ", pDoc->m_LocationDoc.GetSize());
			
			// *���� ������
			for(i=0; i<pDoc->m_LocationDoc.GetSize(); i++)
				fprintf(SaveMapFile, "%d %s %d %d %d %d %d ", pDoc->m_LocationDoc[i].LocationID, pDoc->m_LocationDoc[i].LocationName, pDoc->m_LocationDoc[i].LocationColor,
						pDoc->m_LocationDoc[i].LocationX1, pDoc->m_LocationDoc[i].LocationY1, pDoc->m_LocationDoc[i].LocationX2, pDoc->m_LocationDoc[i].LocationY2);
			
			fclose(SaveMapFile);
			AfxMessageBox(FileName + " �� ���� �Ǿ����ϴ�.");
		} // *end ���� �̸��� ������ ������
	}// *end ������ �� ������ ������

	// *������ �� ������ ������
	else
		AfxMessageBox("������ ���� �����ϴ�.", MB_ICONEXCLAMATION);
}

void CMainFrame::OnOpenfile() 
{	
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();
	CString FileName = "";
	FILE *OpenMapFile;

	// *���� �� ������ ������
	if(pView->m_IsMapExist == FALSE)
	{
		CFileOpenDialog OpenDlg;

		if(OpenDlg.DoModal() == IDOK)
		{
			FileName = OpenDlg.m_FileName + ".roa";
			if(OpenMapFile = fopen(FileName, "r"))
			{
				fscanf(OpenMapFile, "%d %d ",&(pDoc->m_SizeWidthDoc), &(pDoc->m_SizeHeightDoc));

				// *���� ������
				pDoc->m_MapDataDoc.SetSize((pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc));
				
				// *���� ������
				for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
					fscanf(OpenMapFile, "%d %d %d %d ", &(pDoc->m_MapDataDoc[i]).TrinID, &(pDoc->m_MapDataDoc[i]).TrinNextID,
						   &(pDoc->m_MapDataDoc[i]).TrinHigh, &(pDoc->m_MapDataDoc[i]).TrinMove);

				// *���� ������
				int LocSize;
				fscanf(OpenMapFile, "%d ", &LocSize);
				pDoc->m_LocationDoc.SetSize(LocSize);
				
				// *���� ������
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
				AfxMessageBox(FileName + " �� �ҷ��Խ��ϴ�.");
			}
			else
				AfxMessageBox(FileName + " �� ���� �����Դϴ�.");
		}
	} // *end ���� �� ������ ������
	
	// *���� �� ������ ������
	else
	{
		if(IDOK == AfxMessageBox("�۾����� ���� �ֽ��ϴ�. ���� �ҷ����ðڽ��ϱ�?\n�۾����� ���� ������� �ʽ��ϴ�.", MB_ICONEXCLAMATION | MB_OKCANCEL))
		{
			CFileOpenDialog OpenDlg;

			if(OpenDlg.DoModal() == IDOK)
			{
				FileName = OpenDlg.m_FileName + ".roa";
				
				if(OpenMapFile = fopen(FileName, "r"))
				{
					fscanf(OpenMapFile, "%d %d ",&(pDoc->m_SizeWidthDoc), &(pDoc->m_SizeHeightDoc));

					// *���� ������
					pDoc->m_MapDataDoc.SetSize((pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc));
					
					// *���� ������
					for(int i=0; i<(pDoc->m_SizeWidthDoc)*(pDoc->m_SizeHeightDoc); i++)
						fscanf(OpenMapFile, "%d %d %d %d ", &(pDoc->m_MapDataDoc[i]).TrinID, &(pDoc->m_MapDataDoc[i]).TrinNextID,
						&(pDoc->m_MapDataDoc[i]).TrinHigh, &(pDoc->m_MapDataDoc[i]).TrinMove);
					
					// *���� ������
					int LocSize;
					fscanf(OpenMapFile, "%d ", &LocSize);
					pDoc->m_LocationDoc.SetSize(LocSize);
					
					// *���� ������
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
					AfxMessageBox(FileName + " �� �ҷ��Խ��ϴ�.");
				}

				else
					AfxMessageBox(FileName + " �� ���� �����Դϴ�.");
			}
		}
	} // *end ���� �� ������ ������
}

void CMainFrame::OnMapEdge() 
{
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)this->GetActiveDocument();
	CRuneStoneView *pView = (CRuneStoneView*)this->GetActiveView();

	// *�� ���� ���� ���
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

	//* �� ���� ���� ���
	else
		AfxMessageBox("������ ������ ���� �����ϴ�.");
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
			AfxMessageBox("���� �귯�ø� �������ּ���.");
	}
	else
		AfxMessageBox("���� �����ϴ�.");
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
				AfxMessageBox("����� �����Ͱ� �����ϴ�.");
		}
		else
			AfxMessageBox("���� �귯�ø� �������ּ���.");
	}
	else
		AfxMessageBox("���� �����ϴ�.");
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
			AfxMessageBox("���� �귯�ø� �������ּ���.");
	}
	else
		AfxMessageBox("���� �����ϴ�.");
}

