#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sapi_s7.h"
#include <COMDEF.H>
#include "CConnection.h"
#include "CPSession.h"

union ConvHeap{
	float f;
	long l;
	int i;
	DWORD dw;
	WORD sw;
	unsigned char s[4];
};

void str2log(char *);

CPSession *Sess[SessQty];
int ActSessQty;

extern char MesBuf[2048];

CConnection::CConnection(const char *pszCP, const char *ConName)
{
	sprintf(MesBuf, "CConnection:: Create new connection: Session = %s, Connection = %s.", pszCP, ConName);
	str2log(MesBuf);

	sprintf(szConName, "%s", ConName);

	for (int iI = 0;iI < ActSessQty;iI++)
		if (!strcmp(Sess[iI]->CPName(), pszCP)) // If there is such a session
		{
			sprintf(MesBuf, "CConnection:: Session exists.");
			str2log(MesBuf);
			break;
		}

	if (iI == ActSessQty)					// Have to create new Session
	{
		sprintf(MesBuf, "CConnection:: Session doesn't exist.");
		str2log(MesBuf);

		if (ActSessQty == SessQty)			// Limit of session is reached
		{
			sprintf(MesBuf, "CConnection:: Limit of session is reached. Return (0 point)");
			str2log(MesBuf);

			Status = -100;
			return;
		}

		sprintf(MesBuf, "CConnection:: Attempt to create a new session.");
		str2log(MesBuf);

		Sess[iI] = new CPSession((LPSTR)pszCP);

		SessStatus = Sess[iI]->SessStatus();

		sprintf(MesBuf, "CConnection:: Session is created. Status = %u.", SessStatus);
		str2log(MesBuf);

		if (SessStatus != 0)	// Unable to create Session !
		{
			Status = -200;

			sprintf(MesBuf, "CConnection:: Attempt to delete bad Session.");
			str2log(MesBuf);

			delete Sess[iI];

			sprintf(MesBuf, "CConnection:: Session has been deleted. Return (1 point).");
			str2log(MesBuf);

			return;
		}
		Sess[iI]->SetOrdNum(iI);	// Set order number of session
		ActSessQty++;				// OK, Session was created
	}

	sprintf(MesBuf, "CConnection:: Attempt to initialise Connection parameters.");
	str2log(MesBuf);

	Session = Sess[iI];	// Initialise CPSession
	Session->IncRef();	// Increase reference count for that session
	m_dwDecsCP = Session->DescCPRef();
	
	sprintf(MesBuf, "CConnection:: Attempt to obtain CRef using s7_get_cref(...) call.");
	str2log(MesBuf);

	iI = s7_get_cref(m_dwDecsCP, (char *)ConName, &wConRef);

	sprintf(MesBuf, "CConnection:: CRef is obtained. ReturnValue = %d.", iI);
	str2log(MesBuf);

	if( iI != S7_OK ) // Unable to create Connection !
	{
		Status = -300;

		sprintf(MesBuf, "CConnection:: Return (2 point).");
		str2log(MesBuf);

		return;
	}

	Status = 0;

	sprintf(MesBuf, "CConnection:: OK, return (3 point).");
	str2log(MesBuf);

	return;
}

CConnection::~CConnection()
{
	sprintf(MesBuf, "CConnection:: Delete connection: Connection = %s.", ConnName());
	str2log(MesBuf);

	if (SessStatus == 0 && Session->DecRef() == 0)	// Decrease reference count
	{
		sprintf(MesBuf, "CConnection:: RefCount is zero, trying to delete Session.");
		str2log(MesBuf);

		int Num = Session->GetOrdNum();
		delete Session;								// Delete Session if nobody uses

		sprintf(MesBuf, "CConnection:: Session is deleted.");
		str2log(MesBuf);

		sprintf(MesBuf, "CConnection:: Move the rest of array.");
		str2log(MesBuf);

		for (int iI = Num;iI < (ActSessQty - 1);iI++)
			Sess[iI] = Sess[iI+1];

		ActSessQty--;
	}
}

