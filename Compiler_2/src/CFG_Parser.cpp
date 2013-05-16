/*
 * FileParser.cpp
 *
 *  Created on: 4 May 2013
 *      Author: Mostafa Mokhtar
 */

#include "CFG_Parser.h"

CFG_Parser::CFG_Parser(char* path)
{
	// TODO Auto-generated constructor stub
	filePath = path;
	lambda = new Rule("\\L");
	DollerSign = new Rule("$");
	sync = new Rule("->");
}

CFG_Parser::~CFG_Parser()
{
	// TODO Auto-generated destructor stub
}

void CFG_Parser::startParser()
{

	ifstream myfile(filePath);
	string parseLine = "";
	string readLine;
	cout << "Reading from " << filePath << endl << endl;

	unsigned int i = 0;
	if (myfile.is_open())
	{
		while (myfile.good())
		{

			i = 0;
			getline(myfile, readLine);
			for (i = 0; i < readLine.length(); i++)
			{
				if (readLine[i] == '#')
				{
					if (parseLine.length() != 0)
					{
						parse_Line(parseLine);
						parseLine = "";
					}
					parseLine += readLine[i];
				} else
				{
					parseLine += readLine[i];
				}
			}
		}

		if (parseLine.length())
		{
			parse_Line(parseLine);
			parseLine = "";

		}

		myfile.close();
	}

	else
		cout << "Unable to open file";

}

void CFG_Parser::parse_Line(string line)
{

	unsigned int i = 1; // to skip '#'
	Rule* newRule;
	newRule = new Rule("");
	string newName = "";

	while (line[i] != ':')
	{
		if (line[i] != ' ')
		{
			newName += line[i];
		}
		i++;
	}
	i++;
	i++;
	i++;

	int index = getIndexIntable(newName);
	if (index == -1)
	{
		newRule->name = newName;
		newName = "";
		cout << newRule->name << "=>";
		rulesTable.push_back(newRule);
	} else
	{
		newRule = rulesTable.at(index);
	}

	vector<Rule*> setOfRules;
	newRule->children.push_back(setOfRules);

	newName = "";

	for (; i < line.length(); i++)
	{

		if (line[i] == '|')
		{
			// Handle OR

			if (newName.length() != 0)
			{

				int index = getIndexIntable(newName);
				if (newName.length() == 2 && newName[0] == '\\'
				        && newName[1] == 'L')
				{
					newRule->children.at(newRule->children.size() - 1).push_back(
					        lambda);
					newRule->hasEpsilon = true;
				} else if (index == -1)
				{
					Rule* r;
					r = new Rule(newName);
					if (r->name[0] == '\'')
					{
						r->isTerminal = true;
					}
					rulesTable.push_back(r);
					newRule->children.at(newRule->children.size() - 1).push_back(
					        r);

				} else
				{
					newRule->children.at(newRule->children.size() - 1).push_back(
					        rulesTable.at(index));

				}
			}

			vector<Rule*> setOfRules;
			newRule->children.push_back(setOfRules);
			newName = "";
		} else if (line[i] != ' ')
		{
			newName += line[i];

		} else if (line[i] == ' ')
		{

			if (newName.length() != 0)
			{

				int index = getIndexIntable(newName);

				if (newName.length() == 2 && newName[0] == '\\'
				        && newName[1] == 'L')
				{
					newRule->children.at(newRule->children.size() - 1).push_back(
					        lambda);
					newRule->hasEpsilon = true;
				} else if (index == -1)
				{
					Rule* r;
					r = new Rule(newName);
					rulesTable.push_back(r);
					if (r->name[0] == '\'')
					{
						r->isTerminal = true;
					}
					newRule->children.at(newRule->children.size() - 1).push_back(
					        r);

				} else
				{
					newRule->children.at(newRule->children.size() - 1).push_back(
					        rulesTable.at(index));

				}

			}

			newName = "";
		}

	}

	if (newName.length() != 0)
	{

		int index = getIndexIntable(newName);
		if (newName.length() == 2 && newName[0] == '\\' && newName[1] == 'L')
		{
			newRule->children.at(newRule->children.size() - 1).push_back(
			        lambda);
			newRule->hasEpsilon = true;
		} else if (index == -1)
		{
			Rule* r;
			r = new Rule(newName);
			if (r->name[0] == '\'')
			{
				r->isTerminal = true;
			}
			rulesTable.push_back(r);
			newRule->children.at(newRule->children.size() - 1).push_back(r);

		} else
		{
			newRule->children.at(newRule->children.size() - 1).push_back(
			        rulesTable.at(index));

		}

	} else
	{
		cout << endl;
	}

}
int CFG_Parser::getIndexIntable(string name)
{
	unsigned int i = 0;
	bool found = false;
	for (i = 0; i < rulesTable.size(); i++)
	{
		if (rulesTable.at(i)->name.length() == name.length())
		{
			unsigned int j = 0;
			found = true;
			for (j = 0; j < name.length(); j++)
			{
				if (name[j] != rulesTable.at(i)->name[j])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				return i;
			}
		}
	}
	return -1;
}

void CFG_Parser::printOut()
{

	unsigned int i, j, k;
	string printOut = "";
	for (i = 0; i < rulesTable.size(); ++i)
	{
		Rule* r = rulesTable.at(i);
		printOut = r->name + " ::= {";

		for (j = 0; j < r->children.size(); ++j)
		{
			printOut += "{";
			vector<Rule*> setOfRules = r->children.at(j);
			for (k = 0; k < setOfRules.size(); ++k)
			{
				if (setOfRules.at(k) == lambda)
				{
					printOut = printOut + ", " + setOfRules.at(k)->name
					        + "[ it is lamda ] ";
				} else
				{
					printOut = printOut + ", " + setOfRules.at(k)->name;
				}
			}
			printOut += "},";
		}
		printOut += "}";

		if (r->isTerminal)
		{
			cout << printOut << "it is terminal" << endl;
		} else if (r == lambda)
		{
			cout << printOut << "  it contain lamda " << endl;

		} else
		{
			cout << printOut << endl;

		}
	}
}

