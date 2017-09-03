#ifndef FX_CELL_H
#define FX_CELL_H

class FXCell
{
public:
	FXCell(const BYTE & idxX, const BYTE & idxY);
	~FXCell();
	void SetNumber(const BYTE & number);
	BYTE GetNumber();
	bool HasNumber();
	bool SetOutNumbers(std::unordered_set<BYTE> & outNumSet);
	BYTE MaybeNumberCount();
	BYTE CheckMaybeNumber();
	bool RemoveMaybeNumber(const BYTE & number);
	std::unordered_set<BYTE> * GetMaybeNumbers();
	BYTE GetIndexX();
	BYTE GetIndexY();

private:
	BYTE m_number;
	BYTE m_indexX;
	BYTE m_indexY;
	std::unordered_set<BYTE> m_maybeNumSet;
};

#endif // !FX_CELL_H
