#include "stdafx.h"
#include "fx_sudoku.h"
#include "fx_grid.h"
#include "fx_cell.h"

FXSudoku::FXSudoku()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT; ++j)
			m_grids[i][j] = new FXGrid();
	}
}


FXSudoku::~FXSudoku()
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

FXGrid * FXSudoku::GetGrid(const BYTE & idxX, const BYTE & idxY)
{
	if (idxX >= 0 && idxX < MAX_CELL_COUNT && idxY >= 0 && idxY < MAX_CELL_COUNT)
		return m_grids[idxX][idxY];
	return nullptr;
}

FXCell * FXSudoku::GetCell(const BYTE & idxX, const BYTE & idxY)
{
	FXGrid * pGrid = GetGrid(idxX / MAX_CELL_COUNT, idxY / MAX_CELL_COUNT);
	if (pGrid)
		return pGrid->GetCell(idxX % MAX_CELL_COUNT, idxY % MAX_CELL_COUNT);
	return nullptr;
}

bool FXSudoku::SetCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number)
{
	if (FXCell * pCell = GetCell(idxX, idxY))
		return pCell->SetNumber(number);
	return false;
}

BYTE FXSudoku::GetCellNumber(const BYTE & idxX, const BYTE & idxY)
{
	if (FXCell * pCell = GetCell(idxX, idxY))
		return pCell->GetNumber();
	return 0;
}

void FXSudoku::Print()
{
	Print(std::cout);
}

void FXSudoku::Print(std::ostream & os)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT * MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT * MAX_GRID_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);
			os << (int)pCell->GetNumber() << "  ";
		}
		os << std::endl;
	}
}

bool FXSudoku::Decode()
{
	if (CalcCellMaybe())
	{
		if (!IsAllCellFinished())
			return TryMaybeDecode();
		return true;
	}
	return false;
}

bool FXSudoku::CalcCellMaybe()
{
	for (BYTE i = 0; i < MAX_CELL_COUNT * MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT * MAX_GRID_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);
			if (pCell->HasNumber())
				continue;

			std::unordered_set<BYTE> outNumSet;
			GetCellOutNumber(i, j, outNumSet);
			FXGrid * pGrid = GetGrid(i / MAX_CELL_COUNT, j / MAX_CELL_COUNT);
			assert(pGrid);
			pGrid->GetOutNumber(outNumSet);

			pCell->SetOutNumber(outNumSet);
		}
	}

	return CheckCellMaybeNumber();
}

bool FXSudoku::GetCellOutNumber(const BYTE & idxX, const BYTE & idxY, std::unordered_set<BYTE>& outNumSet)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT * MAX_GRID_COUNT; ++i)
	{
		FXCell * pCell = GetCell(i, idxY);
		assert(pCell);
		if (BYTE number = pCell->GetNumber())
			outNumSet.insert(number);
	}

	for (BYTE j = 0; j < MAX_CELL_COUNT * MAX_GRID_COUNT; ++j)
	{
		FXCell * pCell = GetCell(idxX, j);
		assert(pCell);
		if (BYTE number = pCell->GetNumber())
			outNumSet.insert(number);
	}

	return true;
}

bool FXSudoku::CheckCellMaybeNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT * MAX_GRID_COUNT; ++i)
	{
		FXCell * pCell = GetCell(i, idxY);
		assert(pCell);
		BYTE num = pCell->RemoveMaybe(number);
		if (num > 0)
		{
			if (!CheckCellMaybeNumber(i, idxY, num))
				return false;
		}
		else if (num < 0)
			return false;
	}

	for (BYTE j = 0; j < MAX_CELL_COUNT * MAX_GRID_COUNT; ++j)
	{
		FXCell * pCell = GetCell(idxX, j);
		assert(pCell);
		BYTE num = pCell->RemoveMaybe(number);
		if (num > 0)
		{
			if (!CheckCellMaybeNumber(idxX, j, num))
				return false;
		}
		else if (num < 0)
			return false;
	}

	FXGrid * pGrid = GetGrid(idxX / MAX_CELL_COUNT, idxY / MAX_CELL_COUNT);
	assert(pGrid);
	BYTE i_x = 0, i_y = 0;
	while (BYTE num = pGrid->CheckCellMaybeNumber(number, &i_x, &i_y))
	{
		if (num < 0 || !CheckCellMaybeNumber(idxX / MAX_CELL_COUNT * MAX_CELL_COUNT + i_x, idxY / MAX_CELL_COUNT * MAX_CELL_COUNT + i_y, num))
			return false;
	}
	return true;
}

