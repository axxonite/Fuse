// DXInputSystem.cpp

#include "stdafx.h"
#include "DXInputSystem.h"
#include "DXRenderer.h"

CDXInputSystem::CDXInputDevicePlatformData::CDXInputDevicePlatformData() :
	pIDevice( NULL )
{
}

CDXInputSystem::CDXInputDevicePlatformData::~CDXInputDevicePlatformData()
{
	if ( pIDevice )
		pIDevice->Release();
}

CDXInputSystem::CDXInputSystem() :
	m_pIDirectInput		( NULL	),
	m_bMouseExclusive	( false ),
	m_bCursorShown		( true	),
	m_hWnd				( 0		)
{
}

CDXInputSystem::~CDXInputSystem()
{
	if ( m_pIDirectInput )
		m_pIDirectInput->Release();
}

CKeyboard* CDXInputSystem::GetKeyboard()
{
	return &m_Keyboard;
}

CMouse* CDXInputSystem::GetMouse()
{
	return &m_Mouse;
}

void CDXInputSystem::Init( HWND hWnd, bool bMouseExclusive )
{
	CHECK_DIRECTX( DirectInput8Create( GetModuleHandleA(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pIDirectInput, NULL ) );

	m_hWnd = hWnd;
	InitKeyboard( hWnd );
	InitMouse( hWnd, bMouseExclusive );
}

void CDXInputSystem::InitKeyboard( HWND hWnd )
{
	CDXInputDevicePlatformData* pPlatformData = new CDXInputDevicePlatformData();
	CHECK_DIRECTX( m_pIDirectInput->CreateDevice( GUID_SysKeyboard, &pPlatformData->pIDevice, NULL ) );
	CHECK_DIRECTX( pPlatformData->pIDevice->SetDataFormat( &c_dfDIKeyboard ) );
	CHECK_DIRECTX( pPlatformData->pIDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = PROP_BUFFER_SIZE;
    CHECK_DIRECTX( pPlatformData->pIDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) );

	pPlatformData->pIDevice->Acquire();
	m_Keyboard.SetPlatformData( pPlatformData );
}

void CDXInputSystem::InitMouse( HWND hWnd, bool bMouseExclusive )
{
	CDXInputDevicePlatformData* pPlatformData = new CDXInputDevicePlatformData();
	CHECK_DIRECTX( m_pIDirectInput->CreateDevice( GUID_SysMouse, &pPlatformData->pIDevice, NULL ) );
	CHECK_DIRECTX( pPlatformData->pIDevice->SetDataFormat( &c_dfDIMouse2 ) );
	
	u32 uCooperativeFlags = DISCL_FOREGROUND | ( bMouseExclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE );
	CHECK_DIRECTX( pPlatformData->pIDevice->SetCooperativeLevel( hWnd, uCooperativeFlags ) );

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = PROP_BUFFER_SIZE;

    CHECK_DIRECTX( pPlatformData->pIDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) );
	pPlatformData->pIDevice->Acquire();
	m_Mouse.SetPlatformData( pPlatformData );
}

void CDXInputSystem::ShowCursor( bool bShow )
{
	if ( bShow == m_bCursorShown )
		return;
	if( bShow )
	{
		ReleaseCapture();
		::ShowCursor( true );
	}
	else
	{
		SetCapture( m_hWnd );
		::ShowCursor( false );
	}
	m_bCursorShown = bShow;
}

