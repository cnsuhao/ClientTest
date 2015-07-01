#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_

#include "windows.h"
#include "./IMySocket.h"
#include "./../Include/Include.h"

class CMsg;
class CClient : IClientSocketEvent
{
public:
	CClient() : m_socket(NULL), m_pClientSocket(NULL), m_nMsgCount(0) {}
	virtual ~CClient(){}

	// 初始化与释放
public:
	bool	Init			(SOCKET idSocket, ST_SERVER_INFO& rServerInfo, ST_CLIENT_INFO& rClientInfo);
	void	Release			(void);
	void	OnTimer			(void);

	// 继承函数
public:
	virtual int		OnRcvMsg		(const char* buf, int nLen);
	virtual void	OnEstablishSck	(void);
	virtual void	OnCloseSck		(void);

	// 连接相关
public:
	bool	ConnectServer	(const char* pszIP, int nPort);
	bool	CloseSocket		(void);
	void	SendMsg			(CMsg& rMsg);
	void	AddWPEInfo		(const char* pSrcBuffer, int nSrcLen, char* pTargetBuffer, int& nTargetLen);

	// 行为相关
public:
	bool	Login			(void);
	bool	Logout			(void);
	bool	IsLogin			(void)	{ return CLIENT_STATUS_RUNNING == this->GetMemoryDataAttr(CLIENT_ATTR_STATE); }

	void	ProcessReplyUserListSnap( OBJID idAccount, OBJID idUser, const char* pszName );
	void	ProcessReplyUserInfo( OBJID idUser, int nAccountType, const char* pszUserName );
	void	ProcessBeKick(int nReason, int nParam);

public:
	int		GetServerID		(void) { return m_nServerID; }
	SOCKET	GetSocket		(void) { return m_socket; }

protected:
	SOCKET				m_socket;			// socket标识
	IClientSocket*		m_pClientSocket;	// 客户端socket
	int					m_nMsgCount;		// 消息计数尾
	ST_CLIENT_INFO		m_stClientInfo;		// 测试账号信息
	int					m_nServerID;		// 登陆服务器ID

	CMxyString			m_strClientName;	// 机器人名字

	DWORD				m_dwOnTimerMinute;	// 每分钟心跳同步

public:
	DEFINE_mda(CLIENT_ATTR);
};

#endif	// _MYCLIENT_H_