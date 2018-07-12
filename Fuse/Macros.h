// Macros.h

#pragma once

namespace Fuse
{

#define CHECK_DIRECTX( x )	{ HRESULT _hResult; if ( ( _hResult = x ) != D3D_OK ) throw EDirectXError( _hResult ); };
#define IS_FLOAT_EQUAL( f , t ) ( (f) < ( (t) + EPSILON ) && (f) > ( (t) - EPSILON ) )

}