/*
 * Rule.h
 *
 *  Created on: May 3, 2013
 *      Author: Hossam
 */

#ifndef RULE_H_
#define RULE_H_
#include "FirstSet.h"
using namespace std;

class Rule {
public:
	vector<vector<Rule*> > children;
	string name;
	bool isTerminal;
	bool hasEpsilon;
	FirstSet first;
	FollowSet follow;
	Rule(string name);
	virtual ~Rule();
};
#endif /* RULE_H_ */
