// OpcS7.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "OpcS7.h"
#include ".\OPCRelease\OPCDAServer.h"
#include "EdpsParams.h"

COpcServer *g_pOpcServer;
BOOL g_bMainServer;
COpcServer *g_pSndOpcServer;
BOOL g_bSndServer;
BOOL g_bInitOK=FALSE;
BOOL g_bSndServerName; 

LPCSTR g_szRdGroupName = "EDPS.ReadGp";
LPCSTR g_szWrGroupName = "EDPS.WriteGp";

COPCGroup *GetOPCGroup(BOOL bMainServer, LPCSTR szName);
BOOL ConnectToServer();

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	BOOL bRet = true;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetIniValues((HMODULE)hModule);
		bRet = SUCCEEDED( ::CoInitialize( NULL ) );
		COpcServer::m_pszClientName = szClientName;
		if(bRet) 
		{
			g_pOpcServer = new COpcServer;
			g_pSndOpcServer = new COpcServer;
			g_bMainServer = g_bSndServer = false;
		}
		g_bInitOK = bRet;
		str2log("### DllMain Initilize with code %u.", bRet);
		break;
	case DLL_PROCESS_DETACH:
		SAPI_Shut();
		::CoUninitialize();
		break;
	}
    return bRet;
}

// Shutdown OPC Client
OPCS7_API long CCONV SAPI_Shut( void )
{
	if( g_bMainServer ) {
		g_pOpcServer->Disconnect();
		delete g_pOpcServer;
		g_pOpcServer = NULL;
		g_bMainServer = false;
	}

	if( g_bSndServer ) {
		g_pSndOpcServer->Disconnect();
		delete g_pSndOpcServer;
		g_pSndOpcServer = NULL;
		g_bSndServer = false;
	}

	g_bInitOK = false;
	::CoFreeUnusedLibraries();
	str2log("### OPC was shuted down");
	return 0;
}


// Read Item
OPCS7_API long CCONV SAPI_Read ( BSTR bsCP, BSTR bsConn, BSTR bsItem, BSTR bsType, BSTR * bsData )
// bsCP - Communication point name is not used in OPC
// bsConn - Connection name 
// bsItem - Item Address. OPC Item is a combine of strings bsConn and bsItem 
//   in joining up <S7:[bsConn]bsItem>
// bsType - Type of variable is not used 
// bsData - Result value
{
	::SysFreeString(*bsData);
	*bsData = NULL;
	COPCGroup *pReadGroup = NULL;
	CEdpsParamList Params;
	int nRes = 0;
	DWORD dwExpCode;
	int n;
	__try {
 	  __try {

			str2log("### Start of SAPI_Read.");

			pReadGroup = GetOPCGroup(true, g_szRdGroupName);
			if(!pReadGroup ) {
				nRes = -1;
				__leave;
			}

			nRes = Params.SingleItem( pReadGroup, (LPCSTR) bsConn, (LPCSTR) bsItem );

			if(nRes<0) {
				str2log("--- Error adding Item to the Group" );
				nRes = -4;
				__leave;
			}

			*bsData = Params.ReadValue(pReadGroup);

			if(*bsData ) {
				nRes = 0;
				str2log("### SAPI_Read successed.");
			}
			else {
				str2log("--- Read failed");
				nRes = -5;
			}
		}
		__finally {
			if( pReadGroup ) pReadGroup->Release();
		}
	}
	__except(dwExpCode=GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		n = MyIni.iTrace;
		MyIni.iTrace = 1;
		str2log("--- SAPI_Read Exeption with Error %X", dwExpCode );
		MyIni.iTrace = n;
		nRes = -10;
	}

	return nRes;
}

// Write Data to Item
OPCS7_API long CCONV SAPI_Write ( BSTR bsCP, BSTR bsConn, BSTR bsItem, BSTR bsType, BSTR bsData )
// bsCP - Communication point name is not used in OPC
// bsConn - Connection name 
// bsItem - Item Address. OPC Item is a combine of strings bsConn and bsItem 
//   in joining up <S7:[bsConn]bsItem>
// bsType - Type of variable is not used 
// bsData - Value to write
{
	CEdpsParamList Params;
	COPCGroup *pWriteGroup = NULL;
	int nRes = 0;

	DWORD dwExpCode;
	int n;
	__try {
 	  __try {
			str2log("### Start of SAPI_Write.");

			pWriteGroup = GetOPCGroup(true, g_szWrGroupName);
			if(!pWriteGroup ){
				nRes = -1;
				__leave;
			}

			nRes = Params.SingleItem( pWriteGroup, (LPCSTR) bsConn, (LPCSTR) bsItem, (LPCSTR) bsType, (LPCSTR) bsData );
			if(nRes<0) {
				str2log("--- Error adding Item to the Group" );
				nRes = -4;
				__leave;
			}

			nRes = Params.WriteValue(pWriteGroup);
			if(! nRes ) {
				str2log("--- Write failed");
				nRes = -6;
			}
			else {
				nRes = 0;
			    str2log("### SAPI_Write successed.");
			}
	    }
		__finally {
			if( pWriteGroup ) pWriteGroup->Release();
		}
	}
	__except(dwExpCode=GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		n = MyIni.iTrace;
		MyIni.iTrace = 1;
		str2log("--- SAPI_Read Exeption with Error %X", dwExpCode );
		MyIni.iTrace = n;
		nRes = -10;
	}

	return nRes;	
}

