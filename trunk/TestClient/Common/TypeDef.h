////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle: TypeDef.h
// Author: ÅíÎÄÆæ(Peng Wenqi)
// Created: 2012-10-23
////////////////////////////////////////////////////////////////////////
#ifndef	_COMMON_TYPEDEF_H_
#define _COMMON_TYPEDEF_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

struct LEV_COUNT
{
	int		nAll;
	int		n7Day;
	int		n3Day;
	int		n1Day;
};

typedef std::map<int, LEV_COUNT>				MAP_LEV_COUNT;
typedef MAP_LEV_COUNT::iterator					ITER_MAP_LEV_COUNT;
typedef MAP_LEV_COUNT::const_iterator			CITER_MAP_LEV_COUNT;

#endif // end of _COMMON_TYPEDEF_H_