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

int main(int argc, char *argv[]) {

//	if (argc < 3) {
//		cout << "please Enter the rule file name and the code file name"
//				<< endl;
//	} else {

	InputFileParser* file = new InputFileParser();
//	file->readFile("input.txt");
		file->readFile(argv[1]);
	NFSA* n = new NFSA(*file);
	n->generateNFSA();
	n->printErrors();
	cout << "hello world" << endl;

	if (n->isValid()) {
		DFA_Engine* dfa_Eng = new DFA_Engine(n, n->getDefinitions().size());
		dfa_Eng->getFirstDFA_State();
		dfa_Eng->generateTable();
		dfa_Eng->print();
		dfa_Eng->minimize_table();

		Simulator* s = new Simulator(n, dfa_Eng);
//		s->fileReader("code.txt");
		s->fileReader(argv[2]);

//		}
	}
	cout << "END" << endl;
	return 0;
}
