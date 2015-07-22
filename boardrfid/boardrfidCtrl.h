#pragma once

// boardrfidCtrl.h : Declaration of the CboardrfidCtrl ActiveX Control class.


// CboardrfidCtrl : See boardrfidCtrl.cpp for implementation.

class CSerialPort;
class CboardrfidCtrl : public COleControl
{
	DECLARE_DYNCREATE(CboardrfidCtrl)

// Constructor
public:
	CboardrfidCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// Implementation
protected:
	~CboardrfidCtrl();

	DECLARE_OLECREATE_EX(CboardrfidCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CboardrfidCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CboardrfidCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CboardrfidCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
        dispidInit = 5L,
        dispidSetBufferSize = 4L,
        dispidTakeOneRfid = 3L,
        dispidOpenListenThread = 2L,
        dispidInitPort = 1L
    };
protected:
    LONG InitPort(LONG nCom);
    LONG OpenListenThread(void);
    BSTR TakeOneRfid(void);

    CSerialPort *m_mySerialPort;
    LONG SetBufferSize(LONG nSize);
    LONG Init(LONG nCom, LONG nBaudRate = 57600);
};

