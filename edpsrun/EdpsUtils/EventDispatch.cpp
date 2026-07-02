#include "StdAfx.h"
#include ".\edpstimer.h"
#include ".\eventdispatch.h"

#define WM_MYTIMER (WM_USER+777)

LPCTSTR g_szClassName=_T("EdpsEventDispatchWndCls");
ATOM CEventDispatch::m_aWndClass = NULL;
DWORD  CEventDispatch::m_dwNextId = 1;

CEventDispatch::CEventDispatch(void)
{
	m_hTmThread = m_hNativeThread = NULL;
	m_pTmrsList = NULL;
	m_hListChangd = ::CreateEvent(NULL, false, false, NULL);
	m_hEvntExit = ::CreateEvent(NULL, false, false, NULL);
	m_hListMtx  = ::CreateMutex(NULL, false, NULL);

}

CEventDispatch::~CEventDispatch(void)
{
	Destroy();
	if(m_hNativeThread) {
		::CloseHandle(m_hNativeThread); 
	}
	::CloseHandle( m_hListChangd );
	::CloseHandle( m_hEvntExit );
	::CloseHandle( m_hListMtx );

}

HWND CEventDispatch::CreateThreadWindow()
{
	if(! CEventDispatch::m_aWndClass ) {
		WNDCLASSEX wcex;
		memset((PVOID) &wcex, 0, sizeof(WNDCLASSEX));
		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= CS_CLASSDC;
		wcex.lpfnWndProc	= (WNDPROC)TimerWindowProc;
		wcex.hInstance		= GetModuleHandle(NULL);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszClassName	= g_szClassName;

		CEventDispatch::m_aWndClass = ::RegisterClassEx(&wcex);
	}

	if( !CEventDispatch::m_aWndClass ) return NULL;
   
	HWND hWnd = ::CreateWindow(g_szClassName, NULL, WS_DISABLED, 1, 1, 1, 1, NULL, NULL, GetModuleHandle(NULL), (LPVOID) this);

	return hWnd;
}

BOOL CEventDispatch::Create()
{
	// Manager Already Created
	if( m_hTmThread ) return true;

	// Create Manager for Current Native thread
	m_hNativeThread = ::GetCurrentThread();

//	EnumThreadWindows();
	if( !(m_hWnd = CreateThreadWindow()) ) return false;
	m_win.Attach(m_hWnd);

	// Create T
	DWORD dwTheadID;
	m_hTmThread = ::CreateThread(NULL, 1000, TimerThread, (LPVOID) this, CREATE_SUSPENDED, &dwTheadID);
	if(!m_hTmThread) return false;
	::SetThreadPriority(m_hTmThread, THREAD_PRIORITY_HIGHEST);

	::ResumeThread(m_hTmThread);

	return true;
}

BOOL CEventDispatch::Destroy()
{
	if( m_hTmThread ) {
//		::CloseWindow(m_hWnd);
		m_hWnd = NULL;
		
		::SetEvent(m_hEvntExit);
		::WaitForSingleObject(m_hTmThread, 8000);
		::CloseHandle(m_hTmThread);
		m_hTmThread = NULL;
	}
	return true;
}

