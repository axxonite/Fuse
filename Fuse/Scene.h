// Scene.h

#pragma once

#include "Node.h"

namespace Fuse
{

class CCamera;

//
// Scene.
//

class CScene
{

public :

	//---------------------------------------------------------------------------------------------

	CScene();
	virtual ~CScene();

	//---------------------------------------------------------------------------------------------

	// Renders the scene through the specified camera.
	void	Render( CCamera* pCamera );

	// Returns scene associated with specified root node.
	static	CScene*			GetScene( CNode* pRootNode );

	static	void			Update();

	//---------------------------------------------------------------------------------------------

	// Returns root node of scene.
	inline	CNode*			GetRootNode();

	// Returns current scene being rendered.
	static	inline			CScene*	GetCurrentScene();

private :

	CNode			m_RootNode;
	list<CNode*>	m_Nodes;

	static	list<CScene*>	ms_Scenes;
	static	CScene*			ms_pCurrentScene;

};

inline CNode* CScene::GetRootNode()
{
	return &m_RootNode;
};

inline CScene* CScene::GetCurrentScene()
{
	return ms_pCurrentScene;
}

}