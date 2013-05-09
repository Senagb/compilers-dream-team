/*
 * parseTablebuilder.cpp
 *
 *  Created on: May 8, 2013
 *      Author: AHMAD
 */

#include "parseTablebuilder.h"

parseTablebuilder::parseTablebuilder(vector<Rule *>* R, Rule * doller, Rule * L,
		Rule* s) {
	Rules = R;
	dollerSing = doller;
	lambda = L;
	sync = s;
	for (int i = 0; i < Rules->size(); ++i) {
		if (Rules->at(i)->isTerminal) {
			terminals.push_back(Rules->at(i));
		} else {
			nonTerminals.push_back(Rules->at(i));
		}
	}
	terminals.push_back(dollerSing);

	Table = new vector<Rule*>*[nonTerminals.size()];
	for (int i = 0; i < nonTerminals.size(); i++)
		Table[i] = new vector<Rule*> [terminals.size()];
}

void parseTablebuilder::makeFirst() {
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
					addToFirst(&holder->first, chHolder, j);
					break;
				} else if (!chHolder->isTerminal) {
					if (chHolder == lambda) {
						addToFirst(&holder->first, chHolder, j);
					} else {
						vector<FsetHolder> oldFirst = chHolder->first.first;
						for (int k = 0; k < oldFirst.size(); ++k) {
							addToFirst(&holder->first, oldFirst.at(k).rule, j);
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
void parseTablebuilder::addToFirst(FirstSet* f, Rule* r, int j) {
	bool found = false;
	vector<FsetHolder> start = f->first;
	for (int i = 0; i < start.size(); ++i) {
		if (r == start.at(i).rule) {
			found = true;
			break;
		}
	}
	if (!found) {
		FsetHolder newone;
		newone.rule = r;
		newone.index = j;
		f->first.push_back(newone);
	}
}

void parseTablebuilder::makeFollow() {

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

void parseTablebuilder::makeParseTable() {

	for (int i = 0; i < Rules->size(); ++i) {
		if (!Rules->at(i)->isTerminal) {
			vector<FsetHolder> firstSet = Rules->at(i)->first.first;
			int row = getRow(Rules->at(i));
			for (int j = 0; j < firstSet.size(); ++j) {
				FsetHolder set = firstSet.at(j);
				if (set.rule != lambda) {
					int column = getColumn(set.rule);
					cout << row << " " << column << " " << i << " " << set.index
							<< endl;
					Table[row][column] = Rules->at(i)->children.at(set.index);
				}
			}
			vector<Rule *> followSet = Rules->at(i)->follow.followTerminals;
			for (int j = 0; j < followSet.size(); ++j) {
				int column = getColumn(followSet.at(j));
				vector<Rule *> temp;
				if (Rules->at(i)->hasEpsilon) {
					temp.push_back(lambda);
				} else {
					temp.push_back(sync);
				}
				Table[row][column] = temp;
			}
		}
	}

}
int parseTablebuilder::getRow(Rule* r) {
	for (int i = 0; i < nonTerminals.size(); ++i) {
		if (r == nonTerminals.at(i))
			return i;
	}
	return -1;
}
int parseTablebuilder::getColumn(Rule * r) {
	for (int i = 0; i < terminals.size(); i++) {
		if (r == terminals.at(i))
			return i;
	}
	return -1;
}
void parseTablebuilder::printer() {
	for (int i = 0; i < Rules->size(); ++i) {
		FirstSet f = Rules->at(i)->first;
		vector<FsetHolder> temp = f.first;
		cout << Rules->at(i)->name << " ";
		for (int j = 0; j < temp.size(); ++j) {
			cout << temp.at(j).rule->name << " ";
		}
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
		cout << endl;
	}

	cout
			<< "-----------------------------Table---------------------------------------"
			<< endl;
	for (int i = 0; i < nonTerminals.size(); ++i) {
		for (int j = 0; j < terminals.size(); ++j) {
			vector<Rule *> cell = Table[i][j];
			for (int p = 0; p < cell.size(); ++p) {
				cout << cell.at(p)->name;
			}
			cout << " ";
		}
		cout << endl;
	}

}
parseTablebuilder::~parseTablebuilder() {
// TODO Auto-generated destructor stub
}

