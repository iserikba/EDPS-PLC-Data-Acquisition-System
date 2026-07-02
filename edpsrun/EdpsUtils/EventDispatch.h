#pragma once

class CEventDispatch
{
	HANDLE m_hTmThread;
	HANDLE m_hListChangd;
	HANDLE m_hEvntExit;
	HANDLE m_hListMtx;
	CEdpsTimer *m_pTmrsList;
	HANDLE m_hNativeThread;	  // Native Thread
	HWND   m_hWnd;		  // Window of the Thread
	static ATOM m_aWndClass;
	static DWORD  m_dwNextId;

	CWindow m_win;
public:
	// 
	BOOL Create();
    BOOL Destroy();

public:
	BOOL AddTimer(CEdpsTimer *pNew); 
	BOOL RemoveTimer(CEdpsTimer *pRemove);

	void ChangeStatus(){ ::SetEvent(m_hListChangd); }

	BOOL InvokeTimer(DWORD dwId);
//public:
//	void Sleep(DWORD dwMs);

protected:
	friend DWORD WINAPI TimerThread(LPVOID lpEventMng);
	friend LRESULT CALLBACK TimerWindowProc(      
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

protected:
	HWND CreateThreadWindow();

public:
	CEventDispatch(void);
	~CEventDispatch(void);
};
