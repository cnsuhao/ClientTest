#include "ClientKernel.h"

CClientKernel* CClientKernel::s_pInstance;
bool CClientKernel::Init(void)
{
	// 加载机器人测试配置
	if (!this->LoadCLientInfo())
	{
		::MessageBox(NULL, "机器人测试配置加载失败", "出错啦", MB_OK|MB_ICONERROR);
		return false;
	}

	m_pMsgPort = CMessagePort::GetInterface(MSGPORT_KERNEL);
	m_pMsgPort->Open();

	m_mapClient.clear();
	m_setSocket.clear();
	m_setBackScoket.clear();
	m_setLoginScoket.clear();
	ZeroMemory(&m_stServerInfo, sizeof(m_stServerInfo));

	m_dwOnTimerSecond	  = ::TimeGet(TIME_SECOND);
	m_dwOnTimerSynSecond3 = m_dwOnTimerSecond;
	return true;
}

void CClientKernel::Release(void)
{
	if (m_pMsgPort)
	{
		m_pMsgPort->Close();
		m_pMsgPort = NULL;
	}

	this->RemoveAllClient();
}

void CClientKernel::ThreadOnProcess(void)
{
	// 管道消息处理
	this->ProcessPortMsg();

	// 定时处理机器人测试
	this->OnTimerClient();

	// 定时处理状态
	this->OnTimerState();
}

void CClientKernel::OnTimerClient(void)
{
	// OnTimer
	if (!m_mapClient.empty())
	{
		for (auto iterClient : m_mapClient)
		{
			iterClient.second->OnTimer();
		}
	}

	// 处理回收socket
	if (!m_setBackScoket.empty())
	{
		for (auto iterBack : m_setBackScoket)
		{
			CHECKC(this->DelClientBySocket(iterBack));
		}
		m_setBackScoket.clear();
	}
}

void CClientKernel::OnTimerState(void)
{
	DWORD dwNowSecond = ::TimeGet(TIME_SECOND);
	// 每3秒同步一次服务器统计状态
	if (dwNowSecond > m_dwOnTimerSynSecond3 + 3)
	{
		this->SynServerStaticsInfo();
		m_dwOnTimerSynSecond3 = dwNowSecond;
	}

	// 每秒处理线程状态
	if (m_dwOnTimerSecond == dwNowSecond)
	{
		return;
	}
	m_dwOnTimerSecond = dwNowSecond;
}

// 管道消息处理
void CClientKernel::ProcessPortMsg(void)
{
	char			buf[MAX_MESSAGESIZE];
	CMessageStatus	cStatus;

	DEBUG_TRY;	// VVVVVVVVVVVVVV
	while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
	{
		// 暂时没有消息处理, 只提取出来
		switch (cStatus.m_nPacket)
		{
		case NETWORK_SERVER_INFO:
			{
				ST_SERVER_INFO* pServerInfo = (ST_SERVER_INFO*)buf;
				this->SetServerInfo(*pServerInfo);
			}
			break;
		case NETWORK_APPLY:
			{
				int* nClientAmount = (int*)buf;
				this->AddClient(*nClientAmount);
			}
			break;
		case NETWORK_BACK:
			{
				int* nClientAmount = (int*)buf;
				this->DelClient(*nClientAmount);
			}
			break;
		case NETWORK_LOGIN:
			{
				ST_SERVER_INFO* pServerInfo = (ST_SERVER_INFO*)buf;
				this->ProcessLogin(*pServerInfo);
			}
			break;
		case NETWORK_LOGOUT:
			{
				ST_SERVER_INFO* pServerInfo = (ST_SERVER_INFO*)buf;
				this->ProcessLogout(*pServerInfo);
			}
			break;
		default:
			break;
		}
	}
	DEBUG_CATCH("CClientKernel::ProcessPortMsg Crash!")	// AAAAAAAAAAA
}

