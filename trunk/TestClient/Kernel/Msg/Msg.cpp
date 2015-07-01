
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: Msg.cpp
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2013-1-15
////////////////////////////////////////////////////////////////////////
#include "./Msg.h"
#include "./AllMsg.h"

//////////////////////////////////////////////////////////////////////
void CMsg::Init()
{
	::memset(m_bufMsg, 0L, sizeof(m_bufMsg));
	m_unMsgSize	=0;
	m_unMsgType	=_MSG_NONE;
	m_socket		= INVALID_SOCKET;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsg::IsValid(void)
{
	if(_MSG_NONE == this->GetType())
		return false;
	
	return true;
}


//////////////////////////////////////////////////////////////////////
// virtual
//////////////////////////////////////////////////////////////////////
BOOL CMsg::Create(char* pszBufMsg, USHORT nsMsgSize)
{
	if(!pszBufMsg)
		return false;
	
	::memcpy(m_bufMsg, pszBufMsg, nsMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsg::Create(char* pszBufMsg, USHORT nsMsgSize, SOCKET sck)
{
	if(!pszBufMsg)
		return false;
	
	::memcpy(m_bufMsg, pszBufMsg, nsMsgSize);
	m_socket = sck;
	return true;
}

CMsg* CMsg::CreateClientMsg( SOCKET idSocket, OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize )
{
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > _MAX_MSGSIZE)
		return NULL;

	CMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
	if(!pMsg)
		return NULL;

	pMsg->m_socket		= idSocket;
	pMsg->m_unMsgType	= idMsg;
	pMsg->m_unMsgSize	= dwMsgSize;

	if(!pMsg->Create((char*)pbufMsg, dwMsgSize))
	{
		SAFE_DELETE(pMsg);
		return NULL;
	}
	else
	{
		return pMsg;
	}
}

CMsg* CMsg::CreateMsg( OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize )
{
	// make it...
	CMsg* pMsg	=NULL;
	switch(idMsg)
	{
	case _MSG_LOGIN_CONNECT:
		pMsg = new CMsgLoginConnect;
		break;
	case _MSG_LOGIN_KICK_REASON:
	case _MSG_GAME_KICK_REASON:
		pMsg = new CMsgKickReason;
		break;
	case _MSG_LOGIN_CONNECT_ACTION:
		pMsg = new CMsgLoginAction;
		break;
	case _MSG_LOGIN_USER_LIST:
		pMsg = new CMsgLoginUserList;
		break;
	case _MSG_GAME_USER_INFO:
		pMsg = new CMsgGameUserInfo;
		break;
	case _MSG_GAME_SMALLACTION:
		pMsg = new CMsgGameSmallAction;
		break;
	case _MSG_GAME_LOGIN_QUERY:
		pMsg = new CMsgGameLoginQuery;
		break;
	case _MSG_GAME_TALK:
		pMsg = new CMsgGameTalk;
		break;
	case _MSG_GAME_USER_MAC:
		pMsg = new CMsgGameUserMac;
		break;
	default:
		break;
	}
	return pMsg;
}