char * CConnection::SessionName(void)
{
	return Session->CPName();
}

int CConnection::ConnStatus(void)
{
	return Status;
}

char * CConnection::ConnName(void)
{
	return szConName;
}

int CConnection::Read(const char *szMemory, void *pDataBuf, int nBufLen)
{
	ord16 wOrderID;
	int32 ret; 
	time_t start, finish;
	double elapsed_time;
	int message;
	int iAtt = 0;

StartPoint:
	if ( iAtt++ >= 3 )	// Sorry, bloody time-out is inescapable
	{
		sprintf(MesBuf, "CConnection::Read function. Three attempts to read are failed due to time-out.. Return (-3).");
		str2log(MesBuf);

		return -3;
	}

	time( &start );

	sprintf(MesBuf, "CConnection:: Read function. Attempt %d to initiate request by s7_initiate_req(...) call.", iAtt);
	str2log(MesBuf);
	
	ret = s7_initiate_req(m_dwDecsCP, wConRef);

	sprintf(MesBuf, "CConnection:: Read function. Request initiated.");
	str2log(MesBuf);

	if ( ret != S7_OK )
	{
		sprintf(MesBuf, "CConnection:: Read function. Bad return value = %u. Return (0 point).", ret);
		str2log(MesBuf);

		return -1;
	}

	sprintf(MesBuf, "CConnection:: Read function. Start loop.");
	str2log(MesBuf);

	do
	{   
		message = s7_receive(m_dwDecsCP, &wConRef, &wOrderID);
		switch (message)
		{   
		case S7_NO_MSG: 
			break;
		case S7_INITIATE_CNF:
		{
			struct S7_READ_PARA rp;

			s7_get_initiate_cnf();
			memset( &rp,0,  sizeof(struct S7_READ_PARA) );
			rp.access = S7_ACCESS_SYMB_ADDRESS;
			strcpy( rp.var_name, szMemory);
			ret = s7_read_req(m_dwDecsCP, wConRef, 0, &rp);
			break;
		}
		case S7_READ_CNF:
		{
			ord16 len = nBufLen;
			ret = s7_get_read_cnf(NULL, &len, pDataBuf);
			if ( ret == S7_OK ) 
				nBufLen = len;
			else
				nBufLen = -4;
			ret = s7_abort(m_dwDecsCP, wConRef);
			break;
		}
		default:
			sprintf(MesBuf, "CConnection:: Read function. Wrong message recieved from S7 (%d). Return (1 point).", message);
			str2log(MesBuf);

			return -2;
		}
		time( &finish );
		elapsed_time = difftime( finish, start );
		if ( elapsed_time >= 10 ) 
		{
			ret = s7_abort(m_dwDecsCP, wConRef);
			if ( ret == S7_OK )
			{
				sprintf(MesBuf, "CConnection:: Read function. TimeOut. s7_abort was OK. Return (2 point).");
				str2log(MesBuf);
				Sleep(1000);
				goto StartPoint;
			}
			else
			{
				sprintf(MesBuf, "CConnection:: Read function. TimeOut. s7_abort failed(%u). Return (3 point).", ret);
				str2log(MesBuf);
				Sleep(1000);
				goto StartPoint;
			}
		}
		Sleep(1);
	} 
	while (message != S7_READ_CNF);

	sprintf(MesBuf, "CConnection::Read function. OK. Stop loop. Return (Last point).");
	str2log(MesBuf);

	return nBufLen;
}

