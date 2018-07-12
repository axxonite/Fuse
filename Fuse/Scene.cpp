// Scene.cpp

#include "stdafx.h"
#include "Scene.h"

list<CScene*>		CScene::ms_Scenes;
CScene*				CScene::ms_pCurrentScene = NULL;

CScene::CScene()
{
	ms_Scenes.push_back( this );
	m_RootNode.SetName( "System Root" );
}

CScene::~CScene()
{
	ms_Scenes.remove( this );
}

void CScene::Render( CCamera* pCamera )
{
	CNode::ResetDisplayList();
	m_RootNode.Render( pCamera );
	CNode::RenderDisplayList( pCamera );
}

CScene* CScene::GetScene( CNode* pRootNode )
{
	for ( list<CScene*>::iterator pScene = ms_Scenes.begin(); pScene != ms_Scenes.end(); ++pScene )
	{
		if ( (*pScene)->GetRootNode() == pRootNode )
			return *pScene;
	}

	return NULL;
}

void CScene::Update()
{
	for ( list<CScene*>::iterator pScene = ms_Scenes.begin(); pScene != ms_Scenes.end(); ++pScene )
	{
		ms_pCurrentScene = (*pScene);
        ms_pCurrentScene->GetRootNode()->Update();
	}

	ms_pCurrentScene = NULL; 
}