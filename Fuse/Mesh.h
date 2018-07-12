// Mesh.h

#pragma once

#include "Renderable.h"

namespace Fuse
{

class CCamera;
class CHALData;
class CNode;

//
// Mesh class.
//

class CMesh : public CRenderable
{

public :

	enum eIndexBufferType
	{
		eTRILIST,
		eLINELIST,
		eMAX_INDEXBUFFERTYPES
	};

	struct CMeshData
	{

		eIndexBufferType	Type;
		u32					uIndexCount;
		u16*				pIndices;
		u32					uVertexComponents;
		u32					uVertexCount;
		float*				pVertices;

		CMeshData();
		virtual ~CMeshData();

	};

	//---------------------------------------------------------------------------------------------
	
	// Default constructor.
	CMesh();
	// Destructor.
	virtual	~CMesh();

	//---------------------------------------------------------------------------------------------

	// Initializes the mesh.
			void	Init( CMesh::CMeshData* pData );
	virtual	void	Init( eIndexBufferType Type, u32 uIndexCount, u16* pIndices, u32 uVertexComponents, u32 uVertexCount, float* pVertices );

	// Renders the mesh.
	virtual	void	Render( CCamera* pCamera, CNode* pNode );

	//---------------------------------------------------------------------------------------------

	// Retrieves the bounding sphere.
	inline CSphere&			GetBoundingSphere();

	// Retrieves platform-specific data.
	inline void*			GetPlatformData();

	// Returns the number of triangles.
	inline u32				GetElemCount() const;

	// Returns the type of mesh.
	inline eIndexBufferType	GetType() const;

	// Returns the number of vertices.
	inline u32				GetVertexCount() const;

	// Returns what component each vertex contains.
	inline u32				GetVertexComponents() const;

	// Returns the vertex size for this mesh.
	inline	u32				GetVertexSize() const;

private :

	CSphere				m_BoundingSphere;
	eIndexBufferType	m_Type;
	u32					m_uVertexComponents;
	u32					m_uElemCount;
	u32					m_uVertexCount;
	u32					m_uVertexSize;

	CHALData*			m_pPlatformData;

	// Computes bounding box of the mesh.
	CBox	ComputeBoundingBox( u32 uComponents, u32 uVertexCount, float* pVertices );

	// Computes bounding sphere of the mesh.
	CSphere	ComputeBoundingSphere( u32 uComponents, u32 uVertexCount, float* pVertices );

};

inline CSphere& CMesh::GetBoundingSphere()
{ 
	return m_BoundingSphere;					
};

inline void* CMesh::GetPlatformData()
{
	return m_pPlatformData;
}

inline u32 CMesh::GetElemCount() const
{
	return m_uElemCount;
}

inline CMesh::eIndexBufferType	CMesh::GetType() const
{
	return m_Type;
}

inline u32 CMesh::GetVertexComponents() const
{
	return m_uVertexComponents;
}

inline u32 CMesh::GetVertexCount() const
{
	return m_uVertexCount;
}

inline u32 CMesh::GetVertexSize() const
{
	return m_uVertexSize;
}

}