#include "ClientThread.h"
#include "ClientKernel.h"

void CClientThread::OnInit()
{
	pClientKernel->Init();	
}

bool CClientThread::OnProcess()
{
	pClientKernel->ThreadOnProcess();
	Sleep(1);	// �л�ʱ��Ƭ
	return true;
}

void CClientThread::OnDestroy()
{
	pClientKernel->Release();
	CClientKernel::DelInstance();
}