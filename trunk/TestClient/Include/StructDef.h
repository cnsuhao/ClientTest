////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: StructDef.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2013-1-15
////////////////////////////////////////////////////////////////////////
#ifndef _DEBUGSERVER_STRUCT_DEF_H_
#define _DEBUGSERVER_STRUCT_DEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./EnumDef.h"
#include "./ConstDef.h"

#include <deque>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <limits>
#include <iostream>
#include <fstream>

// 服务器信息
struct ST_SERVER_INFO
{
	MSG_NUM_4BIT	nServerID;
	MSG_STR_32BIT	szServerName;
	MSG_STR_32BIT	szServerIP;
	MSG_NUM_4BIT	nServerPort;
	MSG_NUM_4BIT	nStatus;
};

// 机器人账号信息
struct ST_CLIENT_INFO
{
	MSG_NUM_4BIT	nClientID;
	MSG_STR_32BIT	szClientAccount;
	MSG_STR_32BIT	szClientPwd;
};

// 服务器信息统计
struct ST_SERVER_STATISTICS_INFO
{
	MSG_NUM_4BIT	nServerID;
	MSG_NUM_4BIT	nStatus;

	MSG_NUM_4BIT	nConnect;		// 连接数
	MSG_NUM_4BIT	nOnline;		// 在线数
	MSG_NUM_4BIT	nRecvMsg;		// 接受消息计数
	MSG_NUM_4BIT	nSendMsg;		// 发送消息计数

	MSG_NUM_4BIT	nBreakCount;	// 连接失败计数
};
typedef std::map<int, ST_SERVER_STATISTICS_INFO>	MAP_SERVER_STATISTICS_INFO;
typedef MAP_SERVER_STATISTICS_INFO::iterator		MAP_SERVER_STATISTICS_ITER;
#endif // end of _DEBUGSERVER_STRUCT_DEF_H_