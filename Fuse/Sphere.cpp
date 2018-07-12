// BaseObject.cpp

#include "stdafx.h"
#include "Sphere.h"
#include "Box.h"

CSphere::CSphere() :
	m_vPosition( 0, 0, 0 ),
	m_fRadius( 0	)
{
}

CSphere::CSphere( float fX, float fY, float fZ, float fRadius ) :
	m_vPosition( fX, fY, fZ ),
	m_fRadius( fRadius	)
{
}

CSphere::CSphere( CVector vPos, float fRadius ) :
 m_vPosition( vPos ),
 m_fRadius( fRadius )
{
}

CSphere::~CSphere()
{
}

CPlane::ePlaneSides CSphere::GetPlaneSide( const CPlane& Plane )
{
	float fDistance = Plane.GetDistance( m_vPosition );
	if ( fDistance < -m_fRadius ) 
		return CPlane::eBACK;
	else if ( fDistance > m_fRadius ) 
		return CPlane::eFRONT;
	else 
		return CPlane::eALIGNED;
}

bool CSphere::Intersect( const CSphere& Sphere ) const
{
	float fDistance = ( m_vPosition - Sphere.m_vPosition ).Length();
	fDistance= fDistance * 2.0f;
	float fSquareSumOfRadius = m_fRadius + Sphere.m_fRadius;
	fSquareSumOfRadius *= fSquareSumOfRadius;

	return fDistance < fSquareSumOfRadius;
}

bool CSphere::Intersect( const CBox& Box ) const
{
	CVector vClosestPoint;
	Box.FindClosestPointOnBox( m_vPosition, vClosestPoint );
	CSphere BoxSphere( vClosestPoint, ( vClosestPoint - ( Box.m_vMax - Box.m_vMin ) / 2.0f ).Length() );
	return Intersect( BoxSphere );
}

void CSphere::Merge( const CSphere& Sphere )
{
	float fSphereDist = ( Sphere.m_vPosition - m_vPosition ).Length();
	if( Sphere.m_fRadius < EPSILON || m_fRadius > fSphereDist + Sphere.m_fRadius )
	{
		// This sphere totally contains the Sphere - do nothing
	}
	else if( m_fRadius < EPSILON || Sphere.m_fRadius > fSphereDist + m_fRadius )
	{
		// Sphere totally contains this sphere
		*this = Sphere;
	}
	else
	{
		if( Sphere.m_vPosition != m_vPosition )
		{
			CVector vNormal = ( Sphere.m_vPosition - m_vPosition ) / fSphereDist;
			CVector vSegStart = Sphere.m_vPosition + vNormal * Sphere.m_fRadius;
			CVector vSegEnd = m_vPosition - vNormal * m_fRadius;
			m_vPosition = ( vSegEnd + vSegStart ) / 2.0f;
		}
		// else otherwise the center stays the same
		m_fRadius = ( fSphereDist + m_fRadius + Sphere.m_fRadius ) / 2.0f;
	}	
}