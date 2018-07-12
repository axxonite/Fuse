// Mesh.cpp

#include "stdafx.h"
#include "Mesh.h"
#include "Box.h"
#include "HALRenderer.h"
#include "VertexTypes.h"
#include "HAL.h"
#include "Node.h"

CMesh::CMeshData::CMeshData() :
	uIndexCount			( 0		),
	uVertexComponents	( 0		),
	uVertexCount		( 0		),
	pIndices			( NULL	),
	pVertices			( NULL	)
{
}

CMesh::CMeshData::~CMeshData()
{
	if ( pIndices )
		delete[] pIndices;
	if ( pVertices )
		delete[] pVertices;
}

CMesh::CMesh() :
	m_uVertexComponents	( 0			),
	m_uElemCount		( 0			),
	m_uVertexCount		( 0			),
	m_pPlatformData		( NULL		),
	m_Type				( eTRILIST	),
	m_uVertexSize		( 0			)
{
}

CMesh::~CMesh()
{
	if ( m_pPlatformData )
		delete m_pPlatformData;
}

CBox CMesh::ComputeBoundingBox( u32 uComponents, u32 uVertexCount, float* pVertices )
{
	CBox Box;
	Box.m_vMin.fX = Box.m_vMin.fY = Box.m_vMin.fZ = FLT_MAX;
	Box.m_vMax.fX = Box.m_vMax.fY = Box.m_vMax.fZ = -FLT_MAX;

	u32 uStepSize = GetVertexFloatSize( uComponents );
	float* pVertex = pVertices;
	for ( u32 i = 0; i < uVertexCount; i++ )
	{
		if ( Box.m_vMin.fX > pVertex[0] ) Box.m_vMin.fX = pVertex[0];
		if ( Box.m_vMin.fY > pVertex[1] ) Box.m_vMin.fY = pVertex[1];
		if ( Box.m_vMin.fZ > pVertex[2] ) Box.m_vMin.fZ = pVertex[2];
		if ( Box.m_vMax.fX < pVertex[0] ) Box.m_vMax.fX = pVertex[0];
		if ( Box.m_vMax.fY < pVertex[1] ) Box.m_vMax.fY = pVertex[1];
		if ( Box.m_vMax.fZ < pVertex[2] ) Box.m_vMax.fZ = pVertex[2];
		pVertex += uStepSize;
	}
	return Box;
}

CSphere CMesh::ComputeBoundingSphere( u32 uComponents, u32 uVertexCount, float* pVertices )
{
	CBox Box		= ComputeBoundingBox( uComponents, uVertexCount, pVertices );
	CVector vSize	= Box.m_vMax - Box.m_vMin;

	// Sphere center is the axis-aligned box center
	CSphere Sphere;
	Sphere.m_vPosition = 0.5f * ( Box.m_vMin + Box.m_vMax );

    // Compute the radius
    float fRadiusSqr = 0.0f;

	u32 uStepSize = GetVertexFloatSize( uComponents );
	float* pVertex = pVertices;
	for ( u32 i = 0; i < uVertexCount; i++ )
	{
		CVector vDiff;
		vDiff.fX = pVertex[0] - Sphere.m_vPosition.fX;
		vDiff.fY = pVertex[1] - Sphere.m_vPosition.fY;
		vDiff.fZ = pVertex[2] - Sphere.m_vPosition.fZ;
		float fLengthSqr	= vDiff.Length();
		fLengthSqr = fLengthSqr * fLengthSqr;
		if ( fLengthSqr > fRadiusSqr ) 
			fRadiusSqr = fLengthSqr;

		pVertex += uStepSize;
	}
    Sphere.m_fRadius = (float)sqrt( fRadiusSqr );
	return Sphere;
}

void CMesh::Init( CMesh::CMeshData* pData )
{
	Init( pData->Type, pData->uIndexCount, pData->pIndices, pData->uVertexComponents, pData->uVertexCount, pData->pVertices );
}

void CMesh::Init( eIndexBufferType Type, u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices )
{
	if ( m_pPlatformData )
		delete m_pPlatformData;
	m_pPlatformData		= g_pRenderer->CreateMeshPlatformData( uIndexCount, pIndices, uVertexComponents, uVertexCount, pVertices );
	m_uVertexComponents = uVertexComponents;
	switch ( Type )
	{
	case eTRILIST :
		m_uElemCount			= uIndexCount / 3;
		break;
	case eLINELIST :
		m_uElemCount				= uIndexCount / 2;
		break;
	}
	m_uVertexCount		= uVertexCount;
	m_Type				= Type;
	m_uVertexSize		= GetVertexFloatSize( uVertexComponents ) * 4;
	ComputeBoundingSphere( uVertexComponents, uVertexCount, pVertices );

}

void CMesh::Render( CCamera* pCamera, CNode* pNode )
{
	g_pRenderer->Render( this );
}