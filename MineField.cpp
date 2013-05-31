#include "MineField.h"
#include <cmath>

MineField::MineField():
height(0), width(0), mines(0), alteredCells()
{
}

MineField::MineField(int _height, int _width, int _mines):
height(_height), width(_width), mines(_mines), alteredCells()
{
	cellRepresentations[0] = "*";
	cellRepresentations[1] = " ";
	for (int i = 2; i < 10; i++)
	{
		cellRepresentations[i] = std::string(1, char('0' + i - 1));
	}
	cellRepresentations[10] = string(1, '!');
	cellRepresentations[11] = string(1, '?');

	field = new Cell*[height];
	for (int i = 0; i < height;i++)
	{
		field[i] = new Cell[width];
	}

	placeMines(mines);
	calculateCells();
}

MineField::MineField(const MineField& other)
{
	copy(other);
}

MineField::~MineField()
{
	del();
}

MineField& MineField::operator=(const MineField& other)
{
	if (this != &other)
	{
		del();
		copy(other);
	}

	return *this;
}

void MineField::setCellRepr(string _cellRepresentations[])
{
	for (int i = 0; i < 12; i++)
	{
		cellRepresentations[i] = _cellRepresentations[i];
	}
}

void MineField::openCell(int x, int y)
{
	if (field[x][y].getStatus() == CLOSED)
	{
		if (field[x][y].getValue() != 0)
		{
			field[x][y].open();
			alteredCells.addElement(&field[x][y]);
		}
		else
		{
			adjacentEmptyCells(x, y);
		}
	}
}

void MineField::flagCell(int x, int y)
{
	if (field[x][y].getStatus() == CLOSED)
	{
		field[x][y].flag();
		alteredCells.addElement(&field[x][y]);
	}
}

void MineField::unflagCell(int x, int y)
{
	if (field[x][y].getStatus() == FLAGGED)
	{
		field[x][y].unflag();
		alteredCells.addElement(&field[x][y]);
	}
}

void MineField::closeCell(int x, int y)
{
	if (field[x][y].getStatus() != CLOSED)
	{
		field[x][y].close();
		alteredCells.addElement(&field[x][y]);
	}
}

void MineField::openAllCells()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			openCell(i, j);
		}
	}
}

void MineField::closeAllCells()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			closeCell(i, j);
		}
	}
}

void MineField::detonateAllMines()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (field[i][j].getValue() == -1 && field[i][j].getStatus() != FLAGGED)
			{
				openCell(i, j);
			}
		}
	}
}

void MineField::placeMines(int mines)
{
	int randomX, randomY, minesCounter = 0;

	while(minesCounter < mines)
	{
		randomX = rand() % height;
		randomY = rand() % width;

		if (field[randomX][randomY].getValue() == 0)
		{
			field[randomX][randomY].setValue(-1);
			alteredCells.addElement(&field[randomX][randomY]);
			minesCounter++;
		}
	}	
}

int MineField::getHeight() const
{
	return height;
}

int MineField::getWidth() const
{
	return width;
}

int MineField::getMines() const
{
	return mines;
}

int MineField::getCellValue(int x, int y) const
{
	return field[x][y].getValue();
}

STATUS MineField::getCellStatus(int x, int y) const
{
	return field[x][y].getStatus();
}

std::string MineField::getCellRepr(int x, int y) const
{
	return field[x][y].getRepr();
}

void MineField::update()
{
	for (int i = 0; i < alteredCells.getSize(); i++)
	{
		if (alteredCells[i]->getStatus() == CLOSED)
		{
			alteredCells[i]->setRepr(cellRepresentations[11]);
		}
		else if (alteredCells[i]->getStatus() == FLAGGED)
		{
			alteredCells[i]->setRepr(cellRepresentations[10]);
		}
		else
		{
			alteredCells[i]->setRepr(cellRepresentations[alteredCells[i]->getValue() + 1]);
		}
	}

	alteredCells.clear();
}

