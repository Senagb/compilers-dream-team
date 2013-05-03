/*
 * InputFileParser.h
 *
 *  Created on: Mar 20, 2013
 *      Author: Ahmad
 */

#ifndef INPUTFILEPARSER_H_
#define INPUTFILEPARSER_H_

#include "Includes.h"
using namespace std;

#define    KEYWORD    "Key"
#define    PUNC       "Punc"
#define    RULE       "Rule"
#define    DEFINITION  "Def"
struct parsed {
	string name;
	string data;
	string type;
	int order;
};

class InputFileParser {
	static const int keyNum = 6, puncnum = 6, relop = 11;
	vector<parsed> defs;
	vector<parsed> rules;
	vector<parsed> combined;
	string keywords[keyNum];
	string punctuation[puncnum];
	string operators[relop];
	int PC1;
public:
	InputFileParser();
	virtual ~InputFileParser();
	vector<parsed> getRuleTable();
	vector<parsed> getDefinitionTable();
	vector<parsed> getCombined();
	void readFile(string fileName);
private:
	void rule(string id, string data, string type, int order);
	void definition(string id, string data, string type, int order);
	bool keyword_check(string key);
	bool punc_check(string key);
	void combine(string id, string data, string type);

};

#endif /* INPUTFILEPARSER_H_ */
