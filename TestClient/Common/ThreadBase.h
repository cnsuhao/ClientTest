// 线程基类
// 仙剑修，2001.11.15

#ifndef THREADBASE_H
#define THREADBASE_H

#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

//#include <afxmt.h>
#include <windows.h>


class CThreadBase
{
public:		// 构造、析构
	//constructors
	CThreadBase();

	//destructor
	virtual ~CThreadBase();

public:		// 公共，由父线程调用。
	bool	CreateThread(bool bRun = true);		// false: 暂不运行，用 ResumeThread() 运行

	bool	ResumeThread();					// return false: 失败

	// 通知子线程关闭，并阻塞 nMilliseconds 毫秒。返回true: 线程关闭成功
	bool	CloseThread(long nMilliseconds = 0);

/////////////////////////////////////////////////////////////////////
protected:	// 派生用
	//overrideable
	virtual	void	OnInit() { }
	virtual bool	OnProcess() = 0;		// 不需要返回DWORD
	virtual void	OnDestroy() { }

/////////////////////////////////////////////////////////////////////
private:	// 内部使用
//	HANDLE	GetThreadHandle() { return m_hThread; }
	bool	IsCloseEvent(long nMilliseconds = 0);		// 检查关闭线程事件是否触发。用于 ThreadProc() 函数调用
	DWORD	ThreadProc();
	bool	IsCreated() { return (m_hThread != NULL); }

protected:
//	CCriticalSection m_xCtrl;		// 用于派生类中的变量共享控制
	HANDLE		m_hThread;
	HANDLE		m_hCloseThreadEvent;
	
/////////////////////////////////////////////////////////////////////
private:
	static DWORD WINAPI TrueThreadProc(LPVOID pParam);
};


//////////////////////////////////////////////////////////////////////
class ISimpleLockObj
{
public:
	virtual void Lock	(void)	= 0;
	virtual void Unlock	(void)	= 0;
};


//////////////////////////////////////////////////////////////////////
class CSimpleCriticalSection : public ISimpleLockObj
{
public:
	CSimpleCriticalSection( )
	{
		InitializeCriticalSection( &m_CritSec );
	};
	
	virtual ~CSimpleCriticalSection()
	{
		DeleteCriticalSection( &m_CritSec );
	};
	
	void    Lock(void)
	{
		EnterCriticalSection( &m_CritSec );
	};
	
	void    Unlock(void)
	{
		LeaveCriticalSection( &m_CritSec );
	};
	
private:
	CRITICAL_SECTION    m_CritSec;
};


//////////////////////////////////////////////////////////////////////
class CSimpleMutexLock : public ISimpleLockObj
{
public:
	CSimpleMutexLock() : m_hMutex(NULL) {}
	~CSimpleMutexLock(void)	{ if (m_hMutex) ::CloseHandle(m_hMutex); }
	
	void Lock	(void)	{ if (m_hMutex) ::WaitForSingleObject(m_hMutex, INFINITE); }
	void Unlock	(void)	{ if (m_hMutex) ::ReleaseMutex(m_hMutex); }
	
private:
	HANDLE m_hMutex;
	
	// static
public:
	static CSimpleMutexLock* CreateNew(const char* pszName = NULL, bool bExistTest = false)
	{
		CSimpleMutexLock* pMutex = new CSimpleMutexLock;
		if (!pMutex)
			return NULL;
		
		pMutex->m_hMutex = ::CreateMutex(NULL, false, pszName);
		if (!pMutex->m_hMutex)
		{
			delete pMutex;
			return NULL;
		}
		
		if (pszName && bExistTest)
		{
			if (ERROR_ALREADY_EXISTS == ::GetLastError())
			{
				delete pMutex;
				return NULL;
			}
		}
		
		return pMutex;
	}
};


//////////////////////////////////////////////////////////////////////
class CMySingleLock 
{
public:
	CMySingleLock(ISimpleLockObj* pLock)
		: m_pLock(pLock)
	{ 		
		if (m_pLock) 
			m_pLock->Lock(); 
	}
	
	~CMySingleLock	(void)	
	{
		if (m_pLock)
			m_pLock->Unlock();
	}
	
private:
	ISimpleLockObj* m_pLock;
};





#endif // THREADBASE_H