// 通用工具
// 仙剑修，2002.9.14
//////////////////////////////////////////////////////////////////////

#ifndef	COMMON_H
#define	COMMON_H
#pragma warning(disable:4786)
#pragma warning(disable:4996)

//#include <windows.h>
#undef _WINDOWS_
#include "afx.h"

#include <time.h>
#include "logfile.h"
#include "iostream"
#include "./TypeDef.h"
#include "SymEngine.h"
using namespace std;

typedef __int64 I64;
typedef unsigned long OBJID;

//////////////////////////////////////////////////////////////////////////
#define		STACK_TRACE			{ try{char szBuf[4096] = {0}; CSymEngine sym; sym.StackTrace(szBuf);	::LogStack(__FILE__, __LINE__,"!!! STACK_TRACE !!!\n%s", szBuf);}catch (...){::LogSave("STACK_TRACE crash!");}}
#define		STACK_TRACE_ONCE	{ try{static bool bFirstTime = true; if(bFirstTime) {bFirstTime = false; char szBuf[4096] = {0}; CSymEngine sym; sym.StackTrace(szBuf);	::LogStack(__FILE__, __LINE__, "!!! STACK_TRACE_ONCE !!!\n%s", szBuf);}}catch (...){::LogSave("STACK_TRACE_ONCE crash!");}}
#define		DUMP_TRACE			{::MyUnhandledExceptionFilter(ex.m_pException);}
#define		DUMP_TRACE_ONCE		{static bool bFirstTime = true; if (bFirstTime){ bFirstTime = false; ::MyUnhandledExceptionFilter(ex.m_pException);}}

//////////////////////////////////////////////////////////////////////////
#define SAFE_DELETE(ptr) { if(ptr){	try{ delete ptr; }catch(...){ LOGERROR("CATCH: *** SAFE_DELETE() crash! ***\n");ptr = 0;ASSERT(ptr); } ptr = 0; } }
#define SAFE_RELEASEBYOWNER(ptr) { if(ptr){ try{ ptr->ReleaseByOwner(); }catch(...){ LOGERROR("CATCH: *** SAFE_RELEASE() crash! ***\n");ptr = 0;ASSERT(ptr); } ptr = 0; } }
#define SAFE_RELEASE(ptr) { if(ptr){ try{ ptr->Release(); }catch(...){ LOGERROR("CATCH: *** SAFE_RELEASE() crash! ***");ptr = 0;ASSERT(ptr); } ptr = 0;} }

#include <assert.h>
#include "logfile.h"
inline void	log_assert(const char* type, const char* str, const char* file, int line) { LOGERROR("%s(%u):** %s(%s) **",file,line,type,str);}

#define		CHECK_NOLOG(x)		do{ if(!(x))	{ return; } }while(0)
#define		CHECKF_NOLOG(x)		do{ if(!(x))	{ return 0; } }while(0)
#define		CHECKF1_NOLOG(x)	do{ if(!(x))	{ return 1; } }while(0)
#define		CHECKC_NOLOG(x)		if(!(x))		{ continue; }
#define		CHECKBK_NOLOG(x)	if(!(x))		{ break; }
#define		CHECKB_NOLOG(x)		if(!(x))		{ }

#define		CHECK(x)	do{ if(!(x))	{ STACK_TRACE_ONCE; log_assert("CHECK", #x, __FILE__, __LINE__); return; } }while(0)
#define		CHECKF(x)	do{ if(!(x))	{ STACK_TRACE_ONCE; log_assert("CHECKF", #x, __FILE__, __LINE__); return 0; } }while(0)
#define		CHECKF1(x)	do{ if(!(x))	{ STACK_TRACE_ONCE; log_assert("CHECKF", #x, __FILE__, __LINE__); return 1; } }while(0)
#define		CHECKC(x)	if(!(x))		{ STACK_TRACE_ONCE; log_assert("CHECKC", #x, __FILE__, __LINE__); continue; }
#define		CHECKBK(x)	if(!(x))		{ STACK_TRACE_ONCE; log_assert("CHECKBK", #x, __FILE__, __LINE__); break; }
#define		CHECKB(x)	if(!(x))		{ STACK_TRACE_ONCE; log_assert("CHECKB", #x, __FILE__, __LINE__); }
#define		PURE_VIRTUAL_FUNCTION_0		{ STACK_TRACE_ONCE; log_assert("ASSERT", "PURE_VIRTUAL_FUNCTION_0", __FILE__, __LINE__); return 0; }
#define		PURE_VIRTUAL_FUNCTION_1		{ STACK_TRACE_ONCE; log_assert("ASSERT", "PURE_VIRTUAL_FUNCTION_1", __FILE__, __LINE__); return 1; }
#define		PURE_VIRTUAL_FUNCTION		{ STACK_TRACE_ONCE; log_assert("ASSERT", "PURE_VIRTUAL_FUNCTION", __FILE__, __LINE__);}
#define		PURE_VIRTUAL_FUNCTION_(x)	{ STACK_TRACE_ONCE; log_assert("ASSERT", "PURE_VIRTUAL_FUNCTION_x", __FILE__, __LINE__); return x(); }

