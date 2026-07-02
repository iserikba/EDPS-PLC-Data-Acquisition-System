#include "StdAfx.h"
#include "cobject.h"


HRESULT STDMETHODCALLTYPE CCObject::QueryInterface(const IID& iid, void** ppv)
{   
     if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this); 
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CCObject::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

ULONG STDMETHODCALLTYPE CCObject::Release() 
{
	long ret = InterlockedDecrement(&m_nRefCount);
	if (ret == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}
CCObjectList::CCObjectList(UINT nInitSize, UINT nAddSize)
{
	if( (m_nMaxCount = nInitSize) < 1 ) m_nMaxCount = 1;
	if( (m_nAddSize = nAddSize) < 1 ) m_nAddSize = 1;
	m_nCount = m_nPtr = 0;
	m_ppOArray = new PCCObject[m_nMaxCount];
}

CCObjectList::~CCObjectList()
{
	for(m_nPtr=0; m_nPtr<m_nCount; m_nPtr++) m_ppOArray[m_nPtr]->Release();
	delete [] m_ppOArray;
}

void CCObjectList::Add(CCObject *pObj)
{

	if( m_nCount==m_nMaxCount ) {
		PCCObject *ppOA = new PCCObject[ m_nMaxCount += m_nAddSize ];
		memcpy(ppOA, m_ppOArray, sizeof(PCCObject)*m_nCount);
		delete [] m_ppOArray;
		m_ppOArray = ppOA;
	}

	m_ppOArray[m_nCount++] = pObj;
	pObj->AddRef();
}

BOOL CCObjectList::Remove(UINT nPosition)
{
	if( nPosition<m_nCount ) {
		m_ppOArray[nPosition]->Release();
		for(UINT n=nPosition; n<m_nCount; n++) 
			m_ppOArray[n] = m_ppOArray[n+1];
		m_nCount--;
		return true;
	}

	return false;
}
	
CCObject * CCObjectList::operator[] ( UINT nPos ) const
{
	if( nPos<m_nCount ) {
		PCCObject p;
		(p=m_ppOArray[nPos])->AddRef();
		return p;
	}
	else	return NULL;
}

