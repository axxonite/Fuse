// Node.h

#pragma once

#include "Resource.h"

namespace Fuse
{

class CCamera;
class CScene;
class CQuaternion;
class CRenderable;
class CSphere;

//
// Scene graph node.
//

class CNode : public CResource
{

public :

	static	const u32	UPDATE_TRANSFORM		= 0x01;			// Force update of the world transform

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CNode();
	// Destructor.
	virtual ~CNode();

	//---------------------------------------------------------------------------------------------

	// Attaches and detaches children.
	void			AttachChild( CNode* pNewChild );
	void			DetachAllChildren();
	void			DetachChild( CNode* pChild );
	void			DetachFromParent();

	// Attaches and detaches renderables.
	void			AttachRenderable( CRenderable* pRenderable, bool bUpdateBoundingSphere = true );
	void			DetachAllRenderables();
	void			DetachRenderable( CRenderable* pRenderable );

	// Dumps the scene graph information to the debugging output
	virtual	void	DebugDumpSceneGraph( u32 uFlags = 0, int uLevel = 0 );

			void	DeleteSceneGraph();

	// Finds the first object in the scene graph with the specified name
	virtual	CNode*	Find( const char* szName );

	// Renders the node contents through the specified camera.
	virtual	void	Render( CCamera* pCamera );
	
	// Renders debug information associated with the node.
	virtual	void	RenderDebug( CCamera* pCamera );


	// Updates cached world transforms. Called by engine before rendering on every frame.
	virtual	void	Update( u32 uUpdateFlags = 0 );

	// Updates bounding sphere manually.
			void	UpdateBoundingSphere();

	// Restarts the display list.
	inline	static	void	ResetDisplayList();

	// Renders the display list.
			static	void	RenderDisplayList( CCamera* pCamera );

	//---------------------------------------------------------------------------------------------

	// Children access.
	inline	list<CNode*>::iterator		GetFirstNodeChild();
	inline	list<CNode*>::iterator		GetLastNodeChild();

	// Renderable access.
	inline	list<CRenderable*>::iterator	GetFirstRenderable();
	inline	list<CRenderable*>::iterator	GetLastRenderable();

	// Returns the node's parent.
	inline	CNode*				GetParent();

	// Returns ultimate parent of this node.
	CNode*						GetRoot();

	// Returns the scene this node is part of.
	CScene*						GetScene();

	// Node's local position.
	inline	void				GetPosition( CVector& vPos ) const;
	inline	CVector				GetPosition() const;
	inline	void				SetPosition( const CVector& vPos );
	inline	void				SetPosition( float fX, float fY, float fZ );

	// Node's local rotation.
	inline	const CMatrix3&		GetRotation() const;
	inline	void				GetRotation( CMatrix3& o_mMatrix );
	inline	void				SetRotation( const CMatrix3& mMatrix );
	inline	void				SetRotation( const CQuaternion& quat );
	inline	void				SetRotation( float fAngle, float fX, float fY, float fZ );
			void				LookAt( const CVector& vLookAt );

	// Node's local scale.
	inline	CVector				GetScale() const;
	inline	void				SetScale( const CVector& value );
	inline	void				SetScale( float fX, float fY, float fZ );

	// Returns the world bounding sphere.
	inline const CSphere&		GetWorldBoundingSphere();

	// World-coordinate position rotation and scale.
	// :WARNING: World transform is only guaranteed to be up to date after a call to Update()
	inline	CMatrix3			GetWorldRotation();
	inline	CVector				GetWorldPosition();
	inline	CVector				GetWorldScale();

	// Visible status.
	inline	bool				GetVisible() const;
	inline	void				SetVisible( bool bVisible );

protected :

	bool			m_bVisible;
	u32				m_uUpdateFlags;				// True if transform has changed since last frame

	CVector			m_vPosition;
	CMatrix3		m_mRotation;
	CVector			m_vScale;

	CVector			m_vWorldPos;
	CMatrix3		m_mWorldRot;
	CVector			m_vWorldScale;

	CNode*				m_pParent;
	list<CNode*>		m_Children;

	list<CRenderable*>	m_Renderables;

	CSphere				m_BoundingSphere;
	CSphere				m_WorldBoundingSphere;

	static	u32							ms_uDisplayListSize;
	static	vector<CNode*>				ms_DisplayList;

	bool	IsCulled( CCamera* pCamera );

	void	PreRender();

};

inline void	CNode::GetPosition( CVector& vPos ) const
{ 
	vPos = m_vPosition;						
};

inline CVector CNode::GetPosition() const
{ 
	return m_vPosition;					
};

inline const CMatrix3& CNode::GetRotation() const					
{ 
	return m_mRotation;				
};

inline void	CNode::GetRotation( CMatrix3& o_mMatrix )	
{ 
	o_mMatrix = m_mRotation;			
};

inline CVector CNode::GetScale() const				
{ 
	return m_vScale;						
};

inline list<CNode*>::iterator CNode::GetFirstNodeChild()
{
	return m_Children.begin();
}

inline list< CNode*>::iterator CNode::GetLastNodeChild()
{
	return m_Children.end();
}

inline CNode* CNode::GetParent()
{
	return m_pParent;
}

inline list<CRenderable*>::iterator CNode::GetFirstRenderable()
{
	return m_Renderables.begin();
}

inline list<CRenderable*>::iterator CNode::GetLastRenderable()
{
	return m_Renderables.end();
}

inline CVector CNode::GetWorldPosition()
{
	return m_vWorldPos;
}

inline CMatrix3 CNode::GetWorldRotation()
{
	return m_mWorldRot;
}

inline CVector CNode::GetWorldScale()
{
	return m_vWorldScale;
}

inline void CNode::ResetDisplayList()
{
	ms_uDisplayListSize = 0;
}

inline void CNode::SetPosition( const CVector& vVector )
{
	m_vPosition = vVector;
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

inline void CNode::SetPosition( float fX, float fY, float fZ )
{
	m_vPosition.Set( fX, fY, fZ );
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

inline void CNode::SetRotation( const CMatrix3& mMatrix )
{
	m_mRotation = mMatrix;
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

inline void CNode::SetRotation( float fAngle, float fX, float fY, float fZ )
{
	m_mRotation.MakeRotation( fAngle, fX, fY, fZ );
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

inline void CNode::SetRotation( const CQuaternion& quat )
{
	quat.GetMatrix( m_mRotation );
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

inline void CNode::SetScale( const CVector& vScale )
{
	m_vScale = vScale;
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

inline void CNode::SetScale( float fX, float fY, float fZ )
{
	m_vScale.fX = fX;
	m_vScale.fY = fY;
	m_vScale.fZ = fZ;
	m_uUpdateFlags |= UPDATE_TRANSFORM;
}

inline bool	CNode::GetVisible() const
{
	return m_bVisible;
}

inline void CNode::SetVisible( bool bVisible )
{
	m_bVisible = bVisible;
}

inline const CSphere& CNode::GetWorldBoundingSphere()
{
	return m_WorldBoundingSphere;
}

}