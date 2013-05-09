/*
 * parseTablebuilder.cpp
 *
 *  Created on: May 8, 2013
 *      Author: AHMAD
 */

#include "parseTablebuilder.h"

parseTablebuilder::parseTablebuilder(vector<Rule *>* R, Rule * doller) {
	Rules = R;
	dollerSing = doller;
	for (int i = 0; i < Rules->size(); ++i) {
		if (Rules->at(i)->isTerminal) {
			terminals.push_back(Rules->at(i));
		} else {
			nonTerminals.push_back(Rules->at(i));
		}
	}
	terminals.push_back(dollerSing);

	Table = new vector<Rule*>*[nonTerminals.size()];
	for(int i=0; i<nonTerminals.size(); i++)
		Table[i] = new vector<Rule*>[terminals.size()];
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
					FsetHolder set;
					set.rule = chHolder;
					set.index = p;
					addToFirst(&holder->first, set);
					break;
				} else if (!chHolder->isTerminal) {
					if (chHolder == lambda) {
						FsetHolder set;
						set.rule = chHolder;
						set.index = p;
						addToFirst(&holder->first, set);
					} else {
						vector<FsetHolder> oldFirst = chHolder->first.first;
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
				FsetHolder set;
				set.rule = lambda;
				set.index = -1;
				holder->first.first.push_back(set);
				holder->hasEpsilon = true;
			}

		}
	}
}
void parseTablebuilder::addToFirst(FirstSet* f, FsetHolder r) {
	bool found = false;
	vector<FsetHolder> start = f->first;
	for (int i = 0; i < start.size(); ++i) {
		if (r.rule == start.at(i).rule) {
			found = true;
			break;
		}
	}
	if (!found)
		f->first.push_back(r);

}

void parseTablebuilder::makeFollow(Rule * lambda) {

	Rules->at(0)->follow.followTerminals.push_back(dollerSing);
	for (int i = 0; i < Rules->size(); ++i) {
		Rule *currentRule = Rules->at(i);
		if (!currentRule->isTerminal)
			findFollow(lambda, currentRule, currentRule);
	}

}

void parseTablebuilder::findFollow(Rule * lambda, Rule *current, Rule* last) {

	for (int j = 0; j < Rules->size(); j++) {
		Rule* checkingRule = Rules->at(j);
		vector<vector<Rule *> > childern = checkingRule->children;
		for (int i = 0; i < childern.size(); i++) {
			vector<Rule *> childernOfChildern = childern.at(i);
			for (int p = 0; p < childernOfChildern.size(); p++) {
				Rule * chHolder = childernOfChildern.at(p);
				if (current == chHolder) {
					//it is the last in the group find follow of the rule
					if (p == childernOfChildern.size() - 1) {
						if (checkingRule == last)
							return;
						findFollow(lambda, checkingRule, current);
						addFollowWithFollow(
								checkingRule->follow.followTerminals, current,
								lambda);
					} else {
						// if the next is terminal then add it
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
							p++;
						} else {
							//if nonterminal and has epsilon move
							if (childernOfChildern.at(p + 1)->hasEpsilon) {
								addFollow(
										childernOfChildern.at(p + 1)->first.first,
										current, lambda);
								if (checkingRule == last)
									return;
								findFollow(lambda, childernOfChildern.at(p + 1),
										current);
								addFollowWithFollow(
										childernOfChildern.at(p + 1)->follow.followTerminals,
										current, lambda);
								p++;
							} else {
								//if it hasn't got epsilon move
								addFollow(
										childernOfChildern.at(p + 1)->first.first,
										current, lambda);
								p++;
							}
						}

					}
				}

			}
		}
	}

}

void parseTablebuilder::addFollow(vector<FsetHolder> r, Rule * addto,
		Rule* lambda) {

	for (int j = 0; j < r.size(); ++j) {

		bool found = false;
		vector<Rule*> start = addto->follow.followTerminals;
		for (int i = 0; i < start.size(); ++i) {
			if (r.at(j).rule == start.at(i)) {
				found = true;
				break;
			}
		}
		if (!found)
			if (r.at(j).rule != lambda) {
				addto->follow.followTerminals.push_back(r.at(j).rule);
			}
	}

}
void parseTablebuilder::addFollowWithFollow(vector<Rule *> r, Rule * addto,
		Rule* lambda) {
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
			if (r.at(j) != lambda) {
				addto->follow.followTerminals.push_back(r.at(j));
			}
	}
}
void parseTablebuilder::printer() {
	for (int i = 0; i < Rules->size(); ++i) {
		FirstSet f = Rules->at(i)->first;
		vector<FsetHolder> temp = f.first;
		cout << Rules->at(i)->name << " ";
		for (int j = 0; j < temp.size(); ++j) {
			cout << temp.at(j).rule->name << " ";
		}
//		if (temp.size() > 0)
		cout << endl;
	}
	cout << "_________________***********________________________" << endl;

	for (int i = 0; i < Rules->size(); ++i) {
		FollowSet f = Rules->at(i)->follow;
		vector<Rule *> temp = f.followTerminals;
		cout << Rules->at(i)->name << " ";
		for (int j = 0; j < temp.size(); ++j) {
			cout << temp.at(j)->name << " ";
		}
//		if (temp.size() > 0)
		cout << endl;
	}
}
parseTablebuilder::~parseTablebuilder() {
// TODO Auto-generated destructor stub
}

