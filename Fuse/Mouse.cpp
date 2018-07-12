// Mouse.cpp

#include "stdafx.h"
#include "Mouse.h"
#include "HAL.h"
#include "HALInputSystem.h"

CMouse::CMouse() :
	m_pPlatformData		( NULL		),
	m_MousePosition		( 0, 0, 0	),
	m_MouseDisplacement	( 0, 0, 0	)
{
	ZeroMemory( &m_PressedButtons, MAX_MOUSE_KEYS );
	ZeroMemory( &m_ToggledButtons, MAX_MOUSE_KEYS );
}

CMouse::~CMouse()
{
	delete m_pPlatformData;
}

void CMouse::Update()
{
	g_pInputSystem->UpdateMouse( m_PressedButtons, m_ToggledButtons, m_MousePosition, m_MouseDisplacement );
}