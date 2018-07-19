// HALRenderer.h

#pragma once

#include "Singleton.h"

namespace Fuse
{

#define g_pRenderer	CHALRenderer::GetInstance()

	class CMesh;
	class CHALData;

	//
	// Renderer HAL interface.
	//

	class CHALRenderer : public CBaseObject, public CSingleton<CHALRenderer>
	{

	public:

		// Enums
		enum eBPP
		{
			eDEFAULT,
			e16BIT,
			e32BIT,
			eUNKNOWN,
			eMAX_BPP
		};

		enum eTransforms
		{
			eWORLD_TRANSFORM,
			eVIEW_TRANSFORM,
			ePROJ_TRANSFORM,
			eMAX_TRANSFORMS
		};

		// Constants
		static	const u32	INIT_WINDOWED = 0x01;

		static	const u32	CLEAR_TARGET = 0x01;
		static	const u32	CLEAR_ZBUFFER = 0x02;
		static	const u32	CLEAR_STENCIL = 0x04;
		static	const u32	CLEAR_ALL = CLEAR_TARGET | CLEAR_ZBUFFER | CLEAR_STENCIL;

		// Exceptions
		class ECantSupportPalettizedDesktop {};
		class EUnsupportedDisplayMode {};

		CHALRenderer();

		//---------------------------------------------------------------------------------------------

		// Initializes renderer. Specify framebuffer width, height, bits per pixel, initialization flags and window handle.
		virtual	void			Init(u32 uScreenWidth, u32 uScreenHeight, u32 uFlags = INIT_WINDOWED, HWND hWindowHandle = 0) = 0;
		// Resets the renderer. Called when rendering devioe focus has been lost.
		virtual	void			Reset(u32 uFrameBufferWidth, u32 uFrameBufferHeight) = 0;

		// Creates platform-specific data for a mesh.
		virtual	CHALData*		CreateMeshPlatformData(u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices) = 0;

		// Starts and ends scene rendering.
		virtual void			BeginScene() = 0;
		virtual void			EndScene() = 0;
		// Clear specified buffer (see CHALRenderer). Framebuffer is cleared with specified color.
		virtual void			ClearScreen(u32 uBuffers, CColor Color) = 0;
		// Flips the rendered image.
		virtual	void			FlipFrame() = 0;

		// Returns framebuffer dimensions.
		u32						GetFrameBufferWidth() const { return m_frameBufferWidth; };
		u32						GetFrameBufferHeight() const { return m_frameBufferHeight; };

		// Returns true if the renderer has been initialized.
		bool					IsInitialized() const { return m_bInitialized; }

		// Renders a specified mesh.
		virtual	void			Render(CMesh* pMesh) = 0;

		// Sets the current transformation matrices.
		virtual void			SetTransform(eTransforms Transform, CMatrix4& Matrix) = 0;

		// Returns render window handle.
		inline	HWND			GetWindowHandle() const { return m_hWnd; };

	protected:

		HWND m_hWnd;
		bool m_bInitialized = false;
		int m_frameBufferWidth = 0;
		int m_frameBufferHeight = 0;

	};

}