// �̻߳���
// �ɽ��ޣ�2001.11.15

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
public:		// ���졢����
	//constructors
	CThreadBase();

	//destructor
	virtual ~CThreadBase();

public:		// �������ɸ��̵߳��á�
	bool	CreateThread(bool bRun = true);		// false: �ݲ����У��� ResumeThread() ����

	bool	ResumeThread();					// return false: ʧ��

	// ֪ͨ���̹߳رգ������� nMilliseconds ���롣����true: �̹߳رճɹ�
	bool	CloseThread(long nMilliseconds = 0);

/////////////////////////////////////////////////////////////////////
protected:	// ������
	//overrideable
	virtual	void	OnInit() { }
	virtual bool	OnProcess() = 0;		// ����Ҫ����DWORD
	virtual void	OnDestroy() { }

/////////////////////////////////////////////////////////////////////
private:	// �ڲ�ʹ��
//	HANDLE	GetThreadHandle() { return m_hThread; }
	bool	IsCloseEvent(long nMilliseconds = 0);		// ���ر��߳��¼��Ƿ񴥷������� ThreadProc() ��������
	DWORD	ThreadProc();
	bool	IsCreated() { return (m_hThread != NULL); }

protected:
//	CCriticalSection m_xCtrl;		// �����������еı����������
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