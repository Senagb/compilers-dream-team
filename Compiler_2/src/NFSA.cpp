#include "NFSA.h"
#include "Parser.h"

using namespace std;

NFSA::~NFSA() {

}

NFSA::NFSA() {

}

NFSA::NFSA(InputFileParser input) {
	file = input;
	start = new state();
	start->set(source, 0);
	definitions = vector<string>();
	valid = true;
	states = vector<state*>();

}

void NFSA::combine() {

}

void NFSA::generateNFSA() {

	vector<parsed> RE = file.getRuleTable();

	vector<parsed> defs = file.getDefinitionTable();

	int size = RE.size();
	vector<string> def_definitions = vector<string>();
	vector<state*> def_states = vector<state*>();
	parser def_parser = parser("", file.getDefinitionTable(), &def_states,
			&def_definitions);
	int def_size = defs.size();
	for (int i = 0; i < def_size; i++) {
		parsed e = defs[i];
		def_parser.reset(e.order, e.data);
		state* temp = def_parser.parse(e.data, e.name, e.type);

		if (def_parser.isValid()) {
		} else
			signal_error(def_parser.errors, e, i);

	}

	parser p = parser("", file.getCombined(), &states, &definitions);
	vector<parsed> RE1 = file.getDefinitionTable();

	states.push_back(start);
	printGraph();

//	return;
	for (int i = 0; i < RE1.size(); i++) {
		parsed e = RE1[i];
		if (e.type.compare(KEYWORD) == 0 || e.type.compare(PUNC) == 0) {
			p.reset(e.order, e.data);
			state* temp = p.parse(e.data, e.name, e.type);
			start->insertChild(epsion, temp, false);
			printGraph();

		}
		cout << e.name << "," << e.name << "," << e.type << endl;
	}

	for (int i = 0; i < size; i++) {
		parsed e = RE[i];
		p.reset(e.order, e.data);
		state* temp = p.parse(e.data, e.name, e.type);

		if (p.isValid()) {
			start->insertChild(epsion, temp, false);
			printGraph();
		} else
			signal_error(p.errors, e, i);

	}

//	cout << "Index Array" << endl;
	for (int i = 0; i < 256; i++) {
		index_array[i] = p.index_array[i];
//		cout << (char) i << "  : " << index_array[i] << endl;
	}
	printGraph();
//	start->print();

}
string getString(int value) {
	char str[15];
	sprintf(str, "%d", value);
	cout << str << endl;
	return str;

}
int NFSA::translate_to_index(int Ascii) {
	return index_array[Ascii];
}
void NFSA::signal_error(vector<string> curr_errors, parsed current, int index) {
	valid = false;

	errors.push_back("Error Found at ");
	errors.push_back(current.name);
	errors.push_back(" Regular Expression # ");

	errors.push_back(getString(index));
	errors.push_back(".\n");
	int size = curr_errors.size();
	int i;
	for (i = 0; i < size; i++)
		errors.push_back(curr_errors[i]);
	errors.push_back("*******************************************\n");

}

vector<parsed> NFSA::getDefinitionTable() {

	return file.getDefinitionTable();
}

vector<string> NFSA::getDefinitions() {
	return definitions;
}

vector<parsed> NFSA::getRuleTable() {
	return file.getRuleTable();
}
state* NFSA::getStart() {
	return start;
}
bool NFSA::isValid() {
	return valid;
}
void NFSA::printErrors() {
	int size = errors.size();

	if (valid && size == 0)
		cout << "No errors." << endl;
	else {
		int i;
		valid = false;
		for (i = 0; i < size; i++)
			cout << errors[i];
	}
}
void NFSA::printGraph1() {
	cout << "Definitions" << endl;
	for (int i = 0; i < definitions.size(); i++) {
		cout << i << "  :  " << definitions[i] << endl;
	}

	//		std::vector<state*>::iterator iter = states.begin();
//		while (iter != states.end())
//		{
//			try
//			{
//				state* e = *iter;
//				e->printTable();
//
//			} catch (int e)
//			{
//				break;
//			}
//			++iter;
//		}
//		cout << endl << endl;

}

void NFSA::printGraph() {

	cout << "Definitions" << endl;
	for (int i = 0; i < definitions.size(); i++) {
		cout << i << "  :  " << definitions[i] << endl;
	}
	std::vector<state*>::iterator iter = states.begin();
	while (iter != states.end()) {
		try {
			state* e = *iter;
			e->printTable();

		} catch (int e) {
			break;
		}
		++iter;
	}
	cout << endl << endl;
}

