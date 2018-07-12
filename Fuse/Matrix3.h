// Matrix3.h

#pragma once

namespace Fuse
{

class CMatrix4;

//
// 3-row matrix.
//

class CMatrix3
{

public :

	// Matrix construction types
	enum eMatrixTypes
	{
		eMATRIX_ZERO,
		eMATRIX_IDENTITY,
		eMATRIX_DIAGONAL,
		eMATRIX_XROTATION,
		eMATRIX_YROTATION,
		eMATRIX_ZROTATION,
		eMATRIX_AXISROTATION,
		eMATRIX_TYPECOUNT
	};
	
	//---------------------------------------------------------------------------------------------

	// Default constructor.
    CMatrix3 ();
	// Initializes matrix with specified elements.
	CMatrix3 (	float fM00, float fM01, float fM02,
				float fM10, float fM11, float fM12,
				float fM20, float fM21, float fM22 );
	// Initializes the matrix. 
	// Type: default values for the matrix. Can be
	//			eMATRIX_ZERO				Zero matrix, no extra arguments.
	//			eMATRIX_IDENTITY			Identity matrix, no extra arguments.
	//			eMATRIX_DIAGONAL			Diagonal matrix, specify 4 diagonal values.
	//			eMATRIX_XROTATION			x-axis rotation matrix, specify angle.
	//			eMATRIX_YROTATION			y-axis rotation matrix, specify angle.
	//			eMATRIX_ZROTATION			z-axis rotation matrix, specify angle.
	//			eMATRIX_AXISROTATION		Axis rotation matrix, specify angle and 3 components of 
	//										pivot axis.
	CMatrix3 ( eMatrixTypes Type, ... );
	// Copy constructor.
	CMatrix3 ( const CMatrix3& mMatrix );

	//---------------------------------------------------------------------------------------------

	// Returns a matrix row.
    inline	float*		operator[] ( u32 uIndex ) const;

	// Asignment and equivalence operators.
	inline	CMatrix3&	operator = ( const CMatrix3& mMatrix );
	inline  CMatrix3&	operator = ( const CMatrix4& mMatrix4 );
	inline	bool		operator == ( const CMatrix3& mMatrix ) const;
    inline	bool		operator != ( const CMatrix3& mMatrix ) const;		

	// Matrix arithmetic.
	inline	CMatrix3	operator + ( const CMatrix3& mMatrix ) const;
	inline	CMatrix3	operator - ( const CMatrix3& mMatrix ) const;
    inline	CMatrix3	operator * ( const CMatrix3& mMatrix ) const;

	// Scalar arithmetic.
    inline	CMatrix3	operator * ( float fScalar ) const;

	// Convert to 4-row matrix.
						operator CMatrix4()	const;

	// Inverses matrix.
			CMatrix3	Inverse() const;
			void		Inverse( CMatrix3& o_mResult ) const;

	// Makes diagonal matrix with specified diagonal values.
    inline void		MakeDiagonal( float fDiag0, float fDiag1, float fDiag2 );
	// Makes identity matrix.
	inline void		MakeIdentity();
	// Make rotation matrix along specified vector axis with specified angular rotation in radians
	void			MakeRotation( float fAngle, float fX, float fY, float fZ );
    void			MakeRotation( float fAngle, const CVector& Axis );
    // Makes X, Y and Z rotation matrices respectively.
	inline void		MakeXRotation( float fAngle );
    inline void		MakeYRotation( float fAngle );
    inline void		MakeZRotation( float fAngle );
	// Resets matrix to zero.
    inline void		MakeZero();

	// Multiply by transpose of the argument.
    inline CMatrix3		MultiplyTranspose( const CMatrix3& mMatrix ) const;
    inline CVector		MultiplyTranspose( const CVector& vVector ) const;

	// Basis vector access.
	inline CVector	GetDirection() const;
	inline CVector	GetUp() const;
	inline CVector	GetRight() const;
	inline void		SetDirection( const CVector& vDirection );
	inline void		SetDirUp( const CVector& vDirection, const CVector& vUp );

	// Extracts angle and axis from matrix.
	inline void		ExtractAngleAndAxis( float& o_fAngle, float& o_fX, float& o_fY, float& o_fZ ) const;

	// Returns basis direction, up and right vectors.
	inline void		GetOrientation( CVector& o_vDirection, CVector& o_vUp ) const;
	inline void		GetOrientation( CVector& o_vDirection, CVector& o_vUp, CVector& o_vRight ) const;

	// Makes matrix that looks at point vAt from point vFrom.
			void	LookAt( const CVector& vFrom, const CVector& vAt );

