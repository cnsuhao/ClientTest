// 通用工具
// 仙剑修，2002.9.14
//////////////////////////////////////////////////////////////////////

#ifndef	COMMON_TYPEDEF_HEADER
#define	COMMON_TYPEDEF_HEADER

#include "common.h"
#pragma warning(disable:4786)
#include <string>
using namespace std;

/*/////////////////////////////////////////////////////////////////////
template<TYPENAME T, int SZ, int D=0>
struct Buffer{
	Buffer()					{ memset(m_buf, D, sizeof(T)*SZ); }
	T& operator[](int idx)		{ IF_OK(idx>=0 && idx<SZ); return m_buf[idx]; return m_buf[SZ]; }
protected:
	T		m_buf[SZ+1];
};*/

//////////////////////////////////////////////////////////////////////
struct true_type {};
struct false_type {};

//////////////////////////////////////////////////////////////////////
class MyString
{
public:
	MyString()												{ m_string = string(); }
	MyString(const char* str)									{ IF_OK(str) m_string = str; }
	MyString& operator=(const char* str)						{ IF_OK(str) m_string = str; else m_string = ""; return *this; }
	operator const char*() const							{ return m_string.c_str(); }
protected:
	MyString(string& s)										{ m_string = s; }

public:
	string&	QueryString()									{ return m_string; }
	bool	operator<(const MyString& rhs) const				{ return m_string < rhs.m_string; }
	bool	operator==(const MyString& rhs) const				{ return m_string == rhs.m_string; }
	MyString&	operator+=(const MyString& rhs)					{ m_string += rhs.m_string; return *this; }
	MyString&	operator+=(char ch) 							{ m_string += ch; return *this; }
	MyString	operator+(const MyString& rhs) const				{ return m_string + rhs.m_string; }
	const MyString&	FormatLong(long nNum)					{ char buf[]="-2147483648"; m_string = ltoa(nNum, buf, 10); return *this; }
	const MyString&	FormatDword(DWORD nNum)					{ char buf[]="-2147483648"; m_string = ultoa(nNum, buf, 10); return *this; }
	long	ConvertLong()									{ return atol(m_string.c_str()); }
	DWORD	ConvertDword()									{ return atol(m_string.c_str()); }

protected:
	string		m_string;
};

//////////////////////////////////////////////////////////////////////
template<TYPENAME T>
class POINT_NO_RELEASE : public T
{
	virtual void	Release()			= 0;		// prevent call this function
	virtual void	RleaseByOwner()		= 0;		// prevent call this function
};

//////////////////////////////////////////////////////////////////////
template<int I>
struct auto_long_
{
	auto_long_() : hide_data(I) {}
	auto_long_<I>& operator =(long n)	{ hide_data=n; return *this; }
	operator long() const	{ return hide_data; }

	long	hide_data;
};

typedef	auto_long_<0>	auto_long;

//////////////////////////////////////////////////////////////////////
template<TYPENAME T>
class hide_type
{
	T	hide_data;
public:
	T&	HideData()		{ return hide_data; }
};


#endif // COMMON_TYPEDEF_HEADER