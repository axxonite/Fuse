// BaseObject.h

#pragma once

#include "Resource.h"

namespace Fuse
{

class CCamera;
class CNode;

//
// Base class for all renderable objects.
//

class CRenderable : public CResource
{

public :

	//---------------------------------------------------------------------------------------------

	// Dumps the scene graph information to the debugging output
	virtual	void	DebugDumpSceneGraph( u32 uFlags = 0, int uLevel = 0 );

	// Called just before rendering the object
	virtual	void	PreRender( CCamera* pCamera, CNode* pNode );
	// Renders the renderable through the specified camera.
	virtual	void	Render( CCamera* pCamera, CNode* pNode ) = 0;
	// Called just after rendering the object.
	virtual	void	PostRender( CCamera* pCamera, CNode* pNode );

	//---------------------------------------------------------------------------------------------

	virtual	const CSphere&	GetBoundingSphere() = 0;

};

}