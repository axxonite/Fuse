// Plane.h

#pragma once

namespace Fuse
{

// Mathematical plane.

class CPlane
{

public :

	enum ePlaneSides
	{
		eUNKNOWN	= -1,
		eFRONT,
		eBACK,
		eALIGNED,
		eMAX_PLANE_SIDES
	};

	//---------------------------------------------------------------------------------------------

	// Default constructor of CPlane class.
	inline	CPlane();
	// Constructs plane from normal and constant value directly.
	inline	CPlane( const CVector& vNormal, float fConstant );
	// Constructs plane from normal and point on the plane.
	inline	CPlane( const CVector& vNormal, const CVector& vPoint );
	// Constructs plane from three points on the plane.
	inline	CPlane( const CVector& vPoint0, const CVector& vPoint1, const CVector& vPoint2 );

	//---------------------------------------------------------------------------------------------

	// Equivalence and assignment operators.
	inline	CPlane&		operator = ( const CPlane& Plane );
	inline	bool const	operator == ( const CPlane& Plane );
	inline	bool		operator != ( const CPlane& Plane );

	// Returns distance between a point and the plane.
	inline	float		GetDistance( const CVector& vPoint ) const;

	// Returns on which side of the plane a point is ( front, back or aligned with the plane ).
	inline	ePlaneSides	GetWhichSide( const CVector& vPoint ) const;

	// Flips plane.
	inline	void		Reverse();

	// Gets intersection of plane with an infinite ray.
			bool		IntersectRay( const CVector& vOrigin, const CVector& vDirection, CVector& vResult ) const;

	// Constructs plane from normal and constant value.
	inline	void		Set( const CVector& vNormal, float fConstant );
	// Constructs plane from normal and point on the plane.
	inline	void		Set( const CVector& vNormal, const CVector& vPoint );
	// Constructs plane from three points on the plane.
			void		Set( const CVector& vPoint0, const CVector& vPoint1, const CVector& vPoint2 );

	//---------------------------------------------------------------------------------------------

	CVector		m_vNormal;				// Normal vector of the plane
	float		m_fConstant;			// Constant of the plane

};

inline CPlane::CPlane()
{
}

inline CPlane::CPlane( const CVector& vNormal, float fConstant )
{
	m_vNormal	= vNormal;
	m_fConstant	= fConstant;
}

inline CPlane::CPlane( const CVector& vNormal, const CVector& vPoint )
{
	Set( vNormal, vPoint );
}

CPlane::CPlane( const CVector& vPoint0, const CVector& vPoint1, const CVector& vPoint2 )
{
	Set( vPoint0, vPoint1, vPoint2 );
}

inline CPlane& CPlane::operator = ( const CPlane& Plane )
{
	m_vNormal	= Plane.m_vNormal;
	m_fConstant = Plane.m_fConstant;
	return (*this);
}

inline bool const CPlane::operator == ( const CPlane& Plane )
{
    return ( m_vNormal == Plane.m_vNormal && IS_FLOAT_EQUAL( m_fConstant, Plane.m_fConstant ) );
}

inline bool	CPlane::operator != ( const CPlane& plane )	
{ 
	return !(*this == plane);				
}

inline float CPlane::GetDistance( const CVector& vPoint ) const
{ 
	return m_vNormal * vPoint - m_fConstant;	
}

inline void CPlane::Reverse()
{
	m_vNormal	= -m_vNormal;
	m_fConstant	= -m_fConstant;
}

inline void CPlane::Set( const CVector& vNormal, float fConstant )
{
	m_vNormal	= vNormal;
	m_fConstant	= fConstant;
}

inline void CPlane::Set( const CVector& vNormal, const CVector& vPoint )
{
	m_vNormal	= vNormal;
	m_fConstant = vNormal * vPoint;
}

inline CPlane::ePlaneSides CPlane::GetWhichSide( const CVector& vPoint ) const
{
    float fDistance =	m_vNormal.fX * vPoint.fX + 
						m_vNormal.fY * vPoint.fY + 
						m_vNormal.fZ * vPoint.fZ - m_fConstant;

    if ( fDistance < -EPSILON )		
		return eBACK;
    else if ( fDistance > EPSILON )	
		return eFRONT;
    else								
		return eALIGNED;
}

}