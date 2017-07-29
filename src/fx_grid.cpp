#include "stdafx.h"
#include "fx_grid.h"
#include "fx_cell.h"

FXGrid::FXGrid()
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
			m_cells[i][j] = new FXCell();
	}
}


FXGrid::~FXGrid()
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			delete m_cells[i][j];
			m_cells[i][j] = nullptr;
		}
	}
}

FXCell * FXGrid::GetCell(const BYTE & idxX, const BYTE & idxY)
{
	if (idxX >= 0 && idxX < MAX_CELL_COUNT && idxY >= 0 && idxY < MAX_CELL_COUNT)
		return m_cells[idxX][idxY];
	return nullptr;
}

bool FXGrid::GetOutNumber(std::unordered_set<BYTE> & outNumSet)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);
			if (BYTE number = pCell->GetNumber())
				outNumSet.insert(number);
		}
	}

	return true;
}

bool FXGrid::IsAllFinished()
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);
			if (!pCell->GetNumber())
				return false;
		}
	}

	return true;
}

BYTE FXGrid::CheckCellMaybeNumber(const BYTE & number, BYTE * idxX, BYTE * idxY)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);
			if (BYTE num = pCell->RemoveMaybe(number))
			{
				*idxX = i;
				*idxY = j;
				return num;
			}
		}
	}
	return 0;
}
