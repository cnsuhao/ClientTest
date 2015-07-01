////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgSmallAction.cpp
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-10-23
////////////////////////////////////////////////////////////////////////
#include "./MsgGameSmallAction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgGameSmallAction::CMsgGameSmallAction()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgType	= _MSG_GAME_SMALLACTION;
	m_unMsgSize	= sizeof(MSG_Info);
}

//////////////////////////////////////////////////////////////////////////
CMsgGameSmallAction::~CMsgGameSmallAction()
{
}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgGameSmallAction::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GAME_SMALLACTION != this->GetType())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  创建SmallAction
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
bool CMsgGameSmallAction::CreateSmallAction(MSG_SMALL_ACTION eAction, MSG_SMALL_RESULT eResult)
{
	this->Init();
	m_unMsgType	= _MSG_GAME_SMALLACTION;
	m_unMsgSize	= sizeof(MSG_Info) - sizeof(MSG_NUM_4BIT);

	m_pInfo->sAction = (MSG_NUM_2BIT)eAction;
	m_pInfo->sResult = (MSG_NUM_2BIT)eResult;
	m_pInfo->nParamNumber = 0;

	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  增加参数
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool CMsgGameSmallAction::AppendParam(int nAddParam)
{
	CHECKF(sizeof(MSG_Info) + sizeof(MSG_NUM_4BIT) * m_pInfo->nParamNumber < MAX_PACKETSIZE);
	m_pInfo->nParam[m_pInfo->nParamNumber] = nAddParam;
	m_unMsgSize = sizeof(MSG_Info) + sizeof(MSG_NUM_4BIT) * m_pInfo->nParamNumber;
	++(m_pInfo->nParamNumber);
	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  获得Param参数容器
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: int
////////////////////////////////////////////////////////////////////////
int CMsgGameSmallAction::GetParamVector(OUT INT_VEC& rVec)
{
	DEBUG_TRY;
	rVec.clear();
	CHECKF(m_pInfo->nParamNumber >= 0 && m_pInfo->nParamNumber < (MAX_PACKETSIZE - 8) / sizeof(int));
	for (int i = 0; i < m_pInfo->nParamNumber; ++i)
	{
		rVec.push_back(m_pInfo->nParam[i]);
	}
	DEBUG_CATCH("CMsgSmallAction::GetParamVector Error");
	return rVec.size();
}

////////////////////////////////////////////////////////////////////////
// Description:  处理消息
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CMsgGameSmallAction::Process(void *pInfo)
{
	CHECK(pInfo);
	DEBUG_TRY;


	DEBUG_CATCH2("CMsgSmallAction::Process Action[%d]", m_pInfo->sAction);
}


