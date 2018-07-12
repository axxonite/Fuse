// Editor.h

#pragma once

#include "Core.h"
#include "NavControls.h"
#include "Singleton.h"
#include "ChildView.h"

namespace Fuse
{

class CMesh;

#define g_pEditor	CEditor::GetInstance()

//
// Editor engine.
//

class CEditor : public CBaseObject, public CSingleton<CEditor>
{

public :

	//---------------------------------------------------------------------------------------------

	CEditor();
	virtual ~CEditor();

	//---------------------------------------------------------------------------------------------

	void			EnableDesignMode( bool bEnable );

	// Initializes editor core. Specify framebuffer width, height, bits per pixel, initialization flags (see CHALRenderer) and window handle.
	void			InitInstance(	u32 uFrameBufferWidth, u32 uFrameBufferHeight, CHALRenderer::eBPP BitsPerPixel, u32 uFlags, 
									CFrameWnd* pMainWnd, HINSTANCE hInstance, bool bDesignMode = true );

	void			OnCreate( CWnd* pMainWnd );

	// Renders the current frame.
	void			Render();

	// Resizes editor GUI.
	void			Resize( u32 uFrameBufferWidth, u32 uFrameBufferHeight );

	// Ticks the editor, updating time-based values.
	void			Tick();

	//---------------------------------------------------------------------------------------------

	// Returns engine core.
	inline	CCore*		GetCore();

	// Returns application instance.
	inline	HINSTANCE	GetHInstance();

	// Returns the main editor window.
	inline	CFrameWnd*	GetMainWnd();

private :

	bool			m_bDesignMode;
	HINSTANCE		m_hInstance;
	CCore			m_Core;
	CNavControls	m_NavControls;
	CMesh*			m_pGrid;
	CFrameWnd*		m_pMainWnd;

	CMenu			m_Menu;
	CStatusBar		m_StatusBar;
	CChildView		m_View;

};

inline CCore* CEditor::GetCore()
{
	return &m_Core;
}

inline HINSTANCE CEditor::GetHInstance()
{
	return m_hInstance;
}

inline CFrameWnd* CEditor::GetMainWnd()
{
	return m_pMainWnd;
}

}