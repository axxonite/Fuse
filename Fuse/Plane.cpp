// Plane.cpp

#include "stdafx.h"
#include "Plane.h"

bool CPlane::IntersectRay( const CVector& vOrigin, const CVector& vDirection, CVector& vResult ) const
{
	float fT = - ( (  m_vNormal * vOrigin - m_fConstant ) / ( m_vNormal * vDirection ) );
	if ( fT < 0.0f ) 
		return false;
	vResult = vOrigin + vDirection * fT;
	return true;
}

void CPlane::Set( const CVector& vPoint0, const CVector& vPoint1, const CVector& vPoint2 )
{
	CVector temp = vPoint1 - vPoint0;
	CVector temp2 = vPoint2 - vPoint0;
	m_vNormal = ( vPoint1 - vPoint0 ).NormCross( vPoint2 - vPoint0 );
    m_fConstant = m_vNormal * vPoint0; 
}