// 注意, 下面的宏是不会出堆栈的, 所以呢, 要用就想好了, 怎么看日志, 是否需要出堆栈
#undef		ASSERT
#define		ASSERT(x)	(void)( (x) || (log_assert("ASSERT", #x, __FILE__, __LINE__), 0) )
#define		IF_NOT(x)	if( !(x) ? ( log_assert("IF_NOT", #x, __FILE__, __LINE__), 1 ) : 0 )
#define		IF_OK(x)	if( (x) ? 1 : ( log_assert("IF_OK", #x, __FILE__, __LINE__), 0 ) )
#define		CHKG(a,b)	if(!(a>b)){ LogSave2(__FILE__,__LINE__,"CHKG(%s:%d>%s:%d)",#a,a,#b,b);	 return;}
#define		CHKGZ(a)	if(!(a>0)){ LogSave2(__FILE__,__LINE__,"CHKGZ(%s:%d>0)",#a,a);	 return;}
#define		CHKFG(a,b)	if(!(a>b)){	 LogSave2(__FILE__,__LINE__,"CHKFG(%s:%d>%s:%d)",#a,a,#b,b); return 0;}
#define		CHKFGZ(a)	if(!(a>0)){	 LogSave2(__FILE__,__LINE__,"CHKFGZ(%s:%d>0)",#a,a); return 0;}
#define		CHKNG(a,b)	if(!(a<=b)){ LogSave2(__FILE__,__LINE__,"CHKNG(%s:%d<=%s:%d)",#a,a,#b,b);  return;}
#define		CHKNGZ(a)	if(!(a<=0)){ LogSave2(__FILE__,__LINE__,"CHKNGZ(%s:%d<=0)",#a,a);  return;}
#define		CHKFNG(a,b)	if(!(a<=b)){ LogSave2(__FILE__,__LINE__,"CHKFNG(%s:%d<=%s:%d)",#a,a,#b,b); return 0;}
#define		CHKFNGZ(a)	if(!(a<=0)){ LogSave2(__FILE__,__LINE__,"CHKFNGZ(%s:%d<=0)",#a,a); return 0;}
#define		CHKL(a,b)	if(!(a<b)){	 LogSave2(__FILE__,__LINE__,"CHKL(%s:%d<%s:%d)",#a,a,#b,b);  return;}
#define		CHKLZ(a)	if(!(a<0)){	 LogSave2(__FILE__,__LINE__,"CHKLZ(%s:%d<0)",#a,a);  return;}
#define		CHKFL(a,b)	if(!(a<b)){	 LogSave2(__FILE__,__LINE__,"CHKFL(%s:%d<%s:%d)",#a,a,#b,b); return 0;}
#define		CHKFLZ(a)	if(!(a<0)){	 LogSave2(__FILE__,__LINE__,"CHKFLZ(%s:%d<0)",#a,a); return 0;}
#define		CHKNL(a,b)	if(!(a>=b)){ LogSave2(__FILE__,__LINE__,"CHKNL(%s:%d>=%s:%d)",#a,a,#b,b);  return;}
#define		CHKNLZ(a)	if(!(a>=0)){ LogSave2(__FILE__,__LINE__,"CHKNLZ(%s:%d>=0)",#a,a);  return;}
#define		CHKFNL(a,b)	if(!(a>=b)){ LogSave2(__FILE__,__LINE__,"CHKFNL(%s:%d>=%s:%d)",#a,a,#b,b); return 0;}
#define		CHKFNLZ(a)	if(!(a>=0)){ LogSave2(__FILE__,__LINE__,"CHKFNLZ(%s:%d>=0)",#a,a); return 0;}
#define		CHKE(a,b)	if(!(a==b)){ LogSave2(__FILE__,__LINE__,"CHKE(%s:%d==%s:%d)",#a,a,#b,b);   return;}
#define		CHKEZ(a)	if(!(a==0)){ LogSave2(__FILE__,__LINE__,"CHKEZ(%s:%d==0)",#a,a);   return;}
#define		CHKFE(a,b)	if(!(a==b)){ LogSave2(__FILE__,__LINE__,"CHKFE(%s:%d==%s:%d)",#a,a,#b,b);  return 0;}
#define		CHKFEZ(a)	if(!(a==0)){ LogSave2(__FILE__,__LINE__,"CHKFEZ(%s:%d==0)",#a,a);  return 0;}
#define		CHKNE(a,b)	if(!(a!=b)){ LogSave2(__FILE__,__LINE__,"CHKNE(%s:%d!=%s:%d)",#a,a,#b,b);  return;}
#define		CHKNEZ(a)	if(!(a!=0)){ LogSave2(__FILE__,__LINE__,"CHKNEZ(%s:%d!=0)",#a,a);  return;}
#define		CHKFNE(a,b)	if(!(a!=b)){ LogSave2(__FILE__,__LINE__,"CHKFNE(%s:%d!=%s:%d)",#a,a,#b,b); return 0;}
#define		CHKFNEZ(a)	if(!(a!=0)){ LogSave2(__FILE__,__LINE__,"CHKFNEZ(%s:%d!=0)",#a,a); return 0;}
#define		CHKBW(a,b,c)if(!(a >= b && a <= c)){ LogSave2(__FILE__,__LINE__,"CHKBW(%d<=%s:%d<=%d)",b,#a,a,c);  return; }
#define		CHKFBW(a,b,c)if(!(a >= b && a <= c)){ LogSave2(__FILE__,__LINE__,"CHKFBW(%d<=%s:%d<=%d)",b,#a,a,c);  return 0; }

