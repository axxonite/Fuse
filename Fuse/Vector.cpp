// Vector.cpp

#include "stdafx.h"

float CVector::LargestComponent() const
{
	return max( fX, max(fY, fZ) );
}
