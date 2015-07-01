#include "Client.h"
#include "ClientKernel.h"
#include "./Msg/MsgLoginConnect.h"
#include "./Msg/MsgLoginAction.h"
#include "./Msg/MsgGameUserMac.h"
#include "./Msg/MsgGameLoginQuery.h"
#include "./Msg/MsgLoginCreateNewUser.h"

// �ò��ֳ������������������������ʹ��
const int	PROTOCOL_MSG_HEADSIZE		= 4;
struct CProtocolMsgStruct
{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	char			pMsg[1];
};

// �ֽ���Ϣ��
// return: netpacket size
inline int	SplitPacket(const char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize)
{
	if(nBufLen > PROTOCOL_MSG_HEADSIZE)
	{
		CProtocolMsgStruct*	pMsgPtr = (CProtocolMsgStruct*)pBuf;
		if(pMsgPtr->nMsgSize <= nBufLen)
		{
			if(pMsgPtr->nMsgSize < 4 || pMsgPtr->nMsgSize > _MAX_MSGSIZE)
				return 0;

			*pidPacket	= pMsgPtr->idPacket;
			*pMsgSize	= pMsgPtr->nMsgSize - PROTOCOL_MSG_HEADSIZE;
			*ppMsg		= pMsgPtr->pMsg;
			return pMsgPtr->nMsgSize;
		}
	}
	return 0;
}

bool CClient::Init(SOCKET idSocket, ST_SERVER_INFO& rServerInfo, ST_CLIENT_INFO& rClientInfo)
{
	CHECKF(idSocket >= 0);
	CHECKF(rServerInfo.szServerIP && rServerInfo.nServerPort > 1024);
	CHECKF(rClientInfo.nClientID > 0);
	
	if (!this->ConnectServer(rServerInfo.szServerIP, rServerInfo.nServerPort))
	{
		return false;
	}

	m_nServerID = rServerInfo.nServerID;
	m_socket = idSocket;
	m_dwOnTimerMinute = 0;
	m_strClientName.clear();
	this->SetMemoryDataAttr(CLIENT_ATTR_STATE, CLIENT_STATUS_INIT);
	memcpy(&m_stClientInfo, &rClientInfo, sizeof(m_stClientInfo));
	return true;
}

void CClient::Release(void)
{
	if (m_pClientSocket)
	{
		m_pClientSocket->Close();
	}
	SAFE_RELEASE(m_pClientSocket);
	delete this;
}

void CClient::OnTimer(void)
{
	if (m_pClientSocket)
	{
		m_pClientSocket->Process();
	}

	// ÿ��������������
	if (this->IsLogin())
	{
		DWORD dwNowMinuteTime = ::TimeGet(TIME_MINUTE);
		if (dwNowMinuteTime == m_dwOnTimerMinute)
		{
			return;
		}
		m_dwOnTimerMinute = dwNowMinuteTime;

		CMsgGameLoginQuery msg;
		IF_OK(msg.CreateLoginQuery(MSG_LOGIN_QUERY_ACTION_HEARTBEAT, MSG_LOGIN_QUERY_RESULT_SUCC))
		{
			this->SendMsg(msg);
		}
	}
}

// ���ӵ�������
bool CClient::ConnectServer( const char* pszIP, int nPort )
{
	CHECKF(pszIP && nPort > 1024);
	if (m_pClientSocket)
	{
		m_pClientSocket->Close();
	}
	m_pClientSocket = ::ClientSocketCreate(*this, pszIP, nPort);
	m_nMsgCount = 0;	// ���ü���
	return m_pClientSocket != NULL;
}

