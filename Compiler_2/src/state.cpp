/*
 * state.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: zonkoly
 */
#include "state.h"
using namespace std;

struct hon
	{
		int i;
	};

/*							State					*/

void state::insertChild(int input, state* child, bool isDefined)
{

	childState* e = new childState();
	e->set(input, isDefined, child);
	children.push_back(e);

}
void state::set(int state_type, int state_name)
{
	visited = false;
	type = state_type;
	name = state_name;
}
bool state::isDummy()
{
	return type == dummy;
}
int state::getSize()
{

	return children.size();
}

void state::getChildren(int input, vector<state*>* composite)
{
	//TODO
	/*
	 * Should i return a copy from children , iterator or just return it ?? */

	unsigned int i, size = children.size();
	for (i = 0; i < size; i++)
	{
		childState* e = children.at(i);
		if (e->input == input)
		{
			if (!exist(e->child, composite))
			{
				composite->push_back(e->child);
				e->child->expand(composite);
			}
		}
	}

}

string state::print()
{
	if (visited)
	{
		cout << "tried to visit state : " << name
		        << " but it is already visited" << endl;
		return "";
	}
	visited = true;
	cout << "***************" << endl;

	int length = children.size();
	std::vector<childState*>::iterator iter = children.begin();

	cout << "State " << name << " , of type " << token_name << " have "
	        << length << " children and its type is " << types[type]
	        << " and its children are :" << endl;

	while (iter != children.end())
	{
		childState* e = *iter;
		cout << "Parent is " << name;
		e->print();
		++iter;
	}
	cout << "***************" << endl;
	return "";
}

/******************************************************************/

/*						Child State 			*/

state* childState::getState()
{

//TODO
	return child;
}
bool childState::match(int user_input, bool def)
{
	if (def != defined)
	{
		return false;
	}

	if (!def)
	{
		return user_input == input;
	}

	//TODO
	return user_input == input;
}

string childState::print()
{

	if (defined)
	{
		cout << "with i/p " << input;
	} else
	{
		if (input >= 0)
			cout << "with i/p " << input << endl;
		else
			cout << "with i/p " << input << endl;
	}

	child->print();
	return "";
}
void childState::set(int child_ip, bool isDefined, state* state_child)
{
	input = child_ip;
	defined = isDefined;
	child = state_child;

}

void state::printTable()
{
	if (type <= 2)
		cout << "state # " << name << " of type " << types[type]
		        << "Token Family : " << token_name << "  has childs:";
	else
		cout << "state # " << name << " of type " << type << "Token Family : "
		        << token_name << " has childs:";
	std::vector<childState*>::iterator iter = children.begin();

	while (iter != children.end())
	{

		childState * e = *iter;
		cout << e->child->name << " with input " << e->input << ", ";
		++iter;
	}
	cout << endl;
	cout << "**********" << endl;
}

void state::expand(vector<state*>* equivalent)
{
	int size = children.size();
	for (int i = 0; i < size; i++)
	{
		childState* child = children.at(i);
		if (child->input == epsion)
		{
			if (!exist(child->child, equivalent))
			{
				equivalent->push_back(child->child);
				child->child->expand(equivalent);
			}
		}

	}

}
bool state::exist(state* newState, vector<state*>* current)
{
	int size = current->size();

	for (int i = 0; i < size; i++)
	{
		if (newState->name == current->at(i)->name)
			return true;
	}

	return false;
}

/******************************************************************/

