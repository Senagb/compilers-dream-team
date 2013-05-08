/*
 * parseTablebuilder.cpp
 *
 *  Created on: May 8, 2013
 *      Author: AHMAD
 */

#include "parseTablebuilder.h"

vector<Rule*> * Rules;

parseTablebuilder::parseTablebuilder(vector<Rule *> *Rule) {
	Rules = Rule;
}

void parseTablebuilder::makeFirst(Rule* lambda) {
	Rule * holder;
	int ruleCounter = 0;
	for (int i = Rules->size() - 1; i >= 0; i--) {
		holder = Rules->at(i);
		ruleCounter = 0;
		vector<vector<Rule *> > childern = holder->children;
		for (int j = 0; j < childern.size(); j++) {
			vector<Rule *> childernOfChildern = childern.at(j);
			for (int p = 0; p < childernOfChildern.size(); ++p) {
				Rule * chHolder = childernOfChildern.at(p);
				if (chHolder->isTerminal) {
					holder->first.first.push_back(chHolder);
					break;
				} else {
					vector<Rule *> oldFirst = chHolder->first.first;
					for (int k = 0; k < oldFirst.size(); ++k) {
						holder->first.first.push_back(oldFirst.at(k));
					}
					if (!chHolder->hasEpsilon) {
						break;
						ruleCounter++;
					}
				}
			}
			if (ruleCounter == childern.size() && childern.size() != 0)
				holder->first.first.push_back(lambda);

		}
	}
}

void parseTablebuilder::printer() {
	for (int i = 0; i < Rules->size(); ++i) {
		FirstSet f = Rules->at(i)->first;
		vector<Rule *> temp = f.first;
		for (int j = 0; j < temp.size(); ++j) {
			cout << temp.at(i)->name << " ";
		}
		cout << endl;
	}
}
parseTablebuilder::~parseTablebuilder() {
// TODO Auto-generated destructor stub
}

