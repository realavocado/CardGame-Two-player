#pragma once
#ifndef _CARD
#define _CARD
using namespace std;
#include <iostream>
#include<string>
class Card
{
public:
	int rank; //from 1 to 10

	Card(int rank);   //constructor
	string display(); // If a card has value 1, its display is [1]
};
#endif 