int CConnection::Write(const char *szMemory, void *pDataBuf, int nBufLen)
{
	ord16 wOrderID;
	int32 ret; 
	time_t start, finish;
	double elapsed_time;
	int message;
	int iAtt = 0;

StartPoint:
	if ( iAtt++ >= 3 )	// Sorry, bloody time-out is inescapable
	{
		sprintf(MesBuf, "CConnection::Write function. Three attempts to write are failed due to time-out.. Return (-3).");
		str2log(MesBuf);

		return -3;
	}

	time( &start );
	
	sprintf(MesBuf, "CConnection:: Write function. Attempt %d to initiate request by s7_initiate_req(...) call.", iAtt);
	str2log(MesBuf);

	ret = s7_initiate_req(m_dwDecsCP, wConRef);

	sprintf(MesBuf, "CConnection::Write function. Request initiated.");
	str2log(MesBuf);

	if( ret != S7_OK ) 
	{
		sprintf(MesBuf, "CConnection::Write function. Bad return value(%u). Return (0 point).", ret);
		str2log(MesBuf);

		return -1;
	}

	sprintf(MesBuf, "CConnection::Write function. Start loop.");
	str2log(MesBuf);

	do
	{   
		message = s7_receive(m_dwDecsCP, &wConRef, &wOrderID);
		switch(message)
		{   
		case S7_NO_MSG: 
			break;
		case S7_INITIATE_CNF:
		{
			struct S7_WRITE_PARA wp;

			s7_get_initiate_cnf();
			memset( &wp,0,  sizeof(struct S7_WRITE_PARA) );
			wp.access = S7_ACCESS_SYMB_ADDRESS;
			strcpy( wp.var_name, szMemory);
			wp.var_length = nBufLen;
			memcpy( wp.value, pDataBuf, nBufLen);
			ret = s7_write_req(m_dwDecsCP, wConRef, 0, &wp, NULL);
			break;
		}
		case S7_WRITE_CNF:
		{
			ret = s7_get_write_cnf();
			if ( ret == S7_OK ) 
				nBufLen = 0;
			else
				nBufLen = -4;
			ret = s7_abort(m_dwDecsCP, wConRef);
			break;
		}
		default:
			sprintf(MesBuf, "CConnection::Write function. Wrong message recieved from S7 (%d). Return (1 point).", message);
			str2log(MesBuf);

			return -2;
		}
		time( &finish );
		elapsed_time = difftime( finish, start );
		if ( elapsed_time >= 10 ) 
		{
			ret = s7_abort(m_dwDecsCP, wConRef);
			if ( ret == S7_OK )
			{
				sprintf(MesBuf, "CConnection::Write function. TimeOut. s7_abort was OK. Return (2 point).");
				str2log(MesBuf);
				Sleep(1000);
				goto StartPoint;
			}
			else
			{
				sprintf(MesBuf, "CConnection::Write function. TimeOut. s7_abort failed (%u). Return (3 point).", ret);
				str2log(MesBuf);
				Sleep(1000);
				goto StartPoint;
			}
		}
		Sleep(1);
	} while (message != S7_WRITE_CNF);

	sprintf(MesBuf, "CConnection::Write function. OK. Stop loop. Return (Last point).");
	str2log(MesBuf);

	return nBufLen;
}

float CConnection::Convert2Flt(void *data)
{
	BYTE *pData = (BYTE*) data;
	ConvHeap ss;
	ss.s[0]=*(pData+3);
	ss.s[1]=*(pData+2);
	ss.s[2]=*(pData+1);
	ss.s[3]=*(pData);

	return ss.f;
}

DWORD CConnection::ConvertFlt(float data){
	BYTE pData;
	ConvHeap ss;
	ss.f = data;
	pData = ss.s[0];
	ss.s[0] = ss.s[3];
	ss.s[3] = pData;
	pData = ss.s[1];
	ss.s[1] = ss.s[2];
	ss.s[2] = pData;

	return ss.dw;
}

long CConnection::Convert2Lng(void *data)
{
	BYTE *pData = (BYTE*) data;
	ConvHeap ss;
	ss.s[0]=*(pData+3);
	ss.s[1]=*(pData+2);
	ss.s[2]=*(pData+1);
	ss.s[3]=*(pData);

	return ss.l;
}

