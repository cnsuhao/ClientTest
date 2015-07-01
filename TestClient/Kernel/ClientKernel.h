#ifndef _CLIENT_KERNEL_H_
#define _CLIENT_KERNEL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./../Include/Include.h"
#include "./Client.h"

typedef std::map<int, ST_CLIENT_INFO>		MAP_CLIENT_INFO;

typedef std::map<SOCKET, CClient*>			MAP_CLIENT;
typedef std::set<SOCKET>					SET_SOCKET;
class CClientKernel
{
public:
	static	CClientKernel*	GetInstance (void)	{ if (!s_pInstance) s_pInstance = new CClientKernel; return s_pInstance;}
	static	void			DelInstance (void)	{ SAFE_DELETE(s_pInstance); }
	bool	Init			(void);
	void	Release			(void);

private:
	CClientKernel() {}
	virtual ~CClientKernel(){}
	CClientKernel(const CClientKernel&){}
	static CClientKernel* s_pInstance;

public:
	void		ThreadOnProcess		(void);
	void		ProcessPortMsg		(void);
	void		OnTimerClient		(void);
	void		OnTimerState		(void);

	//	SOCKET����
public:	
	CClient*	GetClient			(SOCKET idSocket);
	bool		AddClient			(int nAmount = 1);
	bool		DelClient			(int nAmount = 1);
	bool		DelClientBySocket	(SOCKET idSocket);
	bool		RemoveAllClient		(void);
	void		AddBackSocket		(SOCKET idSocket) { m_setBackScoket.insert(idSocket); }
	void		AddLoginSocket		(SOCKET idSocket) { m_setLoginScoket.insert(idSocket); }

	int			GetConnectAmount	(void) { return m_setSocket.size(); }
	int			GetOnlineAmount		(void) { return m_setLoginScoket.size(); }

	// ���ݺ���Ϊ
public:
	bool		ProcessLogin		(ST_SERVER_INFO& stServerInfo);
	bool		ProcessLogout		(ST_SERVER_INFO& stServerInfo);

	int			GetClientAmount		(void)	{ return m_setSocket.size(); }

	void		SetServerInfo		(ST_SERVER_INFO& rInfo);
	void		SynServerStaticsInfo(void);
	ST_SERVER_STATISTICS_INFO& GetServerStaticsInfo(int nServerID);

private:
	bool		GetClientInfo		(SOCKET idSocket, OUT ST_CLIENT_INFO& rInfo);
	bool		LoadCLientInfo		(void);

protected:
	IMessagePort*		m_pMsgPort;

	DWORD				m_dwOnTimerSecond;		// ÿ��
	DWORD				m_dwOnTimerSynSecond3;	// ÿ3��ͬ��

	TEST_TYPE			m_eTestType;			// ��������
	MAP_CLIENT_INFO		m_mapClientInfo;		// ���õ����в��Ի�������Ϣ

	MAP_CLIENT			m_mapClient;			// �������в��Ի�����
	SET_SOCKET			m_setSocket;			// ��������Socket

	SET_SOCKET			m_setBackScoket;		// ����������Ҫ���յ�Socket
	SET_SOCKET			m_setLoginScoket;		// �������е�½�ɹ���Socket

	ST_SERVER_INFO		m_stServerInfo;			// ��ǰ��������Ϣ
	MAP_SERVER_STATISTICS_INFO	m_mapServerStatisticsInfo;	// ������ͳ����Ϣ
};

#define pClientKernel CClientKernel::GetInstance()
#endif // _CLIENT_KERNEL_H_