//============================================================================
// Name        : Parser.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "Parser.h"

using namespace std;

/*				Parser Methods			*/
state* parser::parse(string in, string token_name, string t_type)
{
	t_name = t_type;
	valid = true;
	std::vector<string> tokens;
	std::vector<int> type;
//	childs
	in = getString(token_name, in);
	in = in + " ";
	cout << "input string : "<<in << endl;
	cur_start->set(source, name);
	cur_end->set(final, ++name);
	starts.push_back(cur_start);
	ends.push_back(cur_end);

	states->push_back(cur_start);
	states->push_back(cur_end);

	bool token_before = false, close_valid = false;
	int i = 0;
	int start, type_;
	int length = in.length();

	string sub;
	bool inner_loop = true;

	for (; i < length; i++)
	{
		start = i;
		type_ = -1;
		sub = "";
		inner_loop = true;

		while (inner_loop && i < length)
		{
			char curr = in[i];
			type_ = stop(curr);

			switch (type_) {
				case cont:
					token_before = true;
					OR_statement = false;
					close_valid = true;
					sub += (curr);
//					cout<<sub<<endl;
					break;
				case reserved_key:
					valid = insert_token_space(sub);
					inner_loop = false;

					if (i + 1 >= length)
					{
//						signal_error(reserved_key);
					} else
					{
						curr = in[i + 1];
						switch (curr) {
							case 'L':
								curr = lamda;
								break;
							case one_or_more:
								curr = '+';
								break;
							case zero_or_more:
								curr = '*';
								break;
							case open:
								break;
							case close:
								break;
							case OR:
								break;
							case '=':
								break;
							default:
								curr = '\\';
								i--;
								break;
						}
						i += 1;
					}
					i--;
					sub = curr;
					valid = insert_token_space(sub);
					inner_loop = false;
					token_before = true;
					OR_statement = false;
					close_valid = true;
					break;
				case Space_Key:
					valid = insert_token_space(sub);
					inner_loop = false;
					i--;
					if (!valid)
						return nodes.front();

					break;
				case OR_Key:
					close_valid = false;
					if (OR_statement || !token_before)
					{
						signal_error(OR);
					} else
					{
						if (sub.size() != 0)
						{
							valid = insert_token_space(sub);
							inner_loop = false;
							i--;
							if (!valid)
								return nodes.front();
						}
						OR_statement = true;
						modify_OR();

					}

					break;
				case Open_Key:
					close_valid = false;
					open_tags++;
					//first check if there is previous token
					if (sub.size() > 0)
					{
						valid = insert_token_space(sub);
						inner_loop = false;
						if (!valid)
							return nodes.front();
					}

					insert_token_openTag();

					break;
				case Close_key:
					inner_loop = false;
					if (!close_valid)
					{
						signal_error(Close_key);
						valid = false;
						return nodes.front();
					}
					if (sub.size() > 0)
					{
						valid = insert_token_space(sub);
						inner_loop = false;
						if (!valid)
							return nodes.front();
					}

					int link_type;
					close_tags++;
					link_type = get_linkType(in, i);
					if (link_type > 0)
						i++;

					valid = insert_token_closedTag(link_type);
					i--;
					if (!valid)
						return nodes.front();
					break;
				case Zero_Key:
					valid = insert_token_link(sub, true);
					inner_loop = false;
					i--;
					if (!valid)
						return nodes.front();

					break;
				case Plus_Key:
					valid = insert_token_link(sub, false);
					inner_loop = false;
					i--;
					if (!valid)
						return nodes.front();
					break;
				default:
					break;
			}
			i++;
		}

	}
//	cur_end->set(final, ++name);
	cur_end->setName(token_name, t_type, num);

	if (inner_loop)
	{
		// insert the last token , usually it will be just a defined value / string
		if (sub.compare("\r") != 0)
		{
			valid = insert_token_space(sub);
			if (!valid)
				return nodes.front();
		}
	}

	state* e = nodes.back();
	e->insertChild(epsion, cur_end, false);

	int ii = 0;
	for (ii = 0; ii < states->size(); ii++)
	{
		if (e->name == (*states)[ii]->name)
			break;

	}
	if (ii == states->size())
		states->push_back(e);

	if (open_tags != close_tags)
	{
		signal_error(Open_Key);
		valid = false;
	}
	if (OR_statement)
	{
		signal_error(OR_Key);
		valid = false;
	}

	return starts.front();

}

