#include "stdafx.h"
#include "fx_sudoku.h"
#include "fx_grid.h"
#include "fx_cell.h"

#define GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid)						\
	FXGrid * (pGrid) = GetGrid((idxX) / MAX_CELL_COUNT, (idxY) / MAX_CELL_COUNT);	\
	assert(pGrid);

#define CHECK_NUMBER_RET_FALSE(idxX, idxY, number)				\


#define CHECK_BOOL_RET(r)	if (!(r))	{ goto FX_EXIT; }

FXSudoku::FXSudoku(bool main)
	: m_main(main)
{
	for (BYTE i = 0; i < MAX_GRID_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT; ++j)
		{
			m_grids[i][j] = new FXGrid(i, j);
		}
	}
}


FXSudoku::~FXSudoku()
{
	m_triedCellIndexes.clear();
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
	if (idxX < 0 || idxX >= MAX_GRID_COUNT || idxY < 0 || idxY >= MAX_GRID_COUNT)
		return nullptr;
	return m_grids[idxX][idxY];
}

FXCell * FXSudoku::GetCell(const BYTE & idxX, const BYTE & idxY)
{
	if (idxX < 0 || idxX >= MAX_GRID_COUNT * MAX_CELL_COUNT || idxY < 0 || idxY >= MAX_GRID_COUNT * MAX_CELL_COUNT)
		return nullptr;

	GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid);
	return pGrid->GetCell(idxX % MAX_CELL_COUNT, idxY % MAX_CELL_COUNT);
}

void FXSudoku::SetCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number)
{
	FXCell * pCell = GetCell(idxX, idxY);
	assert(pCell);

	assert(CheckCellNumber(idxX, idxY, number));

	pCell->SetNumber(number);
}

#ifdef _DEBUG
bool FXSudoku::CheckCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & _num)
{
	BYTE number = _num;
	if (number == 0)
	{
		FXCell * pCell = GetCell((idxX), (idxY));
		assert(pCell);
		if (pCell->HasNumber())
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

BYTE FXSudoku::GetCellNumber(const BYTE & idxX, const BYTE & idxY)
{
	FXCell * pCell = GetCell(idxX, idxY);
	assert(pCell);
	return pCell->GetNumber();
}

void FXSudoku::Print()
{
	Print(std::cout);
}

void FXSudoku::Print(std::ostream & os)
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

bool FXSudoku::Decode()
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

#ifdef _DEBUG
		if (m_main)
			Print();
#endif // _DEBUG
	}

	return TryCellMaybeNumber();
}

bool FXSudoku::IsFinished()
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

BYTE FXSudoku::GetTriedCellIndexCount()
{
	return (BYTE)m_triedCellIndexes.size();
}

BYTE FXSudoku::GetUnfinishCount()
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
bool FXSudoku::CheckAllCellNumber()
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
		{
			CheckCellNumber(i, j);
		}
	}
	return true;
}
#endif // _DEBUG

bool FXSudoku::CalcMaybeNumber()
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

bool FXSudoku::TryCellMaybeNumber()
{
	bool decoded = false;
	BYTE idxX = 0, idxY = 0;
	while (FXCell * pBestCell = GetBestMaybeNumberCell(&idxX, &idxY))
	{
		std::unordered_set<BYTE> * maybeNumbers = pBestCell->GetMaybeNumbers();
		auto iter = maybeNumbers->cbegin();
		do{
			FXSudoku * pSudoku = Clone();
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

BYTE FXSudoku::CheckCellMaybeNumber()
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
				RemoveCellMaybeNumbers(i, j, number);
			}
			else if (number < 0)
			{
				return -1;
			}
		}
	}
	return change;
}

bool FXSudoku::CheckCellMaybeNumberOnlyOne()
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

bool FXSudoku::CheckCellMaybeNumberOnlyOne(const std::vector<FXCell*> & pCells)
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
					RemoveCellMaybeNumbers(pCell->GetIndexX(), pCell->GetIndexY(), num);
				}
			}
			assert(change);
		}
	}

	return change;
}

FXCell * FXSudoku::GetBestMaybeNumberCell(BYTE * idxX, BYTE * idxY)
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

void FXSudoku::RemoveCellMaybeNumbers(const BYTE & idxX, const BYTE & idxY, const BYTE & number)
{
	for (BYTE i = 0; i < MAX_GRID_COUNT * MAX_CELL_COUNT; ++i)
	{
		GET_CELL_AND_CHECK_CONTINUE(i, idxY, pCell);
		pCell->RemoveMaybeNumber(number);
	}

	for (BYTE j = 0; j < MAX_GRID_COUNT * MAX_CELL_COUNT; ++j)
	{
		GET_CELL_AND_CHECK_CONTINUE(idxX, j, pCell);
		pCell->RemoveMaybeNumber(number);
	}

	GET_GRID_BY_CELL_INDEX_AND_ASSERT(idxX, idxY, pGrid);
	pGrid->RemoveCellMaybeNumbers(number);
}

bool FXSudoku::GetCellOutNumber(const BYTE & idxX, const BYTE & idxY, std::unordered_set<BYTE> & outNumSet)
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

void FXSudoku::AddTriedCellIndex(const BYTE & idxX, const BYTE & idxY)
{
	m_triedCellIndexes.insert((idxX << 8) | idxY);
}

bool FXSudoku::IsCellTried(const BYTE & idxX, const BYTE & idxY)
{
	return m_triedCellIndexes.find((idxX << 8) | idxY) != m_triedCellIndexes.cend();
}

FXSudoku * FXSudoku::Clone()
{
	FXSudoku * pSudoku = new FXSudoku();
	pSudoku->Sync(this);
	return pSudoku;
}

void FXSudoku::Destroy()
{
	delete this;
}

bool FXSudoku::Sync(FXSudoku * pSudoku)
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
