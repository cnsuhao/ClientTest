////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgLoginAction.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2014-11-4
////////////////////////////////////////////////////////////////////////
#ifndef _GAMESERVER_MSG_LOGIN_ACTION_H_
#define _GAMESERVER_MSG_LOGIN_ACTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Msg.h"

enum MSG_LOGIN_ACTION_TYPE
{
	MSG_LOGIN_ACTION_TYPE_CHOOSE_USER	= 1,	// 选择玩家		C --> S
	MSG_LOGIN_ACTION_TYPE_DELETE_USER	= 2,	// 删除玩家		C <-> S
};

enum MSG_LOGIN_ACTION_RESULT
{
	MSG_LOGIN_ACTION_RESULT_SUCCESS		= 0,	// 成功
	MSG_LOGIN_ACTION_RESULT_FAIL		= 1,	// 失败
};

class CMsgLoginAction : public CMsg  
{
public:
	CMsgLoginAction();
	virtual ~CMsgLoginAction();
	virtual BOOL Create(char* pMsgBuf,DWORD dwSize);
	virtual void	Process		(void* pInfo){}

public:
	bool CreateChooseUser(OBJID idUser);

private:
	typedef struct
	{
		MSG_NUM_2BIT	sAction;
		MSG_NUM_2BIT	sResult;

		MSG_NUM_4BIT	nUserID;
		MSG_NUM_4BIT	nParam;
	}MSG_Info;

	MSG_Info* m_pInfo;
};


#endif // end of _GAMESERVER_MSG_LOGIN_ACTION_H_