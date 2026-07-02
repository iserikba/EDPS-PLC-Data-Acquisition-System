#pragma once

//===========================================================================
// Class COPCItem is a summary of item's properties are used in OPC 
// It can't be createt directly by user but only by COPCItemsList class 
// as a maember of OPC list.
// All properties of the item accessible for reading only since they may change only with 
// changing it's properies in a OPC server
class COPCItem
{

friend class COPCItemsList;

protected:
// Item Definition OPC ITEM DEF used by IOPCItemMgt::AddItems and ValidateItems
	LPWSTR     m_szwItemID;
	LPWSTR     m_szwAccessPath;
	BOOL       m_bActive ; 
	OPCHANDLE  m_hClient; 
	//	DWORD   m_dwBlobSize;		 Blod is not supported by the client
	//	BYTE	*m_pBlob; 
	VARTYPE    m_vtRequestedDataType; 
// Item Result is used by  IOPCItemMgt::AddItems() and ValidateItems()
	OPCHANDLE   m_hServer; 
	VARTYPE     m_vtCanonicalDataType; 
	DWORD		m_dwAccessRights; 
// Item Attributes 
	// EUType is not supported by the client
	//	OPCEUTYPE   m_dwEUType; 
	//	VARIANT     m_vEUInfo; 
// Item State OPC ITEM STATE  is used by IOPCSyncIO::Read
	FILETIME	m_ftTimeStamp; 
	WORD		m_wQuality; 
	VARIANT		m_vDataValue; 
public:
// Result of last operation with item
	HRESULT     m_hrOperation;
	void        *m_pUserData; 

private:
	DWORD	m_dwItemStatus;  // Item Status: Used by List

	void    ItemNew()     { m_dwItemStatus = 1;}
	void    ItemForRead() { m_dwItemStatus |= 2;}
	void    ItemForWrite(VARIANT &var) { 
		::VariantClear(&m_vDataValue);
		::VariantCopy( &m_vDataValue, &var );
		m_dwItemStatus |= 4;
	}

	void    SetInvalid() { m_dwItemStatus = 8; }

public:
	BOOL New() const { return m_dwItemStatus & 1; }	
	BOOL ForRead() const { return m_dwItemStatus & 2; }
	BOOL ForWrite() const { return m_dwItemStatus & 4; }
	BOOL Valid() const { return this && (m_dwItemStatus & 8) == 0; }

private:
	void    RsItemNew()     { m_dwItemStatus &= ~1;}
	void    RsItemForRead() { m_dwItemStatus &= ~2;}
	void    RsItemForWrite() { m_dwItemStatus &= ~4; }


// Status
public:
	VARIANT & Data() { return m_vDataValue; }
	BOOL  Quality() const { return this ? (m_wQuality&OPC_QUALITY_MASK) == OPC_QUALITY_MASK : false; } 
	WORD  QualityVal() const { return m_wQuality; }
	const FILETIME & ReadingTime() const { return m_ftTimeStamp; }
	BOOL  Active() const  { return m_bActive; } 
	VARTYPE Type() const  { return m_vtCanonicalDataType; } 
	BOOL AccessRightsRead() const  { return (m_dwAccessRights & OPC_READABLE) ==OPC_READABLE; } 
	BOOL AccessRightsWrite() const  { return (m_dwAccessRights & OPC_WRITEABLE ) ==OPC_WRITEABLE ; }
	DWORD HandleUser() const {return m_hClient;}
	DWORD HandleServer() const {return m_hServer;}

protected:
	void Empty();
	void Init(LPWSTR szItemID, LPWSTR strAccessPath, BOOL bActive, OPCHANDLE hClient, VARTYPE vtRequestedDataType);


protected:
	~COPCItem(){ Empty(); }
};


//===========================================================================
// Class COPCItemsList is a list of OPC Items and Advise Sink for 
// facilitating async I/O operations.
// The list is a memeber of class COPCGroup. It represents client side presentation
// of the items in a OPC server 
class COPCItemsList : 
	public IOPCDataCallback
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
	// IOPCDataCallback
	//    
    virtual HRESULT STDMETHODCALLTYPE OnDataChange( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors);
    
    virtual HRESULT STDMETHODCALLTYPE OnReadComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors);
    
    virtual HRESULT STDMETHODCALLTYPE OnWriteComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMastererr,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *pClienthandles,
        /* [size_is][in] */ HRESULT *pErrors);
    
    virtual HRESULT STDMETHODCALLTYPE OnCancelComplete( 
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup);

