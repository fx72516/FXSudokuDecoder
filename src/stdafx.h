// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <assert.h>

typedef char BYTE;

const static BYTE MAX_CELL_COUNT = 3;
const static BYTE MAX_GRID_COUNT = 3;
const static BYTE MAX_NUMBER = MAX_CELL_COUNT * MAX_GRID_COUNT;

#define CHECK_RET_FALSE(number) \
	if ((number) <= 0 || (number) > MAX_NUMBER) \
		return false;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
