// Node.cpp

#include "stdafx.h"
#include "Node.h"
#include "Scene.h"
#include "Renderable.h"
#include "Camera.h"
#include "HALRenderer.h"

u32				CNode::ms_uDisplayListSize = 0;
vector<CNode*>	CNode::ms_DisplayList;

CNode::CNode() :
	m_pParent		( NULL								),
	m_vPosition		( 0.0f, 0.0f, 0.0f					),
	m_mRotation		( CMatrix3::eMATRIX_IDENTITY		),
	m_vScale		( 1.0f, 1.0f, 1.0f					),
	m_vWorldPos		( 0.0f, 0.0f, 0.0f					),
	m_mWorldRot		( CMatrix3::eMATRIX_IDENTITY		),
	m_vWorldScale	( 1.0f, 1.0f, 1.0f					),
	m_uUpdateFlags	( UPDATE_TRANSFORM					),
	m_bVisible		( true								)
{
}

CNode::~CNode()
{
}

void CNode::AttachChild( CNode* pNewChild )
{
	assert( pNewChild );
	assert( pNewChild != this );
	if( pNewChild->m_pParent == this )
		return;
	m_Children.push_back( pNewChild );
	pNewChild->DetachFromParent();
	pNewChild->m_pParent = this;
}

void CNode::AttachRenderable( CRenderable* pRenderable, bool bUpdateBoundingSphere )
{
	assert( pRenderable );
	m_Renderables.push_back( pRenderable );
	UpdateBoundingSphere();
}

void CNode::DetachAllRenderables()
{
	m_Renderables.clear();
	UpdateBoundingSphere();
}

void CNode::DetachRenderable( CRenderable* pRenderable )
{
	assert( pRenderable );

	list<CRenderable*>::iterator it = find( m_Renderables.begin(), m_Renderables.end(), pRenderable );
	if ( it != m_Renderables.end() )
		m_Renderables.erase( it );
	else assert( false );

	UpdateBoundingSphere();
}

void CNode::DebugDumpSceneGraph( u32 uFlags, int uLevel )
{
	char szPad[DEFAULT_BUFFER_LENGTH];
	szPad[0] = 0;
	for ( int i = 0; i < uLevel; i++ )
		strcat_s( szPad, "  " );
	strcat_s( szPad,"+ " );
	Trace(	"%-20s%-25s[ %-15s ] Pos: %+010.3f, %+010.3f, %+010.3f\n", szPad, m_szName.c_str(), &(typeid( *this ).name()[6]),	// Discard "class " part from string
			m_vPosition.fX, m_vPosition.fY, m_vPosition.fZ );

	for ( list<CRenderable*>::iterator pRenderable = m_Renderables.begin(); pRenderable != m_Renderables.end(); ++pRenderable )
	{
		(*pRenderable)->DebugDumpSceneGraph( uFlags, uLevel + 1 );
		++pRenderable;
	}

	list<CNode*>::iterator pChild = m_Children.begin();
	while ( pChild != m_Children.end() )
	{
		(*pChild)->DebugDumpSceneGraph( 0, uLevel + 1 );
		++pChild;
	}
}

void CNode::DeleteSceneGraph()
{
	for ( list<CNode*>::iterator it = m_Children.begin(); it != m_Children.end(); ++it )
		(*it)->DeleteSceneGraph();
	delete this;
}

void CNode::DetachAllChildren()
{
	list<CNode*>::iterator it = m_Children.begin();
	while ( it != m_Children.end() )
	{
		list<CNode*>::iterator Next = it;
		++Next;
		(*it)->DetachFromParent();
		it = Next;
	}
}

void CNode::DetachChild( CNode* pChild )
{
	list<CNode*>::iterator it = find( m_Children.begin(), m_Children.end(), pChild );
	if ( it != m_Children.end() )
	{
		pChild->m_pParent = NULL;
		m_Children.erase( it );
	}
	else 
		assert( false );
}

void CNode::DetachFromParent()
{
	if ( m_pParent )
		m_pParent->DetachChild( this );
}

CNode* CNode::Find( const char* szName )
{
	assert( szName );

	if ( _stricmp( szName, m_szName.c_str() ) == 0 ) 
		return this;

	list<CNode*>::iterator pChild = m_Children.begin();
	while ( pChild != m_Children.end() )
	{
		CNode* pObject = (*pChild)->Find( szName );
		if ( pObject ) 
			return pObject;
		++pChild;
	}
	return NULL;
}

CNode* CNode::GetRoot()
{
	CNode* pParent = this;
	while ( pParent->GetParent() )
		pParent = pParent->GetParent();
	return pParent;
}

