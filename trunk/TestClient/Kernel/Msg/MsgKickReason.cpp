////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgKickReason.cpp
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2014-11-15
////////////////////////////////////////////////////////////////////////
#include "./MsgKickReason.h"
#include "./../ClientKernel.h"
#include "./../Client.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgKickReason::CMsgKickReason()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgType	=_MSG_LOGIN_KICK_REASON;
}

CMsgKickReason::~CMsgKickReason()
{
	
}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgKickReason::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if (_MSG_LOGIN_KICK_REASON != this->GetType() &&
		_MSG_GAME_KICK_REASON != this->GetType() )
		return false;
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CMsgKickReason::Process( void* pInfo )
{
	CHECK(pInfo);
	CClient* pClient = pClientKernel->GetClient(m_socket);
	CHECK(pClient);
	pClient->ProcessBeKick(m_pInfo->nReason, m_pInfo->nParam);
}
