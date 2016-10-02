#include <stdio.h>
#include "Grid.h"

namespace sudokusolver {

Grid::Grid(int values[9][9])
{
	std::array<std::array<Cell, 9>, 9> cells;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (values[i][j] != 0) {
				cells[i][j] = Cell(values[i][j]);
			} else {
				cells[i][j] = Cell();
			}
		}
	}

	mCells.push_back(cells);
}

Grid::~Grid()
{

}

void Grid::dump()
{
	mCells.shrink_to_fit();
	printf(" +---------------------+\n");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (j % 3 == 0) {
				printf(" |");
			}
			if ((mCells.back())[i][j].getValue() != 0) {
				printf("%d|", (mCells.back())[i][j].getValue());
			} else {
				printf("\033[31m%d\e[0m|", (mCells.back())[i][j].getValue());
			}
		}
		if (((i+1) % 3 == 0) && i != 8) {
			printf("\n |                     |");
		}
		printf("\n");
	}
	printf(" +---------------------+\n");
}

void Grid::advancedDump()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			(mCells.back())[i][j].dump();
		}
	}
}

bool Grid::resolve(Grid* grid)
{
	bool solved;
	bool suppositionDone;
	bool ret;

	if (!grid->check()) {
		return false;
	}

	if (!grid->basicResolve()) {
		return false;
	}

	solved = grid->isSolved();

	if (!solved) {
		do {
			suppositionDone = grid->doSupposition();
			if (suppositionDone) {
				ret = Grid::resolve(grid);
				if (!ret) {
					grid->undoSupposition();
				}
			}
		} while (suppositionDone && !ret);
	}

	return grid->check();
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
					if ((mCells.back())[i][j].isPossible(value)) {
						check(value, i, j, &isPossible, &isUnique);
						if (!isPossible) {
							updated |= (mCells.back())[i][j].invalidate(value);
							if (!(mCells.back())[i][j].isPossible()) {
								return false;
							}
						} else if (isUnique) {
							updated |= (mCells.back())[i][j].set(value);
						}
					}
				}
			}
		}
	} while (updated);

	return true;
}

bool Grid::findValueForSupposition(int x, int y, int* value)
{
	int supposition = -1;

	for (int lvalue: mCells.back()[x][y].getPossibleValues()) {
		if (!mCells.back()[x][y].wasAlreadySupposed(lvalue)) {
			supposition = lvalue;
			break;
		}
	}

	if (supposition == -1) {
		return false;
	}

	*value = supposition;

	return true;
}

bool Grid::findPlaceForSupposition(int* x, int* y)
{
	int betterX = -1;
	int betterY = -1;
	int betterPossibilityCount = 10;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			std::vector<int> values;
			values = mCells.back()[i][j].getPossibleValues();
			if (values.size() > 1 && values.size() < betterPossibilityCount) {
				betterX = i;
				betterY = j;
				betterPossibilityCount = values.size();
				if (betterPossibilityCount == 2) {
					//We find the better place, do not need to fetch all the grid
					goto exit;
				}
			}
		}
	}

	if (betterX == -1) {
		return false;
	}

exit:
	*x = betterX;
	*y = betterY;

	return true;
}

bool Grid::doSupposition()
{
	int x;
	int y;
	int supposition;
	std::array<std::array<Cell, 9>, 9> newCells;

	if (!findPlaceForSupposition(&x, &y)) {
		return false;
	}

	if (!findValueForSupposition(x, y, &supposition)) {
		return false;
	}

	// Mark this value as tryed
	mCells.back()[x][y].suppose(supposition);

	//Copy the current grid
	newCells = mCells.back();

	//Set the supposed  value
	newCells[x][y].set(supposition);

	//Mark the new grid as current
	mCells.push_back(newCells);

	return true;
}

void Grid::undoSupposition()
{
	mCells.pop_back();
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

		if ((mCells.back())[x][i].getValue() == value) {
			*isPossible = false;
			*isUnique   = false;
			break;
		}

		if ((mCells.back())[x][i].isPossible(value)) {
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

		if ((mCells.back())[i][y].getValue() == value) {
			*isPossible = false;
			*isUnique   = false;
			break;
		}

		if ((mCells.back())[i][y].isPossible(value)) {
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

			if ((mCells.back())[macroX + i][macroY + j].getValue() == value) {
				*isPossible = false;
				*isUnique   = false;
				goto exit;
			}

			if ((mCells.back())[macroX + i][macroY + j].isPossible(value)) {
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
			if (!(mCells.back())[i][j].isFixed()) {
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
			if (!(mCells.back())[i][j].isFixed()) {
				if (!(mCells.back())[i][j].isPossible()) {
					return false;
				}
			} else {
				check((mCells.back())[i][j].getValue(), i, j, &isPossible, &isUnique);
				if (!isPossible) {
					return false;
				}
			}
		}
	}

	return true;
}
} //namespace sudokusolver {