// 添加机器人
bool CClientKernel::AddClient(int nAmount/* = 1*/)
{
	CHECKF(nAmount > 0);

	int nOldAmount = m_setSocket.size();
	for (int i = 0; i < nAmount; i++)
	{
		CClient* client = new CClient;
		CHECKC(client);
		SOCKET idSocket = m_setSocket.size();
		ST_CLIENT_INFO stClientInfo = {0};
		if (!this->GetClientInfo(idSocket, stClientInfo))
		{
			continue;
		}

		if (!client->Init(idSocket, m_stServerInfo, stClientInfo))
		{
			SAFE_RELEASE(client);
		}

		// 同步添加下面3个容器
		m_mapClient.insert(make_pair(idSocket, client));
		m_setSocket.insert(idSocket);
	}

	int nNewAmount = nOldAmount + nAmount;
	int nNowAmount = m_setSocket.size();
	int nAddAmount = nNowAmount - nOldAmount;
	if (nNewAmount != nNowAmount)
	{
		CMxyString strTip;
		strTip.Format("Server Connect Amount [%d] now, had [%d] connect faild!", nNowAmount, nNewAmount - nNowAmount);
		::MessageBox(NULL, strTip.c_str(), "Connect", MB_OK|MB_ICONINFORMATION);
	}

	// 更新统计信息
	if (nAddAmount > 0)
	{
		ST_SERVER_STATISTICS_INFO& rInfo = this->GetServerStaticsInfo(m_stServerInfo.nServerID);
		if (m_stServerInfo.nServerID == rInfo.nServerID)
		{
			rInfo.nConnect += nAddAmount;
			rInfo.nStatus  = SERVER_STATISTICS_STATE_SYN;
		}
	}
	return true;
}

// 回收机器人
bool CClientKernel::DelClient(int nAmount/* = 1*/)
{
	CHECKF(nAmount > 0);

	std::vector<SOCKET> vecDelSocket;
	std::set<SOCKET>::iterator iterDel = m_setSocket.begin();
	for (int i = 0; i < m_setSocket.size(); i++, iterDel++)
	{
		if (i >= nAmount)
		{
			break;
		}

		vecDelSocket.push_back(*iterDel);
	}

	for (auto iterDel : vecDelSocket)
	{
		this->DelClientBySocket(iterDel);
	}
	return true;	
}

// 根据socket删除机器人
bool CClientKernel::DelClientBySocket(SOCKET idSocket)
{
	auto iterSocket = m_setSocket.find(idSocket);
	CHECKF(iterSocket != m_setSocket.end());
	auto iterClient = m_mapClient.find(idSocket);
	CHECKF(iterClient != m_mapClient.end());

	auto nServerID  = iterClient->second->GetServerID();
	auto nDelOnline = iterClient->second->IsLogin() ? 1 : 0;
	SAFE_RELEASE(iterClient->second);

	m_mapClient.erase(iterClient);
	m_setSocket.erase(iterSocket);

	auto iterLoginScoket = m_setLoginScoket.find(idSocket);
	if (iterLoginScoket != m_setLoginScoket.end())
	{
		m_setLoginScoket.erase(idSocket);
	}

	// 更新统计信息
	ST_SERVER_STATISTICS_INFO& rInfo = this->GetServerStaticsInfo(nServerID);
	rInfo.nConnect -= 1;
	rInfo.nOnline  -= nDelOnline;
	rInfo.nStatus  = SERVER_STATISTICS_STATE_SYN;
	return true;
}

// 移除所有机器人
bool CClientKernel::RemoveAllClient(void)
{
	for (auto iterClient : m_mapClient)
	{
		SAFE_RELEASE(iterClient.second);
	}
	m_mapClient.clear();
	return true;
}

/////////////////////////////////////////////////////
// 处理登陆
bool CClientKernel::ProcessLogin(ST_SERVER_INFO& stServerInfo)
{
	CHECKF(stServerInfo.nServerID == m_stServerInfo.nServerID);
	for (auto iterClient : m_mapClient)
	{
		if (stServerInfo.nServerID == iterClient.second->GetServerID())
		{
			CHECKC(iterClient.second->Login());
		}
	}
	return true;
}

// 处理登出
bool CClientKernel::ProcessLogout(ST_SERVER_INFO& stServerInfo)
{
	CHECKF(stServerInfo.nServerID == m_stServerInfo.nServerID);
	for (auto iterClient : m_mapClient)
	{
		if (stServerInfo.nServerID == iterClient.second->GetServerID() && iterClient.second->IsLogin())
		{
			CHECKC(iterClient.second->Logout());
		}
	}
	return true;
}

// 根据socket获取client
CClient* CClientKernel::GetClient(SOCKET idSocket)
{
	auto iterClient = m_mapClient.find(idSocket);
	if (iterClient != m_mapClient.end())
	{
		return iterClient->second;
	}

	return NULL;
}

