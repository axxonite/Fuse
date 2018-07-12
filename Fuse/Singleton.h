// Singleton.h

#pragma once

namespace Fuse
{

// Singleton pattern class.

template <class T> class CSingleton
{

public :

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CSingleton();
	// Destructor.
	virtual ~CSingleton();

	//---------------------------------------------------------------------------------------------

	// Returns the current instance of the singleton.
	inline static	T*	GetInstance();

protected :

	static	T*	ms_pInstance;

};

template <class T> T* CSingleton<T>::ms_pInstance = NULL;

template <class T> inline CSingleton<T>::CSingleton()
{
	assert( !ms_pInstance );
	ms_pInstance = static_cast< T* >( this );
}

template <class T> inline CSingleton<T>::~CSingleton()
{
	assert( ms_pInstance );
	ms_pInstance = NULL;
}

template <class T> inline T* CSingleton<T>::GetInstance()
{
	return ms_pInstance;
}

}