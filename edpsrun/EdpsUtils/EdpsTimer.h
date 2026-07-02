// EdpsTimer.h : Declaration of the CEdpsTimer

#pragma once
#include "resource.h"       // main symbols


// IEdpsTimer
[
	object,
	uuid("5A1EA625-128E-48A7-A506-9E2824F76798"),
	dual,	helpstring("IEdpsTimer Interface"),
	pointer_default(unique)
]
__interface IEdpsTimer : IDispatch
{
	[propget, id(1), helpstring("Scheduled event is Enabled")] HRESULT Enabled([out, retval] VARIANT_BOOL* pVal);
	[propput, id(1), helpstring("Scheduled event is Enabled")] HRESULT Enabled([in] VARIANT_BOOL newVal);
	[propget, id(2), helpstring("Timer Event Interval")] HRESULT Interval([out, retval] LONG* pVal);
	[propput, id(2), helpstring("Timer Event Interval")] HRESULT Interval([in] LONG newVal);
	[propget, id(3), helpstring("property HitsCount")] HRESULT HitsCount([out, retval] LONG* pVal);
};


// _IEdpsTimerEvents
[
	dispinterface,
	uuid("B12F0FB5-245A-415A-844E-38B0B6EF6164"),
	helpstring("_IEdpsTimerEvents Interface")
]
__interface _IEdpsTimerEvents
{
	[id(1), helpstring("Method OnTimer called by the timer service for a scheduled event."), source] HRESULT OnTimer();
};

//#include <ocmm.idl>
 
// CEdpsTimer

[
	coclass,
	threading("free"),
	event_source("com"),
	vi_progid("EdpsUtils.EdpsTimer"),
	progid("EdpsUtils.EdpsTimer.1"),
	version(1.0),
	uuid("E08597E3-A6D1-4549-BD7C-5EDBAEF3094C"),
	helpstring("EdpsTimer Class")
]
class ATL_NO_VTABLE CEdpsTimer : 
	public IEdpsTimer
{
// Properties
protected:
	BOOL  m_bEnabled;
	LONG  m_lInterval;
	LONG  m_lHeatCount;
	HANDLE m_hTimer;

public:
	BOOL Enabled() const { return m_bEnabled && m_lInterval>0; }
	LONG Interval() const { return m_lInterval; }

public:
	CEdpsTimer();

	__event __interface _IEdpsTimerEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	
	void FinalRelease() ;
	void InvokeEvent(void) { __raise OnTimer(); }
public:

	STDMETHOD(get_Enabled)(VARIANT_BOOL* pVal);
	STDMETHOD(put_Enabled)(VARIANT_BOOL newVal);
	STDMETHOD(get_Interval)(LONG* pVal);
	STDMETHOD(put_Interval)(LONG newVal);
	STDMETHOD(get_HitsCount)(LONG* pVal);

// Timer Quiue Related Functions
protected:
	BOOL RestartTimer(BOOL bBefore, LONG lBefore);

	static HANDLE m_hTimerQuiue;
	friend VOID CALLBACK WaitOrTimerCallback( PVOID lpParameter, BOOLEAN TimerOrWaitFired);


};

