////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgGameLoginQuery.cpp
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2014-11-10
////////////////////////////////////////////////////////////////////////
#include "./MsgGameLoginQuery.h"

//////////////////////////////////////////////////////////////////////////
CMsgGameLoginQuery::CMsgGameLoginQuery()
{
	Init();
	m_pInfo = (MSG_Info*)m_bufMsg;
	m_unMsgType = _MSG_GAME_LOGIN_QUERY;
	m_unMsgSize = sizeof(MSG_Info);
}

//////////////////////////////////////////////////////////////////////////
CMsgGameLoginQuery::~CMsgGameLoginQuery()
{

}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgGameLoginQuery::Create( char* pMsgBuf, DWORD dwSize )
{
	if (!CMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GAME_LOGIN_QUERY != this->GetType())
		return false;

	return true;
}



//////////////////////////////////////////////////////////////////////////
void CMsgGameLoginQuery::Process( void *pInfo )
{
	DEBUG_TRY;
	switch (m_pInfo->sAction)
	{
	case MSG_LOGIN_QUERY_ACTION_CLIENT_LOGIN_OVER:
		break;
	default:
		break;
	}

	DEBUG_CATCH("CMsgGameLoginQuery::Process");
}

//////////////////////////////////////////////////////////////////////////
bool CMsgGameLoginQuery::CreateLoginQuery( MSG_LOGIN_QUERY_ACTION eAction, MSG_LOGIN_QUERY_RESULT eResult, int nParam1 /*= 0*/, int nParam2 /*= 0*/ )
{
	this->Init();
	m_unMsgType	= _MSG_GAME_LOGIN_QUERY;
	m_unMsgSize	= sizeof(MSG_Info);

	m_pInfo->sAction = (MSG_NUM_2BIT)eAction;
	m_pInfo->sResult = (MSG_NUM_2BIT)eResult;
	m_pInfo->nParam1 = nParam1;
	m_pInfo->nParam2 = nParam2;

	return true;
}



