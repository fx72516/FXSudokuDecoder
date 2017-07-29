#ifndef FX_SUDOKU_H
#define FX_SUDOKU_H

class FXGrid;
class FXCell;

class FXSudoku
{
public:
	FXSudoku();
	~FXSudoku();

	FXGrid * GetGrid(const BYTE &, const BYTE &);
	FXCell * GetCell(const BYTE &, const BYTE &);
	bool SetCellNumber(const BYTE &, const BYTE &, const BYTE &);
	BYTE GetCellNumber(const BYTE &, const BYTE &);

	void Print();
	void Print(std::ostream& os);
	bool Decode();

private:
	FXSudoku(FXSudoku*) { assert(false); };
	bool CalcCellMaybe();
	bool GetCellOutNumber(const BYTE &, const BYTE &, std::unordered_set<BYTE> & outNumSet);
	bool CheckCellMaybeNumber(const BYTE &, const BYTE &, const BYTE &);
	bool CheckCellMaybeNumber();
	bool IsAllCellFinished();
	bool TryMaybeDecode();
	FXSudoku * Clone();
	FXCell * GetBestMaybeCell(BYTE *, BYTE *);
	void Sync(FXSudoku * pSudoku);
	void AddTryMaybeNumberIndex(const BYTE &, const BYTE &);
	void AddTryMaybeNumberIndex(const std::unordered_set<short> &);
	bool HasTryMaybeNumberIndex(const BYTE &, const BYTE &);

private:
	FXGrid * m_grids[MAX_GRID_COUNT][MAX_GRID_COUNT];
	std::unordered_set<short> m_tryMaybeNumberIndexes;
};

#endif // !FX_SUDOKU_H