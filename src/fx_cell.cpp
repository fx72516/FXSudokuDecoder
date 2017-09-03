#include "stdafx.h"
#include "fx_cell.h"


FXCell::FXCell(const BYTE & idxX, const BYTE & idxY)
	: m_number(0), m_indexX(idxX), m_indexY(idxY)
{
}


FXCell::~FXCell()
{
	m_maybeNumSet.clear();
}

void FXCell::SetNumber(const BYTE & number)
{
	assert(!HasNumber());
	assert(number > 0 && number <= MAX_NUMBER && m_number == 0);

	m_number = number;

	if (m_maybeNumSet.size())
		m_maybeNumSet.clear();
}

BYTE FXCell::GetNumber()
{
	return m_number;
}

bool FXCell::HasNumber()
{
	return m_number > 0;
}

bool FXCell::SetOutNumbers(std::unordered_set<BYTE>& outNumSet)
{
	assert(!HasNumber());

	for (BYTE i = 1; i <= MAX_NUMBER; ++i)
	{
		if (outNumSet.find(i) == outNumSet.cend())
			m_maybeNumSet.insert(i);
	}

	return m_maybeNumSet.size() > 0;
}

BYTE FXCell::MaybeNumberCount()
{
	return (BYTE)m_maybeNumSet.size();
}

BYTE FXCell::CheckMaybeNumber()
{
	assert(!HasNumber());

	if (m_maybeNumSet.size() == 1)
	{
		BYTE number = *m_maybeNumSet.cbegin();
		SetNumber(number);
		return number;
	}
	else if (m_maybeNumSet.size() == 0)
	{
		return -1;
	}
	return 0;
}

bool FXCell::RemoveMaybeNumber(const BYTE & number)
{
	m_maybeNumSet.erase(number);
	
	return m_maybeNumSet.size() == 0;
}

std::unordered_set<BYTE> * FXCell::GetMaybeNumbers()
{
	return &m_maybeNumSet;
}

BYTE FXCell::GetIndexX()
{
	return m_indexX;
}

BYTE FXCell::GetIndexY()
{
	return m_indexY;
}
