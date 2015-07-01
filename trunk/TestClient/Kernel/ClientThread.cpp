#include "ClientThread.h"
#include "ClientKernel.h"

void CClientThread::OnInit()
{
	pClientKernel->Init();	
}

bool CClientThread::OnProcess()
{
	pClientKernel->ThreadOnProcess();
	Sleep(1);	// ÇÐ»»Ê±¼äÆ¬
	return true;
}

void CClientThread::OnDestroy()
{
	pClientKernel->Release();
	CClientKernel::DelInstance();
}