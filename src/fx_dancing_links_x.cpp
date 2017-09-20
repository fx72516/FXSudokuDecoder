#include "stdafx.h"
#include "fx_dancing_links_x.h"



FXDancingLinksX::FXDancingLinksX()
	: m_rowCount(0), m_colCount(0), m_takenRow(), m_eraseElements()
{
	m_head = new DLX_Element();
}

FXDancingLinksX::FXDancingLinksX(const ULONG & col)
	: FXDancingLinksX()
{
	AddColumn(col);
}


FXDancingLinksX::~FXDancingLinksX()
{
	DLX_Element * col = m_head->Right;
	while (col != m_head)
	{
		DLX_Element * ele = col->Down;
		while (ele != col)
		{
			ele = ele->Down;
			delete ele->Up;
		}

		col = col->Right;
		delete col->Left;
	}
	delete m_head;
	m_head = nullptr;
}

void FXDancingLinksX::AddColumn()
{
	DLX_Element * newEle = new DLX_Element();

	DLX_Element * right = m_head->Left;
	m_head->Left = newEle;
	newEle->Right = m_head;
	right->Right = newEle;
	newEle->Left = right;

	newEle->Col = m_colCount++;
}

void FXDancingLinksX::AddColumn(ULONG i)
{
	while (i-- > 0)
		AddColumn();
}

FXDancingLinksX::DLX_Element * FXDancingLinksX::GetColumn(const ULONG & col)
{
	if (col >= m_colCount)
		return nullptr;

	DLX_Element * ele = m_head;
	if (col >= m_colCount / 2)
	{
		ULONG c = m_colCount;
		while ((ele = ele->Left) != m_head)
		{
			if (--c == col)
				return ele;
		}
	}
	else
	{
		ULONG c = 0;
		while ((ele = ele->Right) != m_head)
		{
			if (c++ == col)
				return ele;
		}
	}

	ASSERT(false);
	return nullptr;
}

void FXDancingLinksX::AddRow(const std::vector<ULONG>& data)
{
	ASSERT(data.size() > 0);
	if (data.size() == 0)
		return;

	DLX_Element * row = nullptr;
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		DLX_Element * col = GetColumn(data[i]);
		DLX_Element * newEle = new DLX_Element();

		DLX_Element * down = col->Up;
		col->Up = newEle;
		newEle->Down = col;
		down->Down = newEle;
		newEle->Up = down;

		if (row)
		{
			DLX_Element * right = row->Left;
			row->Left = newEle;
			newEle->Right = row;
			right->Right = newEle;
			newEle->Left = right;
		}
		else
		{
			row = newEle;
		}

		newEle->Col = col->Col;
		newEle->Row = m_rowCount;
	}

	++m_rowCount;
}

void FXDancingLinksX::EraseRow(DLX_Element * ele, std::vector<DLX_Element*> & eraseElements)
{
	ASSERT(ele->Row != UNUSEFULL_ULONG);

	DLX_Element * e = ele;
	while ((e = e->Right) != ele)
	{
		e->Up->Down = e->Down;
		e->Down->Up = e->Up;
		eraseElements.push_back(e);
	}
}

void FXDancingLinksX::EraseColumn(DLX_Element * ele, std::vector<DLX_Element*> & eraseElements)
{
	DLX_Element * e = ele;
	while ((e = e->Down) != ele)
	{
		if (e->Row != UNUSEFULL_ULONG)
		{
			EraseRow(e, eraseElements);
		}
		else
		{
			e->Left->Right = e->Right;
			e->Right->Left = e->Left;
			eraseElements.push_back(e);
		}
	}

	eraseElements.push_back(ele);
}

ULONG FXDancingLinksX::GetColEleCount(DLX_Element * col)
{
	ULONG count = 0;

	DLX_Element * e = col;
	while ((e = e->Down) != col)
		++count;

	return count;
}

bool FXDancingLinksX::TakenRow(DLX_Element * ele)
{
	m_takenRow.push_back(ele);

	std::vector<DLX_Element*> eraseElements;
	DLX_Element * e = ele;
	do
	{
		EraseColumn(e, eraseElements);
	} while ((e = e->Right) != ele);

	m_eraseElements.push(eraseElements);
	return true;
}

void FXDancingLinksX::BackTracking()
{
	m_takenRow.erase(m_takenRow.end() - 1);

	std::vector<DLX_Element*> eraseElements = m_eraseElements.top();
	for (std::size_t i = 0; i < eraseElements.size(); ++i)
	{
		DLX_Element * ele = eraseElements[i];

		if (ele->Up->Down != ele)
			ele->Up->Down = ele;
		if (ele->Down->Up != ele)
			ele->Down->Up = ele;
		if (ele->Left->Right != ele)
			ele->Left->Right = ele;
		if (ele->Right->Left != ele)
			ele->Right->Left = ele;
	}
	eraseElements.clear();
	m_eraseElements.pop();
}

bool FXDancingLinksX::Execute()
{
	if (m_head->Right == m_head)
		return true;

	DLX_Element * col = m_head;
	ULONG colEleCount = UNUSEFULL_ULONG;

	DLX_Element * c = col->Right;
	do 
	{
		ULONG ceCount = GetColEleCount(c);
		if (ceCount < colEleCount)
		{
			col = c;
			colEleCount = ceCount;
			if (colEleCount == 0)
				return false;
		}
	} while ((c = c->Right) != m_head);

	c = col;
	while ((c = c->Down) != col)
	{
		TakenRow(c);

		if (Execute())
			return true;
		else
			BackTracking();
	}

	return m_head->Left == m_head;
}

std::vector<ULONG> FXDancingLinksX::GetTakenRow()
{
	std::vector<ULONG> result;

	for each (DLX_Element * ele in m_takenRow)
	{
		result.push_back(ele->Row);
	}

	return result;
}

std::vector<ULONG> FXDancingLinksX::GetTakenRowData(const ULONG & rowIdx)
{
	std::vector<ULONG> data;

	ASSERT(m_takenRow.size() > rowIdx);
	if (m_takenRow.size() <= rowIdx)
		return data;


	DLX_Element * row = m_takenRow[rowIdx];
	do
	{
		data.push_back(row->Col);
	} while ((row = row->Right) != m_takenRow[rowIdx]);

	return data;
}

void FXDancingLinksX::Print()
{
#ifdef _DEBUG
	Print(std::cout);
#endif //  _DEBUG
}

void FXDancingLinksX::Print(std::ostream & os)
{
	bool ** exactcover = new bool *[m_rowCount];
	for (ULONG i = 0; i < m_rowCount; ++i)
	{
		exactcover[i] = new bool[m_colCount];
		for (ULONG j = 0; j < m_colCount; ++j)
			exactcover[i][j] = false;
	}

	DLX_Element * col = m_head;
	while ((col = col->Right) != m_head)
	{
		DLX_Element * ele = col;
		while ((ele = ele->Down) != col)
			exactcover[ele->Row][ele->Col] = true;
	}

	for (ULONG i = 0; i < m_rowCount; ++i)
	{
		for (ULONG j = 0; j < m_colCount; ++j)
		{
			os << (exactcover[i][j] ? "1  " : "0  ");
		}
		os << std::endl;
	}
	os << std::endl;
}
