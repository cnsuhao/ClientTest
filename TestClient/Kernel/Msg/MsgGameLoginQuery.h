////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgGameLoginQuery.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2014-11-10
////////////////////////////////////////////////////////////////////////
#ifndef _GAMEKERNER_MSG_GAME_LOGIN_QUERY_H_
#define _GAMEKERNER_MSG_GAME_LOGIN_QUERY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Msg.h"

// 登录action枚举, 对应sAction
enum MSG_LOGIN_QUERY_ACTION
{
	MSG_LOGIN_QUERY_ACTION_HEARTBEAT					= 1,	// 心跳				C <-> S
	MSG_LOGIN_QUERY_ACTION_CLIENT_LOGIN_OVER			= 2,	// 客户端登陆完成	C --> S

	MSG_LOGIN_QUERY_ACTION_AUTOBACK_BEGIN				= 100,	// 自动回包开始	以下全部 C <-> S
	MSG_LOGIN_QUERY_ACTION_ALL_MONEY					= 101,	// 查询全部金钱		
	MSG_LOGIN_QUERY_ACTION_MASKINFO						= 102,	// 查询掩码		
	MSG_LOGIN_QUERY_ACTION_PACKAGE						= 103,	// 查询背包
	MSG_LOGIN_QUERY_ACTION_ALL_FRIEND					= 104,	// 查询好友
	MSG_LOGIN_QUERY_ACTION_ALL_ACHIEVEMENT				= 105,	// 查询成就
	MSG_LOGIN_QUERY_ACTION_SHOP							= 106,	// 查询商店
	MSG_LOGIN_QUERY_ACTION_BLACK_LIST					= 107,	// 查询黑名单列表
	MSG_LOGIN_QUERY_ACTION_CLIENT_TMPDATA				= 108,	// 查询客户端自定义TmpData
	MSG_LOGIN_QUERY_ACTION_ALL_TASK						= 109,	// 查询任务列表
	MSG_LOGIN_QUERY_ACTION_UNREADMAILCOUNT				= 110,	// 查询未读邮件数量
	MSG_LOGIN_QUERY_ACTION_ALL_LIVENESS					= 111,	// 查询每日活跃列表
	MSG_LOGIN_QUERY_ACTION_ALL_POKER					= 112,	// 查询卡牌列表
	MSG_LOGIN_QUERY_ACTION_SIGN							= 113,	// 查询签到
	MSG_LOGIN_QUERY_ACTION_LOTTERY_INFO					= 114,	// 查询抽奖信息
	MSG_LOGIN_QUERY_ACTION_LOTTERY_CHAPMAN				= 115,	// 查询抽奖商人
	MSG_LOGIN_QUERY_ACTION_SELF_SYNDICATE				= 116,	// 查询自己帮派信息
	MSG_LOGIN_QUERY_ACTION_TALK_WORLD_LIST				= 117,	// 查询世界聊天列表
	MSG_LOGIN_QUERY_ACTION_TALK_SYN_LIST				= 118,	// 查询帮派聊天列表
	MSG_LOGIN_QUERY_ACTION_TALK_PRIVATE_LIST			= 119,	// 查询私聊列表
	MSG_LOGIN_QUERY_ACTION_VIP							= 120,	// 查询VIP
	MSG_LOGIN_QUERY_SERVER_GROUP						= 121,	// 查询服务器组信息
	MSG_LOGIN_QUERY_BUFF								= 122,	// 查询玩家buff
	MSG_LOGIN_QUERY_ACTION_AUTOBACK_END,
};

// 错误码枚举, 对应sResult
enum MSG_LOGIN_QUERY_RESULT
{
	MSG_LOGIN_QUERY_RESULT_SUCC		= 0,
	MSG_LOGIN_QUERY_RESULT_ERROR	= 1,
};

class CMsgGameLoginQuery : public CMsg
{
public:
	CMsgGameLoginQuery();
	virtual ~CMsgGameLoginQuery();

	virtual BOOL Create(char* pMsgBuf, DWORD dwSize);
	virtual void Process(void *pInfo);

public:
	bool CreateLoginQuery(MSG_LOGIN_QUERY_ACTION eAction, MSG_LOGIN_QUERY_RESULT eResult, int nParam1 = 0, int nParam2 = 0);


private:
	typedef struct  
	{
		MSG_NUM_2BIT	sAction;
		MSG_NUM_2BIT	sResult;

		MSG_NUM_4BIT	nParam1;
		MSG_NUM_4BIT	nParam2;
	}MSG_Info;

	MSG_Info* m_pInfo;
};


#endif // end of _GAMEKERNER_MSG_GAME_LOGIN_QUERY_H_