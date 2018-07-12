#include "stdafx.h"
#include "MeshConverter.h"
#include "VertexTypes.h"
#include "Mesh.h"

CMesh* CMeshConverter::ConvertMesh( MDagPath DagPath )
{
	MStatus Status;
	MFnMesh Mesh( DagPath, &Status );
	CHECK_ERROR( Status );
	if ( Mesh.numPolygons() == 0 )
		return NULL;

	CHECK_ERROR( Mesh.getPoints(			m_VertexArray,		MSpace::kObject ) );
	CHECK_ERROR( Mesh.getNormals(			m_NormalArray,		MSpace::kObject ) );
	CHECK_ERROR( Mesh.getFaceVertexColors(	m_ColorArray ) );
	CHECK_ERROR( Mesh.getCurrentUVSetName(	m_szCurrentUVSetName ) );
	
	MObjectArray		PolygonSets;
	MObjectArray		PolygonComponents;
	DagPath.extendToShape();
	int InstanceNum = 0;
	if ( DagPath.isInstanced() )
		InstanceNum = DagPath.instanceNumber();
	Mesh.getConnectedSetsAndMembers( InstanceNum, PolygonSets, PolygonComponents, true );

	// If there is more than one set, the last set simply consists of all polygons, so we won't include it
	u32 uSetCount = PolygonSets.length();
	if ( uSetCount > 1)
		uSetCount--;

	MIntArray Faces;
	for ( u32 i = 0; i < uSetCount; i++ ) 
	{
		MObject Set		= PolygonSets[i];
		MObject Comp	= PolygonComponents[i];
		MFnSet SetFn( Set, &Status );
		CHECK_ERROR( Status );

		MItMeshPolygon itMeshPolygon( DagPath, Comp, &Status );
		CHECK_ERROR( Status );
		Faces.setLength( itMeshPolygon.count() );

		u32 j = 0;
		for ( itMeshPolygon.reset(); !itMeshPolygon.isDone(); itMeshPolygon.next() ) 
			Faces[j++] = itMeshPolygon.index();

		ConvertSubMesh( Mesh, Faces );
	}
	m_VertexArray.clear();
	m_NormalArray.clear();
	m_ColorArray.clear();
	m_szCurrentUVSetName = "";
	return NULL;
}

struct CVertexRecord
{
	u16	uIndex;
	u16	uPos;
	u16	uNormal;
	u16	uColor;
	u16	uUV[8];
	CVertexRecord* pNext;
};

CVertexRecord ms_VertexRecords[0xFFFF];
CVertexRecord* ms_pAllRecords[0xFFFF];

CMesh* CMeshConverter::ConvertSubMesh( MFnMesh& Mesh, const MIntArray& Faces )
{
	MStatus Status;
	u32 uFaceCount = Faces.length();
	if ( uFaceCount == 0 )
		return NULL;

	FillMemory( ms_VertexRecords, sizeof( ms_VertexRecords ), 0xFF );

	vector<u16> Indices;
	u32 uVertexCount = 0;
	
	vector<CVertexRecord*> AllocatedVertexRecords;
	for ( u32 i = 0; i < uFaceCount; i++) 
	{
		vector<u16> PolyIndices;
		PolyIndices.clear();
		u32 uIndexCount = Mesh.polygonVertexCount( Faces[i], &Status );
		CHECK_ERROR( Status );

		MIntArray VertexIndices;
		CHECK_ERROR( Mesh.getPolygonVertices ( Faces[i], VertexIndices) );
		MIntArray NormalIndices;
		CHECK_ERROR( Mesh.getFaceNormalIds ( Faces[i], NormalIndices) );

		for ( u32 j = 0; j < uIndexCount; j++ ) 
		{
			u32 uColorIndex;
			CHECK_ERROR( Mesh.getFaceVertexColorIndex( Faces[i], j, (int&)uColorIndex ) );

			CVertexRecord* pRecord = &ms_VertexRecords[VertexIndices[j]];
			while ( pRecord && pRecord->uIndex != 0xFF )
			{
				if ( pRecord->uNormal == NormalIndices[j] && pRecord->uColor == uColorIndex )
					break;
				pRecord = pRecord->pNext;
			}
			if ( !pRecord || pRecord->uIndex == 0xFF )
			{
				// New record
				if ( !pRecord )
				{
					pRecord = new CVertexRecord();
					pRecord->pNext = ms_VertexRecords[VertexIndices[j]].pNext;
					ms_VertexRecords[VertexIndices[j]].pNext = pRecord;
					AllocatedVertexRecords.push_back( pRecord );
				}
				pRecord->uPos		= VertexIndices[j];
				pRecord->uNormal	= NormalIndices[j];
				pRecord->uColor		= uColorIndex;
				pRecord->uIndex		= uVertexCount;
				ms_pAllRecords[uVertexCount++] = pRecord;
			}
			PolyIndices.push_back( pRecord->uIndex );

		}
		for ( u32 j = 0; j < uIndexCount - 2; j++ )
		{
			Indices.push_back( PolyIndices[0] );
			Indices.push_back( PolyIndices[j + 1] );
			Indices.push_back( PolyIndices[j + 2] );
		}
	}

	u16* pIndices = new u16[Indices.size()];
	for ( u32 i = 0; i < Indices.size(); i++ )
		pIndices[i] = Indices[i];

	CLitUntexturedVertex* pVertices = new CLitUntexturedVertex[uVertexCount];
	for ( u32 i = 0; i < uVertexCount; i++ )
	{
		CVertexRecord* pRecord = ms_pAllRecords[i];
		MPoint Point = m_VertexArray[pRecord->uPos];
		pVertices->Pos.Set( (float)Point.x, (float)Point.y, (float)Point.z );
		Point = m_NormalArray[pRecord->uNormal];
		pVertices->Normal.Set( (float)Point.x, (float)Point.y, (float)Point.z );
	}

	for ( u32 i = 0; i < AllocatedVertexRecords.size(); i++ )
		delete AllocatedVertexRecords[i];

	CMesh* pMesh = new CMesh();
	pMesh->Init( CMesh::eTRILIST, Indices.size(), pIndices, LIT_UNTEXTURED_VERTEX, uVertexCount, (float*)pVertices );

	Indices.clear();
	delete[] pIndices;
	delete[] pVertices;
	return pMesh;
}