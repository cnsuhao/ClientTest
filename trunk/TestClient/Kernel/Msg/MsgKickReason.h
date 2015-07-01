////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgKickReason.h
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2014-11-15
////////////////////////////////////////////////////////////////////////
#ifndef _GAMESERVER_MSG_LOGIN_KICK_REASON_H_
#define _GAMESERVER_MSG_LOGIN_KICK_REASON_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Msg.h"

class CMsgKickReason : public CMsg  
{
public:
	CMsgKickReason();
	virtual ~CMsgKickReason();
	virtual BOOL Create(char* pMsgBuf,DWORD dwSize);
	virtual void Process(void* pInfo);

private:
	typedef struct
	{
		MSG_NUM_4BIT	nReason;
		MSG_NUM_4BIT	nParam;
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif	// end of _GAMESERVER_MSG_LOGIN_KICK_REASON_H_