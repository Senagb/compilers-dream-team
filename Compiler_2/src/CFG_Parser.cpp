/*
 * FileParser.cpp
 *
 *  Created on: 4 May 2013
 *      Author: Mostafa Mokhtar
 */

#include "CFG_Parser.h"
#include <iostream>
#include <fstream>
#include <vector>
CFG_Parser::CFG_Parser(char* path) {
	// TODO Auto-generated constructor stub
	filePath = path;
	lambda = new Rule("\\L");
}

CFG_Parser::~CFG_Parser() {
	// TODO Auto-generated destructor stub
}

void CFG_Parser::startParser() {

	ifstream myfile(filePath);
	string parseLine = "";
	string readLine;
	cout << "Reading from " << filePath << endl << endl;

	unsigned int i = 0;
	if (myfile.is_open()) {
		while (myfile.good()) {

			i = 0;
			getline(myfile, readLine);
			for (i = 0; i < readLine.length(); i++) {
				if (readLine[i] == '#') {
					if (parseLine.length() != 0) {
						parse_Line(parseLine);
						parseLine = "";
					}
					parseLine += readLine[i];
				} else {
					parseLine += readLine[i];
				}
			}
		}

		if (parseLine.length()) {
			parse_Line(parseLine);
			parseLine = "";

		}

		myfile.close();
	}

	else
		cout << "Unable to open file";

}

void CFG_Parser::parse_Line(string line) {

	unsigned int i = 1; 	// to skip '#'
	Rule* newRule;
	newRule = new Rule("");
	string newName = "";

	while (line[i] != ':') {
		if (line[i] != ' ') {
			newName += line[i];
		}
		i++;
	}
	i++;
	i++;
	i++;

	int index = getIndexIntable(newName);
	if (index == -1) {
		newRule->name = newName;
		newName = "";
		cout << newRule->name << "=>";
		rulesTable.push_back(newRule);
	} else {
		newRule = rulesTable.at(index);
	}

	vector<Rule*> setOfRules;
	newRule->children.push_back(setOfRules);

	newName = "";

	for (; i < line.length(); i++) {

		if (line[i] == '|') {
			// Handle OR

			if (newName.length() != 0) {

				int index = getIndexIntable(newName);
				if(newName.length()==2&&newName[0]=='\\'&&newName[1]=='L'){
					newRule->children.at(newRule->children.size() - 1).push_back(
												lambda);
				}else if (index == -1) {
					Rule* r;
					r = new Rule(newName);
					if (r->name[0] == '\'') {
						r->isTerminal = true;
					}
					rulesTable.push_back(r);
					newRule->children.at(newRule->children.size() - 1).push_back(
							r);

				} else {
					newRule->children.at(newRule->children.size() - 1).push_back(
							rulesTable.at(index));

				}
}

			vector<Rule*> setOfRules;
			newRule->children.push_back(setOfRules);
			newName = "";
		} else if (line[i] != ' ') {
			newName += line[i];

		} else if (line[i] == ' ') {

			if (newName.length() != 0) {

				int index = getIndexIntable(newName);

				if(newName.length()==2&&newName[0]=='\\'&&newName[1]=='L'){
									newRule->children.at(newRule->children.size() - 1).push_back(
																lambda);
								}else if (index == -1) {
					Rule* r;
					r = new Rule(newName);
					rulesTable.push_back(r);
					if (r->name[0] == '\'') {
						r->isTerminal = true;
					}
					newRule->children.at(newRule->children.size() - 1).push_back(
							r);

				} else {
					newRule->children.at(newRule->children.size() - 1).push_back(
							rulesTable.at(index));

				}


			}


			newName = "";
		}

	}

	if (newName.length() != 0) {

		int index = getIndexIntable(newName);
		if(newName.length()==2&&newName[0]=='\\'&&newName[1]=='L'){
							newRule->children.at(newRule->children.size() - 1).push_back(
														lambda);
						}else if (index == -1) {
			Rule* r;
			r = new Rule(newName);
			if (r->name[0] == '\'') {
				r->isTerminal = true;
			}
			rulesTable.push_back(r);
			newRule->children.at(newRule->children.size() - 1).push_back(r);

		} else {
			newRule->children.at(newRule->children.size() - 1).push_back(
					rulesTable.at(index));

		}

	} else {
		cout << endl;
	}

}
int CFG_Parser::getIndexIntable(string name) {
	unsigned int i = 0;
	bool found = false;
	for (i = 0; i < rulesTable.size(); i++) {
		if (rulesTable.at(i)->name.length() == name.length()) {
			unsigned int j = 0;
			found = true;
			for (j = 0; j < name.length(); j++) {
				if (name[j] != rulesTable.at(i)->name[j]) {
					found = false;
					break;
				}
			}
			if (found) {
				return i;
			}
		}
	}
	return -1;
}

void CFG_Parser::printOut(){

	unsigned int i,j , k;
	string printOut = "";
		for (i = 0; i < rulesTable.size(); ++i) {
			Rule* r = rulesTable.at(i);
			printOut = r->name + " ::= {";

			for (j = 0; j < r->children.size(); ++j) {
				printOut += "{" ;
				vector<Rule*> setOfRules = r->children.at(j);
				for (k = 0; k < setOfRules.size(); ++k) {
					if(setOfRules.at(k)==lambda){
					printOut =printOut +", "+ setOfRules.at(k)->name +"[ it is lamda ] ";
				}else{
					printOut =printOut +", "+ setOfRules.at(k)->name ;
					}
				}
				printOut += "}," ;
			}
			printOut += "}" ;

			if(r->isTerminal){
			cout<<printOut<< "it is terminal" <<endl;
			}else if(r==lambda){
				cout<<printOut<< "  it contain lamda "<<endl;

			}else{
				cout<<printOut<<endl;

			}
		}

}

