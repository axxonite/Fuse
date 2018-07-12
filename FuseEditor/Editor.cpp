// Editor.cpp

#include "stdafx.h"
#include "Editor.h"
#include "Primitives.h"
#include "Resource.h"

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

CEditor::CEditor() :
	m_pGrid			( NULL	),
	m_bDesignMode	( true	),
	m_pMainWnd		( NULL	)
{
}

CEditor::~CEditor()
{
	delete m_pGrid;
}

void CEditor::EnableDesignMode( bool bEnable )
{
	m_bDesignMode = bEnable;
	m_StatusBar.ShowWindow( m_bDesignMode ? SW_SHOW : SW_HIDE );
	m_pMainWnd->SetMenu( m_bDesignMode ? &m_Menu : NULL );
	m_pMainWnd->RecalcLayout();
}

void CEditor::InitInstance( u32 uFrameBufferWidth, u32 uFrameBufferHeight, CHALRenderer::eBPP BitsPerPixel, u32 uFlags, CFrameWnd* pMainWnd,
							HINSTANCE hInstance, bool bDesignMode )
{
	m_pMainWnd = pMainWnd;
	m_hInstance = hInstance;
	m_Menu.LoadMenu( MAKEINTRESOURCE( IDR_EDITORMENU ) );
	m_pMainWnd->SetMenu( &m_Menu );
	m_Menu.Detach();
	if ( !bDesignMode )
		EnableDesignMode( false );

	m_Core.Init( uFrameBufferWidth, uFrameBufferHeight, BitsPerPixel, uFlags, m_View.m_hWnd, pMainWnd->m_hWnd );
	m_Core.SetBackgroundColor( 0xFFA1A1A1 );

	CMesh::CMeshData GridData;
	CPrimitives::CreateGrid( 24, 24 , COLOR_BLACK, COLOR_GREY, &GridData );
	m_pGrid = new CMesh();
	m_pGrid->Init( &GridData );
	m_Core.GetScene()->GetRootNode()->AttachRenderable( m_pGrid );

	CCamera* pCamera = m_Core.GetCamera();
	pCamera->SetNearPlane( 0.1f );
	pCamera->SetFarPlane( 1000.0f );
	pCamera->SetViewAngle( ANGLETORADIAN_FACTOR * 54.43f );	// 35mm
	pCamera->SetPosition( 0, 0, -40 );

	m_NavControls.Init( pCamera, m_Core.GetScene()->GetRootNode() );
}

void CEditor::OnCreate( CWnd* pMainWnd )
{
	// create a view to occupy the client area of the frame
	if (!m_View.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), pMainWnd, AFX_IDW_PANE_FIRST, NULL))
		throw EWin32Error();
	
	if ( !m_StatusBar.Create( pMainWnd ) ||
		 !m_StatusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT)))
		throw EWin32Error();
}

void CEditor::Render()
{
	m_Core.Render();
}

void CEditor::Resize( u32 uFrameBufferWidth, u32 uFrameBufferHeight )
{
	if ( m_Core.IsInitialized() )
		m_Core.ResizeFrameBuffer( uFrameBufferWidth, uFrameBufferHeight );
}

void CEditor::Tick()
{
	m_Core.TickInput();
	m_NavControls.Tick();
	m_Core.Tick();

	if ( m_NavControls.GetMode() != CNavControls::eNONE )
		m_StatusBar.SetWindowText( m_NavControls.GetStatus() );
	else
		m_StatusBar.SetWindowText( "Ready." );
}