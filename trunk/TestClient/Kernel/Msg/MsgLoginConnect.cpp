////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgLoginConnect.cpp
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2014-11-3
////////////////////////////////////////////////////////////////////////
#include "./MsgLoginConnect.h"

//////////////////////////////////////////////////////////////////////////
CMsgLoginConnect::CMsgLoginConnect()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgType	= _MSG_LOGIN_CONNECT;
	m_unMsgSize	= sizeof(MSG_Info);
}

//////////////////////////////////////////////////////////////////////////
CMsgLoginConnect::~CMsgLoginConnect()
{

}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgLoginConnect::Create( char* pMsgBuf,DWORD dwSize )
{
	if (!CMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_LOGIN_CONNECT != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CMsgLoginConnect::CreateAccountConnect( const char* pszAccountName, const char* pszAccountPwd )
{
	this->Init();

	::SafeCopy(m_pInfo->szAccountName, pszAccountName, sizeof(m_pInfo->szAccountName));
	::SafeCopy(m_pInfo->szAccountPassword, pszAccountPwd, sizeof(m_pInfo->szAccountPassword));

	m_unMsgType	=_MSG_LOGIN_CONNECT;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}