#define		DEBUG_TRY		try{
#define		DEBUG_CATCH(x)					}catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCH crash:"x)); }
#define		DEBUG_CATCH2(x,y)				}catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCH crash:"x),(y)); }
#define		DEBUG_CATCH3(x,y,z)				}catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCH crash:"x),(y),(z)); }
#define		DEBUG_CATCH4(x,y,z,u)			}catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCH crash:"x),(y),(z),(u)); }
#define		DEBUG_CATCH5(x,y,z,u,t)			}catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCH crash:"x),(y),(z),(u),(t)); }
#define		DEBUG_CATCH6(x,y,z,u,t,l)		}catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCH crash:"x),(y),(z),(u),(t),(l)); }

#define		DEBUG_CATCHF(x)					} catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCHF crash:"x));						return false; }
#define		DEBUG_CATCHF2(x,y)				} catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCHF crash:"x),(y));					return false; }
#define		DEBUG_CATCHF3(x,y,z)			} catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCHF crash:"x),(y),(z));				return false; }
#define		DEBUG_CATCHF4(x,y,z,u)			} catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCHF crash:"x),(y),(z),(u));			return false; }
#define		DEBUG_CATCHF5(x,y,z,u,t)		} catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCHF crash:"x),(y),(z),(u),(t));		return false; }
#define		DEBUG_CATCHF6(x,y,z,u,t,l)		} catch(...){STACK_TRACE_ONCE; ::LogSave(("DEBUG_CATCHF crash:"x),(y),(z),(u),(t),(l));	return false; }

// char szFuncDump[1024]; DumpFuncAddress(6, szFuncDump, ex.m_pException);
#define 	VA_TRY						try{
// #define		VA_CATCH(a)					}catch(CSEHException& ex){/*WheatyExceptionReport::GenerateExceptionString(ex);*/DUMP_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a);}
// #define		VA_CATCH2(a,b)				}catch(CSEHException& ex){/*WheatyExceptionReport::GenerateExceptionString(ex);*/DUMP_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b);}
// #define		VA_CATCH3(a,b,c)			}catch(CSEHException& ex){/*WheatyExceptionReport::GenerateExceptionString(ex);*/DUMP_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c);}
// #define		VA_CATCH4(a,b,c,d)			}catch(CSEHException& ex){/*WheatyExceptionReport::GenerateExceptionString(ex);*/DUMP_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d);}
// #define		VA_CATCH5(a,b,c,d,e)		}catch(CSEHException& ex){/*WheatyExceptionReport::GenerateExceptionString(ex);*/DUMP_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d,e);}
// #define		VA_CATCH6(a,b,c,d,e,f)		}catch(CSEHException& ex){/*WheatyExceptionReport::GenerateExceptionString(ex);*/DUMP_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d,e,f);}
// #define		VA_CATCH7(a,b,c,d,e,f,g)	}catch(CSEHException& ex){/*WheatyExceptionReport::GenerateExceptionString(ex);*/DUMP_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d,e,f,g);}

#define		VA_CATCH(a)					}catch(...){STACK_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a);}
#define		VA_CATCH2(a,b)				}catch(...){STACK_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b);}
#define		VA_CATCH3(a,b,c)			}catch(...){STACK_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c);}
#define		VA_CATCH4(a,b,c,d)			}catch(...){STACK_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d);}
#define		VA_CATCH5(a,b,c,d,e)		}catch(...){STACK_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d,e);}
#define		VA_CATCH6(a,b,c,d,e,f)		}catch(...){STACK_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d,e,f);}
#define		VA_CATCH7(a,b,c,d,e,f,g)	}catch(...){STACK_TRACE_ONCE;LogSave2(__FILE__, __LINE__,"VA_CATCH crash:"a,b,c,d,e,f,g);}
	
#define		DEAD_LOOP_BREAK(x,n)	{ if(++(x) > (n)){ ASSERT(!"DEAD_LOCK_BREAK"); break; } }
#define		TYPENAME	class

// 编译提示宏
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define PRAGMAMSG(x) __FILE__ "(" STRING(__LINE__) ") : " #x " ##########"

#ifdef _WIN32
#include <io.h>
#if ( _MSC_VER >= 1400 )
#define FILE_ACCESS(path,mode)		_access(path,mode)
#else
#define FILE_ACCESS(path,mode)		 access(path,mode)
#endif	/* _MSC_VER >= 1400 */
#elif
#define FILE_ACCESS(path,mode)		 access(path,mode)
};
#endif
#define FILE_EXISTS		0x00
#define EXISTENCE_ONLY	0x00

#endif // COMMON_H