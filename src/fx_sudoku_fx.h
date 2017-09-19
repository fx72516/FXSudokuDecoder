#ifndef FX_SUDOKU_FX_H
#define FX_SUDOKU_FX_H

#include "fx_sudoku_base.h"

class FXCell;

class FXSudokuFX : public FXSudokuBase
{
public:
	FXSudokuFX(bool main = false);
	~FXSudokuFX();

	bool Decode() override;

private:
	bool CalcMaybeNumber();
	bool TryCellMaybeNumber();
	BYTE CheckCellMaybeNumber();
	BYTE GetTriedCellIndexCount();
	bool CheckCellMaybeNumberOnlyOne();
	bool CheckCellMaybeNumberOnlyOne(const std::vector<FXCell*> & pCells);
	FXCell * GetBestMaybeNumberCell(BYTE * idxX, BYTE * idxY);
	bool RemoveCellMaybeNumbers(const BYTE & idxX, const BYTE & idxY, const BYTE & number);
	bool GetCellOutNumber(const BYTE & idxX, const BYTE & idxY, std::unordered_set<BYTE> & outNumSet);
	void AddTriedCellIndex(const BYTE & idxX, const BYTE & idxY);
	bool IsCellTried(const BYTE & idxX, const BYTE & idxY);
	bool Sync(FXSudokuFX * pSudoku);
	FXSudokuFX * Clone();
	void Destroy();

private:
	std::unordered_set<short> m_triedCellIndexes;
	bool m_main;
};

#endif // !FX_SUDOKU_FX_H
