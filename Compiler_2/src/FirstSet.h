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

struct FsetHolder {
	Rule* rule;
	int index;
};
class FirstSet {

public:
	vector<FsetHolder> first;
	bool hasEpsilon;
	FirstSet();
	virtual ~FirstSet();
};

struct FollowSet {
	vector<Rule*> followTerminals;
};
#endif /* FIRSTSET_H_ */
