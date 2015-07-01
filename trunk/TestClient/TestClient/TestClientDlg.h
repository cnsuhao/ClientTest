
// TestClientDlg.h : ͷ�ļ�
//
#include "./../Include/Include.h"
#include "./../Kernel/ClientThread.h"
#pragma once

typedef std::map<int, ST_SERVER_INFO>	MAP_SERVER_INFO;

// CTestClientDlg �Ի���
class CTestClientDlg : public CDialogEx
{
// ����
public:
	CTestClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnLogout();
	afx_msg void OnBnClickedBtnBack();

protected:
	IMessagePort*	m_pMsgPort;
	CClientThread*	m_pClientThread;
	
	MAP_SERVER_INFO	m_mapServerInfo;						// ��������Ϣ
	DLG_STATUS		m_eState;

	MAP_SERVER_STATISTICS_INFO	m_mapServerStatisticsInfo;	// ������ͳ����Ϣ

	bool	GetServerInfo(int nServerId, ST_SERVER_INFO& rInfo);
	bool	LoadServerInfo(void);
	bool	InitCtrlItemView(void);

	void	ProcessPortMsg(void);
	void	OnTimerServerState(void);
	bool	IsServerRuning(void) { return m_eState == DLG_STATUS_RUNNING; }

	void	ProcessServerStatic(ST_SERVER_STATISTICS_INFO& rInfo);
public:
	afx_msg void OnCbnSelchangeCmbServer();
};
