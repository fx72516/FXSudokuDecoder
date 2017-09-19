#ifndef FX_SUDOKU_I_H
#define FX_SUDOKU_I_H

class FXGrid;
class FXCell;

class IXFudoku
{
public:
	virtual FXGrid * GetGrid(const BYTE & idxX, const BYTE & idxY)							PURE;
	virtual FXCell * GetCell(const BYTE & idxX, const BYTE & idxY)							PURE;
	virtual void SetCellNumber(const BYTE & idxX, const BYTE & idxY, const BYTE & number)	PURE;
	virtual BYTE GetCellNumber(const BYTE & idxX, const BYTE & idxY)						PURE;
	virtual void Print()																	PURE;
	virtual void Print(std::ostream & os)													PURE;
	virtual bool Decode()																	PURE;
	virtual bool IsFinished()																PURE;
	virtual BYTE GetUnfinishCount()															PURE;
#ifdef _DEBUG
	virtual bool CheckAllCellNumber()														PURE;
#endif // _DEBUG
};
#endif // !FX_SUDOKU_I_H
