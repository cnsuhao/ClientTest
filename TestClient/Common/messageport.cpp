// �ӿ��ࡣ�ӿڣ�������ϢͨѶ���Ƶ��̼߳�ͨѶ�ӿڡ�
// �ɽ��ޣ�2002.8.28

#include "./MessagePort.h"
#include "./BaseFunc.h"
#include <stdlib.h>

using namespace message_port;

CMessagePort::PORT_SET CMessagePort::m_setPort;
MYHEAP_IMPLEMENTATION(message_port::CMessagePacket, s_heap);
long	g_nMessagePortErrorCount;
///////////////////////////////////////////////////////////////////////////////////////
// interface
///////////////////////////////////////////////////////////////////////////////////////
// ��ʼ�������ýӿ�ID�ţ���ʼ������Ϣ�����ظ�����(PORT_ID���ܸı�)��
bool CMessagePort::Open	()
{
	m_nState = STATE_OK;
	for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
		m_setRecycle.push_back(*iter);
	m_setMsg.clear();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// ������Ϣ��ָ���ӿڡ�������ϢID���������͡����ݡ�
bool CMessagePort::Send	(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)
{
	if (!buf)
	{
		::LogSave( "CMessagePort::Send buf's len is zero.%d send to %d,nPacket=%d",m_id,nPort,nPacket );
	}
	ASSERT(buf);

	if(!IsOpen())
		return false;

	if(nPort == INVALID_PORT)
		return true;

	if(nPort < 0 || nPort >= (int)m_setPort.size())
	{
		ASSERT(!"nPort");
		return false;
	}

	return m_setPort[nPort]->PushMsg(m_id, nPacket, nVarType, buf);
}

///////////////////////////////////////////////////////////////////////////////////////
// ����ָ���ӿ�(�����нӿ�)��������Ϣ����ָ����ϢID��Ҳ�ɲ�ָ����
bool CMessagePort::Recv	(int nPort, int nPacket, VAR_TYPE nVarType, void* buf, CMessageStatus* pStatus /*= NULL*/)	
{
	ASSERT(buf);

	if(m_nState == STATE_CLOSED)
	{
		if(pStatus)
			pStatus->m_nError	= STATUS_FLAG_CLOSE;
		return false;
	}

	int	nRcvPort = nPort, nRcvPacket = nPacket;
	VAR_TYPE	nRcvVarType;
	if(PopMsg(&nRcvPort, &nRcvPacket, &nRcvVarType, buf))		// �ڲ����������ô򿪻�����
	{
		// �������
		if(SIZE_OF_TYPE(nRcvVarType) > SIZE_OF_TYPE(nVarType))
		{
			ASSERT(!"VarType");
			if(pStatus)
				pStatus->m_nError		= STATUS_FLAG_ERROR;			//? �Ժ��֧���Զ�ת������
			return false;
		}

		if(pStatus)
		{
			pStatus->m_nPortFrom	= nRcvPort;
			pStatus->m_nPacket		= nRcvPacket;
			pStatus->m_nVarType		= nRcvVarType;
			pStatus->m_nError		= STATUS_FLAG_OK;
		}
		return true;
	}

	if(pStatus)
		pStatus->m_nError	= STATUS_FLAG_OK;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// �رսӿڣ����ٽ�����Ϣ�����ظ����á�
bool CMessagePort::Close()
{
	m_nState = STATE_CLOSED;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// CMessagePort
///////////////////////////////////////////////////////////////////////////////////////
int CMessagePort::SIZE_OF_TYPE		(int type)
{
	const int	fixlen_type_size = 10;
	const int	size_of[fixlen_type_size] = {1,1,2,2, 4,4,4,4, 4,8};
	if(type < VARTYPE_NONE)
		return type;
	else if(type == VARTYPE_NONE)
	{
		ASSERT(!"SIZE_OF_TYPE");
		return 0;
	}
	else if(type - (VARTYPE_NONE+1) < fixlen_type_size)
		return size_of[type - (VARTYPE_NONE+1)];

	ASSERT(!"SIZE_OF_TYPE.");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::PushMsg(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)	// nData�еĴ��ͽṹ���ᱻCOPY
{
	I64 i64TimeStart = ::GetUtcMillisecond();
	ASSERT(buf);

	if(m_nState != STATE_OK)		// Close()�󣬲�������Ϣ
		return false;

	if(nPort < 0 || nPort >= (int)m_setPort.size())
	{
		::LogSave("PushMsg(nPort) ERROR! nPort = %u,size = %u",nPort,m_setPort.size());
		return false;
	}

	int	nSize = SIZE_OF_TYPE(nVarType);
	if(nSize == 0 || nSize > MAX_MSGPACKSIZE)
	{
		ASSERT(!"PushMsg(nSize)");
		return false;
	}

	CMessagePacket*	pMsg = NULL;

	EnterCriticalSection(&m_cs);
	if(!m_setRecycle.empty())
	{
		pMsg = m_setRecycle.front();
		m_setRecycle.pop_front();
	}
	else
	{
		pMsg = new CMessagePacket;
	}

	IF_NOT (pMsg)
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	pMsg->m_nPortFrom		= nPort;
	pMsg->m_nPacket			= nPacket;
	pMsg->m_nVarType		= nVarType;
	memcpy(pMsg->m_bufData, buf, nSize);

	m_setMsg.push_back(pMsg);		
	LeaveCriticalSection(&m_cs);

	// �������󾯸�
	const int _warningMessagePacket=  50000;	// ���������
	int nSetSize = GetMsgSize(); 
	if(nSetSize >= _warningMessagePacket && nSetSize != m_debugPackets)	
	{
		m_debugPackets = nSetSize;
		::LogSave("Warning! CMessagePacket::PushMsg[%d] SetSize[%u].[%d][%d]", m_id, nSetSize, nPort, nPacket);
		InterlockedExchange(&g_nMessagePortErrorCount, nSetSize);
	}

	// ��������ر�
// 	const int _maxMessagePacket=400000;//20070302������.ԭ10W...����
// 	if(GetMsgSize() > _maxMessagePacket)
// 	{
// 		::LogSave("ERROR: CMessagePort::PushMsg Overflow��MessagePort[%d] Closed!", m_id);
// 		Close();
// 	}


	I64 i64TimeUsedMS = ::GetUtcMillisecond() - i64TimeStart;
	if(i64TimeUsedMS > 50)
	{
		::LogTimeOut("CMessagePort::PushMsg TimeOut: %I64dms > 50ms", i64TimeUsedMS);
	}

	return true;	
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::PopMsg(int *pPort, int *pPacket, VAR_TYPE *pVarType, void* buf)
{

	ASSERT(pPort && pPacket && pVarType && buf);

	if(m_nState != STATE_OK)		// Close()�󣬲�������Ϣ
		return false;
	
	if(GetMsgSize()==0)
		return FALSE;

	EnterCriticalSection(&m_cs);
	
	CMessagePacket* pMsg = m_setMsg.front();
	m_setMsg.pop_front();
	
	*pPort		= pMsg->m_nPortFrom;
	*pPacket	= pMsg->m_nPacket;
	*pVarType	= (VAR_TYPE)pMsg->m_nVarType;

	int	nSize = SIZE_OF_TYPE(*pVarType);
	if(nSize)
	{
		memcpy(buf, pMsg->m_bufData, nSize);
	}

	m_setRecycle.push_back(pMsg);

	LeaveCriticalSection(&m_cs);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::Init()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CMessagePort::Clear()
{
	for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_setMsg.clear();

	for(MSG_SET::iterator iter = m_setRecycle.begin();iter!=m_setRecycle.end();++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_setRecycle.clear();
}

///////////////////////////////////////////////////////////////////////////////////////
// static
///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::InitPortSet(int nPortNum)
{
	ASSERT(nPortNum >= 1);

	for(int i = 0; i < nPortNum; i++)
	{
		CMessagePort*	pPort = new CMessagePort(i);		// VVVVVVVVVVVVVVVV
		if(!pPort)
			return false;
		m_setPort.push_back(pPort);		
		if(!pPort->Init())
			return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CMessagePort::ClearPortSet()
{
	for(int i = 0; i < (int)m_setPort.size(); i++)
	{
		//m_setPort[i]->Clear();
		delete(m_setPort[i]);
	}

	m_setPort.clear();
}

///////////////////////////////////////////////////////////////////////////////////////













