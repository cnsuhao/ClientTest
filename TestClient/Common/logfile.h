// ��ϳ���
// �ɽ���, 2001.10.10

#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

#ifndef	LOGFILE_H
#define LOGFILE_H
#pragma	warning(disable:4786)

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "windows.h"
#include "time.h"
#include <deque>
#include <string> 
#include "./ThreadBase.h"

#define		LOGFUNC_TRY			try{
#define		LOGFUNC_CATCH(x)	}catch(...){ OutputDebugString("GameServer Log Crash! [" x "]"); }

const INT MAX_ASYNC_LOG_AMOUNT	= 10000;	// ���������첽log, �������ֵʱ�������, clearʱ�������ֵ�Ĳ������浽�ļ�
const int MAX_LOGFILESIZE = 20*1024*1024;	// ��־ÿ20M��һ���ļ�
extern BOOL g_bLogSave;						// ��־���ر�־λ
extern BOOL g_bLogWalk;						// ��·��־����
extern BOOL g_bLogDatabaseSql;				// sql��־����
const char SZ_UTF8_FILE_HEADER[3] = {(char)0xEF, (char)0xBB, (char)0xBF};	// utf8�ļ�ͷ

#define		LOGFILE_DIR				"syslog"
#define		LOGFILE_FILENAME		"syslog/sys"
#define		VALUABLESLOG_DIR		"valuableslog"
#define		VALUABLESLOG_FILENAME	"valuableslog/valueables"
#define		GMLOG_DIR				"gmlog"

// ��־�߳�
typedef std::pair<std::string, std::string> PAIR_LOGINFO;
class CLogThread : protected CThreadBase
{
	// �̳�
protected:
	virtual	void	OnInit()	{}
	virtual bool	OnProcess()	;	// �첽�߳�������
	virtual void	OnDestroy() {}

	// �߼�����
public:
	void			LogPrint		(const char* pszLogFile, const char* pszLogInfo);	// ���Ⱪ¶�ӿ�
	bool			StartLogServe	(void);												// ������־�̷߳���
	bool			CloseLogServe	(void);												// �ر���־�̷߳���
private:
	PAIR_LOGINFO	TakeAsyncLog	(void);												// [�̰߳�ȫ]��ȡ�첽��־
	void			PushAsyncLog	(const char* pszLogFile, const char* pszLogInfo);	// [�̰߳�ȫ]ѹ���첽��־
	void			ClearAsynLog	(void);												// ��ն����е���־
	void			ProcessOneLog	(void);												// ����һ����־
	
	// �����
private:
	CSimpleCriticalSection		m_csAsync;			// �첽�߳��ٽ���
	std::deque<PAIR_LOGINFO>	m_deqAsyncLog;		// �첽�߳���־����
	int							m_nAsyncLogAmount;	// �첽�߳���־����
	bool						m_bOpenServe;		// �Ƿ����̷߳���

	// ����
public:
	static CLogThread*	GetInstance(void);
	static void			DelInstance(void);
private:
	CLogThread();
	virtual ~CLogThread();
	static CLogThread*	s_pInstance;
};

// ʹ��ǰ��Ҫ���� InitLog()
void InitLog(const char * pTitle, time_t tStart);

#define LOGWARNING		::LogSave
#define LOGMSG			::LogSave
#define LOGERROR		::LogSave
#define LOGCHEAT		::LogSave


void	MakeLogFileHeader	(const char* pszFileName);
void	GetLogOutputBuffer	(OUT char* pszOutputBuffer, IN const char* pszBuffer);
void	LogSaveBase			(const char* pszLogFileName, const char* pszLogInfo);
void	LogSave				(const char* fmt, ...);
void	LogSave2			(const char* pszSrcFileName, UINT unSrcFileLine, const char* fmt, ...);
void	MyLogSave			(const char* pszName, const char* fmt, ...);
void	MyLogSave2			(const char* pszName, const char* fmt, ...);
void	Trace				(const char* fmt, ...);
void	GmLogSave			(const char* fmt, ...);
void	LogSaveTalk			(const char* fmt, ...);
void	LogWalk				(const char* fmt, ...);
void	LogDatabaseSql		(const char* fmt, ...);
void	LogMoney			(const char* fmt, ...);
void	LogStack			(const char* pszSrcFileName, UINT unSrcFileLine, const char* fmt, ...);
void	LogTimeOut			(const char* fmt, ...);
void	LogLuaError			(const char* fmt, ...);
void	LogShopBuyItem		(const char* fmt, ...);
void	LogMemMonitor		(const char* fmt, ...);
void	LogDirect				(const char* pszFileName, const char* pszInfo);


