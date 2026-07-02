#pragma once
#include "cobject.h"
#include "OPCItems.h"

class COPCGroup :
	public CCObject
{
protected:
// Server Group Object Interfaces
	IUnknown		*m_pUnGroup;
	IOPCItemMgt		*m_pIItems;
	IOPCSyncIO		*m_pISyncIO;

	COPCItemsList   *m_pItems;
protected:
// Local properties
	OPCHANDLE	m_hServer;
	OPCHANDLE	m_hClient;
	DWORD		m_dwUpdateRatio;
	LPTSTR	    m_pszName;        // Name of the group int the Server
	HRESULT		m_hrLast;
	BOOL		m_bActive;
	LONG		m_lTimeBias;
	FLOAT		m_fDeadband;

public:

	// Free all inderfaces prepare to be deleted
	BOOL Detouch();

// Status
protected:
	BOOL UploadStatus(IOPCGroupStateMgt *pIState);

//Add Item Operations  
public:
	// Fist Client shoild decribe List of Items with function NewItem
	// Then Call AddItems to Subsribe Items in the OPC Server
	COPCItem *NewItem(LPCTSTR szItemID, 
				 DWORD dwUserHandle, 
				 VARTYPE vtRequestedDataType=VT_EMPTY, 
				 LPCTSTR szAccessPath=NULL)
	{
		return m_pItems->AddItem(szItemID, dwUserHandle, m_bActive, vtRequestedDataType, szAccessPath);
	}

	// Registrate new items in OPC Server
	HRESULT AddItems();

	// ponter to object COPCItem    
	COPCItem *GetItem(OPCHANDLE dwUserHandle) { return  m_pItems->GetItem(dwUserHandle);}
	COPCItem *GetItem(LPCSTR  szItemId) { 
		USES_CONVERSION;
		return  m_pItems->GetItem(T2W(szItemId));
	}

	// Registrate new items in OPC Server
	DWORD ItemsCount() const { return m_pItems->Count(); }

//Read/Write Operations  
public:
	BOOL InitReading(OPCHANDLE dwUserHandle) { return m_pItems->Choise2Read(dwUserHandle); }
	void InitReading(COPCItem &itm) { m_pItems->Choise2Read(itm); }

	HRESULT ReadItems(BOOL bDevice=TRUE,  BOOL bAllGroup = FALSE);

	BOOL InitWriting(OPCHANDLE dwUserHandle, VARIANT &var ) { return m_pItems->Choise2Write(dwUserHandle, var); }
	void InitWriting(COPCItem &itm, VARIANT &var ) { m_pItems->Choise2Write(itm, var); }

	HRESULT WriteItems();

// Status
public:
	OPCHANDLE GetHandle(BOOL hServer=true) const { return hServer? m_hServer: m_hClient; }
	DWORD UpdateRatio() const { return m_dwUpdateRatio; }
	LPCTSTR Name() const { return m_pszName; }
	BOOL  Active() const { return m_bActive; }
	LONG  TimeBias() const { return m_lTimeBias; }
	FLOAT Deadband() const { return m_fDeadband; }

	HRESULT GetLastError() const { return m_hrLast; }

	BOOL SetStatus( BOOL bActive, DWORD dwUpdateRatio=0, LONG *pdwTimeBias=NULL, FLOAT *pfDeadband=NULL);

public:
	COPCGroup(IUnknown *pIUnk);
	~COPCGroup(void);
};
