
// TestClientDlg.h : 头文件
//
#include "./../Include/Include.h"
#include "./../Kernel/ClientThread.h"
#pragma once

typedef std::map<int, ST_SERVER_INFO>	MAP_SERVER_INFO;

// CTestClientDlg 对话框
class CTestClientDlg : public CDialogEx
{
// 构造
public:
	CTestClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	
	MAP_SERVER_INFO	m_mapServerInfo;						// 服务器信息
	DLG_STATUS		m_eState;

	MAP_SERVER_STATISTICS_INFO	m_mapServerStatisticsInfo;	// 服务器统计信息

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
