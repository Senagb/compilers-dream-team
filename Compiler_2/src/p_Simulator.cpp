/*
 * p_Simulator.cpp
 *
 *  Created on: May 13, 2013
 *      Author: zonkoly
 */
#include "Includes.h"
#include "p_Simulator.h"
using namespace std;

p_Simulator::p_Simulator(parseTablebuilder* table, vector<string> inputString)
{
	input = vector<Rule*>();
	stack = vector<Rule*>();
	valid = true;
	error = vector<string>();
	result = vector<string>();
	old = "";
	current = "";

	parseTable = table;
	parseTable->printer();
	parseTable->dollerSing->isTerminal = true;
	c_state = CONT;

	construct(inputString); // gives each terminal\non-terminal an index , and build the input
}

void p_Simulator::construct(vector<string> inputString)
{
	// first set the index of the nonTerminals and Terminals  , to help during simulation
	int index, size = parseTable->nonTerminals.size();
	for (index = 0; index < size; index++)
	{
		parseTable->nonTerminals.at(index)->index = index;
	}

	size = parseTable->terminals.size();
	for (index = 0; index < size; index++)
	{
		parseTable->terminals.at(index)->index = index;
	}

	// now need to transform the vector<string> inputString to a vector of Rule* of only Terminals
	size = inputString.size();
	int pos = -1;
	input.push_back(parseTable->dollerSing);

	for (index = size - 1; index >= 0; index--)
	{
		pos = getIndex_terminal(inputString.at(index));
		if (pos == -1)
		{
			//TODO
			//signal ERROR ???
		} else
		{
			input.push_back(parseTable->terminals.at(pos));
		}
	}
}

int p_Simulator::getIndex_terminal(string name)
{
	int size = parseTable->terminals.size();
	int index = 0;

	for (index = 0; index < size; index++)
	{
		if (name.compare(parseTable->terminals.at(index)->name) == 0)
		{
			return index;
		}
	}

	return -1;
}