void MineField::draw() const
{
	int heightDigits = int(log10(double(height))) + 1;

	cout << "  ";
	for (int i = 0; i < heightDigits; i++)
	{
		cout << " ";
	}

	for (int i = 0; i < width; i++)
	{
		std::cout << i ;
		if (i < 9)
		{
			cout << " ";
		}
	}
	cout << endl;

	cout << "  ";
	for (int i = 0; i < heightDigits; i++)
	{
		cout << " ";
	}

	for (int i = 0; i < width; i++)
	{
		cout << "- ";
	}
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		cout << i;
		for (int j = 0; j <= heightDigits  - int(log10(double(i==0? 1:i))); j++)
		{
			cout << " ";
		}

		for (int j = 0; j < width; j++)
		{
			field[i][j].draw();
			cout << ' ';
		}

		cout << endl;
	}

	cout << "  ";
	for (int i = 0; i < heightDigits; i++)
	{
		cout << " ";
	}

	for (int i = 0; i < width; i++)
	{
		std::cout << "- ";
	}
	cout << endl;
}

void MineField::calculateCells()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (field[i][j].getValue() != -1)
			{
				field[i][j].setValue(getSurroundingMines(i, j));
				alteredCells.addElement(&field[i][j]);
			}
		}
	}
}

void MineField::adjacentEmptyCells(int x, int y)
{
	if (field[x][y].getStatus() == CLOSED)
	{
		field[x][y].open();
		alteredCells.addElement(&field[x][y]);
	}
	
	if (field[x][y].getValue() != -1 && field[x][y].getValue() != 0)
		return;

	if (x+1 >= 0 && x+1 < height && y >= 0 && y < width && field[x+1][y].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x+1, y);
	}

	if (x-1 >= 0 && x-1 < height && y >= 0 && y < width && field[x-1][y].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x-1, y);
	}

	if (x >= 0 && x < height && y+1 >= 0 && y+1 < width && field[x][y+1].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x, y+1);
	}

	if (x >= 0 && x < height && y-1 >= 0 && y-1 < width && field[x][y-1].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x, y-1);
	}

	if (x+1 >= 0 && x+1 < height && y+1 >= 0 && y+1 < width && field[x+1][y+1].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x+1, y+1);
	}

	if (x+1 >= 0 && x+1 < height && y-1 >= 0 && y-1 < width && field[x+1][y-1].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x+1, y-1);
	}

	if (x-1 >= 0 && x-1 < height && y+1 >= 0 && y+1 < width && field[x-1][y+1].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x-1, y+1);
	}

	if (x-1 >= 0 && x-1 < height && y-1 >= 0 && y-1 < width && field[x-1][y-1].getStatus() == CLOSED)
	{
		adjacentEmptyCells(x-1, y-1);
	}
}

int MineField::getSurroundingMines(int x, int y) const
{
	bool firstCell = (x-1 >= 0) && (x-1 < height) && (field[x-1][y].getValue() == -1);

	bool secondCell = (y-1 >= 0) && (y-1 < width) && (field[x][y-1].getValue() == -1);

	bool thirdCell = (x+1 >= 0) && (x+1 < height) && (field[x+1][y].getValue() == -1);

	bool fourthCell = (y+1 >= 0) && (y+1 < width) && (field[x][y+1].getValue() == -1);

	bool fifthCell = (x-1 >= 0) && (x-1 < height) && (y+1 >= 0) && (y+1 < width) && 
		(field[x-1][y+1].getValue() == -1);

	bool sixthCell = (x+1 >= 0) && (x+1 < height) && (y+1 >= 0) && (y+1 < width) &&
		(field[x+1][y+1].getValue() == -1);

	bool seventhCell = (x-1 >= 0) && (x-1 < height) && (y-1 >= 0) && (y-1 < width) &&
		(field[x-1][y-1].getValue() == -1);

	bool eigthCell = (x+1 >= 0) && (x+1 < height) && (y-1 >= 0) && (y-1 < width) &&
		(field[x+1][y-1].getValue() == -1);

	return firstCell + secondCell + thirdCell + fourthCell + fifthCell + sixthCell + seventhCell + eigthCell;
}

void MineField::copy(const MineField& other)
{
	height = other.height;
	width = other.width;
	mines = other.mines;
	
	field = new Cell*[height];
	for (int i = 0; i < height; i++)
	{
		field[i] = new Cell[width];
		for (int j = 0; j < width; j++)
		{
			field[i][j] = other.field[i][j];
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			alteredCells.addElement(&field[i][j]);
		}
	}

	for (int i = 0; i < 12; i++)
	{
		cellRepresentations[i] = other.cellRepresentations[i];
	}
}

void MineField::del()
{
	for (int i = 0; i < height; i++)
	{
		delete [] field[i];
	}

	delete [] field;
}
