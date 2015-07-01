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

#pragma warning(disable:4996)		// 微软提示要用自家的安全_s函数
#pragma warning(disable:4018)		// 有无符号
#pragma warning(disable:4244)		// 精度损失

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
	TIME_EPOCH_DAY,//20070402彭正帮:时间点.当前时间距离参考时间点的天数
	MONTH_DAY,//20070402彭正帮:得到当月的几号
	YEAR_DAY,//20070402彭正帮:得到一年中的第几天
	WEEK_DAY,//得到一个星期的第几天
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
//获得一个天、小时、分钟、秒、毫秒拼接在数字
DWORD GetTimeDayToMiniSecond();

UINT GetNextUpdateTime(UINT unHour);
UINT GetLog2(UINT nNum);

// 获得当前时间与参数的间隔时间，支持TIME_MINUTE
UINT GetTimeDuring(UINT unLastTime);


//[游途道标 2009.06.10]资产流向日志
//////////////////////////////////////////////////////////////////////////
enum
{
	NONE = 0,
		BOOTH_SALE,//摆摊卖
		BOOTH_BUY,//摆摊购买
		BANGPAI_HONGBAO,//发红包
		TRADE_ONE,//交易1
		TRADE_TWO,//交易2
		SHOP_ADDBUY,//添加寄售信息
		SHOP_SALE,//店铺购买物品
		SHOP_ADDSALE,//添加出售物品
		SHOP_BUY,//店铺收购
		SHOP_DEL_SALE,//退回出售物品
		SHOP_DEL_BUY,//退回收购物品
		SHOP_PAIMAI_TUIHUI,//退回拍卖钱
		SHOP_PAIMAI,//拍卖
		SHOP_XUZU,//店铺续租
		SHOP_TIQU,//店铺提取
		MAIL_YIBAN,//邮件日志
		MAIL_JIAOYI,//邮件日志
		MAIL_TUIHUI,//邮件日志
};

struct STPROPERTYSTREAMLOG
{
	DWORD	idSource;//源标识
	DWORD	idSAccount;
	DWORD	dwSVas;//源当前VAS
	DWORD	dwSMoney;//源当前游戏币
	DWORD	idTarget;//目标标识
	DWORD	idTAccount;
	DWORD	dwTVas;//源当前VAS
	DWORD	dwTMoney;//源当前游戏币
	DWORD	dwVas;//流动VAS
	DWORD	dwMoney;//流动货币
	DWORD	idItemType;
	DWORD	idSItem;//流动物品标识
	DWORD	dwSNumber;//流动物品标识
	DWORD	idTItem;//流动物品标识
	DWORD	dwTNumber;//流动物品标识
	DWORD	dwSysVas;
	DWORD	dwSysMoney;
	DWORD	dwType;//类型
	
	STPROPERTYSTREAMLOG()
	{
		idSource = NULL;//源标识
		idSAccount = NULL;
		dwSVas = NULL;//源当前VAS
		dwSMoney = NULL;//源当前游戏币
		idTarget = NULL;//目标标识
		idTAccount = NULL;
		dwTVas = NULL;//源当前VAS
		dwTMoney = NULL;//源当前游戏币
		dwVas = NULL;//流动VAS
		dwMoney = NULL;//流动货币
		idItemType = NULL;
		idSItem = NULL;//流动物品标识
		dwSNumber = NULL;//流动物品标识
		idTItem = NULL;//流动物品标识
		dwTNumber = NULL;//流动物品标识
		dwSysVas = NULL;
		dwSysMoney = NULL;
		dwType = NULL;//类型
	}
};

//////////////////////////////////////////////////////////////////////////
//	20070709 朱斌 记录贵重物品流向
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

#define _TIMECOSTCHECK///定义时间成本检测开启
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
	const char* m_pszReason;///原因
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

int bit_count(int n);//检测二进制中的1的个数
UINT GetNextUpdateMonthDate();//用于月掩码清理，获取下个月的每一天(YYYYMMDD)精确到日

UINT GetItemLifeTimeMinute(int nLifeTimeOffset);

bool SafeCopy(char * pTarget, const char * pSource, int nBufLen = 0);	// 字符串安全拷贝
bool IsObjType(OBJID idObjType, OBJID idUnion);							// 判断对象type
double SaveTwoDecimal(double dInput);									// double类型保留两位小数

////////////////////////////////////////////////////////////////////////
// Description:  将值限定在合理范围内
// Arguments:
// Author: 彭文奇(Peng Wenqi)
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

string LoadFileToString(const string& strFileName);	//从文件读入到string里
bool ReplaceStdString(IN OUT string& str, const char* pszOld, const char* pszNew, bool bReplaceAll = true);	// 替换std::string中的字符

unsigned int GetMsgKey(unsigned int unMsgType, unsigned int unMsgIndex);
int		MyGetMAC(OUT char* pszMac, int i);     //用NetAPI来获取网卡MAC地址   

bool IsExistFile( const char* pszFileName );
#endif
