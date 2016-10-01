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
	if (!grid->check()) {
		return false;
	}

	if (!grid->basicResolve()) {
		return false;
	}

	return grid->isSolved();
}

bool Grid::basicResolve()
{
	bool updated;
	bool isPossible;
	bool isUnique;

	do {
		updated = false;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				for (int value = 1; value <= 9; value++) {
					if (mCells[i][j]->isPossible(value)) {
						check(value, i, j, &isPossible, &isUnique);
						if (!isPossible) {
							updated |= mCells[i][j]->invalidate(value);
							if (!mCells[i][j]->isPossible()) {
								return false;
							}
						} else if (isUnique) {
							updated |= mCells[i][j]->set(value);
						}
					}
				}
			}
		}
	} while (updated);

	return true;
}

void Grid::check(int value, int x, int y, bool* isPossible, bool* isUnique)
{
	checkHorizontal(value, x, y , isPossible, isUnique);
	if (!(*isPossible)) {
		return;
	}

	checkVertical(value, x, y , isPossible, isUnique);
	if (!(*isPossible)) {
		return;
	}

	checkArea(value, x, y , isPossible, isUnique);
}

void Grid::checkHorizontal(int value, int x, int y, bool* isPossible, bool* isUnique)
{
	*isPossible = true;
	*isUnique   = true;

	for (int i = 0; i < 9; i++) {
		if (i == y) {
			continue;
		}

		if (mCells[x][i]->getValue() == value) {
			*isPossible = false;
			*isUnique   = false;
			break;
		}

		if (mCells[x][i]->isPossible(value)) {
			*isUnique = false;
		}
	}
}

void Grid::checkVertical(int value, int x, int y, bool* isPossible, bool* isUnique)
{
	*isPossible = true;
	*isUnique   = true;

	for (int i = 0; i < 9; i++) {

		if (i == x) {
			continue;
		}

		if (mCells[i][y]->getValue() == value) {
			*isPossible = false;
			*isUnique   = false;
			break;
		}

		if (mCells[i][y]->isPossible(value)) {
			*isUnique = false;
		}
	}
}

void Grid::checkArea(int value, int x, int y, bool* isPossible, bool* isUnique)
{
	int macroX = ((int)(x / 3) * 3);
	int macroY = ((int)(y / 3) * 3);

	*isPossible = true;
	*isUnique   = true;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			if (((macroX + i) == x) && ((macroY + j) == y)) {
				continue;
			}

			if (mCells[macroX + i][macroY + j]->getValue() == value) {
				*isPossible = false;
				*isUnique   = false;
				goto exit;
			}

			if (mCells[macroX + i][macroY + j]->isPossible(value)) {
				*isUnique = false;
			}
		}
	}

exit:
	return;
}

bool Grid::isSolved()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (!mCells[i][j]->isFixed()) {
				return false;
			}
		}
	}

	return check();
}

bool Grid::check()
{
	bool isPossible;
	bool isUnique;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (!mCells[i][j]->isFixed()) {
				if (!mCells[i][j]->isPossible()) {
					return false;
				}
			} else {
				check(mCells[i][j]->getValue(), i, j, &isPossible, &isUnique);
				if (!isPossible) {
					return false;
				}
			}
		}
	}

	return true;
}
} //namespace sudokusolver {
