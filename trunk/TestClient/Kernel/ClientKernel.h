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

	//	SOCKET连接
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

	// 数据和行为
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

	DWORD				m_dwOnTimerSecond;		// 每秒
	DWORD				m_dwOnTimerSynSecond3;	// 每3秒同步

	TEST_TYPE			m_eTestType;			// 测试类型
	MAP_CLIENT_INFO		m_mapClientInfo;		// 配置的所有测试机器人信息

	MAP_CLIENT			m_mapClient;			// 包含所有测试机器人
	SET_SOCKET			m_setSocket;			// 包含所有Socket

	SET_SOCKET			m_setBackScoket;		// 包含所有需要回收的Socket
	SET_SOCKET			m_setLoginScoket;		// 包含所有登陆成功的Socket

	ST_SERVER_INFO		m_stServerInfo;			// 当前服务器信息
	MAP_SERVER_STATISTICS_INFO	m_mapServerStatisticsInfo;	// 服务器统计信息
};

#define pClientKernel CClientKernel::GetInstance()
#endif // _CLIENT_KERNEL_H_