// 获取机器人配置信息
bool CClientKernel::GetClientInfo(SOCKET idSocket, OUT ST_CLIENT_INFO& rInfo)
{
	if (m_mapClientInfo.empty())
	{
		return false;
	}

	auto nClientID = idSocket + 1;
	switch (m_eTestType)
	{
	case TEST_TYPE_NOPING:
		{
			auto iterClientInfo = m_mapClientInfo.find(nClientID);
			if (iterClientInfo == m_mapClientInfo.end())
			{
				return false;
			}

			rInfo = iterClientInfo->second;
		}
		break;
	case TEST_TYPE_PING:
		{
			auto iterClientInfo = m_mapClientInfo.find(0);
			if (iterClientInfo == m_mapClientInfo.end())
			{
				return false;
			}

			// 账号数量不足
			if (iterClientInfo->second.nClientID < nClientID)
			{
				return false;
			}

			// 拼写账号信息
			rInfo.nClientID = nClientID;
			CMxyString strAccount;
			strAccount.Format("%s%d", iterClientInfo->second.szClientAccount, 100000 + nClientID);
			SafeCopy(rInfo.szClientAccount, strAccount.c_str(), sizeof(rInfo.szClientAccount));
			SafeCopy(rInfo.szClientPwd, iterClientInfo->second.szClientPwd, sizeof(rInfo.szClientPwd));
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

// 加载机器人配置信息
bool CClientKernel::LoadCLientInfo(void)
{
	if (!::IsExistFile(CLIENT_FILENAME))
	{
#ifdef _DEBUG
		::MessageBox(NULL, "未找到登陆测试配置文件！并检查调试目录是否设置正确", "出错啦！",MB_OK|MB_ICONERROR);
#else
		::MessageBox(NULL, "未找到登陆测试配置文件！", "出错啦！",MB_OK|MB_ICONERROR);
#endif
		return false;
	}

	m_mapClientInfo.clear();

	CIniFile ini(CLIENT_FILENAME, "Client" ); 
	m_eTestType      = (TEST_TYPE)ini.GetInt("TestType");
	int	nClientCount = ini.GetInt("ClientCount");
	switch (m_eTestType)
	{
	case TEST_TYPE_NOPING:
		{
			CMxyString strClient;
			for (int i = 1; i <= nClientCount; i++)
			{
				strClient.Format("%d", i);
				ST_CLIENT_INFO stClientInfo = {0};
				stClientInfo.nClientID = i;

				CIniFile iniServer(CLIENT_FILENAME, strClient.c_str()); 
				iniServer.GetString(stClientInfo.szClientAccount, "ClientAccount", sizeof(stClientInfo.szClientAccount));
				iniServer.GetString(stClientInfo.szClientPwd,	  "ClientPwd",	   sizeof(stClientInfo.szClientPwd));

				m_mapClientInfo[i] = stClientInfo;
			}
		}
		break;
	case TEST_TYPE_PING:
		{
			ST_CLIENT_INFO stClientInfo = {0};
			stClientInfo.nClientID = nClientCount;
			ini.GetString(stClientInfo.szClientAccount, "TestAccount", sizeof(stClientInfo.szClientAccount));
			ini.GetString(stClientInfo.szClientPwd,	    "TestPwd",	   sizeof(stClientInfo.szClientPwd));
			m_mapClientInfo[0] = stClientInfo;
		}
		break;
	default:
		return false;
		break;
	}

	return true;
}

// 设置当前服务器信息
void CClientKernel::SetServerInfo(ST_SERVER_INFO& rInfo) 
{
	memcpy(&m_stServerInfo, &rInfo, sizeof(m_stServerInfo)); 

	auto iterServer = m_mapServerStatisticsInfo.find(rInfo.nServerID);
	if (iterServer == m_mapServerStatisticsInfo.end())
	{
		ST_SERVER_STATISTICS_INFO stInfo = {0};
		stInfo.nServerID = rInfo.nServerID;
		stInfo.nStatus   = SERVER_STATISTICS_STATE_NONE;
		m_mapServerStatisticsInfo.insert(make_pair(stInfo.nServerID, stInfo));
	}
}

// 获取服务器统计信息引用，用来修改数据（谨慎使用）
ST_SERVER_STATISTICS_INFO& CClientKernel::GetServerStaticsInfo(int nServerID)
{
	ST_SERVER_STATISTICS_INFO stInfo = {0};
	auto iterServer = m_mapServerStatisticsInfo.find(nServerID);
	if (iterServer != m_mapServerStatisticsInfo.end())
	{
		return iterServer->second;
	}

	return stInfo;
}

// 同步服务器统计信息
void CClientKernel::SynServerStaticsInfo(void)
{
	if (m_mapServerStatisticsInfo.empty())
	{
		return;
	}

	for (MAP_SERVER_STATISTICS_ITER iter = m_mapServerStatisticsInfo.begin(); iter != m_mapServerStatisticsInfo.end(); iter++)
	{
		if (SERVER_STATISTICS_STATE_SYN == iter->second.nStatus)
		{
			iter->second.nStatus = SERVER_STATISTICS_STATE_NONE;
			m_pMsgPort->Send(MSGPORT_SHELL, SHELL_SERVER_STATTICS, STRUCT_TYPE(ST_SERVER_STATISTICS_INFO), &iter->second);
		}
	}
}