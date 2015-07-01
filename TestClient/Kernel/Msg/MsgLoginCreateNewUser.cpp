////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgLoginCreateNewUser.cpp
// Author: 彭文奇(Peng Wenqi)
// Created: 2014-11-4
////////////////////////////////////////////////////////////////////////
#include "./MsgLoginCreateNewUser.h"

//////////////////////////////////////////////////////////////////////////
CMsgLoginCreateNewUser::CMsgLoginCreateNewUser()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgType	= _MSG_LOGIN_CREATE_NEW_USER;
	m_unMsgSize	= sizeof(MSG_Info);
}

//////////////////////////////////////////////////////////////////////////
CMsgLoginCreateNewUser::~CMsgLoginCreateNewUser()
{

}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgLoginCreateNewUser::Create( char* pMsgBuf,DWORD dwSize )
{
	if (!CMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_LOGIN_CREATE_NEW_USER != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CMsgLoginCreateNewUser::Process( void* pInfo )
{
	// 登入流程3.5
	DEBUG_TRY;
	DEBUG_CATCH("CMsgLoginCreateNewUser::Process");
}

