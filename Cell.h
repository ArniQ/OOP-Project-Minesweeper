#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>
using namespace std;

enum STATUS {OPEN, CLOSED, FLAGGED};

class Cell
{
public:
	Cell();
	Cell(int, STATUS);

	void setValue(int);
	void setRepr(string);

	void open();
	void close();
	void flag();
	void unflag();

	int getValue() const;
	STATUS getStatus() const;
	string getRepr() const;

	void draw() const;

private:
	int value;
	STATUS status;
	string representation;
};

#endif