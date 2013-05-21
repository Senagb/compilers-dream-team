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
#include "InputFileParser.h"
#include "Parser.h"
#include "NFSA.h"
#include "Simulator.h"
#include "DFAEngine.h"
#include "p_Simulator.h"

using namespace std;
static vector<string> getVector(Simulator* sim) {
	vector<string> input = vector<string>();
	for (int i = 0; i < sim->result.size(); i++)
		cout << sim->result.at(i).tokenType << endl;
	vector<token> t = sim->result;
	int size = t.size();
	for (int index = size - 1; index >= 0; index--) {
		input.push_back(t.at(index).tokenType);
	}
	cout << "GET VECTOR END" << endl;
	return input;
}
int main() {
	vector<string> k = vector<string>();
	bool result = part1(&k);
	CFG_Parser f("example.txt");
	f.startParser();
	f.left_recursion();
	f.left_factoring();
	f.printOut();
	cout << "******************************" << endl;

	parseTablebuilder builder(&f.rulesTable, f.DollerSign, f.lambda, f.sync);
	cout << "constructor" << endl;
	builder.makeFirst();
	builder.makeFollow();
	builder.makeParseTable();
	builder.printer();
	cout << "p_Simulator Creation" << endl;

	p_Simulator p_sim = p_Simulator(&builder, k);
	cout << "Start Simulation" << endl;
	p_sim.simulate();

	cout << "END of Compilers" << endl;
	return 0;
}
