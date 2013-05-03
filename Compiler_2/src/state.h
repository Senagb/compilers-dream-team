/*
 * state.h
 *
 *  Created on: Mar 22, 2013
 *      Author: zonkoly
 */

#ifndef STATE_H_
#define STATE_H_

#include "Includes.h"


using namespace std;

/*Definitions */
class childState;
class state;
#define inner 1
#define final 2
#define dummy -5
#define source 0
#define epsion -10
static string types[] { "source", "inner", "final" };
#define fromTable false
#define character true

/****************************/

class state
	{
	public:
		int type; // indicate whether the state is final state or not.
		int name;
		string token_name;
		string token_type;
		int order;
		bool visited;
		std::vector<childState*> children;
	public:
		state()
		{

		}
		state(int state_type, int state_name) :
				type(state_type), name(state_name)
		{
			children = std::vector<childState*>();
			token_name = "";
			token_type="";
			order=-1;
		}
		void setName(string name, string type,int ord)
		{
			token_name = name;
			token_type = type;
			order=ord;
		}
		string print();
		void getChildren(int input, vector<state*>* composite);

		void insertChild(int input, state* child, bool isDefined);
		void set(int state_type, int state_name);
		int getSize();
		void printTable();
		void expand(vector<state*>* equivalent); // used to add all states reachable from the current state with epsilon input
		bool exist(state* newState, vector<state*>* current); // check if newState exists in current
		bool isDummy();

	};

class childState
	{
	public:
		int input;
		bool defined;
		state* child;
	public:
		childState()
		{

		}

		void set(int child_ip, bool isDefined, state* state_child);

		state* getState();
		bool match(int user_input, bool def);
		string print();
	};

#endif /* STATE_H_ */
