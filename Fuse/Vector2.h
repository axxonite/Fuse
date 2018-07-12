// Vector2.h

#pragma once

namespace Fuse
{

// 2-component vector class

class CVector2
{

public :

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CVector2()												{};
	// Initialized vector with specified components.
	inline CVector2( float fVx, float fVy );
	// Copy constructor.
	inline CVector2( const CVector2& vVec );

	//---------------------------------------------------------------------------------------------

	// Accesses vector as an array of floats.
	inline float&	operator[] ( u32 uIndex ) const;

	// Assignment and equivalence operators.
    inline CVector2&	operator = ( const CVector2& vVec );
	inline bool		operator == ( const CVector2& vVec ) const;
    inline bool		operator != ( const CVector2& vVec ) const;

	// Arithmetic operators.
    inline CVector2	operator + ( const CVector2& vVec ) const;	
    inline CVector2	operator - ( const CVector2& vVec ) const;
    inline float	operator * ( const CVector2& vVec ) const;
    inline CVector2	operator * ( float fScalar ) const;
    inline CVector2	operator / ( float fScalar ) const;
    inline CVector2	operator / ( const CVector2& vVec ) const;
    inline CVector2	operator - () const;

	// Returns length of vector.
    inline float	Length() const;

	// Normalizes vector.
	inline void		Normalize();

	// Component-by-component product.
    inline CVector2	ComponentProduct( const CVector2& vVec ) const;

	// Returns largest component in the vector.
	float			LargestComponent();

	// Sets components of the vector.
	inline void		Set( float fVx, float fVy );

	//---------------------------------------------------------------------------------------------

	// Vector components
	float fX;
	float fY;

};

const CVector2 VECTOR2_ZERO( 0, 0 );
const CVector2 VECTOR2_UNIT( 1, 1 );
const CVector2 VECTOR2_UNITX( 1, 0 );
const CVector2 VECTOR2_UNITY( 0, 1 );

inline CVector2::CVector2( float fVx, float fVy )
{ 
	fX = fVx; fY = fVy;				
};

inline CVector2::CVector2( const CVector2& vVec )								
{
	fX = vVec.fX; fY = vVec.fY;		
};

inline CVector2& CVector2::operator = ( const CVector2& vVec )
{ 
	fX = vVec.fX; fY = vVec.fY;
	return *this;	
};

inline bool	CVector2::operator == ( const CVector2& vVec ) const	
{
	return ( IS_FLOAT_EQUAL( fX, vVec.fX ) && IS_FLOAT_EQUAL( fY, vVec.fY ) );
};

inline bool	CVector2::operator != ( const CVector2& vVec ) const	
{
	return !(*this == vVec);
};

inline CVector2 CVector2::operator + ( const CVector2& vVec ) const	
{
	return CVector2( fX + vVec.fX, fY + vVec.fY );
};

inline CVector2 CVector2::operator - ( const CVector2& vVec ) const	
{ 
	return CVector2( fX - vVec.fX, fY - vVec.fY );
};

inline float CVector2::operator * ( const CVector2& vVec ) const		
{ 
	return fX * vVec.fX + fY * vVec.fY;
}

inline CVector2 CVector2::operator * ( float fScalar ) const
{ 
	return CVector2( fScalar * fX, fScalar * fY );
};

inline CVector2 CVector2::operator - () const	
{ 
	return CVector2( -fX, -fY );
};

inline float& CVector2::operator[] ( u32 uIndex ) const
{
	const float* pBase = &fX;
	return (float&) pBase[uIndex];
}

inline CVector2 operator * ( float fScalar, const CVector2& vVec )
{
    return CVector2( fScalar * vVec.fX, fScalar * vVec.fY );
}

inline CVector2 CVector2::operator / ( float fScalar ) const		
{
	float fInvS = 1.0f / fScalar;
	return CVector2( fInvS * fX, fInvS * fY );
};

inline CVector2 CVector2::operator / ( const CVector2& vVec ) const		
{
	return CVector2( fX / vVec.fX, fY / vVec.fY );
};

inline float CVector2::Length() const
{
	return (float)sqrtf( fX * fX + fY * fY );
};

inline void CVector2::Normalize()
{
    float fLength = Length();

    if ( fLength > EPSILON )
    {
        float fRecip = 1.0f / fLength;
        fX *= fRecip; fY *= fRecip;
    }
    else
    {
        fX = 0.0f; fY = 0.0f;
        fLength = 0.0f;
    }
}

inline void CVector2::Set( float fVx, float fVy )		
{
	fX = fVx; fY = fVy;
}

inline CVector2 CVector2::ComponentProduct( const CVector2& vVec ) const
{ 
	return CVector2( fX * vVec.fX, fY * vVec.fY );	
}

}