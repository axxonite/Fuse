// Matrix4.h

#pragma once

namespace Fuse
{

class CVector;
class CMatrix3;
class CVector4;

//
// 4-row matrix.
//

class CMatrix4
{

public :

	// Matrix construction types
	enum eMatrixTypes									// Matrix constructor types
	{
		eMATRIX_ZERO,
		eMATRIX_IDENTITY,
		eMATRIX_DIAGONAL,
		eMATRIX_XROTATION,
		eMATRIX_YROTATION,
		eMATRIX_ZROTATION,
		eMATRIX_AXISROTATION,
		eMATRIX_TRANSLATION,
		eMATRIX_TYPECOUNT
	};

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CMatrix4 ();
	// Initializes matrix with specified elements.
    CMatrix4 (	float fM00, float fM01, float fM02, float fM03,
				float fM10, float fM11, float fM12, float fM13,
				float fM20, float fM21, float fM22, float fM23,
				float fM30, float fM31, float fM32, float fM33 );
	// Initializes the matrix. 
	// Type: default values for the matrix. Can be
	//			eMATRIX_ZERO				Zero matrix, no extra arguments.
	//			eMATRIX_IDENTITY			Identity matrix, no extra arguments.
	//			eMATRIX_DIAGONAL			Diagonal matrix, specify 4 diagonal values.
	//			eMATRIX_XROTATION			vX-axis rotation matrix, specify angle.
	//			eMATRIX_YROTATION			vY-axis rotation matrix, specify angle.
	//			eMATRIX_ZROTATION			vZ-axis rotation matrix, specify angle.
	//			eMATRIX_AXISROTATION		Axis rotation matrix, specify angle and 3 components of 
	//										pivot axis.
	CMatrix4 ( eMatrixTypes Type, ...);
	// Copy constructor.
	CMatrix4 ( const CMatrix4& mMatrix );

	// Returns a matrix row.
    inline	float*		operator[] ( u32 uIndex ) const;

	// Asignment and equivalence operators.
	inline	CMatrix4&	operator = ( const CMatrix4& mMatrix );
	inline	bool		operator == ( const CMatrix4& mMatrix ) const;
    inline	bool		operator != ( const CMatrix4& mMatrix ) const;		

	// Matrix arithmetic.
	inline	CMatrix4	operator + ( const CMatrix4& mMatrix ) const;
	inline	CMatrix4	operator - ( const CMatrix4& mMatrix ) const;
    inline	CMatrix4	operator * ( const CMatrix4& mMatrix ) const;
    inline	CMatrix4	operator * ( float fScalar ) const;

 	// Makes diagonal matrix with specified diagonal values.
	inline	void		MakeDiagonal( float fDiag0, float fDiag1, float fDiag2 );
	// Makes identity matrix.
	inline	void		MakeIdentity();
	// Make rotation matrix along specified vector axis with specified angular rotation in radians
			void		MakeRotation( float fAngle, float fX, float fY, float fZ );
			void		MakeRotation( float fAngle, const CVector& Axis );
    // Makes vX, vY and vZ rotation matrices respectively.
    inline	void		MakeXRotation( float fAngle );
    inline	void		MakeYRotation( float fAngle );
    inline	void		MakeZRotation( float fAngle );
	// Constructs a translation matrix.
	inline	void		MakeTranslation( const CVector& vTranslation );
	// Resets matrix to zero.
	inline	void		MakeZero();

	// Makes matrix from position, rotation 3x3 matrix and scale arguments.
			void		MakeFromPRS( const CVector& vPos, const CMatrix3& mRot, const CVector& vScale );

 	// Multiply by transpose of the argument.
	inline	CMatrix4	MultiplyTranspose( const CMatrix4& mMatrix ) const;
	inline	CVector4	MultiplyTranspose( const CVector4& vVector ) const;

	// Extracts angle and axis from matrix.
			void		ExtractAngleAndAxis( float& o_fAngle, float& o_fX, float& o_fY, float& o_fZ ) const;

	// Gets or sets translation component of matrix.
	inline	CVector		GetTranslation() const;
	inline	void		SetTranslation( const CVector& vTranslation );
	// Removes rotation without translation or scaling component.
	inline	CMatrix3	GetRotation() const;
	// Gets or sets scaling component without affecting other components.
	inline	CVector		GetScale() const;
	inline	void		SetScale( const CVector& vScale );

	// Basis vector access.
	inline	void		GetDirection( CVector& o_vDirection ) const;
	inline	void		GetUp( CVector& o_vUp ) const;
	inline	void		GetRight( CVector& o_vRight ) const;

	inline	void		SetDirection( const CVector& vDirection );
	inline	void		SetDirUp( const CVector& vDirection, const CVector& vUp );

