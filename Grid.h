#ifndef SUDOKU_SOLVER_GRID_H
#define SUDOKU_SOLVER_GRID_H

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

	bool isAUniqueSolution(int value, int x, int y);
	bool isAUniqueSolutionInVerticalRaw(int value, int x, int y);
	bool isAUniqueSolutionInHorizontalRaw(int value, int x, int y);
	bool isAUniqueSolutionInArea(int value, int x, int y);

	bool isPossible(int value, int x, int y);
	bool isPossibleInVerticalRaw(int value, int x);
	bool isPossibleInHorizontalRaw(int value, int y);
	bool isPossibleInArea(int value, int x, int y);

	bool isSolved();

	Cell* mCells[9][9];
};

} //namespace sudokusolver {


#endif //SUDOKU_SOLVER_GRID_H
