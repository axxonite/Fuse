// Color.h

#pragma once

namespace Fuse
{

//
// Represents a color type.
//

union CColor
{

	u32	uColor;
	struct 
	{
		u8	uBlue;
		u8	uGreen;
		u8	uRed;
		u8	uAlpha;
	} RGBA;

	// Default constructor.
	CColor()							{};
	// Convert from u32 constructor.
	inline CColor( u32 in_uColor );

};

inline CColor::CColor( u32 in_uColor )
{
	uColor = in_uColor;
}

const CColor COLOR_BLACK		= 0xFF000000;
const CColor COLOR_WHITE		= 0xFFFFFFFF;
const CColor COLOR_REF			= 0xFFFF0000;
const CColor COLOR_GREEN		= 0xFF00FF00;
const CColor COLOR_BLUE			= 0xFF0000FF;
const CColor COLOR_YELLOW		= 0xFFFFFF00;
const CColor COLOR_CYAN			= 0xFF00FFFF;
const CColor COLOR_MAGENTA		= 0xFFFF00FF;
const CColor COLOR_ORANGE		= 0xFFFF8000;
const CColor COLOR_GREY			= 0xFF808080;
const CColor COLOR_MAROON		= 0xFF800000;
const CColor COLOR_NAVY			= 0xFF008000;
const CColor COLOR_DARKGREEN	= 0xFF000080;
const CColor COLOR_OLIVE		= 0xFF808000;
const CColor COLOR_TEAL			= 0xFF008080;
const CColor COLOR_BROWN		= 0xFFA52A2A;
const CColor COLOR_VIOLET		= 0xFFEE82EE;
const CColor COLOR_TURQUOISE	= 0xFF40E0D0;
const CColor COLOR_SILVER		= 0xFFC0C0C0;
const CColor COLOR_PINK			= 0xFFFFC0CB;

}
