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
#include "parseTablebuilder.h"
using namespace std;

int main() {

	CFG_Parser f("example.txt");
	f.startParser();
	f.printOut();
	cout << "******************************" << endl;
	parseTablebuilder builder(&f.rulesTable);
	builder.makeFirst(f.lambda);
	builder.printer();
	return 0;

	return 0;
}
