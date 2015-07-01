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

	// ��ʼ�����ͷ�
public:
	bool	Init			(SOCKET idSocket, ST_SERVER_INFO& rServerInfo, ST_CLIENT_INFO& rClientInfo);
	void	Release			(void);
	void	OnTimer			(void);

	// �̳к���
public:
	virtual int		OnRcvMsg		(const char* buf, int nLen);
	virtual void	OnEstablishSck	(void);
	virtual void	OnCloseSck		(void);

	// �������
public:
	bool	ConnectServer	(const char* pszIP, int nPort);
	bool	CloseSocket		(void);
	void	SendMsg			(CMsg& rMsg);
	void	AddWPEInfo		(const char* pSrcBuffer, int nSrcLen, char* pTargetBuffer, int& nTargetLen);

	// ��Ϊ���
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
	SOCKET				m_socket;			// socket��ʶ
	IClientSocket*		m_pClientSocket;	// �ͻ���socket
	int					m_nMsgCount;		// ��Ϣ����β
	ST_CLIENT_INFO		m_stClientInfo;		// �����˺���Ϣ
	int					m_nServerID;		// ��½������ID

	CMxyString			m_strClientName;	// ����������

	DWORD				m_dwOnTimerMinute;	// ÿ��������ͬ��

public:
	DEFINE_mda(CLIENT_ATTR);
};

#endif	// _MYCLIENT_H_