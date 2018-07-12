// Quaternion.h

#pragma once

namespace Fuse
{

//
// Quaternion class.
//

class CQuaternion
{

public :

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CQuaternion();
	// Initializes quaternion from specified components.
	CQuaternion( float fQw, float fQx, float fQy, float fQz );
	// Copy constructor.
	CQuaternion( const CQuaternion& qQuat );

	//---------------------------------------------------------------------------------------------

	// Assignment and equivalence operators.
	CQuaternion&	operator = ( const CQuaternion& qQuat );
	bool			operator == ( const CQuaternion& qQuat ) const;
	bool			operator != ( const CQuaternion& qQuat ) const;

	// Quaternion arithmetic.
	CQuaternion		operator + ( const CQuaternion& qQuat ) const;
	CQuaternion		operator - ( const CQuaternion& qQuat ) const;
	CQuaternion		operator - ();
	CQuaternion		operator * ( const CQuaternion &qQuat ) const;
	CQuaternion		operator * ( float fScalar ) const;
	CQuaternion		operator * ( const CVector& vVector ) const;

	// Dot product.
	float			Dot( const CQuaternion& qQuat ) const;

	// Extracts angle and axis.
	void			GetAngleAxis( float& o_fAngle, CVector& o_vAxis ) const;
	
	// Convertex to a matrix.
	void			GetMatrix( CMatrix3& o_mMatrix ) const;
	CMatrix3		GetMatrix() const;
	
	// Specifies the quaternion's components.
	void			Set( float fQx, float fQy, float fQz, float fQw );
	// Makes a quaternion from an axis of rotation and an angle.
	void			Set( float fAngle, const CVector& vAxis );
	// Convertes a matrix into a quaternion.
	void			Set( const CMatrix3& mMatrix );

	float			Normalize();

	// Spherical interpolation.
	CQuaternion	Slerp( float fT, CQuaternion qQuat ) const;
	
	static	void	Squad(	float fT, const CQuaternion& qP, const CQuaternion& qA, const CQuaternion& qB, const CQuaternion& qQ,
							CQuaternion& qResult);
	static	void	SquadExtraSpins(	float fT, const CQuaternion& qP, const CQuaternion& qA, const CQuaternion& qB, 
										const CQuaternion& qC, int iExtraSpins, CQuaternion& Result );

	//---------------------------------------------------------------------------------------------

	float fX;
	float fY;
	float fZ;
	float fW;

protected :

};

const CQuaternion QUATERNION_IDENTITY( 1, 1, 1, 1 );

inline bool CQuaternion::operator == ( const CQuaternion& qQuat ) const
{ 
	return	IS_FLOAT_EQUAL( fW, qQuat.fW ) && 
			IS_FLOAT_EQUAL( fX, qQuat.fX ) && 
			IS_FLOAT_EQUAL( fY, qQuat.fY ) &&
			IS_FLOAT_EQUAL( fZ, qQuat.fZ );	
};

inline bool CQuaternion::operator != ( const CQuaternion& qQuat ) const
{ 
	return	!(	IS_FLOAT_EQUAL( fW, qQuat.fW ) && 
				IS_FLOAT_EQUAL( fX, qQuat.fX ) && 
				IS_FLOAT_EQUAL( fY, qQuat.fY ) &&
				IS_FLOAT_EQUAL( fZ, qQuat.fZ ) );
};

inline CQuaternion CQuaternion::operator + ( const CQuaternion& qQuat ) const
{ 
	return CQuaternion( fW + qQuat.fW, fX + qQuat.fX, fY + qQuat.fY, fZ + qQuat.fZ );		
};

inline CQuaternion CQuaternion::operator - ( const CQuaternion& qQuat ) const
{ 
	return CQuaternion( fW - qQuat.fW, fX - qQuat.fX, fY - qQuat.fY, fZ - qQuat.fZ );	
};

inline CQuaternion CQuaternion::operator - ()			
{ 
	return CQuaternion( -fW, -fX, -fY, -fZ );	
};

inline CQuaternion operator * ( float fScalar, const CQuaternion& qQuat )
{
	return CQuaternion( fScalar * qQuat.fW, fScalar * qQuat.fX, fScalar * qQuat.fY, fScalar * qQuat.fZ );
}

inline CQuaternion CQuaternion::operator * ( const CQuaternion& qQuat ) const
{
	return CQuaternion(	fW * qQuat.fW - fX * qQuat.fX - fY * qQuat.fY - fZ * qQuat.fZ,
							fW * qQuat.fX + fX * qQuat.fW + fY * qQuat.fZ - fZ * qQuat.fY,
							fW * qQuat.fY + fY * qQuat.fW + fZ * qQuat.fX - fX * qQuat.fZ,
							fW * qQuat.fZ + fZ * qQuat.fW + fX * qQuat.fY - fY * qQuat.fX );
}

inline CQuaternion CQuaternion::operator * ( float fScalar ) const
{ 
	return CQuaternion( fScalar * fW, fScalar * fX, fScalar * fY, fScalar * fZ );	
}

inline float CQuaternion::Dot( const CQuaternion& qQuat ) const
{ 
	return fW * qQuat.fW + fX * qQuat.fX + fY * qQuat.fY + fZ * qQuat.fZ;		
};

inline CMatrix3 CQuaternion::GetMatrix() const
{
	CMatrix3 mResult;
	GetMatrix( mResult );
	return mResult;
}

inline void	CQuaternion::Set( float fQx, float fQy, float fQz, float fQw )
{
	fX = fQx;
	fY = fQy;
	fZ = fQz;
	fW = fQw;
}

}