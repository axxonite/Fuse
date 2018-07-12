// Quaternion.cpp

#include "stdafx.h"
#include "Quaternion.h"

CQuaternion::CQuaternion()
{
}

CQuaternion::CQuaternion( float fQw, float fQx, float fQy, float fQz )
{
	fW = fQw;	fX = fQx;	fY = fQy;	fZ = fQz;
}

CQuaternion::CQuaternion( const CQuaternion& qQuat )
{
	fW = qQuat.fW;
	fX = qQuat.fX;
	fY = qQuat.fY;
	fZ = qQuat.fZ;
}

CQuaternion& CQuaternion::operator = ( const CQuaternion& qQuat )
{
	fW = qQuat.fW;
	fX = qQuat.fX;
	fY = qQuat.fY;
	fZ = qQuat.fZ;
	return *this;
}

CQuaternion CQuaternion::operator * ( const CVector& vVector ) const
{
	//                  1 ( 0  -w^T )
	// implements nv:= ---(         ) self  , where w~ is the outer product mMatrix
	//                  2 ( w  -w~  )  
	//                    (         )
	return CQuaternion(
		-0.5f	* ( vVector.fX * fX + vVector.fY * fY + vVector.fZ * fZ ),
		0.5f	* ( vVector.fX * fW + vVector.fZ * fY - vVector.fY * fZ ),
		0.5f	* ( vVector.fY * fW - vVector.fZ * fX + vVector.fX * fZ ),
		0.5f	* ( vVector.fZ * fW + vVector.fY * fX - vVector.fX * fY ) );
}

void CQuaternion::GetAngleAxis( float& o_fAngle, CVector& o_vAxis ) const
{
	float fLength = (float) sqrt( fX * fX + fY * fY + fZ * fZ );
	if ( fLength < EPSILON )
	{
		o_fAngle = 0.0f;
		o_vAxis.fX = 0.0f;	o_vAxis.fY = 0.0f;	o_vAxis.fZ = 0.f;
	}
	else
	{
		o_fAngle = 2.0f * (float)acos( fW );
		float fInvLength = 1.0f / fLength;
		o_vAxis.fX = fX * fInvLength;
		o_vAxis.fY = fY * fInvLength;
		o_vAxis.fZ = fZ * fInvLength;
	}
}

void CQuaternion::GetMatrix( CMatrix3& mMatrix ) const
{
	float fTX	= 2.0f * fX;
	float fTY	= 2.0f * fY;
	float fTZ	= 2.0f * fZ;
	float fTWX	= fTX * fW;
	float fTWY	= fTY * fW;
	float fTWZ	= fTZ * fW;
	float fTXX	= fTX * fX;
	float fTXY	= fTY * fX;
	float fTXZ	= fTZ * fX;
	float fTYY	= fTY * fY;
	float fTYZ	= fTZ * fY;
	float fTZZ	= fTZ * fZ;

	mMatrix[0][0]	= 1.0f - ( fTYY + fTZZ );
	mMatrix[1][0]	= fTXY - fTWZ;
	mMatrix[2][0]	= fTXZ + fTWY;
	mMatrix[0][1]	= fTXY + fTWZ;
	mMatrix[1][1]	= 1.0f - ( fTXX + fTZZ );
	mMatrix[2][1]	= fTYZ - fTWX;
	mMatrix[0][2]	= fTXZ - fTWY;
	mMatrix[1][2]	= fTYZ + fTWX;
	mMatrix[2][2]	= 1.0f - ( fTXX + fTYY );
};

CQuaternion CQuaternion::Slerp( float fT, CQuaternion qQuat ) const
{
	float fCos = Dot( qQuat );
	//_ASSERT( fCos >= 0.0 );
	if ( fCos < 0 )
	{
		qQuat = -qQuat;
		fCos = -fCos;
	}

	//if ( fCos < -1.0f )
	//	fCos = -1.0f;
	//else 
	if ( fCos > 1.0f )
		fCos = 1.0f;
	
	float fAngle	= (float)acos( fCos );
	float fSin		= (float)sin( fAngle );

	if ( fSin < EPSILON ) return *this;
	else
	{
		float fInvSin = 1.0f / fSin;
		float fCoeff	= (float)sin( ( 1.0f - fT ) * fAngle ) * fInvSin;
		float fQCoeff	= (float)sin( fT * fAngle ) * fInvSin;
		return fCoeff * *this + fQCoeff * qQuat;
	}
}

float CQuaternion::Normalize()
{
	float fLength = fX * fX + fY * fY + fZ * fZ + fW * fW;
	float fRecipLength = 1.0f / fLength;
	if ( fRecipLength > EPSILON )
	{
		fX /= fRecipLength;
		fY /= fRecipLength;
		fZ /= fRecipLength;
		fW /= fRecipLength;
	}
	else
	{
		fX = fY = fZ = 0;
		fW = 1;
	}
	return fLength;
}

void CQuaternion::Squad(	float fT, const CQuaternion& qP, const CQuaternion& qA, const CQuaternion& qB, const CQuaternion& qQuat,
							CQuaternion& qResult )
{
	qResult = qP.Slerp( fT, qQuat ).Slerp( ( 2 * fT * ( 1 - fT ) ), qA.Slerp( fT, qB ) );
}

void CQuaternion::Set( float fAngle, const CVector& axis )
{
	float fHalfAngle = fAngle * 0.5f;
	float fSin = (float)sin( fHalfAngle );
	fW = (float)cos( fHalfAngle );
	
	fX = axis.fX * fSin;
	fY = axis.fY * fSin;
	fZ = axis.fZ * fSin;
}

void CQuaternion::Set( const CMatrix3& mMatrix )
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes article "Quaternion Calculus and 
	// Fast Animation".
	float fTrace = mMatrix[0][0] + mMatrix[1][1] + mMatrix[2][2];
	float fRoot = 0.f;

	if ( fTrace > 0.0f )
	{
		// |w| > 1/2, may as well choose 2 > 1/2

		fRoot = (float)sqrt( fTrace + 1.0f );					// 2w

		fW = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;							// 1 / ( 4w )
		fX = ( mMatrix[1][2] - mMatrix[2][1] ) * fRoot;
		fY = ( mMatrix[2][0] - mMatrix[0][2] ) * fRoot;
		fZ = ( mMatrix[0][1] - mMatrix[1][0] ) * fRoot;
	}
	else
	{
		// |w| <= 1/2
		static int next[3] = { 1, 2, 0 };
		int i = 0;
		if ( mMatrix[1][1] > mMatrix[0][0] ) i = 1;
		if ( mMatrix[2][2] > mMatrix[i][i] ) i = 2;
		int j = next[i];
		int k = next[j];

		fRoot = (float)sqrt( mMatrix[i][i] - mMatrix[j][j] - mMatrix[k][k] + 1.0f );

		float* qQuat[3] = { &fX, &fY, &fZ };
		*qQuat[i] = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		fW = ( mMatrix[j][k] - mMatrix[k][j] ) * fRoot;
		*qQuat[j] = ( mMatrix[i][j] + mMatrix[j][i] ) * fRoot;
		*qQuat[k] = ( mMatrix[i][k] + mMatrix[k][i] ) * fRoot;
	}
}
