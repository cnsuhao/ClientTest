#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

#ifndef _BASEFUNC_H
#define _BASEFUNC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	warning(disable:4786)

#pragma warning(disable:4996)		// ΢����ʾҪ���Լҵİ�ȫ_s����
#pragma warning(disable:4018)		// ���޷���
#pragma warning(disable:4244)		// ������ʧ

#include <time.h>
#include <math.h>
#include "common.h"
#include "BaseType.h"
#include "logfile.h"
#include "AutoPtr.h"
#include <vector>
#include "./MxyString.h"

using namespace std;


UINT	GetNextUpdateTimeOfWeek(UINT unTimeOfDay);
void	DateTime(char * buf20, time_t tInput = 0);		// 0 : curr time
void	TimeFormatFormUtcSecond(char* pszBuf9, unsigned int unUtcSecond = 0);
bool	FormatDateTime(char* buf, const char* szFormat, time_t tInput = 0);		// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
void	DateTimeStamp(char * buf15, time_t tInput = 0);		// 0 : curr time
int		DateStamp(time_t tTime=0);
int		DateStampPass(int nDate, int nDays);	// return -1: error
bool	IsActiveTime(time_t tCurr, unsigned long nFormat);
bool	IsTimeRange(unsigned long nBegin, unsigned long nEnd, time_t tCurr=0);
char*	StrStrCh(const char* string, const char* strCharSet);
int		CalcNumberAxisIntersection(int x1, int y1, int x2, int y2);

template<class T>
bool CheckBetween(IN OUT T& nowVaule, T minVaule, T maxVaule, T defaultValue)
{
	bool bResult = false;
	if (nowVaule >= minVaule && nowVaule <= maxVaule)
	{
		bResult = true;
	}
	else
	{
		nowVaule = defaultValue;
	}

	return bResult;
}

enum TIME_TYPE{
	TIME_BEGIN,
	TIME_MILLISECOND=TIME_BEGIN, 
	TIME_SECOND, 
	TIME_MINUTE, 
	TIME_DAY, 
	TIME_DAYTIME, 
	TIME_STAMP,
	TIME_EPOCH_DAY,//20070402������:ʱ���.��ǰʱ�����ο�ʱ��������
	MONTH_DAY,//20070402������:�õ����µļ���
	YEAR_DAY,//20070402������:�õ�һ���еĵڼ���
	WEEK_DAY,//�õ�һ�����ڵĵڼ���
	YEAR_MONTH_DAY,
	TIME_DAY_START,
	TIME_DAY_END,
	TIME_WEEK_START,
	TIME_WEEK_END,
	TIME_MONTH_START,
	TIME_MONTH_END,
	TIME_END,
};

DWORD	TimeGet(TIME_TYPE type=TIME_MILLISECOND);
I64		GetUtcMillisecond(void);
int GetNextWeekStartUTC();

int		RandGet		(int nMax, BOOL bReset=false);
int		RandGetZT(int nMax, double dRangeParamL=3.0, double dRangeParamR=3.0, bool bRest=false);
void	DDALineEx(int x0, int y0, int x1, int y1, vector<POINT>& vctPoint);
//���һ���졢Сʱ�����ӡ��롢����ƴ��������
DWORD GetTimeDayToMiniSecond();

UINT GetNextUpdateTime(UINT unHour);
UINT GetLog2(UINT nNum);

// ��õ�ǰʱ��������ļ��ʱ�䣬֧��TIME_MINUTE
UINT GetTimeDuring(UINT unLastTime);


//[��;���� 2009.06.10]�ʲ�������־
//////////////////////////////////////////////////////////////////////////
enum
{
	NONE = 0,
		BOOTH_SALE,//��̯��
		BOOTH_BUY,//��̯����
		BANGPAI_HONGBAO,//�����
		TRADE_ONE,//����1
		TRADE_TWO,//����2
		SHOP_ADDBUY,//��Ӽ�����Ϣ
		SHOP_SALE,//���̹�����Ʒ
		SHOP_ADDSALE,//��ӳ�����Ʒ
		SHOP_BUY,//�����չ�
		SHOP_DEL_SALE,//�˻س�����Ʒ
		SHOP_DEL_BUY,//�˻��չ���Ʒ
		SHOP_PAIMAI_TUIHUI,//�˻�����Ǯ
		SHOP_PAIMAI,//����
		SHOP_XUZU,//��������
		SHOP_TIQU,//������ȡ
		MAIL_YIBAN,//�ʼ���־
		MAIL_JIAOYI,//�ʼ���־
		MAIL_TUIHUI,//�ʼ���־
};

