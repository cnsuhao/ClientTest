
// TestClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestClient.h"
#include "TestClientDlg.h"
#include "afxdialogex.h"
#include "version.h"

#pragma comment(lib, "../Lib/DbgHelp.Lib")
#pragma comment(lib, "../Lib/MyNet.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestClientDlg �Ի���



CTestClientDlg::CTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestClientDlg::IDD, pParent)
	, m_pMsgPort(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CTestClientDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CTestClientDlg::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CTestClientDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, &CTestClientDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_BACK, &CTestClientDlg::OnBnClickedBtnBack)
	ON_CBN_SELCHANGE(IDC_CMB_SERVER, &CTestClientDlg::OnCbnSelchangeCmbServer)
END_MESSAGE_MAP()


// CTestClientDlg ��Ϣ�������

BOOL CTestClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	// ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// ����ģʽ����ڴ��й¶
	//_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	//_CrtSetBreakAlloc(0x005C4BE0);
	m_eState = DLG_STATUS_FAILD;

	// ���ó������
	CString strVision;
	strVision.Format(SERVER_TITLE, ::GetCurrentProcessId(), __DATE__, __TIME__, VER_SERVER_SVN_VISION);
	CString strTitle;
	GetWindowText(strTitle);
	strTitle += strVision;
	SetWindowText(strTitle);

	// ���ط���������
	if (!this->LoadServerInfo())
	{
		return FALSE;
	}
	this->InitCtrlItemView();
	
	// �̹߳ܵ���ʼ��
	CMessagePort::InitPortSet(MSGPORT_SIZE);
	m_pMsgPort = CMessagePort::GetInterface(MSGPORT_SHELL);
	m_pMsgPort->Open();

	// ��ʼ������
	m_pClientThread = new CClientThread();
	CHECKF(m_pClientThread);
	m_pClientThread->CreateThread(true);
	//m_mapNetWorkThread[nServerID] = pThread;
	//pThread->ResumeThread();

	// ������־Ŀ¼
	CreateDirectory(LOGFILE_DIR, NULL);	
	//InitLog(strTitle, time(NULL));			// ��ʼ����־ȫ�ֱ���

	m_eState = DLG_STATUS_INIT;

	// �������ontimer
	SetTimer(1, 1000, NULL);
	//_CrtDumpMemoryLeaks();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// ��ȡ��������Ϣ
bool CTestClientDlg::GetServerInfo(int nServerId, ST_SERVER_INFO& rInfo)
{
	if (m_mapServerInfo.empty())
	{
		return false;
	}

	auto iterServerInfo = m_mapServerInfo.find(nServerId);
	if (iterServerInfo == m_mapServerInfo.end())
	{
		return false;
	}

	rInfo = iterServerInfo->second;
	return true;
}

// ���ط�������Ϣ
bool CTestClientDlg::LoadServerInfo(void)
{
	if (!::IsExistFile(SERVER_LIST_FILENAME))
	{
#ifdef _DEBUG
		::MessageBox(NULL, "δ�ҵ������������ļ�����������Ŀ¼�Ƿ�������ȷ", "��������",MB_OK|MB_ICONERROR);
#else
		::MessageBox(NULL, "δ�ҵ������������ļ���", "��������",MB_OK|MB_ICONERROR);
#endif
		return false;
	}

	CIniFile ini(SERVER_LIST_FILENAME, "ServerList" ); 
	int	nServerCount = ini.GetInt("ServerCount");
	CMxyString strServer;
	for (int i = 1; i <= nServerCount; i++)
	{
		strServer.Format("%d", i);
		ST_SERVER_INFO stServerInfo = {0};
		stServerInfo.nServerID = i;

		CIniFile iniServer(SERVER_LIST_FILENAME, strServer.c_str()); 
		iniServer.GetString(stServerInfo.szServerName, "ServerName", sizeof(stServerInfo.szServerName));
		iniServer.GetString(stServerInfo.szServerIP,   "ServerIP", sizeof(stServerInfo.szServerIP));
		stServerInfo.nServerPort = iniServer.GetInt("ServerPort");

		m_mapServerInfo[i] = stServerInfo;
	}
	return true;
}