bool parser::insert_token_openTag()
{
	state* dum = new state();
	dum->set(inner, ++name);
	states->push_back(dum);
	cur_start->insertChild(epsion, dum, false);
	nodes.push_back(dum);
	nodes.push_back(dum);
	nodes.push_back(bracket);
//	starts.push_back(cur_start);
	starts.push_back(dum);
//	ends.push_back(cur_end);
	state* end = new state();
	end->set(inner, ++name);
	states->push_back(end);
	cur_start = dum;

	cur_end = end;
	ends.push_back(cur_end);
	return true;
}

bool parser::insert_token_closedTag(int link_type)
{
	if (close_tags > open_tags)
	{
		signal_error(Close_key);
		return false;
	}

	//TODO

	state* temp;
	temp = nodes.back();
	temp->insertChild(epsion, cur_end, false);
	while (nodes.size() > 0)
	{
		temp = nodes.back();
		nodes.pop_back();
		if (temp->isDummy())
		{
			nodes.pop_back();
			break;
		}

	}
	cur_start = starts.back();
	switch (link_type) {
		case 1:
			cur_end->insertChild(epsion, cur_start, false);
			break;
		case 2:
			cur_start->insertChild(epsion, cur_end, false);
			cur_end->insertChild(epsion, cur_start, false);
			break;
		default:
			break;
	}
	cur_start = cur_end;
	nodes.push_back(cur_start);
	ends.pop_back();
	cur_end = ends.back();
	starts.pop_back();
	return true;

}
void parser::modify_OR()
{
	state* e;
	bool isBracket = false;
	while (nodes.size() > 1)
	{
		e = nodes.back();
		nodes.pop_back();
		if (e->isDummy())
		{
			isBracket = true;
			break;
		}
	}
	e = nodes.back(); //now e is the start of the OR statement
	//TODO remove from the starts vector and ends vector too
	if (isBracket)
	{
		nodes.push_back(bracket);
	}
	cur_start->insertChild(epsion, cur_end, false);
	cur_start = e;

}

bool parser::insert_token_link(string input, bool forward)
{
	if (input.size() == 0)
	{
		signal_error(error_empty);
		return false;
	}
	int index = -1;
	bool def = exist(input, &index);
//	state* t_start = new state();
//	t_start->set(inner, ++name);
	state* e = new state();
	e->set(inner, ++name);
	// if not defined what should i do ?? , need to add another way to accept strings with
	if (def)
	{
	} else
	{
		// first check if the i/p is correct or not
		if (input.length() == 0)
		{
			return true;
		}

		if (input.length() > 1)
		{
			//TODO

//			errors.push_back(
//			        "Undefined Strings should be max 1 character , token {"
//			                + input + "} doesn't match the rules.\n");
//			return false;
			state* temp = cur_start;
			name--;
			for (int i = 0; i < input.length(); i++)
			{

				e->set(inner, ++name);
				cur_start->insertChild(transform(input[i]), e, true);
				cur_start = e;
				states->push_back(e);
				e = new state();

			}
			cur_start->insertChild(epsion, temp, false);
			if (forward)
			{
				temp->insertChild(epsion, cur_start, false);
			}

		} else
		{
			states->push_back(e);

			cur_start->insertChild(transform(input[0]), e, true);
			e->insertChild(epsion, cur_start, false);
			if (forward)
			{
				cur_start->insertChild(epsion, e, false);
			}
			cur_start = e;

		}
	}
	nodes.push_back(cur_start);
	return true;
}
int parser::transform(int ASCII)
{
	if (ASCII < 0)
		return epsion;
	if (index_array[ASCII] == -1)
	{
		string k = "";
		k += ((char) ASCII);
		definitions->push_back(k);
		index_array[ASCII] = def_index;
		def_index++;
	}

	return index_array[ASCII];
}
bool parser::insert_token_space(string input)
{
	int index = -1;
	bool def = exist(input, &index);
	state* e = new state();
	if (input.length() == 0)
	{
		return true;
	}
	e->set(inner, ++name);
	// if not defined what should i do ?? , need to add another way to accept strings with
	if (def)
	{
	} else
	{
		// first check if the i/p is correct or not
		if (input.length() == 0)
		{
			return true;
		}

		if (input.length() > 1)
		{
//			//TODO
//			errors.push_back(
//			        "Undefined Strings should be max 1 character , token {"
//			                + input + "} doesn't match the rules.\n");
//			return false;
			name--;

			for (int i = 0; i < input.length(); i++)
			{
				states->push_back(e);
				e->set(inner, ++name);
				cur_start->insertChild(transform(input[i]), e, false);
				cur_start = e;
				e = new state();
			}
			states->pop_back();
			e = cur_start;
		} else
		{
			states->push_back(e);
			cur_start->insertChild(transform(input[0]), e, def);
		}
	}

	cur_start = e;
	nodes.push_back(e);
	return true;
}
/**
 * Search for the sub string in the predefiend table
 */

