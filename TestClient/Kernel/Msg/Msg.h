////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: Msg.h
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2013-1-15
////////////////////////////////////////////////////////////////////////
#ifndef _DEBUGSERVER_MSG_BASE_H_
#define _DEBUGSERVER_MSG_BASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"
#include "./../../Include/Include.h"

class CMsg
{
public:
	CMsg()	{ this->Init(); }
	virtual ~CMsg()	{}
	
	const char*		GetBuf		(void) const	{ return m_bufMsg; }
	USHORT			GetSize		(void) const	{ return m_unMsgSize; }
	USHORT			GetType		(void) const	{ return m_unMsgType; }
	BOOL			IsValid		(void);
	SOCKET			GetSocketID	(void)			{ return m_socket; }
protected:
	void			Init		(void);
	
public:	
	virtual BOOL	Create		(char* pszBuf, USHORT usBufLen);
	virtual BOOL	Create		(char* pszBuf, USHORT usBufLen, SOCKET sck);
	virtual void	Process		(void* pInfo)	                { ::LogSave("CMsg::Process(void* pInfo) error, m_unMsgType[%u], m_unMsgSize[%d]", m_unMsgType, m_unMsgSize); }

public:
	static CMsg*	CreateClientMsg	(SOCKET idSocket,	OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize);
	static CMsg*	CreateMsg	(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize);

protected:
	OBJID		m_unMsgType;
	int			m_unMsgSize;
	SOCKET		m_socket;
	char		m_bufMsg[_MAX_MSGSIZE];
};

#endif  // end of _DEBUGSERVER_MSG_BASE_H_

