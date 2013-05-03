/*
 * Rule.h
 *
 *  Created on: May 3, 2013
 *      Author: Hossam
 */

#ifndef RULE_H_
#define RULE_H_
#include "Includes.h"

using namespace std;

class Rule {

public:
	vector< vector< Rule* > > children;
	string name;
	bool isTerminal;
	Rule();
	virtual ~Rule();
};

#endif /* RULE_H_ */
