// FXSudokuDecoder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "fx_sudoku.h"


int main()
{
	FXSudoku sudoku(true);
	sudoku.SetCellNumber(0, 2, 5);
	sudoku.SetCellNumber(0, 3, 3);
	sudoku.SetCellNumber(1, 0, 8);
	sudoku.SetCellNumber(1, 7, 2);
	sudoku.SetCellNumber(2, 1, 7);
	sudoku.SetCellNumber(2, 4, 1);
	sudoku.SetCellNumber(2, 6, 5);
	sudoku.SetCellNumber(3, 0, 4);
	sudoku.SetCellNumber(3, 5, 5);
	sudoku.SetCellNumber(3, 6, 3);
	sudoku.SetCellNumber(4, 1, 1);
	sudoku.SetCellNumber(4, 4, 7);
	sudoku.SetCellNumber(4, 8, 6);
	sudoku.SetCellNumber(5, 2, 3);
	sudoku.SetCellNumber(5, 3, 2);
	sudoku.SetCellNumber(5, 7, 8);
	sudoku.SetCellNumber(6, 1, 6);
	sudoku.SetCellNumber(6, 3, 5);
	sudoku.SetCellNumber(6, 8, 9);
	sudoku.SetCellNumber(7, 2, 4);
	sudoku.SetCellNumber(7, 7, 3);
	sudoku.SetCellNumber(8, 5, 9);
	sudoku.SetCellNumber(8, 6, 7);

	sudoku.Print();

	std::cout << std::endl;

	if (!sudoku.Decode())
		std::cout << "no decode!" << std::endl;
#ifdef _DEBUG
	else
		assert(sudoku.CheckAllCellNumber());
#endif // _DEBUG

	sudoku.Print();

	system("pause");

    return 0;
}

