// DXInputSystem.h

#pragma once

#include "HALInputSystem.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "HAL.h"

namespace Fuse
{

//
// DirectX implementation of input system.
//

class CDXInputSystem : public CHALInputSystem
{

public :

	static	const u32	PROP_BUFFER_SIZE	= 0x200;

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CDXInputSystem();
	// Destructor.
	virtual ~CDXInputSystem();

	//---------------------------------------------------------------------------------------------

	// Initializes the input system.
			void Init( HWND hWnd = NULL, bool bMouseExclusive = false );

	// Shows or hides cursor.
	virtual	void ShowCursor( bool bShow );

	// Polls input devices.
			void Update();

	//---------------------------------------------------------------------------------------------

	// Returns the keyboard object.
	virtual	CKeyboard*	GetKeyboard();
	// Returns the mouse object.
	virtual	CMouse*		GetMouse();

private :

	class CDXInputDevicePlatformData : public CHALData
	{

	public :

		CDXInputDevicePlatformData();
		virtual ~CDXInputDevicePlatformData();

		IDirectInputDevice8*	pIDevice;
		DIDEVICEOBJECTDATA		DIDOD[PROP_BUFFER_SIZE];

	};

	IDirectInput8*		m_pIDirectInput;

	CKeyboard			m_Keyboard;
	CMouse				m_Mouse;
	bool				m_bMouseExclusive;
	bool				m_bCursorShown;
	HWND				m_hWnd;

			void	InitKeyboard( HWND hWnd );

			void	InitMouse( HWND hWnd, bool bMouseExclusive );

	virtual	void	UpdateKeyboard( u8* pCurState, u8* pTriggers );

	virtual	void	UpdateMouse( u8* pPressedButtons, u8* pToggledButtons, CVector& MousePosition, CVector& MouseDisplacement );

};

}