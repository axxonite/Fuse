#include "stdafx.h"
#include "Box.h"

CBox::CBox()
{
	m_vMin.Set( 0, 0, 0 );
	m_vMax.Set( 0, 0, 0 );
}

CBox::~CBox()
{
}

bool CBox::FindClosestPointOnBox( CVector vTestPos, CVector& o_vClosestPoint ) const
{
	CVector vBoxHalfSize = ( m_vMax - m_vMin ) * 0.5f;
	CVector vPos = m_vMin + vBoxHalfSize;
	bool bOnBorder	= false;
	vTestPos = vTestPos - vPos;
	o_vClosestPoint = vTestPos;

	// Clip on X.
	if( vTestPos.fX > vBoxHalfSize.fX )
	{
		o_vClosestPoint.fX = vBoxHalfSize.fX;
		bOnBorder = true;
	}
	if( vTestPos.fX < -vBoxHalfSize.fX )
	{
		o_vClosestPoint.fX = -vBoxHalfSize.fX;
		bOnBorder = true;
	}

	// Clip on Y.
	if( vTestPos.fY > vBoxHalfSize.fY )
	{
		o_vClosestPoint.fY = vBoxHalfSize.fY;
		bOnBorder = true;
	}
	if( vTestPos.fY < -vBoxHalfSize.fY )
	{
		o_vClosestPoint.fY = -vBoxHalfSize.fY;
		bOnBorder = true;
	}

	// Clip on Z.
	if( vTestPos.fZ > vBoxHalfSize.fZ )
	{
		o_vClosestPoint.fZ = vBoxHalfSize.fZ;
		bOnBorder = true;
	}
	if( vTestPos.fZ < -vBoxHalfSize.fZ )
	{
		o_vClosestPoint.fZ = -vBoxHalfSize.fZ;
		bOnBorder = true;
	}

	o_vClosestPoint = o_vClosestPoint + vPos;
	return bOnBorder;
}

bool CBox::Intersect( const CSphere& Sphere )
{
	return Sphere.Intersect( *this );
}

bool CBox::Intersect( const CBox& Box )
{
	if (	( m_vMin.fX > Box.m_vMin.fX && m_vMin.fX < Box.m_vMax.fX ) ||
			( m_vMax.fX > Box.m_vMin.fX && m_vMin.fX < Box.m_vMax.fX ) ||
			( Box.m_vMin.fX > m_vMin.fX && Box.m_vMin.fX < m_vMax.fX ) ||
			( Box.m_vMax.fX > m_vMin.fX && Box.m_vMax.fX < m_vMax.fX ) )
		return true;
	if (	( m_vMin.fY > Box.m_vMin.fY && m_vMin.fY < Box.m_vMax.fY ) ||
			( m_vMax.fY > Box.m_vMin.fY && m_vMin.fY < Box.m_vMax.fY ) ||
			( Box.m_vMin.fY > m_vMin.fY && Box.m_vMin.fY < m_vMax.fY ) ||
			( Box.m_vMax.fY > m_vMin.fY && Box.m_vMax.fY < m_vMax.fY ) )
		return true;
	if (	( m_vMin.fZ > Box.m_vMin.fZ && m_vMin.fZ < Box.m_vMax.fZ ) ||
			( m_vMax.fZ > Box.m_vMin.fZ && m_vMin.fZ < Box.m_vMax.fZ ) ||
			( Box.m_vMin.fZ > m_vMin.fZ && Box.m_vMin.fZ < m_vMax.fZ ) ||
			( Box.m_vMax.fZ > m_vMin.fZ && Box.m_vMax.fZ < m_vMax.fZ ) )
		return true;
	return false;
}
