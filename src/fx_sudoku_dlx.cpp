#include "stdafx.h"
#include "fx_sudoku_i.h"
#include "fx_sudoku_dlx.h"
#include "fx_dancing_links_x.h"



FXSudokuDlx::FXSudokuDlx()
	: FXSudokuBase()
{
}

FXSudokuDlx::~FXSudokuDlx()
{
}

bool FXSudokuDlx::Decode()
{
	const ULONG DLC_COLUMN_COUNT1	= MAX_CELL_COUNT * MAX_GRID_COUNT * MAX_CELL_COUNT * MAX_GRID_COUNT;		// ÐÐ*ÁÐ
	const ULONG DLC_COLUMN_COUNT2	= DLC_COLUMN_COUNT1 + MAX_CELL_COUNT * MAX_GRID_COUNT * MAX_NUMBER;			// ÐÐ*Êý×Ö
	const ULONG DLC_COLUMN_COUNT3	= DLC_COLUMN_COUNT2 + MAX_CELL_COUNT * MAX_GRID_COUNT * MAX_NUMBER;			// ÁÐ*Êý×Ö
	const ULONG DLC_COLUMN_COUNT	= DLC_COLUMN_COUNT3 + MAX_GRID_COUNT * MAX_GRID_COUNT * MAX_NUMBER;			// ¹¬*Êý×Ö

	FXDancingLinksX dlx(DLC_COLUMN_COUNT);

	std::vector<ULONG> nums;
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
// 			ULONG row = i;
// 			ULONG col = j;
			ULONG grid = (i / MAX_CELL_COUNT) + (j / MAX_CELL_COUNT) * MAX_GRID_COUNT;
			if (BYTE num = GetCellNumber(i, j))
			{
				nums.clear();
				nums.push_back(i * MAX_CELL_COUNT * MAX_GRID_COUNT + j);
				nums.push_back(DLC_COLUMN_COUNT1 + i * MAX_CELL_COUNT * MAX_GRID_COUNT + num - 1);
				nums.push_back(DLC_COLUMN_COUNT2 + j * MAX_CELL_COUNT * MAX_GRID_COUNT + num - 1);
				nums.push_back(DLC_COLUMN_COUNT3 + grid * MAX_CELL_COUNT * MAX_GRID_COUNT + num - 1);
				dlx.AddRow(nums);
			}
			else
			{
				for (BYTE num = 1; num <= MAX_NUMBER; ++num)
				{
					nums.clear();
					nums.push_back(i * MAX_CELL_COUNT * MAX_GRID_COUNT + j);
					nums.push_back(DLC_COLUMN_COUNT1 + i * MAX_CELL_COUNT * MAX_GRID_COUNT + num - 1);
					nums.push_back(DLC_COLUMN_COUNT2 + j * MAX_CELL_COUNT * MAX_GRID_COUNT + num - 1);
					nums.push_back(DLC_COLUMN_COUNT3 + grid * MAX_CELL_COUNT * MAX_GRID_COUNT + num - 1);
					dlx.AddRow(nums);
				}
			}
		}
	}

// 	std::ofstream ofs("out.txt");
// 	dlx.Print(ofs);
// 	ofs.close();

	bool result = dlx.Execute();

	nums = dlx.GetTakenRow();
	ASSERT(nums.size() == 81);

	for ()

	return result;
}
