#include "StdAfx.h"
#include "opcitems.h"


//===========================================================================
// COPCItem functions 
void COPCItem::Empty()
{
	if(m_szwItemID) {
		delete [] m_szwItemID;
		m_szwItemID = NULL;
	}

	if(m_szwAccessPath) {
		delete [] m_szwAccessPath;
		m_szwAccessPath = NULL;
	}

	::VariantClear( &m_vDataValue );

}

void COPCItem::Init(LPWSTR szItemID, LPWSTR strAccessPath, BOOL bActive, OPCHANDLE hClient, VARTYPE vtRequestedDataType)
{
	m_szwItemID = szItemID;
	m_szwAccessPath = strAccessPath;
	m_bActive = bActive; 
	m_vtRequestedDataType = vtRequestedDataType;
	m_hClient = hClient; 

	m_wQuality = 0; 
	m_pUserData = NULL;
	:: VariantInit(	&m_vDataValue );
	m_hrOperation = E_NOTIMPL;

}


//===========================================================================
// COPCItemsList IUnknown methods
HRESULT STDMETHODCALLTYPE COPCItemsList::QueryInterface(const IID& iid, void** ppv)
{   
     if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this); 
	}
	else if (iid == IID_IOPCDataCallback)
	{
		*ppv = static_cast<IOPCDataCallback*>(this); 
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE COPCItemsList::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG STDMETHODCALLTYPE COPCItemsList::Release() 
{
	long ret = InterlockedDecrement(&m_cRef);
	if (ret == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

//===========================================================================
// COPCItemsList IOPCDataCallback methods
HRESULT STDMETHODCALLTYPE COPCItemsList::OnDataChange( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors)
{

	return E_NOTIMPL;
}
    
HRESULT STDMETHODCALLTYPE COPCItemsList::OnReadComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors)
{
	return E_NOTIMPL;
}

    
HRESULT STDMETHODCALLTYPE COPCItemsList::OnWriteComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMastererr,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *pClienthandles,
        /* [size_is][in] */ HRESULT *pErrors)
{
	return E_NOTIMPL;
}
    
HRESULT STDMETHODCALLTYPE COPCItemsList::OnCancelComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup)
{
	return E_NOTIMPL;
}

//===========================================================================
// COPCItemsList funtions

COPCItemsList::~COPCItemsList(void)
{
	for(UINT n = 0 ; n< m_nCount; )
		m_pItemsList[n++].Empty();
	free( m_pItemsList );
}

COPCItemsList * CreateItemsList( UINT nSize, UINT nGrowBy)
{
	if(nSize<1) nSize=1;
	if(nGrowBy<1) nGrowBy=1;
	COPCItemsList *pNewList = new COPCItemsList(nSize, nGrowBy);
	pNewList->AddRef();
	return pNewList;
}

COPCItem * COPCItemsList::AddItem(LPCTSTR szItemID, DWORD dwUserHandle, BOOL bActive, VARTYPE vtRequestedDataType, LPCTSTR szAccessPath)
{

	if(m_nCount == m_nSize ) {
		m_pItemsList = (COPCItem*) realloc(m_pItemsList, sizeof(COPCItem) *( m_nSize += m_nGrowBy));
	}

	UINT nLen;
	LPWSTR szwIID = new WCHAR [ nLen = lstrlen(szItemID) +1 ];

#ifdef _UNICODE
	lstrcpyW(szwIID, szItemID);
#else
	szwIID[0] = '\0';
	::MultiByteToWideChar(CP_ACP, 0, szItemID, -1, szwIID, nLen);
#endif

	LPWSTR szwAP = NULL;
	if(szAccessPath)	{
		szwAP = new WCHAR [ nLen = lstrlen(szAccessPath) +1 ];
#ifdef _UNICODE
		lstrcpyW(szwAP, szAccessPath);
#else
		szwAP[0] = '\0';
		::MultiByteToWideChar(CP_ACP, 0, szAccessPath, -1, szwAP, nLen );
#endif
	}

	m_pItemsList[m_nCount].Init(szwIID, szwAP, bActive, dwUserHandle, vtRequestedDataType);
	m_pItemsList[m_nCount].ItemNew();

	return &m_pItemsList[m_nCount++];
}

