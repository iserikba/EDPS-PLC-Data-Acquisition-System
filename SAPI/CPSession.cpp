#include <stdio.h>
#include "sapi_s7.h"
#include <COMDEF.H>
#include "CPSession.h"

#define BAD_DESC 0xFFFFFFFF

void str2log(char *);

char pszDefauldCP[] = "CP_H1_1:";
extern char MesBuf[2048];

CPSession::CPSession(const char *pszCP)
{
	char szVfdName[S7_MAX_NAMLEN+1];
    ord16 wNumb;

	sprintf(MesBuf, "\tCPSession:: Create new session: Session = %s.", pszCP);
	str2log(MesBuf);

	if (pszCP == NULL) pszCP = pszDefauldCP;
	
	sprintf(CPSessionName, "%s", pszCP);	// Initialise Session Name
	RefCount = 0;							// Initialise Reference Count

	sprintf(MesBuf, "\tCPSession:: Attempt to call s7_get_vfd(...).");
	str2log(MesBuf);

    int ret = s7_get_vfd((char*) pszCP, 0, &wNumb, szVfdName);

	sprintf(MesBuf, "\tCPSession:: Result of call s7_get_vfd(...) = %d, wNumb = %u", ret, wNumb);
	str2log(MesBuf);

    if ((ret != S7_OK) || (wNumb==0))
    {   // something has gone wrong 
		m_dwDecsCP = BAD_DESC;
		Status = s7_last_iec_err_no() + 1000;	// Just in case if bloody s7_... returns 0

		sprintf(MesBuf, "\tCPSession:: Bad session. Return (0 point)");
		str2log(MesBuf);

		return;
    }

    /* initialize s7 */
	sprintf(MesBuf, "\tCPSession:: Attempt to call s7_init(...).");
	str2log(MesBuf);

    ret = s7_init((char*) pszCP, szVfdName, &m_dwDecsCP);

	sprintf(MesBuf, "\tCPSession:: Result of call s7_init(...) = %d, m_dwDescCP = %X", ret, m_dwDecsCP);
	str2log(MesBuf);

    if (ret != S7_OK)
    {   // something has gone wrong 
		m_dwDecsCP = BAD_DESC;
		Status = -1;

		sprintf(MesBuf, "\tCPSession:: Bad session. Return (1 point).");
		str2log(MesBuf);

		return;
    }
	sprintf(MesBuf, "\tCPSession:: New session has been created. Session = %s.", CPSessionName);
	str2log(MesBuf);

	Status = 0;
}

CPSession::~CPSession()
{
	sprintf(MesBuf, "\tCPSession:: Delete session %s.", CPSessionName);
	str2log(MesBuf);

	if(m_dwDecsCP != BAD_DESC) 
	{
		sprintf(MesBuf, "\tCPSession:: Attempt to call s7_shut(...).");
		str2log(MesBuf);

		s7_shut(m_dwDecsCP);

		sprintf(MesBuf, "\tCPSession:: Returned from call s7_shut(...).");
		str2log(MesBuf);
	}
}

DWORD CPSession::DescCPRef(void)
{
	return m_dwDecsCP;
}

char *CPSession::CPName(void)
{
	return CPSessionName;
}

int CPSession::SessStatus(void)
{
	return Status;
}

int CPSession::IncRef(void)
{
	return ++RefCount;
}

int CPSession::DecRef(void)
{
	return --RefCount;
}

int CPSession::SetOrdNum(int Num)
{
	OrdNum = Num;
	return OrdNum;
}

int CPSession::GetOrdNum(void)
{
	return OrdNum;
}
