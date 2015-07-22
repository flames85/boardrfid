// boardrfid.cpp : Implementation of CboardrfidApp and DLL registration.

#include "stdafx.h"
#include "boardrfid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CboardrfidApp theApp;

const GUID CDECL _tlid = { 0x256872EE, 0x2BDC, 0x43A9, { 0xA1, 0xAB, 0xAD, 0x66, 0xE2, 0xE2, 0x41, 0x7C } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CboardrfidApp::InitInstance - DLL initialization

BOOL CboardrfidApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CboardrfidApp::ExitInstance - DLL termination

int CboardrfidApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
