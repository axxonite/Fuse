// Matrix4.cpp

#include "stdafx.h"
#include "Matrix4.h"

CMatrix4::CMatrix4 ( eMatrixTypes Type, ...)
{
    switch ( Type )
    {
		case eMATRIX_ZERO:
		{
			// CMatrix4( eMATRIX_ZERO );
			MakeZero();
		}
		break;
		case eMATRIX_IDENTITY :
		{
			// CMatrix4( eMATRIX_IDENTITY );
			MakeIdentity();
		}
		break;
		case eMATRIX_DIAGONAL :
		{
			// CMatrix4( eMATRIX_DIAGONAL, f0, f1, f2 );
			va_list ap;
			va_start( ap, Type );
			float fDiag0 = float( va_arg( ap,double ) );
			float fDiag1 = float( va_arg( ap,double ) );
			float fDiag2 = float( va_arg( ap,double ) );
			va_end( ap );

			MakeDiagonal( fDiag0, fDiag1, fDiag2 );
		}
		break;
		case eMATRIX_XROTATION :
		{
			// CMatrix4( eMATRIX_XROTATION, fAngle );
			va_list ap;
			va_start( ap, Type );
			float fAngle = float( va_arg( ap, double ) );
			va_end( ap );

			MakeXRotation( fAngle );
		}
		break;
		case eMATRIX_YROTATION :
		{
			// CMatrix4( eMATRIX_YROTATION, fAngle );
			va_list ap;
			va_start( ap, Type );
			float fAngle = float( va_arg( ap, double ) );
			va_end( ap );

			MakeYRotation( fAngle );
		}
		break;
		case eMATRIX_ZROTATION :
		{
			// CMatrix4( eMATRIX_ZROTATION, fAngle );
			va_list ap;
			va_start( ap, Type );
			float fAngle = float( va_arg( ap, double ) );
			va_end( ap );

			MakeZRotation( fAngle );
		}
		break;
		case eMATRIX_AXISROTATION :
		{
			// CMatrix4( eMATRIX_AXISROTATION, fAngle, fX, fY, fZ);
			va_list ap;
			va_start( ap, Type );
			float fAngle		= float( va_arg( ap, double ) );
			float fX			= float( va_arg( ap, double ) );
			float fY			= float( va_arg( ap, double ) );
			float fZ			= float( va_arg( ap, double ) );
			va_end( ap );

			MakeRotation( fAngle, fX, fY, fZ );
		}
		break;
		case eMATRIX_TRANSLATION :
		{
			va_list ap;
			va_start( ap, Type );
			float fTranslationX = float( va_arg( ap,double ) );
			float fTranslationY = float( va_arg( ap,double ) );
			float fTranslationZ = float( va_arg( ap,double ) );
			va_end( ap );

			MakeTranslation( CVector( fTranslationX, fTranslationY, fTranslationZ ) );
			break;
		}
		default:
		{
			MakeZero();
		}
		break;
	};
}

