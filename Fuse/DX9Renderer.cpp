// DXRenderer.cpp

#include "stdafx.h"
#include "DX9Renderer.h"
#include "Mesh.h"
#include "DX9VertexDeclaration.h"
#include "VertexTypes.h"

const u32 DEFAULT_ADAPTER = 0;

const u32 BPPEnumToNumber[CDX9Renderer::eMAX_BPP] =
{
	0,		// eDEFAULT
	16,		// e16BIT
	32,		// e32BIT
	0		// eUNKNOWN
};

const D3DPRIMITIVETYPE MeshTypeToD3DType[CMesh::eMAX_INDEXBUFFERTYPES] =
{
		D3DPT_TRIANGLELIST,				// eTRILIST
		D3DPT_LINELIST					// eLINELIST
};

CDX9Renderer::CDX9MeshPlatformData::CDX9MeshPlatformData() :
	pIndexBuffer			(nullptr ),
	pVertexBuffer			(nullptr ),
	pVertexDeclaration		(nullptr )
{
}

CDX9Renderer::CDX9MeshPlatformData::~CDX9MeshPlatformData()
{
	if ( pIndexBuffer )
		pIndexBuffer->Release();
	if ( pVertexBuffer )
		pVertexBuffer->Release();
}

CDX9Renderer::CDX9Renderer() :
	m_pD3D					( nullptr ),
	m_pD3DDevice			( nullptr ),	
	m_pCurIndexBuffer		( nullptr ),
	m_pCurVertexBuffer		( nullptr ),
	m_pCurVertexDeclaration	( nullptr )
{
	ZeroMemory( &m_D3DParams, sizeof( m_D3DParams ) );
}

CDX9Renderer::~CDX9Renderer()
{
	for ( map<u32, IDirect3DVertexDeclaration9*>::iterator it = m_VertexDeclarations.begin(); it != m_VertexDeclarations.end(); ++it )
		it->second->Release();
	if ( m_pD3DDevice )
		m_pD3DDevice->Release();
	if ( m_pD3D )
		m_pD3D->Release();
}

void CDX9Renderer::BeginScene()
{
	CHECK_DIRECTX( m_pD3DDevice->BeginScene() );
}

void CDX9Renderer::ConfigureParams( u32 uFrameBufferWidth, u32 uFrameBufferHeight, eBPP BitsPerPixel, u32 uFlags, HWND hWnd  )
{
	ZeroMemory( &m_D3DParams, sizeof( m_D3DParams ) );

	if ( uFrameBufferWidth == 0 || uFrameBufferHeight == 0 )
	{
		RECT Rect;
		GetClientRect( hWnd, &Rect );
		uFrameBufferWidth = Rect.right;
		uFrameBufferHeight = Rect.bottom;
	}
	m_D3DParams.BackBufferWidth			= uFrameBufferWidth;
	m_D3DParams.BackBufferHeight			= uFrameBufferHeight;
	m_D3DParams.SwapEffect				= ( uFlags & INIT_WINDOWED ) ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_FLIP;
	m_D3DParams.EnableAutoDepthStencil	= TRUE;
	m_D3DParams.AutoDepthStencilFormat	= D3DFMT_D24S8;

	if ( uFlags & INIT_WINDOWED )
	{
		// Force bits-per-pixel to current desktop bits-per-pixel
		D3DDISPLAYMODE DisplayMode;
		CHECK_DIRECTX( m_pD3D->GetAdapterDisplayMode( DEFAULT_ADAPTER, &DisplayMode ) );
		if ( DisplayMode.Format == D3DFMT_P8 )
			throw ECantSupportPalettizedDesktop();
		m_D3DParams.Windowed = TRUE;
		m_D3DParams.BackBufferFormat			= DisplayMode.Format;
		m_D3DParams.BackBufferCount			= 1;
		BitsPerPixel = D3DFormatToBPP( DisplayMode.Format );
	}
	else
	{
		m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if ( BitsPerPixel == eDEFAULT ) 
			BitsPerPixel = e32BIT;

		switch ( BitsPerPixel )
		{
			case e16BIT :
			{
				if ( m_pD3D->CheckDeviceType(	DEFAULT_ADAPTER, D3DDEVTYPE_HAL, D3DFMT_R5G6B5, 
												D3DFMT_R5G6B5, FALSE ) != D3D_OK )
				{
					if ( m_pD3D->CheckDeviceType(	DEFAULT_ADAPTER, D3DDEVTYPE_HAL, D3DFMT_X1R5G5B5, 
													D3DFMT_X1R5G5B5, FALSE ) != D3D_OK )
						throw EUnsupportedDisplayMode();
					else m_D3DParams.BackBufferFormat = D3DFMT_X1R5G5B5;
				}
				else m_D3DParams.BackBufferFormat = D3DFMT_R5G6B5;
			}
			break;
			case e32BIT :
			{
				if ( m_pD3D->CheckDeviceType(	DEFAULT_ADAPTER, D3DDEVTYPE_HAL, D3DFMT_R8G8B8, 
												D3DFMT_R8G8B8, FALSE ) != D3D_OK )
				{
					if ( m_pD3D->CheckDeviceType(	DEFAULT_ADAPTER, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 
													D3DFMT_A8R8G8B8, FALSE ) != D3D_OK )
						throw EUnsupportedDisplayMode();
					else m_D3DParams.BackBufferFormat = D3DFMT_A8R8G8B8;
				}
				else m_D3DParams.BackBufferFormat = D3DFMT_R8G8B8;
			}
			break;
			default :
			{
				assert( false );
			}
		}

		m_D3DParams.BackBufferCount = 1;
	}

	Trace( "Setting display mode to %dx%dx%d.\n", uFrameBufferWidth, uFrameBufferHeight, BPPEnumToNumber[BitsPerPixel] );
	Trace( "Windowed mode: %s\n", m_D3DParams.Windowed ? "TRUE" : "FALSE" );
}

