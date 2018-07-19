#include "stdafx.h"
#include "FuseAssert.h"

eAssertResponse Fuse::AssertReport( const char* szExpr, const char* szFile, int iLine )
{
	char szBuffer[1024];
	
	_snprintf_s(	szBuffer, sizeof( szBuffer ),
		"%s\n"
		"assert failed in File: %s, line: %d\n\n", szExpr, szFile, iLine );

	u32 uResponse = MessageBoxA( NULL, szBuffer, "ASSERTION FAILED!", MB_ICONWARNING | MB_ABORTRETRYIGNORE | MB_SYSTEMMODAL) ; 
	switch ( uResponse )
	{
	case IDABORT :
		return eABORT;
	case IDRETRY :
		return eRETRY;
	case IDIGNORE :
		return eIGNORE;
	default :
		return eRETRY;
	}
}