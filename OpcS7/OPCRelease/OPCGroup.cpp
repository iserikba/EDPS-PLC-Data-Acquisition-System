#include "StdAfx.h"
#include "opcgroup.h"

COPCGroup::COPCGroup(IUnknown *pIUnk)
{

	m_pszName = NULL;

	m_hrLast = pIUnk->QueryInterface(IID_IUnknown, (void**)&m_pUnGroup);
	
	if(FAILED(m_hrLast) ) 
		return;

	m_hrLast = m_pUnGroup->QueryInterface(IID_IOPCItemMgt, (void**)&m_pIItems);
	if(FAILED(m_hrLast) ) {	
		m_pUnGroup->Release();	m_pUnGroup = NULL;
		return;
	}

	m_hrLast = m_pUnGroup->QueryInterface(IID_IOPCSyncIO, (void**)&m_pISyncIO);
	if(FAILED(m_hrLast) ) {
		m_pUnGroup->Release();	m_pUnGroup = NULL;
		m_pIItems->Release();   m_pIItems  = NULL;
		return;
	}

	IOPCGroupStateMgt *pIState;
	m_hrLast = m_pUnGroup->QueryInterface(IID_IOPCGroupStateMgt, (void**)&pIState);
	if(FAILED(m_hrLast) ) {
		m_pUnGroup->Release();	m_pUnGroup = NULL;
		m_pIItems->Release();   m_pIItems  = NULL;
		m_pISyncIO->Release();	m_pISyncIO = NULL;
		return;
	}

	if(!UploadStatus(pIState)) {
		m_hrLast = S_OK;
	}
	pIState->Release();

	m_pItems = CreateItemsList(15, 15);
}

BOOL COPCGroup::Detouch()
{
	if(m_pUnGroup) { m_pUnGroup->Release();	m_pUnGroup = NULL; }
	if(m_pIItems) { m_pIItems->Release();   m_pIItems  = NULL; }
	if(m_pISyncIO) { m_pISyncIO->Release();	m_pISyncIO = NULL; }
	return true;
}

COPCGroup::~COPCGroup(void)
{
	Detouch();
	m_pItems->Release();
	if(m_pszName) delete [] m_pszName;
	m_pszName = NULL;
}

BOOL COPCGroup::UploadStatus(IOPCGroupStateMgt *pIState)
{

	LPWSTR szName;
	LCID    dw;

	m_hrLast = pIState->GetState(&m_dwUpdateRatio, &m_bActive, &szName, &m_lTimeBias, &m_fDeadband, &dw, &m_hClient ,&m_hServer);

	if(m_pszName) delete [] m_pszName;

	UINT nLen = lstrlenW(szName);
	m_pszName = new TCHAR [nLen + 1];

#ifdef _UNICODE
	lstrcpyW(m_pszName, szName); 
#else
	::WideCharToMultiByte(CP_ACP, 0, szName, -1, m_pszName, nLen + 1, NULL, NULL);   
#endif

	CoTaskMemFree(szName);

	return SUCCEEDED(m_hrLast);
}

BOOL COPCGroup::SetStatus( BOOL bActive, DWORD dwUpdateRatio, LONG *pdwTimeBias, FLOAT *pfDeadband)
{
	IOPCGroupStateMgt *pIState;
	if( FAILED(	m_hrLast = m_pUnGroup->QueryInterface(IID_IOPCGroupStateMgt, (void**)&pIState) ) )
		return false;
	if(!dwUpdateRatio) dwUpdateRatio = m_dwUpdateRatio;

	pIState->SetState(&dwUpdateRatio, &m_dwUpdateRatio, &bActive, pdwTimeBias, pfDeadband, NULL, NULL);

	UploadStatus(pIState);

	pIState->Release();
	return true;
}

HRESULT COPCGroup::AddItems()
{
	OPCITEMDEF *pItemDef;
	OPCITEMRESULT *pAddRes;
	HRESULT    *pHRes;

	DWORD dwCount = m_pItems->GetNewItems(&pItemDef);
	if(!dwCount) return S_FALSE;

	m_hrLast = m_pIItems->AddItems(dwCount, pItemDef, &pAddRes, &pHRes);
	if( SUCCEEDED(m_hrLast) ) 	{
		m_pItems->SetNewItemsRes(dwCount, &pItemDef, &pAddRes, &pHRes);
	}
	else {
		delete [] pItemDef;
	}
	
	return m_hrLast;
}

HRESULT COPCGroup::ReadItems(BOOL bDevice, BOOL bAllGroup )
{
	OPCHANDLE *phItems=NULL;
	OPCITEMSTATE *pItemStat=NULL;
	HRESULT      *pHRes=NULL;

	DWORD dwCount = m_pItems->GetItems2Read(&phItems, bAllGroup);
	if(! dwCount) return S_FALSE;

	m_hrLast = m_pISyncIO->Read( bDevice ? OPC_DS_DEVICE : OPC_DS_CACHE,
							dwCount,
							phItems, &pItemStat, &pHRes);

	m_pItems->SetReadValue(m_hrLast, dwCount, &phItems, &pItemStat, &pHRes);
	
	return m_hrLast;

}

HRESULT COPCGroup::WriteItems()
{
	OPCHANDLE *phItems=NULL;
	VARIANT    *pVar=NULL;
	HRESULT    *pHRes=NULL;

	DWORD dwCount = m_pItems->GetItems2Write(&phItems, &pVar);
	if(! dwCount) return S_FALSE;

	m_hrLast = m_pISyncIO->Write( dwCount, phItems, pVar, &pHRes);
	m_pItems->SetWriteResult(m_hrLast, dwCount, &phItems, &pVar, &pHRes);
	
	return m_hrLast;

}