LRESULT CALLBACK TimerWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) 
	{
	case WM_CREATE:
	{
		LPCREATESTRUCT pCrst = (LPCREATESTRUCT) lParam;
		CEventDispatch *pMng = (CEventDispatch*) pCrst->lpCreateParams;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) pMng);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	case WM_MYTIMER:
	{
		if( ::InSendMessage() ) ::ReplyMessage(0);
		CEventDispatch *pMng= (CEventDispatch*) (::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if( pMng ) pMng->InvokeTimer( (DWORD)lParam );
		return 0;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// Get System Time in 10 mcs
inline ULONGLONG GetSysTime10mcs()
{
	FILETIME ft;
	::GetSystemTimeAsFileTime(&ft);
	ULARGE_INTEGER ddRes;
	ddRes.LowPart = ft.dwLowDateTime;
	ddRes.HighPart = ft.dwHighDateTime;
	return ddRes.QuadPart/100;
}

inline long STdif(ULONGLONG dwNow, ULONGLONG dwCmp)
{
	LARGE_INTEGER ddRes;
	ddRes.QuadPart = dwCmp - dwNow;
	return ddRes.LowPart;
}

DWORD WINAPI TimerThread(LPVOID lpEventMng)
{
	CEventDispatch *pMng = static_cast<CEventDispatch*> (lpEventMng);

	DWORD dwTRes = 0;

	while(1) // Infinite loop
	{
		HANDLE aEvnts[2];
		aEvnts[0] = pMng->m_hEvntExit;
		aEvnts[1] = pMng->m_hListChangd;
		DWORD dwSleepTime = INFINITE;

		// Find Time of next time out
		::WaitForSingleObject(pMng->m_hListMtx, INFINITE);

		LONGLONG ddTime = ::GetSysTime10mcs();
		for(CEdpsTimer *pNext = pMng->m_pTmrsList; pNext; pNext = pNext->m_pNext)
		{
			if( ! pNext->Enabled() ) continue;

			long lDif = STdif( ::GetSysTime10mcs(), pNext->m_ddNextFire);
			while( lDif <= 100 ) // Time out < 1 Ms
			{ // Fire Timer event
				pNext->m_lHeatCount++;
//				pNext->InvokeEvent();
				::SendNotifyMessage(pMng->m_hWnd, WM_MYTIMER, 0, pNext->m_dwTimerId);
				lDif = STdif( ::GetSysTime10mcs(), pNext->m_ddNextFire) + pNext->Interval()*100;
//				lDif += pNext->Interval()*100;
				pNext->m_ddNextFire = ddTime + lDif;
			}
			lDif /=100;
			if(dwSleepTime> (DWORD)lDif) 
				dwSleepTime = lDif;
		}

		::ReleaseMutex(pMng->m_hListMtx);
		// Sleep Thread until next Event Or Time out 
		DWORD dwRes = ::WaitForMultipleObjects(2, aEvnts, false, dwSleepTime);
		if(dwRes==WAIT_ABANDONED_0 || dwRes==(WAIT_ABANDONED_0+1) ) 
		{ // Event ABANDONED -
			dwTRes = -1;
			break;
		}
		else if(dwRes==WAIT_OBJECT_0 ) 
		{ // Exit Thread Event
			break;
		}
		// else Fire TimeOut Event
		// List of Events has changed

	}

	return dwTRes;
}

BOOL CEventDispatch::InvokeTimer(DWORD dwId)
{
	::WaitForSingleObject(m_hListMtx, INFINITE);
	for(CEdpsTimer *pNext = m_pTmrsList; pNext; pNext = pNext->m_pNext)
		if( pNext->m_dwTimerId == dwId ) {
			pNext->InvokeEvent();
			break;
		}
	::ReleaseMutex(m_hListMtx);
	return true;
}

BOOL CEventDispatch::AddTimer(CEdpsTimer *pNew)
{
	if(! m_hTmThread || m_hNativeThread != ::GetCurrentThread() ) return false;

	ULONGLONG dwNextFire = GetSysTime10mcs() + pNew->Interval()*100;
	
	::WaitForSingleObject(m_hListMtx, INFINITE);
	pNew->m_dwTimerId = CEventDispatch::m_dwNextId++;
	pNew->m_pNext = m_pTmrsList;
	pNew->m_ddNextFire = dwNextFire;
	pNew->m_pPrev = NULL;
	if( m_pTmrsList ) {
		m_pTmrsList->m_pPrev = pNew;
	}
	m_pTmrsList = pNew;
	::ReleaseMutex(m_hListMtx);
	
	::SetEvent(m_hListChangd);
	return true;
}

BOOL CEventDispatch::RemoveTimer(CEdpsTimer *pRemove)
{
	::WaitForSingleObject(m_hListMtx, INFINITE);
	if(pRemove->m_pNext) pRemove->m_pNext->m_pPrev = pRemove->m_pPrev;
	if(pRemove->m_pPrev) pRemove->m_pPrev->m_pNext = pRemove->m_pNext;
	if(m_pTmrsList==pRemove) 
		m_pTmrsList = pRemove->m_pNext;
	pRemove->m_pNext = pRemove->m_pPrev = NULL;
	::ReleaseMutex(m_hListMtx);

	return true;
}

//void CEventDispatch::Sleep(DWORD dwMs)
//{
//	dwMs *=100;
//	ULONGLONG dwExitTime = GetSysTime10mcs() + dwMs;
//	HANDLE hTmt = ::CreateEvent(NULL, false, false, NULL);
//
//	while(1) {
//		DWORD dwWait = MsgWaitForMultipleObjects(1, &hTmt, false, dwMs/100, QS_ALLEVENTS | QS_ALLPOSTMESSAGE | QS_SENDMESSAGE);
//		if(dwWait==WAIT_TIMEOUT) break;
//		MSG mg;
//		while(PeekMessage(&mg, NULL, 0,0, true) ){
//			TranslateMessage(&mg);
//			DispatchMessage(&mg);
//
//		}
//		dwMs = STdif(GetSysTime10mcs(), dwExitTime);
//		if(dwMs<=0) break;
//	}
//	::CloseHandle(hTmt);
//}
//
