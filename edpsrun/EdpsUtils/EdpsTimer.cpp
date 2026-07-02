// EdpsTimer.cpp : Implementation of CEdpsTimer

#include "stdafx.h"
#include "EdpsTimer.h"
#include ".\edpstimer.h"

HANDLE CEdpsTimer::m_hTimerQuiue = NULL;

CEdpsTimer::CEdpsTimer() {
	m_bEnabled = false; 
	m_lInterval = -1;
	m_lHeatCount = 0;
	m_hTimer = NULL;
}

// Called during the final stages of object construction, this method performs any final initialization 
HRESULT CEdpsTimer::FinalConstruct()
{
	if(! m_hTimerQuiue )
		m_hTimerQuiue = ::CreateTimerQueue();

	return S_OK;
}
// This method is called before destructon the object
void CEdpsTimer::FinalRelease() 
{
	if(m_hTimer) DeleteTimerQueueTimer(m_hTimerQuiue, m_hTimer, NULL);
	m_hTimer = NULL;
}

BOOL CEdpsTimer::RestartTimer(BOOL bBeforeEnabled, LONG lBeforeInt)
{
	BOOL bRes = true;

	// Need to Delete Previos Timer 
	if( bBeforeEnabled && ( !Enabled() || lBeforeInt!=m_lInterval) ) 
	{
		::DeleteTimerQueueTimer(m_hTimerQuiue, m_hTimer, NULL);
		m_hTimer = NULL;

	}

	// Have to run new timer 
	if( !bBeforeEnabled && Enabled() ||  Enabled() && ( lBeforeInt != m_lInterval) )
	{
		m_lHeatCount = 0;
		bRes =::CreateTimerQueueTimer(&m_hTimer, m_hTimerQuiue, WaitOrTimerCallback, (PVOID) this, m_lInterval, m_lInterval, WT_EXECUTEINTIMERTHREAD | WT_EXECUTELONGFUNCTION);
	}

	return bRes;
}

void CALLBACK WaitOrTimerCallback( PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	CEdpsTimer *pTimes = static_cast<CEdpsTimer*> (lpParameter);
	pTimes->m_lHeatCount++;
	pTimes->InvokeEvent();
}

STDMETHODIMP CEdpsTimer::get_Enabled(VARIANT_BOOL* pVal)
{
	*pVal = m_bEnabled && m_lInterval>0 ? -1 : 0;
	return S_OK;
}

STDMETHODIMP CEdpsTimer::put_Enabled(VARIANT_BOOL newVal)
{
    BOOL bPrevEn = Enabled();

	m_bEnabled = (newVal!=0);

	return RestartTimer(bPrevEn, m_lInterval) ? S_OK : E_FAIL;
}

STDMETHODIMP CEdpsTimer::get_Interval(LONG* pVal)
{
	*pVal = m_lInterval;
	return S_OK;
}

STDMETHODIMP CEdpsTimer::put_Interval(LONG newVal)
{
    BOOL bPrevEn = Enabled();
	LONG lPrevInt = m_lInterval;

	m_lInterval = newVal;
	if(m_lInterval<10) m_lInterval = 10;

	return RestartTimer(bPrevEn, lPrevInt) ? S_OK : E_FAIL;
}

STDMETHODIMP CEdpsTimer::get_HitsCount(LONG* pVal)
{
	*pVal = m_lHeatCount;
	return S_OK;
}
