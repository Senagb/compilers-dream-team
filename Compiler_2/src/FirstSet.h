/*
 * FirstSet.h
 *
 *  Created on: May 8, 2013
 *      Author: AHMAD
 */

#ifndef FIRSTSET_H_
#define FIRSTSET_H_
#include "Includes.h"

using namespace std;
class Rule;

class FirstSet {

public:
	vector<Rule*> first;
	string name;
	bool hasEpsilon;
	FirstSet();
	virtual ~FirstSet();
};

#endif /* FIRSTSET_H_ */