bool FXSudoku::CheckCellMaybeNumber()
{
	for (BYTE i = 0; i < MAX_CELL_COUNT * MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT * MAX_GRID_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);
			if (pCell->HasNumber())
				continue;

			BYTE num = pCell->CheckMaybeNumber();
			if (num > 0)
			{
				if (!CheckCellMaybeNumber(i, j, num))
					return false;
			}
			else if (num < 0)
				return false;
		}
	}
	return true;
}

bool FXSudoku::IsAllCellFinished()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT; ++j)
		{
			FXGrid * pGrid = GetGrid(i, j);
			assert(pGrid);
			if (!pGrid->IsAllFinished())
				return false;
		}
	}

	return true;
}

bool FXSudoku::TryMaybeDecode()
{
	bool decoded = false;
	while (!decoded)
	{
		BYTE idxX = 0, idxY = 0;
		FXCell * pCell = GetBestMaybeCell(&idxX, &idxY);
		if (!pCell)
			return false;

		std::unordered_set<BYTE> * mayberSet = pCell->GetMayberSet();
		if (!mayberSet->size())
			return false;

		auto iter = mayberSet->cbegin();
		do {
			FXSudoku * pSudoku = Clone();
			pSudoku->AddTryMaybeNumberIndex(idxX, idxY);
			pSudoku->AddTryMaybeNumberIndex(m_tryMaybeNumberIndexes);
			pSudoku->SetCellNumber(idxX, idxY, *iter);

			if (pSudoku->Decode())
			{
				Sync(pSudoku);
				if (!IsAllCellFinished())
				{
					assert(false);
					return false;
				}
				decoded = true;
				delete pSudoku;
				break;
			}
			else
			{
				delete pSudoku;
			}
		} while (++iter != mayberSet->cend());

		if (decoded)
			break;

		AddTryMaybeNumberIndex(idxX, idxY);
	}

	return decoded;
}

FXSudoku * FXSudoku::Clone()
{
	FXSudoku * pSudoku = new FXSudoku();
	pSudoku->Sync(this);
	return pSudoku;
}

FXCell * FXSudoku::GetBestMaybeCell(BYTE * idxX, BYTE * idxY)
{
	FXCell * pBestMaybeCell = NULL;

	for (BYTE i = 0; i < MAX_CELL_COUNT * MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT * MAX_GRID_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);
			if (pCell->HasNumber() || HasTryMaybeNumberIndex(i, j))
				continue;

			if (!pBestMaybeCell || pBestMaybeCell->MaybeCount() > pCell->MaybeCount())
			{
				*idxX = i;
				*idxY = j;
				pBestMaybeCell = pCell;
			}
		}
	}

	return pBestMaybeCell;
}

void FXSudoku::Sync(FXSudoku * pSudoku)
{
	for (BYTE i = 0; i < MAX_CELL_COUNT * MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_CELL_COUNT * MAX_GRID_COUNT; ++j)
		{
			if (BYTE number = pSudoku->GetCellNumber(i, j))
			{
				if (!GetCellNumber(i, j))
					SetCellNumber(i, j, number);
				else
					assert(GetCellNumber(i, j) == number);
			}
		}
	}
}

void FXSudoku::AddTryMaybeNumberIndex(const BYTE & idxX, const BYTE & idxY)
{
	m_tryMaybeNumberIndexes.insert((idxX << 8) | idxY);
	std::cout << "AddTryMaybeNumberIndex " << (int)idxX << " " << (int)idxY << std::endl;
}

void FXSudoku::AddTryMaybeNumberIndex(const std::unordered_set<short> & tryMaybeNumberIndexes)
{
	for each (short idx in tryMaybeNumberIndexes)
		m_tryMaybeNumberIndexes.insert(idx);
}

bool FXSudoku::HasTryMaybeNumberIndex(const BYTE & idxX, const BYTE & idxY)
{
	return m_tryMaybeNumberIndexes.find((idxX << 8) | idxY) != m_tryMaybeNumberIndexes.cend();
}

