#include "stdafx.h"
#include "fx_sudoku_fx.h"
#include "fx_grid.h"
#include "fx_cell.h"

FXSudokuFX::FXSudokuFX(bool main)
	: FXSudokuBase(), m_triedCellIndexes(), m_main(main)
{
}

FXSudokuFX::~FXSudokuFX()
{
	m_triedCellIndexes.clear();
}

bool FXSudokuFX::Decode()
{
	if (!CalcMaybeNumber())
		return false;

	for(;;)
	{
		BYTE result = CheckCellMaybeNumber();
		if (result < 0)
			return false;

		if (IsFinished())
			return true;

		bool change = CheckCellMaybeNumberOnlyOne();

		if (result == 0 && !change)
			break;

		if (m_main)
			Print();
	}

	return TryCellMaybeNumber();
}

BYTE FXSudokuFX::GetTriedCellIndexCount()
{
	return (BYTE)m_triedCellIndexes.size();
}

bool FXSudokuFX::CalcMaybeNumber()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);

			std::unordered_set<BYTE> outNumSet;
			GetCellOutNumber(i, j, outNumSet);
			if (!pCell->SetOutNumbers(outNumSet))
				return false;
		}
	}

	return true;
}

bool FXSudokuFX::TryCellMaybeNumber()
{
	bool decoded = false;
	BYTE idxX = 0, idxY = 0;
	while (FXCell * pBestCell = GetBestMaybeNumberCell(&idxX, &idxY))
	{
		std::unordered_set<BYTE> * maybeNumbers = pBestCell->GetMaybeNumbers();
		auto iter = maybeNumbers->cbegin();
		do{
			FXSudokuFX * pSudoku = Clone();
			pSudoku->SetCellNumber(idxX, idxY, *iter);

			if (pSudoku->m_triedCellIndexes.size() < m_triedCellIndexes.size())
				int x = 0;

			if (pSudoku->Decode())
			{
				pSudoku->Print();
				if (!Sync(pSudoku))
				{
					assert(false);
					pSudoku->Destroy();
					return false;
				}
				decoded = true;
				pSudoku->Destroy();
				break;
			}
			pSudoku->Destroy();
		} while (++iter != maybeNumbers->cend());

		if (decoded)
			break;

		AddTriedCellIndex(idxX, idxY);

		if (m_main)
			std::cout << "\r" << (float)GetTriedCellIndexCount() / GetUnfinishCount() * 100 << "%";
	}

	return decoded;
}

BYTE FXSudokuFX::CheckCellMaybeNumber()
{
	BYTE change = 0;
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);

			BYTE number = pCell->CheckMaybeNumber();
			if (number > 0)
			{
				++change;
				if (RemoveCellMaybeNumbers(i, j, number))
					return -1;
			}
			else if (number < 0)
			{
				return -1;
			}
		}
	}
	return change;
}

bool FXSudokuFX::CheckCellMaybeNumberOnlyOne()
{
	bool change = false;

	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		std::vector<FXCell*> pCells;
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);
			pCells.push_back(pCell);
		}
		if (CheckCellMaybeNumberOnlyOne(pCells))
			change = true;
	}

	for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
	{
		std::vector<FXCell*> pCells;
		for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
		{
			GET_CELL_AND_CHECK_CONTINUE(i, j, pCell);
			pCells.push_back(pCell);
		}
		if (CheckCellMaybeNumberOnlyOne(pCells))
			change = true;
	}

	for (BYTE i = 0; i < MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT; ++j)
		{
			FXGrid * pGrid = GetGrid(i, j);
			assert(pGrid);

			std::vector<FXCell*> pCells;
			pGrid->GetAllNoNumCells(pCells);
			if (CheckCellMaybeNumberOnlyOne(pCells))
				change = true;
		}
	}

	return change;
}

