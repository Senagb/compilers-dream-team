/*
 * p_Simulator.h
 *
 *  Created on: May 13, 2013
 *      Author: zonkoly
 */

#ifndef P_SIMULATOR_H_
#define P_SIMULATOR_H_
#include "Includes.h"
#include "Rule.h"
#include "parseTablebuilder.h"
using namespace std;

//*********************DEFINITIONS********************************//
#define TERMINAL_ERROR -1
#define NONTERMINAL_ERROR -2
#define UNKOWN_ERROR -3
#define INPUT -4
#define STACK -5
#define MATCH 1
#define EXPAND 2
#define SYNC_STATE 3
#define LAMDA 4
#define CONT 0;
 void open_files();
 void close_files();
struct p_Simulator
	{
		vector<Rule*> stack;
		vector<Rule*> input;
		bool valid;
		vector<string> error;
		vector<string> result;
		string old, current;
		int c_state;
		parseTablebuilder* parseTable;
		p_Simulator(parseTablebuilder* table, vector<string> input_file);
		void construct(vector<string> inputString);
		void simulate();
		int expand();
		int getIndex_terminal(string name);
		void signal_error(int ERROR_TYPE);
		void log(string name);
		void print();
		void periodic_print();

	};

#endif /* P_SIMULATOR_H_ */