bool parser::exist(string sub, int * index)
{
	std::vector<parsed>::iterator iter = predefined.begin();
	*index = -1;
	while (iter != predefined.end())
	{
		*index = (*index) + 1;

		if (sub.compare(iter->name) == 0)
		{

			if (iter->order < num)
				return false;
			else
			{
				if (t_name.compare(PUNC) != 0 && t_name.compare(KEYWORD) != 0)
					signal_error(preDefined);
				*index=-1;
				return false;
			}
		}
		++iter;
	}
	*index = -1;
	return false;
}

int parser::stop(char current)
{
	switch (current) {
		case space:
			return Space_Key;
		case OR:
			return OR_Key;
		case one_or_more:
			return Plus_Key;
		case zero_or_more:
			return Zero_Key;
		case open:
			return Open_Key;
		case close:
			return Close_key;
		case reserved:
			return reserved_key;
		default:
			break;
	}
	return cont;
}

void parser::signal_error(int signal_type)
{
	valid = false;
	string e_message;
	switch (signal_type) {
		case OR:
			e_message =
			        "Statement contains 2 OR symbols \'|\' after each other with no string|user defined token between or a | symbol with nothing after it.\n";
			break;
		case error_empty:
			e_message =
			        "Empty String before (* or +) this may happen due to space before the (* or +) or successive 2 (* or +)s.\n";
			break;
		case Close_key:
			e_message =
			        "Invalid closing tag ')' , either it follows a (| or '(' or nothing ) or misplaced ex. {(a|b))(}.\n";
			break;
		case Open_Key:
			e_message = "Open bracket with no close bracket.\n";
			break;
		case preDefined:
			e_message = "RD was used before it is defined.\n";
			break;
		default:
			e_message = "Error !!!.\n";
			break;
	}
	errors.push_back(e_message);
	valid = false;

}

int parser::get_linkType(string input, int index)
{
	int res = 0;
	if ((index + 1) < input.length())
	{
		if (input[index + 1] == '*')
		{
			return 2;
		}
		if (input[index + 1] == '+')
			return 1;
	}

	return res;

}

bool parser::isValid()
{
	if (errors.size() > 0 || !valid)
		return false;

	return true;

}

void parser::prepare()
{
	int size = predefined.size();

	for (int i = 0; i < size; i++)
	{

		parsed e = predefined[i];
		bool RD = false;
		if (e.type.compare(DEFINITION) == 0)
			RD = true;
		string data = pre_parse(e.data, RD);
		pattern_name.push_back(e.name);
		pattern.push_back(data);

	}
	size = pattern_name.size();
	for (int i = 0; i < size; i++)
	{
//		cout << pattern_name[i] << " -> " << pattern[i] << endl;
	}

}

