#include "stdafx.h"
#include ".\OPCRelease\OPCDAServer.h"
#include "EdpsParams.h"

#include <ctype.h>

long GetNextValue( LPCSTR Point, LPSTR Buff );
VARTYPE GetVarType( LPCSTR szName, UINT &nSize );
LPSTR AddValue2Buf(LPSTR szBuff, UINT &nBufLen, UINT &nBufPos, LPCSTR szData, DWORD nID);
LPCSTR GetValueVariant(COPCItem  *pItem, LPSTR szBuffer, DWORD &dwGoodCount);

CEdpsParamList::~CEdpsParamList() 
{ 
	if(m_pStringBuffer) {
		free(m_pStringBuffer);
	}
	while(m_pHead) {
		CEdpsParam *pCurr = m_pHead;
		m_pHead = pCurr->m_pNext;
		::VariantClear( &pCurr->m_val );
		delete pCurr;
	}
}

void AddHash(DWORD &dwHash, LPCSTR szStr)
{
	UINT nLen = 0;
	DWORD dzBit;
	for( ;*szStr; szStr++ ) {
		// Rotation
		dzBit = dwHash & 0x80000000? 1 : 0;
		dwHash = dwHash << 1;
		dwHash |= dzBit;

		dwHash ^= *szStr;
		nLen ++;
	}
	dwHash |= nLen;

}

// Return Name of Group (enum ParID) returned string must be freed by free()
LPCTSTR CEdpsParamList::GetListName(  )
{
	LPTSTR szName;
	if( !m_dwCount ) {
		szName = (LPTSTR) malloc( sizeof(TCHAR) );
		szName[0] = '\0';
	}
	else {	
		DWORD dwHash=0;
		LPTSTR szPtr = szName = (LPTSTR) malloc(sizeof(TCHAR)*m_dwCount*10);
		for( CEdpsParam *pCurr = m_pHead; pCurr; pCurr = pCurr->m_pNext) 
		{
			AddHash(dwHash, pCurr->m_pszItemId);
#ifdef _UNICODE
			_ltow( pCurr->m_dwParID, szPtr, 10); 
#else
			_ltoa( pCurr->m_dwParID, szPtr, 10); 
#endif
			while(*szPtr) szPtr++;
			*szPtr++ =',';
		}
#ifdef _UNICODE
		_ltow( dwHash, szPtr, 10); 
#else
		_ltoa( dwHash, szPtr, 10); 
#endif
	}
		
	return szName;
}

BOOL CEdpsParamList::AddItem(DWORD dwParID, LPCSTR szItemId)
{
	CEdpsParam *pNew = new CEdpsParam;
	pNew->m_dwParID = dwParID;
	pNew->m_pszItemId = szItemId;
	pNew->m_pItem = NULL;
	pNew->m_pNext = NULL;

	::VariantInit( &pNew->m_val );

	if(m_pTail) m_pTail->m_pNext = pNew;
	else		m_pHead = pNew;
	m_pTail = pNew;	

	return true;
}

// Read Single Item and return Result String or NULL if reading failed
BSTR CEdpsParamList::ReadValue(COPCGroup *pGroup)
{
	DWORD dwGoodCount=0;

	if(!m_pTail || !m_pTail->m_pItem ) 
		return NULL;

	pGroup->InitReading(*m_pTail->m_pItem);	
	HRESULT hr = pGroup->ReadItems(true);
	if( FAILED(hr) ) {
		return NULL;
	}

	LPSTR pszBuffer = new char [80];

	LPCSTR pszRes = GetValueVariant(m_pTail->m_pItem, pszBuffer, dwGoodCount);	
	BSTR bsRes = dwGoodCount ?  ::SysAllocStringByteLen( pszRes, lstrlenA(pszRes) ) : NULL;

	
	delete [] pszBuffer;
	return bsRes;
}

