// BaseObject.h

#pragma once

namespace Fuse
{

//
// assert macro.
//

enum eAssertResponse
{

	eABORT,
	eRETRY,
	eIGNORE,
	eMAX_ASSERTRESPONSE

};

#ifndef _DEBUG
#define assert( x )			{}
#else
#define assert( x )																		\
{																						\
	static bool ASSERT_Ignore = false;													\
	if( !( x ) && !ASSERT_Ignore )														\
	{																					\
		eAssertResponse Response = AssertReport( #x, __FILE__, __LINE__ );				\
		if( Response == eRETRY )														\
		{																				\
			__asm { int 3 };															\
		}																				\
		else if( Response == eABORT )													\
			::TerminateProcess( GetCurrentProcess(), 0xdeadf00d );						\
		else																			\
			ASSERT_Ignore = true;														\
	}																					\
}
#endif // _DEBUG

eAssertResponse AssertReport( const char* szExpr, const char* szFile, int iLine );

}