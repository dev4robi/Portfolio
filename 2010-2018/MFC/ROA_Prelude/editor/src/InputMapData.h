#include "MainFrm.h"
#include "RuneStoneDoc.h"

void InputMapData(int X,int Y, int MapID) // *맵 데이터 넣기 함수
{
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CRuneStoneDoc *pDoc = (CRuneStoneDoc*)pMainFrm->GetActiveDocument();

	pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinID = MapID;
	
	switch(MapID)
	{
	case 0:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 1:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 2:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 3:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 4:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 5:
		
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 6:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 7:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 8:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 9:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 10:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 11:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 12:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
		
	case 50:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 51:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 52:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 66:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
		
	case 67:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 68:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 69:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 82:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 83:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 84:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 85:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 98:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 99:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 100:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 101:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 114:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	case 115:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 116:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = FALSE;
		break;
	case 117:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	default:
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinNextID = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinHigh = 0;
		pDoc->m_MapDataDoc[X+Y*(pDoc->m_SizeWidthDoc)].TrinMove = TRUE;
		break;
	}
}