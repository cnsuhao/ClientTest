////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgSmallAction.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-10-23
////////////////////////////////////////////////////////////////////////
#ifndef _GAMESERVER_MSG_GAME_SMALL_ACTION_H_
#define _GAMESERVER_MSG_GAME_SMALL_ACTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Msg.h"

// SmallAction枚举, 对应sAction
enum MSG_SMALL_ACTION
{
	MSG_SMALL_ACTION_NONE = 0,
	MSG_SMALL_ACTION_QUERY_NEWCARDPRIZE_INFO		= 1,	// 查询新手卡奖励信息   	C --> S
	MSG_SMALL_ACTION_RUN_LUA_SCRIPT					= 2,	// 执行LUA脚本				C --> S
};

// 错误码枚举, 对应sResult
enum MSG_SMALL_RESULT
{
	MSG_SMALL_RESULT_SUCC							= 0,
	MSG_SMALL_RESULT_ERROR							= 1,
};

typedef std::vector<UINT> VEC_SCORE;
typedef std::vector<UINT> VEC_ONLINEAWARDINFO;
//////////////////////////////////////////////////////////////////////////
//	特别注意:	[10/24/2012 彭文奇(Peng Wenqi)]
//	由于这个协议是个广泛应用的变长协议
//	新增协议Create或Process时, 请针对每个东西来写一个函数, 不要重用, 可读性会很好 
class CMsgGameSmallAction : public CMsg  
{
public:
	CMsgGameSmallAction();
	virtual ~CMsgGameSmallAction();

	virtual BOOL Create					(char* pMsgBuf, DWORD dwSize);
	virtual void Process				(void *pInfo);


	// Create Func Here...
public:

	// Process Func Here...
private:

private:
	bool CreateSmallAction				(MSG_SMALL_ACTION eAction, MSG_SMALL_RESULT eResult);
	bool AppendParam					(int nAddParam);
	int  GetParamVector					(OUT INT_VEC& rVec);

private:
	typedef struct
	{
		MSG_NUM_2BIT	sAction;
		MSG_NUM_2BIT	sResult;

		MSG_NUM_4BIT	nParamNumber;
		MSG_NUM_4BIT	nParam[1];
	}MSG_Info;

	MSG_Info* m_pInfo;

};

#endif // end of _GAMESERVER_MSG_GAME_SMALL_ACTION_H_