void p_Simulator::signal_error(int ERROR_TYPE)
{
	string error_message = "";
	switch (ERROR_TYPE) {
		case TERMINAL_ERROR:
			error_message =
			        "ERROR : The Top of the stack is a Terminal '"
			                + stack.back()->name
			                + "', which doesn't match with the current input '"
			                + input.back()->name
			                + "' , input symbol inserted and the top of the stack removed\n";
			break;
		case NONTERMINAL_ERROR:
			error_message =
			        "ERROR : The Top of the stack is a NON-Terminal '"
			                + stack.back()->name + "', the current input  '"
			                + input.back()->name
			                + "' is not in the First of this non-terminal, searching for a synch. token, neglect input symbol\n";
			break;
		case UNKOWN_ERROR:
			break;
		case INPUT:
			error_message =
			        "ERROR : INPUT Stack still contains Symbols , while the stack is empty\n";
			for (int i = input.size() - 1; i >= 0; i--)
			{
				error_message += "," + input.at(i)->name;
			}
			error_message += "\n";
			break;
		case STACK:
			error_message =
			        "ERROR :  Stack still contains Symbols , while the input is empty\n";
			for (int i = stack.size() - 1; i >= 0; i--)
			{
				error_message += "," + stack.at(i)->name;
			}
			error_message += "\n";
			break;
		default:

			break;
	}
	valid = false;
	result.push_back(error_message);
	error.push_back(error_message);

	cout << error_message << endl;
}
void p_Simulator::log(string name)
{
	if (name.length() != 0)
	{
		old = old + "," + name;
	}

	current = "&&";
	for (int i = stack.size() - 1; i >= 0; i--)
	{
		current += "," + stack.at(i)->name;
		if (stack.at(i) == parseTable->sync)
			current += "%";
	}
	result.push_back(old + current + "\n");
}
bool compare(string first, string second)
{
	bool n_f = false, n_s = false;
	for (int i = 0; i < first.length(); i++)
	{
		if (first[i] == '\n' || (int) first[i] == 13)
			n_f = true;
	}
	for (int i = 0; i < second.length(); i++)
	{
		if (second[i] == '\n' || (int) second[i] == 13)
			n_s = true;
	}

	if (n_f == n_s)
		return first.compare(second) == 0;
	if (n_f)
	{
		second = second + first[first.length() - 1];
	}
	if (n_s)
		first = first + second[second.length() - 1];

	return first.compare(second) == 0;
}
int p_Simulator::expand()
{

	//TODO
	if (stack.back()->isTerminal)
	{
		//TODO
		if (compare(stack.back()->name, input.back()->name))
			return MATCH;
		signal_error(TERMINAL_ERROR);
		stack.pop_back();
		return UNKOWN_ERROR;
	} else
	{
		if (stack.back() == parseTable->sync)
		{
			//TODO
			return SYNC_STATE;
		}
		if (stack.back() == parseTable->lambda)
		{
			//TODO
			return UNKOWN_ERROR;

		}
		if (stack.back() == parseTable->dollerSing)
		{
			//TODO
			return UNKOWN_ERROR;
		}
		int i = parseTable->getRow(stack.back());
		int j = input.back()->index;
//		cout << stack.back()->name << "&&" << input.back()->name << endl;
//		cout << parseTable->nonTerminals.at(i)->name << "&&"
//		        << parseTable->terminals.at(j)->name << endl;

		if (parseTable->Table[i][j].size() == 0)
		{
			return NONTERMINAL_ERROR;
		}
		if (parseTable->Table[i][j].size() == 1)
		{
			if (parseTable->Table[i][j].at(0) == parseTable->sync)
				return SYNC_STATE;
		}

		if (c_state == SYNC_STATE)
		{
			return NONTERMINAL_ERROR;
		}
		return EXPAND;
		//TODO
		// what if SYNCH. State , how to know it ??
	}

	return UNKOWN_ERROR;
}
void p_Simulator::simulate()
{
	//TODO
	stack.push_back(parseTable->dollerSing);
	stack.push_back(parseTable->nonTerminals.at(0));

	string _name;
	log("");
	int x, y;
	vector<Rule*> cell;
	while (stack.size() != 0 && input.size() != 0)
	{
		periodic_print();
		int _case = expand();
		switch (_case) {
			case MATCH:
				cout << "MATCH" << stack.back()->name << "&&"
				        << input.back()->name << endl;
				_name = stack.back()->name;
				stack.pop_back();
				log(_name);
				input.pop_back();
				break;
			case EXPAND:
				// int x,y --> from table get the expansion
				x = parseTable->getRow(stack.back());
				y = input.back()->index;
				cell = parseTable->Table[x][y];
				stack.pop_back();
				for (int i = cell.size() - 1; i >= 0; i--)
				{
					if (cell.at(i)->name == "" || cell.at(i)->name == "\n"
					        || (int) cell.at(i)->name[0] == 13)
					{
						// EXTRA
					} else
					{
						cout << "EXPAND Adding " << cell.at(i)->name << endl;
						stack.push_back(cell.at(i));
					}
				}
				log("");
				break;
			case LAMDA:
				_name = stack.back()->name;
				stack.pop_back();
				log(_name);
				break;
			case NONTERMINAL_ERROR:

				signal_error(NONTERMINAL_ERROR);
				c_state = SYNC_STATE;
				input.pop_back();
				break;
			case SYNC_STATE:
				c_state = CONT
				;
				result.push_back(
				        "SYNC terminal found ,  the non terminal will be removed"
				                + stack.back()->name + "&&" + input.back()->name
				                + "\n");
				cout << result.back();
				stack.pop_back();

				//TODO
				break;

			default:
				break;
		}
	}
	if (stack.size() != 0)
	{
		signal_error(STACK);
	}
	if (input.size() != 0)
	{
		signal_error(INPUT);
	}
}
string get(string first)
{
	string result = "";
	for (int i = 0; i < first.size(); i++)
	{
		if (first[i] != '\n' && (int) first[i] != 13)
		{
			result += first[i];
		}
	}
	return result;
}
void p_Simulator::periodic_print()
{
	cout << "**********************PERIODIC PRINT*******************" << endl;

	cout << "Input Stack" << endl;
	for (int i = input.size() - 1; i >= 0; i--)
		cout << get(input.at(i)->name) << ",";
	cout << endl;

	cout << "Symbol Stack" << endl;
	for (int i = stack.size() - 1; i >= 0; i--)
		cout << get(stack.at(i)->name) << ",";
	cout << endl;

	cout << "**********************END OF PERIODIC PRINT*******************"
	        << endl;

}
void p_Simulator::print()
{
	if (valid)
	{
		cout << "*********************VALID*********************" << endl;
		for (int i = 0; i < result.size(); i++)
		{
			cout << result.at(i) << endl;
		}
		cout << "*********************VALID*********************" << endl;
	} else
	{
		cout << "********************ERROR***********************" << endl;
		for (int i = 0; i < error.size(); i++)
			cout << error.at(i) << endl;
		cout << "********************ERROR***********************" << endl;

	}
}

