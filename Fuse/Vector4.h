// Vector.h

#pragma once

namespace Fuse
{

// 3-component vector class

class CVector4
{

public :

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CVector4()															{};
	// Initializes vector with specified components.
	inline CVector4( float fVx, float fVy, float fVz, float fVw );
	// Copy constructor.
	inline CVector4( const CVector4& vVec );

	//---------------------------------------------------------------------------------------------

	// Accesses vector as an array of floats.
	inline float&		operator[] ( u32 uIndex ) const;

	// Assignment and equivalence operators.
    inline CVector4&	operator  = ( const CVector4& vVec );
	inline bool			operator == ( const CVector4& vVec ) const;
    inline bool			operator != ( const CVector4& vVec ) const;

	// Vector arithmetic.
    inline CVector4		operator + ( const CVector4& vVec ) const;	
    inline CVector4		operator - ( const CVector4& vVec ) const;
	inline CVector4		operator / ( const CVector4& vVec ) const;
    inline CVector4		operator - () const;

	// Scalar arithmetic.
	inline CVector4		operator / ( float fScalar ) const;
	inline CVector4		operator * ( float fScalar ) const;

	// Dot product.
    inline float		operator * ( const CVector4& vVec ) const;

	// Returns length of vector.
    inline float	Length() const;
   
	// Normalizes vector.
	inline void		Normalize();

	// Sets vector components.
	inline void		Set( float fVx, float fVy, float fVz, float fVw );

	//---------------------------------------------------------------------------------------------

	// Vector components
	float fX;
	float fY;
	float fZ;
	float fW;

};

const CVector4 VECTOR4_ZERO( 0, 0, 0, 0 );
const CVector4 VECTOR4_UNIT( 1, 1, 1, 1 );
const CVector4 VECTOR4_UNITX( 1, 0, 0, 1 );
const CVector4 VECTOR4_UNITY( 0, 1, 0, 1 );
const CVector4 VECTOR4_UNITZ( 0, 0, 1, 1 );

inline CVector4::CVector4( float fVx, float fVy, float fVz, float fVw )
{ 
	fX = fVx; fY = fVy; fZ = fVz; fW = fVw;				
};

inline CVector4::CVector4( const CVector4& vVec )								
{
	fX = vVec.fX; fY = vVec.fY; fZ = vVec.fZ; fW = vVec.fW;
};

inline CVector4& CVector4::operator = ( const CVector4& vVec )
{ 
	fX = vVec.fX; fY = vVec.fY; fZ = vVec.fZ; fW = vVec.fW;
	return *this;	
};

inline bool	CVector4::operator == ( const CVector4& vVec ) const	
{
	return (	IS_FLOAT_EQUAL( fX, vVec.fX ) && IS_FLOAT_EQUAL( fY, vVec.fY ) && IS_FLOAT_EQUAL( fZ, vVec.fZ ) &&
				IS_FLOAT_EQUAL( fZ, vVec.fW ) );
};

inline bool	CVector4::operator != ( const CVector4& vVec ) const	
{
	return !(*this == vVec);
};

inline CVector4	CVector4::operator + ( const CVector4& vVec ) const	
{
	return CVector4( fX + vVec.fX, fY + vVec.fY, fZ + vVec.fZ, fW + vVec.fW );
};

inline CVector4	CVector4::operator - ( const CVector4& vVec ) const	
{ 
	return CVector4( fX - vVec.fX, fY - vVec.fY, fZ - vVec.fZ, fW - vVec.fW );
};

inline float CVector4::operator * ( const CVector4& vVec ) const		
{ 
	return fX * vVec.fX + fY * vVec.fY + fZ * vVec.fZ + fW * vVec.fW;
}

inline CVector4	CVector4::operator * ( float fScalar ) const
{ 
	return CVector4( fScalar * fX, fScalar * fY, fScalar * fZ, fScalar * fW );
};

inline CVector4	CVector4::operator - () const	
{ 
	return CVector4( -fX, -fY, -fZ, -fW );
};

inline float& CVector4::operator[] ( u32 uIndex ) const
{
	const float* pBase = &fX;
	return (float&) pBase[uIndex];
}

inline CVector4 operator * ( float fScalar, const CVector4& vVec )
{
    return CVector4( fScalar * vVec.fX, fScalar * vVec.fY , fScalar * vVec.fZ, fScalar * vVec.fW );
}

inline CVector4 CVector4::operator / ( float fScalar ) const		
{
	float fInvS = 1.0f / fScalar;
	return CVector4( fInvS * fX, fInvS * fY, fInvS * fZ, fInvS * fW );
};

inline CVector4 CVector4::operator / ( const CVector4& vVec ) const		
{
	return CVector4( fX / vVec.fX, fY / vVec.fY, fZ / vVec.fZ, fW / vVec.fW );
};

inline float CVector4::Length() const
{
	return (float)sqrt( fX * fX + fY * fY + fZ * fZ + fW * fW );
};

inline void CVector4::Normalize()
{
    float fLength = Length();

    if ( fLength > EPSILON )
    {
        float fRecip = 1.0f / fLength;
        fX *= fRecip; fY *= fRecip; fZ *= fRecip; fW *= fRecip;
    }
    else
    {
        fX = 0.0f; fY = 0.0f; fZ = 0.0f; fW = 0.0f;
        fLength = 0.0f;
    }
}

inline void CVector4::Set( float fVx, float fVy, float fVz, float fVw )		
{
	fX = fVx; fY = fVy; fZ = fVz; fW = fVw;
}

}