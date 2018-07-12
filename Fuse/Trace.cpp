// Trace.cpp

#include "stdafx.h"
#include "Trace.h"

// Traces output to the debugger.
void Fuse::Trace( const char* szString, ... )
{
	va_list ArgList;
	va_start( ArgList, szString );

	static char szBuffer[DEFAULT_BUFFER_LENGTH];

	_vsnprintf( szBuffer, DEFAULT_BUFFER_LENGTH - 1, szString, ArgList );
	OutputDebugString( szBuffer );

	va_end( ArgList );
}