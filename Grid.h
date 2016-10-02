#ifndef SUDOKU_SOLVER_GRID_H
#define SUDOKU_SOLVER_GRID_H

#include <array>
#include <vector>

#include "Cell.h"

namespace sudokusolver {


class Grid
{
public:
	Grid(int values[9][9]);
	virtual ~Grid();

	static bool resolve(Grid* grid);

	void dump();
	void advancedDump();

private:

	bool basicResolve();

	void check(int value, int x, int y, bool* isPossible, bool* isUnique);
	void checkHorizontal(int value, int x, int y, bool* isPossible, bool* isUnique);
	void checkVertical(int value, int x, int y, bool* isPossible, bool* isUnique);
	void checkArea(int value, int x, int y, bool* isPossible, bool* isUnique);


	bool findPlaceForSupposition(int* x, int* y);
	bool findValueForSupposition(int x, int y, int* value);
	bool doSupposition();
	bool undoSupposition();

	bool isSolved();
	bool check();

	void dump(std::array<std::array<Cell, 9>, 9> cells);

	std::vector<std::array<std::array<Cell, 9>, 9>> mCells;
};

} //namespace sudokusolver {


#endif //SUDOKU_SOLVER_GRID_H