void CDX9Renderer::ClearScreen( u32 uBuffers, CColor Color )
{
	u32 uD3DFlags = 0;
	if ( uBuffers & CLEAR_TARGET )	
		uD3DFlags |= D3DCLEAR_TARGET;
	if ( uBuffers & CLEAR_ZBUFFER )		
		uD3DFlags |= D3DCLEAR_ZBUFFER;
	if ( uBuffers & CLEAR_STENCIL )
		uD3DFlags |= D3DCLEAR_STENCIL;

	CHECK_DIRECTX( m_pD3DDevice->Clear( 0, NULL, uD3DFlags, Color.uColor, 1.0f, 0 ) );
}

u32	CDX9Renderer::ConvertComponentsToFVF( u32 uComponents )
{
	u32 uFVFCode = 0;
	if ( uComponents & VERTEX_POSITION )
		uFVFCode |= D3DFVF_XYZ;
	if ( uComponents & VERTEX_NORMAL )				
		uFVFCode |= D3DFVF_NORMAL;
	if ( uComponents & VERTEX_POINTSIZE )
		uFVFCode |= D3DFVF_PSIZE;
	if ( uComponents & VERTEX_COLOR )				
		uFVFCode |= D3DFVF_DIFFUSE;
	if ( uComponents & VERTEX_SPECCOLOR )			
		uFVFCode |= D3DFVF_SPECULAR;
	if ( uComponents & VERTEX_INTERNALUV8 )			
		uFVFCode |= D3DFVF_TEX8;
	else if ( uComponents & VERTEX_INTERNALUV7 )	
		uFVFCode |= D3DFVF_TEX7;
	else if ( uComponents & VERTEX_INTERNALUV6 )	
		uFVFCode |= D3DFVF_TEX6;
	else if ( uComponents & VERTEX_INTERNALUV5 )	
		uFVFCode |= D3DFVF_TEX5;
	else if ( uComponents & VERTEX_INTERNALUV4 )	
		uFVFCode |= D3DFVF_TEX4;
	else if ( uComponents & VERTEX_INTERNALUV3 )	
		uFVFCode |= D3DFVF_TEX3;
	else if ( uComponents & VERTEX_INTERNALUV2 )	
		uFVFCode |= D3DFVF_TEX2;
	else if ( uComponents & VERTEX_INTERNALUV1 )	
		uFVFCode |= D3DFVF_TEX1;
	if ( uComponents & VERTEX_INTERNALWEIGHT4 )
		uFVFCode |= D3DFVF_XYZB4;
	else if ( uComponents & VERTEX_INTERNALWEIGHT3 )	
		uFVFCode |= D3DFVF_XYZB3;
	else if ( uComponents & VERTEX_INTERNALWEIGHT2 )	
		uFVFCode |= D3DFVF_XYZB2;
	else if ( uComponents & VERTEX_INTERNALWEIGHT1 )	
		uFVFCode |= D3DFVF_XYZB1;
	if ( uComponents & VERTEX_BLENDINDEX )
		uFVFCode |= D3DFVF_LASTBETA_UBYTE4;
	return uFVFCode;
}

