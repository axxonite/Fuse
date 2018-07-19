// DXRenderer.h

#pragma once

#include "HALRenderer.h"
#include "HAL.h"

namespace Fuse
{

//
// DirectX 9 renderer.
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
	void		Init(	u32 uFramebufferWidth, u32 uFramebufferHeight, u32 uFlags = 0, HWND hWindowHandle = nullptr ) override;
	// Resets the renderer. Called when rendering devioe focus has been lost.
	void		Reset( u32 uFrameBufferWidth = 0, u32 uFrameBufferHeight = 0 ) override;

	// Creates platform-specific data for a mesh.
	CHALData*	CreateMeshPlatformData( u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices ) override;

	// Starts and ends scene rendering.
	void		BeginScene() override;
	void		EndScene() override;
	// Clear specified buffer (see CHALRenderer). Framebuffer is cleared with specified color.
	void		ClearScreen( u32 uBuffers, CColor Color ) override;
	// Flips the rendered image.
	void		FlipFrame() override;

	// Renders a specified mesh.
	void		Render( CMesh* pMesh ) override;

	// Sets the current transformation matrices.
	void		SetTransform( eTransforms Transform, CMatrix4& Matrix ) override;

	//---------------------------------------------------------------------------------------------

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

	void		ConfigureParams(	u32 uFrameBufferWidth, u32 uFrameBufferHeight, eBPP BitsPerPixel, u32 uFlags, HWND hWnd );

	static u32	ConvertComponentsToFVF( u32 uComponents );

	static eBPP	D3DFormatToBPP( D3DFORMAT Format );

	void		ResetStates() const;

};

}