	// Tranposes matrix.
	inline CMatrix3	Transpose() const;
	inline void		Transpose( CMatrix3& o_mResult ) const;

	//---------------------------------------------------------------------------------------------

	// Matrix entries
    float			m_fEntry[3][3];

};

const CMatrix3 MATRIX3_ZERO( CMatrix3::eMATRIX_ZERO );
const CMatrix3 MATRIX3_IDENTITY( CMatrix3::eMATRIX_IDENTITY );

inline CMatrix3::CMatrix3()
{
}

inline CMatrix3::CMatrix3(	float fM00, float fM01, float fM02,
							float fM10, float fM11, float fM12,
							float fM20, float fM21, float fM22 )
{
    m_fEntry[0][0] = fM00;
    m_fEntry[0][1] = fM01;
    m_fEntry[0][2] = fM02;
    m_fEntry[1][0] = fM10;
    m_fEntry[1][1] = fM11;
    m_fEntry[1][2] = fM12;
    m_fEntry[2][0] = fM20;
    m_fEntry[2][1] = fM21;
    m_fEntry[2][2] = fM22;
}

inline CMatrix3::CMatrix3( const CMatrix3& mMatrix )
{
	memcpy( &m_fEntry[0][0], &mMatrix.m_fEntry[0][0], 9 * sizeof( float ) );
}

inline CMatrix3& CMatrix3::operator = ( const CMatrix3& mMatrix )
{

    memcpy( &m_fEntry[0][0], &mMatrix.m_fEntry[0][0], 9 * sizeof( float ) );
    return *this;
}

inline bool CMatrix3::operator == ( const CMatrix3& mMatrix ) const
{
	return	IS_FLOAT_EQUAL( m_fEntry[0][0], mMatrix.m_fEntry[0][0] ) &&
			IS_FLOAT_EQUAL( m_fEntry[0][1], mMatrix.m_fEntry[0][1] ) &&
			IS_FLOAT_EQUAL( m_fEntry[0][2], mMatrix.m_fEntry[0][2] ) &&
			IS_FLOAT_EQUAL( m_fEntry[1][0], mMatrix.m_fEntry[1][0] ) &&
			IS_FLOAT_EQUAL( m_fEntry[1][1], mMatrix.m_fEntry[1][1] ) &&
			IS_FLOAT_EQUAL( m_fEntry[1][2], mMatrix.m_fEntry[1][2] ) &&
			IS_FLOAT_EQUAL( m_fEntry[2][0], mMatrix.m_fEntry[2][0] ) &&
			IS_FLOAT_EQUAL( m_fEntry[2][1], mMatrix.m_fEntry[2][1] ) &&
			IS_FLOAT_EQUAL( m_fEntry[2][2], mMatrix.m_fEntry[2][2] ); 

}

inline bool CMatrix3::operator != ( const CMatrix3& mMatrix ) const			
{ 
	return !(*this == mMatrix);				
}

inline float* CMatrix3::operator[] ( u32 uIndex ) const		
{ 
	return (float*) &m_fEntry[uIndex][0];
}

inline CMatrix3 CMatrix3::operator + ( const CMatrix3& mMatrix ) const
{
	CMatrix3 Result = *this;
    Result.m_fEntry[0][0] += mMatrix.m_fEntry[0][0];
    Result.m_fEntry[0][1] += mMatrix.m_fEntry[0][1];
    Result.m_fEntry[0][2] += mMatrix.m_fEntry[0][2];
    Result.m_fEntry[1][0] += mMatrix.m_fEntry[1][0];
    Result.m_fEntry[1][1] += mMatrix.m_fEntry[1][1];
    Result.m_fEntry[1][2] += mMatrix.m_fEntry[1][2];
    Result.m_fEntry[2][0] += mMatrix.m_fEntry[2][0];
    Result.m_fEntry[2][1] += mMatrix.m_fEntry[2][1];
    Result.m_fEntry[2][2] += mMatrix.m_fEntry[2][2];
    return Result;
}

inline CMatrix3 CMatrix3::operator - ( const CMatrix3& mMatrix ) const
{
    CMatrix3 Result = *this;
    Result.m_fEntry[0][0] -= mMatrix.m_fEntry[0][0];
    Result.m_fEntry[0][1] -= mMatrix.m_fEntry[0][1];
    Result.m_fEntry[0][2] -= mMatrix.m_fEntry[0][2];
    Result.m_fEntry[1][0] -= mMatrix.m_fEntry[1][0];
    Result.m_fEntry[1][1] -= mMatrix.m_fEntry[1][1];
    Result.m_fEntry[1][2] -= mMatrix.m_fEntry[1][2];
    Result.m_fEntry[2][0] -= mMatrix.m_fEntry[2][0];
    Result.m_fEntry[2][1] -= mMatrix.m_fEntry[2][1];
    Result.m_fEntry[2][2] -= mMatrix.m_fEntry[2][2];
    return Result;
}

inline CMatrix3 CMatrix3::operator * ( const CMatrix3& mMatrix ) const
{
    CMatrix3 prd;

	prd[0][0] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[0][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[0][2] * mMatrix.m_fEntry[2][0];
    prd[1][0] =	m_fEntry[1][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[1][2] * mMatrix.m_fEntry[2][0];
    prd[2][0] =	m_fEntry[2][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[2][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][0];
    prd[0][1] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[0][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[0][2] * mMatrix.m_fEntry[2][1];
    prd[1][1] =	m_fEntry[1][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[1][2] * mMatrix.m_fEntry[2][1];
    prd[2][1] =	m_fEntry[2][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[2][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][1];
    prd[0][2] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[0][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[0][2] * mMatrix.m_fEntry[2][2];
    prd[1][2] =	m_fEntry[1][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[1][2] * mMatrix.m_fEntry[2][2];
    prd[2][2] =	m_fEntry[2][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[2][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][2];
    return prd;
}

inline CMatrix3 CMatrix3::operator * ( float fScalar ) const
{
    CMatrix3 Result;
    Result.m_fEntry[0][0] = m_fEntry[0][0] * fScalar;
    Result.m_fEntry[0][1] = m_fEntry[0][1] * fScalar;
    Result.m_fEntry[0][2] = m_fEntry[0][2] * fScalar;
    Result.m_fEntry[1][0] = m_fEntry[1][0] * fScalar;
    Result.m_fEntry[1][1] = m_fEntry[1][1] * fScalar;
    Result.m_fEntry[1][2] = m_fEntry[1][2] * fScalar;
    Result.m_fEntry[2][0] = m_fEntry[2][0] * fScalar;
    Result.m_fEntry[2][1] = m_fEntry[2][1] * fScalar;
    Result.m_fEntry[2][2] = m_fEntry[2][2] * fScalar;
    return Result;
}

inline CVector operator * ( const CVector& vVector, const CMatrix3& mMatrix )
{
    return CVector
    (
        vVector.fX * mMatrix.m_fEntry[0][0] + vVector.fY * mMatrix.m_fEntry[1][0] + vVector.fZ * mMatrix.m_fEntry[2][0],
        vVector.fX * mMatrix.m_fEntry[0][1] + vVector.fY * mMatrix.m_fEntry[1][1] + vVector.fZ * mMatrix.m_fEntry[2][1],
        vVector.fX * mMatrix.m_fEntry[0][2] + vVector.fY * mMatrix.m_fEntry[1][2] + vVector.fZ * mMatrix.m_fEntry[2][2]
    );
}

inline CVector CMatrix3::GetDirection() const
{
	CVector vResult;
	vResult.fX = m_fEntry[2][0]; 
	vResult.fY = m_fEntry[2][1]; 
	vResult.fZ = m_fEntry[2][2];
	return vResult;
}

inline CVector CMatrix3::GetUp() const
{
	CVector vResult;
	vResult.fX = m_fEntry[1][0]; 
	vResult.fY = m_fEntry[1][1]; 
	vResult.fZ = m_fEntry[1][2];
	return vResult;
}

inline CVector CMatrix3::GetRight() const
{
	CVector vResult;
	vResult.fX = m_fEntry[0][0];		
	vResult.fY = m_fEntry[0][1];	
	vResult.fZ = m_fEntry[0][2];
	return vResult;
}

inline void	CMatrix3::GetOrientation( CVector& o_vDirection, CVector& o_vUp ) const
{
	o_vDirection.fX	= m_fEntry[2][0]; 
	o_vDirection.fY = m_fEntry[2][1];	
	o_vDirection.fZ = m_fEntry[2][2];
	o_vUp.fX		= m_fEntry[1][0]; 
	o_vUp.fY		= m_fEntry[1][1];		
	o_vUp.fZ		= m_fEntry[1][2];
}

inline void	CMatrix3::GetOrientation( CVector& o_vDirection, CVector& o_vUp, CVector& o_vRight ) const
{
	o_vDirection.fX	= m_fEntry[2][0];		o_vDirection.fY = m_fEntry[2][1];	o_vDirection.fZ = m_fEntry[2][2];
	o_vUp.fX		= m_fEntry[1][0];		o_vUp.fY = m_fEntry[1][1];			o_vUp.fZ = m_fEntry[1][2];
	o_vRight.fX		= m_fEntry[0][0];		o_vRight.fY = m_fEntry[0][1];		o_vRight.fZ = m_fEntry[0][2];
}

inline void CMatrix3::MakeDiagonal( float fDiag0, float fDiag1, float fDiag2 )
{
    m_fEntry[0][0] = fDiag0;
    m_fEntry[0][1] = m_fEntry[0][2] = 0.0f;
	m_fEntry[1][0] = 0.0f;
    m_fEntry[1][1] = fDiag1;
    m_fEntry[1][2] = m_fEntry[2][0] = m_fEntry[2][1] = 0.0f;
    m_fEntry[2][2] = fDiag2;
}

inline void CMatrix3::MakeIdentity()
{
	m_fEntry[0][1] = m_fEntry[0][2]  = 0.0f;
	m_fEntry[1][0] = m_fEntry[1][2]  = 0.0f;
	m_fEntry[2][0] = m_fEntry[2][1]  = 0.0f;
    m_fEntry[0][0] = m_fEntry[1][1] = m_fEntry[2][2] = 1.0f;
}

inline void CMatrix3::MakeRotation( float fAngle, float fX, float fY, float fZ )
{

    float fCs	= (float)cosf( fAngle );
    float fSn	= (float)sinf( fAngle );
    float fOmcs	= 1.0f - fCs;
    float fX2	= fX * fX;
    float fY2	= fY * fY;
    float fZ2	= fZ * fZ;
    float fXym	= fX * fY * fOmcs;
    float fXzm	= fX * fZ * fOmcs;
    float fYzm	= fY * fZ * fOmcs;
    float fXsin	= fX * fSn;
    float fYsin	= fY * fSn;
    float fZsin	= fZ * fSn;

	m_fEntry[0][0] = fX2 * fOmcs + fCs;
    m_fEntry[0][1] = fXym + fZsin;
    m_fEntry[0][2] = fXzm - fYsin;
    m_fEntry[1][0] = fXym - fZsin;
    m_fEntry[1][1] = fY2 * fOmcs + fCs;
    m_fEntry[1][2] = fYzm + fXsin;
    m_fEntry[2][0] = fXzm + fYsin;
    m_fEntry[2][1] = fYzm - fXsin;
    m_fEntry[2][2] = fZ2 * fOmcs + fCs;
}

inline void CMatrix3::MakeRotation( float fAngle, const CVector& vAxis )
{
    MakeRotation( fAngle, vAxis.fX, vAxis.fY, vAxis.fZ );
}

inline void CMatrix3::MakeXRotation( float fAngle )
{
    float fCs = (float)cosf( fAngle );
    float fSn = (float)sinf( fAngle );
    
    m_fEntry[0][0] = 1.0f;
    m_fEntry[0][1] = m_fEntry[0][2] = 0.0f;
	m_fEntry[1][0] = 0.0f;
    m_fEntry[1][1] = fCs;
    m_fEntry[1][2] = fSn;
	m_fEntry[2][0] = 0.0f;
	m_fEntry[2][1] = -fSn;
    m_fEntry[2][2] = fCs;

}

inline void CMatrix3::MakeYRotation( float fAngle )
{
    float fCs = (float)cosf( fAngle );
    float fSn = (float)sinf( fAngle );
    
    m_fEntry[0][0] = fCs;
    m_fEntry[0][1] = 0.0f;
    m_fEntry[0][2] = -fSn;
	m_fEntry[1][0] = 0.0f;
    m_fEntry[1][1] = 1.0f;
	m_fEntry[1][2] = 0.0f;
    m_fEntry[2][0] = fSn;
	m_fEntry[2][1] = 0.0f;
    m_fEntry[2][2] = fCs;
}

inline void CMatrix3::MakeZRotation( float fAngle )
{
    float fCs = (float)cosf( fAngle );
    float fSn = (float)sinf( fAngle );

    m_fEntry[0][0] = fCs;
    m_fEntry[0][1] = fSn;
    m_fEntry[0][2]  = 0.0f;
    m_fEntry[1][0] = -fSn;
    m_fEntry[1][1] = fCs;
	m_fEntry[1][2] = m_fEntry[2][0] = m_fEntry[2][1] = 0.0f;
    m_fEntry[2][2] = 1.0f;
}

inline void CMatrix3::MakeZero()
{
    memset( &m_fEntry[0][0], 0, 9 * sizeof( float ) );
}

inline CMatrix3 CMatrix3::MultiplyTranspose( const CMatrix3& mMatrix ) const
{
	CMatrix3 prd;

	prd[0][0] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][0] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][0] * mMatrix.m_fEntry[2][0];
    prd[1][0] =	m_fEntry[0][1] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][1] * mMatrix.m_fEntry[2][0];
    prd[2][0] =	m_fEntry[0][2] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][2] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][0];
    prd[0][1] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][0] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][0] * mMatrix.m_fEntry[2][1];
    prd[1][1] =	m_fEntry[0][1] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][1] * mMatrix.m_fEntry[2][1];
    prd[2][1] =	m_fEntry[0][2] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][2] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][1];
    prd[0][2] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][0] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][0] * mMatrix.m_fEntry[2][2];
    prd[1][2] =	m_fEntry[0][1] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][1] * mMatrix.m_fEntry[2][2];
    prd[2][2] =	m_fEntry[0][2] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][2] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][2];
    return prd;

}