void CDXInputSystem::UpdateKeyboard( u8* pPressedKeys, u8* pToggledKeys )
{
	CDXInputDevicePlatformData* pPlatformData = static_cast<CDXInputDevicePlatformData*>(m_Keyboard.GetPlatformData());
	if( !pPlatformData->pIDevice )
		return;
    
	ZeroMemory( pToggledKeys, CKeyboard::MAX_KEYBOARD_KEYS );
	u32 uElements;
	u32 i;
	for ( i = 0; i < 3; i++ )		// Try 3 times
	{
	    uElements = PROP_BUFFER_SIZE;
		HRESULT hResult = pPlatformData->pIDevice->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), pPlatformData->DIDOD, (DWORD*)&uElements, 0 );
		if( (hResult == DIERR_NOTACQUIRED) || (hResult == DIERR_INPUTLOST) )
		{
			ZeroMemory( pPressedKeys, CKeyboard::MAX_KEYBOARD_KEYS );
			ZeroMemory( pToggledKeys, CKeyboard::MAX_KEYBOARD_KEYS );
			pPlatformData->pIDevice->Acquire();
		}
		else if( hResult == DI_BUFFEROVERFLOW )
		{
			ZeroMemory( pPressedKeys, CKeyboard::MAX_KEYBOARD_KEYS );
			ZeroMemory( pToggledKeys, CKeyboard::MAX_KEYBOARD_KEYS );
			return;
		}
		else if( hResult == DI_OK )
			break;
	};

	if( i == 3 )
		return;

	for ( u32 i = 0; i < uElements; i++ )
	{
		u32 uKey = pPlatformData->DIDOD[ i ].dwOfs;
		if( pPlatformData->DIDOD[ i ].dwData & 0x80 )
		{
			pPressedKeys[ uKey ] = 1;
			pToggledKeys[ uKey ] = 1;
		}
		else pPressedKeys[ uKey ] = 0;
	}
}

void CDXInputSystem::UpdateMouse( u8* pPressedButtons, u8* pToggledButtons, CVector& MousePosition, CVector& MouseDisplacement )
{
	CDXInputDevicePlatformData* pPlatformData = static_cast<CDXInputDevicePlatformData*>(m_Mouse.GetPlatformData());
	if( !pPlatformData->pIDevice )
		return;

	u32 uElements;
	u32 i;
	for ( i = 0; i < 3; i++ )		// Try 3 times
	{
		uElements = PROP_BUFFER_SIZE;
		HRESULT hResult = pPlatformData->pIDevice->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), pPlatformData->DIDOD, (DWORD*)&uElements, 0 );

		if( ( hResult == DIERR_NOTACQUIRED) || (hResult == DIERR_INPUTLOST ) )
		{
			ZeroMemory( pPressedButtons, CMouse::MAX_MOUSE_KEYS );
			ZeroMemory( pToggledButtons, CMouse::MAX_MOUSE_KEYS );
			pPlatformData->pIDevice->Acquire();
		}
		else if( hResult == DI_BUFFEROVERFLOW )
		{
			ZeroMemory( pPressedButtons, CMouse::MAX_MOUSE_KEYS );
			ZeroMemory( pToggledButtons, CMouse::MAX_MOUSE_KEYS );
			return;
		}
		else if( hResult == DI_OK )
			break;
	}
	if( i == 3 )
		return;

	MouseDisplacement.Set( 0, 0, 0 );
	ZeroMemory( pToggledButtons, CMouse::MAX_MOUSE_KEYS );

	for( u32 i = 0; i < uElements; i++ )
	{
		switch( pPlatformData->DIDOD[ i ].dwOfs )
		{
			case DIMOFS_BUTTON0:
			case DIMOFS_BUTTON1:
			case DIMOFS_BUTTON2:
			case DIMOFS_BUTTON3:
			case DIMOFS_BUTTON4:
			case DIMOFS_BUTTON5:
			case DIMOFS_BUTTON6:
			case DIMOFS_BUTTON7:
				{
					u32 uButton = pPlatformData->DIDOD[ i ].dwOfs - DIMOFS_BUTTON0;
					if( pPlatformData->DIDOD[ i ].dwData & 0x80 )
					{
						pPressedButtons[ uButton ] = 1;
						pToggledButtons[ uButton ] = 1;
					}
					else
					{
						pPressedButtons[ uButton ] = 0;
						pToggledButtons[ uButton ] = 1;
					}
				}
				break;
			case DIMOFS_X:
				MouseDisplacement.fX += (LONG)pPlatformData->DIDOD[ i ].dwData;
				break;
			case DIMOFS_Y:
				MouseDisplacement.fY += (LONG)pPlatformData->DIDOD[ i ].dwData;
				break;
			case DIMOFS_Z:
				MouseDisplacement.fZ += (LONG)pPlatformData->DIDOD[ i ].dwData;
				break;
			default:
				assert( false );
				break;
		}
	}

	POINT Point;
	GetCursorPos( &Point );
	ScreenToClient( g_pDXRenderer->GetWindowHandle(), &Point );
	MousePosition.fX = (float)Point.x;
	MousePosition.fY = (float)Point.y;
}

void CDXInputSystem::Update()
{
	m_Keyboard.Update();
	m_Mouse.Update();
}