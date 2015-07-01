////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: SymEngine.h
// Author: ������(Peng Wenqi)
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

	//ͨ���쳣��ӡ����ջ
	bool StackTrace(char* szBuf, CONTEXT *, unsigned skip = 0);

	//��ӡָ���̵߳���ջ(��ǰ�̴߳�ӡָ���������߳�)
	bool StackTrace(char* szBuf, DWORD dwThread, unsigned skip = 0);

	//��ӡ��ǰ�̵߳���ջ(ע���ǵ�ǰ�߳�)
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

