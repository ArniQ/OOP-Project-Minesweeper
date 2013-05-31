#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "Cell.h"
#include "Vector.h"

class MineField
{
public:
	MineField();
	MineField(int, int, int);
	MineField(const MineField&);
	~MineField();

	MineField& operator=(const MineField&);

	void setCellRepr(string[]);
	
	void openCell(int, int);
	void flagCell(int, int);
	void unflagCell(int, int);
	void closeCell(int, int);
	void openAllCells();
	void closeAllCells();
	void detonateAllMines();

	int getHeight() const;
	int getWidth() const;
	int getMines() const;
	int getCellValue(int, int) const;
	STATUS getCellStatus(int, int) const;
	string getCellRepr(int, int) const;

	void update();
	void draw() const;

private:
	int height, width, mines;
	Cell** field;
	Vector<Cell*> alteredCells;
	string cellRepresentations[12];

	void placeMines(int);
	void calculateCells();
	void adjacentEmptyCells(int, int);
	int getSurroundingMines(int, int) const;

	void copy(const MineField&);
	void del();
};

#endif