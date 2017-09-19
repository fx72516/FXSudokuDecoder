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
	FXDancingLinksX dlx(324);

	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			if (BYTE num = GetCellNumber(i, j))
			{
				
			}
			else
			{
				
			}
		}
	}

}
