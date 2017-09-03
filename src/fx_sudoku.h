#ifndef FX_SUDOKU_H
#define FX_SUDOKU_H

class FXGrid;
class FXCell;

class FXSudoku
{
public:
	FXSudoku(bool main = false);
	~FXSudoku();
	FXGrid * GetGrid(const BYTE & idxX, const BYTE & idxY);
	FXCell * GetCell(const BYTE & idxX, const BYTE & idxY);
	void SetCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number);
	BYTE GetCellNumber(const BYTE & idxX, const BYTE & idxY);
	void Print();
	void Print(std::ostream & os);
	bool Decode();
	bool IsFinished();
	BYTE GetTriedCellIndexCount();
	BYTE GetUnfinishCount();
#ifdef _DEBUG
	bool CheckAllCellNumber();
#endif // _DEBUG

private:
	bool CalcMaybeNumber();
	bool TryCellMaybeNumber();
	BYTE CheckCellMaybeNumber();
	bool CheckCellMaybeNumberOnlyOne();
	bool CheckCellMaybeNumberOnlyOne(const std::vector<FXCell*> & pCells);
	FXCell * GetBestMaybeNumberCell(BYTE * idxX, BYTE * idxY);
	bool RemoveCellMaybeNumbers(const BYTE & idxX, const BYTE & idxY, const BYTE & number);
	bool GetCellOutNumber(const BYTE & idxX, const BYTE & idxY, std::unordered_set<BYTE> & outNumSet);
	void AddTriedCellIndex(const BYTE & idxX, const BYTE & idxY);
	bool IsCellTried(const BYTE & idxX, const BYTE & idxY);
	bool Sync(FXSudoku * pSudoku);
	FXSudoku * Clone();
	void Destroy();

#ifdef _DEBUG
	bool CheckCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number = 0);
#endif // _DEBUG

private:
	FXGrid * m_grids[MAX_GRID_COUNT][MAX_GRID_COUNT];
	std::unordered_set<short> m_triedCellIndexes;
	bool m_main;
};

#endif // !FX_SUDOKU_H
