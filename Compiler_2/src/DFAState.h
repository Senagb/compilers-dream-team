/*
 * DFAState.h
 *
 *  Created on: Apr 7, 2013
 *      Author: mostafa
 */

#ifndef DFASTATE_H_
#define DFASTATE_H_

#include "state.h"
#include "Includes.h"

using namespace std;
#define    KEYWORD    "Key"
#define    PUNC       "Punc"
#define    RULE       "Rule"
#define    DEFINITION  "Def"
class DFA_State
	{

	public:

		DFA_State();

		vector<state*> compositObjects; // describes the NFSA states that the DFA state represents
		bool isFinal;
		vector<DFA_State*> childrenStates; // vector to the child states , if null then no child with this i/p
		int name; // name is by index in the DFA_STATES TABLE
		bool isNull;
		string accepted_rule;
		int its_curr_class;
		int its_next_class;

		virtual ~DFA_State();
		DFA_State* getChild(int input); // if no child with this state , returns null ??
		bool compareWithAnotherDFSA(DFA_State* dfaState);//compares of 2 DFA states matches
		void addCompositeState(state* compState);
		bool checkForExistenceAtCertainInput(state* state, int input,
		        int numOfInp);
		void checkForEmptyClosuresChildren(state* state, int l);
		bool checkExistAtInputOFEMPTYCLOSURE(state* state, int input);
		void print();
		void setFinal();

	};

#endif /* DFASTATE_H_ */
