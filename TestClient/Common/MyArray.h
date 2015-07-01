// MyArray.h: interface for the CMyArray class.
// paled, 2004.6.4
// �����װ�ࣺ
// ���ڷ�װC++���������ͣ����ڱ߽����ʱ���LOG��
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYARRAY_H__04538BAC_0EF7_46EB_AAF7_9201677B5D10__INCLUDED_)
#define AFX_MYARRAY_H__04538BAC_0EF7_46EB_AAF7_9201677B5D10__INCLUDED_

#pragma warning( disable : 4786 ) 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "vector"
#include "common.h"

// �����Ҫ����hint, �ڴ˶���
const int MYARRAY_HINT_MAPITEM_INFO = 1;
const int MYARRAY_HINT_VIPCONFIG	= 2;

// T: �����Ԫ������
// SIZE: ����ĳߴ�
// HINT: �����������಻ͬ���͵����飬�������������������LOGʱ�������ĸ����͵����������⡣Ҳ�ɲ��
template<class T, int SIZE, int HINT=0>
class CMyArray
{
public:
	CMyArray()	{}
	CMyArray(const T* buf, int nSize)		{ IF_OK(nSize && nSize<=SIZE) memcpy(m_set, buf, sizeof(T)*nSize); }
//��֧������	virtual ~CMyArray() {}

public: // �������������
	template<class E>
	T& operator [] (E nPos)	
	{ 
		IF_NOT (nPos >= 0 && nPos < SIZE) 
		{
			STACK_TRACE_ONCE;
			::MyLogSave2("syslog/safe_array_error", "** CMyArray() ** [1] position Error! HINT[%d]", HINT);
			return m_set[0];
		}
		return m_set[nPos]; 
	}

	template<class E>
	const T& operator [] (E nPos) const	
	{ 
		IF_NOT (nPos >= 0 && nPos < SIZE) 
		{
			STACK_TRACE_ONCE;
			::MyLogSave2("syslog/safe_array_error", "** CMyArray() ** [2] position Error! HINT[%d]", HINT);
			return m_set[0];
		}
		return m_set[nPos]; 
	}
public: // ���ݽӿ�
	operator char* ()			{ return (char*)m_set; }

protected:
	T	m_set[SIZE];
};

// ��ά����
#define	MY_ARRAY2(T,CX,CY,H)		CMyArray<CMyArray<T, CY, (H)+1>, CX, H>

// ��ά����
#define	MY_ARRAY3(T,CX,CY,CZ,H)		CMyArray<MY_ARRAY2(T, CY,CZ, (H)+1), CX, H>

#endif // !defined(AFX_MYARRAY_H__04538BAC_0EF7_46EB_AAF7_9201677B5D10__INCLUDED_)
