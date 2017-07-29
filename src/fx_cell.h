#ifndef FX_CELL_H
#define FX_CELL_H

class FXCell
{
public:
	FXCell();
	~FXCell();

	BYTE GetNumber();
	bool SetNumber(const BYTE &);
	bool IsMaybe(const BYTE & number);
	bool AddMaybe(const BYTE & number);
	BYTE RemoveMaybe(const BYTE & number);
	BYTE SetOutNumber(std::unordered_set<BYTE> & outNumSet);
	std::unordered_set<BYTE> * GetMayberSet();
	BYTE MaybeCount();
	bool HasNumber();
	BYTE CheckMaybeNumber();

private:
	BYTE m_number;
	std::unordered_set<BYTE> m_maybeNumber;
};

#endif // !FX_CELL_H