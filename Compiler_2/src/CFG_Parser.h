/*
 * FileParser.h
 *
 *  Created on: 4 May 2013
 *      Author: Mostafa Mokhtar
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include "Includes.h"
#include "Rule.h"

using namespace std;

class CFG_Parser {
public:
	char* filePath;
	CFG_Parser(char* path);
	vector<Rule*> rulesTable;
	Rule* lambda;
	Rule* DollerSign;
	Rule * sync;
	void startParser();
	void parse_Line(string line);
	int getIndexIntable(string name);
	void printOut();
	void left_factoring();
	void left_recursion();
	vector<string> realOrder;
	vector<Rule*> tempRules;
	void reOrderRulesTables();
	bool compare(vector<Rule*>* first, vector<Rule*>* second, int length);
	virtual ~CFG_Parser();
};

#endif /* FILEPARSER_H_ */