struct STPROPERTYSTREAMLOG
{
	DWORD	idSource;//Դ��ʶ
	DWORD	idSAccount;
	DWORD	dwSVas;//Դ��ǰVAS
	DWORD	dwSMoney;//Դ��ǰ��Ϸ��
	DWORD	idTarget;//Ŀ���ʶ
	DWORD	idTAccount;
	DWORD	dwTVas;//Դ��ǰVAS
	DWORD	dwTMoney;//Դ��ǰ��Ϸ��
	DWORD	dwVas;//����VAS
	DWORD	dwMoney;//��������
	DWORD	idItemType;
	DWORD	idSItem;//������Ʒ��ʶ
	DWORD	dwSNumber;//������Ʒ��ʶ
	DWORD	idTItem;//������Ʒ��ʶ
	DWORD	dwTNumber;//������Ʒ��ʶ
	DWORD	dwSysVas;
	DWORD	dwSysMoney;
	DWORD	dwType;//����
	
	STPROPERTYSTREAMLOG()
	{
		idSource = NULL;//Դ��ʶ
		idSAccount = NULL;
		dwSVas = NULL;//Դ��ǰVAS
		dwSMoney = NULL;//Դ��ǰ��Ϸ��
		idTarget = NULL;//Ŀ���ʶ
		idTAccount = NULL;
		dwTVas = NULL;//Դ��ǰVAS
		dwTMoney = NULL;//Դ��ǰ��Ϸ��
		dwVas = NULL;//����VAS
		dwMoney = NULL;//��������
		idItemType = NULL;
		idSItem = NULL;//������Ʒ��ʶ
		dwSNumber = NULL;//������Ʒ��ʶ
		idTItem = NULL;//������Ʒ��ʶ
		dwTNumber = NULL;//������Ʒ��ʶ
		dwSysVas = NULL;
		dwSysMoney = NULL;
		dwType = NULL;//����
	}
};

//////////////////////////////////////////////////////////////////////////
//	20070709 ��� ��¼������Ʒ����
void	MyLogSaveCostlyItem(const char* pszServerName,
							int	nAccountID, int nCharID, const char* pszCharName,
							int nOtherAccountID, int nOtherCharID, const char* pszOtherCharName,
							int nType, int nItemTypeID, int	nItemID, const char* pszItemName, int nItemCount);
void	PropertyStreamLog(STPROPERTYSTREAMLOG &stProStream);

BOOL	StringCheck	(const char* pszString, BOOL bNewLineEnable=false);
BOOL	TalkStrCheck(char* pszString, BOOL bNewLineEnable=false);
BOOL	DisableWordCheck(const char* pszString, BOOL bNewLineEnable=false);
bool	StrSqlCheck(const char* pszString);
BOOL	NameStrCheck(const char* pszString);

double	RandomRateGet	(double dRange);
int		Double2Int		(double dValue);

inline int	AddToTop(int nData, int nAdd, int nTop) { int res=nData+nAdd; if(nAdd>0 && res<nData || res>nTop) return nTop; else return res; }

#define _TIMECOSTCHECK///����ʱ��ɱ���⿪��
#ifdef _TIMECOSTCHECK
	#define TimeCostChk(x,y) CTimeCostChk chk(x,y,100)
#else
	#define TimeCostChk(x,y)
#endif

/////////////////////////////////////////////////////////////////////////////
class CTimeCostChk
{
public:
	CTimeCostChk(const char* szReason,const char* pszStr, DWORD tmChk) : m_tmChk(tmChk) 
	{
		m_tmBegin	= GetTickCount();
		m_pszStr	= pszStr;
		m_pszReason =szReason;
	}
	
	~CTimeCostChk(void)
	{
		DWORD tmCost = GetTickCount()-m_tmBegin;
		if (tmCost > m_tmChk)
		{
			::LogTimeOut("CTimeCostChk Overtime (%ums>%ums) when szReason:%s chk:%s", tmCost,m_tmChk,m_pszReason,m_pszStr); 
		}
	}

private:
	DWORD m_tmChk;
	const char* m_pszStr;
	const char* m_pszReason;///ԭ��
	DWORD m_tmBegin;
};