// Write Single Item
BOOL CEdpsParamList::WriteValue(COPCGroup *pGroup)
{
	if(!m_pTail || !m_pTail->m_pItem ) 
		return FALSE;

	pGroup->InitWriting(*m_pTail->m_pItem, m_pTail->m_val);	
	HRESULT hr = pGroup->WriteItems();
	if( FAILED(hr) || hr==S_FALSE) {
		return FALSE;
	}

	return true;
}


// Set Single Item For Read/Write if it's for read set szType=NULL; 
int CEdpsParamList::SingleItem(COPCGroup *pGroup, LPCSTR szConn, LPCSTR szItem, LPCSTR szType, LPCSTR szDate)
{
	BOOL bRead = szType==NULL;

	UINT nLenConn = lstrlenA( szConn );
	UINT nLenItem = lstrlenA( szItem );

	// Joining OPC Item Name
	m_pStringBuffer = (char*) malloc(nLenConn + nLenItem + 6);
	*((DWORD*)m_pStringBuffer ) = 0x5B3A3753; // "S7:["
	memcpy(m_pStringBuffer + 4, szConn, nLenConn);
	m_pStringBuffer[4+nLenConn] = ']';
	memcpy(m_pStringBuffer + 5 + nLenConn, szItem, nLenItem+1);

	COPCItem *pItem = pGroup->GetItem(m_pStringBuffer);
	if(!pItem->Valid() )	
	{
		USES_CONVERSION;
		if(!pItem && !(pItem = pGroup->NewItem(A2T((LPSTR)m_pStringBuffer), pGroup->ItemsCount() + 1)))
			return -1;

		HRESULT hr = pGroup->AddItems();
		if( FAILED(hr) || hr==S_FALSE ) 
			return  -1;
	}

	if( bRead ) {
		AddItem(pItem->HandleUser(), m_pStringBuffer);
		m_pTail->m_pItem = pItem;
	}
	else
	{
		if(! AddItem(pItem->HandleUser(), m_pStringBuffer, szType, szDate) )
			return -2;
		m_pTail->m_pItem = pItem;
	}
	return 0;
}

int CEdpsParamList::FindOrAddItemsToGroup( COPCGroup *pMyGrop, BOOL bSet2Read)
{
	UINT dwAdd=0;
	int nRet=0;

	for( CEdpsParam *pCurr = m_pHead; pCurr; pCurr = pCurr->m_pNext) 
	{
		USES_CONVERSION;
		LPCTSTR sztItemId = A2T((LPSTR)pCurr->m_pszItemId);
		pCurr->m_pItem = pMyGrop->GetItem(pCurr->m_dwParID);
		if( !pCurr->m_pItem ) { 
			pCurr->m_pItem = pMyGrop->NewItem(sztItemId, pCurr->m_dwParID); 
			if(pCurr->m_pItem) dwAdd ++;
			else return -2;
		}
	}

	if(dwAdd) {
		HRESULT hr = pMyGrop->AddItems();
		if( FAILED( hr) ) return -2;
		if( hr==S_FALSE ) nRet= -1;
		else nRet= dwAdd;
		for( pCurr = m_pHead; pCurr; pCurr = pCurr->m_pNext) 
			pCurr->m_pItem =pMyGrop->GetItem(pCurr->m_dwParID);

	}

	for( pCurr = m_pHead; pCurr; pCurr = pCurr->m_pNext) 
	{
		if(bSet2Read)
			pMyGrop->InitReading( *pCurr->m_pItem );
		else
			pMyGrop->InitWriting( *pCurr->m_pItem, pCurr->m_val );
	}

	return nRet;

}

