// EditorExceptions.h

#pragma once

namespace Fuse
{

//
// WIN32 error exception class.
//

class EWin32Error

{

public :

	inline	EWin32Error();

	inline	HRESULT	GetErrorCode() const;

protected :

	HRESULT	m_hErrorCode;

};

inline EWin32Error::EWin32Error()
{
	m_hErrorCode = GetLastError();
}

inline HRESULT EWin32Error::GetErrorCode() const
{
	return m_hErrorCode;
}

}