CHALData* CDX9Renderer::CreateMeshPlatformData( u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices )
{
	CDX9MeshPlatformData* pPlatformData = new CDX9MeshPlatformData();

	u16* pIndexBufferData;
	CHECK_DIRECTX( m_pD3DDevice->CreateIndexBuffer( uIndexCount * 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pPlatformData->pIndexBuffer, NULL ) );
	CHECK_DIRECTX( pPlatformData->pIndexBuffer->Lock( 0, uIndexCount * 2, reinterpret_cast<void**>(&pIndexBufferData), 0 ) );
	memcpy( pIndexBufferData, pIndices, uIndexCount * 2 );
	CHECK_DIRECTX( pPlatformData->pIndexBuffer->Unlock() );

	float* pVertexBufferData;
	u32 uSize = GetVertexFloatSize( uVertexComponents ) * 4;	
	CHECK_DIRECTX( m_pD3DDevice->CreateVertexBuffer(	uSize * uVertexCount, D3DUSAGE_WRITEONLY, ConvertComponentsToFVF( uVertexComponents ), 
														D3DPOOL_MANAGED, &pPlatformData->pVertexBuffer, NULL ) );
	CHECK_DIRECTX( pPlatformData->pVertexBuffer->Lock( 0, uSize * uVertexCount, reinterpret_cast<void**>(&pVertexBufferData), 0 ) );
	memcpy( pVertexBufferData, pVertices, uSize * uVertexCount );
	CHECK_DIRECTX( pPlatformData->pVertexBuffer->Unlock() );
	
	map<u32, IDirect3DVertexDeclaration9*>::iterator it = m_VertexDeclarations.find( uVertexComponents );
	if ( it != m_VertexDeclarations.end() )
		pPlatformData->pVertexDeclaration = it->second;
	else
	{	
		D3DVERTEXELEMENT9* pDeclaration = CDX9VertexDeclaration::CreateVertexDeclaration( uVertexComponents );
		CHECK_DIRECTX( m_pD3DDevice->CreateVertexDeclaration( pDeclaration, &pPlatformData->pVertexDeclaration ) );
		delete[] pDeclaration;
		m_VertexDeclarations[uVertexComponents] = pPlatformData->pVertexDeclaration;
	}

	return pPlatformData;
}

CDX9Renderer::eBPP CDX9Renderer::D3DFormatToBPP( D3DFORMAT Format )
{
	switch ( Format )
	{
	case D3DFMT_X4R4G4B4 :
	case D3DFMT_R5G6B5 :
	case D3DFMT_X1R5G5B5 :
	case D3DFMT_A1R5G5B5 :
	case D3DFMT_A4R4G4B4 :
		return e16BIT;

	case D3DFMT_R8G8B8 :
	case D3DFMT_X8R8G8B8 :
	case D3DFMT_A8R8G8B8 :
		return e32BIT;

	default :
		assert( false );
	}
	return eUNKNOWN;
}

void CDX9Renderer::EndScene()
{
	CHECK_DIRECTX( m_pD3DDevice->EndScene() );
}

void CDX9Renderer::FlipFrame()
{
	HRESULT hResult = m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr );
	if( hResult == D3DERR_DEVICELOST )
	{
		if( m_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
			Reset();
	}
	else
	{
		// Test the Present() result
		CHECK_DIRECTX( hResult );
	}
}

