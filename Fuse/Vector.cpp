// Vector.cpp

#include "stdafx.h"

float CVector::LargestComponent()
{
	if ( fX > fY )
	{
		if ( fX > fZ ) 
			return fX;
		else
			return fZ;
	}
	else
	{
		if ( fY > fZ )
			return fY;
		else
			return fZ;
	}
}