// Constants.h

#pragma once

namespace Fuse
{

const u32 DEFAULT_BUFFER_LENGTH	= 255;

const float EPSILON					= 1e-03f;
const float PI						= 3.141592653589793f;		// 180 degrees
const float QUARTER_PI				= PI * 0.25f;				// 45 degrees
const float HALF_PI					= PI * 0.5f;				// 90 degrees
const float PI_AND_A_HALF			= PI * 1.5f;				// 270 degrees
const float DOUBLE_PI				= PI * 2.0f;				// 360 degrees
const float ANGLETORADIAN_FACTOR	= PI / 180.0f;			// Angle to radians conversion factor
const float RADIANTOANGLE_FACTOR	= 180.0f / PI;			// Angle to radians conversion factor

const float ANGLE_45			= QUARTER_PI;
const float ANGLE_90			= HALF_PI;
const float ANGLE_180			= PI;
const float ANGLE_270			= PI_AND_A_HALF;
const float ANGLE_360			= DOUBLE_PI;

};