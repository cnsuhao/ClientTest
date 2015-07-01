////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: EnumDef.h
// Author: ������(Peng Wenqi)
// Created: 2013-1-15
////////////////////////////////////////////////////////////////////////
#ifndef _DEBUGSERVER_ENUM_DEF_H_
#define _DEBUGSERVER_ENUM_DEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum { ID_NONE = 0, ID_SYS = 1};

enum E_MSGPORT_TYPE						// ��Ϣ�ܵ�����
{
	MSGPORT_SHELL			= 0,		// �Ի���ӿ�
	MSGPORT_KERNEL			= 1,		// ����

	MSGPORT_SIZE,						// ��Ϣ�ܵ�����
};

enum THREAD_MSG_TYPE					// �߳���Ϣ����
{
	//////////////////////////////////////////////////////////////////////////
	// ����Ǵ������Ϣ
	SHELL_BASE = 0,
	SHELL_SERVER_STATTICS,				// ������ͳ����Ϣ

	//////////////////////////////////////////////////////////////////////////
	// ������Ĵ������Ϣ
	NETWORK_BASE						= 10000,
	NETWORK_SERVER_INFO,				// ���÷�������Ϣ
	NETWORK_APPLY,						// �������ӷ�����
	NETWORK_BACK,						// �������ӷ�����
	NETWORK_LOGIN,						// ��½������
	NETWORK_LOGOUT,						// �ǳ�������
};

// ����״̬
enum DLG_STATUS
{
	DLG_STATUS_FAILD	= 0,			// ʧ��
	DLG_STATUS_INIT,					// ��ʼ��
	DLG_STATUS_RUNNING,					// ������

	DLG_STATUS_SERVER_WAIT,				// �������ȴ�
	DLG_STATUS_SERVER_ERROR,			// ������δ����
};

// ������ͳ��״̬
enum SERVER_STATISTICS_STATE
{
	SERVER_STATISTICS_STATE_NONE,
	SERVER_STATISTICS_STATE_SYN,		// ��Ҫͬ��
};

// ��������
enum TEST_TYPE
{
	TEST_TYPE_NONE		= 0,
	TEST_TYPE_NOPING	= 1,			// ʹ�üȶ��˺�		
	TEST_TYPE_PING		= 2,			// ��Ҫƴ�˺�
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Client���Լ�¼
enum CLIENT_ATTR
{
	CLIENT_ATTR_BEGIN = 0,
	CLIENT_ATTR_STATE,						// ״̬

