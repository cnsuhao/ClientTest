// TimeOut.h: interface for the CTimeOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEOUT_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_)
#define AFX_TIMEOUT_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include "BaseFunc.h"
#include "MyHeap.h"

//////////////////////////////////////////////////////////////////////
// ��ʱ�࣬�ԡ��롱Ϊ��λ
class CTimeOut
{
public:
	CTimeOut(int nIntervalSecs = 0)
	{ 
		m_nInterval = nIntervalSecs;
		m_tUpdateTime = 0;
	}		// =0 : ���������ʼ��
//	virtual ~CTimeOut_() {}

public:
	// ����ʱ���ǩ
	void	Update		()
	{ 
		m_tUpdateTime = ::time(NULL);
	}
	// ����Ƿ�ʱ
	bool	IsTimeOut	()
	{
		bool result= ::time(NULL) >= m_tUpdateTime + m_nInterval;
		return result;
	}
	// ���ڶ�ʱ����������ʱ�ӡ�ÿ�δ���ʱΪtrue��
	bool	ToNextTime	()
	{ 
		bool result=IsTimeOut();
		if(result)
		{
			Update();
			return true;
		}
		else 
			return false;
	}
	// ��ʼ��ʱ��Ƭ��(��������)
	void	SetInterval	(int nSecs)
	{ 
		m_nInterval = nSecs;
	}

	// ��ʼ����ʱ��(ͬSetInterval)
	void	Startup		(int nSecs)
	{ 
		m_nInterval = nSecs;
		Update();
	}
	// ʱ���Ƿ񴥷�(ֻ����һ��)����Startup���ʹ�ã���ʵ��ֻ����һ�ε�ʱ�ӡ�
	bool	TimeOver	()
	{ 
		if(IsActive() && IsTimeOut())
		{
			Clear();
			return true;
		}
		else
			return false;
	}
	void	Clear		()
	{ 
		m_nInterval = 0;
		m_tUpdateTime = 0;
	}
	bool	IsActive	()
	{ 
		return m_tUpdateTime != 0;
	}
	void	IncInterval	(int nSecs, int nLimit)
	{ 
		m_nInterval = ::CutOverflow(m_nInterval+nSecs, nLimit);
	}
	void	DecInterval	(int nSecs)
	{ 
		m_nInterval = ::CutTrail(m_nInterval-nSecs, 0);
	}

	// ����Ƿ�ʱ��ָ��ʱ��Ƭ������ͬһʱ���ǩ�������ͬ��ʱ�¼���(��ʵ��һ��������ƶ��ʱ��)
	bool	IsTimeOut	(int nSecs)
	{
		bool result=::time(NULL) >= m_tUpdateTime + nSecs;
		return result;
	}
	// ���ڱ䳤�ĳ�ʱ�¼���
	bool	ToNextTime	(int nSecs)
	{ 
		bool result=IsTimeOut(nSecs);
		if(result)
		{
			Update();
			return true;
		}
		else 
			return false;
	}
	// ʱ���Ƿ񴥷�(ֻ����һ��)����Startup���ʹ�ã���ʵ��ֻ����һ�ε�ʱ�ӡ�
	bool	TimeOver	(int nSecs)
	{ 
		if(IsActive() && IsTimeOut(nSecs))
		{
			Clear();
			return true;
		}
		else
			return false;
	}
	// ʱ�ӻ���ۣ�������ֵ���������ֵ������ͬToNextTime(...)
	bool	ToNextTick	(int nSecs);
//	void	AppendInterval	(int nSecs)			{ if(ToNextTime()) m_nInterval=nSecs; else m_nInterval+=nSecs; }	// ToNextTime(): ��֤������nSecs��

public: // get
	int		GetRemain()
	{ 
		return m_tUpdateTime ? ::CutRange(m_nInterval - (int)(::time(NULL)-m_tUpdateTime), (int)0, m_nInterval) : 0;
	}
	int		GetInterval()
	{ 
		return m_nInterval;
	}
//06.11.7���������,Ϊ��֧���л���ͼ��-����----begin----------------------------------------------------
	time_t	GetUpdateTime()
	{ 
		return m_tUpdateTime;
	}
	void	SetUpdateTime( time_t time )
	{ 
		m_tUpdateTime = time;
	}
//-----------------------------------------end------------------------------------------------------
protected:
	int		m_nInterval;
	time_t	m_tUpdateTime;
public:
	MYHEAP_DECLARATION(s_heap);
};