	// Returns basis direction, up and right vectors.
	inline	void		GetOrientation( CVector& o_vDirection, CVector& o_vUp ) const;
	inline	void		GetOrientation( CVector& o_vDirection, CVector& o_vUp, CVector& o_vRight ) const;

	// Tranposes matrix.
	inline	void		Transpose();
	inline	void		Transpose( CMatrix4& o_mResult ) const;

	// Inverses matrix.
			CMatrix4	Inverse() const;
			void		Inverse( CMatrix4& o_mResult ) const;

	operator			CMatrix3()	const;

	//---------------------------------------------------------------------------------------------

			// Matrix entries.
    float	m_fEntry[4][4];

};

const CMatrix4 MATRIX4_ZERO( CMatrix4::eMATRIX_ZERO );
const CMatrix4 MATRIX4_IDENTITY( CMatrix4::eMATRIX_IDENTITY );

inline CMatrix4::CMatrix4()
{
	MakeIdentity();
};

inline CMatrix4::CMatrix4 (	float fM00, float fM01, float fM02, float fM03,
							float fM10, float fM11, float fM12, float fM13,
							float fM20, float fM21, float fM22, float fM23,
							float fM30, float fM31, float fM32, float fM33 )
{
    m_fEntry[0][0] = fM00;
    m_fEntry[0][1] = fM01;
    m_fEntry[0][2] = fM02;
    m_fEntry[0][3] = fM03;
    m_fEntry[1][0] = fM10;
    m_fEntry[1][1] = fM11;
    m_fEntry[1][2] = fM12;
    m_fEntry[1][3] = fM13;
    m_fEntry[2][0] = fM20;
    m_fEntry[2][1] = fM21;
    m_fEntry[2][2] = fM22;
    m_fEntry[2][3] = fM23;
    m_fEntry[3][0] = fM30;
    m_fEntry[3][1] = fM31;
    m_fEntry[3][2] = fM32;
    m_fEntry[3][3] = fM33;

}

inline CMatrix4::CMatrix4( const CMatrix4& mMatrix )
{
    memcpy( &m_fEntry[0][0], &mMatrix.m_fEntry[0][0], 16 * sizeof( float ) );
}

inline float* CMatrix4::operator[] ( u32 uIndex ) const
{ 
	return (float*)&m_fEntry[uIndex][0];
}

inline CMatrix4& CMatrix4::operator = ( const CMatrix4& mMatrix )
{
    memcpy( &m_fEntry[0][0], &mMatrix.m_fEntry[0][0], 16 * sizeof( float ) );
	return *this;
}

/*
inline CVector4 CMatrix4::MultiplyTranspose( const CVector4& vVector ) const
{
	CVector4 Result;
	Result.fX = m_fEntry[0][0] * vVector.fX +
				m_fEntry[0][1] * vVector.fY +
				m_fEntry[0][2] * vVector.fZ +
				m_fEntry[0][3] * vVector.fW;
	Result.fY = m_fEntry[1][0] * vVector.fX +
				m_fEntry[1][1] * vVector.fY +
				m_fEntry[1][2] * vVector.fZ +
				m_fEntry[1][3] * vVector.fW;
	Result.fZ = m_fEntry[2][0] * vVector.fX +
				m_fEntry[2][1] * vVector.fY +
				m_fEntry[2][2] * vVector.fZ +
				m_fEntry[2][3] * vVector.fW;
	Result.fW = m_fEntry[3][0] * vVector.fX +
				m_fEntry[3][1] * vVector.fY +
				m_fEntry[3][2] * vVector.fZ +
				m_fEntry[3][3] * vVector.fW;
	return Result;

}
*/

inline bool CMatrix4::operator == ( const CMatrix4& mMatrix ) const
{
    return	IS_FLOAT_EQUAL( m_fEntry[0][0], mMatrix.m_fEntry[0][0] ) &&
			IS_FLOAT_EQUAL( m_fEntry[0][1], mMatrix.m_fEntry[0][1] ) &&
			IS_FLOAT_EQUAL( m_fEntry[0][2], mMatrix.m_fEntry[0][2] ) &&
			IS_FLOAT_EQUAL( m_fEntry[0][3], mMatrix.m_fEntry[0][3] ) &&
			IS_FLOAT_EQUAL( m_fEntry[1][0], mMatrix.m_fEntry[1][0] ) &&
			IS_FLOAT_EQUAL( m_fEntry[1][1], mMatrix.m_fEntry[1][1] ) &&
			IS_FLOAT_EQUAL( m_fEntry[1][2], mMatrix.m_fEntry[1][2] ) &&
			IS_FLOAT_EQUAL( m_fEntry[1][3], mMatrix.m_fEntry[1][3] ) &&
			IS_FLOAT_EQUAL( m_fEntry[2][0], mMatrix.m_fEntry[2][0] ) &&
			IS_FLOAT_EQUAL( m_fEntry[2][1], mMatrix.m_fEntry[2][1] ) &&
			IS_FLOAT_EQUAL( m_fEntry[2][2], mMatrix.m_fEntry[2][2] ) && 
			IS_FLOAT_EQUAL( m_fEntry[2][2], mMatrix.m_fEntry[2][3] ) &&
			IS_FLOAT_EQUAL( m_fEntry[3][0], mMatrix.m_fEntry[3][0] ) &&
			IS_FLOAT_EQUAL( m_fEntry[3][1], mMatrix.m_fEntry[3][1] ) &&
			IS_FLOAT_EQUAL( m_fEntry[3][2], mMatrix.m_fEntry[3][2] ) && 
			IS_FLOAT_EQUAL( m_fEntry[3][2], mMatrix.m_fEntry[3][3] ); 
}

inline bool CMatrix4::operator != ( const CMatrix4& mMatrix ) const			
{ 
	return !(*this == mMatrix);				
}

inline CMatrix4 CMatrix4::operator + ( const CMatrix4& mMatrix ) const
{
    CMatrix4 Result = *this;
    Result.m_fEntry[0][0] += mMatrix.m_fEntry[0][0];
    Result.m_fEntry[0][1] += mMatrix.m_fEntry[0][1];
    Result.m_fEntry[0][2] += mMatrix.m_fEntry[0][2];
    Result.m_fEntry[0][3] += mMatrix.m_fEntry[0][3];
    Result.m_fEntry[1][0] += mMatrix.m_fEntry[1][0];
    Result.m_fEntry[1][1] += mMatrix.m_fEntry[1][1];
    Result.m_fEntry[1][2] += mMatrix.m_fEntry[1][2];
    Result.m_fEntry[1][3] += mMatrix.m_fEntry[1][3];
    Result.m_fEntry[2][0] += mMatrix.m_fEntry[2][0];
    Result.m_fEntry[2][1] += mMatrix.m_fEntry[2][1];
    Result.m_fEntry[2][2] += mMatrix.m_fEntry[2][2];
    Result.m_fEntry[2][3] += mMatrix.m_fEntry[2][3];
    Result.m_fEntry[3][0] += mMatrix.m_fEntry[3][0];
    Result.m_fEntry[3][1] += mMatrix.m_fEntry[3][1];
    Result.m_fEntry[3][2] += mMatrix.m_fEntry[3][2];
    Result.m_fEntry[3][3] += mMatrix.m_fEntry[3][3];
    return Result;
}

inline CMatrix4 CMatrix4::operator - ( const CMatrix4& mMatrix ) const
{
    CMatrix4 Result = *this;
    Result.m_fEntry[0][0] -= mMatrix.m_fEntry[0][0];
    Result.m_fEntry[0][1] -= mMatrix.m_fEntry[0][1];
    Result.m_fEntry[0][2] -= mMatrix.m_fEntry[0][2];
    Result.m_fEntry[0][3] -= mMatrix.m_fEntry[0][3];
    Result.m_fEntry[1][0] -= mMatrix.m_fEntry[1][0];
    Result.m_fEntry[1][1] -= mMatrix.m_fEntry[1][1];
    Result.m_fEntry[1][2] -= mMatrix.m_fEntry[1][2];
    Result.m_fEntry[1][3] -= mMatrix.m_fEntry[1][3];
    Result.m_fEntry[2][0] -= mMatrix.m_fEntry[2][0];
    Result.m_fEntry[2][1] -= mMatrix.m_fEntry[2][1];
    Result.m_fEntry[2][2] -= mMatrix.m_fEntry[2][2];
    Result.m_fEntry[2][3] -= mMatrix.m_fEntry[2][3];
    Result.m_fEntry[3][0] -= mMatrix.m_fEntry[3][0];
    Result.m_fEntry[3][1] -= mMatrix.m_fEntry[3][1];
    Result.m_fEntry[3][2] -= mMatrix.m_fEntry[3][2];
    Result.m_fEntry[3][3] -= mMatrix.m_fEntry[3][3];
    return Result;
}

inline CMatrix4 CMatrix4::operator * ( const CMatrix4& mMatrix ) const
{
    CMatrix4 prd;
	prd[0][0] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[0][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[0][2] * mMatrix.m_fEntry[2][0] +
				m_fEntry[0][3] * mMatrix.m_fEntry[3][0];
    prd[1][0] =	m_fEntry[1][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[1][2] * mMatrix.m_fEntry[2][0] +
				m_fEntry[1][3] * mMatrix.m_fEntry[3][0];
    prd[2][0] =	m_fEntry[2][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[2][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][0] +
				m_fEntry[2][3] * mMatrix.m_fEntry[3][0];
    prd[3][0] =	m_fEntry[3][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[3][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[3][2] * mMatrix.m_fEntry[2][0] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][0];
    prd[0][1] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[0][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[0][2] * mMatrix.m_fEntry[2][1] +
				m_fEntry[0][3] * mMatrix.m_fEntry[3][1];
    prd[1][1] =	m_fEntry[1][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[1][2] * mMatrix.m_fEntry[2][1] +
				m_fEntry[1][3] * mMatrix.m_fEntry[3][1];
    prd[2][1] =	m_fEntry[2][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[2][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][1] +
				m_fEntry[2][3] * mMatrix.m_fEntry[3][1];
    prd[3][1] =	m_fEntry[3][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[3][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[3][2] * mMatrix.m_fEntry[2][1] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][1];
    prd[0][2] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[0][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[0][2] * mMatrix.m_fEntry[2][2] +
				m_fEntry[0][3] * mMatrix.m_fEntry[3][2];
    prd[1][2] =	m_fEntry[1][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[1][2] * mMatrix.m_fEntry[2][2] +
				m_fEntry[1][3] * mMatrix.m_fEntry[3][2];
    prd[2][2] =	m_fEntry[2][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[2][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][2] +
				m_fEntry[2][3] * mMatrix.m_fEntry[3][2];
    prd[3][2] =	m_fEntry[3][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[3][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[3][2] * mMatrix.m_fEntry[2][2] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][2];
    prd[0][3] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][3] +
				m_fEntry[0][1] * mMatrix.m_fEntry[1][3] +
				m_fEntry[0][2] * mMatrix.m_fEntry[2][3] +
				m_fEntry[0][3] * mMatrix.m_fEntry[3][3];
    prd[1][3] =	m_fEntry[1][0] * mMatrix.m_fEntry[0][3] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][3] +
				m_fEntry[1][2] * mMatrix.m_fEntry[2][3] +
				m_fEntry[1][3] * mMatrix.m_fEntry[3][3];
    prd[2][3] =	m_fEntry[2][0] * mMatrix.m_fEntry[0][3] +
				m_fEntry[2][1] * mMatrix.m_fEntry[1][3] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][3] +
				m_fEntry[2][3] * mMatrix.m_fEntry[3][3];
    prd[3][3] =	m_fEntry[3][0] * mMatrix.m_fEntry[0][3] +
				m_fEntry[3][1] * mMatrix.m_fEntry[1][3] +
				m_fEntry[3][2] * mMatrix.m_fEntry[2][3] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][3];
    return prd;
}
inline CMatrix4 CMatrix4::operator * ( float fScalar ) const
{
    CMatrix4 Result;
    Result.m_fEntry[0][0] = m_fEntry[0][0] * fScalar;
    Result.m_fEntry[0][1] = m_fEntry[0][1] * fScalar;
    Result.m_fEntry[0][2] = m_fEntry[0][2] * fScalar;
    Result.m_fEntry[0][3] = m_fEntry[0][3] * fScalar;
    Result.m_fEntry[1][0] = m_fEntry[1][0] * fScalar;
    Result.m_fEntry[1][1] = m_fEntry[1][1] * fScalar;
    Result.m_fEntry[1][2] = m_fEntry[1][2] * fScalar;
    Result.m_fEntry[1][3] = m_fEntry[1][3] * fScalar;
    Result.m_fEntry[2][0] = m_fEntry[2][0] * fScalar;
    Result.m_fEntry[2][1] = m_fEntry[2][1] * fScalar;
    Result.m_fEntry[2][2] = m_fEntry[2][2] * fScalar;
    Result.m_fEntry[2][3] = m_fEntry[2][3] * fScalar;
    Result.m_fEntry[3][0] = m_fEntry[3][0] * fScalar;
    Result.m_fEntry[3][1] = m_fEntry[3][1] * fScalar;
    Result.m_fEntry[3][2] = m_fEntry[3][2] * fScalar;
    Result.m_fEntry[3][3] = m_fEntry[3][3] * fScalar;
    return Result;
}

/*
inline CVector4 operator * ( const CVector& vVector, const CMatrix4& mMatrix )
{
    return CVector4(	vVector.fX * mMatrix.m_fEntry[0][0] + vVector.fY * mMatrix.m_fEntry[1][0] + vVector.fZ * mMatrix.m_fEntry[2][0] + mMatrix.m_fEntry[3][0],
						vVector.fX * mMatrix.m_fEntry[0][1] + vVector.fY * mMatrix.m_fEntry[1][1] + vVector.fZ * mMatrix.m_fEntry[2][1] + mMatrix.m_fEntry[3][1],
						vVector.fX * mMatrix.m_fEntry[0][2] + vVector.fY * mMatrix.m_fEntry[1][2] + vVector.fZ * mMatrix.m_fEntry[2][2] + mMatrix.m_fEntry[3][2],
						vVector.fX * mMatrix.m_fEntry[0][3] + vVector.fY * mMatrix.m_fEntry[1][3] + vVector.fZ * mMatrix.m_fEntry[2][3] + mMatrix.m_fEntry[3][3]
						);
}

inline CVector4 operator * ( const CVector4& vVector, const CMatrix4& mMatrix )
{
    return CVector4(	vVector.fX * mMatrix.m_fEntry[0][0] + vVector.fY * mMatrix.m_fEntry[1][0] + vVector.fZ * mMatrix.m_fEntry[2][0] + vVector.fW * mMatrix.m_fEntry[3][0],
						vVector.fX * mMatrix.m_fEntry[0][1] + vVector.fY * mMatrix.m_fEntry[1][1] + vVector.fZ * mMatrix.m_fEntry[2][1] + vVector.fW * mMatrix.m_fEntry[3][1],
						vVector.fX * mMatrix.m_fEntry[0][2] + vVector.fY * mMatrix.m_fEntry[1][2] + vVector.fZ * mMatrix.m_fEntry[2][2] + vVector.fW * mMatrix.m_fEntry[3][2],
						vVector.fX * mMatrix.m_fEntry[0][3] + vVector.fY * mMatrix.m_fEntry[1][3] + vVector.fZ * mMatrix.m_fEntry[2][3] + vVector.fW * mMatrix.m_fEntry[3][3]
						);
}
*/

inline void	CMatrix4::GetDirection( CVector& o_vDirection ) const
{
	o_vDirection.fX = m_fEntry[2][0]; 
	o_vDirection.fY = m_fEntry[2][1]; 
	o_vDirection.fZ = m_fEntry[2][2];
}

inline void	CMatrix4::GetUp( CVector& o_vUp ) const
{
	o_vUp.fX = m_fEntry[1][0]; 
	o_vUp.fY = m_fEntry[1][1]; 
	o_vUp.fZ = m_fEntry[1][2];
}

inline void	CMatrix4::GetRight( CVector& o_vRight ) const
{
	o_vRight.fX = m_fEntry[0][0];		
	o_vRight.fY = m_fEntry[0][1];	
	o_vRight.fZ = m_fEntry[0][2];
}

inline void	CMatrix4::GetOrientation( CVector& o_vDirection, CVector& o_vUp ) const
{
	o_vDirection.fX	= m_fEntry[2][0]; 
	o_vDirection.fY = m_fEntry[2][1];	
	o_vDirection.fZ = m_fEntry[2][2];
	
	o_vUp.fX		= m_fEntry[1][0]; 
	o_vUp.fY		= m_fEntry[1][1];		
	o_vUp.fZ		= m_fEntry[1][2];
}

inline void	CMatrix4::GetOrientation( CVector& o_vDirection, CVector& o_vUp, CVector& o_vRight ) const
{
	o_vDirection.fX	= m_fEntry[2][0];		
	o_vDirection.fY = m_fEntry[2][1];	
	o_vDirection.fZ = m_fEntry[2][2];
	
	o_vUp.fX		= m_fEntry[1][0];		
	o_vUp.fY		= m_fEntry[1][1];			
	o_vUp.fZ		= m_fEntry[1][2];
	
	o_vRight.fX		= m_fEntry[0][0];		
	o_vRight.fY		= m_fEntry[0][1];		
	o_vRight.fZ		= m_fEntry[0][2];
}

inline CVector CMatrix4::GetTranslation() const
{
	return CVector( m_fEntry[3][0], m_fEntry[3][1], m_fEntry[3][2] );
}

inline void CMatrix4::SetTranslation( const CVector& vTranslation )
{
	m_fEntry[3][0] = vTranslation.fX; m_fEntry[3][1] = vTranslation.fY; m_fEntry[3][2] = vTranslation.fZ;
}

inline CMatrix3 CMatrix4::GetRotation() const
{
	CMatrix4 mCopy = *this;
	mCopy.SetScale( CVector( 1, 1, 1 ) );
	CMatrix3 mMatrix3 = mCopy;
	return mMatrix3;
}

inline CVector CMatrix4::GetScale() const
{
	return CVector(	CVector( m_fEntry[0][0], m_fEntry[0][1], m_fEntry[0][2] ).Length(),
					CVector( m_fEntry[1][0], m_fEntry[1][1], m_fEntry[1][2] ).Length(),
					CVector( m_fEntry[0][0], m_fEntry[2][1], m_fEntry[2][2] ).Length() );
}

inline void CMatrix4::SetScale( const CVector& vScale )
{
	CVector vX( m_fEntry[0][0], m_fEntry[1][0], m_fEntry[2][0] );
	CVector vY( m_fEntry[0][1], m_fEntry[1][1], m_fEntry[2][1] );
	CVector vZ( m_fEntry[0][2], m_fEntry[1][2], m_fEntry[2][2] );
	vX.Normalize();
	vY.Normalize();
	vZ.Normalize();
	m_fEntry[0][0] = vX.fX * vScale.fX;	m_fEntry[0][1] = vY.fX;				m_fEntry[0][2] = vZ.fX;
	m_fEntry[1][0] = vX.fY;				m_fEntry[1][1] = vY.fY * vScale.fY;	m_fEntry[1][2] = vZ.fY;
	m_fEntry[2][0] = vX.fZ;				m_fEntry[2][1] = vY.fZ;				m_fEntry[2][2] = vZ.fZ * vScale.fZ;
}

inline void CMatrix4::MakeDiagonal( float fDiag0, float fDiag1, float fDiag2 )
{
	m_fEntry[0][0] = fDiag0;
    m_fEntry[0][1] = m_fEntry[0][2] = m_fEntry[0][3] = 0.0f;
	m_fEntry[1][0] = 0.0f;
    m_fEntry[1][1] = fDiag1;
    m_fEntry[1][2] = m_fEntry[1][3] = m_fEntry[2][0] = m_fEntry[2][1] = 0.0f;
    m_fEntry[2][2] = fDiag2;
	m_fEntry[2][3] = m_fEntry[3][0] = m_fEntry[3][1] = m_fEntry[3][2] = 0.0f;
	m_fEntry[3][3] = 1.0f;
}

inline void CMatrix4::MakeIdentity()
{
	m_fEntry[0][1] = m_fEntry[0][2] = m_fEntry[0][3] = 0.0f;
	m_fEntry[1][0] = m_fEntry[1][2] = m_fEntry[1][3] = 0.0f;
	m_fEntry[2][0] = m_fEntry[2][1] = m_fEntry[2][3] = 0.0f;
	m_fEntry[3][0] = m_fEntry[3][1] = m_fEntry[3][2] = 0.0f;
    m_fEntry[0][0] = m_fEntry[1][1]	= m_fEntry[2][2] = m_fEntry[3][3] = 1.0f;
}

inline void CMatrix4::MakeRotation( float fAngle, float fX, float fY, float fZ )
{
    float fCs	= (float)cos( fAngle );
    float fSn	= (float)sin( fAngle );
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
	m_fEntry[0][3] = m_fEntry[1][3] = m_fEntry[2][3] = 0.0f;
	m_fEntry[3][0] = m_fEntry[3][1] = m_fEntry[3][2] = 0.0f;
	m_fEntry[3][3] = 1.0f;
}

inline void CMatrix4::MakeRotation( float fAngle, const CVector& vAxis )
{
    MakeRotation( fAngle, vAxis.fX, vAxis.fY, vAxis.fZ );
}

inline void CMatrix4::MakeTranslation( const CVector& vTranslation )
{
	m_fEntry[0][0] = m_fEntry[1][1] = m_fEntry[2][2] = 1;
    m_fEntry[0][1] = m_fEntry[0][2] = m_fEntry[0][3] = 0.0f;
	m_fEntry[1][0] = 0.0f; m_fEntry[1][2] = m_fEntry[1][3] = 0;
	m_fEntry[2][0] = m_fEntry[2][1] = m_fEntry[2][3] = 0;
	m_fEntry[3][0] = vTranslation.fX;
	m_fEntry[3][1] = vTranslation.fY;
	m_fEntry[3][2] = vTranslation.fZ;
	m_fEntry[3][3] = 1;
}

inline void CMatrix4::MakeXRotation( float fAngle )
{
    float fCs = (float)cos( fAngle );
    float fSn = (float)sin( fAngle );
    
    m_fEntry[0][0] = 1.0f;
    m_fEntry[0][1] = m_fEntry[0][2] = 0.0f;
	m_fEntry[1][0] = 0.0f;
    m_fEntry[1][1] = fCs;
    m_fEntry[1][2] = fSn;
	m_fEntry[2][0] = 0.0f;
	m_fEntry[2][1] = -fSn;
    m_fEntry[2][2] = fCs;
	m_fEntry[0][3] = m_fEntry[1][3] = m_fEntry[2][3] = 0.0f;
	m_fEntry[3][0] = m_fEntry[3][1] = m_fEntry[3][2] = 0.0f;
	m_fEntry[3][3] = 1.0f;
}

inline void CMatrix4::MakeYRotation( float fAngle )
{
    float fCs = (float)cos( fAngle );
    float fSn = (float)sin( fAngle );
    
    m_fEntry[0][0] = fCs;
    m_fEntry[0][1] = 0.0f;
    m_fEntry[0][2] = -fSn;
	m_fEntry[1][0] = 0.0f;
    m_fEntry[1][1] = 1.0f;
	m_fEntry[1][2] = 0.0f;
    m_fEntry[2][0] = fSn;
	m_fEntry[2][1] = 0.0f;
    m_fEntry[2][2] = fCs;
	m_fEntry[0][3] = m_fEntry[1][3] = m_fEntry[2][3] = 0.0f;
	m_fEntry[3][0] = m_fEntry[3][1] = m_fEntry[3][2] = 0.0f;
	m_fEntry[3][3] = 1.0f;
}

inline void CMatrix4::MakeZRotation( float fAngle )
{
    float fCs = (float)cos( fAngle );
    float fSn = (float)sin( fAngle );

    m_fEntry[0][0] = fCs;
    m_fEntry[0][1] = fSn;
    m_fEntry[0][2]  = 0.0f;
    m_fEntry[1][0] = -fSn;
    m_fEntry[1][1] = fCs;
	m_fEntry[1][2] = m_fEntry[2][0] = m_fEntry[2][1] = 0.0f;
    m_fEntry[2][2] = 1.0f;
	m_fEntry[0][3] = m_fEntry[1][3] = m_fEntry[2][3] = 0.0f;
	m_fEntry[3][0] = m_fEntry[3][1] = m_fEntry[3][2] = 0.0f;
	m_fEntry[3][3] = 1.0f;
}

inline void CMatrix4::MakeZero()
{
    memset( &m_fEntry[0][0], 0, 16 * sizeof( float ) );
}

//-------------------------------------------------------------------------------------------------
inline CMatrix4 CMatrix4::MultiplyTranspose( const CMatrix4& mMatrix ) const
{
	CMatrix4 prd;
	prd[0][0] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][0] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][0] * mMatrix.m_fEntry[2][0] +
				m_fEntry[3][0] * mMatrix.m_fEntry[3][0];
    prd[1][0] =	m_fEntry[0][1] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][1] * mMatrix.m_fEntry[2][0] +
				m_fEntry[3][1] * mMatrix.m_fEntry[3][0];
    prd[2][0] =	m_fEntry[0][2] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][2] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][0] +
				m_fEntry[3][2] * mMatrix.m_fEntry[3][0];
    prd[3][0] =	m_fEntry[0][3] * mMatrix.m_fEntry[0][0] +
				m_fEntry[1][3] * mMatrix.m_fEntry[1][0] +
				m_fEntry[2][3] * mMatrix.m_fEntry[2][0] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][0];
    prd[0][1] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][0] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][0] * mMatrix.m_fEntry[2][1] +
				m_fEntry[3][0] * mMatrix.m_fEntry[3][1];
    prd[1][1] =	m_fEntry[0][1] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][1] * mMatrix.m_fEntry[2][1] +
				m_fEntry[3][1] * mMatrix.m_fEntry[3][1];
    prd[2][1] =	m_fEntry[0][2] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][2] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][1] +
				m_fEntry[3][2] * mMatrix.m_fEntry[3][1];
    prd[3][1] =	m_fEntry[0][3] * mMatrix.m_fEntry[0][1] +
				m_fEntry[1][3] * mMatrix.m_fEntry[1][1] +
				m_fEntry[2][3] * mMatrix.m_fEntry[2][1] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][1];
    prd[0][2] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][0] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][0] * mMatrix.m_fEntry[2][2] +
				m_fEntry[3][0] * mMatrix.m_fEntry[3][2];
    prd[1][2] =	m_fEntry[0][1] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][1] * mMatrix.m_fEntry[2][2] +
				m_fEntry[3][1] * mMatrix.m_fEntry[3][2];
    prd[2][2] =	m_fEntry[0][2] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][2] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][2] +
				m_fEntry[3][2] * mMatrix.m_fEntry[3][2];
    prd[3][2] =	m_fEntry[0][3] * mMatrix.m_fEntry[0][2] +
				m_fEntry[1][3] * mMatrix.m_fEntry[1][2] +
				m_fEntry[2][3] * mMatrix.m_fEntry[2][2] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][2];
    prd[0][3] =	m_fEntry[0][0] * mMatrix.m_fEntry[0][3] +
				m_fEntry[1][0] * mMatrix.m_fEntry[1][3] +
				m_fEntry[2][0] * mMatrix.m_fEntry[2][3] +
				m_fEntry[3][0] * mMatrix.m_fEntry[3][3];
    prd[1][3] =	m_fEntry[0][1] * mMatrix.m_fEntry[0][3] +
				m_fEntry[1][1] * mMatrix.m_fEntry[1][3] +
				m_fEntry[2][1] * mMatrix.m_fEntry[2][3] +
				m_fEntry[3][1] * mMatrix.m_fEntry[3][3];
    prd[2][3] =	m_fEntry[0][2] * mMatrix.m_fEntry[0][3] +
				m_fEntry[1][2] * mMatrix.m_fEntry[1][3] +
				m_fEntry[2][2] * mMatrix.m_fEntry[2][3] +
				m_fEntry[3][2] * mMatrix.m_fEntry[3][3];
    prd[3][3] =	m_fEntry[0][3] * mMatrix.m_fEntry[0][3] +
				m_fEntry[1][3] * mMatrix.m_fEntry[1][3] +
				m_fEntry[2][3] * mMatrix.m_fEntry[2][3] +
				m_fEntry[3][3] * mMatrix.m_fEntry[3][3];
    return prd;
}

