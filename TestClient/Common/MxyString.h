////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2013, WuHan ChuangYou, All Rights Reserved
// Moudle: MxyString.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2013-7-17
////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_MXY_STRING_H_
#define _COMMON_MXY_STRING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4996)		// 微软提示要用自家的安全_s函数
#pragma warning(disable:4018)		// 有无符号
#pragma warning(disable:4244)		// 精度损失

#include <string>
#include <stdio.h>

const int MAX_FMT_ARG_NUMBER_LEN = 24;
class CMxyString;

enum ARG_TYPE {NO_ARG = 0};
enum { MAX_MXY_STRING = 4096 };

struct FMT_ARG
{
	char buf[MAX_FMT_ARG_NUMBER_LEN];
	const char* str;
	
	FMT_ARG(ARG_TYPE t) {str = NULL;}
	
	FMT_ARG(char num)            {_itoa(num, buf, 10); str = buf;}
	FMT_ARG(unsigned char num)   {_itoa(num, buf, 10); str = buf;}
	FMT_ARG(int num)             {_itoa(num, buf, 10); str = buf;}
	FMT_ARG(unsigned int num)    {_ultoa(num, buf, 10); str = buf;}
	FMT_ARG(short num)           {_itoa(num, buf, 10); str = buf;}
	FMT_ARG(unsigned short num)  {_itoa(num, buf, 10); str = buf;}
	FMT_ARG(__int64 num)         {_i64toa(num, buf, 10); str = buf;}
	FMT_ARG(unsigned __int64 num){_ui64toa(num, buf, 10); str = buf;}
	FMT_ARG(float f)             {_snprintf(buf, 10, "%f", f); str = buf;}
	FMT_ARG(double f)            {_snprintf(buf, 20, "%f", f); str = buf;}
	FMT_ARG(std::string& s)      {str = s.c_str();}
	FMT_ARG(const char* sz)      {str = sz; if(sz && strlen(sz) > MAX_MXY_STRING){str = NULL;} }
	FMT_ARG(long num)            {_ltoa(num, buf, 10); str = buf;}
	FMT_ARG(unsigned long num)   {_ultoa(num, buf, 10); str = buf;}
};

struct SPT_ARG
{
	typedef bool (SPT_ARG::*TRANS_FUNC)(const char*);

	TRANS_FUNC trans;
	void* ptr;
	
	SPT_ARG(ARG_TYPE b)				{ptr = NULL;}
	SPT_ARG(char& c)				{ptr = &c;		trans = &SPT_ARG::to_char;}
	SPT_ARG(unsigned char& uc)		{ptr = &uc;		trans = &SPT_ARG::to_uchar;}
	SPT_ARG(int& i)					{ptr = &i;		trans = &SPT_ARG::to_int;}
	SPT_ARG(unsigned int& ui)		{ptr = &ui;		trans = &SPT_ARG::to_uint;}
	SPT_ARG(short& si)				{ptr = &si;		trans = &SPT_ARG::to_short;}
	SPT_ARG(unsigned short& usi)	{ptr = &usi;	trans = &SPT_ARG::to_ushort;}
	SPT_ARG(__int64& i64)			{ptr = &i64;	trans = &SPT_ARG::to_int64;}
	SPT_ARG(float& f)				{ptr = &f;		trans = &SPT_ARG::to_float;}
	SPT_ARG(double& d)				{ptr = &d;		trans = &SPT_ARG::to_double;}
	SPT_ARG(std::string& s)			{ptr = &s;		trans = &SPT_ARG::to_str;}
	SPT_ARG(CMxyString& s)			{ptr = &s;		trans = &SPT_ARG::to_cstr;}
	SPT_ARG(long& i)				{ptr = &i;		trans = &SPT_ARG::to_int;}
	SPT_ARG(unsigned long& ui)		{ptr = &ui;		trans =&SPT_ARG:: to_uint;}

	bool to_char(const char* s)		{__int64 n = _atoi64(s); *(char*)ptr = n;			return n == *(char*)ptr;}
	bool to_uchar(const char* s)	{__int64 n = _atoi64(s); *(unsigned char*)ptr = n;	return n == *(unsigned char*)ptr;}
	bool to_int(const char* s)		{__int64 n = _atoi64(s); *(int*)ptr = n;			return n == *(int*)ptr;}
	bool to_uint(const char* s)		{__int64 n = _atoi64(s); *(unsigned int*)ptr = n;	return n == *(unsigned int*)ptr;}
	bool to_short(const char* s)	{__int64 n = _atoi64(s); *(short*)ptr = n;			return n == *(short*)ptr;}
	bool to_ushort(const char* s)	{__int64 n = _atoi64(s); *(unsigned short*)ptr = n; return n == *(unsigned short*)ptr;}
	bool to_float(const char* s)	{*(float*)ptr = atof(s);		return true;}
	bool to_double(const char* s)	{*(double*)ptr = atof(s);		return true;}
	bool to_int64(const char* s)	{*(__int64*)ptr = _atoi64(s);	return true;}
	bool to_str(const char* s)		{*(std::string*)ptr = s;		return true;}
	bool to_cstr(const char* s);
	
