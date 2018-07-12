// VertexTypes.h

#pragma once

namespace Fuse
{

const u32	MAX_WEIGHTS		= 4;
const u32	MAX_UV_SETS		= 8;

const u32	VERTEX_INTERNALWEIGHT1	= 0x00000002;
const u32	VERTEX_INTERNALWEIGHT2	= 0x00000004;
const u32	VERTEX_INTERNALWEIGHT3	= 0x00000008;
const u32	VERTEX_INTERNALWEIGHT4	= 0x00000010;

const u32	VERTEX_INTERNALUV1	= 0x00000800;	
const u32	VERTEX_INTERNALUV2	= 0x00001000;
const u32	VERTEX_INTERNALUV3	= 0x00002000;
const u32	VERTEX_INTERNALUV4	= 0x00004000;
const u32	VERTEX_INTERNALUV5	= 0x00008000;
const u32	VERTEX_INTERNALUV6	= 0x00010000;
const u32	VERTEX_INTERNALUV7	= 0x00020000;
const u32	VERTEX_INTERNALUV8	= 0x00040000;

const u32	VERTEX_POSITION		= 0x00000001;
const u32	VERTEX_RHW			= 0x00000002;
const u32	VERTEX_WEIGHT1		= VERTEX_INTERNALWEIGHT1;
const u32	VERTEX_WEIGHT2		= VERTEX_INTERNALWEIGHT1 | VERTEX_INTERNALWEIGHT2;
const u32	VERTEX_WEIGHT3		= VERTEX_INTERNALWEIGHT1 | VERTEX_INTERNALWEIGHT2 | VERTEX_INTERNALWEIGHT3;
const u32	VERTEX_WEIGHT4		= VERTEX_INTERNALWEIGHT1 | VERTEX_INTERNALWEIGHT2 | VERTEX_INTERNALWEIGHT3 | VERTEX_INTERNALWEIGHT4;
const u32	VERTEX_BLENDINDEX	= 0x00000040;
const u32	VERTEX_NORMAL		= 0x00000080;
const u32	VERTEX_POINTSIZE	= 0x00000100;
const u32	VERTEX_COLOR		= 0x00000200;
const u32	VERTEX_SPECCOLOR	= 0x00000400;
const u32	VERTEX_UV1			= VERTEX_INTERNALUV1;
const u32	VERTEX_UV2			= VERTEX_INTERNALUV1 | VERTEX_INTERNALUV2;
const u32	VERTEX_UV3			= VERTEX_INTERNALUV1 | VERTEX_INTERNALUV2 | VERTEX_INTERNALUV3;
const u32	VERTEX_UV4			= VERTEX_INTERNALUV1 | VERTEX_INTERNALUV2 | VERTEX_INTERNALUV3 | VERTEX_INTERNALUV4;
const u32	VERTEX_UV5			= VERTEX_INTERNALUV1 | VERTEX_INTERNALUV2 | VERTEX_INTERNALUV3 | VERTEX_INTERNALUV4 | VERTEX_INTERNALUV5;
const u32	VERTEX_UV6			= VERTEX_INTERNALUV1 | VERTEX_INTERNALUV2 | VERTEX_INTERNALUV3 | VERTEX_INTERNALUV4 | VERTEX_INTERNALUV5 | VERTEX_INTERNALUV6;
const u32	VERTEX_UV7			= VERTEX_INTERNALUV1 | VERTEX_INTERNALUV2 | VERTEX_INTERNALUV3 | VERTEX_INTERNALUV4 | VERTEX_INTERNALUV5 | VERTEX_INTERNALUV6 | VERTEX_INTERNALUV7;
const u32	VERTEX_UV8			= VERTEX_INTERNALUV1 | VERTEX_INTERNALUV2 | VERTEX_INTERNALUV3 | VERTEX_INTERNALUV4 | VERTEX_INTERNALUV5 | VERTEX_INTERNALUV6 | VERTEX_INTERNALUV7 | VERTEX_INTERNALUV8;
const u32	VERTEX_TANGENT		= 0x00080000;
const u32	VERTEX_BINORMAL		= 0x00100000;

// Gets the number of floats in a specified vertex type.
u32		GetVertexFloatSize( u32 uComponents );
// Gets the number of UV coordinates in a specified vertex type.
u32		GetUVCountForComponents( u32 uComponents );
// Gets the number of weights in a specified vertesx type.
u32		GetWeightCountForComponents( u32 uComponents );

// Some common vertex structures.

// Lit vertex
const u32	LIT_VERTEX				= VERTEX_POSITION | VERTEX_NORMAL | VERTEX_UV1;
struct CLitVertex
{
	CVector		Pos;
	CVector		Normal;
	CVector2	UV;
	CLitVertex()					{};
};

// Lit and colored untextured vertex
const u32 LIT_UNTEXTURED_VERTEX		= VERTEX_POSITION | VERTEX_NORMAL;
struct CLitUntexturedVertex
{
	CVector	Pos;
	CVector	Normal;
	CLitUntexturedVertex()			{};
};

// Unlit colored vertex 1
const u32 COLORED_VERTEX			= VERTEX_POSITION | VERTEX_COLOR | VERTEX_UV1;
struct CColoredVertex
{
	CVector		Pos;
	CColor		Color;
	CVector2	UV;
	CColoredVertex()				{};
};

// Unlit colored vertex with specular color
const u32 COLORED_SPEC_VERTEX		= VERTEX_POSITION | VERTEX_COLOR | VERTEX_SPECCOLOR | VERTEX_UV1;
struct CColoredSpecVertex
{
	CVector		Pos;
	CColor		Color;
	CColor		Specular;
	CVector2	UV;
	CColoredSpecVertex()			{};
};

// Lit and colored vertex
const u32 LIT_COLORED_VERTEX		= VERTEX_POSITION | VERTEX_NORMAL | VERTEX_COLOR | VERTEX_UV1;
struct CLitColoredVertex
{
	CVector		Pos;
	CVector		Normal;
	CColor		Color;
	CVector2	UV;
	CLitColoredVertex()				{};
};

// Lit and colored vertex with specular
const u32 LIT_COLORED_SPEC_VERTEX	= VERTEX_POSITION | VERTEX_NORMAL | VERTEX_COLOR | VERTEX_SPECCOLOR | VERTEX_UV1;
struct CLitColoredSpecularVertex
{
	CVector		Pos;
	CVector		Normal;
	CColor		Color;
	CColor		Specular;
	CVector2	UV;
	CLitColoredSpecularVertex()		{};
};

// Uncolored vertex (white)
const u32 UNCOLORED_VERTEX			= VERTEX_POSITION | VERTEX_UV1;
struct CUncoloredVertex
{
	CVector		Pos;
	CVector2	UV;
	CUncoloredVertex()				{};
};

const u32 COLORED_UNTEXTURED_VERTEX	= VERTEX_POSITION | VERTEX_COLOR;
struct CColoredUntexturedVertex
{
	CVector	Pos;
	CColor	Color;
	CColoredUntexturedVertex()		{};
};

// Bare vertex, no uv
const u32 BARE_VERTEX				= 0;
struct CBareVertex
{
	CVector	Pos;
	CBareVertex()					{};
};

const u32 SKINNED_VERTEX1			= VERTEX_POSITION | VERTEX_NORMAL | VERTEX_UV1 | VERTEX_WEIGHT1;
struct CSkinnedVertex1
{
	CVector		Pos;
	float		fWeight[1];
	CVector		Normal;
	CVector2	UV;
	CSkinnedVertex1()				{};
};

const u32 SKINNED_VERTEX2			= VERTEX_POSITION | VERTEX_NORMAL | VERTEX_UV1 | VERTEX_WEIGHT2;
struct CSkinnedVertex2
{
	CVector		Pos;
	float		fWeight[2];
	CVector		Normal;
	CVector2	UV;
	CSkinnedVertex2()				{};
};

const u32 SKINNED_VERTEX3			= VERTEX_POSITION | VERTEX_NORMAL | VERTEX_UV1 | VERTEX_WEIGHT3;
struct CSkinnedVertex3
{
	CVector		Pos;
	float		fWeight[3];
	CVector		Normal;
	CVector2	UV;
	CSkinnedVertex3()				{};
};

const u32 SKINNED_VERTEX4			= VERTEX_POSITION | VERTEX_NORMAL | VERTEX_UV1 | VERTEX_WEIGHT4;
struct CSkinnedVertex4
{
	CVector		Pos;
	float		fWeight[4];
	CVector		Normal;
	CVector2	UV;
	CSkinnedVertex4()				{};
};

}