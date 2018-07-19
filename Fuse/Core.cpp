// Core.cpp

#include "stdafx.h"
#include "Core.h"
#include "Scene.h"
#include "HALRenderer.h"

CCore::CCore() :
	m_BackgroundColor	( COLOR_BLACK	),
	m_bInitialized		( false			)
{
}

void CCore::Init( u32 uFrameBufferWidth, u32 uFrameBufferHeight, u32 uFlags, HWND hRenderWnd, HWND hMainWnd )
{
	Trace( "\n" );
	Trace( "Fuse Engine v1.0\n" );
	Trace( "=====================" );
	Trace( "\n" );
	Trace( "\n" );

	m_IntputSystem.Init( hMainWnd, !( uFlags & CHALRenderer::INIT_WINDOWED ) );
	m_Renderer.Init( uFrameBufferWidth, uFrameBufferHeight, uFlags, hRenderWnd );

	m_Camera.Init();
	m_Camera.SetScene( &m_Scene );
	m_Scene.GetRootNode()->AttachChild( &m_Camera );
	m_bInitialized = true;
}

void CCore::Render()
{
	m_Renderer.BeginScene();

	m_Renderer.ClearScreen( CHALRenderer::CLEAR_ALL, m_BackgroundColor );

 	m_Camera.Render();
	//g_pEngineStats->Render( m_pDefaultCamera );

	m_Renderer.EndScene();
	m_Renderer.FlipFrame();
}

void CCore::ResizeFrameBuffer( u32 uFrameBufferWidth, u32 uFrameBufferHeight )
{
	if ( g_pRenderer && g_pRenderer->IsInitialized() )
	{
		g_pRenderer->Reset( uFrameBufferWidth, uFrameBufferHeight );
		m_Camera.Init();
	}
}

void CCore::Tick()
{
	CScene::Update();
}

void CCore::TickInput()
{
	m_IntputSystem.Update();
}
