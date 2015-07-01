////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: EnumDef.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2013-1-15
////////////////////////////////////////////////////////////////////////
#ifndef _DEBUGSERVER_ENUM_DEF_H_
#define _DEBUGSERVER_ENUM_DEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum { ID_NONE = 0, ID_SYS = 1};

enum E_MSGPORT_TYPE						// 消息管道类型
{
	MSGPORT_SHELL			= 0,		// 对话框接口
	MSGPORT_KERNEL			= 1,		// 核心

	MSGPORT_SIZE,						// 消息管道数量
};

enum THREAD_MSG_TYPE					// 线程消息类型
{
	//////////////////////////////////////////////////////////////////////////
	// 由外壳处理的消息
	SHELL_BASE = 0,
	SHELL_SERVER_STATTICS,				// 服务器统计信息

	//////////////////////////////////////////////////////////////////////////
	// 网络核心处理的消息
	NETWORK_BASE						= 10000,
	NETWORK_SERVER_INFO,				// 设置服务器信息
	NETWORK_APPLY,						// 申请连接服务器
	NETWORK_BACK,						// 回收连接服务器
	NETWORK_LOGIN,						// 登陆服务器
	NETWORK_LOGOUT,						// 登出服务器
};

// 程序状态
enum DLG_STATUS
{
	DLG_STATUS_FAILD	= 0,			// 失败
	DLG_STATUS_INIT,					// 初始化
	DLG_STATUS_RUNNING,					// 运行中

	DLG_STATUS_SERVER_WAIT,				// 服务器等待
	DLG_STATUS_SERVER_ERROR,			// 服务器未启动
};

// 服务器统计状态
enum SERVER_STATISTICS_STATE
{
	SERVER_STATISTICS_STATE_NONE,
	SERVER_STATISTICS_STATE_SYN,		// 需要同步
};

// 测试类型
enum TEST_TYPE
{
	TEST_TYPE_NONE		= 0,
	TEST_TYPE_NOPING	= 1,			// 使用既定账号		
	TEST_TYPE_PING		= 2,			// 需要拼账号
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Client属性记录
enum CLIENT_ATTR
{
	CLIENT_ATTR_BEGIN = 0,
	CLIENT_ATTR_STATE,						// 状态

	CLIENT_ATTR_ACCOUNT_ID,					// 账号id
	CLIENT_ATTR_LOGIN_KEY,					// 登陆key
	CLIENT_ATTR_USER_ID,					// 角色id
	CLIENT_ATTR_RANK,						// 权限
};

// Client状态
enum CLIENT_STATUS
{
	CLIENT_STATUS_INIT = 0,					// 初始态
	CLIENT_STATUS_CONNECT_WAIT,				// 连接服务器, 等待连接成功
	CLIENT_STATUS_CONNECT_PASSWORD,			// 连接服务器, 验证秘密
	CLIENT_STATUS_CONNECT_CHOOSE_PLAYER,	// 连接服务器, 选择玩家
	CLIENT_STATUS_RUNNING,					// 运行中
	CLIENT_STATUS_LOGIN_FAILD,				// 登陆失败
};

// Client权限
enum CLIENT_RANK
{
	CLIENT_RANK_BEGIN = 0,
	CLIENT_RANK_NONE = 0,					// 一般玩家
	CLIENT_RANK_LOW_PM,						// 普通GM
	CLIENT_RANK_HI_PM,						// 高级GM
	CLIENT_RANK_END,
};


// 踢出原因
enum KICK_REASON
{
	KICK_REASON_DEFAULT					= 0,	// 踢出默认值, 一般不要用这个值
	KICK_REASON_PASSWORD_ERROR			= 1,	// 密码错误
	KICK_REASON_BE_KICK_ANOTHER			= 2,	// 被别人踢
	KICK_REASON_RELATION_SERVER			= 3,	// 关系服务器指示你被踢了
	KICK_REASON_FULL					= 4,	// 人太多
	KICK_REASON_SYSTEM					= 5,	// 服务器控制面板踢人 或 GM踢人
	KICK_REASON_NOT_ALLOW				= 6,	// 服务器未开启登陆允许
	KICK_REASON_GM_MAC_ERROR			= 7,	// GM工具Mac地址验证错误
	KICK_REASON_CHECK_SPEED_TOO_FAST	= 8,	// 检测速度过快
	KICK_REASON_GMCMD					= 9,	// GM命令踢自己
	KICK_REASON_LOGIN_TIMEOUT			= 10,	// 登陆过程超时
	KICK_REASON_ACCOUNT_STOP_BYTIME		= 11,	// 账号临时封停
	KICK_REASON_ACCOUNT_STOP_FOREVER	= 12,	// 账号永久封停

};


// 聊天频道
enum TALK_TXTATR
{
	TALK_TXTATR_BEGIN			= 2000,
	TALK_TXTATR_PRIVATE			= 2001,	// 私聊
	TALK_TXTATR_SYNDICATE		= 2002,	// 帮派
	TALK_TXTATR_WORLDSERVER		= 2003,	// 世界(本区)
	TALK_TXTATR_GM_CMD			= 2004,	// GM命令
	TALK_TXTATR_NOTICE			= 2005,	// 公告
	TALK_TXTATR_HINT			= 2006,	// 提示
	TALK_TXTATR_GM_OPERATION	= 2007,	// GM操作(工具)
	TALK_TXTATR_END,
};

// 消息type号定义处, 定义协议号时强制将协议号显式填入
enum _MSG_TYPE_DEFINE_
{
	_MSG_NONE						= 0,

