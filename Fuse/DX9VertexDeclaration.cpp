// VertexDeclaration.cpp

#include "stdafx.h"
#include "DX9VertexDeclaration.h"
#include "VertexTypes.h"

u32 CDX9VertexDeclaration::CountElements( u32 uComponents )
{
	u32 uElemCount		= 0;	
	if ( uComponents & VERTEX_POSITION )
		uElemCount++;
	if ( uComponents & VERTEX_NORMAL )
		uElemCount++;
	if ( uComponents & VERTEX_POINTSIZE )
		uElemCount++;
	if ( uComponents & VERTEX_COLOR )
		uElemCount++;
	if ( uComponents & VERTEX_SPECCOLOR )
		uElemCount++;
	if ( uComponents & VERTEX_INTERNALUV8 )
		uElemCount += 8;
	else if ( uComponents & VERTEX_INTERNALUV7 )
		uElemCount += 7;
	else if ( uComponents & VERTEX_INTERNALUV6 )
		uElemCount += 6;
	else if ( uComponents & VERTEX_INTERNALUV5 )
		uElemCount += 5;
	else if ( uComponents & VERTEX_INTERNALUV4 )
		uElemCount += 4;
	else if ( uComponents & VERTEX_INTERNALUV3 )
		uElemCount += 3;
	else if ( uComponents & VERTEX_INTERNALUV2 )
		uElemCount += 2;
	else if ( uComponents & VERTEX_INTERNALUV1 )
		uElemCount++;
	if ( uComponents & VERTEX_INTERNALWEIGHT4 )
		uElemCount += 4;
	else if ( uComponents & VERTEX_INTERNALWEIGHT3 )
		uElemCount += 3;
	else if ( uComponents & VERTEX_INTERNALWEIGHT2 )
		uElemCount += 2;
	else if ( uComponents & VERTEX_INTERNALWEIGHT1 )
		uElemCount++;
	if ( uComponents & VERTEX_BLENDINDEX )
		uElemCount++;
	if ( uComponents & VERTEX_TANGENT )
		uElemCount++;
	if ( uComponents & VERTEX_BINORMAL )
		uElemCount++;
	uElemCount++;						// 1 for end element
	return uElemCount;
}

D3DVERTEXELEMENT9* CDX9VertexDeclaration::CreateVertexDeclaration( u32 uComponents )
{
	u32 uElemCount = CountElements( uComponents );
	u32 uUVCount = GetUVCountForComponents( uComponents );
	u32 uWeightCount = GetWeightCountForComponents( uComponents );

	D3DVERTEXELEMENT9* pDeclaration = new D3DVERTEXELEMENT9[uElemCount];
	u32 uIndex = 0;
	u32 uOffset = 0;
	if ( uComponents & VERTEX_POSITION )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_POSITION;
		pDeclaration[uIndex].UsageIndex	= 0;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT3;
		uOffset += 12;
	}
	uIndex++;
	if ( uWeightCount > 0 )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		switch ( uWeightCount )
		{
		case 1 :
			pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT1;
			break;	
		case 2 :
			pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT2;
			break;	
		case 3 :
			pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT3;
			break;	
		case 4 :
			pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT4;
			break;
		default: 
			assert(false);
		}
		uOffset += uWeightCount;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_BLENDWEIGHT;
		pDeclaration[uIndex].UsageIndex	= 0;
		uIndex++;
	}
	if ( uComponents & VERTEX_BLENDINDEX )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT4;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_BLENDINDICES;
		pDeclaration[uIndex].UsageIndex	= 0;
		uOffset += 12;
		uIndex++;
	}
	if ( uComponents & VERTEX_NORMAL )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT3;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_NORMAL;
		pDeclaration[uIndex].UsageIndex	= 0;
		uOffset += 9;
		uIndex++;
	}
	if ( uComponents & VERTEX_POINTSIZE )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT1;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_PSIZE;
		pDeclaration[uIndex].UsageIndex	= 0;
		uOffset += 4;
		uIndex++;
	}
	if ( uComponents & VERTEX_COLOR )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_D3DCOLOR;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_COLOR;
		pDeclaration[uIndex].UsageIndex	= 0;
		uOffset += 4;
		uIndex++;
	}
	if ( uComponents & VERTEX_SPECCOLOR )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_D3DCOLOR;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_COLOR;
		pDeclaration[uIndex].UsageIndex	= 1;
		uOffset += 4;
		uIndex++;
	}
	for ( u32 i = 0; i < uUVCount; i++ )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT2;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_TEXCOORD;
		pDeclaration[uIndex].UsageIndex	= i;
		uOffset += 8;
		uIndex++;
	}
	if ( uComponents & VERTEX_TANGENT )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT3;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_TANGENT;
		pDeclaration[uIndex].UsageIndex	= 0;
		uOffset += 9;
		uIndex++;
	}
	if ( uComponents & VERTEX_BINORMAL )
	{
		pDeclaration[uIndex].Stream		= 0;
		pDeclaration[uIndex].Offset		= uOffset;
		pDeclaration[uIndex].Type		= D3DDECLTYPE_FLOAT3;
		pDeclaration[uIndex].Method		= D3DDECLMETHOD_DEFAULT;
		pDeclaration[uIndex].Usage		= D3DDECLUSAGE_BINORMAL;
		pDeclaration[uIndex].UsageIndex	= 0;
		// ReSharper disable once CppAssignedValueIsNeverUsed
		uOffset += 9;
		uIndex++;
	}
	pDeclaration[uIndex].Stream		= 0xFF;
	pDeclaration[uIndex].Offset		= 0;
	pDeclaration[uIndex].Type		= D3DDECLTYPE_UNUSED;
	pDeclaration[uIndex].Method		= 0;
	pDeclaration[uIndex].Usage		= 0;
	pDeclaration[uIndex].UsageIndex	= 0;
	
	return pDeclaration;
}