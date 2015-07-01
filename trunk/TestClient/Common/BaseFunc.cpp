#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

#include <afxmt.h>
#include <windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <io.h>
#include "common.h"
#include "BaseFunc.h"
#include "AFXDISP.H"//2007114������:COleDateTime
#include <fstream>
#include <sstream>

#include <httpext.h>   //��ͷ�ļ�֧��HTTP����   
#include <Nb30.h>      //��ͷ�ļ�������netbios�����еĺ���    
#pragma comment(lib,"netapi32.lib")   //����Netapi32.lib�⣬MAC��ȡ���õ���NetApi32.DLL�Ĺ���   

//20070627������:��־���ر�־λ
BOOL g_bLogSave			= TRUE;
BOOL g_bLogWalk			= FALSE;	
BOOL g_bLogDatabaseSql	= FALSE;	

BYTE g_cbEncryptKey=0;

/////////////////////////////////////////////////////////////////////////////
// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
// ��ʽ������ʱ�䴮
void	DateTime(char * buf20, time_t tInput /*= 0*/)	// ����buf20��
{
	if(!buf20)
		return;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	if(buf20)
		sprintf(buf20, "%04d-%02d-%02d %02d:%02d:%02d", 
					pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}

void TimeFormatFormUtcSecond(char* pszBuf9, unsigned int unUtcSecond /* = 0 */)
{
	CHECK(pszBuf9);
	if(unUtcSecond == 0)
	{
		unUtcSecond = ::time(NULL);
	}

	time_t tTime = unUtcSecond;
	tm* pTm = ::localtime(&tTime);
	if (pTm)
	{
		sprintf(pszBuf9, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
	}
}

////////////////////////////////////////////////////////////////////////////
bool FormatDateTime(char* buf, const char* szFormat, time_t tInput /*= 0*/)
{
	if(!buf || !szFormat)
		return false;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	sprintf(buf, szFormat, 
			pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

	return true;
}

void DateTimeStamp(char * buf15, time_t tInput /*= 0*/)	// ����buf15��
{
	if(!buf15)
		return;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	if(buf15)
		sprintf(buf15, "%04d%02d%02d%02d%02d%02d", 
					pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}

////////////////////////////////////////////////////////////////////////////
int DateStamp(time_t tTime/*=0*/)
{
	time_t	tInput;
	if(tTime)
		tInput	= tTime;
	else
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	return (pTm->tm_year+1900)	* 10000
			+ (pTm->tm_mon+1) * 100
			+ pTm->tm_mday;
}

////////////////////////////////////////////////////////////////////////////
// return: ʱ���ǩnDate����ָ������nDays
// return -1: error
int DateStampPass(int nDate, int nDays)
{
	time_t	tCurr = time(NULL);
	tm	tmComp;
	memset(&tmComp, 0, sizeof(tm));
	tmComp.tm_year	= nDate/10000 - 1900;
	tmComp.tm_mon	= (nDate/100)%100 - 1;
	tmComp.tm_mday	= nDate%100 + nDays;

	time_t	tComp	= mktime(&tmComp);
	if(tComp == -1)
		return -1;

	return DateStamp(tComp);
}

//////////////////////////////////////////////////////////////////////
bool IsActiveTime(time_t tCurr, unsigned long nFormat)		// DDWWHHMMSS
{
	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);

	if(nFormat / 100000000)		// day per month
	{
		if(pTm->tm_mday == int(nFormat/100000000)
			&& pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	else if(nFormat / 1000000)		// day per weak
	{
		if(((pTm->tm_wday+6)%7)+1 == int(nFormat/1000000)
			&& pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	else			// time per day
	{
		if(pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	return false;
}

bool IsBetween(int nCurr, int nBegin, int nEnd)
{
	if(nBegin <= nEnd)
	{
		if(nBegin <= nCurr && nCurr < nEnd)
			return true;
	}
	else // if(nEnd < nBegin)
	{
		if(nCurr < nEnd || nBegin <= nCurr)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool IsTimeRange(unsigned long nBegin, unsigned long nEnd, time_t tCurr/*=0*/)		// DDWWHHMMSS
{
	if(tCurr == 0)
		tCurr = time(NULL);

	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);

	if(nBegin / 100000000)		// per month
	{
		if(IsBetween(pTm->tm_mday*100*1000000 + pTm->tm_hour*10000 + pTm->tm_min*100 + pTm->tm_sec, nBegin, nEnd))
			return true;
	}
	else		// per weak or day ...
	{
		if(IsBetween(pTm->tm_wday*1000000 + pTm->tm_hour*10000 + pTm->tm_min*100 + pTm->tm_sec, nBegin, nEnd))
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
char*	StrStrCh(const char* string, const char* strCharSet)
{
	CHECKF(string && strCharSet);
	unsigned int	nLen	= strlen(strCharSet);
	if(nLen == 0 || strlen(string) < nLen)
		return NULL;

	char	head	= *strCharSet;
	const char* ptr = string;
	while(*ptr)
	{
		while(*ptr && *ptr != head)
		{
			unsigned char uch = static_cast<unsigned char>(*ptr);
			if(uch >= 0x81 && uch <= 0xFE && *(ptr+1))
				ptr++;
			ptr++;
		}
		if(*ptr && strncmp(ptr, strCharSet, nLen) == 0)
			return const_cast<char*>(ptr);
		ptr++;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//	20070709 ��� ������Ʒ��¼
void	MyLogSaveCostlyItem(const char* pszServerName/*��������,����*/,
							int	nAccountID/*�˺�ID*/, int nCharID/*��ɫID*/, const char* pszCharName/*��ɫ����*/,
							int nOtherAccountID/*��һ�˺�ID*/, int nOtherCharID/*��һ��ɫID*/, const char* pszOtherCharName/*��һ��ɫ����*/,
							int nType/*��������*/, int nItemTypeID/*��Ʒ����*/, int	nItemID/*��ƷID*/, const char* pszItemName/*��Ʒ����*/, int nItemCount/*����*/)
{
	LOGFUNC_TRY
	char	str[2048];
	sprintf(str, "%d\t%d\t%s\t%d\t%d\t%s\t%d\t%d\t%d\t%s\t%d", nAccountID, nCharID,
			pszCharName, nOtherAccountID, nOtherCharID, pszOtherCharName,
			nType, nItemTypeID, nItemID, pszItemName, nItemCount);

	MyLogSave2("gmlog/CostlyItem", str);
	LOGFUNC_CATCH("MyLogSaveCostlyItem");
}

//////////////////////////////////////////////////////////////////////////
//[��;���� 2009.06.10]�ʽ�������Ʒ�����¼
void PropertyStreamLog(STPROPERTYSTREAMLOG &stProStream)
{
	LOGFUNC_TRY
	char str[2048];
	sprintf(str, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
		stProStream.idSAccount,
		stProStream.idSource,	//1
		stProStream.dwSVas,		//2
		stProStream.dwSMoney,	//3
		stProStream.idTAccount,
		stProStream.idTarget,	//4
		stProStream.dwTVas,		//5
		stProStream.dwTMoney,	//6
		stProStream.dwVas,		//7
		stProStream.dwMoney,	//8
		stProStream.idItemType,	//9
		stProStream.idSItem,	//10
		stProStream.dwSNumber,	//11
		stProStream.idTItem,	//12
		stProStream.dwTNumber,	//13
		stProStream.dwSysVas,
		stProStream.dwSysMoney,
		stProStream.dwType);	//14

	MyLogSave2("gmlog/ZiChan", str);
	LOGFUNC_CATCH("PropertyStreamLog");
}

//////////////////////////////////////////////////////////////////////
int	Double2Int(double dValue)
{
	if((int)(dValue+0.5) >(int)dValue)
		return int(dValue)+1;
	else
		return int(dValue);
}

//////////////////////////////////////////////////////////////////////////
DWORD TimeGet(TIME_TYPE type/*=TIME_MILLISECOND*/)
{
	DWORD dwTime = 0;
	switch(type)
	{
	case TIME_SECOND:
		dwTime = (DWORD)::time(NULL);
		break;

	case TIME_MINUTE:
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	pTime->tm_year%100*100000000 +
						(pTime->tm_mon+1)*1000000 +
						pTime->tm_mday*10000 +
						pTime->tm_hour*100 + 
						pTime->tm_min;
		}
		break;

	case TIME_DAY:
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	pTime->tm_year*10000 +
						(pTime->tm_mon+1)*100 +
						pTime->tm_mday;
		}
		break;

	case TIME_DAYTIME: 
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	pTime->tm_hour*10000 + 
						pTime->tm_min *100 +
						pTime->tm_sec;
		}
		break;
	case TIME_STAMP: 
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	(pTime->tm_mon+1)*100000000 +
						pTime->tm_mday*1000000 +
						pTime->tm_hour*10000 + 
						pTime->tm_min *100 +
						pTime->tm_sec;
		}
		break;
		//20070402������:ʱ���.����
	case TIME_EPOCH_DAY:
		{
			//����1970 1�� 1�� 0:00:00 ������
			COleDateTime dt(1970,1,1,0,0,0);
			COleDateTimeSpan dts=COleDateTime::GetCurrentTime()-dt;
			dwTime = dts.GetDays();
		}
		break;
	case MONTH_DAY:///������������1~31
		{
			dwTime=COleDateTime::GetCurrentTime().GetDay();
		}
		break;
	case YEAR_DAY:///������������
		{
			dwTime=COleDateTime::GetCurrentTime().GetDayOfYear();
		}
		break;
	case WEEK_DAY:///������������
		{
			dwTime=COleDateTime::GetCurrentTime().GetDayOfWeek() - 1;
		}
		break;
	case YEAR_MONTH_DAY:
		{
			time_t long_time = 0;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime = NULL;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	(pTime->tm_year+1900)*10000 +
				(pTime->tm_mon+1)*100 +
				pTime->tm_mday;
		}
	case TIME_DAY_START:	// ����ĵ�һ��
		{
			time_t long_time = 0;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime = NULL;
			pTime = localtime( &long_time ); /* Convert to local time. */
			pTime->tm_hour	= 0;
			pTime->tm_min	= 0;
			pTime->tm_sec	= 0;

			dwTime = ::mktime(pTime);
		}
		break;
	case TIME_DAY_END:	// �������һ��
		{
			time_t long_time = 0;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime = NULL;
			pTime = localtime( &long_time ); /* Convert to local time. */
			pTime->tm_hour	= 23;
			pTime->tm_min	= 59;
			pTime->tm_sec	= 59;

			dwTime = ::mktime(pTime);
		}
		break;
	case TIME_WEEK_START:	// ���ܵ�һ��
		{
			dwTime = ::GetNextWeekStartUTC() - 7 * 24 * 60 * 60;
		}
		break;
	case TIME_WEEK_END:		// �������һ��
		{
			dwTime = ::GetNextWeekStartUTC() - 1;
		}
		break;
	case TIME_MONTH_START:	// ���µ�һ��
		{
			time_t long_time = 0;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime = NULL;
			pTime = localtime( &long_time ); /* Convert to local time. */
			pTime->tm_mday	= 1;
			pTime->tm_hour	= 0;
			pTime->tm_min	= 0;
			pTime->tm_sec	= 0;

			dwTime = ::mktime(pTime);
		}
		break;
	case TIME_MONTH_END:
		{
			time_t long_time = 0;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime = NULL;
			pTime = localtime( &long_time ); /* Convert to local time. */

			switch (pTime->tm_mon)
			{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				{
					pTime->tm_mday	= 31;
				}
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				{
					pTime->tm_mday	= 30;
				}
				break;
			case 2:
				{
					int nYear = pTime->tm_year + 1900;
					if(  ((0 == nYear%4) &&(0 != nYear % 100)) ||(0 == nYear %400) )
					{
						pTime->tm_mday = 29;
					}
					else
					{
						pTime->tm_mday = 28;
					}
				}
				break;
			default:
				break;
			}

			pTime->tm_hour	= 23;
			pTime->tm_min	= 59;
			pTime->tm_sec	= 59;

			dwTime = ::mktime(pTime);
		}
		break;
		break;
	default:
		dwTime = ::timeGetTime();
		break;
	}

	return dwTime;
}

// ȡ����һ��0ʱ0��0��
int GetNextWeekStartUTC()
{
	COleDateTime tCur = COleDateTime::GetCurrentTime();
	const int DAYS_OF_WEEK = 7;
	int nCurDayOfWeek = tCur.GetDayOfWeek();	// ������Ϊ1 ������Ϊ7
	int nSpanDay = 0;
	if (1 == nCurDayOfWeek) // ���������������
	{
		nSpanDay = 1;
	}
	else
	{
		nSpanDay = DAYS_OF_WEEK - nCurDayOfWeek + 2;
	}
	CHECKF(nSpanDay);

	COleDateTimeSpan tSpan(nSpanDay,0,0,0);
	COleDateTime tNeedTime = tCur + tSpan;
	tNeedTime.SetDateTime(tNeedTime.GetYear(), tNeedTime.GetMonth(), tNeedTime.GetDay(),0,0,0);

	time_t tNowTime = ::time(NULL);
	struct tm * pTime = ::localtime(&tNowTime);
	CHECKF(pTime);
	pTime->tm_year = tNeedTime.GetYear()%1000 + 100;
	pTime->tm_mon = tNeedTime.GetMonth() - 1;
	pTime->tm_mday = tNeedTime.GetDay();
	pTime->tm_hour = tNeedTime.GetHour();
	pTime->tm_min = tNeedTime.GetMinute();
	pTime->tm_sec = tNeedTime.GetSecond();

	int nNeedUTC = ::mktime(pTime);
	return nNeedUTC;
}

////////////////////////////////////////////////////////////////////////
// Description:  �߾���utc����ʱ��, �ú����ο�clockʵ��
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: I64
////////////////////////////////////////////////////////////////////////
I64	GetUtcMillisecond(void)
{
	FILETIME ct = {0};
	::GetSystemTimeAsFileTime( &ct );
	__int64 current_tics = (__int64)ct.dwLowDateTime + (((__int64)ct.dwHighDateTime) << 32);
	return current_tics / 10000i64;
}

//////////////////////////////////////////////////////////////////////
//20061020������.���������д.������Ĭ��RAND_MAX 0x7fff��ߵ�10000W.
int RandGet(int nMax, BOOL bReset)
{
	if( nMax == 0 )
		return 0;
	if( nMax >1000000000 )
	{
		LOGWARNING("RandGet()nMaxֵΪ%d,��������,������������",nMax);
	}
	if ( bReset )
		::srand( (unsigned)TimeGet() );

	int newRand;
	newRand=::rand()%1000*10000 + ::rand()%10000;
	// pengfeng ��������������λ [12/18/2007]//0~9999999,�����߽�
	double num = 0;
	num = ::rand()%100*10000000;
	newRand += (int)num;//0~999999999,�����߽�

	return newRand%nMax;
}

////////////////////////////////////////////////////////////////////////
// Description: ��̬�ֲ�������� ȡ[0, nMax)����̬�ֲ�
//				��������� = 0, �� = 1�ı�׼��̬�ֲ�, 
//				Ĭ��������3����ȡֵ, ���ಿ�ְ���ƽ�����ʷ���RandGet(nMax)
//
// Arguments:	dRangeParam ��ʾȡֵ��Χ, Ĭ��3.0. ������3����ȡֵ, 
//				ʣ�ಿ�־�����ƽ�����ʷ���RandGet(nMax)����, ��֤��̬�ֲ�ͼ�β�ͻأ
//				1�Ұ���68.268949% �������
//				2�Ұ���95.449974% �������
//				3�Ұ���99.730020% �������
//				4�Ұ���99.993666% �������
//				dRangeParam���С��0.1��ȡֵ0.1
//				dRangeParamԽС����λ������ȡֵԽƽ��, Խ������, ������̬�ֲ����ɵ����˲�������, һ��ȡ3.0����
//
//				��ѧԭ��ΪBox-Muller����, ��:		
//				��������x,y�����, ����(0,1]ƽ���ֲ�, 
//				��ô (-2 * log(x))^0.5 * cos(2 * pi * y) �� (-2 * log(x))^0.5 * sin(2 * pi * y) ������(0,1)��̬�ֲ�
//				�����в���cos���ַ���
// 
// Author: ������(Peng Wenqi)
// Return Value: INT
////////////////////////////////////////////////////////////////////////
int	RandGetZT(int nMax, double dRangeParamL/*=3.0*/, double dRangeParamR/*=3.0*/, bool bRest/*=false*/)
{
	if (nMax < 0)
	{
		return 0;
	}
	
	if (dRangeParamL < 0.1)
	{
		dRangeParamL = 0.1;
	}
	
	if (dRangeParamR < 0.1)
	{
		dRangeParamR = 0.1;
	}
	
	const double dSigma			= 1.0;						// ��Ҫ�޸Ħ�, ������Χ��RangeParam
	const double dPi			= 3.1415926535897932;	
	const _int64 i64RandBase	= /*0x7fffffff*/1000000000;	// ����ƽ���ֲ�����,

	// �� dRandGet_X, dRandGet_Y ����(0,1]ƽ���ֲ�, ��Ϊ�뿪�������, ������ +1
	_int64 i64RandGet_X = RandGet(i64RandBase, bRest) + 1;
	_int64 i64RandGet_Y = RandGet(i64RandBase, false) + 1;
	
	double dRandGet_X = (double)i64RandGet_X / (double)i64RandBase;	
	double dRandGet_Y = (double)i64RandGet_Y / (double)i64RandBase;
	
	double dParam	= -2.0 * ::log(dRandGet_X);
	if (dParam < 0.0)
	{
		return 0;
	}
	
	double dResultZT = ::sqrt(dParam) * ::cos(2.0 * dPi * dRandGet_Y);
	
	// ������λ��Ϊ3��
	dResultZT += (dRangeParamL * dSigma);
	
	// ������3����������ݽ��б߽紦��
	if (dResultZT < 0.0 || dResultZT > (dRangeParamL + dRangeParamR) * dSigma)
	{
		return RandGet(nMax);
	}
	
	// ����Ϊ0-1֮��ȡֵ
	dResultZT /= ((dRangeParamL + dRangeParamR) * dSigma);
	
	int nResultZT = (int)(dResultZT * nMax);
	
	// ��ֹԽ��
	if (nResultZT < 0 || nResultZT >= nMax)
	{
		return RandGet(nMax);
	}
	
	return nResultZT;
}


//////////////////////////////////////////////////////////////////////
double RandomRateGet(double dRange)
{
	double pi=3.1415926;

	int nRandom	=RandGet(999, false)+1;
	double a	=sin(nRandom*pi/1000);
	double b;
	if (nRandom >= 90)
		b	=(1.0+dRange)-sqrt(sqrt(a))*dRange;
	else
		b	=(1.0-dRange)+sqrt(sqrt(a))*dRange;

	return b;
}

//////////////////////////////////////////////////////////////////////
void DDALineEx(int x0, int y0, int x1, int y1, vector<POINT>& vctPoint)
{
	vctPoint.clear();
	if (x0 == x1 && y0 == y1)
		return;

	int dx = x1 - x0;
	int dy = y1 - y0;
	int abs_dx = abs(dx);
	int abs_dy = abs(dy);
	if(abs_dx > abs_dy)
	{
		int _0_5		= abs_dx * (dy>0 ? 1:-1);
		int numerator	= dy * 2;
		int denominator	= abs_dx * 2;
		// x ����
		if(dx > 0)
		{
			// x0 ++
			for(int i = 1; i <= abs_dx; i ++)
			{
				POINT point;
				point.x = x0 + i;
				point.y = y0 + ((numerator*i + _0_5) / denominator);
				vctPoint.push_back(point);
			}
		}
		else if(dx < 0)
		{
			// x0 --
			for(int i = 1; i <= abs_dx; i ++)
			{
				POINT point;
				point.x = x0 - i;
				point.y = y0 + ((numerator*i + _0_5) / denominator);
				vctPoint.push_back(point);
			}
		}
	}
	else
	{
		int _0_5		= abs_dy * (dx>0 ? 1:-1);
		int numerator	= dx * 2;
		int denominator	= abs_dy * 2;
		// y ����
		if(dy > 0)
		{
			// y0 ++
			for(int i = 1; i <= abs_dy; i ++)
			{
				POINT point;
				point.y = y0 + i;
				point.x = x0 + ((numerator*i + _0_5) / denominator);
				int nAmount = vctPoint.size();
				vctPoint.push_back(point);
			}
		}
		else if(dy < 0)
		{
			// y0 -- 
			for(int i = 1; i <= abs_dy; i ++)
			{
				POINT point;
				point.y = y0 - i;
				point.x = x0 + ((numerator*i + _0_5) / denominator);
				vctPoint.push_back(point);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
BOOL StringCheck(const char* pszString, BOOL bNewLineEnable)
{
	return true;

	if (!pszString)
		return false;

	int nLen	=strlen(pszString);
	for (int i=0; i < nLen; i++)

	{
		unsigned char c	=(unsigned char)pszString[i];
		if (c >= 0x81 && c <= 0xfe)	
		{
			if (i+1 >= nLen)
				return false;

			unsigned char c2	=(unsigned char)pszString[i+1];
			if (c2 < 0x40 && c2 > 0x7e && c2 < 0x80 && c2 > 0xfe)
				return false;
			else
				i++;
		}
		else
		{
			if (c==0x80)
				return false;
			else
			{
				if(bNewLineEnable)
				{
					if (c < ' ' && c != 0x0A && c != 0x0D)
						return false;
				}
				else
				{
					if (c < ' ')
						return false;
				}
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
BOOL NameStrCheck(const char* pszString)
{
 	if (!pszString)
		return false;

	if (!StrSqlCheck(pszString))
		return false;

	if (strstr(pszString, "��"))
		return false;

	if (strstr(pszString, "��"))
		return false;

	if (strstr(pszString, "GM"))
		return false;

	if (strstr(pszString, "gm"))
		return false;

	if (strstr(pszString, "PM"))
		return false;

	if (strstr(pszString, "pm"))
		return false;

	if (strstr(pszString, "SYSTEM"))
		return false;

	if (strstr(pszString, "system"))
		return false;

	if (strstr(pszString, "NPC"))
		return false;

	if (strstr(pszString, "npc"))
		return false;

	return ::DisableWordCheck(pszString);
}

//////////////////////////////////////////////////////////////////////
BOOL TalkStrCheck(char* pszString, BOOL bNewLineEnable)
{
	return true;
	if (!pszString)
		return false;

	int nLen	=strlen(pszString);
	for (int i=0; i < nLen; i++)
	{
		unsigned char c	=pszString[i];
		if(c < ' ' && c != 0x0d && c != 0x0a)
			return false;
		switch(c)
		{
		case '\\':
		//case '\''://20090511haocq��˫���Ų�����
		//case '"':
			return false;
		}
	}

	return ::DisableWordCheck(pszString, bNewLineEnable);
}

//////////////////////////////////////////////////////////////////////
BOOL DisableWordCheck(const char* pszString, BOOL bNewLineEnable)
{
	if (strstr(pszString, "fuck"))
		return false;

	if (strstr(pszString, "FUCK"))
		return false;
	return ::StringCheck(pszString, bNewLineEnable);
}

//////////////////////////////////////////////////////////////////////////
DWORD GetTimeDayToMiniSecond()
{
	SYSTEMTIME sysTime={0};
	::GetLocalTime(&sysTime);
	
	DWORD dwDay = sysTime.wDay;
	DWORD dwHour = sysTime.wHour;
	DWORD dwMinute = sysTime.wMinute;
	DWORD dwSecond = sysTime.wSecond;
	DWORD dwMiniSecond = sysTime.wMilliseconds;

	return dwDay*100000000 + dwHour*1000000 + dwMinute*10000 + dwSecond*100 + dwMiniSecond/10;
}

//////////////////////////////////////////////////////////////////////////
UINT GetLog2(UINT nNum)
{
	CHECKF(nNum);
	UINT i = 0;
	for ( i = 0; i < 32; ++i)	//���2����ָ����ȡ��
	{
		nNum = nNum >> 1;
		if (nNum<=0)
		{
			break;
		}
	}
	
	return i;
}

//************************************
// Method:    GetNextUpdateTime
// FullName:  GetNextUpdateTime
// Access:    public 
// Returns:   bool
// Qualifier: ���ڴ���ÿ���0����������
// Parameter: UINT unHour
//************************************
UINT GetNextUpdateTime(UINT unHour)
{
	COleDateTimeSpan tDaySpan(1,0,0,0);
	COleDateTime tCur = COleDateTime::GetCurrentTime();
	COleDateTime tNext(tCur.GetYear(),tCur.GetMonth(),tCur.GetDay(),unHour,0,0);
	if((UINT)tCur.GetHour() >= unHour)
		tNext+=tDaySpan;
	UINT unYear		= tNext.GetYear();
	UINT unMonth	= tNext.GetMonth();
	UINT unDay		= tNext.GetDay();
	UINT unHours	= tNext.GetHour();
	UINT unMinite	= tNext.GetMinute();

	// ������TimeGet�������صķ���ֵһ��
	UINT unNextTime = unYear%100 * 100000000 + unMonth * 1000000 + unDay * 10000 + unHours * 100 + unMinite;	

	return unNextTime;
}

//************************************
// Method:    GetNextUpdateTimeOfWeek
// FullName:  GetNextUpdateTimeOfWeek
// Access:    public 
// Returns:   UINT
// Qualifier: ������һ�ܵ�һ��ʱ��� TIME_MINUTE ��ʽʱ��
// Parameter: UINT unTimeOfDay һ����ʱ����2300��ʾ23�㣬���Ϊ0��ȡ��ǰʱ��
//************************************
UINT GetNextUpdateTimeOfWeek(UINT unTimeOfDay)
{
	CHECKF(unTimeOfDay<2400);
	COleDateTime tCur = COleDateTime::GetCurrentTime();
	COleDateTimeSpan tSpan(7,0,0,0);
	COleDateTime tNext;
	if (unTimeOfDay!=0)
	{
		tCur.SetDateTime(tCur.GetYear(),tCur.GetMonth(),tCur.GetDay(),unTimeOfDay/100,unTimeOfDay%100,0);
	}
	tNext = tCur + tSpan;
	UINT unYear = tNext.GetYear();
	UINT unMonth = tNext.GetMonth();
	UINT unDay = tNext.GetDay();
	UINT unHours = tNext.GetHour();
	UINT unMinute = tNext.GetMinute();
	UINT unNextTime = unYear%100 * 100000000 + unMonth*1000000 + unDay*10000+unHours*100+unMinute;
	
	return unNextTime;
}

////////////////////////////////////////////////////////////////////////
// Description:  ���������������߶εĽ�������(�߶�Ϊ������)
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: int
////////////////////////////////////////////////////////////////////////
int CalcNumberAxisIntersection(int x1, int y1, int x2, int y2)
{
	// ��֤ÿ���߶ε�x��yС
	if (x1 > y1)
		swap(x1, y1);
	if (x2 > y2)
		swap(x2, y2);
	
	// ��֤�߶�1����ʼֵ���߶�2С
	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}
	
	if		(y1 <= x2)	// �޽������
		return 0;
	else if (y1 >= y2)	// �߶�2������
		return y2 - x2;
	else if (y1 >= x2)	// �����߶ν���
		return y1 - x2;
	
	return -1;
}

////////////////////////////////////////////////////////////////////////
// Description:  ת���ַ�����Ansi��Unicode����
// Arguments: ��OUT��������, ����ֵ��ʾת���Ƿ�ɹ�
// Author: ������(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool MyANSIToUnicode(IN const char* pszStr, OUT wchar_t* pwszStr, int nUnicodeStrSize)
{
	CHECKF(pszStr && pwszStr && nUnicodeStrSize > 0);
	const int nUnicodeNeedLen = ::MultiByteToWideChar(CP_ACP, 0, pszStr, -1, NULL, 0);
	CHECKF(nUnicodeNeedLen < nUnicodeStrSize);
	ZeroMemory(pwszStr, nUnicodeStrSize);
	::MultiByteToWideChar(CP_ACP, 0, pszStr, -1, pwszStr, nUnicodeNeedLen);  
	return  true;  
}

////////////////////////////////////////////////////////////////////////
// Description:  ת���ַ��� ��Unicode��Ansi����
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool MyUnicodeToANSI(IN const wchar_t* pwszStr, OUT char* pszStr, int nAnsiStrSize)
{
	CHECKF(pszStr && pwszStr && nAnsiStrSize > 0);	
	const int nAnsiNeedLen = ::WideCharToMultiByte(CP_ACP, 0, pwszStr, -1, NULL, 0, NULL, NULL);
	CHECKF(nAnsiNeedLen < nAnsiStrSize);
	ZeroMemory(pszStr, nAnsiStrSize);
	::WideCharToMultiByte(CP_ACP, 0, pwszStr, -1, pszStr, nAnsiNeedLen, NULL, NULL);
	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  ת���ַ��� ��Unicode��UTF8
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool MyUnicodeToUTF8(IN const wchar_t* pwszStr, OUT char* pszStr, int nUtf8StrSize)
{
	CHECKF(pwszStr && pszStr && nUtf8StrSize > 0);
	const int nNeedLen = ::WideCharToMultiByte(CP_UTF8, NULL, pwszStr, wcslen(pwszStr), NULL, 0, NULL, NULL);
	CHECKF(nNeedLen < nUtf8StrSize);
	ZeroMemory(pszStr, nUtf8StrSize);
	::WideCharToMultiByte(CP_UTF8, NULL, pwszStr, wcslen(pwszStr), pszStr, nNeedLen, NULL, NULL);
	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  ת���ַ��� ��UTF8��Unicode
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool MyUTF8ToUnicode(IN const char* pszStr, OUT wchar_t* pwszStr, int nUnicodeStrSize)
{
	CHECKF(pszStr && pwszStr && nUnicodeStrSize > 0);
	const int nNeedLen = ::MultiByteToWideChar(CP_UTF8, NULL, pszStr, strlen(pszStr), NULL, 0);
	CHECKF(nNeedLen < nUnicodeStrSize);
	ZeroMemory(pwszStr, nUnicodeStrSize);
	::MultiByteToWideChar(CP_UTF8, NULL, pszStr, strlen(pszStr), pwszStr, nUnicodeStrSize);
	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  ����ַ����Ƿ����sql���
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool StrSqlCheck(const char* pszString)
{
	if (!pszString)
		return false;

	wchar_t wszTempStr[2048];
	ZeroMemory(wszTempStr, sizeof(wszTempStr));
	::MyUTF8ToUnicode(pszString, wszTempStr, sizeof(wszTempStr) / sizeof(wchar_t));

	int nLen = wcslen(wszTempStr);
	wchar_t c = L'\0';
	for (int i=0; i < nLen; i++)
	{
		c = wszTempStr[i];
		if(c < L' ')
			return false;
		switch(c)
		{
// 		case L'	'://tab
// 		case L' ':
		case L';':
		case L',':
		case L'/':
		case L'\\':
		case L'=':
		case L'%':
		case L'@':
		case L'\'':
		case L'"':
		case L'[':
		case L']':
		case L'#':
		case L'?':
		case L'*':
		case L'!':
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
int bit_count(int n)
{
	int nCount = 0;
	for (int i = 0; i < 32; i++)
	{
		if ((n & (1<<i)) != 0)
			nCount++;
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////////
UINT GetNextUpdateMonthDate()
{
	COleDateTime tCur = COleDateTime::GetCurrentTime();
	UINT nNextYear = tCur.GetYear();
	UINT nNextMonth = tCur.GetMonth() + 1;
	if (nNextMonth > 12)//�Ƿ񳬹�����Ȼ��
	{
		nNextYear += 1;
		nNextMonth = 1;
	}
	COleDateTime tNext = COleDateTime(nNextYear, nNextMonth, 1, 0, 0, 0);
	CHECKF(tNext.GetStatus() == COleDateTime::valid);
	UINT nNextTime = tNext.GetYear()*10000 + tNext.GetMonth()*100 + tNext.GetDay();//YYYYMMDD
	return nNextTime;
}

////////////////////////////////////////////////////////////////////////
// Description:  ��ȡ��Ʒʣ��ʱ����ʵ�ķ�����, nLifeTimeOffsetΪƫ����, ֧�ִ���0�ķ���ƫ����
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
UINT GetItemLifeTimeMinute(int nLifeTimeOffset)
{
	CHECKF(nLifeTimeOffset > 0);
	CTime time = CTime::GetCurrentTime();
	CTimeSpan timeSpan(0, 0, nLifeTimeOffset, 0);
	CTime timeEnd = time + timeSpan;
	UINT unResult = (timeEnd.GetYear() - 2000) * 100000000 + timeEnd.GetMonth() * 1000000  + timeEnd.GetDay() * 10000+ timeEnd.GetHour() * 100+ timeEnd.GetMinute();
	return unResult;
}

//////////////////////////////////////////////////////////////////////////
bool	SafeCopy(char * pTarget, const char * pSource, int nBufLen /*= 0*/)
{
	DEBUG_TRY
	if(pTarget)
	{
		pTarget[0] = 0;
		if(pSource)
		{
			if(nBufLen && (int)strlen(pSource) >= nBufLen)
			{
				strncpy(pTarget, pSource, nBufLen-1);
				pTarget[nBufLen-1] = 0;
				return false;
			}
			strcpy(pTarget, pSource);
			return true;
		}
	}
	DEBUG_CATCH("CATCH: *** SafeCopy() crash! ***\n");
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool IsObjType(OBJID idObjType, OBJID idUnion) 
{ 
	return (idObjType & idUnion & 0x0FFF) != 0; 
}

////////////////////////////////////////////////////////////////////////
// Description:  double���ͱ�����λС��
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
double SaveTwoDecimal(double dInput)
{
	I64 i64Vaule =  (I64)(100i64 * dInput);
	double dResult = i64Vaule / 100.0;
	return dResult;
}

//////////////////////////////////////////////////////////////////////////
// ��dump
LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	CHECKF(ExceptionInfo);
	MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	ExInfo.ThreadId = ::GetCurrentThreadId();
    ExInfo.ExceptionPointers = ExceptionInfo;
	ExInfo.ClientPointers = TRUE;
	
	COleDateTime dt=COleDateTime::GetCurrentTime();
	CString strDumpFileName;
	CString temp=dt.Format("%Y��%m��%d��%Hʱ%M��%S��");
	strDumpFileName.Format("./dump/%s_%lu.dmp", temp, ::GetTickCount());

	::CreateDirectory("dump", NULL);
	HANDLE lhDumpFile = CreateFile(strDumpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
	if (NULL != lhDumpFile)
	{
		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, MiniDumpNormal/*MiniDumpWithFullMemory*/, &ExInfo, NULL, NULL);		// ��ѡ��:MiniDumpNormal  ����ӡ�ڴ�, С�ܶ�
		CloseHandle(lhDumpFile);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
///////////////////////////////////////////////////////////////////////
// ��TimeGet�ӿڻ��ʱ��ת����UTC��
int GetUTCSecond(int nTimeType, int nTime)
{
	CHECKF(nTimeType > TIME_BEGIN && nTimeType < TIME_END);
	time_t tCurTime = ::time(NULL);
	struct tm *pTime = ::localtime(&tCurTime);
	CHECKF(pTime);
	switch (nTimeType)
	{
	case TIME_MINUTE:
		{
			pTime->tm_sec	= 0;
			pTime->tm_min	= nTime%100;
			pTime->tm_hour	= nTime%10000/100;
			pTime->tm_mday	= nTime%1000000/10000;
			pTime->tm_mon	= nTime%100000000/1000000 - 1;
			pTime->tm_year	= nTime%10000000000/100000000 + 100;
		}
		break;
	case  TIME_DAY:
		{
			pTime->tm_sec	= 0;
			pTime->tm_min	= 0;
			pTime->tm_hour	= 0;
			pTime->tm_mday	= nTime%100;
			pTime->tm_mon	= nTime%10000/100 - 1;
			pTime->tm_year	= nTime/10000;
		}
		break;
	default:
		tolog2("GetUTCSecond need process type:%d",nTimeType);
	}
	int nNeedTime = (int)::mktime(pTime);

	return nNeedTime;
}

////////////////////////////////////////////////////////////////////////
// Description:  ���ļ�����string����
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: std::string
////////////////////////////////////////////////////////////////////////
string LoadFileToString(const string& strFileName)
{
	std::ostringstream buf;
	std::ifstream inFile(strFileName.c_str(), std::ios::in);
	char ch;
	int nCount = 0;
	while(buf && inFile.get(ch))
	{
		if (nCount < 3)
		{
			if (SZ_UTF8_FILE_HEADER[nCount++] == ch)
			{
				continue;
			}
		}
		buf.put(ch);
	}
	inFile.close();
	return buf.str();
}

////////////////////////////////////////////////////////////////////////
// Description:  �滻std::string�е��ַ�
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool ReplaceStdString(IN OUT string& str, const char* pszOld, const char* pszNew, bool bReplaceAll/* = true*/)
{
	CHECKF(pszOld && ::strlen(pszOld) > 0);
	CHECKF(pszNew && ::strlen(pszNew) > 0);
	if (str.empty())
	{
		return false;
	}
	
	std::string::size_type unOldSize = ::strlen(pszOld);
	std::string::size_type unNewSize = ::strlen(pszNew);
	
	std::string::size_type unPos = 0;
	while ((unPos = str.find(pszOld, unPos)) != str.npos)
	{
		str.replace(unPos, unOldSize, pszNew);
		unPos += unNewSize;
		if (!bReplaceAll)
		{
			break;
		}
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////
// Description:  �����Ϣ��Կ
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: unsigned int
////////////////////////////////////////////////////////////////////////
unsigned int GetMsgKey( unsigned int unMsgType, unsigned int unMsgIndex )
{
	__int64 i64MyRand = unMsgType + unMsgIndex;
	i64MyRand *= ((_int64)22695477);
	i64MyRand ++;
	i64MyRand %= 100000007;
	return (unsigned int)i64MyRand;
}


//ͨ��WindowsNT/Win2000�����õ�NetApi32.DLL�Ĺ�����ʵ�ֵġ�����ͨ������NCBENUM����,��ȡ������   
//��Ŀ��ÿ���������ڲ����,Ȼ���ÿ��������ŷ���NCBASTAT�����ȡ��MAC��ַ��   
int MyGetMAC(OUT char* pszMac, int i)     //��NetAPI����ȡ����MAC��ַ   
{       
	NCB ncb;     //����һ��NCB(������ƿ�)���͵Ľṹ�����ncb   
	typedef struct _ASTAT_     //�Զ���һ���ṹ��_ASTAT_   
	{  
		ADAPTER_STATUS   adapt;   
		NAME_BUFFER   NameBuff   [30];       
	}ASTAT, *PASTAT;  
	ASTAT Adapter;     

	typedef struct _LANA_ENUM     //�Զ���һ���ṹ��_ASTAT_   
	{  
		UCHAR length;   
		UCHAR lana[MAX_LANA];     //�������MAC��ַ    
	}LANA_ENUM;       
	LANA_ENUM lana_enum;     

	//   ȡ��������Ϣ�б�        
	UCHAR uRetCode;       
	memset(&ncb, 0, sizeof(ncb));     //���ѿ����ڴ�ռ�ncb ��ֵ����Ϊֵ 0   
	memset(&lana_enum, 0, sizeof(lana_enum));     //���һ���ṹ���͵ı���lana_enum����ֵΪ0   
	//�Խṹ�����ncb��ֵ   
	ncb.ncb_command = NCBENUM;     //ͳ��ϵͳ������������   
	ncb.ncb_buffer = (unsigned char *)&lana_enum; //ncb_buffer��Աָ����LANA_ENUM�ṹ���Ļ�����   
	ncb.ncb_length = sizeof(LANA_ENUM);     
	//����������NCBENUM����Ի�ȡ��ǰ������������Ϣ�����ж��ٸ�������ÿ�������ı�ţ�MAC��ַ��    
	uRetCode = Netbios(&ncb); //����netbois(ncb)��ȡ�������к�       
	if(uRetCode != NRC_GOODRET)       
		return uRetCode;       

	//��ÿһ�������������������Ϊ�����ţ���ȡ��MAC��ַ      
	for(int lana=0; lana<lana_enum.length; lana++)       
	{  
		ncb.ncb_command = NCBRESET;   //����������NCBRESET������г�ʼ��   
		ncb.ncb_lana_num = lana_enum.lana[lana];   
		uRetCode = Netbios(&ncb);     
	}   
	if(uRetCode != NRC_GOODRET)  
		return uRetCode;       

	//   ׼��ȡ�ýӿڿ���״̬��,ȡ��MAC��ַ   
	memset(&ncb, 0, sizeof(ncb));   
	ncb.ncb_command = NCBASTAT;    //����������NCBSTAT�����ȡ������Ϣ   
	ncb.ncb_lana_num = lana_enum.lana[i];     //ָ�������ţ��������ָ����һ��������ͨ��Ϊ��������    
	strcpy((char*)ncb.ncb_callname, "*");     //Զ��ϵͳ����ֵΪ*   
	ncb.ncb_buffer = (unsigned char *)&Adapter; //ָ�����ص���Ϣ��ŵı���   
	ncb.ncb_length = sizeof(Adapter);  
	//���ŷ���NCBASTAT�����Ի�ȡ��������Ϣ   
	uRetCode = Netbios(&ncb);   
	//   ȡ����������Ϣ����������������������Ļ������ر�׼��ð�ŷָ���ʽ��      
	if(uRetCode != NRC_GOODRET)     
		return uRetCode;     
	//������MAC��ַ��ʽת��Ϊ���õ�16������ʽ,������ַ���mac��    
	//	sprintf(pszMac,"%02X-%02X-%02X-%02X-%02X-%02X",       
	sprintf(pszMac,"%02X%02X%02X%02X%02X%02X",    
		Adapter.adapt.adapter_address[0],       
		Adapter.adapt.adapter_address[1],       
		Adapter.adapt.adapter_address[2],       
		Adapter.adapt.adapter_address[3],       
		Adapter.adapt.adapter_address[4],       
		Adapter.adapt.adapter_address[5]   
	);   
	return 0;     
}  

//////////////////////////////////////////////////////////////////////////
CMxyString MyUtf8ToANSI( const char* pszString )
{
	CMxyString strResult = "";
	wchar_t wszTempStr[8192];
	char	szAnsiStr[8192];
	if (::MyUTF8ToUnicode(pszString, wszTempStr, sizeof(wszTempStr) / sizeof(wchar_t)) && 
		::MyUnicodeToANSI(wszTempStr, szAnsiStr, sizeof(szAnsiStr) / sizeof(szAnsiStr[0])))
	{
		strResult = szAnsiStr;
	}
	return strResult;
}

//////////////////////////////////////////////////////////////////////////
CMxyString MyANSIToUtf8( const char* pszString )
{
	CMxyString strResult = "";
	wchar_t wszTempStr[8192];
	char	szAnsiStr[8192];
	if (::MyANSIToUnicode(pszString, wszTempStr, sizeof(wszTempStr) / sizeof(wchar_t)) && 
		::MyUnicodeToUTF8(wszTempStr, szAnsiStr, sizeof(szAnsiStr) / sizeof(szAnsiStr[0])))
	{
		strResult = szAnsiStr;
	}
	return strResult;
}

////////////////////////////////////////////////////////////////////////
// Description: �ж��ļ�������
// Arguments:
// Author: ������(Peng Wenqi)
// Return Value: bool
////////////////////////////////////////////////////////////////////////
bool IsExistFile( const char* pszFileName )
{
	CHECKF(pszFileName);
	if (FILE_EXISTS == FILE_ACCESS(pszFileName, EXISTENCE_ONLY))
	{
		return true;
	}
	return false;
}
