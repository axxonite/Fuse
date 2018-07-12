// AboutDlg.h

#pragma once

#include "Resource.h"

//
// About box dialog.
//

class CAboutDlg : public CDialog
{

public:

	enum { IDD = IDD_ABOUTBOX };

	//---------------------------------------------------------------------------------------------

	CAboutDlg();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};