void CMatrix4::ExtractAngleAndAxis( float& o_fAngle, float& o_fX, float& o_fY, float& o_fZ ) const
{
	// : NOTE : Translation does not affect this

    // Let (o_fX,o_fY,o_fZ) be the unit-fLength axis and let a be an fAngle of rotation.
    // The rotation matrix is r = I + sin( a ) * fP + ( 1 - cos( a ) ) * fP ^ 2 where
    // I is the identity and
    //
    //       +-        -+
    //  fP = |  0 +o_fZ -o_fY |
    //       | -o_fZ  0 +o_fX |
    //       | +o_fY -o_fX  0 |
    //       +-        -+
    //
    // Some algebra will show that
    //
    //   cos( a ) = ( fTrace( r ) - 1 ) / 2  and  r - r ^ t = 2 * sin( a ) * fP
    float fTrace = m_fEntry[0][0] + m_fEntry[1][1] + m_fEntry[2][2];

	// :PS2:
//#ifndef _PS2
    o_fAngle = (float)acosf( 0.5f * ( fTrace - 1.0f ) );
//#endif

    //float fTrace = m_fEntry[0][0] + m_fEntry[1][1] + m_fEntry[2][2];
    //o_fAngle = (float)cos( 0.5f * ( fTrace - 1.0f ) );

    o_fX = m_fEntry[1][2] - m_fEntry[2][1];
    o_fY = m_fEntry[2][0] - m_fEntry[0][2];
    o_fZ = m_fEntry[0][1] - m_fEntry[1][0];

	// :PS2:
//#ifndef _PS2
    float fLength = (float)sqrt( o_fX * o_fX + o_fY * o_fY + o_fZ * o_fZ );
//#else
//	float fLength;
//#endif

    if ( fLength > EPSILON )
    {
        float fInvLength = 1.0f / fLength;
        o_fX *= fInvLength;
        o_fY *= fInvLength;
        o_fZ *= fInvLength;
    }
    else  // o_fAngle is 0 or pi
    {
        if ( o_fAngle > 1.0f )		// any number strictly between 0 and pi works
        {
            // o_fAngle must be pi
            o_fX = (float)sqrt( 0.5f * ( 1.0f + m_fEntry[0][0] ) );
            o_fY = (float)sqrt( 0.5f * ( 1.0f + m_fEntry[1][1] ) );
            o_fZ = (float)sqrt( 0.5f * ( 1.0f + m_fEntry[2][2] ) );

            // determine signs of axis components
            float fTx, fTy, fTz;
            fTx = m_fEntry[0][0] * o_fX + m_fEntry[0][1] * o_fY+ m_fEntry[0][2] * o_fZ - o_fX;
            fTy = m_fEntry[1][0] * o_fX + m_fEntry[1][1] * o_fY+ m_fEntry[1][2] * o_fZ - o_fY;
            fTz = m_fEntry[2][0] * o_fX + m_fEntry[2][1] * o_fY+ m_fEntry[2][2] * o_fZ - o_fZ;
            fLength = fTx * fTx+ fTy * fTy + fTz * fTz;
            if ( fLength < EPSILON ) 
				return;

            o_fY = -o_fY;
            fTx = m_fEntry[0][0] * o_fX + m_fEntry[0][1] * o_fY + m_fEntry[0][2] * o_fZ - o_fX;
            fTy = m_fEntry[1][0] * o_fX + m_fEntry[1][1] * o_fY + m_fEntry[1][2] * o_fZ - o_fY;
            fTz = m_fEntry[2][0] * o_fX + m_fEntry[2][1] * o_fY + m_fEntry[2][2] * o_fZ - o_fZ;
            fLength = fTx * fTx + fTy * fTy + fTz * fTz;
            if ( fLength < EPSILON ) 
				return;

            o_fZ = -o_fZ;
            fTx = m_fEntry[0][0] * o_fX + m_fEntry[0][1] * o_fY + m_fEntry[0][2] * o_fZ - o_fX;
            fTy = m_fEntry[1][0] * o_fX + m_fEntry[1][1] * o_fY + m_fEntry[1][2] * o_fZ - o_fY;
            fTz = m_fEntry[2][0] * o_fX + m_fEntry[2][1] * o_fY + m_fEntry[2][2] * o_fZ - o_fZ;
            fLength = fTx * fTx + fTy * fTy + fTz * fTz;
            if ( fLength < EPSILON ) 
				return;

            o_fY = -o_fY;
            fTx = m_fEntry[0][0] * o_fX + m_fEntry[0][1] * o_fY + m_fEntry[0][2] * o_fZ - o_fX;
            fTy = m_fEntry[1][0] * o_fX + m_fEntry[1][1] * o_fY + m_fEntry[1][2] * o_fZ - o_fY;
            fTz = m_fEntry[2][0] * o_fX + m_fEntry[2][1] * o_fY + m_fEntry[2][2] * o_fZ - o_fZ;
            fLength = fTx * fTx + fTy * fTy + fTz * fTz;
            if ( fLength < EPSILON ) 
				return;
        }
        else
        {
            // o_fAngle is zero, matrix is the identity, no unique axis, so
            // return (1,0,0) for as good a guess as any.
            o_fX = 1.0;
            o_fY = 0.0;
            o_fZ = 0.0;
        }
    }
}

void CMatrix4::Inverse( CMatrix4& o_mResult ) const
{
	// The rotational part of the matrix is simply the transpose of the  original matrix
	o_mResult[0][0] = m_fEntry[0][0];
	o_mResult[1][0] = m_fEntry[0][1];
	o_mResult[2][0] = m_fEntry[0][2];
	o_mResult[0][1] = m_fEntry[1][0];
	o_mResult[1][1] = m_fEntry[1][1];
	o_mResult[2][1] = m_fEntry[1][2];
	o_mResult[0][2] = m_fEntry[2][0];
	o_mResult[1][2] = m_fEntry[2][1];
	o_mResult[2][2] = m_fEntry[2][2];
	o_mResult[0][3] = o_mResult[1][3] = o_mResult[2][3] = 0;
	o_mResult[3][3] = 1;

	// The translation components of the original matrix.
	float fX = m_fEntry[3][0];
	float fY = m_fEntry[3][1];
	float fZ = m_fEntry[3][2];
  
	// Result = -(Tm * Rm) to get the translation part of the inverse
	o_mResult[3][0] = -( m_fEntry[0][0] * fX + m_fEntry[0][1] * fY + m_fEntry[0][2] * fZ );
	o_mResult[3][1] = -( m_fEntry[1][0] * fX + m_fEntry[1][1] * fY + m_fEntry[1][2] * fZ );
	o_mResult[3][2] = -( m_fEntry[2][0] * fX + m_fEntry[2][1] * fY + m_fEntry[2][2] * fZ );
}

CMatrix4 CMatrix4::Inverse () const
{
    CMatrix4 o_mResult;
	Inverse( o_mResult );
	return o_mResult;
}

void CMatrix4::MakeFromPRS( const CVector& vPos, const CMatrix3& mRot, const CVector& vScale )
{
	m_fEntry[0][0] = mRot[0][0] * vScale.fX;	
	m_fEntry[0][1] = mRot[0][1] * vScale.fX;	
	m_fEntry[0][2] = mRot[0][2] * vScale.fX;
	m_fEntry[1][0] = mRot[1][0] * vScale.fY;	
	m_fEntry[1][1] = mRot[1][1] * vScale.fY;	
	m_fEntry[1][2] = mRot[1][2] * vScale.fY;
	m_fEntry[2][0] = mRot[2][0] * vScale.fZ;	
	m_fEntry[2][1] = mRot[2][1] * vScale.fZ;	
	m_fEntry[2][2] = mRot[2][2] * vScale.fZ;
	m_fEntry[3][0] = vPos.fX;	
	m_fEntry[3][1] = vPos.fY;		
	m_fEntry[3][2] = vPos.fZ;
	m_fEntry[0][3] = m_fEntry[1][3] = m_fEntry[2][3] = 0.0f;
	m_fEntry[3][3] = 1.0f;
}