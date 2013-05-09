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
	vector<Rule*> terminals;
	vector<Rule*> nonTerminals;
	Rule* dollerSing;
	Rule* lambda;
	Rule * sync;
	parseTablebuilder(vector<Rule *>* Rules, Rule* doller, Rule * Lambda,
			Rule* s);
	virtual ~parseTablebuilder();
	void makeFirst();
	void makeFollow();
	void printer();
	void makeParseTable();
	vector<Rule*>** Table;

private:
	void findFollow(Rule* lambda, Rule* cu, Rule* l);
	void addFollow(vector<FsetHolder> r, Rule * addto, Rule* lambda);
	void addFollowWithFollow(vector<Rule *> r, Rule * addto, Rule* lambda);
	void addToFirst(FirstSet* f, Rule* r, int index);
	int getRow(Rule* r);
	int getColumn(Rule* r);

};

#endif /* PARSETABLEBUILDER_H_ */
