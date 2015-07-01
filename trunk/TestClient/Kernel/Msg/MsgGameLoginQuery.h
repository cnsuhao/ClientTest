////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgGameLoginQuery.h
// Author: ������(Peng Wenqi)
// Created: 2014-11-10
////////////////////////////////////////////////////////////////////////
#ifndef _GAMEKERNER_MSG_GAME_LOGIN_QUERY_H_
#define _GAMEKERNER_MSG_GAME_LOGIN_QUERY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Msg.h"

// ��¼actionö��, ��ӦsAction
enum MSG_LOGIN_QUERY_ACTION
{
	MSG_LOGIN_QUERY_ACTION_HEARTBEAT					= 1,	// ����				C <-> S
	MSG_LOGIN_QUERY_ACTION_CLIENT_LOGIN_OVER			= 2,	// �ͻ��˵�½���	C --> S

	MSG_LOGIN_QUERY_ACTION_AUTOBACK_BEGIN				= 100,	// �Զ��ذ���ʼ	����ȫ�� C <-> S
	MSG_LOGIN_QUERY_ACTION_ALL_MONEY					= 101,	// ��ѯȫ����Ǯ		
	MSG_LOGIN_QUERY_ACTION_MASKINFO						= 102,	// ��ѯ����		
	MSG_LOGIN_QUERY_ACTION_PACKAGE						= 103,	// ��ѯ����
	MSG_LOGIN_QUERY_ACTION_ALL_FRIEND					= 104,	// ��ѯ����
	MSG_LOGIN_QUERY_ACTION_ALL_ACHIEVEMENT				= 105,	// ��ѯ�ɾ�
	MSG_LOGIN_QUERY_ACTION_SHOP							= 106,	// ��ѯ�̵�
	MSG_LOGIN_QUERY_ACTION_BLACK_LIST					= 107,	// ��ѯ�������б�
	MSG_LOGIN_QUERY_ACTION_CLIENT_TMPDATA				= 108,	// ��ѯ�ͻ����Զ���TmpData
	MSG_LOGIN_QUERY_ACTION_ALL_TASK						= 109,	// ��ѯ�����б�
	MSG_LOGIN_QUERY_ACTION_UNREADMAILCOUNT				= 110,	// ��ѯδ���ʼ�����
	MSG_LOGIN_QUERY_ACTION_ALL_LIVENESS					= 111,	// ��ѯÿ�ջ�Ծ�б�
	MSG_LOGIN_QUERY_ACTION_ALL_POKER					= 112,	// ��ѯ�����б�
	MSG_LOGIN_QUERY_ACTION_SIGN							= 113,	// ��ѯǩ��
	MSG_LOGIN_QUERY_ACTION_LOTTERY_INFO					= 114,	// ��ѯ�齱��Ϣ
	MSG_LOGIN_QUERY_ACTION_LOTTERY_CHAPMAN				= 115,	// ��ѯ�齱����
	MSG_LOGIN_QUERY_ACTION_SELF_SYNDICATE				= 116,	// ��ѯ�Լ�������Ϣ
	MSG_LOGIN_QUERY_ACTION_TALK_WORLD_LIST				= 117,	// ��ѯ���������б�
	MSG_LOGIN_QUERY_ACTION_TALK_SYN_LIST				= 118,	// ��ѯ���������б�
	MSG_LOGIN_QUERY_ACTION_TALK_PRIVATE_LIST			= 119,	// ��ѯ˽���б�
	MSG_LOGIN_QUERY_ACTION_VIP							= 120,	// ��ѯVIP
	MSG_LOGIN_QUERY_SERVER_GROUP						= 121,	// ��ѯ����������Ϣ
	MSG_LOGIN_QUERY_BUFF								= 122,	// ��ѯ���buff
	MSG_LOGIN_QUERY_ACTION_AUTOBACK_END,
};

// ������ö��, ��ӦsResult
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