////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgGameUserMac.h
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2014-12-20
////////////////////////////////////////////////////////////////////////
#ifndef _GAMEKERNER_MSG_GAME_USER_MAC_H_
#define _GAMEKERNER_MSG_GAME_USER_MAC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Msg.h"

class CMsgGameUserMac : public CMsg
{
public:
	CMsgGameUserMac();
	virtual ~CMsgGameUserMac();

	virtual BOOL Create(char* pMsgBuf, DWORD dwSize);

public:
	bool CreateMac(const char* pszMac);


private:
	typedef struct  
	{
		char	szMac[64];		
	}MSG_Info;

	MSG_Info* m_pInfo;
};


#endif _GAMEKERNER_MSG_GAME_USER_MAC_H_