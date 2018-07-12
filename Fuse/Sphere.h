// Sphere.h

#pragma once

namespace Fuse
{

class CBox;

//
// Sphere representation.
//

class CSphere
{

public :

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CSphere();
	// Constructor allowing to specify sphere position and radius using float values.
	CSphere( float fX, float fY, float fZ, float fRadius );
	// Constructor allowing to specify sphere position and radius with a vector and a float.
	CSphere( CVector vPos, float fRadius );
	// Destructor.
	~CSphere();

	//---------------------------------------------------------------------------------------------

	// Finds on which side of the given plane the sphere lies.
	CPlane::ePlaneSides GetPlaneSide( const CPlane& Plane );

	// Finds out if the sphere intersects with another sphere.
	bool	Intersect( const CSphere& Sphere ) const;

	// Finds out if the sphere intersects with a box.
	bool	Intersect( const CBox& Box ) const;

	// Grow the sphere to include the given sphere.
	void Merge( const CSphere& Sphere );

	CVector		m_vPosition;
	float		m_fRadius;

};

const CSphere NULL_SPHERE( 0.0f, 0.0f, 0.0f, 0.0f );
const CSphere INFINITE_SPHERE( 0.0f, 0.0f, 0.0f, FLT_MAX );

}