// TimeOut.cpp: implementation of the CTimeOut class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "TimeOut.h"

MYHEAP_IMPLEMENTATION(CTimeOut, s_heap);
MYHEAP_IMPLEMENTATION(CTimeOutMS, s_heap);

//////////////////////////////////////////////////////////////////////
// CTimeOut
//////////////////////////////////////////////////////////////////////
// ��ʱ����۵Ķ�ʱʱ��
bool CTimeOut::ToNextTick(int nSecs)
{
	bool result=IsTimeOut(nSecs);
	if(result)
	{
		time_t value=time(NULL);
		if(value >= m_tUpdateTime + nSecs*2)
		{
			Update();
			return true;
		}
		else
		{	
			m_tUpdateTime+=nSecs;
			return true;
		}
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////
// CTimeOutMS
//////////////////////////////////////////////////////////////////////
// ��ʱ����۵Ķ�ʱʱ��
bool CTimeOutMS::ToNextTick(int nMilliSecs)
{
	bool result=IsTimeOut(nMilliSecs);
	if(result)
	{
		if(clock() >= m_tUpdateTime + nMilliSecs*2)
		{
			Update();
			return true;
		}
		else
		{
			m_tUpdateTime+=nMilliSecs;
			return true;
		}
	}
	else
		return false;
}

int CTimeOutMS::GetRemain()  // ���������������Ϊ����BUFʱ��Ϊ-1 [�½���8/4/2007]
{
	if(m_nInterval < 0) 
		return m_nInterval; 
	else 
		return m_tUpdateTime ? ::CutRange(m_nInterval - (int)(clock()-m_tUpdateTime), 0, m_nInterval) : 0; 
}
