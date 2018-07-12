// VertexDeclaration.h

#pragma once

namespace Fuse
{

//
// Vertex declaration utility class.
//

class CDX9VertexDeclaration
{

public :

	static	D3DVERTEXELEMENT9* CreateVertexDeclaration( u32 uComponents );

private :

	static	u32 CountElements( u32 uComponents );

};

}