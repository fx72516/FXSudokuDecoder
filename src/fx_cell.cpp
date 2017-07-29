#include "stdafx.h"
#include "fx_cell.h"

FXCell::FXCell()
	: m_number(0)
{
}


FXCell::~FXCell()
{
	m_maybeNumber.clear();
}

BYTE FXCell::GetNumber()
{
	return m_number;
}

bool FXCell::SetNumber(const BYTE & number)
{
	assert(m_number == 0);
	CHECK_RET_FALSE(number);

	m_number = number;

	if (m_maybeNumber.size())
		m_maybeNumber.clear();

	return true;
}

bool FXCell::IsMaybe(const BYTE & number)
{
	return m_maybeNumber.find(number) != m_maybeNumber.cend();
}

bool FXCell::AddMaybe(const BYTE & number)
{
	CHECK_RET_FALSE(number);

	return m_maybeNumber.insert(number).second;
}

BYTE FXCell::RemoveMaybe(const BYTE & number)
{
	if (HasNumber())
		return 0;

	m_maybeNumber.erase(number);

	return CheckMaybeNumber();
}

BYTE FXCell::SetOutNumber(std::unordered_set<BYTE>& outNumSet)
{
	for (BYTE num = 1; num <= MAX_NUMBER; ++num)
	{
		if (outNumSet.find(num) == outNumSet.cend())
			AddMaybe(num);
	}

	return 0;
}

std::unordered_set<BYTE>* FXCell::GetMayberSet()
{
	return &m_maybeNumber;
}

BYTE FXCell::MaybeCount()
{
	return (BYTE)m_maybeNumber.size();
}

bool FXCell::HasNumber()
{
	return m_number != 0;
}

BYTE FXCell::CheckMaybeNumber()
{
	if (m_maybeNumber.size() == 1)
	{
		BYTE num = *m_maybeNumber.begin();
		if (!SetNumber(num)) assert(false);
		assert(num > 0 && num <= MAX_NUMBER);
		return num;
	}
	else if (m_maybeNumber.size() == 0)
	{
		return -1;
	}
	return 0;
}
