#include "stdafx.h"
#include "fx_grid.h"
#include "fx_cell.h"


FXGrid::FXGrid(const BYTE & idxX, const BYTE & idxY)
	: m_indexX(idxX), m_indexY(idxY)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			m_cells[i][j] = new FXCell(m_indexX * MAX_CELL_COUNT + i, m_indexY * MAX_CELL_COUNT + j);
		}
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
	if (idxX < 0 || idxX >= MAX_CELL_COUNT || idxY < 0 || idxY >= MAX_CELL_COUNT)
		return nullptr;
	return m_cells[idxX][idxY];
}

bool FXGrid::GetOutNumbers(std::unordered_set<BYTE>& outNumSet)
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

bool FXGrid::RemoveCellMaybeNumbers(const BYTE & number)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);
			if (pCell->RemoveMaybeNumber(number))
				return true;
		}
	}
	return false;
}

BYTE FXGrid::CheckCellMaybeNumberOnlyOne(BYTE * idxX, BYTE * idxY)
{
	*idxX = 0;
	*idxY = 0;

	BYTE maybeNumberCounts[MAX_NUMBER] = {0};

	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);

			std::unordered_set<BYTE> * maybeNumbers = pCell->GetMaybeNumbers();
			for each (BYTE number in *maybeNumbers)
			{
				++maybeNumberCounts[--number];
			}
		}
	}

	BYTE num = 0;
	for (BYTE i = 0; i < MAX_NUMBER; ++i)
	{
		if (maybeNumberCounts[num] == 1)
		{
			num = i + 1;
			break;
		}
	}

	if (num == 0)
		return 0;

	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);

			std::unordered_set<BYTE> * maybeNumbers = pCell->GetMaybeNumbers();
			if (maybeNumbers->find(num) != maybeNumbers->cend())
			{
				pCell->SetNumber(num);
				*idxX = i;
				*idxY = j;
				return num;
			}
		}
	}

	assert(num);
	return 0;
}

void FXGrid::GetAllNoNumCells(std::vector<FXCell*> & pCells)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);
			pCells.push_back(pCell);
		}
	}
}

BYTE FXGrid::GetIndexX()
{
	return m_indexX;
}

BYTE FXGrid::GetIndexY()
{
	return m_indexY;
}

#ifdef _DEBUG
bool FXGrid::CheckCellNumber(const BYTE & number)
{
	BYTE numCount = 0;
	for (BYTE i = 0; i < MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);

			if (pCell->GetNumber() == number)
				++numCount;
		}
	}
	return numCount <= 1;
}
#endif // _DEBUG
