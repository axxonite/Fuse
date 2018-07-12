// Keyboard.cpp

#include "stdafx.h"
#include "Keyboard.h"
#include "HAL.h"
#include "HALInputSystem.h"

CKeyboard::CKeyboard() :
	m_pPlatformData( NULL )
{
	ZeroMemory( &m_PressedKeys, MAX_KEYBOARD_KEYS );
	ZeroMemory( &m_ToggledKeys, MAX_KEYBOARD_KEYS );
}

CKeyboard::~CKeyboard()
{
	delete m_pPlatformData;
}

void CKeyboard::Update()
{
	g_pInputSystem->UpdateKeyboard( m_PressedKeys, m_ToggledKeys );
}