DWORD COPCItemsList::GetNewItems(OPCITEMDEF **ppItemDef)
{

	UINT nNewCount = 0;
	for(UINT n = 0 ; n< m_nCount; n++) 
		if( m_pItemsList[n].New() ) nNewCount++;

	if(!nNewCount) {
		*ppItemDef = NULL;
		return 0;
	}

	OPCITEMDEF *pItemDef = *ppItemDef = new OPCITEMDEF [nNewCount];
	memset(pItemDef, 0,  sizeof(OPCITEMDEF)*nNewCount );

	nNewCount = 0;
	for(UINT n = 0 ; n< m_nCount; n++) 
		if(m_pItemsList[n].New() ) 
		{
			pItemDef[nNewCount].bActive = m_pItemsList[n].m_bActive;
			pItemDef[nNewCount].szItemID = m_pItemsList[n].m_szwItemID;
			pItemDef[nNewCount].szAccessPath = m_pItemsList[n].m_szwAccessPath;
			pItemDef[nNewCount].hClient = m_pItemsList[n].m_hClient;
			pItemDef[nNewCount++].vtRequestedDataType = m_pItemsList[n].m_vtRequestedDataType;
		}
	

	return nNewCount;
}

void COPCItemsList::SetNewItemsRes(DWORD dwCount, OPCITEMDEF **ppItemDef, OPCITEMRESULT **ppItemRes, HRESULT **pphr)
{
	OPCITEMRESULT *pItemRes= *ppItemRes;
	HRESULT *phr = *pphr;
	OPCITEMDEF *pItemDef= *ppItemDef;

	for(UINT n = 0 ; n< dwCount; n++) 	{
		COPCItem *pItem = GetItem( pItemDef[n].hClient );
		if( pItem ) {
			if( SUCCEEDED(m_pItemsList[n].m_hrOperation = phr[n]) ) {
				pItem->m_hServer = pItemRes[n].hServer;
				pItem->m_vtCanonicalDataType = pItemRes[n].vtCanonicalDataType;
				pItem->m_dwAccessRights = pItemRes[n].dwAccessRights;
				pItem->RsItemNew();
			}
			else {
				pItem->SetInvalid();
				pItem->m_hServer = 0;
				pItem->m_vtCanonicalDataType = VT_EMPTY;
				pItem->m_dwAccessRights = 0;
			}
		}
		if( pItemRes[n].dwBlobSize >0 ) ::CoTaskMemFree( pItemRes[n].pBlob );
	}

	::CoTaskMemFree(pItemRes);
	::CoTaskMemFree(phr);
	*ppItemRes=NULL;
	*pphr = NULL;
	delete [] *ppItemDef;
	*ppItemDef = NULL;

}

BOOL COPCItemsList::Choise2Read(OPCHANDLE hClient)
{
	COPCItem *pItem = GetItem( hClient );
	if( pItem ) {
		pItem->ItemForRead();
		return true;
	}
	else  return false;
}


// Return list of OPCHANDLE for all Valied Items if bAll or for Items Choise to Read
DWORD COPCItemsList::GetItems2Read(OPCHANDLE **ppHandls, BOOL bAll)
{
	DWORD nUsedCount = 0;
	for(UINT n = 0 ; n< m_nCount; n++) {
		COPCItem &it = m_pItemsList[n];
		if(it.Valid() && (bAll || it.ForRead()) ) nUsedCount++;
	}

	if(!nUsedCount) {
		*ppHandls = NULL;
		return 0;
	}

	OPCHANDLE *pItemHd = *ppHandls = new OPCHANDLE [nUsedCount];

	nUsedCount = 0;
	for(UINT n = 0 ; n< m_nCount; n++) 
	{
		COPCItem &it = m_pItemsList[n];
		if(it.Valid() && (bAll || it.ForRead()) ) 
		{
			it.ItemForRead();
			pItemHd[nUsedCount++] = it.m_hServer;
		}
	}

	return nUsedCount;

}

// This function is optimized for S7 OPC Server. I assume the server didn't change
// the original oreder of items 
// Set Read data result in OPCITEMSTATE and HRESULT. Free *ppHandls, 
// *ppOpcItemState include the Variand values in it and *pphr
void COPCItemsList::SetReadValue(HRESULT hMasterRes, DWORD dwCount, OPCHANDLE **ppHandls, OPCITEMSTATE **ppOpcItemState, HRESULT **pphr)
{

	OPCITEMSTATE *pOpcItemState = *ppOpcItemState;
	HRESULT *phr = *pphr;
	OPCHANDLE *pItemHd = *ppHandls;

	UINT n = 0;
	for( UINT nItem = 0 ; nItem < m_nCount; nItem++ ) 
	{
		COPCItem *pItem = m_pItemsList+ nItem;
		if( pItem->m_hClient == pOpcItemState[n].hClient) 
		{
			pItem->RsItemForRead();
			::VariantClear(&pItem->m_vDataValue);
			if( FAILED(hMasterRes) ) pItem->m_hrOperation = hMasterRes;
			else pItem->m_hrOperation = phr[n];

			if( SUCCEEDED(pItem->m_hrOperation ) ) {
				pItem->m_wQuality = pOpcItemState[n].wQuality;
				::VariantCopy( &pItem->m_vDataValue, &pOpcItemState[n].vDataValue);
				pItem->m_ftTimeStamp = pOpcItemState[n].ftTimeStamp;
			}	
			else
				pItem->m_wQuality = 0;
	
			if( ++n==dwCount ) break;
		}
	}

	_ASSERT(n==dwCount);

	if(pOpcItemState) {
		for(n=0; n<dwCount; )
			::VariantClear(&pOpcItemState[n++].vDataValue);
		::CoTaskMemFree(pOpcItemState);
	}

	if(phr) ::CoTaskMemFree(phr);
	delete [] *ppHandls;

	*ppHandls = NULL;
	*ppOpcItemState = NULL;
	*pphr = NULL;

}

