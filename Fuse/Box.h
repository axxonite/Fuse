// Box.h

#pragma once

namespace Fuse
{

//
// Box primitive.
//

class CBox
{

public :

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CBox();
	// Destructor.
	~CBox();

	//---------------------------------------------------------------------------------------------

	// Find the closest point to the test position on the surface of the box.
	bool FindClosestPointOnBox( CVector vTestPos, CVector& o_vClosestPoint ) const;

	// Finds out if the box intersects with a sphere.
	bool Intersect( const CSphere& Sphere );
	// Finds out if box intersects with another box.
	bool Intersect( const CBox& Box );

	CVector	m_vMin;
	CVector	m_vMax;

};

}