	_MSG_RL_SERVER_BEGIN			= 100,	// 入口服消息开始
	_MSG_RL_SERVER_REQ_LOGIN		= 101,	// 客户端请求登陆
	_MSG_RL_SERVER_QUERY_ONLINE		= 102,	// 关系服查询在线
	_MSG_RL_SERVER_END				= 200,	// 入口服消息结束

	// 客户端 登陆线程 消息区间
	_MSG_LOGIN_BEGIN				= 1000,
	_MSG_LOGIN_CONNECT				= 1001,	// 连接			C --> S
	_MSG_LOGIN_CONNECT_ACTION		= 1002,	// 连接操作		C <-> S
	_MSG_LOGIN_USER_LIST			= 1003,	// 玩家列表		C <-- S
	_MSG_LOGIN_CREATE_NEW_USER		= 1004,	// 建立新角色	C <-> S
	_MSG_LOGIN_KICK_REASON			= 1005,	// 踢出原因		C <-- S
	_MSG_LOGIN_END					= 2000,

	_MSG_GAME_BEGIN					= 5000,
	_MSG_GAME_LOGIN_QUERY			= 5001,	// 登陆查询				C <-> S
	_MSG_GAME_USER_INFO				= 5002,	// UserInfo				C <-- S
	_MSG_GAME_USER_ATTR				= 5003,	// UserAttr				C <-- S
	_MSG_GAME_USER_MONEY			= 5004,	// 货币同步				C <-- S
	_MSG_GAME_KICK_REASON			= 5005,	// 踢出原因				C <-- S
	_MSG_GAME_MASK					= 5006,	// 掩码同步				C <-- S
	_MSG_GAME_TMP_DATA				= 5007,	// UserTmpData			C <-> S
	_MSG_GAME_SMALLACTION			= 5008,	// SmallAction消息		C <-> S
	_MSG_GAME_TALK					= 5009, // 聊天消息				C <-> S
	_MSG_GAME_NEWCARD				= 5010,	// 新手卡				C <-> S
	_MSG_GAME_NEWCARD_PRIZE			= 5011,	// 新手卡奖励			C <-- S
	_MSG_GAME_TASK					= 5012, // 任务					C <-> S
	_MSG_GAME_ACTIVITIES			= 5013,	// 活动					C <-> S
	_MSG_GAME_SIGN					= 5014,	// 签到					C <-> S
	_MSG_GAME_BLACK_LIST			= 5015,	// 黑名单				C <-> S
	_MSG_GAME_ONLINE_INFO			= 5016,	// 在线玩家信息			S <-> R
	_MSG_GAME_MAIL_LIST				= 5017,	// 邮件列表				C <-> S
	_MSG_GAME_MAIL_OPT				= 5018,	// 邮件操作				C <-> S
	_MSG_GAME_SYN_SERVER			= 5019,	// 帮派服务器间消息		S <-> R
	_MSG_GAME_SYN_CLIENT			= 5020,	// 帮派客户端消息		C <-> S
	_MSG_GAME_VIP					= 5021,	// VIP消息				C <-> S
	_MSG_GAME_POKER_ACT				= 5022,	// 卡牌操作				C <-> S
	_MSG_GAME_POKER_INFO			= 5023,	// 卡牌信息				C <-- S
	_MSG_GAME_POKER_ATTR			= 5024,	// 卡牌属性				C <-- S
	_MSG_GAME_FRIEND				= 5025,	// 好友系统				C <-> S
	_MSG_GAME_MONEYTREE				= 5026,	// 摇钱系统				C <-> S
	_MSG_GAME_ACHIEVEMENT			= 5027,	// 成就系统				C <-> S
	_MSG_GAME_LIVENESS				= 5028,	// 每日活跃				C <-> S
	_MSG_GAME_SHOP					= 5029,	// 商店系统				C <-> S
	_MSG_GAME_LOTTERY				= 5030,	// 抽奖系统				C <-> S
	_MSG_GAME_ITEM					= 5031,	// 物品消息				C <-> S
	_MSG_GAME_EQUIP					= 5032,	// 装备消息				C <-> S
	_MSG_GAME_TOPLIST				= 5033,	// 排行榜系统			C <-> S
	_MSG_GAME_CHALLENGE_BOSS		= 5034,	// 挑战boss				C <-> S
	_MSG_GAME_BATTLE_INIT_INFO		= 5035,	// 战斗初始化信息		C <-- S
	_MSG_GAME_BATTLE_INIT_FIGHTER	= 5036,	// 战斗初始化参战者		C <-- S
	_MSG_GAME_BATTLE_END_INFO		= 5037,	// 战斗结束信息			C --> S
	_MSG_GAME_RELAY_DATABASE		= 5039, // 中转数据库			S <-> R
	_MSG_GAME_INSTANCE_ACT			= 5040, // 副本操作				C <-> S
	_MSG_GAME_ARENA_INFO			= 5041,	// 竞技场信息			C <-> S
	_MSG_GAME_ARENA_ACT				= 5042,	// 竞技场操作			C <-> S
	_MSG_GAME_GOOSE_TOWER			= 5043,	// 大雁塔				C <-> S
	_MSG_GAME_PUZZLE_BUFF			= 5044,	// 迷宫BUFF				C <-- S
	_MSG_GAME_PUZZLE				= 5045,	// 迷宫副本				C <-> S
	_MSG_GAME_SYN_LIST				= 5046,	// 帮派列表				C <-- S
	_MSG_GAME_SYN_MEMBER_LIST		= 5047,	// 帮派成员列表			C <-- S
	_MSG_GAME_SYN_INFO				= 5048,	// 帮派详情				C <-- S
	_MSG_GAME_SELF_SYN_INFO			= 5049,	// 自己帮派详情			C <-- S
	_MSG_GAME_SYN_LOG				= 5050,	// 帮派日志				C <-> S
	_MSG_GAME_MINE_CLIENT			= 5051,	// 矿洞消息				C <-> S
	_MSG_GAME_MERCENARY				= 5052,	// 佣兵系统				C <-> S
	_MSG_GAME_POKER_VIEW			= 5053,	// 卡牌查看信息			C <-- S
	_MSG_GAME_SYN_TERRIRORY			= 5054,	// 帮派领地战消息		C <-> S
	_MSG_GAME_VAS_INFO				= 5055,	// 元宝详情				C <-- S
	_MSG_GAME_SYN_WORSHIP			= 5056,	// 帮派膜拜大神			C <-> S
	_MSG_GAME_MINE_LOG				= 5057,	// 矿洞日志				C <-> S
	_MSG_GAME_WAR_ROAD_ACT			= 5058,	// 战役操作				C <-> S
	_MSG_GAME_WAR_ROAD_INFO			= 5059,	// 战役信息				C <-- S
	_MSG_GAME_USER_MAC				= 5060,	// 玩家mac地址			C --> S
	_MSG_GAME_END					= 7900,


