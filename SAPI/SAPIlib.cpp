#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <COMDEF.H>
#include "CConnection.h"

#define CCONV _stdcall

CConnection *Con[ConnQty];
int ActConnQty = 0;
char MesBuf[2048];

void str2log(char *mess)
{
	char tmpbuff[2048], tmbuf1[50], tmbuf2[50];
	char *TrEnv;

/*
**	Set SAPITRACE=YES environment variable to trace this DLL. Any 
**  other value or absense of that variable means "don't trace". 
*/
	TrEnv = getenv( "SAPITRACE" );
	if( TrEnv != NULL && !strcmp(TrEnv, "YES") )
	{
		int fh = _open( "SAPILib.log", _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE );
		if( fh != -1 )
		{
			sprintf(tmpbuff, "%s %s: %s\n", _strdate(tmbuf1), _strtime(tmbuf2), mess);
			long pos = _lseek( fh, 0L, SEEK_END );
			if( pos != -1L ) _write(fh, tmpbuff, strlen(tmpbuff));
			_close( fh );
		}
	}
	return;
}

long CCONV SAPI_Shut( void )
{
	sprintf(MesBuf, "### Shutting down SAPI.");
	str2log(MesBuf);

	for (int iI = 0;iI < ActConnQty;iI++)
		delete Con[iI];

	ActConnQty = 0;

	sprintf(MesBuf, "### SAPI is shut down.");
	str2log(MesBuf);

	return 0;
}

long CCONV SAPI_read ( BSTR bsCP, BSTR bsConn, BSTR bsItem, BSTR bsType, BSTR * bsData )
{
	sprintf(MesBuf, "### Start of SAPI_read.");
	str2log(MesBuf);

	for (int iI = 0;iI < ActConnQty;iI++)	// Look for existing connection
		if (!strcmp(Con[iI]->ConnName(), (LPSTR)bsConn) && !strcmp(Con[iI]->SessionName(), (LPSTR)bsCP)) 
			break;

	if (iI == ActConnQty)	// There isn't such a connection, have to create new one
	{
		if (ActConnQty == ConnQty)			// Limit of connections is reached
		{
			sprintf(MesBuf, "### Unable to create connection, limit is reached. Return.");
			str2log(MesBuf);

			return -99;
		}

		Con[iI] = new CConnection((LPSTR)bsCP, (LPSTR)bsConn);
		long Stat = Con[iI]->ConnStatus();
		if (Stat != 0)		// Connection was not created
		{
			delete Con[iI];

			sprintf(MesBuf, "### Bad connection was deleted. Return.");
			str2log(MesBuf);

			return Stat;
		}
		ActConnQty++;
	}

	sprintf(MesBuf, "### Session&Connection part done well. Try to read.");
	str2log(MesBuf);

	long lRet = Con[iI]->ReadData( (LPSTR)bsItem, (LPSTR)bsType, bsData );

	sprintf(MesBuf, "### ReadData finished with code %u.", lRet);
	str2log(MesBuf);

	if (lRet < 0)			// Some error during execution
		SAPI_Shut();		// Close all existing connection & Log off CP (VFD)

	sprintf(MesBuf, "### Leaving SAPI_read.");
	str2log(MesBuf);

	return lRet;
}

long CCONV SAPI_write ( BSTR bsCP, BSTR bsConn, BSTR bsItem, BSTR bsType, BSTR bsData )
{
	sprintf(MesBuf, "### Start of SAPI_write.");
	str2log(MesBuf);

	for (int iI = 0;iI < ActConnQty;iI++)	// Look for existing connection
		if (!strcmp(Con[iI]->ConnName(), (LPSTR)bsConn) && !strcmp(Con[iI]->SessionName(), (LPSTR)bsCP)) 
			break;

	if (iI == ActConnQty)	// There isn't such a connection, have to create new one
	{
		if (ActConnQty == ConnQty)			// Limit of connections is reached
		{
			sprintf(MesBuf, "### Unable to create connection, limit is reached. Return.");
			str2log(MesBuf);

			return -99;
		}

		Con[iI] = new CConnection((LPSTR)bsCP, (LPSTR)bsConn);  
		long Stat = Con[iI]->ConnStatus();
		if (Stat != 0)		// Connection was not created
		{
			delete Con[iI];

			sprintf(MesBuf, "### Bad connection was deleted. Return.");
			str2log(MesBuf);

			return Stat;
		}
		ActConnQty++;
	}

	sprintf(MesBuf, "### Session&Connection part done well. Try to write.");
	str2log(MesBuf);

	long lRet = Con[iI]->WriteData( (LPSTR)bsItem, (LPSTR)bsType, (LPSTR)bsData);

	sprintf(MesBuf, "### WriteData finished with code %u.", lRet);
	str2log(MesBuf);

	if (lRet < 0)			// Some error during execution
		SAPI_Shut();		// Close all existing connection & Log off CP (VFD)

	sprintf(MesBuf, "### Leaving SAPI_write.");
	str2log(MesBuf);

	return lRet;
}

