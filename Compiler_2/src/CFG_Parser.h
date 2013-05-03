/*
 * CFG_Parser.h
 *
 *  Created on: May 3, 2013
 *      Author: ALEX-2010
 */

#ifndef CFG_PARSER_H_
#define CFG_PARSER_H_
#include "Rule.h"
#include "Includes.h"
using namespace std;

class CFG_Parser {
public:
	vector<Rule*> rulesTable;
	Rule* lambda;
	CFG_Parser();
	virtual ~CFG_Parser();
};

#endif /* CFG_PARSER_H_ */
