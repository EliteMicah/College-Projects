#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"

using std::vector;
using std::string;

class Interface {

	friend class lexicalAnalyzer;
	friend class expEvaluator;
	friend class Interpreter;

public:
	void startInterface();
	void userInput(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
	void userHelp(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
	void userRead(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
	void userShow(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
	void userClear(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
	void interactiveMode(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
	void findPrec(expEvaluator&);

	Interface();

private:
	typedef vector<string> programType;
	programType programCode;
	string filename;
	string precidence;
	string printStatement;

	bool exp = false;
	bool interp = false;
	bool intMode = false;
	bool indentMode = false;

};

#endif