// Mouse.h

#pragma once

namespace Fuse
{

class CHALData;

//
// Mouse input device.
//

class CMouse : public CBaseObject
{

public :

	static	const u32	MAX_MOUSE_KEYS	= 0x08;

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CMouse();
	// Destructor.
	virtual ~CMouse();

	//---------------------------------------------------------------------------------------------

	// Polls the mouse state and updates mouse information.
	void	Update();

	//---------------------------------------------------------------------------------------------

	// Internal. Determines platform-specific data to this device.
	inline	CHALData*	GetPlatformData();
	inline	void		SetPlatformData( CHALData* pBaseObject );

	// Returns whether a button is pressed, toggled, clicked or released since the last udpate.
	inline bool		IsButtonPressed( u8 uButton ) const;
	inline bool		IsButtonToggled( u8 uButton ) const;
	inline bool		IsButtonClicked( u8 uButton ) const;
	inline bool		IsButtonReleased( u8 uButton ) const;

	// Returns the mouse cursor position in our window.
	inline	CVector	GetMousePosition() const;
	// Returns the displacement of the mouse since the last update.
	inline	CVector	GetMouseDisplacement() const;

private :

	CHALData*	m_pPlatformData;
	u8			m_PressedButtons[MAX_MOUSE_KEYS];
	u8			m_ToggledButtons[MAX_MOUSE_KEYS];
	CVector		m_MousePosition;
	CVector		m_MouseDisplacement;

};

inline CVector CMouse::GetMouseDisplacement() const
{
	return m_MouseDisplacement;
}

inline CVector CMouse::GetMousePosition() const
{
	return m_MousePosition;
}

inline CHALData* CMouse::GetPlatformData()
{
	return m_pPlatformData;
}

inline bool CMouse::IsButtonPressed( u8 uButton ) const
{
	return m_PressedButtons[uButton] != 0;
}

inline bool CMouse::IsButtonToggled( u8 uButton ) const
{
	return m_ToggledButtons[uButton] != 0;
}

inline bool	CMouse::IsButtonClicked( u8 uButton ) const
{
	return m_ToggledButtons[uButton] != 0 && m_PressedButtons[uButton] != 0;
}

inline bool	CMouse::IsButtonReleased( u8 uButton ) const
{
	return m_ToggledButtons[uButton] != 0 && m_PressedButtons[uButton] == 0;
}

inline void CMouse::SetPlatformData( CHALData* pHALData )
{
	m_pPlatformData = pHALData;
}

}