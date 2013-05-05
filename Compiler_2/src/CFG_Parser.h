/*
 * FileParser.h
 *
 *  Created on: 4 May 2013
 *      Author: Mostafa Mokhtar
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include <string>
#include "Rule.h"

using namespace std;

class CFG_Parser {
public:
	char* filePath;
	CFG_Parser(char* path);
	vector<Rule*> rulesTable;
	Rule* lambda;
	void startParser();
	void parse_Line(string line);
	int getIndexIntable(string name);
	void printOut();
	virtual ~CFG_Parser();
};

#endif /* FILEPARSER_H_ */
