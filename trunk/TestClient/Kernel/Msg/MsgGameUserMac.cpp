////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgGameUserMac.cpp
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2014-12-20
////////////////////////////////////////////////////////////////////////
#include "./MsgGameUserMac.h"

//////////////////////////////////////////////////////////////////////////
CMsgGameUserMac::CMsgGameUserMac()
{
	Init();
	m_pInfo = (MSG_Info*)m_bufMsg;
	m_unMsgType = _MSG_GAME_USER_MAC;
	m_unMsgSize = sizeof(MSG_Info);
}

//////////////////////////////////////////////////////////////////////////
CMsgGameUserMac::~CMsgGameUserMac()
{

}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgGameUserMac::Create( char* pMsgBuf, DWORD dwSize )
{
	if (!CMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GAME_USER_MAC != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CMsgGameUserMac::CreateMac( const char* pszMac )
{
	CHECKF(pszMac);

	this->Init();
	m_unMsgType	= _MSG_GAME_USER_MAC;
	m_unMsgSize	= sizeof(MSG_Info);

	::SafeCopy(m_pInfo->szMac, pszMac, sizeof(m_pInfo->szMac));
	return true;
}
