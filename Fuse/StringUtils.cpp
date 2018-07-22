#include "stdafx.h"

string WideCharToString(WCHAR* wideString)
{
	char buffer[DEFAULT_BUFFER_LENGTH];
	size_t numberOfCharsConverted;
	wcstombs_s(&numberOfCharsConverted, buffer, DEFAULT_BUFFER_LENGTH, wideString, DEFAULT_BUFFER_LENGTH);
	return string(buffer);
}