void CDX9Renderer::Init(u32 uFrameBufferWidth, u32 uFrameBufferHeight, u32 uFlags, HWND hWnd )
{
	Trace( "Initializing renderer...\n" );

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if ( !m_pD3D ) 
		throw ECannotCreateDirect3D9();

	D3DADAPTER_IDENTIFIER9 DeviceID;
	CHECK_DIRECTX( m_pD3D->GetAdapterIdentifier( DEFAULT_ADAPTER, 0, &DeviceID ) );
	Trace( "Driver : %s.\n", DeviceID.Driver );
	Trace( "Videocard : %s.\n",	DeviceID.Description );

	m_hWnd = hWnd;
	m_frameBufferWidth = uFrameBufferWidth;
	m_frameBufferHeight = uFrameBufferHeight;
	ConfigureParams( uFrameBufferWidth, uFrameBufferHeight, e32BIT, uFlags, hWnd );

	CHECK_DIRECTX( m_pD3D->CreateDevice(DEFAULT_ADAPTER, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,  &m_D3DParams, &m_pD3DDevice ) );
	m_bInitialized = true;
	Trace( "Direct3D device created successfully.\n" );
	Trace( "Renderer initialized.\n" );

	ResetStates();
}

void CDX9Renderer::Render( CMesh* pMesh )
{
	CDX9MeshPlatformData* pPlatformData = static_cast<CDX9MeshPlatformData*>(pMesh->GetPlatformData());
	if ( m_pCurVertexDeclaration != pPlatformData->pVertexDeclaration  )
	{
		CHECK_DIRECTX( m_pD3DDevice->SetVertexDeclaration( pPlatformData->pVertexDeclaration ) );
		m_pCurVertexDeclaration	= pPlatformData->pVertexDeclaration;
	}

	if ( m_pCurIndexBuffer != pPlatformData->pIndexBuffer )
	{
		CHECK_DIRECTX(	m_pD3DDevice->SetIndices( pPlatformData->pIndexBuffer ) );
		m_pCurIndexBuffer = pPlatformData->pIndexBuffer;
	}
	
	if ( m_pCurVertexBuffer != pPlatformData->pVertexBuffer )
	{
		CHECK_DIRECTX(	m_pD3DDevice->SetStreamSource(	0, pPlatformData->pVertexBuffer, 0, pMesh->GetVertexSize() ) );
		m_pCurVertexBuffer = pPlatformData->pVertexBuffer;
	}

	CHECK_DIRECTX(	m_pD3DDevice->DrawIndexedPrimitive( MeshTypeToD3DType[pMesh->GetType()], 0, 0, pMesh->GetVertexCount(), 0, 
					pMesh->GetElemCount() ) );
}

void CDX9Renderer::Reset( u32 uFrameBufferWidth, u32 uFrameBufferHeight )
{
	if ( uFrameBufferWidth != 0 && uFrameBufferHeight != 0 )
	{
		if (	m_D3DParams.BackBufferWidth == uFrameBufferWidth &&
				m_D3DParams.BackBufferHeight == uFrameBufferHeight )
				return;
		m_D3DParams.BackBufferWidth = uFrameBufferWidth;
		m_D3DParams.BackBufferHeight = uFrameBufferHeight;
	}
	CHECK_DIRECTX( m_pD3DDevice->Reset( &m_D3DParams ) );
	m_pCurVertexDeclaration = nullptr;
	m_pCurIndexBuffer = nullptr;
	m_pCurVertexBuffer = nullptr;
	ResetStates();
}

void CDX9Renderer::ResetStates() const
{
	CHECK_DIRECTX( m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) );
}

void CDX9Renderer::SetTransform( eTransforms transform, CMatrix4& mTransform )
{
	switch (transform)
	{
	case eWORLD_TRANSFORM :
		m_mWorldTransform		= mTransform;
		CHECK_DIRECTX( m_pD3DDevice->SetTransform( D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&m_mWorldTransform) ) ); 
		break;
	case eVIEW_TRANSFORM :
		m_mViewTransform		= mTransform;
		CHECK_DIRECTX( m_pD3DDevice->SetTransform( D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&m_mViewTransform) ) );
		break;	
	case ePROJ_TRANSFORM :
		m_mProjTransform		= mTransform;
		CHECK_DIRECTX( m_pD3DDevice->SetTransform( D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&m_mProjTransform) ) );
		break;
	default: 
		assert(false);
	}
}