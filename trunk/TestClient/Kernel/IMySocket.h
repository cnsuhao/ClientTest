#ifndef I_MYSOCKET_H
#define	I_MYSOCKET_H

#ifdef MYNET_EXPORTS
#define MYNET_API __declspec(dllexport)
#else
#define MYNET_API __declspec(dllimport)
#endif

#ifndef SOCKET
	typedef unsigned int	SOCKET;
#endif

// ����������ö��
enum ENCRYPTOR_TYPE{ ENCRYPTOR_ALL = 0, ENCRYPTOR_SND, ENCRYPTOR_RCV };

// ����������Ӧ���ڶ��̻߳����������Ͷ���������Խ��ѽ����ڴ����
class IEncryptor
{
public:
	// �ͷŴ˼������ӿ�
	virtual unsigned long Release(void)		= 0;

	// ��ָ�����ȼ�������
	virtual void Encrypt	(unsigned char* buf, int nLen)	= 0;

	// ��ָ�����Ƚ�������	
	virtual void Decrypt	(unsigned char* buf, int nLen)	= 0;

	// �˷������԰�ָ�����ȵĽ����������¼��ܣ��ָ�������ǰ��״̬
	virtual void Rencrypt	(unsigned char* buf, int nLen)	= 0;

	// �ı����������
	virtual void ChangeCode	(DWORD dwCode)	= 0;
	virtual void ChangeCode	(const char* pszKey)	= 0;

	// ����һ���µļ������ӿ�
	virtual IEncryptor*	 Duplicate	(void)	= 0;
};


// �ͻ���socket�¼��ӿڣ�Ӧ�ö���Ӧ�̳в�ʵ�ִ˽ӿ�
class IClientSocketEvent
{
public:
	// ��������£��˺���Ӧ����һ��������ָ���ӽ�������������������ݴ�С��һ����Ǵ������Ϣ����С����
	// �������-1�����رմ�socket��
	virtual int		OnRcvMsg		(const char* buf, int nLen)	= 0;
	
	// socket���������ô˷�����
	virtual void	OnEstablishSck	(void)	= 0;

	// socket�����ر�ʱ����ô˷���, ע���������� Close ������á�
	virtual void	OnCloseSck		(void)	= 0;
};


// �ͻ���socket�ӿڣ�Ӧ�ö���Ӧ������ӵ�д˽ӿڣ������ڵ���Process�Բ���IClientSocketEvent���á�
class MYNET_API IClientSocket
{
public:
	virtual unsigned long Release(void)		= 0;

	// ������Ϣ
	virtual bool	SendMsg			(const void* pBuf, int nSize)	= 0;

	// �˷����������Ե��ã���������IClientSocketEvent��
	virtual void	Process			(void)			= 0;

	// �Ļ���������ԭ���ļ��������Զ��ͷš�ע�������Ӧ����new�ķ�������
	virtual void	ChgEncryptor	(ENCRYPTOR_TYPE nType, IEncryptor* pEncryptor) = 0;

	// ��ѯ������
	virtual IEncryptor*	QueryEncryptor	(ENCRYPTOR_TYPE nType) = 0;

	// �����ر�socket, �˷����������� OnCloseSck ����.
	virtual void	Close			(void)	= 0;
};


// ����socket�¼��ӿڣ�Ӧ�ö���Ӧ�̳в�ʵ�ִ˽ӿ�
class IServeSocketEvent
{
public:
	// ��������£��˺���Ӧ����һ��������ָ���ӽ�������������������ݴ�С��һ����Ǵ������Ϣ����С����
	// �������-1�����رմ�socket��
	virtual int		OnRcvMsg		(SOCKET socket, const char* buf, int nLen)	= 0;
	
	// ����(listen)��socket�½���һ����socket�����ô˺���������false��ص���socket�������ᵼ�������onCloseSck���ã�
	virtual bool	OnAcceptSck		(SOCKET socket, unsigned int ipRemote) = 0;

	// �µ�socket���������ô˷�����
	virtual void	OnEstablishSck	(SOCKET socket) = 0;

	// socket�����ر�ʱ����ô˷���, ע���������� Close ������á�
	virtual void	OnCloseSck		(SOCKET socket)	= 0;
};


// ����socket�ӿڣ�Ӧ�ö���Ӧ������ӵ�д˽ӿڣ������ڵ���Process�Բ���IServeSocketEvent���á�
class MYNET_API IServeSocket
{
public:
	virtual unsigned long Release(void)		= 0;

	// ������Ϣ��socket
	virtual bool	SendMsg			(SOCKET socket, const void* pBuf, int nSize)	= 0;

	// �����Ե��ô˷���������IServeSocketEvent��
	virtual void	Process			(void)			= 0;

	// �Ļ���������ԭ���ļ��������Զ��ͷš�ע�������Ӧ����new�ķ�������
	virtual void	ChgEncryptor	(SOCKET socket, ENCRYPTOR_TYPE nType, IEncryptor* pEncryptor) = 0;

	// ��ѯ������
	virtual IEncryptor*	QueryEncryptor	(SOCKET socket, ENCRYPTOR_TYPE nType) = 0;

