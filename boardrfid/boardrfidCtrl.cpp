// boardrfidCtrl.cpp : Implementation of the CboardrfidCtrl ActiveX Control class.

#include "stdafx.h"
#include "boardrfid.h"
#include "boardrfidCtrl.h"
#include "boardrfidPropPage.h"
#include "afxdialogex.h"
#include "SerialPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CboardrfidCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CboardrfidCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CboardrfidCtrl, COleControl)
    DISP_FUNCTION_ID(CboardrfidCtrl, "InitPort", dispidInitPort, InitPort, VT_I4, VTS_I4)
    DISP_FUNCTION_ID(CboardrfidCtrl, "OpenListenThread", dispidOpenListenThread, OpenListenThread, VT_I4, VTS_NONE)
    DISP_FUNCTION_ID(CboardrfidCtrl, "TakeOneRfid", dispidTakeOneRfid, TakeOneRfid, VT_BSTR, VTS_NONE)
    DISP_FUNCTION_ID(CboardrfidCtrl, "SetBufferSize", dispidSetBufferSize, SetBufferSize, VT_I4, VTS_I4)
    DISP_FUNCTION_ID(CboardrfidCtrl, "Init", dispidInit, Init, VT_I4, VTS_I4 VTS_I4)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CboardrfidCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CboardrfidCtrl, 1)
	PROPPAGEID(CboardrfidPropPage::guid)
END_PROPPAGEIDS(CboardrfidCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CboardrfidCtrl, "BOARDRFID.boardrfidCtrl.1",
	0x41b20f36, 0x8986, 0x40d4, 0x9d, 0x16, 0xe3, 0x46, 0x6b, 0x13, 0xf3, 0x86)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CboardrfidCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_Dboardrfid = { 0x2E04D3BE, 0x200F, 0x4CA8, { 0x9C, 0xF9, 0x8A, 0x7E, 0x5A, 0xB7, 0x64, 0x5A } };
const IID IID_DboardrfidEvents = { 0x65647ACC, 0xBC07, 0x449E, { 0x9F, 0xBC, 0xA2, 0x99, 0x8, 0x2E, 0xEC, 0xDF } };


// Control type information

static const DWORD _dwboardrfidOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CboardrfidCtrl, IDS_BOARDRFID, _dwboardrfidOleMisc)



// CboardrfidCtrl::CboardrfidCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CboardrfidCtrl

BOOL CboardrfidCtrl::CboardrfidCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_BOARDRFID,
			IDB_BOARDRFID,
			afxRegApartmentThreading,
			_dwboardrfidOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CboardrfidCtrl::CboardrfidCtrl - Constructor

CboardrfidCtrl::CboardrfidCtrl()
{
	InitializeIIDs(&IID_Dboardrfid, &IID_DboardrfidEvents);
	// TODO: Initialize your control's instance data here.
    m_mySerialPort = new CSerialPort();
}



// CboardrfidCtrl::~CboardrfidCtrl - Destructor

CboardrfidCtrl::~CboardrfidCtrl()
{
	// TODO: Cleanup your control's instance data here.
    if(m_mySerialPort)
    {
        delete m_mySerialPort;
        m_mySerialPort = 0;
    }
}



// CboardrfidCtrl::OnDraw - Drawing function

void CboardrfidCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CboardrfidCtrl::DoPropExchange - Persistence support

void CboardrfidCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CboardrfidCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CboardrfidCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control will not be redrawn when making the transition
	// between the active and inactivate state.
	dwFlags |= noFlickerActivate;
	return dwFlags;
}



// CboardrfidCtrl::OnResetState - Reset control to default state

void CboardrfidCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CboardrfidCtrl message handlers


LONG CboardrfidCtrl::InitPort(LONG nCom)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: Add your dispatch handler code here 

    if (!m_mySerialPort->InitPort(nCom, CBR_57600))  
    {
        return -1;
    }
    
    return 0;
}


LONG CboardrfidCtrl::OpenListenThread(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: Add your dispatch handler code here
    if (!m_mySerialPort->OpenListenThread())  
    {
        return -1;
    }
    return 0;
}


BSTR CboardrfidCtrl::TakeOneRfid(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strResult;

    // TODO: Add your dispatch handler code here
    string strRfid;
    if(m_mySerialPort->TakeRfid(strRfid))
    {
        strResult = strRfid.c_str();
    }

    return strResult.AllocSysString();
}


LONG CboardrfidCtrl::SetBufferSize(LONG nSize)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: Add your dispatch handler code here
    return m_mySerialPort->SetBufferSize(nSize);
}


LONG CboardrfidCtrl::Init(LONG nCom, LONG nBaudRate)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: Add your dispatch handler code here

    if (!m_mySerialPort->InitPort(nCom, nBaudRate))  
    {
        return -1;
    }

    return 0;
}
