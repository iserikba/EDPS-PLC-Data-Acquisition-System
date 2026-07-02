#ifndef __SAPI_S7_CPSession
#define __SAPI_S7_CPSession

#include <stdio.h>
#include "sapi_s7.h"
#include <COMDEF.H>

#define SessQty 16

class CPSession
{
protected:
	int OrdNum;				// Order number of Session in the array
	int Status;				// Last error number
	int RefCount;			// Reference count
	DWORD m_dwDecsCP;		// S7 Session number
	char CPSessionName[128];// Symbolic name of the Session

public:
	CPSession(const char *pszCP); 
	~CPSession();

	int SetOrdNum(int);		// Set order number
	int GetOrdNum(void);	// Get order number

	int IncRef(void);		// Increase reference count
	int DecRef(void);		// Decrease reference count
	int SessStatus(void);	// Returns Status of the Session
	char *CPName(void);		
	DWORD DescCPRef(void);
};

#endif
