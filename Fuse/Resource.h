// BaseObject.h

#pragma once

namespace Fuse
{

// Resource base class.

class CResource : public CBaseObject
{

public :

	//---------------------------------------------------------------------------------------------

	// Resource name.
	inline	string	GetName();
	inline	void	SetName( string szName );

protected :

	string	m_szName;

};

inline string CResource::GetName()
{
	return m_szName;
}

inline void	CResource::SetName( string szName )
{
	m_szName = szName;
}

}