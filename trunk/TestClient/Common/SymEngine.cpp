/*
 Copyright (c) 2001 - 2002
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 
 Created: 25.12.2001 19:41:07
 Version: 1.0.2

 Permission to use, copy, modify, distribute and sell this software
 and its documentation for any purpose is hereby granted without fee,
 provided that the above copyright notice appear in all copies and
 that both that copyright notice and this permission notice appear
 in supporting documentation.  Konstantin Boukreev makes no representations
 about the suitability of this software for any purpose.
 It is provided "as is" without express or implied warranty.

*/


#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501 
#define VC_EXTRALEAN
#define NOSOUND

#ifdef _DEBUG
#pragma warning(disable:4127)		// conditional expression is constant
#endif

#pragma warning(disable:4786)		// disable "identifier was truncated to 'number' characters in the debug information"
#pragma warning(disable:4290)		// C++ Exception Specification ignored
#pragma warning(disable:4097)		// typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable:4001)		// nonstandard extension 'single line comment' was used
#pragma warning(disable:4100)		// unreferenced formal parameter
#pragma warning(disable:4699)		// Note: Creating precompiled header 
#pragma warning(disable:4710)		// function not inlined
#pragma warning(disable:4514)		// unreferenced inline function has been removed
#pragma warning(disable:4512)		// assignment operator could not be generated
#pragma warning(disable:4310)		// cast truncates constant value
#pragma warning(disable:4018)
#pragma warning(disable:4284)
#pragma warning(disable:4146)
#pragma warning(disable:4996)
#include <eh.h>

#pragma warning(push, 3) 
#include <exception>
#pragma warning(pop) 

#include "SymEngine.h"
#include <crtdbg.h>
#include <malloc.h> 
#include <tlhelp32.h>
#include <stdio.h>
#include <string>
#include <time.h>

#ifdef VERIFY
#undef VERIFY
#endif // VERIFY

#ifdef _DEBUG
#define VERIFY(x) (x)
#else
#define VERIFY(x) (x)
#endif //_DEBUG

// 线程互斥量, 全局只能有唯一的这东西在运行
CSimpleCriticalSection	g_csSymEngineLock;

