#pragma once

#include "cobject.h"
//===========================================================================
// Class COpcServerDes implements OPC Servers List 
class COpcServerDes : public CCObject
{
friend class CEnumOpcServers;
protected: 
	COpcServerDes(){}
    ~COpcServerDes();

public: // Server information
	GUID   m_GUID;
	LPTSTR m_szName;
	LPTSTR m_szInfo;
	LPTSTR m_szNode;

};

//===========================================================================
// Class CEnumOpcServers implements gethering information about 
// OPC Servers List in selected node
// Connection to OPC Enumeration Folder, Unumerate list of OPC Servers on the Node
// 
class CEnumOpcServers : public CCObject
{
private: 
	HRESULT m_hrLast;

public: // Constructur
	CEnumOpcServers(void) { m_hrLast=S_OK; }

public: // 
	CCObjectList* GetServers( DWORD *pnServersCount = NULL, LPCTSTR szNode = NULL );

	HRESULT GetLastError();

public: // Destroyer

//	~CEnumOpcServers(void) {}
};
