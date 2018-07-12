// BaseObject.h

#pragma once

#include "Mesh.h"

namespace Fuse
{

//
// Utility class for mesh primitives.
//

class CPrimitives
{

public :

	static	void CreateGrid(  float fSize, u32 uSubdivisions, CColor PrimaryColor, CColor SecondaryColor, CMesh::CMeshData* pData );

};

}