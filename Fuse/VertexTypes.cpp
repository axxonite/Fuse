// VertexTypes.cpp

#include "stdafx.h"
#include "VertexTypes.h"

u32 Fuse::GetUVCountForComponents( u32 uComponents )
{
	if ( uComponents & VERTEX_INTERNALUV8 )
		return 8;
	if ( uComponents & VERTEX_INTERNALUV7 )
		return 7;
	if ( uComponents & VERTEX_INTERNALUV6 )
		return 6;
	if ( uComponents & VERTEX_INTERNALUV5 )
		return 5;
	if ( uComponents & VERTEX_INTERNALUV4 )
		return 4;
	if ( uComponents & VERTEX_INTERNALUV3 )
		return 3;
	if ( uComponents & VERTEX_INTERNALUV2 )
		return 2;
	if ( uComponents & VERTEX_INTERNALUV1 )
		return 1;
	return 0;
}

u32	Fuse::GetWeightCountForComponents( u32 uComponents )
{
	if ( uComponents & VERTEX_INTERNALWEIGHT4 )
		return 4;
	if ( uComponents & VERTEX_INTERNALWEIGHT3 )
		return 3;
	if ( uComponents & VERTEX_INTERNALWEIGHT2 )
		return 2;
	if ( uComponents & VERTEX_INTERNALWEIGHT1 )
		return 1;
	return 0;
}

u32	Fuse::GetVertexFloatSize( u32 uComponents )
{
	u32 uSize	= 3;		
	u32 uWeightFlag = VERTEX_INTERNALWEIGHT1;
	for ( int i = 0; i < MAX_WEIGHTS; i++ )
	{
		if ( uComponents & uWeightFlag )	
			uSize++;
		else break;
		uWeightFlag <<= 1;
	}
	if ( uComponents & VERTEX_BLENDINDEX )
		uSize++;
	if ( uComponents & VERTEX_NORMAL )
		uSize += 3;
	if ( uComponents & VERTEX_POINTSIZE )
		uSize++;
	if ( uComponents & VERTEX_COLOR )
		uSize++;
	if ( uComponents & VERTEX_SPECCOLOR )
		uSize++;
	u32 uUVFlag = VERTEX_INTERNALUV1;
	for ( int i = 0; i < MAX_UV_SETS; i++ )
	{
		if ( uComponents & uUVFlag )	
			uSize	+= 2;
		else break;
		uUVFlag <<= 1;
	}
	if ( uComponents & VERTEX_TANGENT )	
		uSize	+= 3;
	if ( uComponents & VERTEX_BINORMAL )
		uSize	+= 3;
	return uSize;
}