////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: MemMonitor.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-12-29
//		内存监视模块, 统计内存使用总量以及自建堆上的内存统计数值
////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_MEM_MONITOR_H_
#define _COMMON_MEM_MONITOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"
#include <vector>
const int MEM_MONITER_ONTIMER_SECOND = 300;	// 内存监视跳动间隔

class CMyHeap;
typedef std::vector<CMyHeap*> VEC_HEAP;

class CMemMonitor
{
	// 单例
public:
	static CMemMonitor*	GetInstance(void);
	static void			DelInstance(void);
private:
	CMemMonitor();
	virtual ~CMemMonitor();
	static CMemMonitor*	s_pInstance;

public:
	void	RegMyHeap(CMyHeap* pHeap);
	void	OnTimerMonitor(const char* pszOtherLogText);

private:
	VEC_HEAP		m_vecMyHeap;
	DWORD			m_dwOnTimerSecond;
	int				m_nCount;
};

#endif // end of _COMMON_MEM_MONITOR_H_