// Read Group
OPCS7_API long CCONV SAPI_MRead ( BSTR bsParStr, BSTR *bsData )
{
	::SysFreeString(*bsData);
	*bsData = NULL;
	COPCGroup *pMrGroup=NULL;
	CEdpsParamList Params;
	int nRes =0;
	DWORD dwRead=0;
	DWORD dwExpCode;

	int n;
	__try {
 	  __try {
		str2log("### Start of SAPI_MRead.");


		nRes = Params.ParseRequest((LPCSTR) bsParStr, TRUE); 
		if( nRes ) 
			str2log("### Parsing succeeded, conn. qty = %d.", nRes);
		else {
			str2log("--- No Parameters Exit with code -1");
			nRes = -1;
			__leave;
		}

		LPCTSTR szGroupName  = Params.GetListName(); 

		pMrGroup = GetOPCGroup(true, szGroupName);
		free((void*)szGroupName);

		if(!pMrGroup ) {
			nRes = -2;
			__leave;
		}

		nRes = Params.FindOrAddItemsToGroup( pMrGroup, true );
		if(nRes>0) 
			str2log("### %u Items add to the Group", nRes );

		if(nRes==-1) 
			str2log("--- Adding some Items to the Group failed" );

		if(nRes==-2) {
			str2log("--- Error adding Items to the Group" );
			nRes = -4;
			__leave;
		}

		HRESULT hr = pMrGroup->ReadItems(true);
		if( FAILED(hr) ) {
			str2log("--- Group Read failed with Error  %X", hr );
			nRes = -5;
			__leave;
		}

		if( hr == S_FALSE)  
			str2log("--- Not all member of Group was read");

		*bsData =  Params.GetMReadResult(dwRead);
		if( dwRead ) {
			nRes = 0;
			str2log("### SAPI_MRead successed.");
		}else {
			nRes = -6;
			str2log("--- SAPI_MRead failed. Can't raed data from server.");
		}

	  }
	  __finally {
		  if( pMrGroup ) pMrGroup->Release();
	  }
	}
	__except(dwExpCode=GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		n = MyIni.iTrace;
		MyIni.iTrace = 1;
		str2log("--- SAPI_MRead Exeption with Error %X", dwExpCode );
		MyIni.iTrace = n;
		nRes = -10;
	}

	return nRes;

}

// Write Group
long CCONV SAPI_MWrite ( BSTR bsParStr )
{
	str2log("### Start of SAPI_MWrite.");

	CEdpsParamList Params;

	int nRes = Params.ParseRequest((LPCSTR) bsParStr, false); 
	if( nRes ) 
		str2log("### Parsing succeeded, conn. qty = %d.", nRes);
	else {
		str2log("--- No Parameters Exit with code -1");
		return -1;
	}

	LPCTSTR szGroupName  = Params.GetListName(); 

	COPCGroup *pMwGroup = GetOPCGroup(true, szGroupName);
	free((void*)szGroupName);

	if(!pMwGroup )
		return -2;

	nRes = Params.FindOrAddItemsToGroup( pMwGroup, false );
	if(nRes>0) 
		str2log("### %u Items add to the Group", nRes );

	if(nRes==-1) 
		str2log("--- Adding some Items to the Group failed" );

	if(nRes==-2) {
		str2log("--- Error adding Items to the Group" );
		pMwGroup->Release();
		return -4;
	}

	nRes = 0;
	HRESULT hr = pMwGroup->WriteItems();
	if( FAILED(hr) ) {
		str2log("--- Group Write failed with Error  %X", hr );
		nRes = -5;
	}

	if( hr == S_FALSE) {
		str2log("--- Not all member of Group was written");
		nRes = -6;
	}

	pMwGroup->Release();
	return nRes;
}

COPCGroup *GetOPCGroup(BOOL bMainServer, LPCSTR szName)
{
	if(!g_bInitOK) {
		str2log("--- OPC Not init.");
		return NULL;
	}

	COpcServer *pServer=NULL;
	if(bMainServer) {
		str2log("### Local Server Selected.");
		if(g_bMainServer) 
			pServer = g_pOpcServer;
		else {
			ConnectToServer();
			if(g_bMainServer)
				pServer = g_pOpcServer;
		}	
	}
	else  {
		str2log("### Remote Server Selected.");
		if( g_bSndServer ) 
			pServer = g_pSndOpcServer;
	}

	if(!pServer) {
		str2log("--- Can't initialize the OPC server.");
		return NULL;
	}

	str2log("### Selecting OPC Group %s", szName);

	COPCGroup *pGroup = pServer->GetGroupByName(szName);
	if(pGroup) {
		str2log("### Group is found");
	}
	else {
		pGroup = g_pOpcServer->AddGroup(szName, MyIni.nRefreshRatio, false);
		str2log("### Group Created");
	}

	if(!pGroup) {
		str2log("--- Adding the Group failed with Error %X", pServer->GetLastError() );
		return NULL;
	}

	return pGroup;
}

BOOL ConnectToServer()
{
static LPCSTR szFail = "--- Connecting failed with code %X";

	if(!g_bMainServer) {	
		str2log("### Connect to Local OPC Server %s ", MyIni.S7OpcServer);
		g_bMainServer = g_pOpcServer->Connect(MyIni.S7OpcServer);
		if(!g_bMainServer)
			str2log(szFail, g_pOpcServer->GetLastError() );
	}

	// First call
	if(!g_bSndServer && !g_bInitOK && _tcslen(MyIni.EmergencyServerNode)>0) {
		str2log("### Connect to OPC Server %s at node %s", MyIni.S7OpcServer, MyIni.EmergencyServerNode);
		g_bSndServer = g_pSndOpcServer->Connect(MyIni.S7OpcServer, MyIni.EmergencyServerNode);
		if(!g_bSndServer)
			str2log(szFail, g_pSndOpcServer->GetLastError() );
	}
	return g_bMainServer || g_bSndServer; 
}
