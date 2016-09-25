#include <stdio.h>

#include "Cell.h"

#define LOGD printf

namespace sudokusolver {

Cell::Cell()
{
	for (int i = 0; i < 9; i++) {
		mValues[i] = true;
	}
	mBase = false;
}

Cell::Cell(int value)
{
	for (int i = 0; i < 9; i++) {
		if (value == (i+1)) {
			mValues[i] = true;
		} else {
			mValues[i] = false;
		}
	}

	mBase = true;
}

Cell::~Cell()
{

}

bool Cell::isFixed()
{
	int count = 0;

	for (int i = 0; i < 9; i++) {
		if (mValues[i]) {
			count++;
		}
	}
	return count == 1;
}

bool Cell::invalidate(int value)
{
	bool ret = mValues[value - 1];

	mValues[value - 1] = false;

	return ret;
}

int Cell::getValue()
{
	if (!isFixed()) {
		return 0;
	}

	for (int i = 0; i < 9; i++) {
		if (mValues[i]) {
			return i + 1;
		}
	}

	LOGD("invalid case at getValue!\n");

	return 0;
}

void Cell::dump()
{
	int count = 0;

	printf("(%d)", mBase);
	printf("|");
	for (int i = 0; i < 9; i++) {
		printf("%d|", mValues[i]);
		if (mValues[i]) {
			count++;
		}
	}
	printf("(%d)\n", count);
}

} //namespace sudokusolver {
