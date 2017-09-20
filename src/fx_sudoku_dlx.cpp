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

	if (!dlx.Execute())
		return false;

	ASSERT(dlx.GetTakenRow().size() == DLC_COLUMN_COUNT1);

	for (size_t c = 0; c < DLC_COLUMN_COUNT1; ++c)
	{
		nums = dlx.GetTakenRowData(c);

#ifdef _DEBUG
		BYTE row = UNUSEFULL_BYTE;
		BYTE col = UNUSEFULL_BYTE;
		BYTE num = UNUSEFULL_BYTE;
		BYTE grid = UNUSEFULL_BYTE;
		for (ULONG i = 0; i < nums.size(); ++i)
		{
			BYTE _row = UNUSEFULL_BYTE;
			BYTE _col = UNUSEFULL_BYTE;
			BYTE _num = UNUSEFULL_BYTE;
			if (nums[i] < DLC_COLUMN_COUNT1)
			{
				_row = (nums[i] % DLC_COLUMN_COUNT1) / (MAX_CELL_COUNT * MAX_GRID_COUNT);
				_col = (nums[i] % DLC_COLUMN_COUNT1) % (MAX_CELL_COUNT * MAX_GRID_COUNT);
			}
			else if (nums[i] < DLC_COLUMN_COUNT2)
			{
				_row = ((nums[i] - DLC_COLUMN_COUNT1) % (DLC_COLUMN_COUNT2 - DLC_COLUMN_COUNT1)) / (MAX_CELL_COUNT * MAX_GRID_COUNT);
				_num = ((nums[i] - DLC_COLUMN_COUNT1) % (DLC_COLUMN_COUNT2 - DLC_COLUMN_COUNT1)) % (MAX_CELL_COUNT * MAX_GRID_COUNT);
			}
			else if (nums[i] < DLC_COLUMN_COUNT3)
			{
				_col = ((nums[i] - DLC_COLUMN_COUNT2) % (DLC_COLUMN_COUNT3 - DLC_COLUMN_COUNT2)) / (MAX_CELL_COUNT * MAX_GRID_COUNT);
				_num = ((nums[i] - DLC_COLUMN_COUNT2) % (DLC_COLUMN_COUNT3 - DLC_COLUMN_COUNT2)) % (MAX_CELL_COUNT * MAX_GRID_COUNT);
			}
			else
			{
				ASSERT(nums[i] < DLC_COLUMN_COUNT);
				grid = (BYTE)(((nums[i] - DLC_COLUMN_COUNT3)) / (MAX_CELL_COUNT * MAX_GRID_COUNT));
				_num = (((nums[i] - DLC_COLUMN_COUNT3)) % (MAX_CELL_COUNT * MAX_GRID_COUNT));
			}

			if (_row != UNUSEFULL_BYTE)
			{
				if (row == UNUSEFULL_BYTE)
					row = _row;
				else
					ASSERT(row == _row);
			}

			if (_col != UNUSEFULL_BYTE)
			{
				if (col == UNUSEFULL_BYTE)
					col = _col;
				else
					ASSERT(col == _col);
			}

			if (_num != UNUSEFULL_BYTE)
			{
				if (num == UNUSEFULL_BYTE)
					num = _num;
				else
					ASSERT(num == _num);
			}
		}
		ASSERT(row != UNUSEFULL_BYTE && col != UNUSEFULL_BYTE && num != UNUSEFULL_BYTE && grid != UNUSEFULL_BYTE);
		ASSERT(grid == (row / MAX_CELL_COUNT) + (col / MAX_CELL_COUNT) * MAX_GRID_COUNT);
#else
		std::sort(nums.begin(), nums.end());
		ASSERT(nums[0] < nums[1] && nums[1] < nums[2] && nums[2] < nums[3]);
		BYTE row = (nums[0] % DLC_COLUMN_COUNT1) / (MAX_CELL_COUNT * MAX_GRID_COUNT);
		BYTE col = (nums[0] % DLC_COLUMN_COUNT1) % (MAX_CELL_COUNT * MAX_GRID_COUNT);
		BYTE num = ((nums[1] - DLC_COLUMN_COUNT1) % (DLC_COLUMN_COUNT2 - DLC_COLUMN_COUNT1)) % (MAX_CELL_COUNT * MAX_GRID_COUNT);;
#endif // _DEBUG

		if (!GetCellNumber(row, col))
			SetCellNumber(row, col, num + 1);
		else
			ASSERT((row = GetCellNumber(row, col)) == num + 1);
	}

	ASSERT(IsFinished());
	return IsFinished();
}
