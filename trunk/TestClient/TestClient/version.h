////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
//		版本号模板头文件
//		每次出版本, 服务端版本号会自动读取svn目录
// 
// 注意: ★★★★★ version.h 是无需修改的, 每次重新编译均会自动重新生成 ★★★★★ 
//		 只需修改version_template.h文件中客户端版本号, version.h文件会根据当前版本号进行生成
// 
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-3-12
////////////////////////////////////////////////////////////////////////
#ifndef _TalkMonitor_VERSION_H_
#define _TalkMonitor_VERSION_H_

// 服务端版本号, 不用理会, 编译时会提取svn目录的版本号
#define VER_SERVER_SVN_VISION	1580

// 服务端SVN的URL地址
#define VER_SERVER_SVN_URL		"https://192.168.1.253/svn/card_server/Tool/TalkMonitor"

#endif // _TalkMonitor_VERSION_H_