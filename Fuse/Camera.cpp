// Camera.cpp

#include "stdafx.h"
#include "Camera.h"
#include "Scene.h"
#include "HALRenderer.h"

const float DEFAULT_NEAR_PLANE = 1;
const float DEFAULT_FAR_PLANE = 20000;
const float DEFAULT_VIEW_ANGLE = 60;

CCamera::CCamera() :
	m_vViewPlaneNormal		( VECTOR_UNITZ		),
	m_vViewPlaneUp			( VECTOR_UNITY		),
	m_vViewPlaneRight		( VECTOR_UNITX		),
	m_vWorldViewPlanePos	( VECTOR_UNITZ		),
	m_vWorldViewPlaneNormal	( VECTOR_UNITZ		),
	m_vWorldViewPlaneUp		( VECTOR_UNITY		),
	m_vWorldViewPlaneRight	( VECTOR_UNITX		),
	m_mView					( MATRIX4_IDENTITY	),
	m_bInitialized			( false				)
{
	m_Frustum.fNear		= DEFAULT_NEAR_PLANE;
	m_Frustum.fFar		= DEFAULT_FAR_PLANE;
	m_fViewAngle = ANGLETORADIAN_FACTOR * DEFAULT_VIEW_ANGLE;	// 60 degrees
	for ( int i = 0; i < eMAX_PLANES; i++ ) 
		m_ClipPlanes[i].Set( VECTOR_ZERO, 0 );
}

CCamera::~CCamera()
{
}

void CCamera::ComputeProjMatrix()
{
	m_mProjection[0][0] = m_Frustum.fNear / m_Frustum.fHalfWidth ;
	m_mProjection[1][1] = m_Frustum.fNear / m_Frustum.fCorrectedHeight;

	float fFMN = m_Frustum.fFar - m_Frustum.fNear;
	m_mProjection[2][3] = 1.0f;
	m_mProjection[3][3] = 0.0f;

	// This is the projection matrix for shadow volumes
	m_mProjection[2][2] = 1.0f - EPSILON;
	m_mProjection[3][2] = m_Frustum.fNear * ( EPSILON - 1.0f );
}

void CCamera::Init()
{
	m_Frustum.fCorrectedHeight = m_Frustum.fHalfHeight * ( (float)g_pRenderer->GetFrameBufferHeight() / (float)g_pRenderer->GetFrameBufferWidth() );
	SetViewAngle( m_fViewAngle );
	m_bInitialized = true;
}

void CCamera::Render()
{
	assert( m_pScene );
	assert( m_bInitialized );
	g_pRenderer->SetTransform( CHALRenderer::eVIEW_TRANSFORM, m_mView );
	g_pRenderer->SetTransform( CHALRenderer::ePROJ_TRANSFORM, m_mProjection );
	m_pScene->Render( this );
}

void CCamera::SetViewAngle( float fAngle )
{
	m_fViewAngle				= fAngle;
	m_Frustum.fHalfWidth		= tan( m_fViewAngle / 2.0f ) * m_Frustum.fNear;
	m_Frustum.fHalfHeight		= m_Frustum.fHalfWidth;
	m_Frustum.fCorrectedHeight	= m_Frustum.fHalfHeight * ( (float)g_pRenderer->GetFrameBufferHeight() / (float)g_pRenderer->GetFrameBufferWidth() );
	ComputeProjMatrix();
}

void CCamera::Update( u32 uUpdateFlags )
{
	CNode::Update( uUpdateFlags );

	m_vWorldViewPlaneNormal		= m_vViewPlaneNormal * m_mWorldRot;
	m_vWorldViewPlaneUp			= m_vViewPlaneUp * m_mWorldRot;
	m_vWorldViewPlaneRight		= m_vWorldViewPlaneUp.Cross( m_vWorldViewPlaneNormal );

	UpdateClipPlanes();
	UpdateViewMatrix();
}

void CCamera::UpdateClipPlanes()
{
	m_vWorldViewPlanePos	= m_vWorldPos + m_Frustum.fNear * m_vWorldViewPlaneNormal;

	m_ClipPlanes[eNEAR].Set( m_vWorldViewPlaneNormal, m_vWorldViewPlanePos );
	CVector vDirection( 0.0f, 0.0f, 1.0f );
	CVector vPoint			= m_vWorldPos + m_Frustum.fFar * m_vWorldViewPlaneNormal;
	m_ClipPlanes[eFAR].Set( -m_vWorldViewPlaneNormal, vPoint );

	CVector vVector = m_Frustum.fNear * m_vWorldViewPlaneNormal - m_Frustum.fHalfWidth * m_vWorldViewPlaneRight;
	vVector.Normalize();
	CVector vNormal			= m_vWorldViewPlaneUp.NormCross( vVector );
	m_ClipPlanes[eLEFT].Set( vNormal, m_vWorldPos );

	vVector = m_Frustum.fNear * m_vWorldViewPlaneNormal + m_Frustum.fHalfWidth * m_vWorldViewPlaneRight;
	vVector.Normalize();
	vNormal					= vVector.NormCross( m_vWorldViewPlaneUp );
	m_ClipPlanes[eRIGHT].Set( vNormal, m_vWorldPos );

	vVector = m_Frustum.fNear * m_vWorldViewPlaneNormal + m_Frustum.fCorrectedHeight * m_vWorldViewPlaneUp;
	vVector.Normalize();
	vNormal					= m_vWorldViewPlaneRight.NormCross( vVector );
	m_ClipPlanes[eTOP].Set( vNormal, m_vWorldPos );

	vVector = m_Frustum.fNear * m_vWorldViewPlaneNormal - m_Frustum.fCorrectedHeight * m_vWorldViewPlaneUp;
	vVector.Normalize();
	vNormal					= vVector.NormCross( m_vWorldViewPlaneRight );
	m_ClipPlanes[eBOTTOM].Set( vNormal, m_vWorldPos );
}

void CCamera::UpdateViewMatrix()
{
	m_mView[0][0] = m_vWorldViewPlaneRight.fX;	
	m_mView[1][0] = m_vWorldViewPlaneRight.fY;	
	m_mView[2][0] = m_vWorldViewPlaneRight.fZ;
	m_mView[0][1] = m_vWorldViewPlaneUp.fX;		
	m_mView[1][1] = m_vWorldViewPlaneUp.fY;		
	m_mView[2][1] = m_vWorldViewPlaneUp.fZ;
	m_mView[0][2] = m_vWorldViewPlaneNormal.fX;	
	m_mView[1][2] = m_vWorldViewPlaneNormal.fY;	
	m_mView[2][2] = m_vWorldViewPlaneNormal.fZ;
	m_mView[3][0] = -( m_vWorldViewPlaneRight	* m_vWorldPos );
	m_mView[3][1] = -( m_vWorldViewPlaneUp		* m_vWorldPos );
	m_mView[3][2] = -( m_vWorldViewPlaneNormal	* m_vWorldPos );
	m_mView[0][3] = m_mView[1][3] = m_mView[2][3] = 0.0f;
	m_mView[3][3] = 1.0f;
}