inline CVector CMatrix3::MultiplyTranspose( const CVector& vVector ) const
{
	CVector Result;
	Result.fX = m_fEntry[0][0] * vVector.fX +
				m_fEntry[0][1] * vVector.fY +
				m_fEntry[0][2] * vVector.fZ;
	Result.fY = m_fEntry[1][0] * vVector.fX +
				m_fEntry[1][1] * vVector.fY +
				m_fEntry[1][2] * vVector.fZ;
	Result.fZ = m_fEntry[2][0] * vVector.fX +
				m_fEntry[2][1] * vVector.fY +
				m_fEntry[2][2] * vVector.fZ;
	return Result;
}

inline void CMatrix3::SetDirection( const CVector& vDirection )
{
	CVector vWorldUp( 0.0f, 1.0f, 0.0f );
	CVector vRight = vWorldUp.NormCross( vDirection );
	CVector vUp = vDirection.Cross( vRight );
	m_fEntry[2][0] = vDirection.fX;	
	m_fEntry[2][1] = vDirection.fY;	
	m_fEntry[2][2] = vDirection.fZ;
	m_fEntry[1][0] = vUp.fX;		
	m_fEntry[1][1] = vUp.fY;		
	m_fEntry[1][2] = vUp.fZ;
	m_fEntry[0][0] = vRight.fX;
	m_fEntry[0][1] = vRight.fY;	
	m_fEntry[0][2] = vRight.fZ;
}

