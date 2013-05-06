/*
 * FileParser.cpp
 *
 *  Created on: 4 May 2013
 *      Author: Mostafa Mokhtar
 */

#include "CFG_Parser.h"

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

bool CFG_Parser::compare(vector<Rule*>* first, vector<Rule*>* second, int length)
{
	for(int i=0; i<=length; i++)
	{
		if(i>= second->size() || strcmp(first->at(i)->name.c_str(), second->at(i)->name.c_str()) != 0)
		{
			return false;
		}
	}
	return true;
}

void CFG_Parser::left_factoring()
{
	int size = rulesTable.size();
	Rule* pntr;
	vector< Rule* >* vct_pntr;
	vector<vector<Rule*>* >* pntrs = new vector<vector<Rule*>* >;
	int matches =0;
	int counter =0;
	stringstream ss;
	for(int i=0; i<  rulesTable.size(); i++)
	{
		pntr = rulesTable.at(i);
		counter =0;
		for(unsigned int j=0; j<pntr->children.size(); j++)
		{
			vct_pntr = &(pntr->children.at(j));
			for(int k =vct_pntr->size()-1; k>-1; k--)
			{
				matches = 0;
				pntrs->clear();
				for(int m=j; m<pntr->children.size(); m++)
				{
					if(compare(vct_pntr, &(pntr->children.at(m)), k)){
						matches++;
						pntrs->push_back(&(pntr->children.at(m)));
					}
				}
				if(matches > 1)
				{
					ss << counter;
					Rule* newRule = new Rule(pntr->name+ss.str());
					counter++;
					ss.str("");
					ss.clear();
					for(int n=0; n<pntrs->size(); n++)
					{
						if(k == pntrs->at(n)->size()-1)
						{
							vector<Rule*> to_add;
							to_add.push_back(lambda);
							newRule->children.push_back(to_add);
						}else
						{
							vector<Rule*> to_add;
							for(int p=k+1; p<pntrs->at(n)->size();p++)
							{
								to_add.push_back(pntrs->at(n)->at(p));
							}
							newRule->children.push_back(to_add);
						}
						if(n==0)
						{
							while(pntrs->at(n)->size() != k+1)
							{
								pntrs->at(n)->pop_back();
							}
							pntrs->at(n)->push_back(newRule);
						}else
							pntrs->at(n)->clear();
					}
					rulesTable.push_back(newRule);
				}
			}
		}
	}
}

