/*
 * ClassOfStates.h
 *
 *  Created on: Apr 9, 2013
 *      Author: ALEX-2010
 */

#ifndef CLASSOFSTATES_H_
#define CLASSOFSTATES_H_

#include "DFAState.h"
#include "Includes.h"



using namespace std;

class ClassOfStates {
public:
	int name;
	vector< DFA_State* > compStates;
	string trans;
	ClassOfStates();
	virtual ~ClassOfStates();
};


#endif /* CLASSOFSTATES_H_ */
