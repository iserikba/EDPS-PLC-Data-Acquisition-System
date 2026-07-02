// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <tchar.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <objbase.h>
#include <malloc.h>
#include <crtdbg.h>
// OPC Interfaces and Errors definitions
#include ".\Interfaces\opcda.h"   
#include ".\Interfaces\OPCEnum.h"
#include ".\Interfaces\opcae_er.h"
#include ".\Interfaces\opcerror.h"


#ifndef __ATLCONV_H__

#define USES_CONVERSION int _convert = 0; LPCWSTR _lpw = NULL; LPCSTR _lpa = NULL

inline LPWSTR WINAPI AtlA2WHelper(LPWSTR lpw, LPCSTR lpa, int nChars) throw()
{
	if (lpw == NULL)	return NULL;
	lpw[0] = '\0';
	MultiByteToWideChar(CP_ACP, 0, lpa, -1, lpw, nChars);
	return lpw;
}

inline LPSTR WINAPI AtlW2AHelper(LPSTR lpa, LPCWSTR lpw, int nChars) throw()
{
	if (lpa == NULL)	return NULL;
	lpa[0] = '\0';
	WideCharToMultiByte(CP_ACP, 0, lpw, -1, lpa, nChars, NULL, NULL);
	return lpa;
}

#define A2W(lpa) (\
	((_lpa = lpa) == NULL) ? NULL : (\
		_convert = (lstrlenA(_lpa)+1),\
		AtlA2WHelper((LPWSTR) _alloca(_convert*2), _lpa, _convert)))

#define W2A(lpw) (\
	((_lpw = lpw) == NULL) ? NULL : (\
		_convert = (lstrlenW(_lpw)+1)*2,\
		AtlW2AHelper((LPSTR) _alloca(_convert), _lpw, _convert)))


#ifdef _UNICODE
	#define T2A W2A
	#define A2T A2W
	inline LPWSTR T2W(LPTSTR lp) { return lp; }
	inline LPTSTR W2T(LPWSTR lp) { return lp; }
	#define T2CA W2CA
	#define A2CT A2CW
	inline LPCWSTR T2CW(LPCTSTR lp) { return lp; }
	inline LPCTSTR W2CT(LPCWSTR lp) { return lp; }
#else
	#define T2W A2W
	#define W2T W2A
	inline LPSTR T2A(LPTSTR lp) { return lp; }
	inline LPTSTR A2T(LPSTR lp) { return lp; }
	#define T2CW A2CW
	#define W2CT W2CA
	inline LPCSTR T2CA(LPCTSTR lp) { return lp; }
	inline LPCTSTR A2CT(LPCSTR lp) { return lp; }
#endif

#endif /* __ATLCONV_H__ */
// TODO: reference additional headers your program requires here
