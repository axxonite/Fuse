// DXRenderer.cpp

#include "stdafx.h"
#include "Mesh.h"
#include "DX12Renderer.h"

CDX12Renderer::CDX12Renderer() :
	m_bInitialized(false)
{
}

CDX12Renderer::~CDX12Renderer()
{
}

void CDX12Renderer::BeginScene()
{
}

void CDX12Renderer::ClearScreen(u32 uBuffers, CColor Color)
{
}

CHALData* CDX12Renderer::CreateMeshPlatformData(u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices)
{
	return nullptr;
}

void CDX12Renderer::EndScene()
{
}

void CDX12Renderer::FlipFrame()
{
}

u32 CDX12Renderer::GetFrameBufferWidth() const
{
	return 0;
}

u32 CDX12Renderer::GetFrameBufferHeight() const
{
	return 0;
}

void CDX12Renderer::Init(u32 uFrameBufferWidth, u32 uFrameBufferHeight, CDX12Renderer::eBPP BitsPerPixel, u32 uFlags, HWND hWnd)
{
	Trace("Initializing renderer...\n");
	//Trace("Driver : %s.\n", DeviceID.Driver);
	//Trace("Videocard : %s.\n", DeviceID.Description);
	Trace("Direct3D device created successfully.\n");
	Trace("Renderer initialized.\n");
}

bool CDX12Renderer::IsInitialized()
{
	return m_bInitialized;
}

void CDX12Renderer::Render(CMesh* pMesh)
{
}

void CDX12Renderer::Reset(u32 uFrameBufferWidth, u32 uFrameBufferHeight)
{
}

void CDX12Renderer::SetTransform(eTransforms Transform, CMatrix4& mTransform)
{
}