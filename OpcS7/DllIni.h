#ifndef ini_DEFS
#define ini_DEFS

#define NAMEBUF_SIZE 128

struct MY_INI_STRUCT
{
	BOOL iTrace;
	UINT nRefreshRatio;
	TCHAR S7OpcServer[NAMEBUF_SIZE];
	TCHAR EmergencyServerNode[NAMEBUF_SIZE];
	TCHAR LogFile[NAMEBUF_SIZE];
};


extern struct MY_INI_STRUCT MyIni;
extern LPCTSTR szClientName;
//void ResolveIniFile( void );
extern void SetIniValues(HMODULE hInst);
extern void str2log( const char *format, ... );

#endif
