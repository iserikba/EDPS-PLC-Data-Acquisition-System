#include "stdafx.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <stdarg.h>

#include "DllIni.h"

LPCTSTR szClientName = _T("EDPS.S7OPCCLIENT.1");
LPCTSTR szIniFile	= _T("Edps.ini");
LPCTSTR szS7OPC		= _T("OPCS7");
LPCTSTR szTrace		= _T("Trace");
LPCTSTR szRefrRatio	= _T("RefreshRatio");
LPCTSTR szOpcServer	= _T("S7OpcServer");
LPCTSTR szEmgNode	= _T("RemoteServerNode");
LPCTSTR szLogFile	= _T("s7opcdll.log");

UINT nTraceDef			= 0;
UINT nRefrRatioDef		= 5000;
LPCTSTR szS7OPCDef		= _T("OPC.SimaticNET");
LPCTSTR szEmgNodeDef	= _T("");

struct MY_INI_STRUCT MyIni;

//void ResolveIniFile( void )
//{
//	if (GetPrivateProfileInt( szS7OPC, szTrace, -1, szIniFile) == -1) 
//	{
//		// No file, create one
//		WritePrivateProfileInt( szS7OPC, szSleep, szSleepDef, szIniFile);
//		WritePrivateProfileString( szS7OPC, szTrace, szTraceDef, szIniFile);
//		WritePrivateProfileString( szS7OPC, szRepeat, szRepeatDef, szIniFile);
//		WritePrivateProfileString( szS7OPC, szTimeOut, szTimeOutDef, szIniFile);
//		WritePrivateProfileString( szS7OPC, szSleepOnError, szSleepOnErrorDef, szIniFile);
//	}
//	return;
//}

void SetIniValues(HMODULE hInst)
{
	TCHAR szIniFileName[NAMEBUF_SIZE];
	::GetModuleFileName(hInst, MyIni.LogFile, NAMEBUF_SIZE);
	for(int n = lstrlen(MyIni.LogFile)-1; n>0; n--)
	{
		if(MyIni.LogFile[n]=='\\') break;
		MyIni.LogFile[n]=0;
	}
	lstrcpy(szIniFileName, MyIni.LogFile);
	lstrcat(szIniFileName, szIniFile);
	lstrcat(MyIni.LogFile, szLogFile);

	MyIni.iTrace = ::GetPrivateProfileInt( szS7OPC, szTrace, nTraceDef, szIniFileName );
	MyIni.nRefreshRatio = ::GetPrivateProfileInt( szS7OPC, szRefrRatio, nRefrRatioDef, szIniFileName );
	::GetPrivateProfileString(szS7OPC, szOpcServer, szS7OPCDef, MyIni.S7OpcServer, NAMEBUF_SIZE, szIniFileName);
	::GetPrivateProfileString(szS7OPC, szEmgNode, szEmgNodeDef, MyIni.EmergencyServerNode, NAMEBUF_SIZE, szIniFileName);
 	return;
}

void str2log( const char *format, ... )
{
	static HANDLE hSema = NULL;
	char tmpbuff[1024], tmbuf1[50];

	if( MyIni.iTrace != 0 )
	{
		if(!hSema) 
			hSema = CreateSemaphore(NULL, 1, 1, NULL); // Init count = 1, Max count = 1

		// Make formatting string 
		strcpy(tmpbuff, _strdate(tmbuf1));
		strcat(tmpbuff, " ");
		strcat(tmpbuff, _strtime(tmbuf1));
		strcat(tmpbuff, " : ");
		DWORD nBufLen = (DWORD)strlen(tmpbuff);

		va_list param;
		va_start(param, format);

		vsprintf(tmpbuff+nBufLen, format, param);

		va_end(param);

		strcat(tmpbuff, "\n");
		nBufLen = (DWORD)strlen(tmpbuff);

		// Waiting for signal of Sema then locking it
		WaitForSingleObject(hSema, INFINITE); 

		int fh = _open( MyIni.LogFile, _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE );
		if( fh != -1 )
		{
			long pos = _lseek( fh, 0L, SEEK_END );
			if( pos != -1L ) _write(fh, tmpbuff, nBufLen);
			_close( fh );
		}

		ReleaseSemaphore(hSema, 1, NULL); 
	}
	return;
}
