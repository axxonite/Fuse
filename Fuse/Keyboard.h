// Keyboard.h

#pragma once

namespace Fuse
{

class CHALData;

//
// Keyboard input device.
//

class CKeyboard : public CBaseObject
{

public :

	static	const u32	MAX_KEYBOARD_KEYS	= 0xFF;

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CKeyboard();
	// Destructor.
	virtual ~CKeyboard();

	//---------------------------------------------------------------------------------------------

	// Polls the keyboard state and updates key information.
	void	Update();

	//---------------------------------------------------------------------------------------------

	// Internal. Determines platform-specific data to this device.
	inline	CHALData*	GetPlatformData();
	inline	void		SetPlatformData( CHALData* pBaseObject );

	// Returns whether a key is pressed, toggled, clicked or released since the last udpate.
	inline bool		IsKeyPressed( u8 uKey ) const;
	inline bool		IsKeyToggled( u8 uKey ) const;
	inline bool		IsKeyClicked( u8 uButton ) const;
	inline bool		IsKeyReleased( u8 uButton ) const;

private :

	CHALData*	m_pPlatformData;
	u8			m_PressedKeys[MAX_KEYBOARD_KEYS];
	u8			m_ToggledKeys[MAX_KEYBOARD_KEYS];

};

inline bool CKeyboard::IsKeyPressed( u8 uKey ) const
{
	return m_PressedKeys[uKey] != 0;
}

inline bool CKeyboard::IsKeyToggled( u8 uKey ) const
{
	return m_ToggledKeys[uKey] != 0;
}

inline bool	CKeyboard::IsKeyClicked( u8 uKey ) const
{
	return m_ToggledKeys[uKey] != 0 && m_PressedKeys[uKey] != 0;
}

inline bool	CKeyboard::IsKeyReleased( u8 uKey ) const
{
	return m_ToggledKeys[uKey] != 0 && m_PressedKeys[uKey] == 0;
}

inline CHALData* CKeyboard::GetPlatformData()
{
	return m_pPlatformData;
}

inline void CKeyboard::SetPlatformData( CHALData* pHALData )
{
	m_pPlatformData = pHALData;
}

}