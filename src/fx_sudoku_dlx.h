#ifndef FX_SUDOKU_DLX_H
#define FX_SUDOKU_DLX_H

#include "fx_sudoku_base.h"



class FXSudokuDlx : public FXSudokuBase
{
public:
	FXSudokuDlx();
	~FXSudokuDlx();

	bool Decode() override;
};

#endif // !FX_SUDOKU_DLX_H
