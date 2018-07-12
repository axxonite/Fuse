// DXRenderer.h

#pragma once

#include "HALRenderer.h"
#include "HAL.h"

namespace Fuse
{

#define g_pDXRenderer	((CDX9Renderer*)CHALRenderer::GetInstance())

	//
// DirectX renderer.
//

class CDX9Renderer : public CHALRenderer
{

public :

	// Exceptions
	class ECannotCreateDirect3D9			{};

	//---------------------------------------------------------------------------------------------

	CDX9Renderer();
	virtual ~CDX9Renderer();

	//---------------------------------------------------------------------------------------------

	// Initializes renderer. Specify framebuffer width, height, bits per pixel, initialization flags (see CHALRenderer) and window handle.
	virtual	void			Init(	u32 uFramebufferWidth, u32 uFramebufferHeight, CHALRenderer::eBPP BitsPerPixel, u32 uFlags = 0, 
									HWND hWindowHandle = 0 );
	// Resets the renderer. Called when rendering devioe focus has been lost.
	virtual	void			Reset( u32 uFrameBufferWidth = 0, u32 uFrameBufferHeight = 0 );

	// Creates platform-specific data for a mesh.
	virtual	CHALData*		CreateMeshPlatformData( u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices );

	// Starts and ends scene rendering.
	virtual void			BeginScene();
	virtual void			EndScene();
	// Clear specified buffer (see CHALRenderer). Framebuffer is cleared with specified color.
	virtual void			ClearScreen( u32 uBuffers, CColor Color );
	// Flips the rendered image.
	virtual	void			FlipFrame();

	// Returns true if the renderer has been initialized.
	virtual	bool			IsInitialized();

	// Renders a specified mesh.
	virtual	void			Render( CMesh* pMesh );

	// Sets the current transformation matrices.
	virtual void			SetTransform( eTransforms Transform, CMatrix4& Matrix );

	//---------------------------------------------------------------------------------------------

	// Returns framebuffer dimensions.
	virtual	u32				GetFrameBufferWidth() const;
	virtual	u32				GetFrameBufferHeight() const;

	// Returns render window handle.
	inline	HWND			GetWindowHandle() const;

private :

	class CDX9MeshPlatformData : public CHALData
	{

	public :

		IDirect3DIndexBuffer9*			pIndexBuffer;
		IDirect3DVertexBuffer9*			pVertexBuffer;
		IDirect3DVertexDeclaration9*	pVertexDeclaration;

		CDX9MeshPlatformData();
		virtual ~CDX9MeshPlatformData();

	};

	IDirect3D9*				m_pD3D;
	IDirect3DDevice9*		m_pD3DDevice;
	D3DPRESENT_PARAMETERS	m_D3DParams;

	CMatrix4				m_mWorldTransform;
	CMatrix4				m_mViewTransform;
	CMatrix4				m_mProjTransform;

	IDirect3DIndexBuffer9*			m_pCurIndexBuffer;
	IDirect3DVertexBuffer9*			m_pCurVertexBuffer;
	IDirect3DVertexDeclaration9*	m_pCurVertexDeclaration;

	map<u32, IDirect3DVertexDeclaration9*>	m_VertexDeclarations;

	HWND					m_hWnd;
	bool					m_bInitialized;

	void					ConfigureParams(	u32 uFrameBufferWidth, u32 uFrameBufferHeight, eBPP BitsPerPixel, 
												u32 uFlags, HWND hWnd );

	u32						ConvertComponentsToFVF( u32 uComponents );

	eBPP					D3DFormatToBPP( D3DFORMAT Format );

	void					ResetStates();

	IDirect3DVertexDeclaration9*	GetVertexDeclaration( u32 uComponents );

};

inline HWND CDX9Renderer::GetWindowHandle() const
{
	return m_hWnd;
}

}