// NavControls.cpp

#include "stdafx.h"
#include "NavControls.h"
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "HALInputSystem.h"
#include "Resource.h"
#include "Editor.h"

CNavControls::CNavControls() :
	m_fRotateSpeedX	( ANGLETORADIAN_FACTOR * 0.4f			),
	m_fRotateSpeedY	( ANGLETORADIAN_FACTOR * 0.6f			),
	m_fPitch		( ANGLETORADIAN_FACTOR * 45.0f * 0.5f	),
	m_Mode			( eNONE									),
	m_bInvertYaw	( false									),
	m_vTotalDisplacement	( 0, 0, 0						),
	m_ConstraintRefPoint	( 0, 0, 0						),
	m_bHasMoved		( false									),
	m_ConstraintMode( eCONSTRAINT_NONE						),
	m_pCamera		( NULL									),
	m_fZoomSpeed	( 0.005f								),
	m_fWheelZoomSpeed	( 0.1f								)
{
	CMatrix3 mRotation;
	m_mPitch.MakeXRotation( m_fPitch );
	CMatrix3 mRotation2;
	m_mYaw.MakeYRotation( ANGLETORADIAN_FACTOR * 45.0f );
}

CNavControls::~CNavControls()
{
}

void CNavControls::ApplyZoom( float fDelta, bool bUpdateStatus )
{
	float fZoom = fDelta * fabs( m_pCamera->GetPosition().fZ ) * m_fZoomSpeed;
	//if ( m_pCamera->GetPosition().fZ > 0 )
	//	fZoom = fZoom * 3;
	if ( fDelta != 0 )
	{
		if ( fabs( fZoom ) < 0.025f )
			fZoom = fDelta < 0 ? -0.025f : 0.025f;
		m_bHasMoved = true;
		m_pCamera->SetPosition( m_pCamera->GetPosition() + CVector( 0, 0, fZoom ) );
	}
	if ( bUpdateStatus )
	{
		HCURSOR hCursor = LoadCursor( g_pEditor->GetInstance()->GetHInstance(), MAKEINTRESOURCE( IDC_ZOOM ) );
		SetCursor( hCursor );
		if ( m_bHasMoved )
		{
			char szBuffer[DEFAULT_BUFFER_LENGTH];
			sprintf( szBuffer, "Distance : %.3f", -m_pCamera->GetPosition().fZ );
			m_Status = szBuffer;
		}
		else m_Status = "Dolly Tool: Use mouse to dolly.";
	}
}

void CNavControls::AdjustPivot()
{
	if ( m_pCamera->GetPosition().fZ > 0.0f )
	{
		m_PivotNode.SetPosition( m_PivotNode.GetPosition() + CVector( 0, 0, m_pCamera->GetPosition().fZ ) * m_PivotNode.GetRotation() );
		m_pCamera->SetPosition( 0, 0, 0 );
	}
}

void CNavControls::Init( CCamera* pCamera, CNode* pRootNode )
{
	pRootNode->AttachChild( &m_PivotNode );
	m_PivotNode.AttachChild( pCamera );
	m_PivotNode.SetRotation( m_mPitch * m_mYaw );
	m_pCamera = pCamera;
	GetCursorPos( &m_LastMousePos );
}

