// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OPCS7_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OPCS7_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef OPCS7_EXPORTS
#define OPCS7_API __declspec(dllexport)
#else
#define OPCS7_API __declspec(dllimport)
#endif

#define CCONV _stdcall

#include "dllini.h"

// This functions are exported from the OpcS7.dll
OPCS7_API long CCONV SAPI_Shut( void );
OPCS7_API long CCONV SAPI_Read( BSTR, BSTR, BSTR, BSTR, BSTR * );
OPCS7_API long CCONV SAPI_Write( BSTR, BSTR, BSTR, BSTR, BSTR );
OPCS7_API long CCONV SAPI_MRead( BSTR, BSTR * );
OPCS7_API long CCONV SAPI_MWrite( BSTR );
