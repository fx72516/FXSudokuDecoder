#ifndef FX_SUDOKU_BASE_H
#define FX_SUDOKU_BASE_H

#include "fx_sudoku_i.h"

class FXGrid;
class FXCell;

class FXSudokuBase : public IXFudoku
{
public:
	FXSudokuBase();
	~FXSudokuBase();
	FXGrid * GetGrid(const BYTE & idxX, const BYTE & idxY) override;
	FXCell * GetCell(const BYTE & idxX, const BYTE & idxY) override;
	void SetCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number) override;
	BYTE GetCellNumber(const BYTE & idxX, const BYTE & idxY) override;
	void Print() override;
	void Print(std::ostream & os) override;
	bool IsFinished() override;
	BYTE GetUnfinishCount() override;
#ifdef _DEBUG
	bool CheckAllCellNumber() override;
#endif // _DEBUG

protected:
#ifdef _DEBUG
	bool CheckCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number = 0);
#endif // _DEBUG

private:
	FXGrid * m_grids[MAX_GRID_COUNT][MAX_GRID_COUNT];
};

#endif // !FX_SUDOKU_BASE_H
