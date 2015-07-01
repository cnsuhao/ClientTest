//////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle:	PerformanceStatistics.cpp
// Author:	������(PengWenqi)
// Create: 	2012/12/29
//////////////////////////////////////////////////////////////////////////
#pragma	warning(disable:4786)
#include "./PerformanceStatistics.h"
#include "./BaseFunc.h"

CPerformanceStatistics* CPerformanceStatistics::s_pInstance = NULL;
////////////////////////////////////////////////////////////////////////
// Description:  ������ȡ
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: CMemMonitor*
////////////////////////////////////////////////////////////////////////
CPerformanceStatistics* CPerformanceStatistics::GetInstance( void )
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new CPerformanceStatistics;
	}
	
	return s_pInstance;	
}

////////////////////////////////////////////////////////////////////////
// Description:  ��������
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::DelInstance( void )
{
	SAFE_DELETE(s_pInstance);
}

//////////////////////////////////////////////////////////////////////////
CPerformanceStatistics::CPerformanceStatistics()
{
	for (int i = PERFORMANCE_STATISTICS_TYPE_BEGIN; i < PERFORMANCE_STATISTICS_TYPE_END; i++)
	{
		this->SetData((PERFORMANCE_STATISTICS_TYPE)i, 0i64);
	}
	::SafeCopy(m_szLogBuffer, "", sizeof(m_szLogBuffer));
	m_tmLogTimer.Startup(PERFORMANCE_STATISTICS_LOGSAVE_SECOND);
}

//////////////////////////////////////////////////////////////////////////
CPerformanceStatistics::~CPerformanceStatistics()
{
	
}

////////////////////////////////////////////////////////////////////////
// Description:  ��ȡ����
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
I64 CPerformanceStatistics::GetData(PERFORMANCE_STATISTICS_TYPE eType) const
{
	I64 i64Result = 0i64;
	if (eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		i64Result = m_arrData[eType];
	}
	return i64Result;
}

////////////////////////////////////////////////////////////////////////
// Description:  ԭ�Ӳ���, ��������
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::SetData(PERFORMANCE_STATISTICS_TYPE eType, I64 i64NewData)
{
	CHECK(eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END);
//	InterlockedExchange64(&(m_arrData[eType]), i64NewData);
	CMySingleLock xLock(&m_csAsync);
	m_arrData[eType] = i64NewData;
}

////////////////////////////////////////////////////////////////////////
// Description:  ԭ�Ӳ���, ����
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::AddData(PERFORMANCE_STATISTICS_TYPE eType, I64 i64AddData)
{
	CHECK(eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END);
//	InterlockedExchangeAdd64(&(m_arrData[eType]), i64AddData);
	CMySingleLock xLock(&m_csAsync);
	m_arrData[eType] += i64AddData;
}

////////////////////////////////////////////////////////////////////////
// Description:  ԭ�Ӳ���, �������ֵ
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::SetMax(PERFORMANCE_STATISTICS_TYPE eType, I64 i64NewData)
{
	CHECK(eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END);
	if (i64NewData > m_arrData[eType])
	{
//		InterlockedExchange64(&(m_arrData[eType]), i64MaxData);
		CMySingleLock xLock(&m_csAsync);
		m_arrData[eType] = i64NewData;
	}
}

