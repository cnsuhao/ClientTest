////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgGameUserInfo.cpp
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2014-11-10
////////////////////////////////////////////////////////////////////////
#include "./MsgGameUserInfo.h"
#include "./../ClientKernel.h"
#include "./../Client.h"

//////////////////////////////////////////////////////////////////////////
CMsgGameUserInfo::CMsgGameUserInfo()
{
	Init();
	m_pInfo = (MSG_Info*)m_bufMsg;
	m_unMsgType = _MSG_GAME_USER_INFO;
	m_unMsgSize = sizeof(MSG_Info);
}

//////////////////////////////////////////////////////////////////////////
CMsgGameUserInfo::~CMsgGameUserInfo()
{

}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgGameUserInfo::Create( char* pMsgBuf, DWORD dwSize )
{
	if (!CMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GAME_USER_INFO != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CMsgGameUserInfo::Process( void *pInfo )
{
	CHECK(pInfo);
	CClient* pClient = pClientKernel->GetClient(m_socket);
	CHECK(pClient);
	pClient->ProcessReplyUserInfo(m_pInfo->nUserID, m_pInfo->nAccountType, m_pInfo->szUserName);
}
