#include "Client.h"
#include "ClientKernel.h"
#include "./Msg/MsgLoginConnect.h"
#include "./Msg/MsgLoginAction.h"
#include "./Msg/MsgGameUserMac.h"
#include "./Msg/MsgGameLoginQuery.h"
#include "./Msg/MsgLoginCreateNewUser.h"

// 该部分常量、定义仅由以下两个函数使用
const int	PROTOCOL_MSG_HEADSIZE		= 4;
struct CProtocolMsgStruct
{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	char			pMsg[1];
};

// 分解消息包
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

	// 每分钟请求心跳包
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

// 连接到服务器
bool CClient::ConnectServer( const char* pszIP, int nPort )
{
	CHECKF(pszIP && nPort > 1024);
	if (m_pClientSocket)
	{
		m_pClientSocket->Close();
	}
	m_pClientSocket = ::ClientSocketCreate(*this, pszIP, nPort);
	m_nMsgCount = 0;	// 重置计数
	return m_pClientSocket != NULL;
}

//  主动关闭socket
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
// Description:  发送消息给服务器
// Arguments:
// Author: 彭文奇(Peng Wenqi)
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

	// wpe 头
	char	szAddWpeBuf[_MAX_MSGSIZE] = {0};
	int		nAddWpeBuf = _MAX_MSGSIZE;
	this->AddWPEInfo(buf, nLen, szAddWpeBuf, nAddWpeBuf);

	m_pClientSocket->SendMsg(szAddWpeBuf, nAddWpeBuf);

	// 更新统计信息
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nSendMsg += 1;
		rInfo.nStatus  = SERVER_STATISTICS_STATE_SYN;
	}
	DEBUG_CATCH("CClient::SendMsg Crash!");
}

////////////////////////////////////////////////////////////////////////
// Description:  增加防止wpe攻击包的计数函数
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: void
////////////////////////////////////////////////////////////////////////
void CClient::AddWPEInfo( const char* pSrcBuffer, int nSrcLen, char* pTargetBuffer, int& nTargetLen )
{
	CHECK(pSrcBuffer && pTargetBuffer);

	// 复制原消息
	memcpy(pTargetBuffer, pSrcBuffer, nSrcLen);

	CProtocolMsgStruct* pSrcMsg		= (CProtocolMsgStruct*)pSrcBuffer;
	CProtocolMsgStruct*	pAddWpeMsg	= (CProtocolMsgStruct*)pTargetBuffer;

	// 长度变更
	nTargetLen = nSrcLen + 8;
	pAddWpeMsg->nMsgSize += 8;

	m_nMsgCount++;

	int* pCount = (int*)(pTargetBuffer + nSrcLen);
	*pCount = m_nMsgCount;

	int* pMsgKey = (int*)(pTargetBuffer + nSrcLen + 4);
	*pMsgKey = ::GetMsgKey(pSrcMsg->idPacket, m_nMsgCount);
}

//  连接成功(新的socket建立后会调用此方法)
void CClient::OnEstablishSck(void)
{
	// 通知连接成功

}

// 客户端断开连接触发（socket被动关闭时会调用此方法, 注意主动调用 Close 不会调用）
void CClient::OnCloseSck(void)
{
	// 通知连接断开
	pClientKernel->AddBackSocket(m_socket);
}

// 收到消息处理（正常情况下，此函数应返回一个正整数指明从接受数据区清除掉的数据大小（一般就是处理的消息包大小）；如果返回-1，则会关闭此socket。）
int CClient::OnRcvMsg(const char* buf, int nLen)
{
	//将消息压入消息队列等待处理
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
		// 将测试消息结构体填入即可断点看到消息
		struct TEST_MSG
		{
			int n1;
			int n2;
		};
		TEST_MSG* pTestMsg = (TEST_MSG*)pMsg;
		int nMsgType = idMsg;		// 在此断点

	}

	// 更新统计信息
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nRecvMsg += 1;
		rInfo.nStatus  = SERVER_STATISTICS_STATE_SYN;
	}
	return nNetPacketSize;
}

///////////////////////////////////////////////////////////////
// 登陆
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

// 登出
bool CClient::Logout(void)
{
	pClientKernel->AddBackSocket(m_socket);
	return true;
}

// 接受角色快照信息
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
	else  // 创建角色
	{
		CMsgLoginCreateNewUser msg;

	}

}

// 处理游戏服返回玩家UserInfo
void CClient::ProcessReplyUserInfo( OBJID idUser, int nAccountType, const char* pszUserName )
{
	CHECK(idUser);
	CHECK(CLIENT_STATUS_CONNECT_CHOOSE_PLAYER == this->GetMemoryDataAttr(CLIENT_ATTR_STATE));
	CHECK(pszUserName);

	this->SetMemoryDataAttr(CLIENT_ATTR_USER_ID, idUser);
	m_strClientName = pszUserName;

	// 设置权限
	this->SetMemoryDataAttr(CLIENT_ATTR_RANK, (CLIENT_RANK)nAccountType);

	// gm工具登陆GM号需要mac地址验证
	if (this->GetMemoryDataAttr(CLIENT_ATTR_RANK) > CLIENT_RANK_NONE)
	{
		char szMac[64] = "";
		::MyGetMAC(szMac, 0);
		CMsgGameUserMac msgUserMac;
		CHECK(msgUserMac.CreateMac(szMac));
		this->SendMsg(msgUserMac);
	}

	// 发送登陆查询
	CMsgGameLoginQuery msg;
	for (int nActionIndex = MSG_LOGIN_QUERY_ACTION_AUTOBACK_BEGIN + 1; nActionIndex < MSG_LOGIN_QUERY_ACTION_AUTOBACK_END; nActionIndex++)
	{
		IF_OK(msg.CreateLoginQuery((MSG_LOGIN_QUERY_ACTION)nActionIndex, MSG_LOGIN_QUERY_RESULT_SUCC))
		{
			this->SendMsg(msg);
		}
	}

	// 发送登录完成
	IF_OK(msg.CreateLoginQuery(MSG_LOGIN_QUERY_ACTION_CLIENT_LOGIN_OVER, MSG_LOGIN_QUERY_RESULT_SUCC))
	{
		this->SendMsg(msg);
	}
	this->SetMemoryDataAttr(CLIENT_ATTR_STATE, CLIENT_STATUS_RUNNING);

	pClientKernel->AddLoginSocket(m_socket);

	// 更新统计信息
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nOnline += 1;
		rInfo.nStatus = SERVER_STATISTICS_STATE_SYN;
	}
}

// 处理被踢
void CClient::ProcessBeKick(int nReason, int nParam)
{
	// 设置登陆失败状态
	this->SetMemoryDataAttr(CLIENT_ATTR_STATE, CLIENT_STATUS_LOGIN_FAILD);

	pClientKernel->AddBackSocket(m_socket);

	// 更新统计信息
	ST_SERVER_STATISTICS_INFO& rInfo = pClientKernel->GetServerStaticsInfo(m_nServerID);
	if (m_nServerID == rInfo.nServerID)
	{
		rInfo.nBreakCount += 1;
		rInfo.nStatus = SERVER_STATISTICS_STATE_SYN;
	}
}