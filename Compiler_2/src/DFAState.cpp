//============================================================================
// Name        : DFAState.cpp
// Author      : Mostafa Mokhtar
// Version     :
// Copyright   : Any modification should be revised accuratly by the author
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "DFAState.h"

using namespace std;

DFA_State::DFA_State() {

	// TODO Auto-generated constructor stub
	name = -1;
	isNull = false;
}

DFA_State::~DFA_State() {
	// TODO Auto-generated destructor stub
}

DFA_State* DFA_State::getChild(int input) {

}

bool DFA_State::compareWithAnotherDFSA(DFA_State* dfaSt1) {
	int i, j;
	int list_1_Size, list_2_Size;
	list_1_Size = compositObjects.size();
	list_2_Size = dfaSt1->compositObjects.size();
	if (list_1_Size != list_2_Size) {
		return false;
	}
	bool oneElementNotFound = false;
	for (i = 0; i < list_1_Size; i++) {
		oneElementNotFound = false;
		for (j = 0; j < list_2_Size; j++) {
			if (compositObjects.at(i)->name
					== dfaSt1->compositObjects.at(j)->name) {
				oneElementNotFound = true;
				break;
			}
		}
		if (oneElementNotFound == false) {
			return false;
		}
	}
	return true;
}

void DFA_State::addCompositeState(state* compState) {
	compositObjects.push_back(compState);
}

/*
 *
 * This method is to check if the current state
 * inside a next state of a certain input is put
 * before or not.
 * then if it is not put before, it will add it and then return true
 * if add or not
 *
 * */
bool DFA_State::checkForExistenceAtCertainInput(state* state, int l,
		int numOfInp) {
	bool exist = false;
	unsigned int ii;
	cout << "=====================================" << endl;

	if (childrenStates.size() == 0) {
		//////////////// intialize number of inputs columnssssssss
		unsigned int i;
		for (i = 0; i < numOfInp; i++) {
			DFA_State* dfa;
			childrenStates.push_back(dfa);
		}
		exist = false;
//		cout << "====================================111111111111=" << ii
//				<< endl;
	} else {
		int index = state->children.at(l)->input;
		DFA_State* temp = childrenStates.at(index);
		int mm = temp->compositObjects.size();
//		cout
//				<< "===================================== CHILDREN STATE NUMBER CONFLICT "
//				<< ii << endl;
		for (ii = 0; ii < mm && !exist; ii++) {
			if (childrenStates.at(state->children.at(l)->input)->compositObjects.at(
					ii)->name == (state->children.at(l)->getState()->name)) {
				exist = true;
			}
		}
	}
	if (!exist) {
		/////////////////////////////////////////////////////////////////
		childrenStates.at(state->children.at(l)->input)->compositObjects.push_back(
				state->children.at(l)->getState());
		cout << "composite state : " << state->children.at(l)->getState()->name
				<< " is inserted to col No : " << state->children.at(l)->input
				<< endl;
		if (state->children.at(l)->getState()->type == 2/*means final*/) {
			if (isFinal == true) {
				if (state->children.at(l)->getState()->token_name.compare(
						"keyword") == 0) {
					accepted_rule =
							state->children.at(l)->getState()->token_name;
				}
			} else {
				isFinal = true;
				accepted_rule = state->children.at(l)->getState()->token_name;
			}
		}
	}

	if (!exist) {
		cout << "fLAG CAME HERE" << endl;

		checkForEmptyClosuresChildren(state->children.at(l)->child,
				state->children.at(l)->input);
	}

	// check for existence
	return exist;
}