//////////////////////////////////////////////////////////////////////////
BOOL __stdcall My_ReadProcessMemory (HANDLE, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead)
{
    return ReadProcessMemory(GetCurrentProcess(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

//////////////////////////////////////////////////////////////////////////
CSymEngine::CSymEngine()
: m_dwAddress(0)
, m_pframe(0) 
{
	m_pctx = NULL;
	this->Init();
}

//////////////////////////////////////////////////////////////////////////
CSymEngine::~CSymEngine()
{
	VERIFY(SymCleanup(GetCurrentProcess()));
	if (m_pframe)
	{
		delete m_pframe;
		m_pframe = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::Init()
{
	SymSetOptions (SymGetOptions()|SYMOPT_DEFERRED_LOADS|SYMOPT_LOAD_LINES);

	HANDLE hProc = GetCurrentProcess();
	if (!::SymInitialize(hProc, 0, TRUE))
	{
		return false;
	}

	HINSTANCE hInst = LoadLibrary("psapi.dll");
	if (!hInst)
	{
		return false;
	}
	
	typedef BOOL (WINAPI *ENUMPROCESSMODULES)(HANDLE, HMODULE*, DWORD, LPDWORD);	
	ENUMPROCESSMODULES fnEnumProcessModules = (ENUMPROCESSMODULES)GetProcAddress(hInst, "EnumProcessModules");
	
	DWORD cbNeeded = 0;
	if (fnEnumProcessModules &&	fnEnumProcessModules(GetCurrentProcess(), 0, 0, &cbNeeded) && cbNeeded)
	{	
		HMODULE * pmod = (HMODULE *)alloca(cbNeeded);
		DWORD cb = cbNeeded;
		if (fnEnumProcessModules(GetCurrentProcess(), pmod, cb, &cbNeeded))
		{
			for (unsigned i = 0; i < cb / sizeof (HMODULE); ++i)		// enumerate modules
			{								
				if (!this->LoadModuleFile(hProc, pmod[i]))
				{
					//	m_ref = -1;
					//	break;
				}									
			}							
		}
	}

	VERIFY(FreeLibrary(hInst));
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::GetSymbol(std::string& strSymbol)
{
	BYTE symbol[512] = {0};
	PIMAGEHLP_SYMBOL pSym = (PIMAGEHLP_SYMBOL)&symbol;

	pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) ;
    pSym->MaxNameLength = sizeof(symbol) - sizeof(IMAGEHLP_SYMBOL);

	HANDLE hProc = GetCurrentProcess();
	DWORD displacement = 0;//if need, then get this.
	int r = SymGetSymFromAddr(hProc, m_dwAddress, &displacement, pSym);
	if (!r) return false;

	strSymbol = pSym->Name;
	return true;
}

//////////////////////////////////////////////////////////////////////////
DWORD CSymEngine::GetFileLine (std::string& strFileName)
{
	IMAGEHLP_LINE img_line;
	memset(&img_line, 0, sizeof(IMAGEHLP_LINE));
	img_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

	HANDLE hProc = GetCurrentProcess();

	// "Debugging Applications" John Robbins
    // The problem is that the symbol engine finds only those source
    // line addresses (after the first lookup) that fall exactly on
    // a zero displacement. I'll walk backward 100 bytes to
    // find the line and return the proper displacement.
    DWORD dwDisplacementOutput = 0;//if need, then get this!
	int n = 0;
    while (!SymGetLineFromAddr (hProc, m_dwAddress - n, &dwDisplacementOutput, &img_line))
    {        
        if (100 == ++n)
            return false;        
    }

	strFileName = img_line.FileName;
	return img_line.LineNumber;
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::StackFirst (CONTEXT* pctx)
{
	if (!pctx || IsBadReadPtr(pctx, sizeof(CONTEXT))) 
		return false;

	if (!m_pframe) 
	{
		m_pframe = new STACKFRAME;
		if (!m_pframe) return false;
	}
				
	memset(m_pframe, 0, sizeof(STACKFRAME));

    #ifdef _X86_
    m_pframe->AddrPC.Offset       = pctx->Eip;
    m_pframe->AddrPC.Mode         = AddrModeFlat;
    m_pframe->AddrStack.Offset    = pctx->Esp;
    m_pframe->AddrStack.Mode      = AddrModeFlat;
    m_pframe->AddrFrame.Offset    = pctx->Ebp;
    m_pframe->AddrFrame.Mode      = AddrModeFlat;
    #else
    m_pframe->AddrPC.Offset       = (DWORD)pctx->Fir;
    m_pframe->AddrPC.Mode         = AddrModeFlat;
    m_pframe->AddrReturn.Offset   = (DWORD)pctx->IntRa;
    m_pframe->AddrReturn.Mode     = AddrModeFlat;
    m_pframe->AddrStack.Offset    = (DWORD)pctx->IntSp;
    m_pframe->AddrStack.Mode      = AddrModeFlat;
    m_pframe->AddrFrame.Offset    = (DWORD)pctx->IntFp;
    m_pframe->AddrFrame.Mode      = AddrModeFlat;
    #endif

	m_pctx = pctx;
	return StackNext();
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::StackNext  ()
{
	if (!m_pframe || !m_pctx) 
	{		
		return false;
	}
	
	SetLastError(0);
	HANDLE hProc = GetCurrentProcess();
	BOOL r = StackWalk (IMAGE_FILE_MACHINE_I386,
				hProc, 
				GetCurrentThread(), 
				m_pframe, 
				m_pctx,
                (PREAD_PROCESS_MEMORY_ROUTINE)My_ReadProcessMemory,
				SymFunctionTableAccess,
                SymGetModuleBase,
				0);

	if (!r || 
		!m_pframe->AddrFrame.Offset)
	{		
		return false;
	}		

	// "Debugging Applications" John Robbins
	// Before I get too carried away and start calculating
	// everything, I need to double-check that the address returned
	// by StackWalk really exists. I've seen cases in which
	// StackWalk returns TRUE but the address doesn't belong to
	// a module in the process.
	DWORD dwModBase = SymGetModuleBase (hProc, m_pframe->AddrPC.Offset);
	if (!dwModBase) 
	{	
		return false;
	}

	m_dwAddress = m_pframe->AddrPC.Offset;
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::GetModuleName (std::string& strModName)
{
	HANDLE hProc = GetCurrentProcess();
	HMODULE hMod = (HMODULE)SymGetModuleBase (hProc, m_dwAddress);
	if (!hMod) return false;

	char szFileName[MAX_PATH];
	DWORD r = GetModuleFileName(hMod, szFileName, MAX_PATH);
	if (!r) return false;
	
	szFileName[r] = 0;
	char * p = 0;
	strModName = szFileName;

	// Find the last '\' mark.
	int i = r - 1;
	for (; i >= 0; i--)
	{	
		if (szFileName[i] == '\\') 
		{
			strModName = &szFileName[i + 1]; 
			break;
		} 
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::LoadModuleFile(HANDLE hProcess, HMODULE hMod)
{	
	char filename[MAX_PATH];
	if (!GetModuleFileName(hMod, filename, MAX_PATH))
		return false;
	
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	// "Debugging Applications" John Robbins	
    // For whatever reason, SymLoadModule can return zero, but it still loads the modules. Sheez.
	SetLastError(ERROR_SUCCESS);
    if (!SymLoadModule(hProcess, hFile, filename, 0, (DWORD)hMod, 0) && 
		ERROR_SUCCESS != GetLastError())
	{
		::CloseHandle(hFile);
		return false;
	}
	::CloseHandle(hFile);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::StackTrace(char* szBuf, CONTEXT * pctx, unsigned skip)
{
	return StackTrace(szBuf, *this, pctx, skip);
}

//////////////////////////////////////////////////////////////////////////
// typedef   HANDLE  (WINAPI *OPENTHREAD)   (DWORD dwFlag, BOOL bUnknow, DWORD dwThreadId);
// OPENTHREAD   OpenThread=(OPENTHREAD)::GetProcAddress(::LoadLibrary("Kernel32.dll"),"OpenThread");
bool CSymEngine::StackTrace(char* szBuf, DWORD dwThread, unsigned skip)
{
	CONTEXT context;
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwThread);
	if (dwThread == ::GetCurrentThreadId())
    {
		return false;
    }
    else
    {
		::SuspendThread(hThread);
		::memset(&context, 0, sizeof(CONTEXT));
		context.ContextFlags = CONTEXT_FULL;
		if (::GetThreadContext(hThread, &context) == false)
		{
			::ResumeThread(hThread);
			return false;
		}
    }

	this->StackTrace(szBuf, *this, &context, skip);
	::ResumeThread(hThread);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// typedef   HANDLE  (WINAPI *RTLCAPTURECONTEXT)   (PCONTEXT context);
// RTLCAPTURECONTEXT   RtlCaptureContext=(RTLCAPTURECONTEXT)::GetProcAddress(::LoadLibrary("Kernel32.dll"),"RtlCaptureContext");
bool CSymEngine::StackTrace(char* szBuf)
{
	CONTEXT context;
	::memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&context);
	this->StackTrace(szBuf, *this, &context, 0);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CSymEngine::StackTrace(char* pszBuf, CSymEngine& sym, CONTEXT * pctx, unsigned skip)
{	
	CMySingleLock lock(&g_csSymEngineLock);
	clock_t tStart = ::clock();
	std::string strtmp = "";
	if (!sym.StackFirst(pctx)) 
		return false;
	do
	{
		if (!skip)
		{			
			std::string strModule = "?";
			sym.GetModuleName(strModule);

			std::string strFile = "?.?";
			DWORD dwLine = sym.GetFileLine(strFile);
			char szLine[20];
			sprintf(szLine, "(%u) : ", dwLine);

			std::string strSymbol = "?()";
			sym.GetSymbol(strSymbol);

			strtmp += strFile;
			strtmp += szLine;
			strtmp += strModule;
			strtmp += "-->";
			strtmp += strSymbol;
			strtmp +="()\n";
		}
		else
		{
			--skip;
		}
	}
	while (sym.StackNext());
	clock_t tEnd = ::clock();

	char szTempBuffer[60] = "";
	sprintf(szTempBuffer, "This Stack Use [%d] ms!", tEnd - tStart);
	strtmp += szTempBuffer;

	strncpy(pszBuf, strtmp.c_str(), MAX_BUFF_SIZE - 1);
	return true;
}