/////////////////////////////////////////////////////////////////////////////
template<TYPENAME T>
inline T	CutOverflow(T x, T y) { return (x <= y) ? x : y; }
template<TYPENAME T>
inline T	CutTrail(T x, T y) { return (x >= y) ? x : y; }
template<TYPENAME T>
inline T	CutRange(T n, T min, T max) { return (n<min) ? min : ((n>max) ? max : n); }

/////////////////////////////////////////////////////////////////////////////
inline unsigned int	ExchangeShortBits(unsigned long nData, int nBits)
{
	ASSERT(nBits >= 0 && nBits < 16);
	nData	&= 0xFFFF;
	return ((nData>>nBits) | (nData<<(16-nBits))) & 0xFFFF;
}

inline unsigned int	ExchangeLongBits(unsigned long nData, int nBits)
{
	ASSERT(nBits >= 0 && nBits < 32);
	return (nData>>nBits) | (nData<<(32-nBits));
}

inline double CalculationRatio(DWORD dwData, DWORD dwRatio)
{
	double dValue = ceil(dwData*dwRatio/10000.0);
	return dValue;
}


//////////////////////////////////////////////////////////////////////
inline int pos2index(int x, int y, int cx, int cy) { return (x + y*cx); }
inline int index2x(int idx, int cx, int cy) { return (idx % cy); }
inline int index2y(int idx, int cx, int cy) { return (idx / cy); }
inline void pos2lt(POINT* pTarget, const POINT& pos, int nRange) { pTarget->x = pos.x - nRange; pTarget->y = pos.y - nRange; }
inline void posadd(POINT* pTarget, int nOffsetX, int nOffsetY) { pTarget->x += nOffsetX; pTarget->y += nOffsetY; }
inline void possub(POINT* pTarget, int nOffsetX, int nOffsetY) { pTarget->x -= nOffsetX; pTarget->y -= nOffsetY; }

inline bool ConvertMailText(OUT LPSTR lpstrText, UINT nAction, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0, 
							int nParam4 = 0, int nParam5 = 0, int nParam6 = 0, LPCSTR lpstrParam1 = "", LPCSTR lpstrParam2 = "");

bool MyANSIToUnicode(IN const char* pszStr,		OUT wchar_t* pwszStr,	int nUnicodeStrSize);
bool MyUnicodeToANSI(IN const wchar_t* pwszStr,	OUT char* pszStr,		int nAnsiStrSize);
bool MyUnicodeToUTF8(IN const wchar_t* pwszStr, OUT char* pszStr,		int nUtf8StrSize);
bool MyUTF8ToUnicode(IN const char* pszStr,		OUT wchar_t* pwszStr,	int nUnicodeStrSize);

CMxyString MyUtf8ToANSI(const char* pszString);
CMxyString MyANSIToUtf8(const char* pszString);

int bit_count(int n);//���������е�1�ĸ���
UINT GetNextUpdateMonthDate();//����������������ȡ�¸��µ�ÿһ��(YYYYMMDD)��ȷ����

UINT GetItemLifeTimeMinute(int nLifeTimeOffset);

bool SafeCopy(char * pTarget, const char * pSource, int nBufLen = 0);	// �ַ�����ȫ����
bool IsObjType(OBJID idObjType, OBJID idUnion);							// �ж϶���type
double SaveTwoDecimal(double dInput);									// double���ͱ�����λС��

////////////////////////////////////////////////////////////////////////
// Description:  ��ֵ�޶��ں���Χ��
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: class T
////////////////////////////////////////////////////////////////////////
template <class T>
T GetValueInRightRange(T value, T minValue, T maxValue)
{
	value = min(value, maxValue);
	value = max(value, minValue);
	return value;
}

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);

int GetUTCSecond(int nTimeType, int nTime);

string LoadFileToString(const string& strFileName);	//���ļ����뵽string��
bool ReplaceStdString(IN OUT string& str, const char* pszOld, const char* pszNew, bool bReplaceAll = true);	// �滻std::string�е��ַ�

unsigned int GetMsgKey(unsigned int unMsgType, unsigned int unMsgIndex);
int		MyGetMAC(OUT char* pszMac, int i);     //��NetAPI����ȡ����MAC��ַ   

bool IsExistFile( const char* pszFileName );
#endif