inline void CMatrix4::SetDirection( const CVector& vDirection )
{
	CVector vUp;	vUp.Set( 0.0f, 1.0f, 0.0f );
	SetDirUp( vDirection, vUp );
}

inline void CMatrix4::SetDirUp( const CVector& vDirection, const CVector& vUp )
{
	CVector vRight;
	if ( vDirection == vUp ) 
	{
		assert( false );
	}
	else 
		vRight  = vUp.Cross( vDirection );
	m_fEntry[2][0] = vDirection.fX;	m_fEntry[2][1] = vDirection.fY;	m_fEntry[2][2] = vDirection.fZ;
	m_fEntry[1][0] = vUp.fX;		m_fEntry[1][1] = vUp.fY;		m_fEntry[1][2] = vUp.fZ;
	m_fEntry[0][0] = vRight.fX;	m_fEntry[0][1] = vRight.fY;	m_fEntry[0][2] = vRight.fZ;
	m_fEntry[0][3] = m_fEntry[1][3] = m_fEntry[2][3] = 0.0f;
	m_fEntry[3][0] = m_fEntry[3][1] = m_fEntry[3][2] = 0.0f;
	m_fEntry[3][3] = 1.0f;
}

inline void CMatrix4::Transpose()
{
	float	fTemp = m_fEntry[0][1];	m_fEntry[0][1] = m_fEntry[1][0];	m_fEntry[1][0] = fTemp;
			fTemp = m_fEntry[0][2];	m_fEntry[0][2] = m_fEntry[2][0];	m_fEntry[2][0] = fTemp;
			fTemp = m_fEntry[0][3];	m_fEntry[0][3] = m_fEntry[3][0];	m_fEntry[3][0] = fTemp;
			fTemp = m_fEntry[1][2];	m_fEntry[1][2] = m_fEntry[2][1];	m_fEntry[2][1] = fTemp;
			fTemp = m_fEntry[1][3];	m_fEntry[1][3] = m_fEntry[3][1];	m_fEntry[3][1] = fTemp;
			fTemp = m_fEntry[2][3];	m_fEntry[2][3] = m_fEntry[3][2];	m_fEntry[3][2] = fTemp;
}

