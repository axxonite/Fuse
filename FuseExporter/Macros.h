// Macros.h

#pragma once

namespace FuseExporter
{

#define	CHECK_ERROR( x )	{												\
	MStatus _Status = x;													\
	if ( _Status != MStatus::kSuccess )										\
	{																		\
		MGlobal::displayError( #x );										\
		throw EMayaError( Status );											\
	}																		\
}

}