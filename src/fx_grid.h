#ifndef FX_GRID_H
#define FX_GRID_H

class FXCell;

class FXGrid
{
public:
	FXGrid(const BYTE & idxX, const BYTE & idxY);
	~FXGrid();
	FXCell * GetCell(const BYTE & idxX, const BYTE & idxY);
	bool GetOutNumbers(std::unordered_set<BYTE> & outNumSet);
	bool RemoveCellMaybeNumbers(const BYTE & number);
	BYTE CheckCellMaybeNumberOnlyOne(BYTE * idxX, BYTE * idxY);
	void GetAllNoNumCells(std::vector<FXCell*> & pCells);
	BYTE GetIndexX();
	BYTE GetIndexY();

#ifdef _DEBUG
	bool CheckCellNumber(const BYTE & number);
#endif // _DEBUG

private:
	BYTE m_indexX;
	BYTE m_indexY;
	FXCell * m_cells[MAX_CELL_COUNT][MAX_CELL_COUNT];
};

#endif // !FX_GRID_H