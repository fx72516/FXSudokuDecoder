// FXSudokuDecoder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "fx_sudoku.h"


int main(int argc, TCHAR* args[])
{
	if (argc > 0)
	{
		std::ifstream ifs(args[0]);
		if (ifs.is_open())
		{
			FXSudoku sudoku;

			char buffer[256];
			ifs.getline(buffer, 256);
			//ifs >> number;
			ifs.close();
		}
	}

	FXSudoku sudoku;
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

	//sudoku.Print();
	//system("pause");

	if (sudoku.Decode())
		sudoku.Print();
	else
		std::cout << "no decode" << std::endl;

	system("pause");

    return 0;
}

