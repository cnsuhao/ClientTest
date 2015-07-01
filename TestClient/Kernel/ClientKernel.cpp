#include "ClientKernel.h"

CClientKernel* CClientKernel::s_pInstance;
bool CClientKernel::Init(void)
{
	// ���ػ����˲�������
	if (!this->LoadCLientInfo())
	{
		::MessageBox(NULL, "�����˲������ü���ʧ��", "������", MB_OK|MB_ICONERROR);
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
	// �ܵ���Ϣ����
	this->ProcessPortMsg();

	// ��ʱ��������˲���
	this->OnTimerClient();

	// ��ʱ����״̬
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

	// �������socket
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
	// ÿ3��ͬ��һ�η�����ͳ��״̬
	if (dwNowSecond > m_dwOnTimerSynSecond3 + 3)
	{
		this->SynServerStaticsInfo();
		m_dwOnTimerSynSecond3 = dwNowSecond;
	}

	// ÿ�봦���߳�״̬
	if (m_dwOnTimerSecond == dwNowSecond)
	{
		return;
	}
	m_dwOnTimerSecond = dwNowSecond;
}

// �ܵ���Ϣ����
void CClientKernel::ProcessPortMsg(void)
{
	char			buf[MAX_MESSAGESIZE];
	CMessageStatus	cStatus;

	DEBUG_TRY;	// VVVVVVVVVVVVVV
	while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
	{
		// ��ʱû����Ϣ����, ֻ��ȡ����
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

// ��ӻ�����
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

		// ͬ���������3������
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

	// ����ͳ����Ϣ
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

// ���ջ�����
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

// ����socketɾ��������
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

	// ����ͳ����Ϣ
	ST_SERVER_STATISTICS_INFO& rInfo = this->GetServerStaticsInfo(nServerID);
	rInfo.nConnect -= 1;
	rInfo.nOnline  -= nDelOnline;
	rInfo.nStatus  = SERVER_STATISTICS_STATE_SYN;
	return true;
}

// �Ƴ����л�����
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
// �����½
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

// ����ǳ�
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

// ����socket��ȡclient
CClient* CClientKernel::GetClient(SOCKET idSocket)
{
	auto iterClient = m_mapClient.find(idSocket);
	if (iterClient != m_mapClient.end())
	{
		return iterClient->second;
	}

	return NULL;
}

// ��ȡ������������Ϣ
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

			// �˺���������
			if (iterClientInfo->second.nClientID < nClientID)
			{
				return false;
			}

			// ƴд�˺���Ϣ
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

// ���ػ�����������Ϣ
bool CClientKernel::LoadCLientInfo(void)
{
	if (!::IsExistFile(CLIENT_FILENAME))
	{
#ifdef _DEBUG
		::MessageBox(NULL, "δ�ҵ���½���������ļ�����������Ŀ¼�Ƿ�������ȷ", "��������",MB_OK|MB_ICONERROR);
#else
		::MessageBox(NULL, "δ�ҵ���½���������ļ���", "��������",MB_OK|MB_ICONERROR);
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

// ���õ�ǰ��������Ϣ
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

// ��ȡ������ͳ����Ϣ���ã������޸����ݣ�����ʹ�ã�
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

// ͬ��������ͳ����Ϣ
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