#include "./MsgGameTalk.h"

CMsgGameTalk::CMsgGameTalk( )
{
	Init();
	m_pInfo		=(MSG_Info *)m_bufMsg;
	m_unMsgType	= _MSG_GAME_TALK;
	m_unMsgSize	= sizeof(MSG_Info);
}

CMsgGameTalk::~CMsgGameTalk( )
{

}

BOOL CMsgGameTalk::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_GAME_TALK != this->GetType())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
// Description: 创建聊天消息
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: 
////////////////////////////////////////////////////////////////////////
bool CMsgGameTalk::CreateTalk( const char* pszText, TALK_TXTATR nTxtAttribute /*= TALK_TXTATR_WORLDSERVER*/, const char* pszTargetName /*= NULL*/ )
{
	CHECKF(pszText);
	short sTextSize = strlen(pszText) + 1;
	CHECKF(sTextSize >= 0 && sTextSize < MAX_PACKETSIZE - sizeof(MSG_Info) + 1);
	this->Init();
	m_unMsgType	= _MSG_GAME_TALK;
	m_unMsgSize	= sizeof(MSG_Info) - sizeof(char) + sTextSize;

	m_pInfo->nTxtAttribute	= nTxtAttribute;
	m_pInfo->nDataSize		= sTextSize;

	m_pInfo->idSender	= ID_NONE;
	::SafeCopy(m_pInfo->szSenderName, "SYSTEM", sizeof(m_pInfo->szSenderName));
	m_pInfo->nSenderLookface = 1;
	m_pInfo->nSenderLev = 999;

	if (pszTargetName)
	{
		::SafeCopy(m_pInfo->szReceiverName, pszTargetName, sizeof(m_pInfo->szReceiverName));
	}
	::SafeCopy(m_pInfo->szData, pszText, sTextSize + 1);
	return true;
}


// 处理消息
void CMsgGameTalk::Process(void *pInfo)
{
	//CHECK(pInfo);
	//CLogic* pLogic = (CLogic*)pInfo;
	//DEBUG_TRY;
	//pLogic->ProcessReplyTalk(m_pInfo->szSenderName, m_pInfo->szReceiverName, m_pInfo->szData, m_pInfo->nTxtAttribute);
	//DEBUG_CATCH("CMsgTalk::Process Crash!");
}