#define tolog(a)  LogSave2(__FILE__, __LINE__,a);
#define tolog2(a,b)  LogSave2(__FILE__, __LINE__,a,b);
#define tolog3(a,b,c)  LogSave2(__FILE__, __LINE__,a,b,c);
#define tolog4(a,b,c,d)  LogSave2(__FILE__, __LINE__,a,b,c,d);
#define tolog5(a,b,c,d,e)  LogSave2(__FILE__, __LINE__,a,b,c,d,e);
#define tolog6(a,b,c,d,e,f)	 LogSave2(__FILE__, __LINE__,a,b,c,d,e,f);
#define tolog7(a,b,c,d,e,f,g)  LogSave2(__FILE__, __LINE__,a,b,c,d,e,f,g);
#define tolog8(a,b,c,d,e,f,g,h)  LogSave2(__FILE__, __LINE__,a,b,c,d,e,f,g,h);

#define iflog(a,b,c) if(a)LogSave2(__FILE__, __LINE__,b,c);
#define iflog2(a,b,c,d) if(a)LogSave2(__FILE__, __LINE__,b,c,d);
#define iflog3(a,b,c,d,e) if(a)LogSave2(__FILE__, __LINE__,b,c,d,e);
#define iflog4(a,b,c,d,e,f) if(a)LogSave2(__FILE__, __LINE__,b,c,d,e,f);
#define iflog5(a,b,c,d,e,f,g) if(a)LogSave2(__FILE__, __LINE__,b,c,d,e,f,g);

#define ifcheck(a,b,c) if(a){LogSave2(__FILE__, __LINE__,b,c);return;}
#define ifcheck2(a,b,c,d) if(a){LogSave2(__FILE__, __LINE__,b,c,d);return;}
#define ifcheck3(a,b,c,d,e) if(a){LogSave2(__FILE__, __LINE__,b,c,d,e);return;}
#define ifcheck4(a,b,c,d,e,f) if(a){LogSave2(__FILE__, __LINE__,b,c,d,e,f);return;}
#define ifcheck5(a,b,c,d,e,f,g) if(a){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g);return;}

#define ifcheckf(a,b,c) if(a){LogSave2(__FILE__, __LINE__,b,c);return 0;}
#define ifcheckf2(a,b,c,d) if(a){LogSave2(__FILE__, __LINE__,b,c,d);return 0;}
#define ifcheckf3(a,b,c,d,e) if(a){LogSave2(__FILE__, __LINE__,b,c,d,e);return 0;}
#define ifcheckf4(a,b,c,d,e,f) if(a){LogSave2(__FILE__, __LINE__,b,c,d,e,f);return 0;}
#define ifcheckf5(a,b,c,d,e,f,g) if(a){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g);return 0;}


#define ifchkfail(a,b,c) if(!(a)){LogSave2(__FILE__, __LINE__,b,c);return;}
#define ifchkfail2(a,b,c,d) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d);return;}
#define ifchkfail3(a,b,c,d,e) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e);return;}
#define ifchkfail4(a,b,c,d,e,f) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f);return;}
#define ifchkfail5(a,b,c,d,e,f,g) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g);return;}
#define ifchkfail6(a,b,c,d,e,f,g,h) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g,h);return;}
#define ifchkfail7(a,b,c,d,e,f,g,h,i) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g,h,i);return;}

#define ifchkfailf(a,b,c) if(!(a)){LogSave2(__FILE__, __LINE__,b,c);return 0;}
#define ifchkfailf2(a,b,c,d) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d);return 0;}
#define ifchkfailf3(a,b,c,d,e) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e);return 0;}
#define ifchkfailf4(a,b,c,d,e,f) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f);return 0;}
#define ifchkfailf5(a,b,c,d,e,f,g) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g);return 0;}
#define ifchkfailf6(a,b,c,d,e,f,g,h) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g,h);return 0;}
#define ifchkfailf7(a,b,c,d,e,f,g,h,i) if(!(a)){LogSave2(__FILE__, __LINE__,b,c,d,e,f,g,h,i);return 0;}


#endif	// LOGFILE_H
