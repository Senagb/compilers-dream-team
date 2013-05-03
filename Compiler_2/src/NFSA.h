/*
 * NFSA.h
 *
 *  Created on: Apr 7, 2013
 *      Author: zonkoly
 */

#ifndef NFSA_H_
#define NFSA_H_

#include "Includes.h"
#include "state.h"
#include "InputFileParser.h"

using namespace std;
/*Definitions */
#define max  100000000
#define space ' '
#define OR '|'
#define one_or_more '+'
#define zero_or_more '*'
#define open '('
#define close ')'
#define error_empty -11
#define Space_Key 0
#define OR_Key 1
#define Plus_Key 2
#define Zero_Key 3
#define Open_Key 4
#define Close_key 5
#define cont -1

/****************************/

class NFSA
	{
		InputFileParser file;
		vector<string> definitions;
		state* start;
		vector<string> errors;
		vector<state*> states;
		bool valid;

	public:
		int index_array[256];
		NFSA(InputFileParser file);
		NFSA();
		virtual ~NFSA();
		vector<parsed> getRuleTable();
		vector<parsed> getDefinitionTable();
		void generateNFSA();
		state* getStart();
		vector<string> getDefinitions();
		void printErrors();
		bool isValid();
		void printGraph();
		void printGraph1();
		int translate_to_index(int Ascii);
	private:
		void combine();
		void signal_error(vector<string> errors, parsed current, int index);

	};

#endif /* NFSA_H_ */
