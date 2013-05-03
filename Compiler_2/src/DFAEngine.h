/*
 * DFAEngine.h
 *
 *  Created on: Apr 7, 2013
 *      Author: mostafa
 */

#ifndef DFAENGINE_H_
#define DFAENGINE_H_


#include "Includes.h"
#include "DFAState.h"
#include "ClassOfStates.h"
#include "state.h"
#include "NFSA.h"


using namespace std;




class DFA_Engine {


public:
	NFSA* NfsaEngine;
	vector< DFA_State*> dfaStates;
	state* startState;
	DFA_State* start_state_minimized;
	int numOfInput;
	int name;
	vector< DFA_State* > minimizedTable;



	DFA_Engine(NFSA* ourNFSA,int numOfInp);
	virtual ~DFA_Engine();
	void generateTable();
	void getFirstDFA_State();
	void minimize_table();
	vector<DFA_State* >* getMinimizedTabel();
	DFA_State* getStart();
	void print();
	void update_its_class(vector<ClassOfStates>* vec);



};

#endif /* DFAENGINE_H_ */
