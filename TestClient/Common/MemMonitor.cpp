////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: MemMonitor.cpp
// Author: ������(Peng Wenqi)
// Created: 2012-12-29
////////////////////////////////////////////////////////////////////////
#pragma	warning(disable:4786)
#include "./MemMonitor.h"
#include "./BaseFunc.h"
#include "./MyHeap.h"

CMemMonitor* CMemMonitor::s_pInstance = NULL;
////////////////////////////////////////////////////////////////////////
// Description:  ������ȡ
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: CMemMonitor*
////////////////////////////////////////////////////////////////////////
CMemMonitor* CMemMonitor::GetInstance( void )
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new CMemMonitor;
	}
	
	return s_pInstance;
}

////////////////////////////////////////////////////////////////////////
// Description:  ��������
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CMemMonitor::DelInstance( void )
{
	SAFE_DELETE(s_pInstance);
}

////////////////////////////////////////////////////////////////////////
// Description:  ע��һ���Խ��Ѽ���
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CMemMonitor::RegMyHeap(CMyHeap* pHeap)
{
	CHECK(pHeap);
	m_vecMyHeap.push_back(pHeap);
}

//////////////////////////////////////////////////////////////////////////
CMemMonitor::CMemMonitor()
{
	m_dwOnTimerSecond	= ::TimeGet(TIME_SECOND) - ::RandGet(60);
	m_nCount			= 0;
}

//////////////////////////////////////////////////////////////////////////
CMemMonitor::~CMemMonitor()
{
	m_dwOnTimerSecond = 2000000000;	// �óɺ�ңԶ��2033��, ��ontimer���ڴ�������
	m_vecMyHeap.clear();
}

////////////////////////////////////////////////////////////////////////
// Description:  ��ʱ�������ڴ�
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CMemMonitor::OnTimerMonitor(const char* pszOtherLogText)
{
	TimeCostChk("TimeCheck", "CMemMonitor::OnTimerMonitor");
	CHECK(pszOtherLogText);
	const DWORD dwNowSecond = ::TimeGet(TIME_SECOND);
	if (dwNowSecond < m_dwOnTimerSecond + MEM_MONITER_ONTIMER_SECOND)
	{
		return;
	}
	m_dwOnTimerSecond = dwNowSecond;
	m_nCount++;

	MEMORYSTATUSEX stMemStatus = {0};
	stMemStatus.dwLength = sizeof(stMemStatus);
	::GlobalMemoryStatusEx(&stMemStatus);

	::LogMemMonitor("\n========================================== Start Mem Monitor nCount[%04d] ==========================================", m_nCount);
	::LogMemMonitor("SystemInfo: MemLoad[%03u]Percent, TotalPhys: [%06I64u]MB.[%04I64u]KB, AvailPhys: [%06I64u]MB.[%04I64u]KB     ", stMemStatus.dwMemoryLoad, stMemStatus.ullTotalPhys / 1024 / 1024, stMemStatus.ullTotalPhys / 1024 % 1024, stMemStatus.ullAvailPhys / 1024 / 1024, stMemStatus.ullAvailPhys / 1024 % 1024);
	::LogMemMonitor("            TotalPageFile: [%06I64u]MB.[%04I64u]KB, AvailPageFile:[%06I64u]MB.[%04I64u]KB                   ",	stMemStatus.ullTotalPageFile/ 1024 / 1024, stMemStatus.ullTotalPageFile/ 1024 % 1024, stMemStatus.ullAvailPageFile / 1024 / 1024, stMemStatus.ullAvailPageFile/ 1024 % 1024);
	::LogMemMonitor("            TotalVirtual:  [%06I64u]MB.[%04I64u]KB, TotalVirtual: [%06I64u]MB.[%04I64u]KB                   ",	stMemStatus.ullTotalVirtual / 1024 / 1024, stMemStatus.ullTotalVirtual/ 1024 % 1024, stMemStatus.ullAvailVirtual / 1024 / 1024, stMemStatus.ullAvailVirtual/ 1024 % 1024);
	::LogMemMonitor("GameServer: %s", pszOtherLogText);
	for (VEC_HEAP::const_iterator iter = m_vecMyHeap.begin(); iter != m_vecMyHeap.end(); iter++)
	{
		CMyHeap* pHeap = *iter;
		CHECKC(pHeap);
		if (pHeap->GetMaxAlloc() <= 2)
		{
			continue;
		}
		::LogMemMonitor("HeapClassName[%32s], NumAlloc[%08d], MaxAlloc[%08d]\t\t\t\t", pHeap->GetClassName(), pHeap->GetNumAlloc(), pHeap->GetMaxAlloc());
	}
}