void CNavControls::Tick()
{
	CMouse*		pMouse			= g_pInputSystem->GetMouse();
	CKeyboard*	pKeyboard		= g_pInputSystem->GetKeyboard();
	POINT Point;
	GetCursorPos( &Point );
	CVector		vDisplacement;
	vDisplacement.fX			= (float)( Point.x - m_LastMousePos.x );
	vDisplacement.fY			= (float)( Point.y - m_LastMousePos.y );
	vDisplacement.fZ			= pMouse->GetMouseDisplacement().fZ;
	m_LastMousePos				= Point;
	//Trace( "%f, %f\n", vDisplacement.fX, vDisplacement.fY );

	if ( ( pKeyboard->IsKeyPressed( DIK_LMENU ) || pKeyboard->IsKeyPressed( DIK_RMENU ) ) && m_Mode == eNONE )
	{
		if ( pMouse->IsButtonClicked( 0 ) )
		{
			m_Mode = eORBIT;
			m_bInvertYaw = fabs( m_fPitch ) > HALF_PI;
		}
		if ( pMouse->IsButtonClicked( 1 ) )
			m_Mode = eZOOM;
		if ( pMouse->IsButtonClicked( 2 ) )
		{
			m_Mode = ePAN;
		}
		if ( pMouse->IsButtonClicked( 0 ) || pMouse->IsButtonClicked( 1 ) || pMouse->IsButtonClicked( 2 ) )
		{
			m_vTotalDisplacement.Set( 0, 0, 0 );
			SetCapture( g_pEditor->GetMainWnd()->m_hWnd );
			m_bHasMoved = false;
		}
	}
	if (	( m_Mode == eORBIT && !pMouse->IsButtonPressed( 0 ) ) ||
			( m_Mode == eZOOM && !pMouse->IsButtonPressed( 1 ) ) ||
			( m_Mode == ePAN && !pMouse->IsButtonPressed( 2 ) ) )
	{
		AdjustPivot();
		m_Mode = eNONE;
		m_ConstraintMode = eCONSTRAINT_NONE;
		SetCursor( LoadCursor(NULL, IDC_ARROW ) );
		ReleaseCapture();
	}

	if ( m_Mode != eNONE )
	{
		m_bHasMoved |= m_vTotalDisplacement.fX != 0 || m_vTotalDisplacement.fY != 0;

		if ( pKeyboard->IsKeyPressed( DIK_LSHIFT ) || pKeyboard->IsKeyPressed( DIK_RSHIFT ) )
		{
			if ( m_ConstraintMode == eCONSTRAINT_NONE )
			{
				m_ConstraintMode = eCONSTRAINT_UNKNOWN;
				m_ConstraintRefPoint = pMouse->GetMousePosition();
			}
		}
		else
			m_ConstraintMode = eCONSTRAINT_NONE;

		if ( m_ConstraintMode == eCONSTRAINT_UNKNOWN )
		{
			float fDistX = fabs( pMouse->GetMousePosition().fX - m_ConstraintRefPoint.fX );
			float fDistY = fabs( pMouse->GetMousePosition().fY - m_ConstraintRefPoint.fY );

			if ( fDistX > 0 && fDistX >= fDistY )
				m_ConstraintMode = eCONSTRAINT_X;
			else if ( fDistX > 0 && fDistY > fDistX )
				m_ConstraintMode = eCONSTRAINT_Y;
		}
	}
	
	switch ( m_Mode )
	{
	case eNONE :
		{
			ApplyZoom( vDisplacement.fZ * m_fWheelZoomSpeed, false );
			AdjustPivot();
			if ( pKeyboard->IsKeyClicked( DIK_F ) )
				m_PivotNode.SetPosition( 0, 0, 0 );
		}
		break;
	case eORBIT :
		{
			if ( pMouse->IsButtonPressed( 2 ) )
			{
				ApplyZoom( vDisplacement.fX + vDisplacement.fY );
				break;
			}
			AdjustPivot();
			HCURSOR hCursor = LoadCursor( g_pEditor->GetInstance()->GetHInstance(), MAKEINTRESOURCE( IDC_ORBIT ) );
			SetCursor( hCursor );
			CMatrix3 mMatrix;
			if ( m_ConstraintMode != eCONSTRAINT_Y )
			{
				float fAngle = m_fRotateSpeedX * vDisplacement.fX;
				m_vTotalDisplacement.fX += fAngle * RADIANTOANGLE_FACTOR;
				mMatrix.MakeRotation( m_bInvertYaw ? -fAngle : fAngle, 0.0f, 1.0f, 0.0f );
				m_mYaw = m_mYaw * mMatrix;
			}
			if ( m_ConstraintMode != eCONSTRAINT_X )
			{
				mMatrix.MakeRotation( m_fRotateSpeedY * vDisplacement.fY, 1.0f, 0.0f, 0.0f );
				m_mPitch = m_mPitch * mMatrix;
				float fDisplacement = m_fRotateSpeedY * vDisplacement.fY;
				m_fPitch += fDisplacement;
				m_vTotalDisplacement.fY += fDisplacement * RADIANTOANGLE_FACTOR;
				if ( m_fPitch > PI ) m_fPitch -= 2 * PI;
				if ( m_fPitch < -PI ) m_fPitch += 2 * PI;
			}
			m_PivotNode.SetRotation( m_mPitch * m_mYaw );
			m_bHasMoved |= m_vTotalDisplacement.fX != 0 || m_vTotalDisplacement.fY != 0;

			if ( m_bHasMoved )
			{
				char szBuffer[DEFAULT_BUFFER_LENGTH];
				sprintf( szBuffer, "Rotation : %-30.2f %.2f", m_vTotalDisplacement.fX, m_vTotalDisplacement.fY );
				m_Status = szBuffer;
			}
			else m_Status = "Tumble Tool: Use LMB or MMB to tumble. Shift to constrain rotation.";
			break;
		}
	case ePAN :
		{
			if ( pMouse->IsButtonPressed( 0 ) )
			{
				ApplyZoom( vDisplacement.fX + vDisplacement.fY );
				break;
			}
			AdjustPivot();
			HCURSOR hCursor = LoadCursor( g_pEditor->GetInstance()->GetHInstance(), MAKEINTRESOURCE( IDC_PAN ) );
			SetCursor( hCursor );
			if ( m_ConstraintMode != eCONSTRAINT_Y )
			{
				float fDeltaX = ( vDisplacement.fX / g_pRenderer->GetFrameBufferWidth() ) * 
								tan( m_pCamera->GetViewAngle() * 0.5f ) * -m_pCamera->GetPosition().fZ * 2.0f;
				m_PivotNode.SetPosition( m_PivotNode.GetPosition() + -m_PivotNode.GetRotation().GetRight() * fDeltaX );
				m_vTotalDisplacement.fX += fDeltaX;
			}
			if ( m_ConstraintMode != eCONSTRAINT_X )
			{
				float fDeltaY = ( vDisplacement.fY / g_pRenderer->GetFrameBufferHeight() ) * 
								tan( m_pCamera->GetViewAngle() * 0.5f ) * 2.0f *
								( (float)g_pRenderer->GetFrameBufferHeight() / (float)g_pRenderer->GetFrameBufferWidth() )* 
								-m_pCamera->GetPosition().fZ;
				m_PivotNode.SetPosition( m_PivotNode.GetPosition() + m_PivotNode.GetRotation().GetUp() * fDeltaY );
				m_vTotalDisplacement.fY += fDeltaY;
			}
			m_bHasMoved |= m_vTotalDisplacement.fX != 0 || m_vTotalDisplacement.fY != 0;
			if ( m_bHasMoved )
			{
				char szBuffer[DEFAULT_BUFFER_LENGTH];
				sprintf( szBuffer, "Translation : %-30.2f %.2f", m_vTotalDisplacement.fX, m_vTotalDisplacement.fY );
				m_Status = szBuffer;
			}
			else m_Status = "Track Tool: Use LMB or MMB to track. Shift to constrain rotation.";
			break;
		}
	case eZOOM :
		{
			ApplyZoom( vDisplacement.fX + vDisplacement.fY );
			break;
		}
	}
}