	bool trans_type(const char* s)
	{
		return (this->*trans)(s);
	}
};

// 这里要求后续使用CString格式化后的字符串长度不能超过4096-1
class CMxyString : public std::string  
{
public:
	CMxyString();
	CMxyString(const std::string& str)				{ this->assign(str.c_str()); }
	CMxyString(const CMxyString& str)				{ this->assign(str.c_str()); }

	//CString(const char* pstr)					{ if (pstr) this->assign(pstr); }
	CMxyString(const char* pszFmt, 
			FMT_ARG d0 = NO_ARG, FMT_ARG d1 = NO_ARG, FMT_ARG d2 = NO_ARG, FMT_ARG d3 = NO_ARG, 
			FMT_ARG d4 = NO_ARG, FMT_ARG d5 = NO_ARG, FMT_ARG d6 = NO_ARG, FMT_ARG d7 = NO_ARG, 
			FMT_ARG d8 = NO_ARG, FMT_ARG d9 = NO_ARG, FMT_ARG da = NO_ARG, FMT_ARG db = NO_ARG, 
			FMT_ARG dc = NO_ARG, FMT_ARG dd = NO_ARG, FMT_ARG de = NO_ARG, FMT_ARG df = NO_ARG,
			FMT_ARG dg = NO_ARG, FMT_ARG dh = NO_ARG, FMT_ARG di = NO_ARG, FMT_ARG dj = NO_ARG);
	virtual ~CMxyString();
	
	CMxyString& operator = (const std::string& str)	{ this->assign(str.c_str()); return *this; }
	CMxyString& operator = (const CMxyString& str)	{ this->assign(str.c_str()); return *this; }
	CMxyString& operator = (const char* pstr)		{ if (pstr) this->assign(pstr); return *this; }

	
	bool operator == (const std::string& str)	{ return (0 == this->compare(str)); }
	bool operator != (const std::string& str)	{ return (0 != this->compare(str)); }
	
	bool operator == (const char* pstr)			
	{ 
		if (pstr) 
			return (0 == this->compare(pstr)); 
		else
			return false;
	}
	bool operator != (const char* pstr)			
	{ 
		return !(operator == (pstr));
	}
	
	operator const char*() const				{ return this->c_str(); }
	
	//void Format(const char* fmt, ...);
	
public:
	void Format(const char* pszFmt, 
		FMT_ARG d0 = NO_ARG, FMT_ARG d1 = NO_ARG, FMT_ARG d2 = NO_ARG, FMT_ARG d3 = NO_ARG, 
		FMT_ARG d4 = NO_ARG, FMT_ARG d5 = NO_ARG, FMT_ARG d6 = NO_ARG, FMT_ARG d7 = NO_ARG, 
		FMT_ARG d8 = NO_ARG, FMT_ARG d9 = NO_ARG, FMT_ARG da = NO_ARG, FMT_ARG db = NO_ARG, 
		FMT_ARG dc = NO_ARG, FMT_ARG dd = NO_ARG, FMT_ARG de = NO_ARG, FMT_ARG df = NO_ARG,
		FMT_ARG dg = NO_ARG, FMT_ARG dh = NO_ARG, FMT_ARG di = NO_ARG, FMT_ARG dj = NO_ARG);

	int Scanf(SPT_ARG d0 = NO_ARG, SPT_ARG d1 = NO_ARG, SPT_ARG d2 = NO_ARG, SPT_ARG d3 = NO_ARG, 
			  SPT_ARG d4 = NO_ARG, SPT_ARG d5 = NO_ARG, SPT_ARG d6 = NO_ARG, SPT_ARG d7 = NO_ARG, 
			  SPT_ARG d8 = NO_ARG, SPT_ARG d9 = NO_ARG, SPT_ARG da = NO_ARG, SPT_ARG db = NO_ARG, 
			  SPT_ARG dc = NO_ARG, SPT_ARG dd = NO_ARG, SPT_ARG de = NO_ARG, SPT_ARG df = NO_ARG,
			  SPT_ARG dg = NO_ARG, SPT_ARG dh = NO_ARG, SPT_ARG di = NO_ARG, SPT_ARG dj = NO_ARG);

	void ToUpper(void);
	void ToLower(void);
	bool Replace(const char* pszStr, const char* pszNewStr, bool bReplaceAll = false);
};


#endif // _COMMON_MXY_STRING_H_