	// �����ر�ָ����socket, �˷����������� OnCloseSck ����.
	// ���Ƿǳ���Ҫ֮�߼�,Ӧ�����������������ر�socket!
	virtual bool	CloseSocket		(SOCKET socket)	= 0;

	// �����Ƿ�����µ�����
	virtual void	RefuseConnect	(bool bEnable = true)	= 0;

	// ȡ��ָ��socket��ip��ַ
	virtual const char*	GetSocketIP	(SOCKET socket) = 0;

	// ȡ����socket������
	virtual int		GetSocketAmount	(void)	= 0;
};

//////////////////////////////////////////////////////////////////////
// ��̬��ʽ���ӷ�ʽ����Ҫ��LIB���ӡ�
//////////////////////////////////////////////////////////////////////

// ���ɿͻ���socket�ӿڣ�ʧ�ܷ���NULL��
// ����������Ӧ��new���Ķ����������ʧ�ܣ������IEncryptor���Զ��ͷš�
// ���������ͷ�(Release)�˽ӿ�ʱ���Զ��ͷš�
// dwReconnectInterval��socket������ʱ���������λ���룻�����Ϊ�㣬IClientSocket�ᰴָ����ʱ������ԭ���Ĳ������½���socket���ӡ����Ϊ�㣬�򲻻��Զ�������
extern MYNET_API 
IClientSocket* ClientSocketCreate (IClientSocketEvent& iSocketEvent, const char* pszIP, int nPort, IEncryptor* pEncryptorSnd = NULL, IEncryptor* pEncryptorRcv = NULL, DWORD dwReconnectInterval = 0);


// ���ɷ���socket�ӿڣ�ʧ�ܷ���NULL;
// ����������Ӧ��new���Ķ����������ʧ�ܣ������IEncryptor���Զ��ͷš�
// ���������ͷ�(Release)�˽ӿ�ʱ���Զ��ͷš�
extern MYNET_API 
IServeSocket* ServeSocketCreate(IServeSocketEvent& iSocketEvent, int nServePort, IEncryptor* pEncryptorSnd = NULL, IEncryptor* pEncryptorRcv = NULL);

//////////////////////////////////////////////////////////////////////
// ��̬��ʽ�����ӷ�ʽ������LIB���Ƽ�ʹ��
//////////////////////////////////////////////////////////////////////
namespace CSolidNetworkSystem
{	
	// ���ɿͻ���socket�ӿڣ�ʧ�ܷ���NULL��
	// ����������Ӧ��new���Ķ����������ʧ�ܣ������IEncryptor���Զ��ͷš�
	// ���������ͷ�(Release)�˽ӿ�ʱ���Զ��ͷš�
	// dwReconnectInterval��socket������ʱ���������λ���룻�����Ϊ�㣬IClientSocket�ᰴָ����ʱ������ԭ���Ĳ������½���socket���ӡ����Ϊ�㣬�򲻻��Զ�������
	static IClientSocket* ClientSocketCreate(IClientSocketEvent& iCommEvent, const char* pszIP, int nPort, IEncryptor* pEncryptorSnd = NULL, IEncryptor* pEncryptorRcv = NULL, DWORD dwReconnectInterval = 0)
	{		
		// ��̬��ʼ����
		typedef IClientSocket* (*CLIENT_COMMUNICATION_CREATE_FUNC)(IClientSocketEvent&, const char*, int, IEncryptor*, IEncryptor*, DWORD);
		static const HMODULE hSolidNetworkDLL = LoadLibrary("MyNet.DLL");

		if(hSolidNetworkDLL == NULL)
		{
			int f = 0;
		}

		static const CLIENT_COMMUNICATION_CREATE_FUNC funcClientCommCreate = (CLIENT_COMMUNICATION_CREATE_FUNC)GetProcAddress(hSolidNetworkDLL, "ClientSocketCreate");
		
		if(funcClientCommCreate)
		{	
			return funcClientCommCreate(iCommEvent, pszIP, nPort, pEncryptorSnd, pEncryptorRcv, dwReconnectInterval);
		}
		else return NULL;
	}
	
	// ���ɷ���socket�ӿڣ�ʧ�ܷ���NULL;
	// ����������Ӧ��new���Ķ����������ʧ�ܣ������IEncryptor���Զ��ͷš�
	// ���������ͷ�(Release)�˽ӿ�ʱ���Զ��ͷš�
	static IServeSocket* ServeSocketCreate(IServeSocketEvent& iCommEvent, int nServePort, IEncryptor* pEncryptorSnd = NULL, IEncryptor* pEncryptorRcv = NULL)
	{		
		// ��̬��ʼ����
		typedef IServeSocket* (*SERVE_COMMUNICATION_CREATE_FUNC)(IServeSocketEvent&, int, IEncryptor*, IEncryptor*);
		static const HMODULE hSolidNetworkDLL = LoadLibrary("MyNet.DLL");
		static const SERVE_COMMUNICATION_CREATE_FUNC funcServeCommCreate = (SERVE_COMMUNICATION_CREATE_FUNC)GetProcAddress(hSolidNetworkDLL, "ServeSocketCreate");
		
		if(funcServeCommCreate)
		{	
			return funcServeCommCreate(iCommEvent, nServePort, pEncryptorSnd, pEncryptorRcv);
		}
		else return NULL;
	}
}

#endif