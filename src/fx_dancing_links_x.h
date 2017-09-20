#ifndef FX_DANCING_LINKS_X_H
#define FX_DANCING_LINKS_X_H

class FXDancingLinksX
{
	struct DLX_Element
	{
		DLX_Element * Up;
		DLX_Element * Down;
		DLX_Element * Left;
		DLX_Element * Right;

		ULONG Row;
		ULONG Col;

		DLX_Element() : Up(this), Down(this), Left(this), Right(this), Row(UNUSEFULL_ULONG), Col(UNUSEFULL_ULONG) {}
	};

public:
	FXDancingLinksX();
	FXDancingLinksX(const ULONG & col);
	~FXDancingLinksX();

	void AddRow(const std::vector<ULONG> & data);
	void AddColumn();
	void AddColumn(ULONG i);

	void Print();
	void Print(std::ostream & os);

	bool Execute();

	std::vector<ULONG> GetTakenRow();
	std::vector<ULONG> GetTakenRowData(const ULONG & rowIdx);

private:
	void EraseRow(DLX_Element * ele, std::vector<DLX_Element*> & eraseElements);
	void EraseColumn(DLX_Element * col, std::vector<DLX_Element*> & eraseElements);
	DLX_Element * GetColumn(const ULONG & col);
	ULONG GetColEleCount(DLX_Element * col);

	bool TakenRow(DLX_Element * ele);
	void BackTracking();

private:
	DLX_Element * m_head;
	ULONG m_rowCount;
	ULONG m_colCount;

	std::vector<DLX_Element*> m_takenRow;
	std::stack<std::vector<DLX_Element*>> m_eraseElements;

//	std::vector<bool**> m_Ans;
};

#endif // !FX_DANCING_LINKS_X_H
