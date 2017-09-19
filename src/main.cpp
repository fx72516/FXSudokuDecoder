// FXSudokuDecoder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "fx_sudoku.h"
#include "fx_dancing_links_x.h"
#include <fstream>


int main(int argc, char * args[])
{




	return 0;

	FXDancingLinksX dlx(8);
	dlx.AddRow({ 0, 3, 6 });
	dlx.AddRow({ 0, 3 });
	dlx.AddRow({ 3, 4, 6, 7 });
	dlx.AddRow({ 2, 4, 5 });
	dlx.AddRow({ 1, 2, 5, 6, 7 });
	dlx.AddRow({ 1, 6 });

	dlx.Print(std::cout);

	if (dlx.Execute())
	{
		std::vector<ULONG> result = dlx.GetTakenRow();
		std::cout << "result:" << std::endl;
		for each (ULONG var in result)
		{
			std::cout << var << ",";
		}
		std::cout << std::endl;
	}
	else
	{
		std::cout << "no solution!" << std::endl;
	}

	system("pause");
	return 0;

	for (BYTE i = 0; i < argc; ++i)
		std::cout << args[i] << std::endl;

	FXSudoku sudoku(true);

	if (argc > 1)
	{
		BYTE row = 0;
		BYTE col = 0;
		std::ifstream ifs(args[1]);
		while (!ifs.eof())
		{
			char buff[256];
			ifs.getline(buff, 256);
			for (BYTE i = 0; buff[i]; ++i)
			{
				if (buff[i] == ' ' || buff[i] == '\t')
					continue;
		
				if (buff[i] >= '0' && buff[i] <= '9')
				{
					if (buff[i] != '0')
						sudoku.SetCellNumber(row, col, buff[i] - 0x30);
				}
				else
				{
					return -1;
				}
			}

			if (++col >= 9)
			{
				if (++row >= 9)
					break;
				col = 0;
			}
		}

// 		while (!ifs.eof())
// 		{
// 			BYTE col = 0;
// 			char buff[256];
// 			ifs.getline(buff, 256);
// 			for (BYTE i = 0; buff[i]; ++i)
// 			{
// 				if (buff[i] == ' ' || buff[i] == '\t')
// 					continue;
// 
// 				if (buff[i] >= '0' && buff[i] <= '9')
// 				{
// 					if (buff[i] != '0')
// 						sudoku.SetCellNumber(row, col, buff[i] - 0x30);
// 				}
// 				else
// 				{
// 					return -1;
// 				}
// 
// 				if (++col >= 9)
// 					break;
// 			}
// 
// 			if (++row >= 9)
// 				break;
// 		}
	}
	else
	{
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
	}

	sudoku.Print(std::cout);

	std::cout << std::endl;

	if (!sudoku.Decode())
		std::cout << "no decode!" << std::endl;
#ifdef _DEBUG
	else
		assert(sudoku.CheckAllCellNumber());
#endif // _DEBUG

	sudoku.Print(std::cout);

	system("pause");

    return 0;
}

