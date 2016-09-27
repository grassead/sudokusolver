#include <stdio.h>
#include "Grid.h"

namespace sudokusolver {


Grid::Grid(int values[9][9])
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (values[i][j] != 0) {
				mCells[i][j] = new Cell(values[i][j]);
			} else {
				mCells[i][j] = new Cell();
			}
		}
	}
}

Grid::~Grid()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			delete mCells[i][j];
		}
	}
}

void Grid::dump()
{
	printf(" +---------------------+\n");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (j % 3 == 0) {
				printf(" |");
			}
			if (mCells[i][j]->getValue() != 0) {
				printf("%d|", mCells[i][j]->getValue());
			} else {
				printf("\033[31m%d\e[0m|", mCells[i][j]->getValue());
			}
		}
		if (((i+1) % 3 == 0) && i != 8) {
			printf("\n");
		}
		printf("\n");
	}
	printf(" +---------------------+\n");
}

void Grid::advancedDump()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			mCells[i][j]->dump();
		}
	}
}

bool Grid::resolve(Grid* grid)
{
	bool resolved;

	resolved = grid->basicResolve();
/*
	while (!resolved) {

	}
*/
	return resolved;
}

bool Grid::basicResolve()
{
	bool updated;
	do {
		updated = false;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				for (int value = 1; value <= 9; value++) {
					if (mCells[i][j]->isFixed()) {
						continue;
					}
					if (!isPossible(value, i, j)) {
						updated |= mCells[i][j]->invalidate(value);
					} else if (isAUniqueSolution(value, i, j)) {
						updated |= mCells[i][j]->force(value);
					}
				}
			}
		}
	} while (updated);

	return isSolved();
}

bool Grid::isAUniqueSolution(int value, int x, int y)
{
	if (isAUniqueSolutionInVerticalRaw(value, x, y) ||
	    isAUniqueSolutionInHorizontalRaw(value, x, y) ||
	    isAUniqueSolutionInArea(value, x, y)) {
		return true;
	}

	return false;
}

bool Grid::isAUniqueSolutionInVerticalRaw(int value, int x, int y)
{
	for (int i = 0; i < 9; i++) {
		if (i != y && mCells[x][i]->isPossible(value)) {
			return false;
		}
	}

	return true;
}

bool Grid::isAUniqueSolutionInHorizontalRaw(int value, int x, int y)
{
	for (int i = 0; i < 9; i++) {
		if (i != x && mCells[i][y]->isPossible(value)) {
			return false;
		}
	}

	return true;
}

bool Grid::isAUniqueSolutionInArea(int value, int x, int y)
{
	int macroX = ((int)(x / 3) * 3);
	int macroY = ((int)(y / 3) * 3);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (((macroX + i) == x) && ((macroY + j) == y)) {
				continue;
			}
			if (mCells[macroX + i][macroY + j]->isPossible(value)) {
				return false;
			}
		}
	}

	return true;
}

bool Grid::isPossibleInVerticalRaw(int value, int x)
{
	for (int i = 0; i < 9; i++) {
		if (mCells[x][i]->getValue() == value) {
			return false;
		}
	}
	return true;
}

bool Grid::isPossibleInHorizontalRaw(int value, int y)
{
	for (int i = 0; i < 9; i++) {
		if (mCells[i][y]->getValue() == value) {
			return false;
		}
	}
	return true;
}

bool Grid::isPossibleInArea(int value, int x, int y)
{
	int macroX = ((int)(x / 3) * 3);
	int macroY = ((int)(y / 3) * 3);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (mCells[macroX + i][macroY + j]->getValue() == value) {
				return false;
			}
		}
	}

	return true;
}

bool Grid::isPossible(int value, int x, int y)
{
	return (isPossibleInArea(value, x, y)       &&
			isPossibleInHorizontalRaw(value, y) &&
			isPossibleInVerticalRaw(value, x));
}

bool Grid::isSolved()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (!mCells[i][j]->isFixed()) {
				return false;
			}
		}
	}

	return true;
}
} //namespace sudokusolver {