	CLIENT_ATTR_ACCOUNT_ID,					// �˺�id
	CLIENT_ATTR_LOGIN_KEY,					// ��½key
	CLIENT_ATTR_USER_ID,					// ��ɫid
	CLIENT_ATTR_RANK,						// Ȩ��
};

// Client״̬
enum CLIENT_STATUS
{
	CLIENT_STATUS_INIT = 0,					// ��ʼ̬
	CLIENT_STATUS_CONNECT_WAIT,				// ���ӷ�����, �ȴ����ӳɹ�
	CLIENT_STATUS_CONNECT_PASSWORD,			// ���ӷ�����, ��֤����
	CLIENT_STATUS_CONNECT_CHOOSE_PLAYER,	// ���ӷ�����, ѡ�����
	CLIENT_STATUS_RUNNING,					// ������
	CLIENT_STATUS_LOGIN_FAILD,				// ��½ʧ��
};

// ClientȨ��
enum CLIENT_RANK
{
	CLIENT_RANK_BEGIN = 0,
	CLIENT_RANK_NONE = 0,					// һ�����
	CLIENT_RANK_LOW_PM,						// ��ͨGM
	CLIENT_RANK_HI_PM,						// �߼�GM
	CLIENT_RANK_END,
};


// �߳�ԭ��
enum KICK_REASON
{
	KICK_REASON_DEFAULT					= 0,	// �߳�Ĭ��ֵ, һ�㲻Ҫ�����ֵ
	KICK_REASON_PASSWORD_ERROR			= 1,	// �������
	KICK_REASON_BE_KICK_ANOTHER			= 2,	// ��������
	KICK_REASON_RELATION_SERVER			= 3,	// ��ϵ������ָʾ�㱻����
	KICK_REASON_FULL					= 4,	// ��̫��
	KICK_REASON_SYSTEM					= 5,	// ����������������� �� GM����
	KICK_REASON_NOT_ALLOW				= 6,	// ������δ������½����
	KICK_REASON_GM_MAC_ERROR			= 7,	// GM����Mac��ַ��֤����
	KICK_REASON_CHECK_SPEED_TOO_FAST	= 8,	// ����ٶȹ���
	KICK_REASON_GMCMD					= 9,	// GM�������Լ�
	KICK_REASON_LOGIN_TIMEOUT			= 10,	// ��½���̳�ʱ
	KICK_REASON_ACCOUNT_STOP_BYTIME		= 11,	// �˺���ʱ��ͣ
	KICK_REASON_ACCOUNT_STOP_FOREVER	= 12,	// �˺����÷�ͣ

};


// ����Ƶ��
enum TALK_TXTATR
{
	TALK_TXTATR_BEGIN			= 2000,
	TALK_TXTATR_PRIVATE			= 2001,	// ˽��
	TALK_TXTATR_SYNDICATE		= 2002,	// ����
	TALK_TXTATR_WORLDSERVER		= 2003,	// ����(����)
	TALK_TXTATR_GM_CMD			= 2004,	// GM����
	TALK_TXTATR_NOTICE			= 2005,	// ����
	TALK_TXTATR_HINT			= 2006,	// ��ʾ
	TALK_TXTATR_GM_OPERATION	= 2007,	// GM����(����)
	TALK_TXTATR_END,
};

// ��Ϣtype�Ŷ��崦, ����Э���ʱǿ�ƽ�Э�����ʽ����
enum _MSG_TYPE_DEFINE_
{
	_MSG_NONE						= 0,

	_MSG_RL_SERVER_BEGIN			= 100,	// ��ڷ���Ϣ��ʼ
	_MSG_RL_SERVER_REQ_LOGIN		= 101,	// �ͻ��������½
	_MSG_RL_SERVER_QUERY_ONLINE		= 102,	// ��ϵ����ѯ����
	_MSG_RL_SERVER_END				= 200,	// ��ڷ���Ϣ����

	// �ͻ��� ��½�߳� ��Ϣ����
	_MSG_LOGIN_BEGIN				= 1000,
	_MSG_LOGIN_CONNECT				= 1001,	// ����			C --> S
	_MSG_LOGIN_CONNECT_ACTION		= 1002,	// ���Ӳ���		C <-> S
	_MSG_LOGIN_USER_LIST			= 1003,	// ����б�		C <-- S
	_MSG_LOGIN_CREATE_NEW_USER		= 1004,	// �����½�ɫ	C <-> S
	_MSG_LOGIN_KICK_REASON			= 1005,	// �߳�ԭ��		C <-- S
	_MSG_LOGIN_END					= 2000,