BOOL COPCItemsList::Choise2Write(OPCHANDLE hClient, VARIANT &var)
{
	COPCItem *pItem = GetItem( hClient );
	if( pItem ) {
		pItem->ItemForWrite(var);
		return true;
	}
	else  return false;
}

// Return list of OPCHANDLE for Items Choise to Write
DWORD COPCItemsList::GetItems2Write(OPCHANDLE **ppHandls, VARIANT **ppVar)
{
	DWORD nUsedCount = 0;
	for(UINT n = 0 ; n< m_nCount; n++) {
		COPCItem &it = m_pItemsList[n];
		if( it.Valid() && it.ForWrite() ) nUsedCount++;
	}

	if(!nUsedCount) {
		*ppHandls = NULL;
		return 0;
	}

	OPCHANDLE *pItemHd = *ppHandls = new OPCHANDLE [nUsedCount];
	VARIANT *pVar = *ppVar = new VARIANT [nUsedCount];

	nUsedCount = 0;
	for(UINT n = 0 ; n< m_nCount; n++) 
	{
		COPCItem &it = m_pItemsList[n];
		if(it.Valid() && it.ForWrite() ) {
			pItemHd[nUsedCount] = it.m_hServer;
			::VariantInit(pVar + nUsedCount);
			::VariantCopy(pVar + nUsedCount, &it.m_vDataValue);
			nUsedCount++;
		}
	}

	return nUsedCount;

}

// Set Write result in OPCITEMSTATE and HRESULT. Free *ppHandls, 
// *ppVar include the Variand values and *pphr
void COPCItemsList::SetWriteResult(HRESULT hMasterRes, DWORD dwCount, OPCHANDLE **ppHandls, VARIANT **ppVar, HRESULT **pphr)
{
	HRESULT *phr = *pphr;
	VARIANT *pVar = *ppVar;
	OPCHANDLE *pItemHd = *ppHandls;

	UINT n = 0;
	for( UINT nItem = 0 ; nItem < m_nCount; nItem++ ) 
	{
		COPCItem *pItem = m_pItemsList+ nItem;
		if( pItem->m_hServer == pItemHd[n]) 
		{
			if( FAILED(hMasterRes) ) pItem->m_hrOperation = hMasterRes;
			else pItem->m_hrOperation = phr[n];
			pItem->RsItemForWrite();
			if( ++n==dwCount ) break;
		}

	}

	_ASSERT(n==dwCount);

	for(n=0; n<dwCount; n++) ::VariantClear(pVar + n);
	delete [] *ppVar;
	delete [] *ppHandls;
	if(phr) ::CoTaskMemFree(phr);

	*ppHandls = NULL;
	*ppVar = NULL;
	*pphr = NULL;
}

/*
void COPCItemsList::SetWriteResult(HRESULT hMasterRes, DWORD dwCount, OPCHANDLE **ppHandls, VARIANT **ppVar, HRESULT **pphr)
{
	HRESULT *phr = *pphr;
	VARIANT *pVar = *ppVar;
	OPCHANDLE *pItemHd = *ppHandls;

	for(UINT n = 0; n< dwCount; n++)
	{
		::VariantClear(pVar+ n);

		COPCItem *pItem = ByServerID( pItemHd[n] );
		if (pItem ) {
			if( FAILED(hMasterRes) ) pItem->m_hrOperation = hMasterRes;
			else pItem->m_hrOperation = phr[n];
		}
	}

	delete [] *ppVar;
	*ppVar = NULL;
	delete [] *ppHandls;
	*ppHandls = NULL;

	if(phr) ::CoTaskMemFree(phr);
	*pphr = NULL;

}
*/