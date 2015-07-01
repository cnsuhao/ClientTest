#ifndef _GAMEKERNEL_MSG_TALK_H_
#define _GAMEKERNEL_MSG_TALK_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "./Msg.h"

// ���������Ϣ
struct ST_TALK_USER_INFO
{
	OBJID	idUser;						// ���ID
	OBJID	idSyn;						// ��Ұ���ID
	char	szUserName[MAX_NAME_SIZE];	// ��ҽ�ɫ��
	int		nLookface;					// ���ͷ��
	int		nLev;						// ��ҵȼ�
	int		nUtcTime;					// Utcʱ��
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
		MSG_NUM_4BIT	nTxtAttribute;		// Ƶ��

		MSG_NUM_4BIT	idSender;			// ���ID
		MSG_STR_32BIT	szSenderName;		// ��ҽ�ɫ��
		MSG_NUM_4BIT	nSenderLookface;	// ���ͷ��
		MSG_NUM_4BIT	nSenderLev;			// ��ҵȼ�

		MSG_NUM_4BIT	nServerUtcTime;		// Utcʱ��
		MSG_NUM_4BIT	nSynID;				// ��Ұ���ID

		MSG_NUM_4BIT	nReceiverID;		// ������ID	
		MSG_STR_32BIT	szReceiverName;		// �����߽�ɫ��
		
		MSG_NUM_4BIT	nTxtType;			// ����
		MSG_NUM_4BIT	nClientParam1;		// client����1
		MSG_NUM_4BIT	nClientParam2;		// client����2
		MSG_NUM_4BIT	nClientParam3;		// client����3
		MSG_NUM_4BIT	nClientParam4;		// client����4

		MSG_NUM_4BIT	nServerParam1;		// server����1
		MSG_NUM_4BIT	nServerParam2;		// server����2
		MSG_NUM_4BIT	nServerParam3;		// server����3
		MSG_NUM_4BIT	nServerParam4;		// server����4

		MSG_NUM_4BIT	nDataSize;			// �䳤���鳤��
		char			szData[1];			// �䳤����
	}MSG_Info;
	MSG_Info* m_pInfo;
};


#endif