inline void CMatrix3::SetDirUp( const CVector& vDirection, const CVector& vUp )
{
	CVector vRight;
	if ( vDirection == vUp ) 
	{
		assert( false );
	}
	else 
		vRight  = vUp.Cross( vDirection );
	m_fEntry[2][0] = vDirection.fX;	
	m_fEntry[2][1] = vDirection.fY;	
	m_fEntry[2][2] = vDirection.fZ;
	m_fEntry[1][0] = vUp.fX;		
	m_fEntry[1][1] = vUp.fY;		
	m_fEntry[1][2] = vUp.fZ;
	m_fEntry[0][0] = vRight.fX;
	m_fEntry[0][1] = vRight.fY;	
	m_fEntry[0][2] = vRight.fZ;
}

inline CMatrix3 CMatrix3::Transpose() const
{
	CMatrix3 mMatrix;
	mMatrix[0][0] = m_fEntry[0][0];	mMatrix[0][1] = m_fEntry[1][0];	mMatrix[0][2] = m_fEntry[2][0];
	mMatrix[1][0] = m_fEntry[0][1];	mMatrix[1][1] = m_fEntry[1][1];	mMatrix[1][2] = m_fEntry[2][1];
	mMatrix[2][0] = m_fEntry[0][2];	mMatrix[2][1] = m_fEntry[1][2];	mMatrix[2][2] = m_fEntry[2][2];
	return mMatrix;
}

inline void CMatrix3::Transpose( CMatrix3& o_mResult ) const
{
	o_mResult[0][0] = m_fEntry[0][0];	o_mResult[0][1] = m_fEntry[1][0];	o_mResult[0][2] = m_fEntry[2][0];
	o_mResult[1][0] = m_fEntry[0][1];	o_mResult[1][1] = m_fEntry[1][1];	o_mResult[1][2] = m_fEntry[2][1];
	o_mResult[2][0] = m_fEntry[0][2];	o_mResult[2][1] = m_fEntry[1][2];	o_mResult[2][2] = m_fEntry[2][2];
}

}