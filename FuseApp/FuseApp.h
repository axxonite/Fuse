// FuseApp.h : main header file for the Fuse application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		    // main symbols

#include "Editor.h"
//
// Main Fuse application.
//

class CFuseApp : public CWinApp
{

public:

	//---------------------------------------------------------------------------------------------

	CFuseApp();
	virtual ~CFuseApp();

	//---------------------------------------------------------------------------------------------

	// Called when app exits.
	virtual int		ExitInstance();

	// Called when app starts.
	virtual BOOL	InitInstance();

	//---------------------------------------------------------------------------------------------

	// Returns the editor engine.
	inline	CEditor*	GetEditor();

private:

	CEditor*	m_pEditor;

	afx_msg void OnAppAbout();

	virtual	BOOL OnIdle( LONG lCount );

	DECLARE_MESSAGE_MAP()

};

inline CEditor* CFuseApp::GetEditor()
{
	return m_pEditor;
}

extern CFuseApp theApp;