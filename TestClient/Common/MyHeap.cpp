////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: MyHeap.cpp
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-10-30
////////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "MyHeap.h"
#include "common.h"
#include "logfile.h"
#include "BaseFunc.h"
#include "MemMonitor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyHeap::CMyHeap(const char* pszClassName)
{
	m_hHeap	=::HeapCreate(0, 0, 0);
	m_lNumAlloc	= 0;
	m_lMaxAlloc	= 0;

	if(pszClassName)
		SafeCopy(m_szClassName, pszClassName, CLASSNAME_SIZE);
	else
		SafeCopy(m_szClassName, "Default Class Name", CLASSNAME_SIZE);

	// 每个自建堆开启低碎片标记, debug模式lfh不开启
#ifndef _DEBUG
	if (NULL != m_hHeap)
	{ 
		int nFlag = HEAP_LFH;
		BOOL bResult = ::HeapSetInformation(m_hHeap, HeapCompatibilityInformation, &nFlag, sizeof(nFlag));
		if (FALSE == bResult) 
		{
			::LogSave("[MyHeap] Heap[%s] Open LFH Failed! LastError[%d] ", m_szClassName, GetLastError());
		}
	}
#endif // end of USE_LFH_FOR_MY_HEAP

	if (NULL == m_hHeap)
	{
		::LogSave("[MyHeap] ERROR: Heap[%s] Create Heap Failed! ", m_szClassName);
	}
	else
	{
		CMemMonitor::GetInstance()->RegMyHeap(this);	// 注册内存监视器
	}
}

//////////////////////////////////////////////////////////////////////
CMyHeap::~CMyHeap()
{
	if (m_hHeap)
		::HeapDestroy(m_hHeap);

	if (m_lNumAlloc != 0)
	{
		if(strlen(m_szClassName) > 0)
		{
			::LogSave("[MyHeap] Warning !!! MemoryLeak!!!!!! Class [%s] have [%d] memory which did not release", m_szClassName, m_lNumAlloc);
		}
		else
		{
			::LogSave("[MyHeap] Warning !!! MemoryLeak!!!!!! Have [%d] memory which did not release", m_lNumAlloc);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void* CMyHeap::Alloc(size_t size)
{
	IF_NOT (m_hHeap)
		return NULL;
	if(size == 0)
		return false;
	
	// alloc mem for new obj
	void* p = NULL;
	VA_TRY
	p	=::HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY, size);
	}catch(CMemoryException* ) { ::LogSave("CATCH: *** {" "HeapAlloc memory failed[%s][%d]." "} new failed! ***", m_szClassName, size);
	VA_CATCH("HeapAlloc")

	if (p)
	{
		InterlockedIncrement(&m_lNumAlloc);
		if (m_lNumAlloc > m_lMaxAlloc)
		{
			m_lMaxAlloc = m_lNumAlloc;
		}
	}
	return p;
}

//////////////////////////////////////////////////////////////////////
void CMyHeap::Free(void* p)
{
	if (!p)
		return;
	
	IF_NOT (m_hHeap)
		return;
	
	IF_NOT(m_lNumAlloc > 0)
	{
		::LogSave("Memory amount of Class [%s] is error!", m_szClassName);
	}
	
	// free it...
	if (::HeapFree(m_hHeap, 0, p))
	{
		::InterlockedDecrement(&m_lNumAlloc);
	}
}
	
//////////////////////////////////////////////////////////////////////
bool CMyHeap::IsValidPt(void* p)
{
	if (!m_hHeap || !p)
		return false;
	
	// user address range from 1M--2G-64k
	if ((DWORD)p < 0x00010000 || (DWORD)p >= 0x7FFEFFFF)	
		return false;
	
	if (::IsBadCodePtr((FARPROC)p))
		return false;
	
	if (::HeapValidate(m_hHeap, 0, p))
		return true;
	
	return false;
}





