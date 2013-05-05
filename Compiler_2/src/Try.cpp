//============================================================================
// Name        : Try.cpp
// Author      : Mostaga Mokhtar
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <iostream>
#include <fstream>
#include "CFG_Parser.h"
using namespace std;

int main() {

	CFG_Parser f("example.txt");
	f.startParser();

	f.printOut();
	return 0;


	return 0;
}
