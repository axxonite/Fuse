// Exceptions.h

#pragma once

namespace Fuse
{

//
// DirectX error exception class.
//

class EDirectXError

{

public :

	inline	EDirectXError( HRESULT hErrorCode );

	inline	HRESULT	GetErrorCode() const;

private :

	HRESULT	m_hErrorCode;

};

inline EDirectXError::EDirectXError( HRESULT hErrorCode )
{
	m_hErrorCode = hErrorCode;
}

inline HRESULT EDirectXError::GetErrorCode() const
{
	return m_hErrorCode;
}

}