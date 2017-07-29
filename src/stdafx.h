// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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

// TODO: 在此处引用程序需要的其他头文件
