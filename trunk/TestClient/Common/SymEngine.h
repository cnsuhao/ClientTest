////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: SymEngine.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-11-16
///////////////////////////////////////////////////////////////////////
#ifndef _COMMON_SYM_ENGINE_H
#define _COMMON_SYM_ENGINE_H

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN64
#error Win64 is not supported
#endif

#include <windows.h>
#include "./dbghelp.h"
#include <string>
#include "./ThreadBase.h"

const INT MAX_BUFF_SIZE = 4096;
class CSymEngine
{
 public:
	 CSymEngine ();
	~CSymEngine();
	bool Init();

	//通过异常打印调用栈
	bool StackTrace(char* szBuf, CONTEXT *, unsigned skip = 0);

	//打印指定线程调用栈(当前线程打印指定的其他线程)
	bool StackTrace(char* szBuf, DWORD dwThread, unsigned skip = 0);

	//打印当前线程调用栈(注意是当前线程)
	bool StackTrace(char* szBuf);

 private:
	// symbol handler queries
	bool	GetSymbol(std::string& strSymbol);
	bool	GetModuleName (std::string& strModName);
	DWORD	GetFileLine(std::string& strFileName);
	 
	 // stack walk
	bool StackFirst (CONTEXT* pctx);
	bool StackNext  ();

	bool StackTrace(char*, CSymEngine&, CONTEXT*, unsigned skip = 1);

	bool LoadModuleFile(HANDLE, HMODULE);

 private:
	DWORD			m_dwAddress;
	STACKFRAME *	m_pframe;
	CONTEXT *		m_pctx;	
};


#endif //_COMMON_SYM_ENGINE_H

