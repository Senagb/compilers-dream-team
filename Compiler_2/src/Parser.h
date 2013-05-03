/*
 * Parser.h
 *
 *  Created on: Mar 20, 2013
 *      Author: zonkoly
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Includes.h"
#include "state.h"
#include "InputFileParser.h"

using namespace std;
/*Definitions */
#define max  100000000
#define space ' '
#define OR '|'
#define one_or_more '+'
#define zero_or_more '*'
#define open '('
#define close ')'
#define reserved '\\'
#define error_empty -11
#define Space_Key 0
#define stop_key 8
#define OR_Key 1
#define Plus_Key 2
#define Zero_Key 3
#define Open_Key 4
#define Close_key 5
#define reserved_key 6
#define preDefined 7
#define newLine 9
#define neglect 10
#define cont -1
#define lamda (char)(-10)

/****************************/
struct parser
	{
		int index_array[256];
		state* bracket;
		string input;
		int name;
		std::vector<parsed> predefined;
		std::vector<string> errors;
		bool valid;
		std::vector<state*> starts;
		std::vector<state*> ends;
		std::vector<state*> nodes;
		std::vector<string>* definitions;
		std::vector<string> pattern;
		std::vector<string> pattern_name;
		state* cur_start;
		state* cur_end;
		int open_tags, close_tags;
		bool OR_statement;
		int num;
		int def_index;
		string t_name;
		vector<state*>* states;
		parser(string in, std::vector<parsed> pre, vector<state*>* stat,
		        vector<string>*def) :
				input(in), predefined(pre), states(stat), definitions(def)
		{
			pattern = vector<string>();
			pattern_name = vector<string>();
			def_index = 0;
			num = 0;
			bracket = new state();
			bracket->set(dummy, dummy);
			OR_statement = false;
			open_tags = 0;
			close_tags = 0;
			starts = vector<state*>();
			ends = vector<state*>();
			nodes = vector<state*>();
			cur_start = new state();
			cur_end = new state();
			starts.push_back(cur_start);
			ends.push_back(cur_end);
			nodes.push_back(cur_start);
			name = 0;
			cout << input << endl;
			errors = vector<string>();
			valid = false;

			for (int i = 0; i < 256; i++)
				index_array[i] = -1;


			prepare();
		}
		void reset(int number, string in)
		{
			input = in;
			num = number;
			bracket = new state();
			bracket->set(dummy, dummy);
			OR_statement = false;
			open_tags = 0;
			close_tags = 0;
			starts = vector<state*>();
			ends = vector<state*>();
			nodes = vector<state*>();
			cur_start = new state();
			cur_end = new state();
			starts.push_back(cur_start);
			errors = vector<string>();
			ends.push_back(cur_end);
			nodes.push_back(cur_start);
			cout << input << endl;
			valid = false;
			name++;

		}
		state* parse(string in, string token_name,string t_type);
		bool exist(string sub, int * index);
		int stop(char current);
		bool insert_token_space(string input);
		bool insert_token_link(string input, bool forward);
		bool insert_token_openTag();
		bool insert_token_closedTag(int link_type);
		void signal_error(int);
		void modify_OR();
		int get_linkType(string input, int index);
		bool isValid();
		void prepare();
		string pre_exist(string sub,bool RD);
		int pre_stop(char curr);
		int transform(int ASCCI);
		string pre_parse(string input,bool RD);
		string getString(string token_name,string in);
	};

#endif /* PARSER_H_ */
