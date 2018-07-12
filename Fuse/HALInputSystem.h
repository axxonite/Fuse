// BaseObject.h

#pragma once

#include "Singleton.h"

namespace Fuse
{

class CKeyboard;
class CMouse;

//
// Input system HAL interface.
//

#define g_pInputSystem	CHALInputSystem::GetInstance()

class CHALInputSystem : public CBaseObject, public CSingleton<CHALInputSystem>
{

public :

	friend class CKeyboard;
	friend class CMouse;

	//---------------------------------------------------------------------------------------------

	// Initializes the input system.
	virtual	void Init( HWND hWnd = NULL, bool bMouseExclusive = false ) = 0;

	// Shows or hides cursor.
	virtual	void ShowCursor( bool bShow ) = 0;

	// Polls input devices.
	virtual	void Update() = 0;

	//---------------------------------------------------------------------------------------------

	// Returns the keyboard object.
	virtual	CKeyboard*	GetKeyboard() = 0;
	// Returns the mouse object.
	virtual	CMouse*		GetMouse() = 0;

protected :

	virtual	void	UpdateKeyboard( u8* pCurState, u8* pTriggers ) = 0;

	virtual	void	UpdateMouse( u8* pPressedButtons, u8* pToggledButtons, CVector& MousePosition, CVector& MouseDisplacement ) = 0;

};

}