// ��ʼ���ؼ���ʾ
bool CTestClientDlg::InitCtrlItemView(void)
{
	SetDlgItemText(IDC_EDT_APPLY, "1");
	((CButton*)GetDlgItem(IDC_CHK_ALL))->SetCheck(true);

	// ״̬��ʾ�ؼ�
	if (!m_mapServerInfo.empty())
	{
		auto pListCtrlState = ((CListCtrl*)GetDlgItem(IDC_LIST_STATE));
		pListCtrlState->ModifyStyle(0, LVS_REPORT);
		pListCtrlState->InsertColumn(0, _T("Server"), LVCFMT_CENTER, 75);

		pListCtrlState->InsertItem(0, _T("ID"));
		pListCtrlState->InsertItem(1, _T("IP"));
		pListCtrlState->InsertItem(2, _T("Connect"));
		pListCtrlState->InsertItem(3, _T("Online"));
		pListCtrlState->InsertItem(4, _T("RecvMsg"));
		pListCtrlState->InsertItem(5, _T("SendMsg"));
		pListCtrlState->InsertItem(6, _T("BreakCount"));

		auto pComboxServer = ((CComboBox*)GetDlgItem(IDC_CMB_SERVER));
		for (auto iterServerInfo : m_mapServerInfo)
		{
			pComboxServer->AddString(iterServerInfo.second.szServerName);

			pListCtrlState->InsertColumn(iterServerInfo.second.nServerID, iterServerInfo.second.szServerName, LVCFMT_CENTER, 120);

			CMxyString strServerID;
			strServerID.Format("%d", iterServerInfo.second.nServerID);
			pListCtrlState->SetItemText(0, iterServerInfo.second.nServerID, strServerID.c_str());
			pListCtrlState->SetItemText(1, iterServerInfo.second.nServerID, iterServerInfo.second.szServerIP);
		}
		pComboxServer->SetCurSel(0);

		// ͳ����
		auto nStatistics = m_mapServerInfo.size() + 1;
		pListCtrlState->InsertColumn(nStatistics, _T("STATISTICS All"), LVCFMT_CENTER, 120);
		pListCtrlState->SetItemText(0, nStatistics, _T("N/A"));
		pListCtrlState->SetItemText(1, nStatistics, _T("N/A"));
		pListCtrlState->SetExtendedStyle(pListCtrlState->GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	}

	return true;
}

// ���������ͳ����Ϣ
void CTestClientDlg::ProcessServerStatic(ST_SERVER_STATISTICS_INFO& rInfo)
{
	auto pListCtrlState = ((CListCtrl*)GetDlgItem(IDC_LIST_STATE));
	CMxyString strString;
	strString.Format("%d", rInfo.nConnect);
	pListCtrlState->SetItemText(2, rInfo.nServerID, strString.c_str());
	strString.Format("%d", rInfo.nOnline);
	pListCtrlState->SetItemText(3, rInfo.nServerID, strString.c_str());
	strString.Format("%d", rInfo.nRecvMsg);
	pListCtrlState->SetItemText(4, rInfo.nServerID, strString.c_str());
	strString.Format("%d", rInfo.nSendMsg);
	pListCtrlState->SetItemText(5, rInfo.nServerID, strString.c_str());
	strString.Format("%d", rInfo.nBreakCount);
	pListCtrlState->SetItemText(6, rInfo.nServerID, strString.c_str());

	MAP_SERVER_STATISTICS_ITER iter = m_mapServerStatisticsInfo.find(rInfo.nServerID);
	if (iter == m_mapServerStatisticsInfo.end())
	{
		m_mapServerStatisticsInfo[rInfo.nServerID] = rInfo;
	}
	else
	{
		memcpy(&iter->second, &rInfo, sizeof(iter->second));
	}

	// ͳ����ֵ
	auto nTotalConnect = 0;
	auto nTotalOnline  = 0;
	auto nTotalBreakCount  = 0;
	for (auto iterServer : m_mapServerStatisticsInfo)
	{
		nTotalConnect += iterServer.second.nConnect;
		nTotalOnline  += iterServer.second.nOnline;
		nTotalBreakCount += iterServer.second.nBreakCount;
	}

	auto nStatistics = m_mapServerInfo.size() + 1;
	strString.Format("%d", nTotalConnect);
	pListCtrlState->SetItemText(2, nStatistics, strString.c_str());
	strString.Format("%d", nTotalOnline);
	pListCtrlState->SetItemText(3, nStatistics, strString.c_str());
	pListCtrlState->SetItemText(4, nStatistics, _T("-"));
	pListCtrlState->SetItemText(5, nStatistics, _T("-"));
	strString.Format("%d", nTotalBreakCount);
	pListCtrlState->SetItemText(6, nStatistics, strString.c_str());
}

void CTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ʱ����
void CTestClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateData(TRUE);
	// ����Shell��Ϣ
	this->ProcessPortMsg();

	// ��ʱ���������״̬
	this->OnTimerServerState();

	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

////////////////////////////////////////////////////////////////////////
// Description:  ����Ի�����Ϣ
// Arguments:
// Author: �½���(Chen Jianjun)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CTestClientDlg::ProcessPortMsg( void )
{
	char			buf[MAX_MESSAGESIZE];
	CMessageStatus	cStatus;

	DEBUG_TRY	// VVVVVVVVVVVVVV
	while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
	{
		// (cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom)
		switch (cStatus.m_nPacket)
		{
		case SHELL_SERVER_STATTICS:
			{
				ST_SERVER_STATISTICS_INFO* stInfo = (ST_SERVER_STATISTICS_INFO*)buf;
				this->ProcessServerStatic(*stInfo);
			}
			break;
		default:
			break;
		}
	}
	DEBUG_CATCH("CTestClientDlg::ProcessMsg Crash!")	// AAAAAAAAAAA
}

// ��ʱ���������״̬
void CTestClientDlg::OnTimerServerState(void)
{
	switch (m_eState)
	{
	case DLG_STATUS_FAILD:
		{
			this->OnBnClickedBtnClose();
		}
		break;
	case DLG_STATUS_INIT:
		{
			// Ĭ��ѡ���һ������������
			ST_SERVER_INFO stServerInfo = {0};
			if (!GetServerInfo(1, stServerInfo))
			{
				::MessageBox(NULL, "���������ò���ȷ", "������", MB_OK|MB_ICONERROR);
				m_eState = DLG_STATUS_SERVER_ERROR;
			}
			else
			{
				m_pMsgPort->Send(MSGPORT_KERNEL, NETWORK_SERVER_INFO, STRUCT_TYPE(ST_SERVER_INFO), &stServerInfo);
				m_eState = DLG_STATUS_RUNNING;
			}
		}
		break;
	case DLG_STATUS_RUNNING:
		break;
	case DLG_STATUS_SERVER_WAIT:
	case DLG_STATUS_SERVER_ERROR:
		break;
	default:
		break;
	}
}

void CTestClientDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDCANCEL == this->MessageBox("Are You Sure Close?", "Close" , MB_ICONEXCLAMATION | MB_OKCANCEL)) 
	{
		return; 
	}

	if (m_pClientThread)
	{
		m_pClientThread->CloseThread(30000);
		SAFE_DELETE(m_pClientThread);
	}

	if (m_pMsgPort)
	{
		m_pMsgPort->Close();
		CMessagePort::ClearPortSet();
		m_pMsgPort = NULL;
	}
	
	Sleep(1000);
	OnCancel();
}


