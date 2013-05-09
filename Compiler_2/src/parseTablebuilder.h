/*
 * parseTablebuilder.h
 *
 *  Created on: May 8, 2013
 *      Author: AHMAD
 */

#ifndef PARSETABLEBUILDER_H_
#define PARSETABLEBUILDER_H_
#include "Rule.h"

class parseTablebuilder {
public:
	vector<Rule*> * Rules;
	Rule* dollerSing;
	parseTablebuilder(vector<Rule *>* Rules);
	virtual ~parseTablebuilder();
	void makeFirst(Rule* lambda);
	void makeFollow(Rule * Lambda, Rule* d);
	void printer();

private:
	void findFollow(Rule* lambda, Rule* cu);
	void addFollow(vector<Rule *> r, Rule * addto);
	void addToFirst(FirstSet* f, Rule* r);

};

#endif /* PARSETABLEBUILDER_H_ */
