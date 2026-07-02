#include "StdAfx.h"
#include "enumopcservers.h"

COpcServerDes::~COpcServerDes() {
	delete [] m_szName;
	delete [] m_szInfo;
	if(m_szNode) delete [] m_szNode;
}


CCObjectList* CEnumOpcServers::GetServers(DWORD *pnServersCount, LPCTSTR szNode)
{
	USES_CONVERSION;
    
	if( pnServersCount ) *pnServersCount = 0;

	MULTI_QI mqi;
	memset( &mqi, 0, sizeof(mqi) );
	mqi.pIID = &IID_IOPCServerList;

	DWORD cltx = CLSCTX_INPROC_SERVER  | CLSCTX_LOCAL_SERVER ; 
	COSERVERINFO *pServerInfo = NULL;

	if(szNode) {
		cltx = CLSCTX_REMOTE_SERVER;
		pServerInfo = new COSERVERINFO;
		memset( pServerInfo, 0, sizeof(COSERVERINFO) );
        pServerInfo->pwszName = T2W(szNode);
	}

	m_hrLast = ::CoCreateInstanceEx(CLSID_OpcServerList,
							NULL,
							cltx, 
							pServerInfo,
							1, &mqi);

    if(pServerInfo) delete pServerInfo;

    if( FAILED( m_hrLast) ) return NULL;

	if( FAILED(mqi.hr) ) {
		m_hrLast = mqi.hr;
		return NULL;
	}

	IOPCServerList *piServList = (IOPCServerList*) mqi.pItf;

	IEnumGUID * piEnim;

	IID *pIIDs = new IID [3];
	pIIDs[0] = IID_CATID_OPCDAServer10;
	pIIDs[1] = IID_CATID_OPCDAServer20;
	pIIDs[2] = IID_CATID_OPCDAServer30; 

	m_hrLast = piServList->EnumClassesOfCategories(3, pIIDs, 0, NULL, &piEnim);
    delete [] pIIDs;
    
	if( FAILED(m_hrLast) ) {
		piServList->Release();
		return NULL;
	}

	CCObjectList *pSvrsList =  new CCObjectList;
	pSvrsList->AddRef();
   
	while(1) {
		GUID gd;
		DWORD dw;
		piEnim->Next(1, &gd, &dw);
		if( !dw ) break;
		LPOLESTR oszName=NULL, oszInfo=NULL;

		if( SUCCEEDED(piServList->GetClassDetails( gd, &oszName, &oszInfo ) ) ) {
			COpcServerDes *pSvrNew = new COpcServerDes;
			pSvrsList->Add( pSvrNew );

			pSvrNew->m_GUID = gd;
			if( szNode ) 
			{
				pSvrNew->m_szNode = new TCHAR [ _tcslen(szNode) + 1 ];
				_tcscpy( pSvrNew->m_szNode, szNode);
			}
			else
				pSvrNew->m_szNode = NULL;
			
			if( oszName ) { 
				pSvrNew->m_szName = new TCHAR [ wcslen( (LPCWSTR) oszName) +1 ];
				_tcscpy( pSvrNew->m_szName, OLE2T(oszName));
				CoTaskMemFree( oszName );
			}
			else {
				pSvrNew->m_szName = new TCHAR;
				pSvrNew->m_szName[0] = 0;
			}

			if( oszInfo ) {
				pSvrNew->m_szInfo = new TCHAR [ wcslen( (LPCWSTR) oszInfo) +1 ];
				_tcscpy( pSvrNew->m_szInfo, OLE2T(oszInfo));
				CoTaskMemFree( oszInfo );
			}
			else {
				pSvrNew->m_szInfo = new TCHAR;
				pSvrNew->m_szInfo[0] = 0;
			}
		}
	}

	piEnim->Release();
	piServList->Release();

	if( pnServersCount ) *pnServersCount = pSvrsList->Count();
	return pSvrsList;
}

HRESULT CEnumOpcServers::GetLastError()
{
	return m_hrLast;
}


