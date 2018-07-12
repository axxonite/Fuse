// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "ChildView.h"

//
// Main MFC frame.
//

class CMainFrame : public CFrameWnd
{
	
public:

	//---------------------------------------------------------------------------------------------

	CMainFrame();
	virtual ~CMainFrame();

	//---------------------------------------------------------------------------------------------

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	//---------------------------------------------------------------------------------------------

private:

	DECLARE_DYNAMIC(CMainFrame)
	
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSetFocus(CWnd *pOldWnd);

	DECLARE_MESSAGE_MAP()

};