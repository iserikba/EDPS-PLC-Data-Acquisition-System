class CEdpsParam
{
public:
	DWORD m_dwParID;
	LPCSTR m_pszItemId;
	VARIANT m_val;

	CEdpsParam * m_pNext;
	COPCItem   * m_pItem;
};

class CEdpsParamList
{
protected:
	LPSTR m_pStringBuffer;
	DWORD m_dwCount;
	CEdpsParam * m_pHead;
	CEdpsParam * m_pTail;

public:
	BOOL AddItem(DWORD dwParID, LPCSTR szItemId);   
	BOOL AddItem(DWORD dwParID, LPCSTR szItemId, LPCSTR szType, LPCSTR szDate);   

	// If bReadRequest szRequest contains a read request
	// If NOT bReadRequest szRequest contains a write request
	DWORD ParseRequest( LPCSTR szRequest, BOOL bReadRequest);
	// Return Name of Group (enum ParID) returned string must be freed by free()
	LPCTSTR GetListName(  );

	// 
	int FindOrAddItemsToGroup( COPCGroup *pGroup, BOOL bSet2Read);

	// Return bstr and count of items read succesfully
	BSTR GetMReadResult(DWORD &dwOk);

	// Set Single Item For Read/Write if it's for read set szType=NULL; 
	// 0 on return - successed, -1 - can't add value, -2 - wrong type or value of data 
	int SingleItem(COPCGroup *pGroup, LPCSTR szConn, LPCSTR szItem, LPCSTR szType=NULL, LPCSTR szDate=NULL);
	
	// Read Single Item and return Result String or NULL if reading failed
	BSTR ReadValue(COPCGroup *pGroup);

	// Write Single Item 
	BOOL WriteValue(COPCGroup *pGroup);

public:
	CEdpsParamList() { m_pStringBuffer=NULL; m_dwCount=0; m_pHead=m_pTail=NULL; }
	~CEdpsParamList();
};