bool FXSudokuFX::CheckCellMaybeNumberOnlyOne(const std::vector<FXCell*> & pCells)
{
	BYTE maybeNumberCounts[MAX_NUMBER] = { 0 };
	BYTE cellCount = (BYTE)pCells.size();
	for (BYTE i = 0; i < cellCount; ++i)
	{
		FXCell * pCell = pCells[i];
		assert(pCell);

		std::unordered_set<BYTE> * maybeNumbers = pCell->GetMaybeNumbers();
		for each (BYTE number in *maybeNumbers)
		{
			++maybeNumberCounts[--number];
		}
	}

	bool change = false;
	BYTE num = 0;
	for (BYTE i = 0; i < MAX_NUMBER; ++i)
	{
		if (maybeNumberCounts[i] == 1)
		{
			num = i + 1;
			for (BYTE j = 0; j < cellCount; ++j)
			{
				FXCell * pCell = pCells[j];
				assert(pCell);
				if (pCell->HasNumber())
					continue;

				std::unordered_set<BYTE> * maybeNumbers = pCell->GetMaybeNumbers();
				if (maybeNumbers->find(num) != maybeNumbers->cend())
				{
					change = true;
					pCell->SetNumber(num);
					if (RemoveCellMaybeNumbers(pCell->GetIndexX(), pCell->GetIndexY(), num))
						return false;
				}
			}
			assert(change);
		}
	}

	return change;
}

FXCell * FXSudokuFX::GetBestMaybeNumberCell(BYTE * idxX, BYTE * idxY)
{
	FXCell * pBestCell = nullptr;

	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			FXCell * pCell = GetCell(i, j);
			assert(pCell);

			if (pCell->HasNumber())
				continue;
			if (pCell->MaybeNumberCount() == 0)
				return false;
			if (IsCellTried(i, j))
				continue;

			if (!pBestCell || pBestCell->MaybeNumberCount() > pCell->MaybeNumberCount())
			{
				*idxX = i;
				*idxY = j;
				pBestCell = pCell;
			}
		}
	}

	return pBestCell;
}

bool FXSudokuFX::RemoveCellMaybeNumbers(const BYTE & idxX, const BYTE & idxY, const BYTE & number)
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		GET_CELL_AND_CHECK_CONTINUE(i, idxY, pCell);
		if (pCell->RemoveMaybeNumber(number))
			return true;
	}

	for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
	{
		GET_CELL_AND_CHECK_CONTINUE(idxX, j, pCell);
		if (pCell->RemoveMaybeNumber(number))
			return true;
	}

	GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid);
	if (pGrid->RemoveCellMaybeNumbers(number))
		return true;

	return false;
}

bool FXSudokuFX::GetCellOutNumber(const BYTE & idxX, const BYTE & idxY, std::unordered_set<BYTE> & outNumSet)
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		if (BYTE number = GetCellNumber(i, idxY))
			outNumSet.insert(number);
	}

	for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
	{
		if (BYTE number = GetCellNumber(idxX, j))
			outNumSet.insert(number);
	}

	GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid);
	return pGrid->GetOutNumbers(outNumSet);
}

void FXSudokuFX::AddTriedCellIndex(const BYTE & idxX, const BYTE & idxY)
{
	m_triedCellIndexes.insert((idxX << 8) | idxY);
}

bool FXSudokuFX::IsCellTried(const BYTE & idxX, const BYTE & idxY)
{
	return m_triedCellIndexes.find((idxX << 8) | idxY) != m_triedCellIndexes.cend();
}

FXSudokuFX * FXSudokuFX::Clone()
{
	FXSudokuFX * pSudoku = new FXSudokuFX();
	pSudoku->Sync(this);
	return pSudoku;
}

void FXSudokuFX::Destroy()
{
	delete this;
}

bool FXSudokuFX::Sync(FXSudokuFX * pSudoku)
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
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

	if (!IsFinished())
	{
		m_triedCellIndexes.insert(pSudoku->m_triedCellIndexes.cbegin(), pSudoku->m_triedCellIndexes.cend());
		return false;
	}
	return true;
}
