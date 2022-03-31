#include "Card.h"
Card::Card(int rank)
{
	this->rank = rank;
}

string Card::display()
{
	string a = "[";
	string b = "]";
	string transfer = to_string(this->rank);
	string display = a.append(transfer).append(b);
	return display;
}