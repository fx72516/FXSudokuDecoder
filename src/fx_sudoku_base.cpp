#include "stdafx.h"
#include "fx_sudoku_base.h"
#include "fx_grid.h"
#include "fx_cell.h"

FXSudokuBase::FXSudokuBase()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT; ++j)
		{
			m_grids[i][j] = new FXGrid(i, j);
		}
	}
}


FXSudokuBase::~FXSudokuBase()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT; ++j)
		{
			delete m_grids[i][j];
			m_grids[i][j] = nullptr;
		}
	}
}

FXGrid * FXSudokuBase::GetGrid(const BYTE & idxX, const BYTE & idxY)
{
	if (idxX < 0 || idxX >= MAX_GRID_COUNT || idxY < 0 || idxY >= MAX_GRID_COUNT)
		return nullptr;
	return m_grids[idxX][idxY];
}

FXCell * FXSudokuBase::GetCell(const BYTE & idxX, const BYTE & idxY)
{
	if (idxX < 0 || idxX >= MAX_GRID_COUNT * MAX_CELL_COUNT || idxY < 0 || idxY >= MAX_GRID_COUNT * MAX_CELL_COUNT)
		return nullptr;

	GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid);
	return pGrid->GetCell(idxX % MAX_CELL_COUNT, idxY % MAX_CELL_COUNT);
}

void FXSudokuBase::SetCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number)
{
	FXCell * pCell = GetCell(idxX, idxY);
	assert(pCell);

	assert(CheckCellNumber(idxX, idxY, number));

	pCell->SetNumber(number);
}

#ifdef _DEBUG
bool FXSudokuBase::CheckCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & _num)
{
	BYTE number = _num;
	if (number == 0)
	{
		FXCell * pCell = GetCell((idxX), (idxY));
		assert(pCell);
		if (!pCell->HasNumber())
			return false;
		number = pCell->GetNumber();
	}

	bool result = false;

	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		if (idxX != i)
			CHECK_BOOL_RET(GetCellNumber(i, idxY) != number);
	}

	for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
	{
		if (idxY != j)
			CHECK_BOOL_RET(GetCellNumber(idxX, j) != number);
	}

	GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid);
	result = pGrid->CheckCellNumber(number);

FX_EXIT:
	if (!result)
		Print();
	return result;
}
#endif // _DEBUG

BYTE FXSudokuBase::GetCellNumber(const BYTE & idxX, const BYTE & idxY)
{
	FXCell * pCell = GetCell(idxX, idxY);
	assert(pCell);
	return pCell->GetNumber();
}

void FXSudokuBase::Print()
{
#ifdef _DEBUG
	Print(std::cout);
#endif
}

void FXSudokuBase::Print(std::ostream & os)
{
	os << std::endl;
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			os << (int)GetCellNumber(i, j) << "  ";

			if ((j + 1) % MAX_CELL_COUNT == 0)
				os << "  ";
		}
		if ((i + 1) % MAX_CELL_COUNT == 0)
			os << std::endl;
		os << std::endl;
	}
	os << std::endl;
}

bool FXSudokuBase::IsFinished()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);

			if (!pCell->HasNumber())
				return false;
		}
	}
	return true;
}

BYTE FXSudokuBase::GetUnfinishCount()
{
	BYTE count = 0;
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);

			if (!pCell->HasNumber())
				++count;
		}
	}
	return count;
}

#ifdef _DEBUG
bool FXSudokuBase::CheckAllCellNumber()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			if (!CheckCellNumber(i, j))
				return false;
		}
	}
	return true;
}
#endif // _DEBUG

