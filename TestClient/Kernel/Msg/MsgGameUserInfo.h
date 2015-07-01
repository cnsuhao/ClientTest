////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgGameUserInfo.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2014-11-10
////////////////////////////////////////////////////////////////////////
#ifndef _GAMEKERNER_MSG_GAME_USER_INFO_H_
#define _GAMEKERNER_MSG_GAME_USER_INFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Msg.h"

class CMsgGameUserInfo : public CMsg
{
public:
	CMsgGameUserInfo();
	virtual ~CMsgGameUserInfo();

	virtual BOOL Create(char* pMsgBuf, DWORD dwSize);
	virtual void Process(void *pInfo);


private:
	typedef struct  
	{
		MSG_NUM_4BIT	nUserID;			// 角色id
		MSG_NUM_4BIT	nAccountID;			// 账号id
		MSG_NUM_4BIT	nAccountType;		// 账号类型
		MSG_NUM_4BIT	nLastLoginDate;		// 最后一次登陆日期	YYYYMMDD
		MSG_NUM_4BIT	nCreateTime;		// 创建时间 utc秒
		MSG_NUM_4BIT	nLastExitTime;		// 上一次登出时间

		MSG_NUM_4BIT	nLev;				// 等级
		MSG_NUM_8BIT	i64Exp;				// 经验
		MSG_NUM_4BIT	nSex;				// 性别
		MSG_NUM_4BIT	nLookface;			// 头像
		
		MSG_NUM_2BIT	nServerGroup;		// 服务器组号
		MSG_NUM_2BIT	nServerLine;		// 服务器线路号

		MSG_STR_32BIT	szUserName;			// 角色名
	}MSG_Info;

	MSG_Info* m_pInfo;
};


#endif // end of _GAMEKERNER_MSG_GAME_USER_INFO_H_