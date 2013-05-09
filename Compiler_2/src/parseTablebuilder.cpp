/*
 * parseTablebuilder.cpp
 *
 *  Created on: May 8, 2013
 *      Author: AHMAD
 */

#include "parseTablebuilder.h"

parseTablebuilder::parseTablebuilder(vector<Rule *>* Rule) {
	Rules = Rule;

}

void parseTablebuilder::makeFirst(Rule* lambda) {
	Rule * holder;
	int ruleCounter = 0;
	for (int i = Rules->size() - 1; i >= 0; i--) {
		holder = Rules->at(i);

		vector<vector<Rule *> > childern = holder->children;
		for (int j = 0; j < childern.size(); j++) {
			vector<Rule *> childernOfChildern = childern.at(j);
			ruleCounter = 0;
			for (int p = 0; p < childernOfChildern.size(); ++p) {
				Rule * chHolder = childernOfChildern.at(p);
				if (chHolder->isTerminal) {

					addToFirst(&holder->first, chHolder);
					break;
				} else if (!chHolder->isTerminal) {
					if (chHolder == lambda) {
						addToFirst(&holder->first, lambda);
					} else {
						vector<Rule *> oldFirst = chHolder->first.first;
						for (int k = 0; k < oldFirst.size(); ++k) {
							addToFirst(&holder->first, oldFirst.at(k));
						}
						if (!chHolder->hasEpsilon) {
							break;
							ruleCounter++;
						}
					}
				}
			}
			if (ruleCounter == childernOfChildern.size()
					&& childernOfChildern.size() != 0
					&& holder->hasEpsilon == false) {
				holder->first.first.push_back(lambda);
				holder->hasEpsilon = true;
			}

		}
	}
}
void parseTablebuilder::addToFirst(FirstSet* f, Rule* r) {
	bool found = false;
	vector<Rule*> start = f->first;
	for (int i = 0; i < start.size(); ++i) {
		if (r == start.at(i)) {
			found = true;
			break;
		}
	}
	if (!found)
		f->first.push_back(r);

}

void parseTablebuilder::makeFollow(Rule * lambda, Rule* d) {
	dollerSing = d;
	Rules->at(0)->follow.followTerminals.push_back(dollerSing);
	for (int i = 0; i < Rules->size(); ++i) {
		Rule *currentRule = Rules->at(i);
		findFollow(lambda, currentRule);
	}

}

void parseTablebuilder::findFollow(Rule * lambda, Rule *current) {
	for (int j = 0; j < Rules->size(); ++j) {
		Rule* checkingRule = Rules->at(j);
		vector<vector<Rule *> > childern = current->children;
		for (int i = 0; i < childern.size(); i++) {
			vector<Rule *> childernOfChildern = childern.at(i);
			for (int p = 0; p < childernOfChildern.size(); ++p) {
				Rule * chHolder = childernOfChildern.at(p);
				cout << current->name << "  --- " << chHolder->name << "  ---- "
						<< checkingRule->name << endl;
				if (current == chHolder) {
					if (p == childernOfChildern.size() - 1) {
						findFollow(lambda, checkingRule);
						addFollow(checkingRule->follow.followTerminals,
								current);
					} else {
						if (childernOfChildern.at(p + 1)->isTerminal) {
							bool found = false;
							vector<Rule*> start =
									current->follow.followTerminals;
							for (int i = 0; i < start.size(); ++i) {
								if (childernOfChildern.at(p + 1)
										== start.at(i)) {
									found = true;
									break;
								}
							}
							if (!found)
								current->follow.followTerminals.push_back(
										childernOfChildern.at(p + 1));
						} else {
							if (childernOfChildern.at(p + 1)->hasEpsilon) {
								addFollow(
										childernOfChildern.at(p + 1)->first.first,
										current);
								findFollow(lambda,
										childernOfChildern.at(p + 1));
								addFollow(
										childernOfChildern.at(p + 1)->follow.followTerminals,
										current);
							} else {
								addFollow(
										childernOfChildern.at(p + 1)->first.first,
										current);
							}
						}

					}
				}

			}
		}
	}

}

void parseTablebuilder::addFollow(vector<Rule*> r, Rule * addto) {
	for (int j = 0; j < r.size(); ++j) {

		bool found = false;
		vector<Rule*> start = addto->follow.followTerminals;
		for (int i = 0; i < start.size(); ++i) {
			if (r.at(j) == start.at(i)) {
				found = true;
				break;
			}
		}
		if (!found)
			addto->follow.followTerminals.push_back(r.at(j));
	}
}

void parseTablebuilder::printer() {
	for (int i = 0; i < Rules->size(); ++i) {
		FirstSet f = Rules->at(i)->first;
		vector<Rule *> temp = f.first;
//		cout << Rules->at(i)->name << " ";
		for (int j = 0; j < temp.size(); ++j) {
			cout << temp.at(j)->name << " ";
		}
		if (temp.size() > 0)
			cout << endl;
	}
	cout << "_________________***********________________________" << endl;
	for (int i = 0; i < Rules->size(); ++i) {
		FollowSet f = Rules->at(i)->follow;
		vector<Rule *> temp = f.followTerminals;
		for (int j = 0; j < temp.size(); ++j) {
			cout << temp.at(j)->name << " ";
		}
		if (temp.size() > 0)
			cout << endl;
	}
}
parseTablebuilder::~parseTablebuilder() {
// TODO Auto-generated destructor stub
}

