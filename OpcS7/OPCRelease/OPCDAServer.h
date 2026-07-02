#pragma once

#include "opcgroup.h"

//===========================================================================
// Class COpcAdviseSink implements interfaces IUnknown and IOPCShutdown 
// from OPC spec 2.5A
// It's intetend to get Event on Shutdown Request from Server Object
class COpcShutdownAdviseSink;

//===========================================================================
// Class COpcServer implements acces to OPC Server 
class COpcServer
{
	friend class COpcShutdownAdviseSink;

protected: 
	// Server Information
	CLSID   m_idServer;
	LPWSTR m_pszwNode;
	// Last Call Result
	HRESULT m_hrLast;
	// Server Locale
	LCID    m_lidServer;
	// OPC Server Interfaces
	IOPCCommon *m_pIOpcCommon;
	IOPCServer *m_pIOpcServer;
	IConnectionPoint *m_pConnShutdown;
	// Advise Sink mini-object
	COpcShutdownAdviseSink *m_pObjShutdown;
	DWORD m_dwAdviseCookie;

protected: // Group List
	CCObjectList   *m_pGroupList;

public:
	static LPCTSTR m_pszClientName;

public: // Connect and disconnect object to OPC server
	BOOL Connect( const CLSID &id , LPCTSTR szNode = NULL);
	BOOL Connect( LPCTSTR szServerID , LPCTSTR szNode = NULL);

	BOOL Disconnect();

public: // Server Items' Groups Managing
	COPCGroup *AddGroup(LPCTSTR		pszName, 
					DWORD		dwRequestedRatio=0, 
					BOOL		bActive=TRUE, 
					LONG		*plTimeBias=NULL,
					FLOAT		*pfPercentDeadband=NULL );

	COPCGroup *GetGroupByName(LPCTSTR pszName, UINT *pnPos=NULL);

	BOOL RemoveGroup(LPCTSTR pszName);

public: 
	// Upload Disconnected Groups from Server
	void  UploadGroups();  
	// Remove all Groups
	void  RemoveGroups();  

public: // Status
	HRESULT GetLastError() const { return m_hrLast; }

	BOOL GetLocale(LCID &loc);
	BOOL SetLocale(LCID loc);
	BOOL GetStatus(OPCSERVERSTATUS &status, LPTSTR szBufVendorInfo, UINT nBufSize);

private: // Event Handler
	virtual void OnShutdownRequest(LPCTSTR szReason);

public:
	COpcServer(void);
	~COpcServer(void);
};


inline BOOL COpcServer::GetLocale(LCID &loc) { 
	if(m_pIOpcCommon) 
		return SUCCEEDED(m_hrLast= m_pIOpcCommon->GetLocaleID(&loc));
	else
		return false;
}

inline BOOL COpcServer::SetLocale(LCID loc) { 
	if(m_pIOpcCommon && SUCCEEDED(m_hrLast= m_pIOpcCommon->SetLocaleID(loc))) {
		m_lidServer = loc;
		return true;
	}
	else
		return false;
}