BOOL CEdpsParamList::AddItem(DWORD dwParID, LPCSTR szItemId, LPCSTR szType, LPCSTR szData)
{
	AddItem(dwParID, szItemId);

	if( strcmp(szType, "BIT")==0 ) {
		m_pTail->m_val.vt = VT_BOOL;
	}
	else if( strcmp(szType, "FLOAT")==0) {
		m_pTail->m_val.vt = VT_R4;
		m_pTail->m_val.fltVal = (float) atof(szData);
		return true;
	}
	else if( strcmp(szType, "LONG")==0) {
		m_pTail->m_val.vt = VT_I4;
	}
	else if( strcmp(szType, "INTEGER")==0) {
		m_pTail->m_val.vt = VT_I2;
	}
	else if( strcmp(szType, "BYTE")==0 || strcmp(szType, "CHAR")==0) {
		m_pTail->m_val.vt = VT_UI1;
	}
	else if( strncmp(szType, "STRING*", 7)==0)	{
		m_pTail->m_val.vt = VT_ARRAY | VT_UI1;
		SAFEARRAYBOUND rgsabound[1];

		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = atoi(szType + 7);
		m_pTail->m_val.parray = ::SafeArrayCreate(VT_UI1, 1, rgsabound);
		BYTE HUGEP *pChar;
		HRESULT hr = ::SafeArrayAccessData(m_pTail->m_val.parray, (void HUGEP**)&pChar);
		if( SUCCEEDED(hr) ) {
			BOOL bZero = false;
			for(UINT i = 0; i < rgsabound[0].cElements; i++) {
				bZero |=  szData[i] == 0;
				pChar[i] = bZero ? 0 : szData[i];
			}
			::SafeArrayUnaccessData(m_pTail->m_val.parray);
			return true;
		}
		else {
			m_pTail->m_val.vt = VT_EMPTY;
			return false;
		}
	}
	else
		return false;

	m_pTail->m_val.lVal = atol(szData);
	return true;
}

LPCSTR GetValueVariant(COPCItem  *pItem, LPSTR szBuffer, DWORD &dwGoodCount)
{
	static LPCSTR szErrorR = "Error Read";
	static LPCSTR szErrorT = "Error Type";
	if(pItem->Quality() ) 	{
		VARIANT &var = pItem->Data();
		switch( var.vt ) {
		case VT_BOOL:
			szBuffer[0] = var.boolVal ? '1' : '0';
			szBuffer[1] = '\0';
			break;
		case VT_R4:
			_gcvt((double)var.fltVal, 8, szBuffer); 
			break;
		case VT_R8:
			_gcvt(var.dblVal, 12, szBuffer);
			break;
		case VT_I2:
		case VT_I4:
		case VT_INT:
			_ltoa(var.lVal, szBuffer, 10);
			break;
		case VT_UI2:
		case VT_UI4:
		case VT_UINT:
			_ltoa(var.ulVal, szBuffer, 10);
			break;
		case VT_ARRAY | VT_UI1:
		case VT_ARRAY | VT_I1:
		{
			BYTE HUGEP *pChar;
			HRESULT hr = ::SafeArrayAccessData(var.parray, (void HUGEP**)&pChar);
			if( SUCCEEDED(hr) ) {
				for(UINT i = 0; i < var.parray->rgsabound[0].cElements; i++) {
					szBuffer[i] = pChar[var.parray->rgsabound[0].lLbound + i];
				}
				szBuffer[var.parray->rgsabound[0].cElements] = '\0';
				::SafeArrayUnaccessData(var.parray);
			}
			else {
				szBuffer[0] = '\0';
			}
		}
			break;
		default:
			return szErrorT;
		}
	}
	else {
		return szErrorR;
	}

	dwGoodCount++;
	return szBuffer;
}

