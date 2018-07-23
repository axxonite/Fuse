#pragma once
// DXRenderer.h

#pragma once

#include "HALRenderer.h"
#include "HAL.h"
#include "FuseDX12.h"

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
		void		Init(u32 uFramebufferWidth, u32 uFramebufferHeight, u32 uFlags = 0, HWND hWindowHandle = 0) override;
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

		// Renders a specified mesh.
		void		Render(CMesh* pMesh) override;

		// Sets the current transformation matrices.
		void		SetTransform(eTransforms Transform, CMatrix4& Matrix) override;

	private:

		static const UINT FrameCount = 2;

		struct Vertex
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		struct SceneConstantBuffer
		{
			XMFLOAT4 offset;
		};

		ComPtr<IDXGISwapChain3> m_swapChain;
		ComPtr<ID3D12Device> m_device;
		ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator> m_commandAllocator;
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		UINT m_rtvDescriptorSize = 0;

		// App resources.
		ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		ComPtr<ID3D12Resource> m_constantBuffer;
		SceneConstantBuffer m_constantBufferData;
		UINT8* m_pCbvDataBegin = nullptr;
		CD3D12_VIEWPORT m_viewport;
		CD3D12_RECT m_scissorRect;

		// Synchronization objects.
		UINT m_frameIndex = 0;
		HANDLE m_fenceEvent = nullptr;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue = 0;
		float m_aspectRatio = 0;

		static void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

		void CreateShaders();
		void CreateRootSignature();
		void CreateCommandList();
		void CreateVertexBuffer();
		void CreateConstantBuffer();
		void CreaterDeviceAndSwapChain(u32 uFrameBufferWidth, u32 uFrameBufferHeight, HWND hWnd);
		void CreateViews();
		void CreateFences();

		void WaitForPreviousFrame();
		void PopulateCommandList() const;
	
	};
}