void CTestClientDlg::OnBnClickedBtnApply()
{
	CHECK(this->IsServerRuning());

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrClientAmount;
	GetDlgItem(IDC_EDT_APPLY)->GetWindowText(cstrClientAmount);

	char* pcClientAmount = cstrClientAmount.GetBuffer(cstrClientAmount.GetLength());
	int   nClientAmount  = atoi(pcClientAmount);
	m_pMsgPort->Send(MSGPORT_KERNEL, NETWORK_APPLY, STRUCT_TYPE(int), &nClientAmount);
}


void CTestClientDlg::OnBnClickedBtnBack()
{
	CHECK(this->IsServerRuning());

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrClientAmount;
	GetDlgItem(IDC_EDT_APPLY)->GetWindowText(cstrClientAmount);

	char* pcClientAmount = cstrClientAmount.GetBuffer(cstrClientAmount.GetLength());
	int   nClientAmount  = atoi(pcClientAmount);
	m_pMsgPort->Send(MSGPORT_KERNEL, NETWORK_BACK, STRUCT_TYPE(int), &nClientAmount);
}


void CTestClientDlg::OnBnClickedBtnLogin()
{
	CHECK(this->IsServerRuning());

	int nServerID = ((CComboBox*)GetDlgItem(IDC_CMB_SERVER))->GetCurSel() + 1;
	ST_SERVER_INFO stServerInfo = {0};
	CHECK(GetServerInfo(nServerID, stServerInfo));

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pMsgPort->Send(MSGPORT_KERNEL, NETWORK_LOGIN, STRUCT_TYPE(ST_SERVER_INFO), &stServerInfo);
}


void CTestClientDlg::OnBnClickedBtnLogout()
{
	CHECK(this->IsServerRuning());

	int nServerID = ((CComboBox*)GetDlgItem(IDC_CMB_SERVER))->GetCurSel() + 1;
	ST_SERVER_INFO stServerInfo = {0};
	CHECK(GetServerInfo(nServerID, stServerInfo));

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pMsgPort->Send(MSGPORT_KERNEL, NETWORK_LOGOUT, STRUCT_TYPE(ST_SERVER_INFO), &stServerInfo);
}


void CTestClientDlg::OnCbnSelchangeCmbServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CHECK(this->IsServerRuning());

	int nServerID = ((CComboBox*)GetDlgItem(IDC_CMB_SERVER))->GetCurSel() + 1;
	ST_SERVER_INFO stServerInfo = {0};
	if (!GetServerInfo(nServerID, stServerInfo))
	{
		::MessageBox(NULL, "���������ò���ȷ", "������", MB_OK|MB_ICONERROR);
	}
	else
	{
		m_pMsgPort->Send(MSGPORT_KERNEL, NETWORK_SERVER_INFO, STRUCT_TYPE(ST_SERVER_INFO), &stServerInfo);
	}
}
