// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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



#ifdef _DEBUG
//#define TEST
#define ASSERT(exp)	\
	if (!(exp))		\
		__asm { int 3 }
#else
#define ASSERT(exp)	0
#endif

typedef char BYTE;
typedef unsigned long ULONG;
const static ULONG UNUSEFULL = 0xffffffff;

const static BYTE MAX_CELL_COUNT = 3;
const static BYTE MAX_GRID_COUNT = MAX_CELL_COUNT;
const static BYTE MAX_NUMBER = MAX_CELL_COUNT * MAX_GRID_COUNT;

// TODO ...
#define GET_CELL_AND_CHECK_CONTINUE(idxX, idxY, pCell)	\
	FXCell * (pCell) = GetCell((idxX), (idxY));			\
	assert(pCell);										\
	if ((pCell)->HasNumber())							\
		continue;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