BSTR CEdpsParamList::GetMReadResult(DWORD &dwOk)
{
	char szDataBuf[64];
	char szId[24];
	UINT nBufLen = m_dwCount*20;
	UINT nBufPos = 0;
	LPSTR szResult= (LPSTR) malloc(nBufLen);
	dwOk = 0;

	for( CEdpsParam *pCurr = m_pHead; pCurr; pCurr = pCurr->m_pNext) 
	{
		LPCSTR pszData = GetValueVariant(pCurr->m_pItem, szDataBuf, dwOk);
		UINT nLen = lstrlenA( _ltoa(pCurr->m_dwParID, szId, 10) );
		UINT nLenD = lstrlenA( pszData );

		if( (nBufLen-nBufPos)<(nLen+nLenD+7) ) 
		{ // Not enough space to save parameter
			szResult = (LPSTR) realloc((void*) szResult, nBufLen += nLen + nLenD + 100);
		}
		char * pPosId = szResult + nBufPos;
		char * pPosDt = szResult + nBufPos + nLen + 3;
		nBufPos += nLen + nLenD + 6;
		szResult[nBufPos] ='\0';

		memcpy(pPosId + 3, szId, nLen);
		pPosId[2] = (nLen % 10) + '0';  nLen /=10;
	    pPosId[1] = (nLen % 10) + '0';  nLen /=10;
		pPosId[0] = (nLen % 10) + '0'; 

		memcpy(pPosDt + 3, pszData, nLenD);
		pPosDt[2] = (nLenD % 10) + '0';  nLenD /=10;
		pPosDt[1] = (nLenD % 10) + '0';  nLenD /=10;
		pPosDt[0] = (nLenD % 10) + '0'; 
	}


	BSTR bszRes = dwOk ? ::SysAllocStringByteLen(szResult, nBufPos) : NULL;
	
	free((void*)szResult);
	return bszRes;
}

DWORD CEdpsParamList::ParseRequest( LPCSTR szRequest, BOOL bReadRequest)
{
	UINT nLen = lstrlenA(szRequest);
	m_pStringBuffer = (LPSTR) malloc(nLen + 1);

	m_dwCount = 0;
	LPSTR szPtr = m_pStringBuffer;
	while(1) {
		//Connection point not used
		long lPassed = GetNextValue(szRequest, NULL );
		if(!lPassed) break;
		szRequest += lPassed;

		LPSTR pszItemId = szPtr;
		*((DWORD*)szPtr )= 0x5B3A3753; // "S7:["
		szPtr += 4;

		// Connection Name 
		if(! (lPassed = GetNextValue(szRequest, szPtr )) ) break;
		szRequest += lPassed;
		szPtr += lPassed-3;
		*szPtr++ = ']';

		// Item Address 
		if(! (lPassed = GetNextValue(szRequest, szPtr )) ) break;
		szRequest += lPassed;
		szPtr += lPassed-2;

		// Item Type  
		LPSTR pszType = szPtr;
		if(! (lPassed = GetNextValue(szRequest, szPtr )) ) break;
		szRequest += lPassed;
		szPtr += lPassed-2;

		long lParID;
		LPSTR pszValue = NULL;
		if( bReadRequest ) {
			// Parameter ID  
			if(! (lPassed = GetNextValue(szRequest, szPtr )) ) break;
			szRequest += lPassed;
			lParID = atol(szPtr);
			if(lParID<=0) break;

			AddItem(lParID, pszItemId);
		}
		else {
			// Parameter ID  
			lParID = m_dwCount + 1;
			// Value  
			pszValue = szPtr;
			if(! (lPassed = GetNextValue(szRequest, szPtr )) ) break;
			szRequest += lPassed;
			szPtr += lPassed-2;

			// Value Length (not used)
			if(! (lPassed = GetNextValue(szRequest, NULL )) ) break;
			szRequest += lPassed;

			AddItem(lParID, pszItemId, pszType, pszValue);
		}

		LPSTR szPtr= pszType;
		m_dwCount++;
	}

	return m_dwCount;
}

inline BOOL CharNumberic(BYTE ch) { return ch>=0x30 && ch<=0x39; }

long GetNextValue( LPCSTR Point, LPSTR Buff )
{
	if( !(CharNumberic(Point[0]) && CharNumberic(Point[1]) && CharNumberic(Point[2])) )
		return 0;

// Read First 3 char to calc length of Item
	int nLen =  (Point[0]-0x30)*100 + (Point[1]-0x30)*10 + (Point[2]-0x30);

	if(Buff) 
	{
		for(int i = 0; i<nLen; i++) Buff[i] = Point[3+i];
		Buff[nLen] = '\0';
	}

	return 3 + nLen;
}