	// php服务器 消息区间
	_MSG_PHP_BEGIN					= 7900,
	_MSG_PHP_PAY					= 8000,	// 充值消息
	_MSG_PHP_KICK					= 8001,	// php踢人
	_MSG_PHP_END					= 9000,

	// 关系服全部消息开始
	_MSG_RELATION_BEGIN						= 10000,

	// 关系服直接转发全服消息
	_MSG_RELATION_DIRCET_ALL_BEGIN			= 10000,
	_MSG_RELATION_DIRCET_ALL_TALK			= 10001,	// 聊天
	_MSG_RELATION_DIRCET_ALL_END			= 12000,

	// 关系服直接转发其他服消息
	_MSG_RELATION_DIRCET_OTHER_BEGIN		= 12000,
	_MSG_RELATION_DIRCET_OTHER_END			= 15000,

	// 需要关系服处理消息
	_MSG_RELATION_PROCESS_BEGIN				= 15000,
	_MSG_RELATION_PROCESS_LOGIN				= 15001,	// 游戏服登陆到关系服
	_MSG_RELATION_PROCESS_ACTION			= 15002,	// 游戏服与关系服 通用操作action
	_MSG_RELATION_PROCESS_NEWCARD			= 15003,	// 新手卡消息
	_MSG_RELATION_PROCESS_ONLINE_INFO		= 15004,	// 更新角色在线信息
	_MSG_RELATION_PROCESS_SYN_SERVER		= 15005,	// 帮派处理消息
	_MSG_RELATION_PROCESS_MAIL				= 15006,	// 邮件消息
	_MSG_RELATION_PROCESS_FRIEND			= 15007,	// 好友处理消息
	_MSG_RELATION_PROCESS_TOPLIST			= 15008,	// 排行榜系统
	_MSG_RELATION_PROCESS_REALY_DATABASE	= 15009,	// 中转数据库
	_MSG_RELATION_PROCESS_ARENA				= 15010,	// 竞技场
	_MSG_RELATION_PROCESS_SYN_WORSHIP		= 15011,	// 帮派膜拜大神
	_MSG_RELATION_PROCESS_END				= 20000,

	// 关系服全部消息结束
	_MSG_RELATION_END						= 20000,
};


#endif // end of _DEBUGSERVER_ENUM_DEF_H_