//  �����ر�socket
bool CClient::CloseSocket(void)
{
	if (m_pClientSocket)
	{
		m_pClientSocket->Close();
		m_pClientSocket = NULL;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  ������Ϣ��������
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: void
////////////////////////////////////////////////////////////////////////
void CClient::SendMsg( CMsg& rMsg )
{
	DEBUG_TRY;
	struct MSG_BASE
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		char buffer[MAX_PACKETSIZE];
	};

	MSG_BASE stMsgBase = {0};
	stMsgBase.unMsgType = rMsg.GetType();
	stMsgBase.unMsgSize = rMsg.GetSize() + 4;
	memcpy(stMsgBase.buffer, rMsg.GetBuf(), rMsg.GetSize());

	const char* buf = (char*)&stMsgBase;
	int nLen = rMsg.GetSize() + 4;
	CHECK(buf && nLen >= 4 && nLen <= _MAX_MSGSIZE);

	// wpe ͷ
	char	szAddWpeBuf[_MAX_MSGSIZE] = {0};
	int		nAddWpeBuf = _MAX_MSGSIZE;
	this->AddWPEInfo(buf, nLen, szAddWpeBuf, nAddWpeBuf);

	m_pClientSocket->SendMsg(szAddWpeBuf, nAddWpeBuf);

	// ����ͳ����Ϣ
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nSendMsg += 1;
		rInfo.nStatus  = SERVER_STATISTICS_STATE_SYN;
	}
	DEBUG_CATCH("CClient::SendMsg Crash!");
}

////////////////////////////////////////////////////////////////////////
// Description:  ���ӷ�ֹwpe�������ļ�������
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: void
////////////////////////////////////////////////////////////////////////
void CClient::AddWPEInfo( const char* pSrcBuffer, int nSrcLen, char* pTargetBuffer, int& nTargetLen )
{
	CHECK(pSrcBuffer && pTargetBuffer);

	// ����ԭ��Ϣ
	memcpy(pTargetBuffer, pSrcBuffer, nSrcLen);

	CProtocolMsgStruct* pSrcMsg		= (CProtocolMsgStruct*)pSrcBuffer;
	CProtocolMsgStruct*	pAddWpeMsg	= (CProtocolMsgStruct*)pTargetBuffer;

	// ���ȱ��
	nTargetLen = nSrcLen + 8;
	pAddWpeMsg->nMsgSize += 8;

	m_nMsgCount++;

	int* pCount = (int*)(pTargetBuffer + nSrcLen);
	*pCount = m_nMsgCount;

	int* pMsgKey = (int*)(pTargetBuffer + nSrcLen + 4);
	*pMsgKey = ::GetMsgKey(pSrcMsg->idPacket, m_nMsgCount);
}

//  ���ӳɹ�(�µ�socket���������ô˷���)
void CClient::OnEstablishSck(void)
{
	// ֪ͨ���ӳɹ�

}

// �ͻ��˶Ͽ����Ӵ�����socket�����ر�ʱ����ô˷���, ע���������� Close ������ã�
void CClient::OnCloseSck(void)
{
	// ֪ͨ���ӶϿ�
	pClientKernel->AddBackSocket(m_socket);
}

// �յ���Ϣ������������£��˺���Ӧ����һ��������ָ���ӽ�������������������ݴ�С��һ����Ǵ������Ϣ����С�����������-1�����رմ�socket����
int CClient::OnRcvMsg(const char* buf, int nLen)
{
	//����Ϣѹ����Ϣ���еȴ�����
	IF_NOT (buf)
	{
		return -1;
	}

	if (nLen < sizeof(unsigned short))
	{
		return 0;
	}

	OBJID idMsg = ID_NONE;
	char* pMsg = NULL;
	int	  nMsgSize = 0;
	int	  nNetPacketSize = SplitPacket(buf, nLen, &idMsg, &pMsg, &nMsgSize);
	if(!(nNetPacketSize))
	{
		return 0;
	}

	IF_NOT (nMsgSize <= _MAX_MSGSIZE)
	{
		return -1;
	}

	CMsg* pNetMsg = CMsg::CreateClientMsg(m_socket, idMsg, pMsg, nMsgSize);
	if (pNetMsg)
	{
		DEBUG_TRY;
		pNetMsg->Process(this);
		DEBUG_CATCH4("ProcessNetworkMsg  MsgType[%d], MsgLen[%d], SocketID[%d]", idMsg, nMsgSize, m_socket);
		SAFE_DELETE(pNetMsg);
	}
	else
	{
		// ��������Ϣ�ṹ�����뼴�ɶϵ㿴����Ϣ
		struct TEST_MSG
		{
			int n1;
			int n2;
		};
		TEST_MSG* pTestMsg = (TEST_MSG*)pMsg;
		int nMsgType = idMsg;		// �ڴ˶ϵ�

	}

	// ����ͳ����Ϣ
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nRecvMsg += 1;
		rInfo.nStatus  = SERVER_STATISTICS_STATE_SYN;
	}
	return nNetPacketSize;
}

