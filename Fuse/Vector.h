// Vector.h

#pragma once

namespace Fuse
{

// 3-component vector class

class CVector
{

public :

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CVector()												{};
	// Initialized vector with specified components.
	inline CVector( float fVx, float fVy, float fVz );
	// Copy constructor.
	inline CVector( const CVector& vVec );

	//---------------------------------------------------------------------------------------------

	// Accesses vector as an array of floats.
	inline float&	operator[] ( u32 uIndex ) const;

	// Assignment and equivalence operators.
    inline CVector&	operator = ( const CVector& vVec );
	inline bool		operator == ( const CVector& vVec ) const;
    inline bool		operator != ( const CVector& vVec ) const;

	// Arithmetic operators.
    inline CVector	operator + ( const CVector& vVec ) const;	
    inline CVector	operator - ( const CVector& vVec ) const;
    inline float	operator * ( const CVector& vVec ) const;
    inline CVector	operator * ( float fScalar ) const;
    inline CVector	operator / ( float fScalar ) const;
    inline CVector	operator / ( const CVector& vVec ) const;
    inline CVector	operator - () const;

	// Returns length of vector.
    inline float	Length() const;

	// Normalizes vector.
	inline void		Normalize();

	// Cross product.
	inline CVector	Cross( const CVector& vVec ) const;	
	// Normalized cross product.
	inline CVector	NormCross( const CVector& vVec ) const;

	// Component-by-component product.
    inline CVector	ComponentProduct( const CVector& vVec ) const;

	// Returns largest component in the vector.
	float			LargestComponent() const;

	// Sets components of the vector.
	inline void		Set( float fVx, float fVy, float fVz );

	//---------------------------------------------------------------------------------------------

	// Vector components
	float fX;
	float fY;
	float fZ;

};

const CVector VECTOR_ZERO( 0, 0, 0 );
const CVector VECTOR_UNIT( 1, 1, 1 );
const CVector VECTOR_UNITX( 1, 0, 0 );
const CVector VECTOR_UNITY( 0, 1, 0 );
const CVector VECTOR_UNITZ( 0, 0, 1 );

inline CVector::CVector( float fVx, float fVy, float fVz )
{ 
	fX = fVx; fY = fVy; fZ = fVz;				
};

inline CVector::CVector( const CVector& vVec )								
{
	fX = vVec.fX; fY = vVec.fY; fZ = vVec.fZ;		
};

inline CVector& CVector::operator = ( const CVector& vVec )
{ 
	fX = vVec.fX; fY = vVec.fY; fZ = vVec.fZ;
	return *this;	
};

inline bool	CVector::operator == ( const CVector& vVec ) const	
{
	return ( IS_FLOAT_EQUAL( fX, vVec.fX ) && IS_FLOAT_EQUAL( fY, vVec.fY ) && IS_FLOAT_EQUAL( fZ, vVec.fZ ) );
};

inline bool	CVector::operator != ( const CVector& vVec ) const	
{
	return !(*this == vVec);
};

inline CVector CVector::operator + ( const CVector& vVec ) const	
{
	return CVector( fX + vVec.fX, fY + vVec.fY, fZ + vVec.fZ );
};

inline CVector CVector::operator - ( const CVector& vVec ) const	
{ 
	return CVector( fX - vVec.fX, fY - vVec.fY, fZ - vVec.fZ );
};

inline float CVector::operator * ( const CVector& vVec ) const		
{ 
	return fX * vVec.fX + fY * vVec.fY + fZ * vVec.fZ;
}

inline CVector CVector::operator * ( float fScalar ) const
{ 
	return CVector( fScalar * fX, fScalar * fY, fScalar * fZ );
};

inline CVector CVector::operator - () const	
{ 
	return CVector( -fX, -fY, -fZ );
};

inline float& CVector::operator[] ( u32 uIndex ) const
{
	const float* pBase = &fX;
	return (float&) pBase[uIndex];
}

inline CVector operator * ( float fScalar, const CVector& vVec )
{
    return CVector( fScalar * vVec.fX, fScalar * vVec.fY , fScalar * vVec.fZ );
}

inline CVector CVector::operator / ( float fScalar ) const		
{
	float fInvS = 1.0f / fScalar;
	return CVector( fInvS * fX, fInvS * fY, fInvS * fZ );
};

inline CVector CVector::operator / ( const CVector& vVec ) const		
{
	return CVector( fX / vVec.fX, fY / vVec.fY, fZ / vVec.fZ );
};

inline CVector CVector::Cross( const CVector& vVec ) const	
{ 
	return CVector( fY * vVec.fZ - fZ * vVec.fY, fZ * vVec.fX - fX * vVec.fZ, fX * vVec.fY - fY * vVec.fX );	
};

inline float CVector::Length() const
{
	return (float)sqrtf( fX * fX + fY * fY + fZ * fZ );
};

inline void CVector::Normalize()
{
    float fLength = Length();

    if ( fLength > EPSILON )
    {
        float fRecip = 1.0f / fLength;
        fX *= fRecip; fY *= fRecip; fZ *= fRecip;
    }
    else
    {
        fX = 0.0f; fY = 0.0f; fZ = 0.0f;
        fLength = 0.0f;
    }
}

inline CVector CVector::NormCross (const CVector& vVec ) const
{
    CVector cross( fY * vVec.fZ - fZ * vVec.fY, fZ * vVec.fX - fX * vVec.fZ, fX * vVec.fY - fY * vVec.fX );
	cross.Normalize();
	return cross;
}

inline void CVector::Set( float fVx, float fVy, float fVz )		
{
	fX = fVx; fY = fVy; fZ = fVz;	
}

inline CVector CVector::ComponentProduct( const CVector& vVec ) const
{ 
	return CVector( fX * vVec.fX, fY * vVec.fY, fZ * vVec.fZ );	
}

}