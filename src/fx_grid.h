#ifndef FX_GRID_H
#define FX_GRID_H

class FXCell;

class FXGrid
{
public:
	FXGrid();
	~FXGrid();

	FXCell * GetCell(const BYTE &, const BYTE &);
	bool GetOutNumber(std::unordered_set<BYTE> &);
	bool IsAllFinished();
	BYTE CheckCellMaybeNumber(const BYTE &, BYTE *, BYTE *);

private:
	FXCell * m_cells[MAX_CELL_COUNT][MAX_CELL_COUNT];
};

#endif // !FX_GRID_H