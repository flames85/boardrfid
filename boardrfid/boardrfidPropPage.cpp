// boardrfidPropPage.cpp : Implementation of the CboardrfidPropPage property page class.

#include "stdafx.h"
#include "boardrfid.h"
#include "boardrfidPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CboardrfidPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CboardrfidPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CboardrfidPropPage, "BOARDRFID.boardrfidPropPage.1",
	0x21a3d070, 0xe6fa, 0x4f77, 0x81, 0x49, 0xb4, 0x22, 0xfe, 0x9f, 0xcb, 0xe5)



// CboardrfidPropPage::CboardrfidPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CboardrfidPropPage

BOOL CboardrfidPropPage::CboardrfidPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_BOARDRFID_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CboardrfidPropPage::CboardrfidPropPage - Constructor

CboardrfidPropPage::CboardrfidPropPage() :
	COlePropertyPage(IDD, IDS_BOARDRFID_PPG_CAPTION)
{
}



// CboardrfidPropPage::DoDataExchange - Moves data between page and properties

void CboardrfidPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CboardrfidPropPage message handlers
