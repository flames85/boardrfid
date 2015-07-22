#pragma once

// boardrfidPropPage.h : Declaration of the CboardrfidPropPage property page class.


// CboardrfidPropPage : See boardrfidPropPage.cpp for implementation.

class CboardrfidPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CboardrfidPropPage)
	DECLARE_OLECREATE_EX(CboardrfidPropPage)

// Constructor
public:
	CboardrfidPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_BOARDRFID };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

