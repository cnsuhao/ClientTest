//////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle:	PerformanceStatistics.h
// Author:	彭文奇(PengWenqi)
// Create: 	2012/12/29
//		性能统计模块
//////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_PERFORMANCE_STATISTICS_H_
#define _COMMON_PERFORMANCE_STATISTICS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

#include "./MyArray.h"
#include "./TimeOut.h"
#include "./ThreadBase.h"

const int PERFORMANCE_STATISTICS_ONTIMER_SECOND = 5;		// 每5秒进行一次性能统计
const int PERFORMANCE_STATISTICS_LOGSAVE_SECOND = 5 * 60;	// 每5分钟将性能统计记录日志

enum PERFORMANCE_STATISTICS_TYPE
{
	PERFORMANCE_STATISTICS_TYPE_BEGIN = 0,

	PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM,							// 数据库操作计时总和
	PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT,						// 数据库操作计次总和
	PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM_ASYNC,					// 数据库操作计时总和(异步)
	PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT_ASYNC,					// 数据库操作计次总和(异步)

	PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_MSGPORT_RECV_TIME_SUM,			// 网络线程OnProcess时, Recv总耗时
	PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_SUM,				// 网络线程OnProcess时, OnTimer总耗时
	PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_COUNT,			// 网络线程OnProcess时, OnTimer总次数
	PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONPROCESS_TIME_MAX,			// 网络线程OnProcess时, 最大耗时

	PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_MSGPORT_RECV_TIME_SUM,			// 世界线程OnProcess时, Recv总耗时
	PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ACCOUNT_PROCESS_MSG_TIME_SUM,	// 世界线程OnProcess时, 账号服消息处理总耗时
	PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_RELATION_PROCESS_MSG_TIME_SUM,	// 世界线程OnProcess时, 关系服消息处理总耗时
	PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_WORLDLINE_PROCESS_TIME_SUM,		// 世界线程OnProcess时, 世界服消息处理总耗时
	PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ONTIME_TIME_SUM,				// 世界线程OnProcess时, 世界服OnTimer处理总耗时
	PERFORMANCE_STATISTICS_TYPE_WORLDTHREAD_ONTIME_TIME_COUNT,				// 世界线程OnProcess时, 世界服OnTimer处理总次数

	PERFORMANCE_STATISTICS_TYPE_MAPGROUP_MSGPORT_RECV_TIME_SUM,				// 地图组线程OnProcess时, Recv总耗时
	PERFORMANCE_STATISTICS_TYPE_MAPGROUP_ONTIME_TIME_SUM,					// 地图组线程OnProcess时, OnTimer总耗时
	PERFORMANCE_STATISTICS_TYPE_MAPGROUP_ONTIME_TIME_COUNT,					// 地图组线程OnProcess时, OnTimer总次数

	PERFORMANCE_STATISTICS_TYPE_MAPGROUPKERNEL_PROCESSCLIENTMSG_MAX,		// 地图组处理消息最大耗时

	PERFORMANCE_STATISTICS_TYPE_CONNECT_USER_AMOUNT,						// 当前排队登陆玩家总数
	PERFORMANCE_STATISTICS_TYPE_CONNECT_USER_AMOUNT_MAX,					// 最大排队登陆玩家总数
	PERFORMANCE_STATISTICS_TYPE_NOW_USER_AMOUNT,							// 当前玩家数
	PERFORMANCE_STATISTICS_TYPE_MAX_USER_AMOUNT,							// 最大玩家数

	PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND,					// 五秒内客户端连接Send Byte
	PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND,					// 五秒内客户端连接Recv Byte
	PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND_MAX,				// 五秒内客户端连接Send Byte Max
	PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND_MAX,				// 五秒内客户端连接Recv Byte Max

	PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND,						// 五秒内NPC服连接Send Byte
	PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND,						// 五秒内NPC服连接Recv Byte
	PERFORMANCE_STATISTICS_TYPE_NET_NPC_SEND_5SECOND_MAX,					// 五秒内NPC服连接Send Byte Max
	PERFORMANCE_STATISTICS_TYPE_NET_NPC_RECV_5SECOND_MAX,					// 五秒内NPC服连接Recv Byte Max

	PERFORMANCE_STATISTICS_TYPE_END,
};

typedef CMyArray<I64, PERFORMANCE_STATISTICS_TYPE_END - PERFORMANCE_STATISTICS_TYPE_BEGIN>		ARR_PERFORMANCE_STATISTICS;

class CPerformanceStatistics
{
	// 单例
public:
	static CPerformanceStatistics*	GetInstance(void);
	static void						DelInstance(void);
private:
	CPerformanceStatistics();
	virtual ~CPerformanceStatistics();
	static CPerformanceStatistics*	s_pInstance;


public:
	I64				GetData (PERFORMANCE_STATISTICS_TYPE eType) const;
	void			SetData	(PERFORMANCE_STATISTICS_TYPE eType, I64 i64NewData);
	void			AddData	(PERFORMANCE_STATISTICS_TYPE eType, I64 i64AddData);
	void			SetMax	(PERFORMANCE_STATISTICS_TYPE eType, I64 i64NewData);
	void			OnTimer (void);
	const char*		GetLogBuffer(void) const {return m_szLogBuffer;};


private:
	ARR_PERFORMANCE_STATISTICS		m_arrData;
	char							m_szLogBuffer[3072];
	CSimpleCriticalSection			m_csAsync;				// 线程临界区
	CTimeOut						m_tmLogTimer;			// 记录日志定时器
};

// 性能统计数据类
class CPerformanceStatisticsData
{
public:
	CPerformanceStatisticsData(PERFORMANCE_STATISTICS_TYPE eType, I64 i64Data);
	~CPerformanceStatisticsData();
private:
	PERFORMANCE_STATISTICS_TYPE m_eType;
	I64 m_i64Data;
};

// 性能统计计时类
class CPerformanceStatisticsMS
{
public:
	CPerformanceStatisticsMS(PERFORMANCE_STATISTICS_TYPE eType, bool bMax = false);
	~CPerformanceStatisticsMS();
private:
	PERFORMANCE_STATISTICS_TYPE m_eType;
	I64 m_i64Start;
	bool m_bMax;
};

// 性能统计计次类
class CPerformanceStatisticsCount
{
public:
	CPerformanceStatisticsCount(PERFORMANCE_STATISTICS_TYPE eType, I64 i64AddCount = 1, bool bMax = false);
	~CPerformanceStatisticsCount();
private:
	PERFORMANCE_STATISTICS_TYPE m_eType;
	I64 m_i64AddCount;
	bool m_bMax;
};

#endif // end of _COMMON_PERFORMANCE_STATISTICS_H_
