#pragma once


class CCObject : IUnknown
{
	LONG m_nRefCount;

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
	CCObject(void) { m_nRefCount=0; }
protected: 
	virtual ~CCObject() { ; }

  
};

typedef CCObject* PCCObject;

class CCObjectList : public CCObject
{
	PCCObject *m_ppOArray;
	UINT  m_nCount;
	UINT  m_nPtr;
	UINT  m_nMaxCount;
	UINT  m_nAddSize;
public:
	CCObjectList(UINT nInitSize=10, UINT nAddSize=+5);
	~CCObjectList();

public:
	void Add(PCCObject pObj);
	BOOL Remove(UINT nPosition);

public:
	UINT Count() const { return  m_nCount; }
	
	CCObject * operator[] ( UINT nPos ) const;

	PCCObject First(){
		m_nPtr = 0;
		if(  m_nCount ) {
			PCCObject p;
			(p=m_ppOArray[0])->AddRef();
			return p;
		}
		else	return NULL;
	}

	PCCObject Next(){
		if( (++m_nPtr) < m_nCount ){
			PCCObject p;
			(p=m_ppOArray[m_nPtr])->AddRef();
			return p;
		}
		else	return NULL;
	}

};