	_MSG_GAME_BEGIN					= 5000,
	_MSG_GAME_LOGIN_QUERY			= 5001,	// ��½��ѯ				C <-> S
	_MSG_GAME_USER_INFO				= 5002,	// UserInfo				C <-- S
	_MSG_GAME_USER_ATTR				= 5003,	// UserAttr				C <-- S
	_MSG_GAME_USER_MONEY			= 5004,	// ����ͬ��				C <-- S
	_MSG_GAME_KICK_REASON			= 5005,	// �߳�ԭ��				C <-- S
	_MSG_GAME_MASK					= 5006,	// ����ͬ��				C <-- S
	_MSG_GAME_TMP_DATA				= 5007,	// UserTmpData			C <-> S
	_MSG_GAME_SMALLACTION			= 5008,	// SmallAction��Ϣ		C <-> S
	_MSG_GAME_TALK					= 5009, // ������Ϣ				C <-> S
	_MSG_GAME_NEWCARD				= 5010,	// ���ֿ�				C <-> S
	_MSG_GAME_NEWCARD_PRIZE			= 5011,	// ���ֿ�����			C <-- S
	_MSG_GAME_TASK					= 5012, // ����					C <-> S
	_MSG_GAME_ACTIVITIES			= 5013,	// �					C <-> S
	_MSG_GAME_SIGN					= 5014,	// ǩ��					C <-> S
	_MSG_GAME_BLACK_LIST			= 5015,	// ������				C <-> S
	_MSG_GAME_ONLINE_INFO			= 5016,	// ���������Ϣ			S <-> R
	_MSG_GAME_MAIL_LIST				= 5017,	// �ʼ��б�				C <-> S
	_MSG_GAME_MAIL_OPT				= 5018,	// �ʼ�����				C <-> S
	_MSG_GAME_SYN_SERVER			= 5019,	// ���ɷ���������Ϣ		S <-> R
	_MSG_GAME_SYN_CLIENT			= 5020,	// ���ɿͻ�����Ϣ		C <-> S
	_MSG_GAME_VIP					= 5021,	// VIP��Ϣ				C <-> S
	_MSG_GAME_POKER_ACT				= 5022,	// ���Ʋ���				C <-> S
	_MSG_GAME_POKER_INFO			= 5023,	// ������Ϣ				C <-- S
	_MSG_GAME_POKER_ATTR			= 5024,	// ��������				C <-- S
	_MSG_GAME_FRIEND				= 5025,	// ����ϵͳ				C <-> S
	_MSG_GAME_MONEYTREE				= 5026,	// ҡǮϵͳ				C <-> S
	_MSG_GAME_ACHIEVEMENT			= 5027,	// �ɾ�ϵͳ				C <-> S
	_MSG_GAME_LIVENESS				= 5028,	// ÿ�ջ�Ծ				C <-> S
	_MSG_GAME_SHOP					= 5029,	// �̵�ϵͳ				C <-> S
	_MSG_GAME_LOTTERY				= 5030,	// �齱ϵͳ				C <-> S
	_MSG_GAME_ITEM					= 5031,	// ��Ʒ��Ϣ				C <-> S
	_MSG_GAME_EQUIP					= 5032,	// װ����Ϣ				C <-> S
	_MSG_GAME_TOPLIST				= 5033,	// ���а�ϵͳ			C <-> S
	_MSG_GAME_CHALLENGE_BOSS		= 5034,	// ��սboss				C <-> S
	_MSG_GAME_BATTLE_INIT_INFO		= 5035,	// ս����ʼ����Ϣ		C <-- S
	_MSG_GAME_BATTLE_INIT_FIGHTER	= 5036,	// ս����ʼ����ս��		C <-- S
	_MSG_GAME_BATTLE_END_INFO		= 5037,	// ս��������Ϣ			C --> S
	_MSG_GAME_RELAY_DATABASE		= 5039, // ��ת���ݿ�			S <-> R
	_MSG_GAME_INSTANCE_ACT			= 5040, // ��������				C <-> S
	_MSG_GAME_ARENA_INFO			= 5041,	// ��������Ϣ			C <-> S
	_MSG_GAME_ARENA_ACT				= 5042,	// ����������			C <-> S
	_MSG_GAME_GOOSE_TOWER			= 5043,	// ������				C <-> S
	_MSG_GAME_PUZZLE_BUFF			= 5044,	// �Թ�BUFF				C <-- S
	_MSG_GAME_PUZZLE				= 5045,	// �Թ�����				C <-> S
	_MSG_GAME_SYN_LIST				= 5046,	// �����б�				C <-- S
	_MSG_GAME_SYN_MEMBER_LIST		= 5047,	// ���ɳ�Ա�б�			C <-- S
	_MSG_GAME_SYN_INFO				= 5048,	// ��������				C <-- S
	_MSG_GAME_SELF_SYN_INFO			= 5049,	// �Լ���������			C <-- S
	_MSG_GAME_SYN_LOG				= 5050,	// ������־				C <-> S
	_MSG_GAME_MINE_CLIENT			= 5051,	// ����Ϣ				C <-> S
	_MSG_GAME_MERCENARY				= 5052,	// Ӷ��ϵͳ				C <-> S
	_MSG_GAME_POKER_VIEW			= 5053,	// ���Ʋ鿴��Ϣ			C <-- S
	_MSG_GAME_SYN_TERRIRORY			= 5054,	// �������ս��Ϣ		C <-> S
	_MSG_GAME_VAS_INFO				= 5055,	// Ԫ������				C <-- S
	_MSG_GAME_SYN_WORSHIP			= 5056,	// ����Ĥ�ݴ���			C <-> S
	_MSG_GAME_MINE_LOG				= 5057,	// ����־				C <-> S
	_MSG_GAME_WAR_ROAD_ACT			= 5058,	// ս�۲���				C <-> S
	_MSG_GAME_WAR_ROAD_INFO			= 5059,	// ս����Ϣ				C <-- S
	_MSG_GAME_USER_MAC				= 5060,	// ���mac��ַ			C --> S
	_MSG_GAME_END					= 7900,


