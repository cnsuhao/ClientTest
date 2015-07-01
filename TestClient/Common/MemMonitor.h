////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: MemMonitor.h
// Author: ������(Peng Wenqi)
// Created: 2012-12-29
//		�ڴ����ģ��, ͳ���ڴ�ʹ�������Լ��Խ����ϵ��ڴ�ͳ����ֵ
////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_MEM_MONITOR_H_
#define _COMMON_MEM_MONITOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"
#include <vector>
const int MEM_MONITER_ONTIMER_SECOND = 300;	// �ڴ�����������

class CMyHeap;
typedef std::vector<CMyHeap*> VEC_HEAP;

class CMemMonitor
{
	// ����
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