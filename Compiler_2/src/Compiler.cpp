//============================================================================
// Name        : Compiler.cpp
// Author      : ahmad magdy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Includes.h"
#include "InputFileParser.h"
#include "Parser.h"
#include "NFSA.h"
#include "Simulator.h"
#include "DFAEngine.h"
using namespace std;

bool part1(vector<string>* res)
{

	file = new InputFileParser();
	file->readFile("input.txt");
	n = new NFSA(*file);
	n->generateNFSA();
	n->printErrors();
	cout << "hello world" << endl;

	if (n->isValid())
	{
		dfa_Eng = new DFA_Engine(n, n->getDefinitions().size());
		dfa_Eng->getFirstDFA_State();
		dfa_Eng->generateTable();
		dfa_Eng->print();
		dfa_Eng->minimize_table();

		s = new Simulator(n, dfa_Eng);
		s->fileReader("code.txt");

		for (int i = 0; i < s->result.size(); i++)
			res->push_back("'"+s->result.at(i).tokenType+"'");
	} else
	{
		return false;
	}
	cout << "END" << endl;
	return true;
}
