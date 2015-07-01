////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: StructDef.h
// Author: ������(Peng Wenqi)
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

// ��������Ϣ
struct ST_SERVER_INFO
{
	MSG_NUM_4BIT	nServerID;
	MSG_STR_32BIT	szServerName;
	MSG_STR_32BIT	szServerIP;
	MSG_NUM_4BIT	nServerPort;
	MSG_NUM_4BIT	nStatus;
};

// �������˺���Ϣ
struct ST_CLIENT_INFO
{
	MSG_NUM_4BIT	nClientID;
	MSG_STR_32BIT	szClientAccount;
	MSG_STR_32BIT	szClientPwd;
};

// ��������Ϣͳ��
struct ST_SERVER_STATISTICS_INFO
{
	MSG_NUM_4BIT	nServerID;
	MSG_NUM_4BIT	nStatus;

	MSG_NUM_4BIT	nConnect;		// ������
	MSG_NUM_4BIT	nOnline;		// ������
	MSG_NUM_4BIT	nRecvMsg;		// ������Ϣ����
	MSG_NUM_4BIT	nSendMsg;		// ������Ϣ����

	MSG_NUM_4BIT	nBreakCount;	// ����ʧ�ܼ���
};
typedef std::map<int, ST_SERVER_STATISTICS_INFO>	MAP_SERVER_STATISTICS_INFO;
typedef MAP_SERVER_STATISTICS_INFO::iterator		MAP_SERVER_STATISTICS_ITER;
#endif // end of _DEBUGSERVER_STRUCT_DEF_H_