///////////////////////////////////////////////////////////////
// ��½
bool CClient::Login(void)
{
	CHECKF1_NOLOG(CLIENT_STATUS_INIT == this->GetMemoryDataAttr(CLIENT_ATTR_STATE));
	CMsgLoginConnect msg;
	if (msg.CreateAccountConnect(m_stClientInfo.szClientAccount, m_stClientInfo.szClientPwd))
	{
		this->SendMsg(msg);
		this->SetMemoryDataAttr(CLIENT_ATTR_STATE, CLIENT_STATUS_CONNECT_PASSWORD);
	}
	return true;
}

// �ǳ�
bool CClient::Logout(void)
{
	pClientKernel->AddBackSocket(m_socket);
	return true;
}

// ���ܽ�ɫ������Ϣ
void CClient::ProcessReplyUserListSnap( OBJID idAccount, OBJID idUser, const char* pszName )
{
	CHECK(idAccount && idUser && pszName);
	CHECK(CLIENT_STATUS_CONNECT_PASSWORD == this->GetMemoryDataAttr(CLIENT_ATTR_STATE));

	this->SetMemoryDataAttr(CLIENT_ATTR_ACCOUNT_ID, idAccount);

	this->SetMemoryDataAttr(CLIENT_ATTR_STATE, CLIENT_STATUS_CONNECT_CHOOSE_PLAYER);
	if (ID_NONE != idUser)
	{
		CMsgLoginAction msg;
		CHECK(msg.CreateChooseUser(idUser));
		this->SendMsg(msg);
	}
	else  // ������ɫ
	{
		CMsgLoginCreateNewUser msg;

	}

}

// ������Ϸ���������UserInfo
void CClient::ProcessReplyUserInfo( OBJID idUser, int nAccountType, const char* pszUserName )
{
	CHECK(idUser);
	CHECK(CLIENT_STATUS_CONNECT_CHOOSE_PLAYER == this->GetMemoryDataAttr(CLIENT_ATTR_STATE));
	CHECK(pszUserName);

	this->SetMemoryDataAttr(CLIENT_ATTR_USER_ID, idUser);
	m_strClientName = pszUserName;

	// ����Ȩ��
	this->SetMemoryDataAttr(CLIENT_ATTR_RANK, (CLIENT_RANK)nAccountType);

	// gm���ߵ�½GM����Ҫmac��ַ��֤
	if (this->GetMemoryDataAttr(CLIENT_ATTR_RANK) > CLIENT_RANK_NONE)
	{
		char szMac[64] = "";
		::MyGetMAC(szMac, 0);
		CMsgGameUserMac msgUserMac;
		CHECK(msgUserMac.CreateMac(szMac));
		this->SendMsg(msgUserMac);
	}

	// ���͵�½��ѯ
	CMsgGameLoginQuery msg;
	for (int nActionIndex = MSG_LOGIN_QUERY_ACTION_AUTOBACK_BEGIN + 1; nActionIndex < MSG_LOGIN_QUERY_ACTION_AUTOBACK_END; nActionIndex++)
	{
		IF_OK(msg.CreateLoginQuery((MSG_LOGIN_QUERY_ACTION)nActionIndex, MSG_LOGIN_QUERY_RESULT_SUCC))
		{
			this->SendMsg(msg);
		}
	}

	// ���͵�¼���
	IF_OK(msg.CreateLoginQuery(MSG_LOGIN_QUERY_ACTION_CLIENT_LOGIN_OVER, MSG_LOGIN_QUERY_RESULT_SUCC))
	{
		this->SendMsg(msg);
	}
	this->SetMemoryDataAttr(CLIENT_ATTR_STATE, CLIENT_STATUS_RUNNING);

	pClientKernel->AddLoginSocket(m_socket);

	// ����ͳ����Ϣ
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nOnline += 1;
		rInfo.nStatus = SERVER_STATISTICS_STATE_SYN;
	}
}

// ������
void CClient::ProcessBeKick(int nReason, int nParam)
{
	// ���õ�½ʧ��״̬
	this->SetMemoryDataAttr(CLIENT_ATTR_STATE, CLIENT_STATUS_LOGIN_FAILD);

	pClientKernel->AddBackSocket(m_socket);

	// ����ͳ����Ϣ
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nBreakCount += 1;
		rInfo.nStatus = SERVER_STATISTICS_STATE_SYN;
	}
}