// �ӿ��ࡣ�ӿڣ�������ϢͨѶ���Ƶ��̼߳�ͨѶ�ӿڡ�
// �ɽ��ޣ�2002.8.28

#ifndef	MESSAGEPORT_H
#define MESSAGEPORT_H

#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

#pragma warning(disable:4786)
#include <list>
#include <vector>
#include "windows.h"
#include "./I_MessagePort.h"
#include "./MyHeap.h"
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////
// ͨ�ö���
/////////////////////////////////////////////////////////////////////////////////////////////////
MSGPORT_BEGIN
const int	MAX_MSGPACKSIZE = 2048;			// ��Ϣ�����ݵ����ߴ�
struct	CMessagePacket
{
public:
	unsigned int		m_nPortFrom;
	unsigned int		m_nPacket;
	unsigned int		m_nVarType;
	char	m_bufData[MAX_MSGPACKSIZE];
	MYHEAP_DECLARATION(s_heap);
};
MSGPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
class	CMessagePort : private IMessagePort
{
protected:
	CMessagePort(int nPort) 
	{ 
		m_id			= nPort; 
		m_nState		= STATE_CLOSED; 
		m_debugPackets	= 0;
		InitializeCriticalSection(&m_cs);
	}
	virtual ~CMessagePort	() 
	{ 
		EnterCriticalSection(&m_cs);
		Clear(); 
		LeaveCriticalSection(&m_cs);
		DeleteCriticalSection(&m_cs);
	}

	IMessagePort*	GetInterface() { return (IMessagePort*)this; }

protected: // Interface
	virtual bool	IsOpen	() {  return m_nState == STATE_OK; }

	// ȡ���ӿڵ�ID��
	virtual int		GetID	() {  return m_id; }
	virtual int		GetSize	() { return m_setPort.size(); }

	// ��ʼ�������ýӿ�ID�ţ���ʼ������Ϣ�����ظ�����(PORT_ID���ܸı�)��
	virtual bool	Open	();
	// �رսӿڣ����ٽ�����Ϣ�����ظ����á�
	virtual bool	Close	();

	// ������Ϣ��ָ���ӿڡ�������ϢID���������͡����ݡ�return false: ���Ͳ��ɹ������Ѿ��ر�
	virtual bool	Send	(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf);
	// ����ָ���ӿ�(�����нӿ�)��������Ϣ����ָ����ϢID��Ҳ�ɲ�ָ����VARTYPE�����塣return false: û���յ�����
	virtual bool	Recv	(int nPort, int nPacket, VAR_TYPE nVarType, void* buf, CMessageStatus *pStatus = NULL);

protected:	// ������ú�������Ҫ����
	bool		PushMsg		(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf);

	size_t		GetMsgSize()	{ return m_setMsg.size(); }//20070518������:public��֤����
private:	// �ڲ�����������Ҫ����
	bool		PopMsg		(int *nPort, int *nPacket, VAR_TYPE *nVarType, void* buf);
	static int	SIZE_OF_TYPE		(int type);

	// ��SET�ĺ�������
	bool		Init();
	void		Clear();

protected:
	int			m_id;
	enum { STATE_OK, STATE_CLOSED };
	int			m_nState;
	typedef	list<message_port::CMessagePacket*>	MSG_SET;			//?? �ɽ��л���
	MSG_SET		m_setMsg;
	CRITICAL_SECTION m_cs;
	int			m_debugPackets;
	MSG_SET		m_setRecycle;

////////////////////////////////////////////////////////////////////////////////////////
// ���þ�̬����
// ���нӿڵ���ӱ������״�ͨѶǰ��ɡ�ͨѶ�ڼ�ӿڼ������б仯���������ͨѶ�󣬲�����սӿڡ�
// ע�⣺Ŀǰֻ֧�֡���̬���ӿڼ���(��ò��ֲ�֧���̰߳�ȫ��)
public:
	static bool	InitPortSet(int nPortNum);
	static void	ClearPortSet();
	static IMessagePort*	GetInterface(int nPort) 
	{
		if (nPort >= 0 && nPort < (int)m_setPort.size()) 
		{
			return m_setPort[nPort]->GetInterface(); 
		}
		else 
		{
			return NULL;
		}
	}

	static int	GetQueueSize(int nPort)				
	{
		if(nPort >= 0 && nPort < (int)m_setPort.size()) 
		{
			return m_setPort[nPort]->m_setMsg.size(); 
		}
		else 
		{
			return -1;
		}
	}

protected: //static
	typedef	vector<CMessagePort*>	PORT_SET;			
	static PORT_SET					m_setPort;	// ����̬���������ڼ䲻�޸ġ�
};


#endif // MESSAGEPORT_H














