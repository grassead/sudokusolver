#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"

#define LOGD printf

namespace sudokusolver {

Cell::Cell()
{
	for (int i = 0; i < 9; i++) {
		mPossibleValues[i] = true;
		mAlreadySupposed[i] = false;
	}
	mBase = false;
	mFixed = false;
}

Cell::Cell(int value)
{
	for (int i = 0; i < 9; i++) {
		if (value == (i+1)) {
			mPossibleValues[i] = true;
		} else {
			mPossibleValues[i] = false;
		}
		mAlreadySupposed[i] = false;
	}

	mBase = true;
	mFixed = true;
}


Cell::~Cell()
{

}

bool Cell::isFixed()
{
	return mFixed;
}

bool Cell::invalidate(int value)
{
	bool ret = mPossibleValues[value - 1];

	mPossibleValues[value - 1] = false;

	//An update occurs
	if (ret) {
		int count = 0;
		for (int i = 0; i < 9; i++) {
			if (isPossible(i+1)) {
				count++;
				if (count > 1) {
					break;
				}
			}
		}

		mFixed = (count == 1);
	}

	return ret;
}

bool Cell::set(int value)
{
	if (isFixed()) {
		if (value == getValue()) {
			return false;
		} else {
			abort();
		}
	}

	for (int i = 0; i < 9; i++) {
		if (value == (i+1)) {
			mPossibleValues[i] = true;
		} else {
			mPossibleValues[i] = false;
		}
	}

	mFixed = true;
	return true;
}

bool Cell::isPossible()
{
	for (int i = 1;  i <= 9; i++) {
		if (isPossible(i)) {
			return true;
		}
	}

	return false;
}

bool Cell::isPossible(int value)
{
	return mPossibleValues[value - 1];
}

int Cell::getValue()
{
	if (!isFixed()) {
		return 0;
	}

	for (int i = 0; i < 9; i++) {
		if (mPossibleValues[i]) {
			return i + 1;
		}
	}

	LOGD("invalid case at getValue!\n");

	return 0;
}

bool Cell::suppose(int value)
{
	bool ret = mAlreadySupposed[value - 1];

	mAlreadySupposed[value - 1] = true;

	return ret;
}

bool Cell::wasAlreadySupposed(int value)
{
	return mAlreadySupposed[value - 1];
}

std::vector<int> Cell::getPossibleValues()
{
	std::vector<int> values;

	for (int i = 0; i < 9; i++) {
		if (mPossibleValues[i]) {
			values.push_back(i+1);
		}
	}

	return values;
}

std::vector<int> Cell::getPossibleSupposition()
{
	std::vector<int> values;

	for (int i = 0; i < 9; i++) {
		if (mPossibleValues[i] && !wasAlreadySupposed(i+1)) {
			values.push_back(i+1);
		}
	}

	return values;
}

void Cell::dump()
{
	int count = 0;

	printf("(%d)", mBase);
	printf("|");
	for (int i = 0; i < 9; i++) {
		printf("%d|", mPossibleValues[i]);
		if (mPossibleValues[i]) {
			count++;
		}
	}
	printf("(%d)\n", count);
}

} //namespace sudokusolver {
