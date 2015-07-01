////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgLoginUserList.h
// Author: ������(Peng Wenqi)
// Created: 2014-11-4
////////////////////////////////////////////////////////////////////////
#ifndef _GAMESERVER_MSG_LOGIN_USER_LIST_H_
#define _GAMESERVER_MSG_LOGIN_USER_LIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Msg.h"

struct USER_LOGIN_SNAP
{
	MSG_NUM_4BIT	nUserID;	// ��ɫid
	MSG_STR_32BIT	szName;		// ��ɫ��
	MSG_NUM_4BIT	nLastLogin;	// �ϴε�½����	YYYYMMDD
	MSG_NUM_4BIT	nLev;		// �ȼ�
	MSG_NUM_4BIT	nSex;		// �Ա�
	MSG_NUM_4BIT	nLookface;	// ͷ��
	MSG_NUM_4BIT	nSaveParam1;// ��������1
	MSG_NUM_4BIT	nSaveParam2;// ��������2
};

class CMsgLoginUserList : public CMsg  
{
public:
	CMsgLoginUserList();
	virtual ~CMsgLoginUserList();
	virtual BOOL Create(char* pMsgBuf,DWORD dwSize);
	virtual void Process		(void* pInfo);

public:
	bool CreateBase(OBJID idAccount);
	bool AppendInfo(const USER_LOGIN_SNAP& rSnap);

private:
	typedef struct
	{
		MSG_NUM_4BIT	nAccountID;
		MSG_NUM_4BIT	nCount;
		USER_LOGIN_SNAP	stInfo[1];
	}MSG_Info;

	MSG_Info* m_pInfo;
};


#endif // end of _GAMESERVER_MSG_LOGIN_USER_LIST_H_