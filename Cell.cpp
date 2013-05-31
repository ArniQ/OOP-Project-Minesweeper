#include "Cell.h"

Cell::Cell(): 
value(), status(CLOSED), representation(" ")
{
}

Cell::Cell(int _value, STATUS _status=CLOSED):
value(_value), status(_status), representation("?")
{
}

void Cell::setValue(int _value)
{
	value = _value;
}

void Cell::setRepr(string _representation)
{
	representation = _representation;
}

void Cell::open()
{
	status = OPEN;
}

void Cell::close()
{
	status = CLOSED;
}

void Cell::flag()
{
	status = FLAGGED;
}

void Cell::unflag()
{
	status = CLOSED;
}

int Cell::getValue() const
{
	return value;
}

STATUS Cell::getStatus() const
{
	return status;
}

string Cell::getRepr() const
{
	return representation;
}

void Cell::draw() const
{
	std::cout << representation;
}