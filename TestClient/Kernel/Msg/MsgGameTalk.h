#ifndef _GAMEKERNEL_MSG_TALK_H_
#define _GAMEKERNEL_MSG_TALK_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "./Msg.h"

// 聊天玩家信息
struct ST_TALK_USER_INFO
{
	OBJID	idUser;						// 玩家ID
	OBJID	idSyn;						// 玩家帮派ID
	char	szUserName[MAX_NAME_SIZE];	// 玩家角色名
	int		nLookface;					// 玩家头像
	int		nLev;						// 玩家等级
	int		nUtcTime;					// Utc时间
};

class CMsgGameTalk : public CMsg
{
public:
	CMsgGameTalk();
	virtual ~CMsgGameTalk();

public:
	virtual BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
	virtual void	Process(void *pInfo);

public:
	bool	CreateTalk		(const char* pszText, TALK_TXTATR nTxtAttribute = TALK_TXTATR_WORLDSERVER, const char* pszTargetName = NULL);


private:
	typedef struct
	{
		MSG_NUM_4BIT	nTxtAttribute;		// 频道

		MSG_NUM_4BIT	idSender;			// 玩家ID
		MSG_STR_32BIT	szSenderName;		// 玩家角色名
		MSG_NUM_4BIT	nSenderLookface;	// 玩家头像
		MSG_NUM_4BIT	nSenderLev;			// 玩家等级

		MSG_NUM_4BIT	nServerUtcTime;		// Utc时间
		MSG_NUM_4BIT	nSynID;				// 玩家帮派ID

		MSG_NUM_4BIT	nReceiverID;		// 接收者ID	
		MSG_STR_32BIT	szReceiverName;		// 接收者角色名
		
		MSG_NUM_4BIT	nTxtType;			// 类型
		MSG_NUM_4BIT	nClientParam1;		// client参数1
		MSG_NUM_4BIT	nClientParam2;		// client参数2
		MSG_NUM_4BIT	nClientParam3;		// client参数3
		MSG_NUM_4BIT	nClientParam4;		// client参数4

		MSG_NUM_4BIT	nServerParam1;		// server参数1
		MSG_NUM_4BIT	nServerParam2;		// server参数2
		MSG_NUM_4BIT	nServerParam3;		// server参数3
		MSG_NUM_4BIT	nServerParam4;		// server参数4

		MSG_NUM_4BIT	nDataSize;			// 变长数组长度
		char			szData[1];			// 变长数组
	}MSG_Info;
	MSG_Info* m_pInfo;
};


#endif