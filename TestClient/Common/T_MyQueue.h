// 队列
// 仙剑修，2003.5.24
//////////////////////////////////////////////////////////////////////
// 说明：pop不成功时返回0。
//////////////////////////////////////////////////////////////////////


#ifndef	MYQUEUE_H
#define MYQUEUE_H

#include "../MsgServer/share/define.h"

#pragma warning(disable:4786)
#include <deque>
using namespace std;

template	< TYPENAME T >
class CMyQueue
{
public:
	void	push(T obj)					{ m_set.push_back(obj); }
	T		pop()						{ if(size()) { T obj = *m_set.begin(); m_set.pop_front(); return obj; } return 0; }
	void	push_front(T obj)			{ m_set.push_front(obj); }
	T*		getFirst();					
	bool	isExists(T& idType);
	void	Erase(UINT index);
	T*		GetObjByIndex(UINT index);
	int		size()						{ return m_set.size(); }
	void	clear()						{ m_set.clear(); }
//	deque<T>&	QueryList()				{ return m_set; }
protected:
	typedef	deque<T>		QUEUE_SET;
	QUEUE_SET		m_set;
};

//////////////////////////////////////////////////////////////////////////
template	< TYPENAME T >
T* CMyQueue<T>::getFirst()
{
	if(size())
	{
		T& obj = *m_set.begin();
		return &obj;
	}
	else
	{
		return NULL;
	}
}

template <TYPENAME T>
bool CMyQueue<T>::isExists(T& idType)		//用于简单数据类型
{
	for (int i = 0; i < m_set.size(); ++i)
	{
		if (m_set[i] == idType)
		{
			return true;
		}
	}
	return false;
}

template <TYPENAME T>
void CMyQueue<T>::Erase(UINT index)
{
	CHECK(index < size());
	m_set.erase(m_set.begin() + index);
}

template<TYPENAME T>
T* CMyQueue<T>::GetObjByIndex(UINT index)
{
	CHECKF(index < size());
	T& obj = m_set[index];
	return &obj;
}


#endif // MYQUEUE_H
