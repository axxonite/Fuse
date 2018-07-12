#pragma once
// DXRenderer.h

#pragma once

#include "HALRenderer.h"
#include "HAL.h"

namespace Fuse
{
	//
	// DirectX 12 renderer.
	//

	class CDX12Renderer : public CHALRenderer
	{

	public:

		// Exceptions
		class ECannotCreateDirect3D12 {};

		//---------------------------------------------------------------------------------------------

		CDX12Renderer();
		virtual ~CDX12Renderer();

		//---------------------------------------------------------------------------------------------

		// Initializes renderer. Specify framebuffer width, height, bits per pixel, initialization flags (see CHALRenderer) and window handle.
		void		Init(u32 uFramebufferWidth, u32 uFramebufferHeight, CHALRenderer::eBPP BitsPerPixel, u32 uFlags = 0, HWND hWindowHandle = 0) override;
		// Resets the renderer. Called when rendering devioe focus has been lost.
		void		Reset(u32 uFrameBufferWidth = 0, u32 uFrameBufferHeight = 0) override;

		// Creates platform-specific data for a mesh.
		CHALData*	CreateMeshPlatformData(u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices) override;

		// Starts and ends scene rendering.
		void		BeginScene() override;
		void		EndScene() override;
		// Clear specified buffer (see CHALRenderer). Framebuffer is cleared with specified color.
		void		ClearScreen(u32 uBuffers, CColor Color) override;
		// Flips the rendered image.
		void		FlipFrame() override;

		// Returns true if the renderer has been initialized.
		bool		IsInitialized() override;

		// Renders a specified mesh.
		void		Render(CMesh* pMesh) override;

		// Sets the current transformation matrices.
		void		SetTransform(eTransforms Transform, CMatrix4& Matrix) override;

		//---------------------------------------------------------------------------------------------

		// Returns framebuffer dimensions.
		u32			GetFrameBufferWidth() const override;
		u32			GetFrameBufferHeight() const override;

	private:

		bool m_bInitialized;
	};
}