////////////////////////////////////////////////////////////////////////
// Description:  ����
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::OnTimer(void)
{
	VA_TRY
	::SafeCopy(m_szLogBuffer, "", sizeof(m_szLogBuffer));
	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND_MAX]	= max(m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND_MAX],	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND]);
	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND_MAX]	= max(m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND_MAX],	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND]);
	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND_MAX]		= max(m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND_MAX],		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND]);
	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND_MAX]		= max(m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND_MAX],		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND]);

	sprintf(m_szLogBuffer,	"UserSet:\n"
							"    NowUser[%I64d], MaxUser[%I64d], ConnectUser[%I64d], MaxConnectUser[%I64d]\n"
							"Database:\n"
							"    [Total] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"    [Async] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"SocketThread:\n"
							"    [MsgPort] RecvTimeSum[%I64d]\n"
							"    [OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"    [MaxMS]   OnProcess[%I64d]\n"
							"WorldThread:\n"
							"    [MsgPort] RecvTimeSum[%I64d]\n"
							"    [ProcessMsg] AccountTimeSum  [%I64d]\n"
							"                 RelationTimeSum [%I64d]\n"
							"                 WorldLineTimeSum[%I64d]\n"
							"    [OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"MapGroupThread:\n"
							"    [MsgPort] RecvTimeSum[%I64d]\n"
							"    [OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"    [MaxMS]   ProcessClientMsg[%I64d]\n"
							"Network:\n"
							"    [Client]\n"
							"        Send[%8I64d]KB/S, SendMax[%8I64d]KB/S\n"
							"        Recv[%8I64d]KB/S, RecvMax[%8I64d]KB/S\n"
							"    [Npc]\n"
							"        Send[%8I64d]KB/S, SendMax[%8I64d]KB/S\n"
							"        Recv[%8I64d]KB/S, RecvMax[%8I64d]KB/S\n"
							,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NOW_USER_AMOUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_MAX_USER_AMOUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_CONNECT_USER_AMOUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_CONNECT_USER_AMOUNT_MAX],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM_ASYNC], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT_ASYNC], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT_ASYNC] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM_ASYNC] / m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT_ASYNC] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_MSGPORT_RECV_TIME_SUM],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONPROCESS_TIME_MAX],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_MSGPORT_RECV_TIME_SUM],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ACCOUNT_PROCESS_MSG_TIME_SUM], 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_RELATION_PROCESS_MSG_TIME_SUM], 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_WORLDLINE_PROCESS_TIME_SUM],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ONTIME_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ONTIME_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ONTIME_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ONTIME_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ONTIME_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_MAPGROUP_MSGPORT_RECV_TIME_SUM],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_MAPGROUP_ONTIME_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_MAPGROUP_ONTIME_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_MAPGROUP_ONTIME_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_MAPGROUP_ONTIME_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_MAPGROUP_ONTIME_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_MAPGROUPKERNEL_PROCESSCLIENTMSG_MAX],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND_MAX] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND_MAX] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND_MAX] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND_MAX] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND
		);


	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND]	= 0i64;
	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND]	= 0i64;
	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND]		= 0i64;
	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND]		= 0i64;

	if (m_tmLogTimer.ToNextTime())
	{
		::MyLogSave("syslog/PerformanceStatistics", "==================================================================================================\n%s", m_szLogBuffer);
	}

	int aaa = strlen(m_szLogBuffer);
	VA_CATCH("CPerformanceStatistics::OnTimer")
}

////////////////////////////////////////////////////////////////////////
// Description:  ����ͳ��������(����)
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsData::CPerformanceStatisticsData(PERFORMANCE_STATISTICS_TYPE eType, I64 i64Data)
	: m_i64Data(i64Data)
{
	IF_NOT (eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		m_eType = PERFORMANCE_STATISTICS_TYPE_BEGIN;
		STACK_TRACE_ONCE;
		return;
	}
	m_eType = eType;
}

////////////////////////////////////////////////////////////////////////
// Description:  ����ͳ��������(����)
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsData::~CPerformanceStatisticsData()
{
	if (m_eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && m_eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		CPerformanceStatistics::GetInstance()->SetData(m_eType, m_i64Data);
	}
}

////////////////////////////////////////////////////////////////////////
// Description:  ����ͳ�Ƽ�ʱ��(����)
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsMS::CPerformanceStatisticsMS(PERFORMANCE_STATISTICS_TYPE eType, bool bMax/* = false*/)
	: m_eType(PERFORMANCE_STATISTICS_TYPE_BEGIN)
	, m_i64Start(0i64)
	, m_bMax(bMax)
{
	IF_NOT (eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		STACK_TRACE_ONCE;
		return;
	}
	m_eType = eType;
	m_i64Start = ::GetUtcMillisecond();
}

////////////////////////////////////////////////////////////////////////
// Description:  ����ͳ�Ƽ�ʱ��(����)
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsMS::~CPerformanceStatisticsMS()
{
	if (m_eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && m_eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		I64 i64End = ::GetUtcMillisecond();
		if (m_bMax)
		{
			CPerformanceStatistics::GetInstance()->SetMax(m_eType, i64End - m_i64Start);
		}
		else
		{
			CPerformanceStatistics::GetInstance()->AddData(m_eType, i64End - m_i64Start);
		}
	}
}

////////////////////////////////////////////////////////////////////////
// Description:  ����ͳ�Ƽƴ���(����)
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsCount::CPerformanceStatisticsCount(PERFORMANCE_STATISTICS_TYPE eType, I64 i64AddCount/* = 1*/, bool bMax/* = false*/)
	: m_i64AddCount(i64AddCount)
	, m_bMax(bMax)
{
	IF_NOT (eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		m_eType = PERFORMANCE_STATISTICS_TYPE_BEGIN;
		STACK_TRACE_ONCE;
		return;
	}
	m_eType = eType;
	m_i64AddCount = ::GetValueInRightRange(i64AddCount, 1i64, 10000i64);	// ÿ�ؼǴ�ֻ֧��1-10000�Ĵ���
}

////////////////////////////////////////////////////////////////////////
// Description:  ����ͳ�Ƽƴ���(����)
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsCount::~CPerformanceStatisticsCount()
{
	if (m_eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && m_eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		if (m_bMax)
		{
			CPerformanceStatistics::GetInstance()->SetMax(m_eType, m_i64AddCount);
		}
		else
		{
			CPerformanceStatistics::GetInstance()->AddData(m_eType, m_i64AddCount);
		}
	}
}
