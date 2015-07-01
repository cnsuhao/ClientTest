////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: MxyString.cpp
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2013-7-17
////////////////////////////////////////////////////////////////////////
#include "MxyString.h"
#include "PerformanceStatistics.h"
#include "common.h"
#include "MyArray.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMxyString::CMxyString()
{

}

//////////////////////////////////////////////////////////////////////
CMxyString::CMxyString(const char* pszFmt, 
				 FMT_ARG d0, FMT_ARG d1, FMT_ARG d2, FMT_ARG d3, 
				 FMT_ARG d4, FMT_ARG d5, FMT_ARG d6, FMT_ARG d7, 
				 FMT_ARG d8, FMT_ARG d9, FMT_ARG da, FMT_ARG db, 
				 FMT_ARG dc, FMT_ARG dd, FMT_ARG de, FMT_ARG df,
				 FMT_ARG dg, FMT_ARG dh, FMT_ARG di, FMT_ARG dj)
{
	this->Format(pszFmt, d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df, dg, dh, di, dj);
}

//////////////////////////////////////////////////////////////////////
CMxyString::~CMxyString()
{

}

//////////////////////////////////////////////////////////////////////
// void CString::Format(const char* fmt, ...)
// {
// 	if (!fmt)
// 		return;
// 
// 	try {
// 		char buffer[MAX_STRING] = "";
// 		vsprintf( buffer, fmt, (char*) ((&fmt)+1) );
// 		this->assign(buffer);
// 	}catch(...)
// 	{
// 		//LogSave("Error: too big size of string in format.");
// 	}
// }

//////////////////////////////////////////////////////////////////////
void 
CMxyString::Format(const char* pszFmt, 
				FMT_ARG d0, FMT_ARG d1, FMT_ARG d2, FMT_ARG d3, 
				FMT_ARG d4, FMT_ARG d5, FMT_ARG d6, FMT_ARG d7, 
				FMT_ARG d8, FMT_ARG d9, FMT_ARG da, FMT_ARG db, 
				FMT_ARG dc, FMT_ARG dd, FMT_ARG de, FMT_ARG df,
				FMT_ARG dg, FMT_ARG dh, FMT_ARG di, FMT_ARG dj)
{
	int pos = 0;
	DEBUG_TRY 
	{
		IF_NOT (pszFmt)
		{
			STACK_TRACE_ONCE;
			return;
		}

		IF_NOT (strlen(pszFmt) < MAX_MXY_STRING)
		{
			return;
		}

		const int ARG_NUM = 20;
		FMT_ARG* buf[ARG_NUM] = {&d0, &d1, &d2, &d3, &d4, &d5, &d6, &d7, &d8, &d9, &da, &db, &dc, &dd, &de, &df, &dg, &dh, &di, &dj};
		const CMyArray<FMT_ARG*, ARG_NUM> arg(buf, ARG_NUM);
		this->clear();

		int nArgPtr = 0;
		for (; pszFmt[pos] != 0; ++pos)
		{
			if (nArgPtr >= ARG_NUM || !arg[nArgPtr]->str)
			{
				this->push_back(pszFmt[pos]);
				continue;
			}
			
			if (pszFmt[pos] != '%')
			{
				this->push_back(pszFmt[pos]);
				continue;
			}
			
			++pos;
			switch (pszFmt[pos])
			{
			case '%':
				{
					this->push_back('%');
					continue;
				}
				break;
				
			case 'd':
			case 'u':
			case 's':
			case 'f':
				{
					this->append(arg[nArgPtr]->str);
					++nArgPtr;
					continue;
				}
				break;
				
			case 'I':
				if (pszFmt[pos+1] == '6' && pszFmt[pos+2] == '4' && (pszFmt[pos+3] == 'd' || pszFmt[pos+3] == 'u'))
				{
					this->append(arg[nArgPtr]->str);
					++nArgPtr;
					pos += 3;
					continue;
				}
				break;
				
			case '.':
				{
					int num = pszFmt[pos+1] - '0';
					if(num >= 0 && num <= 9 && pszFmt[pos+2] == 'f')
					{
						char* pszEnd = (char*)arg[nArgPtr]->str;
						
						for(; *pszEnd && *pszEnd != '.'; ++pszEnd){}
						if (*pszEnd == '.')
						{
							for(++pszEnd; *pszEnd && num; --num,++pszEnd){}
							*pszEnd = 0;
						}
						this->append(arg[nArgPtr]->str);
						++nArgPtr;
						pos += 2;
						continue;
					}
				}
				break;
				
			default:
				break;
			}
			this->push_back('%');
			this->push_back(pszFmt[pos]);
		}
		
		if (this->size() > MAX_MXY_STRING)
		{
			::LogSave("CString size > 4096 !!");
		}
	}
	DEBUG_CATCH3("Exception: Error: CString::Format fail! pszFmt:%s, pos:%d", pszFmt, pos);
}

