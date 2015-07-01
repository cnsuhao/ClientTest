////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: T_SafeVector.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2012-10-23
////////////////////////////////////////////////////////////////////////
#ifndef	_COMMON_SAFE_VECTOR_H_
#define _COMMON_SAFE_VECTOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <vector>
#include "common.h"

// 如果需要定义hint, 在此定义
const int SAFEVECTOR_HINT_OBJID				= 1;
const int SAFEVECTOR_HINT_INT				= 2;

const int SAFEVECTOR_HINT_USER_POINT		= 100;
const int SAFEVECTOR_HINT_SESSION			= 101;
const int SAFEVECTOR_HINT_PROCESS_ID		= 102;
const int SAFEVECTOR_HINT_NET_BREAK_FLAG	= 103;
const int SAFEVECTOR_HINT_MAILLIST			= 104;

template <class T, int HINT=0>
class TSafeVector : public std::vector<T>
{
	//-----------------------------------------------
public: 
	// 重载vector操作符 防止越界访问, 如果越界访问会默认返回一个静态成员, 不至于崩溃
	template<class E>
	T& operator [] (E position)					
	{ 
		IF_NOT (position>=0 && position<this->size())
		{
			STACK_TRACE_ONCE;
			::MyLogSave2("syslog/safe_vertor_error", "** CSafeVector() ** [1] position Error! HINT[%d]", HINT);
			static T stErrorReturn;
			return stErrorReturn;
		}
		return this->at(position);

	}
	template<class E>
	const T& operator [] (E position) const		
	{ 
		IF_NOT(position>=0 && position<this->size()) 
		{
			STACK_TRACE_ONCE;
			::MyLogSave2("syslog/safe_vertor_error", "** CSafeVector() ** [2] position Error! HINT[%d]", HINT);
			static T stErrorReturn;
			return stErrorReturn;
		}
		return this->at(position);
	}
};

typedef TSafeVector<OBJID, SAFEVECTOR_HINT_OBJID>	ID_VEC;
typedef ID_VEC::iterator							ID_VEC_ITER;
typedef ID_VEC::const_iterator						ID_VEC_CITER;
typedef TSafeVector<int, SAFEVECTOR_HINT_INT>		INT_VEC;
typedef INT_VEC::iterator							INT_VEC_ITER;
typedef INT_VEC::const_iterator						INT_VEC_CITER;

#endif // end of _COMMON_SAFE_VECTOR_H_