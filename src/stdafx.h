// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <algorithm>



#ifdef _DEBUG
//#define TEST
#define ASSERT(exp)	\
	if (!(exp))		\
		__asm { int 3 }
#else
#define ASSERT(exp)	0
#endif

#define PURE			= 0

typedef unsigned char BYTE;
typedef unsigned long ULONG;
const static BYTE	UNUSEFULL_BYTE	= 0xff;
const static ULONG	UNUSEFULL_ULONG	= 0xffffffff;

const static BYTE MAX_CELL_COUNT	= 3;
const static BYTE MAX_GRID_COUNT	= MAX_CELL_COUNT;
const static BYTE MAX_NUMBER		= MAX_CELL_COUNT * MAX_GRID_COUNT;

// TODO ...
#define GET_CELL_AND_CHECK_CONTINUE(idxX, idxY, pCell)	\
	FXCell * (pCell) = GetCell((idxX), (idxY));			\
	assert(pCell);										\
	if ((pCell)->HasNumber())							\
		continue;

#define GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid)						\
	FXGrid * (pGrid) = GetGrid((idxX) / MAX_CELL_COUNT, (idxY) / MAX_CELL_COUNT);	\
	assert(pGrid);

#define CHECK_BOOL_RET(r)	if (!(r))	{ goto FX_EXIT; }

// TODO: 在此处引用程序需要的其他头文件
