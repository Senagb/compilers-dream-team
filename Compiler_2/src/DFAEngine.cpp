/*
 * DFAEngine.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: mostafa
 */

#include "DFAEngine.h"

using namespace std;

DFA_Engine::DFA_Engine(NFSA* ourNFSA, int numOfInp) {
	// TODO Auto-generated constructor stub

	NfsaEngine = ourNFSA;
	startState = NfsaEngine->getStart();
	numOfInput = numOfInp;
	name = 0;
}

DFA_Engine::~DFA_Engine() {
	// TODO Auto-generated destructor stub
}
void DFA_Engine::getFirstDFA_State() {

	DFA_State* start = new DFA_State();
	start->compositObjects.push_back(startState);
	startState->expand(&start->compositObjects);
	start->setFinal();
	start->isNull = false;
	start->name = name;
	dfaStates = vector<DFA_State*>();
	dfaStates.push_back(start);
}
void DFA_Engine::generateTable() {
	DFA_State* null = new DFA_State();
	null->name = -1;
	null->isFinal = false;
	null->isNull = true;

	unsigned int i, size = dfaStates.size();
	// first loop for all DFA_States in the table
	for (i = 0; i < size; i++) {

		DFA_State* current = dfaStates.at(i);
		unsigned int childIndex, childrenSize = current->compositObjects.size();
		// For each DFA_State loop for all possible inputs
		for (int input = 0; input < numOfInput; input++) {
			DFA_State* child = new DFA_State();
			child->compositObjects = vector<state*>();
			// For each input loop for all the compositObjects to find all the NFA states that represents the new state
			for (childIndex = 0; childIndex < childrenSize; childIndex++) {
				current->compositObjects.at(childIndex)->getChildren(input,
						&child->compositObjects);
			}
			unsigned int existIndex = 0;
			// after determining the NFA states representing the child state , check if it exists before
			for (existIndex = 0; existIndex < size; existIndex++) {
				if (child->compareWithAnotherDFSA(dfaStates.at(existIndex))) {
					break;
				}
			}
			if (existIndex != size) {
				//exist before
				current->childrenStates.push_back(dfaStates.at(existIndex));

			} else {
				// if we need to create a new DFA_State ,check to see if it is a null state or a real state
				if (child->compositObjects.size() == 0) {
					//empty node , i.e null state
					child = null;
				} else {
					// if a real state
					child->name = ++name;
					// TODO final ?? check for final state
					child->setFinal();
					child->isNull = false;
					// add the new state to the dfaStates
					dfaStates.push_back(child);
					// increment the counter
					size++;
				}
				current->childrenStates.push_back(child);
			}

		}
	}

}

