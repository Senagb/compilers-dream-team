/*
 * InputFileParser.cpp
 *
 *  Created on: Mar 20, 2013
 *      Author: Ahmad
 */

#include "InputFileParser.h"
using namespace std;

InputFileParser::InputFileParser() {
	//initialize the keywords
	keywords[0] = "int";
	keywords[1] = "float";
	keywords[2] = "boolean";
	keywords[3] = "if";
	keywords[4] = "else";
	keywords[5] = "while";

	//initialize the punctuations
	punctuation[0] = "(";
	punctuation[1] = ")";
	punctuation[2] = "{";
	punctuation[3] = "}";
	punctuation[4] = ",";
	punctuation[5] = ";";

	//operators
	operators[0] = "+";
	operators[1] = "-";
	operators[2] = "*";
	operators[3] = "/";
	operators[4] = "=";
	operators[5] = "<=";
	operators[6] = "<";
	operators[7] = ">";
	operators[8] = ">=";
	operators[9] = "!=";
	operators[10] = "==";
	operators[0] = "==";

	PC1 = 1;
}

InputFileParser::~InputFileParser() {

}

void InputFileParser::readFile(string fileName) {
	int PC = 1;
	try {
		ifstream file((char *) fileName.c_str());
		if (file) {
			string line, temp;
			while (getline(file, line)) {
				temp = "";
				try {
					for (int i = 0; i < line.length(); ++i) {
						temp += line[i];
					}

					char *input = (char*) line.c_str();
					//keywords
					if (line[0] == '{') {
						string out;
						int counter = 0;
						while (input[counter] != '{') {
							counter++;
						}
						counter++;
						while (input[counter] != '}') {
							out += input[counter];
							counter++;
						}
						input = (char*) out.c_str();
						char * keyword = strtok(input, " ");
						while (keyword != NULL) {
							if (keyword_check(keyword)) {
								definition(keyword, keyword, KEYWORD, PC);
								PC++;
								combine(keyword, keyword, KEYWORD);
							} else
								cout << "**error not matched keyword" << keyword
										<< endl;
							keyword = strtok(NULL, " ");
						}

					}
					// punctuation
					else if (line[0] == '[') {
						string out;
						int counter = 0;
						while (input[counter] != '[') {
							counter++;
						}
						counter++;
						while (input[counter] != ']') {
							out += input[counter];
							counter++;
						}
						input = (char*) out.c_str();
						char * punc = strtok(input, " ");
						while (punc != NULL) {
							if (punc_check(punc)) {
								definition(punc, punc, PUNC, PC);
								PC++;
								combine(punc, punc, PUNC);
							} else
								cout << "**error not matched punctuation "
										<< punc << endl;
							punc = strtok(NULL, " ");
						}
					}
					//rules and definitions
					else if (line.length() > 0) {

						string first = strtok(input, " ");
						string second = strtok(NULL, " ");
						//rule
						if (first[first.length() - 1] == ':') {
							int length = first.length();
							first = first.substr(0, first.length() - 1);
							rule(first, temp.substr(length, line.length() + 1),
									RULE, PC);
							PC++;
							combine(first,
									temp.substr(length, line.length() + 1),
									RULE);
						} //definition
						else if (second.compare("=") == 0) {
							int pos = line.find("=");
							definition(first,
									temp.substr(pos + 1, temp.length() + 1),
									DEFINITION, PC);
							PC++;
							combine(first,
									temp.substr(pos + 1, temp.length() + 1),
									DEFINITION);
						} else {
							cout << "*** really an error nothing matched : f :"
									<< first << " s :" << second << endl;
						}

					}
				} catch (int i) {
					cout << "** error in in parsing  " << endl;
				}

			}
		} else {

			cout << "** error in reading File " << endl;
		}
		file.close();
	} catch (int i) {
		cout << "** error in reading File " << endl;
	}
}
void InputFileParser::rule(string id, string data, string type, int order) {
	parsed out;
	out.name = id;
	out.data = data;
	out.type = type;
	out.order = order;
	rules.push_back(out);

//	cout << "++rule : name :" << id << " data :" << data << endl;
//	cout << " type :" << type << endl;

}
void InputFileParser::definition(string id, string data, string type,
		int order) {
	parsed out;
	out.name = id;
	out.data = data;
	out.type = type;
	out.order = order;
	defs.push_back(out);

//	cout << "++defs : name :" << id << " data :" << data << endl;
//	cout << " type :" << type << endl;
}

bool InputFileParser::punc_check(string key) {
	string k;

	if (key[0] == '\\') {
		for (int i = 1; i < key.length(); i++) {
			k += key[i];
		}
	} else
		k = key;

	for (int i = 0; i < puncnum; ++i) {
		if (k.compare(punctuation[i]) == 0)
			return true;
	}
	return false;
}
bool InputFileParser::keyword_check(string key) {
	string k;

	if (key[0] == '\\') {
		for (int i = 1; i < key.length(); i++) {
			k += key[i];

		}
	} else
		k = key;
	for (int i = 0; i < keyNum; i++) {
		if (k.compare(keywords[i]) == 0)
			return true;
	}
	return false;
}
vector<parsed> InputFileParser::getRuleTable() {
	return rules;
}
vector<parsed> InputFileParser::getDefinitionTable() {
	return defs;
}

vector<parsed> InputFileParser::getCombined() {
	return combined;
}
void InputFileParser::combine(string id, string data, string type) {
	if (type.compare(KEYWORD) == 0 || type.compare(PUNC) == 0) {
		PC1++;
		return;
	}
	parsed out;
	out.name = id;
	out.data = data;
	out.type = type;
	out.order = PC1;
	combined.push_back(out);
	PC1++;

}
