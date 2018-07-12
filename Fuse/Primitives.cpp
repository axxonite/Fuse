// Primitives.cpp

#include "stdafx.h"
#include "Primitives.h"
#include "VertexTypes.h"
#include "Mesh.h"

void CPrimitives::CreateGrid(  float fSize, u32 uSubdivisions, CColor PrimaryColor, CColor SecondaryColor, CMesh::CMeshData* pData )
{
	u32 uLineCount = ( uSubdivisions + 1 ) * 2;

	pData->Type							= CMesh::eLINELIST;
	pData->uIndexCount					= uLineCount * 2;
	pData->uVertexCount					= ( uSubdivisions + 1 ) * 4;
	pData->uVertexComponents			= COLORED_UNTEXTURED_VERTEX;
	pData->pIndices						= new u16[pData->uIndexCount];
	CColoredUntexturedVertex* pVertex	= new CColoredUntexturedVertex[pData->uVertexCount];
	pData->pVertices					= (float*)pVertex;

	float fX = -fSize / 2.0f;
	for ( u32 i = 0; i <= uSubdivisions; i++, fX += ( fSize / (float)uSubdivisions ) )
	{
		pVertex->Pos.Set( fX, 0, -fSize / 2.0f );
		pVertex->Color = IS_FLOAT_EQUAL( pVertex->Pos.fX, 0 ) ? PrimaryColor : SecondaryColor;
		pVertex++;
		pVertex->Pos.Set( fX, 0, fSize / 2.0f );
		pVertex->Color = IS_FLOAT_EQUAL( pVertex->Pos.fX, 0 ) ? PrimaryColor : SecondaryColor;
		pVertex++;
	}

	float fZ = -fSize / 2.0f;
	for ( u32 j = 0; j <= uSubdivisions; j++, fZ += ( fSize / (float)uSubdivisions ) )
	{
		pVertex->Pos.Set( -fSize / 2.0f, 0, fZ );
		pVertex->Color = IS_FLOAT_EQUAL( pVertex->Pos.fZ, 0 ) ? PrimaryColor : SecondaryColor;
		pVertex++;
		pVertex->Pos.Set( fSize / 2.0f, 0, fZ );
		pVertex->Color = IS_FLOAT_EQUAL( pVertex->Pos.fZ, 0 ) ? PrimaryColor : SecondaryColor;
		pVertex++;
	}

	u16* pTri = pData->pIndices	;
	for ( u32 i = 0; i < uLineCount * 2; i++ )
		*pTri++ = i;
}