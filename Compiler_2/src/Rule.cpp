/*
 * Rule.cpp
 *
 *  Created on: 4 May 2013
 *      Author: Mostafa Mokhtar
 */

#include "Rule.h"

Rule::Rule(string nam) {
	// TODO Auto-generated constructor stub
	name = nam;
	isTerminal = false;
	hasEpsilon = false;
}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
}

