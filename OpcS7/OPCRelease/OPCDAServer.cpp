#include "StdAfx.h"
#include "OPCDAServer.h"

#define SERV_IIF_COUNT 3

LPCTSTR COpcServer::m_pszClientName = NULL;

//===========================================================================
// Class COpcAdviseSink implements interfaces IUnknown and IOPCShutdown 
// from OPC spec 2.5A
// It's intetend to get Event on Shutdown Request from Server Object
class COpcShutdownAdviseSink :
	public IOPCShutdown
{
public:
	//
	// IUnknown
	//    
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
    
    virtual ULONG STDMETHODCALLTYPE AddRef( void);
    
    virtual ULONG STDMETHODCALLTYPE Release( void);

public:
	//
	// IOPCShutdown
	//    
    virtual HRESULT STDMETHODCALLTYPE ShutdownRequest( 
        /* [string][in] */ LPCWSTR szReason);

public:
	COpcServer *m_pServParent;
	LONG        m_cRef;

public:
	COpcShutdownAdviseSink(void);
	~COpcShutdownAdviseSink(void);
};

HRESULT STDMETHODCALLTYPE COpcShutdownAdviseSink::QueryInterface(const IID& iid, void** ppv)
{   
     if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this); 
	}
	else if (iid == IID_IOPCShutdown)
	{
		*ppv = static_cast<IOPCShutdown*>(this); 
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE COpcShutdownAdviseSink::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG STDMETHODCALLTYPE COpcShutdownAdviseSink::Release() 
{
	long ret = InterlockedDecrement(&m_cRef);
	if (ret == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

HRESULT STDMETHODCALLTYPE COpcShutdownAdviseSink::ShutdownRequest(LPCWSTR szReason)
{
	if(m_pServParent)
	{
		USES_CONVERSION;	
		m_pServParent->OnShutdownRequest(W2T(szReason));
	}
	return S_OK;
}

COpcShutdownAdviseSink::COpcShutdownAdviseSink(void)
{
	m_cRef = 0;
	m_pServParent = NULL;
}

COpcShutdownAdviseSink::~COpcShutdownAdviseSink(void)
{
	if( m_pServParent ) {
		m_pServParent->m_pObjShutdown = NULL;
	}
}


COpcServer::COpcServer(void)
{
	m_pIOpcServer = NULL;
	m_pIOpcCommon = NULL;
	m_pConnShutdown = NULL;
	m_pObjShutdown = NULL;
	m_hrLast = S_OK;
	m_lidServer = LOCALE_SYSTEM_DEFAULT;
	m_pGroupList = NULL;
	m_pszwNode = NULL;
	m_dwAdviseCookie = -1;
}

COpcServer::~COpcServer(void)
{
	Disconnect();
}

BOOL COpcServer::Disconnect()
{
	if(m_pIOpcServer) {
		// Free Sink mini-object
		if( m_pConnShutdown ) {
			if( m_pObjShutdown && m_dwAdviseCookie!=-1) {
				m_pConnShutdown->Unadvise(m_dwAdviseCookie);
				m_pObjShutdown->Release();
			}
			m_pConnShutdown->Release();
		}
 
		RemoveGroups();

		if( m_pIOpcServer ) m_pIOpcServer->Release();	
		if( m_pIOpcCommon ) m_pIOpcCommon->Release();

		m_pIOpcServer = NULL;
		m_pIOpcCommon = NULL;
		m_pConnShutdown = NULL;

		m_hrLast = S_OK;
		memset( &m_idServer, 0, sizeof(CLSID) );
		if( m_pszwNode ) delete [] m_pszwNode; 
		m_pszwNode = NULL;

		return TRUE;
	}
	else
		return FALSE;
}

BOOL COpcServer::Connect( const CLSID &id , LPCTSTR szNode)
{

	// Free server if it's already connected
	Disconnect();

	// Fill the record for query-multi-interfaces
	MULTI_QI *pMQI = new MULTI_QI[SERV_IIF_COUNT];
	memset( pMQI, 0, sizeof(MULTI_QI)*SERV_IIF_COUNT );
	pMQI[0].pIID = &IID_IOPCCommon;
    pMQI[1].pIID = &IID_IOPCServer;
	pMQI[2].pIID = &IID_IConnectionPointContainer;

	DWORD cltx = CLSCTX_INPROC_SERVER  | CLSCTX_LOCAL_SERVER ; 
	COSERVERINFO *pServerInfo = NULL;

	if(szNode) {
		cltx = CLSCTX_REMOTE_SERVER;
		pServerInfo = new COSERVERINFO;
		memset( pServerInfo, 0, sizeof(COSERVERINFO) );
		UINT nLen = lstrlen(szNode);
		m_pszwNode = new WCHAR [ nLen + 1 ]; 
#ifdef _UNICODE
		lstrcpyW(m_pszwNode, szNode);
#else
		m_pszwNode[0] = '\0';
		::MultiByteToWideChar(CP_ACP, 0, szNode, -1, m_pszwNode, nLen + 1);
#endif
        pServerInfo->pwszName = m_pszwNode;
	}
	else
		m_pszwNode = NULL;

	// Create Server object and retrive for it a list of interfaces pMQI
	m_hrLast = ::CoCreateInstanceEx(id,
							NULL,
							cltx, 
							pServerInfo,
							SERV_IIF_COUNT, pMQI);


    if(pServerInfo) delete pServerInfo;

	if( FAILED( m_hrLast) && m_hrLast != E_NOINTERFACE) {
		delete pMQI;
		return FALSE;
	}

	// Story interfaces
	if( FAILED(pMQI[0].hr) ) m_hrLast = pMQI[0].hr;
	else m_pIOpcCommon = (IOPCCommon*) pMQI[0].pItf;

	if( FAILED(pMQI[1].hr) ) m_hrLast = pMQI[1].hr;
	else m_pIOpcServer = (IOPCServer*) pMQI[1].pItf;

	IConnectionPointContainer* pIConnectionPointContainer=NULL;
	if( SUCCEEDED(pMQI[2].hr) ) 
		pIConnectionPointContainer = (IConnectionPointContainer*) pMQI[2].pItf;

	delete pMQI;

	// Whole done well
    if( FAILED( m_hrLast) ) 
	{
		if( m_pIOpcCommon) {
			m_pIOpcCommon->Release();
			m_pIOpcCommon = NULL;
		}
		if( m_pIOpcServer) {
			m_pIOpcServer->Release();
			m_pIOpcServer = NULL;
		}
		if( pIConnectionPointContainer) 
			pIConnectionPointContainer->Release();

		return FALSE;
	}
	
	m_idServer = id;
	
	// Set the client name for server
	USES_CONVERSION;
	m_pIOpcCommon->SetClientName( T2W(m_pszClientName) );
	// Set default locale ID
	m_pIOpcCommon->SetLocaleID(m_lidServer);
	
	// Create an Advise Sink for the Event on Server Shutdown
	if( pIConnectionPointContainer) {
		// Take the connection point for IID_IOPCShutdown
		m_hrLast = pIConnectionPointContainer->FindConnectionPoint(IID_IOPCShutdown, &m_pConnShutdown);
		if( SUCCEEDED(m_hrLast) ) 
		{
			// Create sink mini-object
			m_pObjShutdown = new COpcShutdownAdviseSink;
			m_pObjShutdown->AddRef();
			m_pObjShutdown->m_pServParent = this;

			// Retrive IUnknown from sink object
			IUnknown *pUnk;
			m_pObjShutdown->QueryInterface(IID_IUnknown, (void**)&pUnk);
			// Advise Sink
			m_hrLast = m_pConnShutdown->Advise(pUnk, &m_dwAdviseCookie);
			// Release IUnknown
			pUnk->Release();

			// If advising failed release mini-object and connection point
			if( FAILED(m_hrLast) ) {
				m_pObjShutdown->Release();
				m_pConnShutdown->Release();
				m_pConnShutdown = NULL;
			}
		}
		// Release connection point container
		pIConnectionPointContainer->Release();
	}
	
	m_hrLast = S_OK;
	return TRUE;

}



BOOL COpcServer::Connect( LPCTSTR szServerID , LPCTSTR szNode)
{
	USES_CONVERSION;

	CLSID id;
	HRESULT hr = CLSIDFromProgID(T2W(szServerID), &id);
	if( FAILED( hr) ) return FALSE;
	else return COpcServer::Connect( id, szNode);
}

BOOL COpcServer::GetStatus(OPCSERVERSTATUS &status,  LPTSTR szBufVendorInfo, UINT nBufSize) { 

	if(!m_pIOpcServer) return FALSE;

	OPCSERVERSTATUS *pStatus;
	m_hrLast= m_pIOpcServer->GetStatus( &pStatus);
	if( FAILED(m_hrLast) ) return FALSE;
	if(szBufVendorInfo && nBufSize) {
		UINT nLen = lstrlenW(pStatus->szVendorInfo);
		if(nBufSize < nLen) 
			nLen = nBufSize-1;

#ifdef _UNICODE
		lstrcpynW(szBufVendorInfo, pStatus->szVendorInfo, nBufSize <= nLen ? nBufSize : nLen + 1); 
#else
		::WideCharToMultiByte(CP_ACP, 0, pStatus->szVendorInfo, nLen, szBufVendorInfo, nBufSize, NULL, NULL);   
#endif

	}
	CoTaskMemFree(pStatus->szVendorInfo);
	pStatus->szVendorInfo = NULL;
	status = *pStatus;
	CoTaskMemFree(pStatus);

	return TRUE;
}

void COpcServer::OnShutdownRequest(LPCTSTR szReason) { 

}

COPCGroup * COpcServer::AddGroup(LPCTSTR		pszName, 
				DWORD		dwRequestedRatio, 
				BOOL		bActive, 
				LONG		*plTimeBias,
				FLOAT		*pfPercentDeadband)
{

	if(!m_pIOpcServer) return FALSE;

	if(!m_pGroupList) {
		m_pGroupList = new CCObjectList;
		m_pGroupList->AddRef();
	}

	OPCHANDLE	hServerHandle, hUserHandle = m_pGroupList->Count() + 1;
	DWORD  dwUpdateRatio;
	IUnknown *pIUnk;
#ifndef _UNICODE
	UINT __nLen = lstrlen(pszName) + 1;
	LPWSTR __szName = (LPWSTR) malloc( __nLen * sizeof(WCHAR) );
	::MultiByteToWideChar(CP_ACP, 0, pszName, -1, __szName, __nLen);
#else
	LPWSTR __szName = pszName;
#endif



	m_hrLast = m_pIOpcServer->AddGroup(
		__szName,
		bActive,
		dwRequestedRatio,
		hUserHandle,
		plTimeBias,
		pfPercentDeadband,
		m_lidServer,
		&hServerHandle,
		&dwUpdateRatio,
		IID_IUnknown,
		&pIUnk);

#ifndef _UNICODE
	free(__szName);
#endif

	if( FAILED(m_hrLast) ) return false;

	COPCGroup *pNewGroup = new COPCGroup(pIUnk);
	pNewGroup->AddRef();
	pIUnk->Release();

	if( FAILED(m_hrLast=pNewGroup->GetLastError() )) {
		pNewGroup->Release();
		return NULL;
	}

	m_pGroupList->Add( static_cast<CCObject*>(pNewGroup) );
	return pNewGroup;
}

COPCGroup *COpcServer::GetGroupByName(LPCTSTR pszName, UINT *pnPos)
{
	if(!m_pGroupList) return NULL;

	UINT nPos = 0;
	for(COPCGroup* pNext = static_cast<COPCGroup*> (m_pGroupList->First()); 
		pNext;
		pNext = static_cast<COPCGroup*> (m_pGroupList->Next()) ) 
	{
		if( _tcscmp(pNext->Name(), pszName)==0 ) break;
		pNext->Release();
		nPos++;
	}
	if(pnPos) *pnPos= nPos;
	return pNext;
}

BOOL COpcServer::RemoveGroup(LPCTSTR pszName)
{
	UINT  nPos;
	COPCGroup *pGroup = GetGroupByName(pszName, &nPos);
	if(! pGroup ) return true;

	pGroup->Detouch();
	m_hrLast = m_pIOpcServer->RemoveGroup(pGroup->GetHandle(), false );
	if(m_hrLast==OPC_S_INUSE) {
		m_hrLast = m_pIOpcServer->RemoveGroup(pGroup->GetHandle(), true );
	}
	pGroup->Release();
	if( FAILED(m_hrLast) ) return false;
	m_pGroupList->Remove(nPos);
	
	return true;
}

void  COpcServer::UploadGroups()
{
	IEnumUnknown *pIEnum=NULL;
	HRESULT hr = m_pIOpcServer->CreateGroupEnumerator(OPC_ENUM_PRIVATE, IID_IEnumUnknown, (LPUNKNOWN*)&pIEnum);

	if( FAILED(hr) || hr==S_FALSE) {
		if(pIEnum) pIEnum->Release();
		return;
	}

	if(!m_pGroupList) {
		m_pGroupList = new CCObjectList;
		m_pGroupList->AddRef();
	}

	IUnknown *pIUnk;
	ULONG     nFetched;

	while( SUCCEEDED(pIEnum->Next(1, &pIUnk, &nFetched)) && nFetched==1 )
	{
		COPCGroup *pGroup = new COPCGroup(pIUnk);
		pGroup->AddRef();
		
		BOOL bFound = FALSE;
		for(COPCGroup* pNext = static_cast<COPCGroup*> (m_pGroupList->First()); 
			pNext;
			pNext = static_cast<COPCGroup*> (m_pGroupList->Next()) ) 
		{
			bFound = pNext->GetHandle() == pGroup->GetHandle();
			pNext->Release();
			if( bFound ) break;
		}

		if(!bFound) {
			COPCGroup *pNewGroup = new COPCGroup(pIUnk);
			pNewGroup->AddRef();
			if( SUCCEEDED(pNewGroup->GetLastError()) ) 
				m_pGroupList->Add( static_cast<CCObject*>(pNewGroup) );
			
			pNewGroup->Release();
		}

		pIUnk->Release();		
	}
	
	pIEnum->Release();

}

void  COpcServer::RemoveGroups()
{
	if(!m_pGroupList) return;

	for(COPCGroup* pNext = static_cast<COPCGroup*> (m_pGroupList->First()); 
			pNext;
			pNext = static_cast<COPCGroup*> (m_pGroupList->Next()) )
	{
		pNext->Detouch();
		m_pIOpcServer->RemoveGroup(pNext->GetHandle(), false );
		pNext->Release();
	}

	m_pGroupList->Release();
	m_pGroupList = NULL;

}


