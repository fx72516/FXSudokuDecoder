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

typedef char BYTE;

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
