// CCore.h

#pragma once

#include "Camera.h"
#include "Scene.h"

#include "DXInputSystem.h"
#include "HALRenderer.h"
#include "DX9Renderer.h"
#include "DX12Renderer.h"

#define USE_LEGACY_RENDERER 0

namespace Fuse
{

class CEngineStats;
class CAnimationSystem;
class CGUISystem;

//
// Main engine core.
// 

class CCore : public CBaseObject
{

public:

	//---------------------------------------------------------------------------------------------

	CCore();

	//---------------------------------------------------------------------------------------------

	// Initializes engine core. Specify framebuffer width, height, bits per pixel, initialization flags (see CHALRenderer) and window handle.
	void			Init(	u32 uFrameBufferWidth, u32 uFrameBufferHeight, CHALRenderer::eBPP BitsPerPixel, u32 uFlags, HWND hRenderWnd, HWND hMainWnd );

	// Renders the current frame.
	void			Render();

	// Resizes the frame buffer.
	void			ResizeFrameBuffer( u32 uFrameBufferWidth, u32 uFrameBufferHeight );

	// Ticks the engine, updating time-based values.
	void			Tick();

	// Ticks input devices.
	void			TickInput();

	//---------------------------------------------------------------------------------------------

	// Returns main engine camera,
	inline CCamera*	GetCamera();

	// Returns main engine scene.
	inline CScene*	GetScene();

	// Determines the background color.
	inline	CColor	GetBackgroundColor() const;
	inline	void	SetBackgroundColor( CColor Color );

	inline	bool	IsInitialized() const;

private:

	CScene				m_Scene;
	CCamera				m_Camera;
	CColor				m_BackgroundColor;

	bool				m_bInitialized;
#if USE_LEGACY_RENDERER
	CDX9Renderer		m_Renderer;
#else
	CDX12Renderer		m_Renderer;
#endif
	CDXInputSystem		m_IntputSystem;

};

inline CCamera* CCore::GetCamera()
{
	return &m_Camera;
}

inline CScene* CCore::GetScene()
{
	return &m_Scene;
}

inline CColor CCore::GetBackgroundColor() const
{
	return m_BackgroundColor;
}

inline bool CCore::IsInitialized() const
{
	return m_bInitialized;
}

inline void CCore::SetBackgroundColor( CColor Color )
{
	m_BackgroundColor = Color;
}

}