/*
 * returns the matching pattern for this substring , if it exists , then it return its matching pattern , else it returns the word as it is or checks for - etc..
 * */
string parser::pre_exist(string sub, bool RD)
{
	string result = "";
	int p_size = pattern_name.size();
	for (int i = 0; i < p_size; i++)
	{
		if (sub.compare(pattern_name[i]) == 0)
		{
			return pattern[i];
		}
	}
	int size = sub.size();

	int i = 0;
	string sub1 = "";
	for (i = 0; i < size; i++)
	{
		if (sub[i] == '-' && RD)
		{
			break;
		}
		sub1 += (sub[i]);
	}

	if (i == size || i == size - 1)
		return sub1;

	if (i == 0)
	{
		result = "";
		for (int k = 0; k < size; k++)
		{
			result += (sub[k]);

		}
		return result;
	}
	for (int j = 0; j < (i - 1); j++)
	{
		result += (sub[j]);
	}
	int pre = sub[i - 1];
	int last = sub[i + 1];
	if (last < pre)
	{
		for (int j = i - 1; j < size; j++)
			result += (sub[j]);
		return result;
	}

	result += ('(');
	result += ((char) pre);

	for (int j = pre + 1; j <= last; j++)
	{
		result += ('|');
		result += ((char) j);
	}
	result += (')');
	for (int k = i + 2; k < size; k++)
		result += (sub[k]);

	return result;
}

int parser::pre_stop(char curr)
{
	int result = cont;
	switch (curr) {
		case OR:
			result = stop_key;
			break;
		case one_or_more:
			result = stop_key;
			break;
		case zero_or_more:
			result = stop_key;
			break;
		case open:
			result = stop_key;
			break;
		case close:
			result = stop_key;
			break;

		case space:
			result = Space_Key;
			break;
		case '/':
			result = reserved_key;
			break;
		case (char) 13:
			result = newLine;
			break;
		case (char) neglect:
			result = neglect;
			break;
		default:
			break;
	}

	return result;
}
string parser::pre_parse(string input, bool RD)
{
	string result = "";
	int curr = 0;
	int size = input.size();
	string sub = "";
	for (int i = 0; i < size && RD; i++)
	{

		if (input[i] == '-')
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (input[j] == ' ')
				{
					input[j] = neglect;
				} else
					j = -1;
			}

			for (int j = i + 1; j < size; j++)
			{
				if (input[j] == ' ')
				{
					input[j] = neglect;
				} else
					j = size;
			}
		}

	}

	int type = -1;
	for (int i = 0; i < size; i++)
	{
		char curr = input[i];
		type = pre_stop(curr);

		switch (type) {
			case stop_key:
				if (sub.size() > 0)
				{
					result += pre_exist(sub, RD);
				}
				result += (curr);
				sub = "";
				break;
			case cont:
				sub += (curr);
				break;

			case Space_Key:
				if (sub.size() > 0)
				{
					result += pre_exist(sub, RD);
				}
				result += (curr);
				sub = "";

				break;
			case neglect:
				break;
			case reserved_key:
				if (i + 1 < size)
				{

					if (sub.size() > 0)
					{
						result += pre_exist(sub, RD);
					}
					result += (curr);
					sub = "";
					result += (' ');
					result += ('/');
					result += (input[i + 1]);
					i++;

				} else
				{
					sub += (curr);
				}
				break;
			case newLine:
				if (i == size - 1)
					break;
				break;
			default:
				break;
		}

	}
	if (sub.size() != 0)
	{
		result += pre_exist(sub, RD);
	}
	return "("+result+")";
}

string parser::getString(string token_name, string in)
{
	string result = in;

	int size = pattern_name.size();
	for (int i = 0; i < size; i++)
	{
		if (token_name.compare(pattern_name[i]) == 0)
			return pattern[i];
	}

	return result;
}

