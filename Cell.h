#ifndef SUDOKU_SOLVER_CELL_H
#define SUDOKU_SOLVER_CELL_H

namespace sudokusolver {

class Cell
{
public:
	Cell();
	Cell(int value);
	virtual ~Cell();

	bool isFixed();
	bool invalidate(int value);

	int getValue();

	void dump();

private:
	bool mValues[9];
	bool mBase;

};//class Cell

} //namespace sudokusolver {

#endif //SUDOKU_SOLVER_CELL_H