CScene* CNode::GetScene()
{
	CScene* pScene = CScene::GetCurrentScene();
	if ( !pScene ) // we don't know the current scene so get it from the root
		pScene = CScene::GetScene( GetRoot() );
	return pScene;
}

bool CNode::IsCulled( CCamera* pCamera )
{
	if ( m_WorldBoundingSphere.m_fRadius > EPSILON )
	{
		for ( u32 i = 0; i < CCamera::eMAX_PLANES; i++ )
		{
			CPlane::ePlaneSides Side = m_WorldBoundingSphere.GetPlaneSide( pCamera->GetClipPlane( (CCamera::eClipPlanes)i ) );
			if ( Side == CPlane::eBACK )
				// Object is completely on negative side; cull it.
				return true;
		}
	}
	return false;
}

void CNode::LookAt( const CVector& vLookAt )
{
	assert( m_pParent );
	m_mRotation.LookAt( m_vWorldPos, vLookAt );
	m_mRotation = m_mRotation * m_pParent->GetWorldRotation().Transpose();
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

void CNode::PreRender()
{
	CMatrix4 mWorld;
	mWorld.MakeFromPRS( m_vWorldPos, m_mWorldRot, m_vWorldScale );
	g_pRenderer->SetTransform( CHALRenderer::eWORLD_TRANSFORM, mWorld );
}

void CNode::Render( CCamera* pCamera )
{
	if ( !m_bVisible )
		return;

	if ( !IsCulled( pCamera ) )
	{
		// g_pRenderer->GetStats()->m_uModelsRendered[0]++;

		if ( ms_uDisplayListSize >= ms_DisplayList.size() )
			ms_DisplayList.push_back( this );
		else 
			ms_DisplayList[ms_uDisplayListSize] = this;
		ms_uDisplayListSize++;
	}

	for ( list<CNode*>::iterator pNode = m_Children.begin(); pNode != m_Children.end(); ++pNode )
		(*pNode)->Render( pCamera );
}

void CNode::RenderDisplayList( CCamera* pCamera )
{
	for ( u32 i = 0; i < ms_uDisplayListSize; i++ )
	{
		CNode* pNode = ms_DisplayList[i];
		pNode->PreRender();
		for ( list<CRenderable*>::iterator pRenderable = pNode->m_Renderables.begin(); pRenderable != pNode->m_Renderables.end(); ++pRenderable )
			(*pRenderable)->Render( pCamera, pNode );
		// g_pRenderer->DoDeferredRender();
		// if (  g_pDebugManager->IsActive() )
			pNode->RenderDebug( pCamera );
	}
}

void CNode::RenderDebug( CCamera* pCamera )
{
	/*
	if ( g_pDebugManager->GetShowModelBoundingSpheres() )
	{
		g_pDebugManager->RenderDebugSphere( m_WorldBoundingSphere.m_vPosition, m_WorldBoundingSphere.m_fRadius, 
											c_Yellow );
	}
	*/
}

void CNode::Update( u32 uUpdateFlags )
{
	u32 uFlags = uUpdateFlags | m_uUpdateFlags;
	if ( uFlags & UPDATE_TRANSFORM )
	{
		if ( m_pParent )
		{
			m_vWorldPos =	m_pParent->GetWorldPosition() +
							m_pParent->GetWorldScale().ComponentProduct( m_vPosition ) * m_pParent->GetWorldRotation();
			m_mWorldRot			= m_mRotation * m_pParent->GetWorldRotation();
 			m_vWorldScale		= m_pParent->GetWorldScale().ComponentProduct( m_vScale );
		}
		else
		{
			m_vWorldPos			= m_vPosition;
			m_mWorldRot			= m_mRotation;
			m_vWorldScale		= m_vScale;
		}
		m_WorldBoundingSphere.m_fRadius = m_BoundingSphere.m_fRadius * m_vWorldScale.LargestComponent();
		m_WorldBoundingSphere.m_vPosition = m_vWorldPos + 
										    m_vWorldScale.ComponentProduct( m_BoundingSphere.m_vPosition ) * m_mWorldRot;
	}

	for ( list<CNode*>::iterator pChild = m_Children.begin(); pChild != m_Children.end(); ++pChild )
		(*pChild)->Update( uFlags );
	m_uUpdateFlags = 0;
}

void CNode::UpdateBoundingSphere()
{
	m_BoundingSphere.m_fRadius = 0.0f;

	for ( list<CRenderable*>::iterator pRenderable = m_Renderables.begin(); pRenderable != m_Renderables.end(); ++pRenderable )
		m_BoundingSphere.Merge( (*pRenderable)->GetBoundingSphere() );
}