void DFA_State::checkForEmptyClosuresChildren(state* stat, int columnNum) {
	stack<state*> stack;
	unsigned int i;
	for (i = 0; i < stat->children.size(); ++i) {
		if (stat->children.at(i)->input == -10) {
			stack.push(stat->children.at(i)->child);
		}
	}
	vector<state*>* composObjs;
	if (columnNum == -1) {
		composObjs = &compositObjects;
	} else {
		composObjs = &(childrenStates.at(columnNum)->compositObjects);
	}

	while (!stack.empty()) {
		state* currState = stack.top();
		stack.pop();
		bool ChildOfEmptyClosExist = checkExistAtInputOFEMPTYCLOSURE(currState,
				columnNum); /// THIS LINE HAS BIG CONFLICT

		if (!ChildOfEmptyClosExist) {
			composObjs->push_back(currState);
			cout << "child of state : " << stat->name
					<< " with epsilon Input of state : " << currState->name
					<< " is inserted" << endl;
		}
		for (i = 0; i < currState->children.size(); ++i) {
			if (currState->children.at(i)->input == -10
					&& !checkExistAtInputOFEMPTYCLOSURE(
							currState->children.at(i)->child, columnNum)) {
				stack.push(currState->children.at(i)->child);
			}
		}

	}

}

bool DFA_State::checkExistAtInputOFEMPTYCLOSURE(state* state, int columnNum) {
	unsigned int i;
	if (columnNum != -1) {
		for (i = 0; i < childrenStates.at(columnNum)->compositObjects.size();
				i++) {
			if (childrenStates.at(columnNum)->compositObjects.at(i)->name
					== state->name) {
				return true;
			}
		}
	} else if (columnNum == -1) {
		for (i = 0; i < compositObjects.size(); i++) {
			if (compositObjects.at(i)->name == state->name) {
				return true;
			}
		}
	}
	return false;
}

void DFA_State::print() {
	if (isNull) {
		cout << "NULL";
		return;
	}
	cout << "DFA STATE #" << name;
	if (isFinal) {
		cout << " , Final represents " << accepted_rule;
	}

	cout << " Represents : (" << compositObjects.at(0)->name;
//	<< " of name "
//	        << compositObjects.at(0)->token_name << " of type "
//	        << compositObjects.at(0)->token_type << " of #"
//	        << compositObjects.at(0)->order;

	for (unsigned int i = 1; i < compositObjects.size(); i++) {
		cout << " ," << compositObjects.at(i)->name;
//				<< " of name "
//		        << compositObjects.at(i)->token_name << " of type "
//		        << compositObjects.at(i)->token_type << " of #"
//		        << compositObjects.at(i)->order;
	}
	cout << ")";
	cout << " Type ";
	if (isFinal)
		cout << "final";
	else
		cout << "inner";
	cout << ",with children" << "(";

	if (childrenStates.at(0)->isNull)
		childrenStates.at(0)->print();
	else
		cout << childrenStates.at(0)->name;

	for (unsigned int i = 1; i < childrenStates.size(); i++) {
		cout << " ,";
		if (childrenStates.at(i)->isNull)
			childrenStates.at(i)->print();
		else
			cout << childrenStates.at(i)->name;
	}
	cout << ")" << endl;

}
int getPriority(string type) {
	if (type.compare(KEYWORD) == 0)
		return 4;
	if (type.compare(PUNC) == 0)
		return 3;
	if (type.compare(RULE) == 0)
		return 2;
	if (type.compare(DEFINITION) == 0)
		return 1;
	return -10;

}

state* compare(state* first, state* second) {
	if (first->order == epsion)
		return second;
	if (second->order == epsion)
		return first;
	int pr1 = getPriority(first->token_type);
	int pr2 = getPriority(second->token_type);
	if (pr1 > pr2)
		return first;
	if (pr2 > pr1)
		return second;

	if (first->order > second->order)
		return second;
	return first;

}

void DFA_State::setFinal() {
	unsigned int index = 0, size = compositObjects.size();
	state* dominant = new state();
	dominant->order = epsion;
	isFinal = false;
	for (; index < size; index++) {

		if (compositObjects.at(index)->type == final) {
			isFinal = true;
			dominant = compare(dominant, compositObjects.at(index));
		}
	}
	if (isFinal) {
		accepted_rule = dominant->token_name;

	}
}
