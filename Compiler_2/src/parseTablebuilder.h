/*
 * parseTablebuilder.h
 *
 *  Created on: May 8, 2013
 *      Author: AHMAD
 */

#ifndef PARSETABLEBUILDER_H_
#define PARSETABLEBUILDER_H_
#include "Includes.h"
#include "Rule.h"
#include "FirstSet.h"

class parseTablebuilder {
public:
	parseTablebuilder(vector<Rule *>* Rules);
	virtual ~parseTablebuilder();
	void makeFirst(Rule* lambda);
	void printer();

};

#endif /* PARSETABLEBUILDER_H_ */