protected:
	// IUnknown 
	LONG        m_cRef;
	// COPCItemsList
	COPCItem *m_pItemsList;
	UINT m_nCount;
	UINT m_nSize;
	UINT m_nGrowBy;


// Item List Operations
public:
	friend COPCItemsList * CreateItemsList( UINT nSize=10, UINT nGrowBy=10);

	COPCItem * AddItem(LPCTSTR szItemID, DWORD dwUserHandle, BOOL bActive=TRUE, VARTYPE vtRequestedDataType=VT_EMPTY, LPCTSTR szAccessPath=NULL);

	void SetActive( BOOL bActive=TRUE) {
		for(UINT n = 0 ; n< m_nCount; )
			m_pItemsList[n++].m_bActive = bActive;
	}

	// Return list of "NEW" Items
	DWORD GetNewItems(OPCITEMDEF **ppItemDef);

	//Take OPC Item Result, free Blobs and structure. If the result faild for item it's state will be "NEW"
	// It means it's Item has no the Server handle and excluded from any operations
	void SetNewItemsRes(DWORD dwCount, OPCITEMDEF **ppItemDef, OPCITEMRESULT **ppItemRes, HRESULT **pphr);

	BOOL Choise2Read(OPCHANDLE hClient);
	void Choise2Read(COPCItem &itm) { itm.ItemForRead(); }

	// Return list of OPCHANDLE for all Valied Items if bAll or for Items Choise to Read
	DWORD GetItems2Read(OPCHANDLE **ppHandls, BOOL bAll=FALSE);

	// Set Read data result in OPCITEMSTATE and HRESULT. Free *ppHandls, 
	// *ppOpcItemState include the Variand values in it and *pphr
	void SetReadValue(HRESULT hMasterRes, DWORD dwCount, OPCHANDLE **ppHandls, OPCITEMSTATE **ppOpcItemState, HRESULT **pphr);

	BOOL Choise2Write(OPCHANDLE hClient, VARIANT &var);
	void Choise2Write(COPCItem &itm, VARIANT &var) { itm.ItemForWrite(var); }

	// Return list of OPCHANDLE for Items Choise to Write
	DWORD GetItems2Write(OPCHANDLE **ppHandls, VARIANT **ppVar);

	// Set Write result in OPCITEMSTATE and HRESULT. Free *ppHandls, 
	// *ppVar include the Variand values and *pphr
	void SetWriteResult(HRESULT hMasterRes, DWORD dwCount, OPCHANDLE **ppHandls, VARIANT **ppVar, HRESULT **pphr);

	COPCItem * GetItem( OPCHANDLE hClient ) {
		for( UINT n = 0 ; n < m_nCount; n++ ) 
			if( m_pItemsList[n].m_hClient == hClient ) return m_pItemsList+ n;
		return NULL;
	}

	COPCItem * GetItem( LPCWSTR  szwItemId ) {
		for( UINT n = 0 ; n < m_nCount; n++ ) 
			if( lstrcmpW(m_pItemsList[n].m_szwItemID,szwItemId) == 0 ) return m_pItemsList+ n;
		return NULL;
	}

// Item List status
public:
	DWORD Count() const { return m_nCount; }
protected:
	COPCItemsList(UINT nSize, UINT nGrowBy){
		m_pItemsList = (COPCItem*) malloc(sizeof(COPCItem) *( m_nSize=nSize));
		m_nCount=0;
		m_nGrowBy=nGrowBy;
		m_cRef = 0;
	}

	COPCItem * ByServerID( OPCHANDLE hID ) {
		for( UINT n = 0 ; n < m_nCount; n++ ) 
			if( m_pItemsList[n].m_hServer == hID ) return m_pItemsList+ n;
		return NULL;
	}

	~COPCItemsList(void);
private:

};
