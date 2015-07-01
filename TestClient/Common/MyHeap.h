////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: MyHeap.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-10-30
////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_MY_HEAP_H_
#define _COMMON_MY_HEAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

#include <windows.h>

const int	HEAP_LFH		= 2;		// 低碎片堆标记
const int	CLASSNAME_SIZE	= 64;		// 最长类名

class CMyHeap
{
public:
	CMyHeap(const char* pszClassName = NULL);
	~CMyHeap();
	
public:
	void*		Alloc(size_t size);
	void		Free(void* ptr);
	bool		IsValidPt(void* p);
	LONG		GetNumAlloc(void) const {return m_lNumAlloc;}
	LONG		GetMaxAlloc(void) const {return m_lMaxAlloc;}
	const char*	GetClassName(void)const {return m_szClassName;}

protected:
	HANDLE		m_hHeap;
	LONG		m_lNumAlloc;
	LONG		m_lMaxAlloc;
	char		m_szClassName[CLASSNAME_SIZE];
};

// Marco define	
#define	MYHEAP_DECLARATION(VAR)													\
		void*  operator new	(size_t size)	{ return VAR.Alloc(size); }			\
		void   operator delete	(void* p)	{ VAR.Free(p); }					\
		void*  operator new[] (size_t size)	{ return VAR.Alloc(size); }			\
		void   operator delete[] (void* p)	{ VAR.Free(p); }					\
	public:																		\
		static bool			IsValidPt(void* p)	{ return VAR.IsValidPt(p); }	\
	protected:																	\
		static CMyHeap	VAR;
	
#define	MYHEAP_IMPLEMENTATION(T,VAR)					CMyHeap	T::VAR(#T);	
#define	MYHEAP_IMPLEMENTATION_NICKNAME(T,VAR,NICKNAME)	CMyHeap	T::VAR(#NICKNAME);			

#endif // _COMMON_MY_HEAP_H_