//////////////////////////////////////////////////////////////////////
// ��ʱ�࣬�ԡ����롱Ϊ��λ
class CTimeOutMS
{
public:
	CTimeOutMS(int nIntervalSecs = 0)
	{ 
		m_nInterval = nIntervalSecs;
		m_tUpdateTime = 0;
	}		// =0 : ���������ʼ��
//	virtual ~CTimeOutMS_() {}

public:
	void	Update		()
	{ 
		m_tUpdateTime = clock();
	}
	bool	IsTimeOut	()						
	{//[2009.07.06]
		//return clock() >= m_tUpdateTime+m_nInterval
		clock_t value=clock();
		return DWORD(value) >= DWORD(m_tUpdateTime) + DWORD(m_nInterval);
	}
	bool	ToNextTime	()
	{ 
		if(IsTimeOut())
		{
			Update();
			return true;
		}
		else
			return false;
	}
	void	SetInterval	(int nMilliSecs)
	{ 
		m_nInterval = nMilliSecs;
	}
	void	Startup		(int nMilliSecs)
	{ 
		m_nInterval = nMilliSecs;
		Update();
	}
	bool	TimeOver	()
	{ 
		if(IsActive() && IsTimeOut())
		{
			Clear();
			return true;
		}
		else
			return false;
	}
	void	Clear		()
	{ 
		m_nInterval = 0;
		m_tUpdateTime = 0;
	}
	bool	IsActive	()
	{ 
		return m_tUpdateTime != 0;
	}
	void	IncInterval	(int nMilliSecs, int nLimit)
	{ 
		m_nInterval = ::CutOverflow(m_nInterval+nMilliSecs, nLimit);
	}
	void	DecInterval	(int nMilliSecs)
	{ 
		m_nInterval = ::CutTrail(m_nInterval-nMilliSecs, 0);
	}
	bool	IsTimeOut	(int nMilliSecs)
	{ 
		clock_t value=clock();
		return value >= m_tUpdateTime+nMilliSecs;
	}
	int		LeaveTime()
	{ 
		return (int)(clock() -m_tUpdateTime-m_nInterval);
	}
	bool	ToNextTime	(int nMilliSecs)
	{ 
		if(IsTimeOut(nMilliSecs))
		{
			Update();
			return true;
		}
		else 
			return false;
	}
	bool	TimeOver	(int nMilliSecs)
	{ 
		if(IsActive() && IsTimeOut(nMilliSecs))
		{
			Clear();
			return true;
		}
		else
			return false;
	}
	bool	ToNextTick	(int nMilliSecs);
//	void	AppendInterval	(int nMilliSecs)	{ if(ToNextTime()) m_nInterval=nMilliSecs; else m_nInterval+=nMilliSecs; }	// ToNextTime(): ��֤������nSecs��

public: // get
	int		GetRemain();							
	int		GetInterval()
	{ 
		return m_nInterval;
	}
//06.11.7���������,Ϊ��֧���л���ͼ��,����-----begin----------------------------------------------------
	time_t	GetUpdateTime()
	{ 
		return m_tUpdateTime;
	}
	void	SetUpdateTime( time_t time )
	{ 
		m_tUpdateTime = time;
	}
//-----------------------------------------end------------------------------------------------------
protected:
	int		m_nInterval;
	time_t	m_tUpdateTime;
public:
	MYHEAP_DECLARATION(s_heap);
};

#endif // !defined(AFX_TIMEOUT_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_)