inline void CMatrix4::Transpose( CMatrix4& o_mResult ) const
{
	o_mResult[0][0] = m_fEntry[0][0];	
	o_mResult[0][1] = m_fEntry[1][0];	
	o_mResult[0][2] = m_fEntry[2][0];
	o_mResult[0][3] = m_fEntry[3][0];
	
	o_mResult[1][0] = m_fEntry[0][1];	
	o_mResult[1][1] = m_fEntry[1][1];	
	o_mResult[1][2] = m_fEntry[2][1];
	o_mResult[1][3] = m_fEntry[3][1];
	
	o_mResult[2][0] = m_fEntry[0][2];	
	o_mResult[2][1] = m_fEntry[1][2];	
	o_mResult[2][2] = m_fEntry[2][2];
	o_mResult[2][3] = m_fEntry[3][2];

	o_mResult[3][0] = m_fEntry[0][3];	
	o_mResult[3][1] = m_fEntry[1][3];	
	o_mResult[3][2] = m_fEntry[2][3];
	o_mResult[3][3] = m_fEntry[3][3];
}

inline CMatrix3& CMatrix3::operator = ( const CMatrix4& mMatrix4 )
{
	m_fEntry[0][0] = mMatrix4[0][0];	m_fEntry[0][1] = mMatrix4[0][1];
	m_fEntry[1][0] = mMatrix4[1][0];	m_fEntry[1][1] = mMatrix4[1][1];
	m_fEntry[2][0] = mMatrix4[2][0];	m_fEntry[2][1] = mMatrix4[2][1];
	return *this;
}

inline CMatrix4::operator CMatrix3() const
{
	return CMatrix3(	m_fEntry[0][0],	m_fEntry[0][1],	m_fEntry[0][2],
						m_fEntry[1][0],	m_fEntry[1][1],	m_fEntry[1][2],
						m_fEntry[2][0],	m_fEntry[2][1],	m_fEntry[2][2]);
}

}