	// php������ ��Ϣ����
	_MSG_PHP_BEGIN					= 7900,
	_MSG_PHP_PAY					= 8000,	// ��ֵ��Ϣ
	_MSG_PHP_KICK					= 8001,	// php����
	_MSG_PHP_END					= 9000,

	// ��ϵ��ȫ����Ϣ��ʼ
	_MSG_RELATION_BEGIN						= 10000,

	// ��ϵ��ֱ��ת��ȫ����Ϣ
	_MSG_RELATION_DIRCET_ALL_BEGIN			= 10000,
	_MSG_RELATION_DIRCET_ALL_TALK			= 10001,	// ����
	_MSG_RELATION_DIRCET_ALL_END			= 12000,

	// ��ϵ��ֱ��ת����������Ϣ
	_MSG_RELATION_DIRCET_OTHER_BEGIN		= 12000,
	_MSG_RELATION_DIRCET_OTHER_END			= 15000,

	// ��Ҫ��ϵ��������Ϣ
	_MSG_RELATION_PROCESS_BEGIN				= 15000,
	_MSG_RELATION_PROCESS_LOGIN				= 15001,	// ��Ϸ����½����ϵ��
	_MSG_RELATION_PROCESS_ACTION			= 15002,	// ��Ϸ�����ϵ�� ͨ�ò���action
	_MSG_RELATION_PROCESS_NEWCARD			= 15003,	// ���ֿ���Ϣ
	_MSG_RELATION_PROCESS_ONLINE_INFO		= 15004,	// ���½�ɫ������Ϣ
	_MSG_RELATION_PROCESS_SYN_SERVER		= 15005,	// ���ɴ�����Ϣ
	_MSG_RELATION_PROCESS_MAIL				= 15006,	// �ʼ���Ϣ
	_MSG_RELATION_PROCESS_FRIEND			= 15007,	// ���Ѵ�����Ϣ
	_MSG_RELATION_PROCESS_TOPLIST			= 15008,	// ���а�ϵͳ
	_MSG_RELATION_PROCESS_REALY_DATABASE	= 15009,	// ��ת���ݿ�
	_MSG_RELATION_PROCESS_ARENA				= 15010,	// ������
	_MSG_RELATION_PROCESS_SYN_WORSHIP		= 15011,	// ����Ĥ�ݴ���
	_MSG_RELATION_PROCESS_END				= 20000,

	// ��ϵ��ȫ����Ϣ����
	_MSG_RELATION_END						= 20000,
};


#endif // end of _DEBUGSERVER_ENUM_DEF_H_