void DFA_Engine::minimize_table() {
	unsigned int i;
	int count = 2;

	vector<ClassOfStates> finals;
	ClassOfStates start;

	start.name = 1;
	/*find start class and final classes*/
	for (i = 0; i < dfaStates.size(); i++) {
		if (dfaStates.at(i)->isFinal) {
			unsigned int j;
			bool inserted = false;
			for (j = 0; j < finals.size(); j++) {
				if (finals.at(j).compStates.at(0)->accepted_rule.compare(dfaStates.at(i)->accepted_rule) == 0) {
					dfaStates.at(i)->its_next_class = finals.at(j).name;
					finals.at(j).compStates.push_back(dfaStates.at(i));
					inserted = true;
				}
			}
			if (!inserted) {
				ClassOfStates newClass;
				newClass.name = count;
				count++;
				dfaStates.at(i)->its_next_class = newClass.name;
				newClass.compStates.push_back(dfaStates.at(i));
				finals.push_back(newClass);
			}
		} else {
			dfaStates.at(i)->its_next_class = start.name;
			start.compStates.push_back(dfaStates.at(i));
		}
	}
	/*end of finding classes*/
	/*----------------debugging-----------*/
	cout << "----- Printing Classes for debugging -----" << endl;
	cout << "Start Class #" << start.name << " : {";
	for (unsigned int i = 0; i < start.compStates.size(); i++) {
		cout << start.compStates.at(i)->name << ", ";
	}
	cout << "}" << endl;

	cout << "---- Now print the final Classes ----" << endl;
	for (unsigned int i = 0; i < finals.size(); i++) {
		cout << "Class #" << finals.at(i).name << " : {";
		for (unsigned int j = 0; j < finals.at(i).compStates.size(); j++) {
			cout << finals.at(i).compStates.at(j)->name << ", ";
		}
		cout << "}" << " With accepting rule : "
				<< finals.at(i).compStates.at(0)->accepted_rule << endl;

	}
	cout << endl;
	/*----------------end debugging-----------*/

	/* now split each class*/
	DFA_State* tempoState;
	ClassOfStates current_class;
	vector<ClassOfStates>* current_queue = new vector<ClassOfStates>;
	vector<ClassOfStates>* next_queue = new vector<ClassOfStates>;
	vector<ClassOfStates>* tempPointer;
	vector<ClassOfStates>* next_sub_queue = new vector<ClassOfStates>;
	vector<ClassOfStates>* final_classes = new vector<ClassOfStates>;
	string trans_name;
	int class_name = count;
	next_queue->push_back(start);
	for (unsigned int p = 0; p < finals.size(); p++)
		next_queue->push_back(finals.at(p));

	stringstream conv;
	unsigned int j, cc;
	unsigned int k;
	bool found;
	int countSplits = 0; //this to count number of splits for each class
	while (!next_queue->empty()) {
		cout << "Next queue size is : " << next_queue->size() << endl;
		tempPointer = current_queue;
		current_queue = next_queue;
		cout << "Current queue size updated : " << current_queue->size()
				<< endl;
		next_queue = tempPointer;
		update_its_class(current_queue);
		next_queue->clear();
		cout << " (Expecting 0) Next Queue size updated : "
				<< next_queue->size() << endl;
		for (cc = 0; cc < current_queue->size(); cc++) {
			next_sub_queue->clear();
			current_class = current_queue->at(cc);
			countSplits = 0;
			for (j = 0; j < current_class.compStates.size(); j++) {
				tempoState = current_class.compStates.at(j);
				trans_name = "";
				for (k = 0; k < tempoState->childrenStates.size(); k++) {
					if (tempoState->childrenStates.at(k)->isNull != true) {
						conv.str("");
						conv.clear();
						conv
								<< tempoState->childrenStates.at(k)->its_curr_class;
						trans_name += conv.str() + ",";
					} else
						trans_name += "-,";

				}
				found = false;
				//search in the next_sub_queue;
				for (k = 0; k < next_sub_queue->size(); k++) {
					if (trans_name.compare(next_sub_queue->at(k).trans) == 0) {
						found = true;
						tempoState->its_next_class = next_sub_queue->at(k).name;
						next_sub_queue->at(k).compStates.push_back(tempoState);
						break;
					}
				}
				if (!found) {
					ClassOfStates newClass;
					newClass.name = class_name;
					newClass.trans = trans_name;
					class_name++;
					tempoState->its_next_class = class_name;
					newClass.compStates.push_back(tempoState);
					next_sub_queue->push_back(newClass);
					countSplits++;
					cout << "New Class created : " << (newClass.name)
							<< " , #Splits : " << countSplits << endl;
				}
			}
			if (countSplits == 1) {
/*				next_sub_queue->back().name = next_sub_queue->back().compStates.at(0)->its_curr_class;*/
				for(int yy=0; yy< next_sub_queue->back().compStates.size(); yy++)
					next_sub_queue->back().compStates.at(yy)->its_curr_class = next_sub_queue->back().name;
				final_classes->push_back(next_sub_queue->back());
				next_sub_queue->pop_back();

			}
			//for loop to move the elements from next_sub_queue to next_queue
			for (unsigned int s = 0; s < next_sub_queue->size(); s++) {
				next_queue->push_back(next_sub_queue->at(s));
			}
		}
	}
	unsigned int m;
	DFA_State* DFState;
	for (m = 0; m < final_classes->size(); m++) {
		DFState = final_classes->at(m).compStates.at(0);
		if (DFState->name == 0) {
			start_state_minimized = DFState;
			cout << "^^^^^^^^ I found the start state one time ^^^^^^^^ "
					<< endl;
		}
		DFState->name = m;
		minimizedTable.push_back(DFState);
	}

	DFA_State* dfs;
	unsigned int mm;
	unsigned int mmm;
	int state_class;
	for (m = 0; m < minimizedTable.size(); m++) {
		dfs = minimizedTable.at(m);
		for (mm = 0; mm < dfs->childrenStates.size(); mm++) {
			if (dfs->childrenStates.at(mm)->isNull != true) {
				state_class = dfs->childrenStates.at(mm)->its_curr_class;
				for (mmm = 0; mmm < minimizedTable.size(); mmm++) {
					if (state_class == minimizedTable.at(mmm)->its_curr_class) {
						dfs->childrenStates.at(mm) = minimizedTable.at(mmm);
						break;
					}
				}
			}
		}
	}
	cout << "---- Print the Minimized Table -----" << endl;
	DFA_State* debug_state;
	for (m = 0; m < minimizedTable.size(); m++) {
		cout << "Minimized state #" << m << ": {";
		debug_state = minimizedTable.at(m);
		for (mm = 0; mm < debug_state->childrenStates.size(); mm++) {
			if (debug_state->childrenStates.at(mm)->isNull != true) {
				cout << debug_state->childrenStates.at(mm)->name << ", ";
			} else {
				cout << "-, ";
			}
		}
		cout << "}";
		if (debug_state->isFinal) {
			cout << " With accepted rule : " << debug_state->accepted_rule;
		} else if (start_state_minimized->name == debug_state->name) {
			cout << "It is the START STATE";
		}
		cout << endl;
	}
	cout << "----- End printing Minimized Table ----" << endl;

}

vector<DFA_State*>* DFA_Engine::getMinimizedTabel() {
	return &minimizedTable;
}

DFA_State* DFA_Engine::getStart() {
	return start_state_minimized;
//	return dfaStates.at(0);
}

void DFA_Engine::update_its_class(vector<ClassOfStates>* vec) {
	for (unsigned int i = 0; i < vec->size(); i++) {
		for (unsigned int j = 0; j < vec->at(i).compStates.size(); j++) {
			vec->at(i).compStates.at(j)->its_curr_class =
					vec->at(i).compStates.at(j)->its_next_class;
		}
	}
}
void DFA_Engine::print() {

	for (unsigned int index = 0; index < dfaStates.size(); index++) {
		dfaStates.at(index)->print();
		cout << "**************************" << endl;

	}
}

