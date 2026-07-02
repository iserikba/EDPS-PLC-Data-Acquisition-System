#ifndef __SAPI_S7_CConnection
#define __SAPI_S7_CConnection

#include <stdio.h>
#include "sapi_s7.h"
#include "CPSession.h"
#include <COMDEF.H>

#define ConnQty 100

class CConnection
{
protected:
	int Status;
	int SessStatus;
	CPSession *Session;
	DWORD m_dwDecsCP;
	ord16 wConRef;
	char szConName[128];

	float Convert2Flt(void *data);
	long Convert2Lng(void *data);
	int Convert2Int(void *data);
	int Read(const char *szMemory, void *pDataBuf, int nBufLen);

	DWORD ConvertFlt(float data);
	DWORD ConvertLng(long data);
	WORD ConvertInt(int data);
	int Write(const char *szMemory,	void *pDataBuf, int nBufLen);

public:
	CConnection(const char *pszCP, const char *ConName);
	~CConnection();

	int ConnStatus(void);
	char *ConnName(void);		// Returns connection name
	char *SessionName(void);	// Returns session name

	long ReadData(const char *szItemName, const char *szType, BSTR * bsData );
	long WriteData(const char *szItemName, const char *szType, const char *szData);
};

#endif
