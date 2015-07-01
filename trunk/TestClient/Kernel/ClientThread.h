#ifndef _CLIENT_THREAD_H_
#define _CLIENT_THREAD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./../Include/Include.h"

class CClientKernel;
class CClientThread : public CThreadBase
{
public:
	CClientThread() {};
	virtual ~CClientThread() {};

	// 继承
protected:
	virtual	void	OnInit();
	virtual bool	OnProcess()	;	// 线程主函数
	virtual void	OnDestroy();

};

#endif // _CLIENT_THREAD_H_