DWORD CConnection::ConvertLng(long data){
	BYTE pData;
	ConvHeap ss;
	ss.l = data;
	pData = ss.s[0];
	ss.s[0]=ss.s[3];
	ss.s[3]=pData;
	pData = ss.s[1];
	ss.s[1]=ss.s[2];
	ss.s[2]=pData;

	return ss.dw;
}

int CConnection::Convert2Int(void *data)
{
	BYTE *pData = (BYTE*) data;
	ConvHeap ss;
	ss.s[0]=*(pData+1);
	ss.s[1]=*(pData);
	ss.s[2]=0;
	ss.s[3]=0;

	return ss.i;
}

WORD CConnection::ConvertInt(int data){
	BYTE pData;
	ConvHeap ss;
	ss.i = data;
	pData = ss.s[0];
	ss.s[0]=ss.s[1];
	ss.s[1]=pData;
	ss.s[2]=0;
	ss.s[3]=0;

	return ss.sw;
}

long CConnection::ReadData(const char *szItemName, const char *szType, BSTR *bsData)
{
	DWORD dwData;
	WORD wData;
	BYTE bData;
	float fl;
	long ln;
	char szData[20];
	int res;
	int slen;

	sprintf(MesBuf, "CConnection::ReadData function. Start.");
	str2log(MesBuf);

	SysFreeString(*bsData);

	if (strcmp(szType,"BIT") == 0) 
	{
		res = Read(szItemName, &bData, 1);
		if ( res < 0 ) 
		{
			*bsData = SysAllocString((OLECHAR FAR*)"Error!!!");

			sprintf(MesBuf, "CConnection::ReadData function. BIT type. Error reading (%d).", res);
			str2log(MesBuf);

			return res;
		}

		bData = bData & 0x00000001;
		ln = bData;
		_ltoa( ln, szData, 10 );
		slen = strlen(szData);
		*bsData = SysAllocStringByteLen((char*)szData, slen);
		
		sprintf(MesBuf, "CConnection::ReadData function. BIT type. OK.");
		str2log(MesBuf);
	
		return res;
	}

	if (strcmp(szType,"FLOAT") == 0) 
	{
		res = Read(szItemName, &dwData, 4);
		if ( res < 0 ) 
		{
			*bsData = SysAllocString((OLECHAR FAR*)"Error!!!");

			sprintf(MesBuf, "CConnection::ReadData function. FLOAT type. Error reading (%d).", res);
			str2log(MesBuf);

			return res;
		}
		fl = Convert2Flt(&dwData);
		_gcvt( fl, 12, szData );
		slen = strlen(szData);
		*bsData = SysAllocStringByteLen(szData, slen);
		
		sprintf(MesBuf, "CConnection::ReadData function. FLOAT type. OK.");
		str2log(MesBuf);
	
		return res;
	}

	if (strcmp(szType,"LONG") == 0) 
	{
		res = Read(szItemName, &dwData, 4);
		if ( res < 0 ) 
		{
			*bsData = SysAllocString((OLECHAR FAR*)"Error!!!");

			sprintf(MesBuf, "CConnection::ReadData function. LONG type. Error reading (%d).", res);
			str2log(MesBuf);

			return res;
		}
		ln = Convert2Lng(&dwData);
		_ltoa( ln, szData, 10 );
		slen = strlen(szData);
		*bsData = SysAllocStringByteLen((char*)szData, slen);
		
		sprintf(MesBuf, "CConnection::ReadData function. LONG type. OK.");
		str2log(MesBuf);
	
		return res;
	}

	if (strcmp(szType,"INTEGER") == 0) 
	{
		res = Read(szItemName, &wData, 2);
		if ( res < 0 ) 
		{
			*bsData = SysAllocString((OLECHAR FAR*)"Error!!!");

			sprintf(MesBuf, "CConnection::ReadData function. INTEGER type. Error reading (%d).", res);
			str2log(MesBuf);

			return res;
		}
		ln = Convert2Int(&wData);
		_itoa( ln, szData, 10 );
		slen = strlen(szData);
		*bsData = SysAllocStringByteLen((char*)szData, slen);
		
		sprintf(MesBuf, "CConnection::ReadData function. INTEGER type. OK.");
		str2log(MesBuf);
	
		return res;
	}

	if ( memcmp(szType,"STRING*", 7) == 0 )
	{
		int num = atoi(szType + 7);
		if ( num > 0 ) 
		{
			*bsData = SysAllocStringByteLen(NULL, num);;

			res = Read(szItemName, (char*)*bsData, num);
			if ( res < 0 ) 
			{
				SysFreeString(*bsData);
				*bsData = SysAllocString((OLECHAR FAR*)"Error!!!");

				sprintf(MesBuf, "CConnection::ReadData function. STRING* type. Error reading (%d).", res);
				str2log(MesBuf);

				return res;
			}
		
			sprintf(MesBuf, "CConnection::ReadData function. STRING* type. OK.");
			str2log(MesBuf);
	
			return res;
		}

		*bsData = SysAllocString((OLECHAR FAR*)"Error!!! Wrong parameter in STRING*");

		sprintf(MesBuf, "CConnection::ReadData function. Wrong parameter in STRING* (%s).", szType);
		str2log(MesBuf);

		return -1;
	}

	*bsData = SysAllocString((OLECHAR FAR*)"Error!!! Unknown type!");

	sprintf(MesBuf, "CConnection::ReadData function. Unknown type (%s).", szType);
	str2log(MesBuf);

	return -1;
}