//////////////////////////////////////////////////////////////////////
int 
CMxyString::Scanf(SPT_ARG d0, SPT_ARG d1, SPT_ARG d2, SPT_ARG d3, 
			   SPT_ARG d4, SPT_ARG d5, SPT_ARG d6, SPT_ARG d7, 
			   SPT_ARG d8, SPT_ARG d9, SPT_ARG da, SPT_ARG db, 
			   SPT_ARG dc, SPT_ARG dd, SPT_ARG de, SPT_ARG df,
			   SPT_ARG dg, SPT_ARG dh, SPT_ARG di, SPT_ARG dj)
{
	int nRet = 0;
	DEBUG_TRY 
	const char DELIMITER = ' ';
	const int ARG_NUM = 20;
	SPT_ARG* buf[ARG_NUM] = {&d0, &d1, &d2, &d3, &d4, &d5, &d6, &d7, &d8, &d9, &da, &db, &dc, &dd, &de, &df, &dg, &dh, &di, &dj};
	const CMyArray<SPT_ARG*, ARG_NUM> arg(buf, ARG_NUM);

	int nArgPtr = 0;
	
	const char* pszStart = this->c_str();
	while (*pszStart && arg[nArgPtr]->ptr && nArgPtr < ARG_NUM)
	{
		for(; *pszStart == DELIMITER; ++pszStart)
		{
			if (!*pszStart)
			{
				return nRet;
			}
		}
		
		char* pszEnd = (char*)pszStart;
		for(; *pszEnd && *pszEnd != DELIMITER; ++pszEnd){}
		
		char cSave = *pszEnd;
		*pszEnd = 0;
		
		if (arg[nArgPtr]->trans_type(pszStart))
		{
			++nRet;
		}
		else
		{
			*pszEnd = cSave;
			::LogSave("CString::Scanf error string:%s arg:%d", this->c_str(), nArgPtr);
		}
		++nArgPtr;
		
		*pszEnd = cSave;
		pszStart = pszEnd;
	}
	DEBUG_CATCH("Exception: Error: CString::Scanf fail!");
	return nRet;
}

//////////////////////////////////////////////////////////////////////
void 
CMxyString::ToUpper(void)
{
	for (CMxyString::iterator iter = this->begin(); iter != this->end(); iter++)
	{
		(*iter) = ::toupper(*iter);
	}
}

//////////////////////////////////////////////////////////////////////
void 
CMxyString::ToLower(void)
{
	for (CMxyString::iterator iter = this->begin(); iter != this->end(); iter++)
	{
		(*iter) = ::tolower(*iter);
	}
}

//////////////////////////////////////////////////////////////////////
bool
CMxyString::Replace(const char* pszStr, const char* pszNewStr, bool bReplaceAll)
{
	IF_NOT (pszStr && ::strlen(pszStr) > 0)
	{
		return false;
	}

	if (this->empty())
	{
		return false;
	}

	if (!pszNewStr)
	{
		pszNewStr = "";
	}

	std::string::size_type unOldSize = ::strlen(pszStr);
	std::string::size_type unNewSize = ::strlen(pszNewStr);

	std::string::size_type unPos = 0;
	while ((unPos = this->find(pszStr, unPos)) != std::string::npos)
	{
		this->replace(unPos, unOldSize, pszNewStr);
		unPos += unNewSize;
		if (!bReplaceAll)
		{
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool 
SPT_ARG::to_cstr(const char* s)	
{
	*(CMxyString*)ptr = s;
	return true;
}

