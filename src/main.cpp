/*  This program solves a given sudoku by applying simple algorithms that mortals use to solve sudokus
    Copyright (C) 2016  Kiran Dhana, Sireesh Kodali

    Sudoku-solver is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Sudoku-solver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    contact - 
    sireeshkodali1@gmail.com
    dkiran100@gmail.com
    */
#include <chrono>
#include <iostream>
#include "Sudoku-solver.h"
#include "termcolor.hpp"

using namespace CSudokuSolver;

int main(int argc, char *argv[])
{
	bool print_steps = false, silent = false;
	while (--argc > 0 && (*++argv)[0] == '-')
	{
		char c;
		while((c = *++argv[0]))
			switch (c)
			{
				case 'l':
					std::cout << "Sudoku-solver (C) 2016 Kiran Dhana and Sireesh Kodali.\n";
					std::cout << "This program comes with ABSOLUTLY NO WARRANTY; for details check license.txt\n";
					std::cout << "This program is free software, and you are welcome to redistribute it under\n";
					std::cout << "certain conditions; check license.txt for more details\n\n";
					return 0;
				case 'h':
					std::cout << "Sudoku-solver : version 0.9 (Dharma)\n\n";
					std::cout << "usage : Sudoku-solver [arguments]\n";
					std::cout << "Arguments:\n";
					std::cout << " -l\t Print license info and exit\n";
					std::cout << " -h\t Print this help menu and exit\n";
					std::cout << " -s\t Only print unformatted input and answer (useful for automated solving of sudokus)\n";
					std::cout << " -t\t Print how to solve the given sudoku (step by step solution!)\n";
					std::cout << " -v\t Print version info and exit\n";
					return 0;
				case 't' :
					if (!silent)
						print_steps = true;
					else
					{
						std::cout << termcolor::red << "Conflicting options! -s and -t can not be used together!\nExiting!\n" << termcolor::reset;
						return 1;
					}
					break;
				case 'v' :
					std::cout << "Sudoku-solver : version 0.9 (Dharma)\n";
					std::cout << "Built with command: $ clang++ -I. Sudoku-solver.cpp main.cpp -o ./bin/Sudoku-solver -std=c++11 -O3 -march=native\n";
					return 0;
				case 's' :
					if (!print_steps)
						silent = true;
					else
					{
						std::cout << termcolor::red << "Conflicting options! -s and -t can not be used together\nExiting!\n" << termcolor::reset;
						return 1;
					}
					break;
				default :
					std::cout << termcolor::red << "Invalid Option : " << c << termcolor::reset << '\n';
					std::cout << "usage : Sudoku-solver [arguments]\n";
					std::cout << "Arguments:\n";
					std::cout << " -l\t Print license info and exit\n";
					std::cout << " -h\t Print this help menu and exit\n";
					std::cout << " -s\t Only print unformatted input and answer (useful for automated solving of sudokus)\n";
					std::cout << " -t\t Print how to solve the given sudoku (step by step solution!)\n";
					std::cout << " -v\t Print version info and exit\n";
					return -1;
			}
	}

	SUDOKU_ANS_BOARD sudoku_ans;
	int sudoku_q[9][9];

	if (!inputSudoku(sudoku_q))
	{
		std::cout << "Exiting ...\n";
		return -2;
	}

	int sudoku_a[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			sudoku_a[i][j] = sudoku_q[i][j];

	if (!initialiseSudoku(sudoku_q, sudoku_ans) || count(sudoku_q) < 17) 
	{ 
		std::cerr << "The input sudoku is invalid! It contains too few numbers or an impossible question.\n";
		printSudoku(sudoku_q);
		std::cout << termcolor::red << "The sudoku contains " << count(sudoku_q) << " clues.\n" << termcolor::reset;
		return -1;
	}

	if (!silent)
	{
		std::cout << "The given sudoku is :\n";
		printfSudoku(sudoku_q, sudoku_a);
	}

	if (!silent)
		std::cout << "Given : " << termcolor::red << count(sudoku_q) << termcolor::reset << '\n';

	auto sTime = std::chrono::high_resolution_clock::now();

	while (count(sudoku_a) < 81 && sudoku_ans.changed) 
	{
		sudoku_ans.changed = false;
		checkColumns(sudoku_ans, sudoku_a, print_steps);
		checkRows(sudoku_ans, sudoku_a, print_steps);
		nakedSingle(sudoku_ans, sudoku_a, print_steps);
		checkBox(sudoku_ans, sudoku_a, print_steps);
		if (!sudoku_ans.changed) 
			nakedPair(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			nakedTriple(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			pointingBoxColumns(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			pointingBoxRows(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			boxLineReduceRow(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			boxLineReduceColumn(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			xWing(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			bruteForce(sudoku_ans, sudoku_a, print_steps);
	}

	auto eTime = std::chrono::high_resolution_clock::now();	

	if (checkError(sudoku_ans, sudoku_a)) 
	{
		std::cerr << "Something went wrong!\n";
		printSudoku(sudoku_a);
		return -3;
	}

	if (!silent)
	{
		printfSudoku(sudoku_q, sudoku_a);
		std::cout << "Answered : " << termcolor::green << count(sudoku_a) << termcolor::reset << '\n';
		std::cout << "Time taken = " << std::chrono::duration_cast<std::chrono::nanoseconds>(eTime - sTime).count() * 1E-6 << " milliseconds\n";
	}
	else 
		printSudoku(sudoku_a);

	return 0;
}