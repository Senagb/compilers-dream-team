/*
 * Simulator.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: Ahmad
 */

#include "Simulator.h"

Simulator::Simulator(NFSA* n, DFA_Engine* d) {
	nfa = n;
	dfa = d;
	current_pos = 0;
}

void Simulator::fileReader(string filename) {
	ifstream readFile((char*) filename.c_str());
	string outName = "";
	for (int i = 0; i < filename.length(); ++i) {
		if (filename[i] != '.') {
			outName += filename[i];
		} else {
			break;
		}
	}
	outName += "-output.txt";
	ofstream outputFile((char*) outName.c_str());

	if (readFile) {

		string line, tokenId;
		int startIndex = 0, endIndex = 0, lastAccpetance = -1,
				indexLastAccpeted = 0;
		DFA_State* current = dfa->getStart(); // equal to start state
		bool isNull = false;
		while (getline(readFile, line)) {
			cout << line << endl;
			startIndex = 0, endIndex = 0, lastAccpetance = -1;
			current = dfa->getStart();
			isNull = false;
			while (line.length() > 0 && startIndex <= line.length()) {
				char next = line[endIndex];
				int childIndex = nfa->index_array[(int) next];
				if (childIndex != -1) {
					cout << "Input is : " << line[endIndex];
					cout << ", Index of the child: " << childIndex;
					cout << ", Child is : "
							<< current->childrenStates.at(childIndex)->name
							<< endl;
					current = current->childrenStates.at(childIndex);
					;
				} else {
					isNull = true;
				}
				if (!(current->isNull || isNull))
					if (current->isFinal) {
						lastAccpetance = endIndex;
						tokenId = current->accepted_rule;
						if (tokenId.length() >= 2
								&& (tokenId[1] == '(' || tokenId[1] == ')'))
							tokenId = tokenId.substr(1, 1);
						indexLastAccpeted = endIndex;
					}
				if (current->isNull || isNull) {
					if (lastAccpetance != -1) {
						string temp = line.substr(startIndex,
								indexLastAccpeted - startIndex + 1);
						outputFile << "-" << temp << endl;
						outputFile << tokenId << endl;
						token accepted;
						accepted.tokenType = tokenId;
						accepted.tokenconent = temp;
						result.push_back(accepted);
						if (indexLastAccpeted == endIndex - 1
								&& (line[endIndex] == ' '
										|| line[endIndex] == '\0')) {
							endIndex++;
							startIndex = endIndex;
						} else if (indexLastAccpeted == endIndex - 1) {
							startIndex = endIndex;
						} else {
							startIndex = indexLastAccpeted + 1;
						}
						current = dfa->getStart();
						lastAccpetance = -1;
						tokenId = "";
						isNull = false;
						endIndex = startIndex;

					} else {
//						if (current->isFinal) {
//							lastAccpetance = endIndex;
//							tokenId = current->accepted_rule;
//							string temp = line.substr(startIndex,
//									endIndex - startIndex);
//							outputFile << "-" << temp << endl;
//							outputFile << tokenId << endl;
//							token accepted;
//							accepted.tokenType = tokenId;
//							accepted.tokenconent = temp;
//							result.push_back(accepted);
//							if (indexLastAccpeted == endIndex - 1
//									&& line[endIndex] == ' ') {
//								endIndex++;
//								startIndex = endIndex;
//							} else if (indexLastAccpeted == endIndex - 1) {
//								startIndex = endIndex;
//								endIndex++;
//							}
//							current = dfa->getStart();
//							lastAccpetance = -1;
//							tokenId = "";
//							isNull = false;
//						} else {
						string temp = line.substr(startIndex, endIndex + 1);
						if (temp.length() > 0)
							cout << "**** Pannic mode Error happened :" << temp
									<< endl;
						startIndex++;
						endIndex = startIndex;
						isNull = false;
						current = dfa->getStart();
					}
//					}

				} else {

					endIndex++;
				}
			}

		}
	} else {
		cout << "error the source file couldn't been accessed " << endl;
	}
}

Simulator::~Simulator() {
// TODO Auto-generated destructor stub
}


