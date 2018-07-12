// Matrix3.cpp

#include "stdafx.h"
#include "Matrix3.h"

CMatrix3::CMatrix3 ( eMatrixTypes Type, ...)
{
    switch ( Type )
    {
    case eMATRIX_ZERO:
    {
        // CMatrix3( eMATRIX_ZERO );
        MakeZero();
        break;
    }
    case eMATRIX_IDENTITY :
    {
        // CMatrix3( eMATRIX_IDENTITY );
        MakeIdentity();
        break;
    }
    case eMATRIX_DIAGONAL :
    {
        // CMatrix3( eMATRIX_DIAGONAL, f0, f1, f2 );
        va_list ap;
        va_start( ap, Type );
        float fDiag0 = float( va_arg( ap,double ) );
        float fDiag1 = float( va_arg( ap,double ) );
        float fDiag2 = float( va_arg( ap,double ) );
        va_end( ap );

        MakeDiagonal( fDiag0, fDiag1, fDiag2 );
        break;
    }
    case eMATRIX_XROTATION :
    {
        // CMatrix3( eMATRIX_XROTATION, fAngle );
        va_list ap;
        va_start( ap, Type );
        float fAngle = float( va_arg( ap, double ) );
        va_end( ap );

        MakeXRotation( fAngle );
        break;
    }
    case eMATRIX_YROTATION :
    {
        // CMatrix3( eMATRIX_YROTATION, fAngle );
        va_list ap;
        va_start( ap, Type );
        float fAngle = float( va_arg( ap, double ) );
        va_end( ap );

        MakeYRotation( fAngle );
        break;
    }
    case eMATRIX_ZROTATION :
    {
        // CMatrix3( eMATRIX_ZROTATION, fAngle );
        va_list ap;
        va_start( ap, Type );
        float fAngle = float( va_arg( ap, double ) );
        va_end( ap );

        MakeZRotation( fAngle );
        break;
    }
    case eMATRIX_AXISROTATION :
    {
        // CMatrix3( eMATRIX_AXISROTATION, fAngle, fX, fY, fZ);
        va_list ap;
        va_start( ap, Type );
        float fAngle		= float( va_arg( ap, double ) );
        float fX			= float( va_arg( ap, double ) );
        float fY			= float( va_arg( ap, double ) );
        float fZ			= float( va_arg( ap, double ) );
        va_end( ap );

        MakeRotation( fAngle, fX, fY, fZ );
        break;
    }
    default:
        MakeZero();
        break;
    };
}

void CMatrix3::ExtractAngleAndAxis( float& o_fAngle, float& o_fX, float& o_fY, float& o_fZ ) const
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
    float fLength = (float)sqrt( o_fX * o_fX + o_fY * o_fY + o_fZ * o_fZ );
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

void CMatrix3::Inverse( CMatrix3& o_mResult ) const
{
	//Transpose( o_mResult );
    // Invert a 3x3 using cofactors.  This is about 8 times faster than
    // the Numerical Recipes code which uses Gaussian elimination.
	
	// :NOTE: If the matrix is a pure rotation, you should use a transpose instead.
    o_mResult[0][0] = m_fEntry[1][1] * m_fEntry[2][2] - m_fEntry[1][2] * m_fEntry[2][1];
    o_mResult[0][1] = m_fEntry[0][2] * m_fEntry[2][1] - m_fEntry[0][1] * m_fEntry[2][2];
    o_mResult[0][2] = m_fEntry[0][1] * m_fEntry[1][2] - m_fEntry[0][2] * m_fEntry[1][1];
    o_mResult[1][0] = m_fEntry[1][2] * m_fEntry[2][0] - m_fEntry[1][0] * m_fEntry[2][2];
    o_mResult[1][1] = m_fEntry[0][0] * m_fEntry[2][2] - m_fEntry[0][2] * m_fEntry[2][0];
    o_mResult[1][2] = m_fEntry[0][2] * m_fEntry[1][0] - m_fEntry[0][0] * m_fEntry[1][2];
    o_mResult[2][0] = m_fEntry[1][0] * m_fEntry[2][1] - m_fEntry[1][1] * m_fEntry[2][0];
    o_mResult[2][1] = m_fEntry[0][1] * m_fEntry[2][0] - m_fEntry[0][0] * m_fEntry[2][1];
    o_mResult[2][2] = m_fEntry[0][0] * m_fEntry[1][1] - m_fEntry[0][1] * m_fEntry[1][0];

    float fDet =	m_fEntry[0][0] * o_mResult[0][0] +
					m_fEntry[0][1] * o_mResult[1][0] +
					m_fEntry[0][2] * o_mResult[2][0];
 //   if ( fabs( fDet ) <= EPSILON ) 
//		return false;

    float fInvDet = 1.0f / fDet;
    for ( u32 uRow = 0; uRow < 3; uRow++)
    {
        for ( u32 uCol = 0; uCol < 3; uCol++)
		{
			o_mResult[uRow][uCol] *= fInvDet;
		}
    }
}

CMatrix3 CMatrix3::Inverse () const
{
    CMatrix3 o_mResult;
	Inverse( o_mResult );
	return o_mResult;
}

void CMatrix3::LookAt( const CVector& vFrom, const CVector& vAt )
{
	CVector vDir = ( vAt - vFrom );
	vDir.Normalize();
	SetDirection( vDir );
}

CMatrix3::operator CMatrix4()	const
{
	return CMatrix4(
		m_fEntry[0][0],	m_fEntry[0][1],	m_fEntry[0][2],	0.0f,
		m_fEntry[1][0],	m_fEntry[1][1],	m_fEntry[1][2],	0.0f,
		m_fEntry[2][0],	m_fEntry[2][1],	m_fEntry[2][2],	0.0f,
		0.0f,			0.0f,			0.0f,			1.0f );
}