long CConnection::WriteData(const char *szItemName, const char *szType, const char *szData)
{
    _bstr_t bsResult;
	DWORD dwData;
	WORD wData;
	BYTE bData;
	long in;
	float conv;
	int ii;

	sprintf(MesBuf, "CConnection::WriteData function. Start.");
	str2log(MesBuf);

	if (strcmp(szType,"BIT") == 0) 
	{
		in = atol (szData);
		bData = in;
		in = Write(szItemName, &bData, 1);

		sprintf(MesBuf, "CConnection::WriteData function. BIT type. OK.");
		str2log(MesBuf);

		return in;
	}

	if (strcmp(szType,"FLOAT") == 0) 
	{
		conv = atof (szData);
		dwData = ConvertFlt( conv );
		in = Write(szItemName, &dwData, 4);

		sprintf(MesBuf, "CConnection::WriteData function. FLOAT type. OK.");
		str2log(MesBuf);

		return in;
	}

	if (strcmp(szType,"LONG") == 0) 
	{
		in = atol(szData);
		dwData = ConvertLng( in );
		in = Write(szItemName, &dwData, 4);

		sprintf(MesBuf, "CConnection::WriteData function. LONG type. OK.");
		str2log(MesBuf);

		return in;
	}

	if (strcmp(szType,"INTEGER") == 0) 
	{
		ii = atoi(szData);
		wData = ConvertInt( ii );
		in = Write(szItemName, &wData, 2);

		sprintf(MesBuf, "CConnection::WriteData function. INTEGER type. OK.");
		str2log(MesBuf);

		return in;
	}

	if ( memcmp(szType,"STRING*", 7) == 0 )
	{
		int num = atoi(szType + 7);
		if ( num > 0 ) 
		{
			in = Write(szItemName, (void*)szData, num);

			sprintf(MesBuf, "CConnection::WriteData function. STRING* type. OK.");
			str2log(MesBuf);

			return in;
		}

		sprintf(MesBuf, "CConnection::WriteData function. Wrong parameter in STRING* (%s).", szType);
		str2log(MesBuf);

		return -5;
	}

	sprintf(MesBuf, "CConnection::WriteData function. Unknown type (%s).", szType);
	str2log(MesBuf);

	return -6;
}
