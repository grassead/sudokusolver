#ifndef SUDOKU_SOLVER_CELL_H
#define SUDOKU_SOLVER_CELL_H

#include <array>
#include <vector>

namespace sudokusolver {

class Cell
{
public:
	Cell();
	Cell(int value);
	virtual ~Cell();

	bool isFixed();
	bool invalidate(int value);
	bool set(int value);
	bool isPossible(int value);
	bool isPossible();
	std::vector<int> getPossibleValues();

	int getValue();

	bool suppose(int value);
	bool wasAlreadySupposed(int value);

	void dump();

private:
	std::array<bool, 9> mPossibleValues;
	std::array<bool, 9> mAlreadySupposed;
	bool mBase;

};//class Cell

} //namespace sudokusolver {

#endif //SUDOKU_SOLVER_CELL_H
