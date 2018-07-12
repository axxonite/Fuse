// BaseObject.h

#pragma once

namespace Fuse
{
	class CMesh;
};

namespace FuseExporter
{

//
// Base class for all engine objects.
//

class CMeshConverter
{

public :

	// Default constructor.
	CMeshConverter();
	// Destructor.
	virtual ~CMeshConverter();

	CMesh*	ConvertMesh( MDagPath DagPath );

private :

	MPointArray			m_VertexArray;
	MColorArray			m_ColorArray;
	MFloatVectorArray	m_NormalArray;
	MString				m_szCurrentUVSetName;

	CMesh*	ConvertSubMesh( MFnMesh& Mesh, const MIntArray& Faces );

};

}