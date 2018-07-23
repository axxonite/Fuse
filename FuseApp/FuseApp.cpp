// FuseApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FuseApp.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "Editor.h"
#include "Res\Resource.h"

BEGIN_MESSAGE_MAP(CFuseApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()

CFuseApp theApp;

CFuseApp::CFuseApp() :
	m_pEditor( nullptr )
{
}

CFuseApp::~CFuseApp()
{
}

int CFuseApp::ExitInstance()
{
	if ( m_pEditor )
	{
		delete m_pEditor;
		m_pEditor = nullptr;
	}
	return CWinApp::ExitInstance();
}

BOOL CFuseApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();
	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	SetRegistryKey(_T("Fuse"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	
	m_pEditor = new CEditor();

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr, nullptr);
	pFrame->SetMenu( nullptr );

	// The one and only window has been initialized, so show and update it
	pFrame->SetWindowPos( nullptr, 0, 0, 640, 480, SWP_NOMOVE | SWP_NOZORDER );
	WINDOWPLACEMENT windowPlacement = {};
	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	windowPlacement.showCmd = SW_SHOWMAXIMIZED;
	windowPlacement.rcNormalPosition = { 0, 0, 640, 480 };
	pFrame->SetWindowPlacement(&windowPlacement);

	m_pEditor->InitInstance( 0, 0, CHALRenderer::INIT_WINDOWED, pFrame, theApp.m_hInstance );
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

BOOL CFuseApp::OnIdle( LONG lCount )
{
	CWinApp::OnIdle( lCount );

	m_pEditor->Tick();

	HWND hWnd = GetForegroundWindow();
	if( hWnd != m_pMainWnd->m_hWnd && GetWindow( hWnd, GW_OWNER ) != m_pMainWnd->m_hWnd )
	{
		Sleep( 1 );
		return true;
	}

	m_pEditor->Render();

	return true;		
}

void CFuseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}