bool CFG_Parser::compare(vector<Rule*>* first, vector<Rule*>* second,
        int length)
{
	for (int i = 0; i <= length; i++)
	{
		if (i >= second->size()
		        || strcmp(first->at(i)->name.c_str(),
		                second->at(i)->name.c_str()) != 0)
		{
			return false;
		}
	}
	return true;
}

void CFG_Parser::left_factoring()
{
	Rule* pntr;
	vector<Rule*>* vct_pntr;
	vector<vector<Rule*>*>* pntrs = new vector<vector<Rule*>*>;
	int matches = 0;
	int counter = 0;
	stringstream ss;

	for (int i = 0; i < rulesTable.size(); i++)
	{
		pntr = rulesTable.at(i);
		counter = 0;
		for (unsigned int j = 0; j < pntr->children.size(); j++)
		{
			vct_pntr = &(pntr->children.at(j));
			for (int k = vct_pntr->size() - 1; k > -1; k--)
			{
				matches = 0;
				pntrs->clear();
				for (int m = j; m < pntr->children.size(); m++)
				{
					if (compare(vct_pntr, &(pntr->children.at(m)), k))
					{
						matches++;
						pntrs->push_back(&(pntr->children.at(m)));
					}
				}
				if (matches > 1)
				{
					ss << counter;
					Rule* newRule = new Rule(pntr->name + ss.str());
					counter++;
					ss.str("");
					ss.clear();
					for (int n = 0; n < pntrs->size(); n++)
					{
						if (k == pntrs->at(n)->size() - 1)
						{
							vector<Rule*> to_add;
							to_add.push_back(lambda);
							newRule->children.push_back(to_add);
						} else
						{
							vector<Rule*> to_add;
							for (int p = k + 1; p < pntrs->at(n)->size(); p++)
							{
								to_add.push_back(pntrs->at(n)->at(p));
							}
							newRule->children.push_back(to_add);
						}
						if (n == 0)
						{
							while (pntrs->at(n)->size() != k + 1)
							{
								pntrs->at(n)->pop_back();
							}
							pntrs->at(n)->push_back(newRule);
						} else
							pntrs->at(n)->clear();
					}
					//TODO my changes (Zonkoly)
					if (i != rulesTable.size() - 1)
					{
						std::vector<Rule*>::iterator it = rulesTable.begin();
						for (int j = 0; j <= i; j++)
						{
							it++;
						}
						rulesTable.insert(it, newRule);
					} else
					{
						rulesTable.push_back(newRule);
					}
				}
			}
		}
	}
}

void CFG_Parser::left_recursion()
{
	//substitution
	//Rule tempRule("");
	int size = rulesTable.size();
	for (int i = 0; i < size && (!rulesTable.at(i)->isTerminal); i++)
	{
		Rule tempRule("");
		tempRule = *(rulesTable.at(i));
		for (int j = 0; j < i && (!rulesTable.at(j)->isTerminal); j++)
		{
			for (int k = 0; k < tempRule.children.size(); k++)
			{
				if (strcmp(rulesTable.at(j)->name.c_str(),
				        tempRule.children.at(k).at(0)->name.c_str()) == 0)
				{
					tempRule.children.at(k).erase(
					        tempRule.children.at(k).begin());
					for (int l = 0; l < rulesTable.at(j)->children.size(); l++)
					{
						vector<Rule*> to_add;
						for (int m = 0;
						        m < rulesTable.at(j)->children.at(l).size();
						        m++)
							to_add.push_back(
							        rulesTable.at(j)->children.at(l).at(m));

						for (int m = 0; m < tempRule.children.at(k).size(); m++)
							to_add.push_back(tempRule.children.at(k).at(m));

						tempRule.children.push_back(to_add);
					}
					tempRule.children.erase(tempRule.children.begin() + k);
				}
			}
		}
		Rule* newRule = new Rule(tempRule.name + "'");
		for (int j = 0; j < tempRule.children.size(); j++)
		{
			if (strcmp(tempRule.name.c_str(),
			        tempRule.children.at(j).at(0)->name.c_str()) == 0)
			{
				vector<Rule*> to_add;
				for (int k = 1; k < tempRule.children.at(j).size(); k++)
				{
					to_add.push_back(tempRule.children.at(j).at(k));
				}
				to_add.push_back(newRule);
				newRule->children.push_back(to_add);
				tempRule.children.at(j).clear();
			} else
			{
				tempRule.children.at(j).push_back(newRule);
			}
		}
		vector<Rule*> to_add;
		to_add.push_back(lambda);
		newRule->children.push_back(to_add);
		if (newRule->children.size() > 1)
		{
			//rulesTable.erase(rulesTable.begin()+i);
			//rulesTable.insert(rulesTable.begin()+i, &tempRule);
			rulesTable.at(i)->children.clear();
			for (int j = 0; j < tempRule.children.size(); j++)
				rulesTable.at(i)->children.push_back(tempRule.children.at(j));
			//TODO My changes (Zonkoly)
			if (i != rulesTable.size() - 1)
			{
				std::vector<Rule*>::iterator it = rulesTable.begin();
				for (int j = 0; j <= i; j++)
				{
					it++;
				}
				rulesTable.insert(it, newRule);
			} else
			{
				rulesTable.push_back(newRule);
			}
			//			rulesTable.push_back(newRule);
		}

	}
}

