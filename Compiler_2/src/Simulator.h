/*
 * Simulator.h
 *
 *  Created on: Apr 9, 2013
 *      Author: Ahmad
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "Includes.h"
#include "DFAEngine.h"
#include "NFSA.h"
#include "InputFileParser.h"
#include "DFAState.h"

struct token
	{
		string tokenType;
		string tokenconent;
	};

class Simulator
	{
	public:
		vector<token> result;
		int current_pos;
		NFSA * nfa;
		DFA_Engine* dfa;
	public:
		Simulator();
		Simulator(NFSA* n, DFA_Engine* d);
		virtual ~Simulator();
		void fileReader(string filename);
	};
static Simulator* s;

bool part1(vector<string>* sim);

#endif /* SIMULATOR_H_ */
