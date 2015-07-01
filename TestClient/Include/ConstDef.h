////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: ConstDef.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2013-1-15
////////////////////////////////////////////////////////////////////////
#ifndef _DEBUGSERVER_CONST_DEF_H_
#define _DEBUGSERVER_CONST_DEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./EnumDef.h"

// 消息结构体
typedef char	MSG_NUM_1BIT;
typedef short	MSG_NUM_2BIT;
typedef int		MSG_NUM_4BIT;
typedef __int64 MSG_NUM_8BIT;
typedef char	MSG_STR_16BIT[16];
typedef char	MSG_STR_32BIT[32];
typedef char	MSG_STR_128BIT[128];
typedef char	MSG_STR_192BIT[192];

typedef	char	SQLBUF[2048];

const int	MAX_PACKETSIZE		= 1024;							// 网络消息包尺寸
const int	MAX_MESSAGESIZE		= MAX_PACKETSIZE + 24;			// 内部消息包尺寸
const int	CLOSETHREAD_MILLISECS = 30000;


const int	TIMER_SERVER_INFO = 15;	// 每15秒同步一次服务器状态
const int	MAX_RECONNECT_COUNT = 10;	// 最大重连次数
const int	MAX_TALK_CAN_SEE_COUNT = 500;	// 最大聊天监控条数

const int	_MAX_NAMESIZE		=32;
const int	_MAX_ACCOUNT_NAMESIZE		=36;
const int	_MAX_ACCOUNTPSWSIZE	=64;
const int	MAX_NAME_SIZE		= 32;
const int	_MAX_WORDSSIZE		=256;
const int	MAX_CMD_SIZE		= 64;
const int	MAX_TXT_SIZE		= 1024;
const int	MAX_CONTENT_SIZE  = 1024;


#ifdef _DEBUG
	static const char* SERVER_TITLE = " -- PID:[%u] CreateTime:[%s %s] SVN Version:[%d] Debug";		// 服务器标题
#else
	static const char* SERVER_TITLE = " -- PID:[%u] CreateTime:[%s %s] SVN Version:[%d] Release";	// 服务器标题
#endif

static const char* SERVER_LIST_FILENAME		= "server.ini";	// 服务器列表文件
static const char* CLIENT_FILENAME			= "client.ini";	// 登陆配置文件	
static const char* SENSITIVE_KEY_FILENAME	= "key.ini";	// 过滤字文件列表

const int DBSTR_SIZE = 256;

const int	_MAX_MSGSIZE		= 1024;


#endif // end of _DEBUGSERVER_CONST_DEF_H_