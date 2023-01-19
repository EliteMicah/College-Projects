#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include "expevaluator.h"
#include "lexanalyzer.h"

class Interpreter {

	friend class expEvaluator;
	friend class lexicalAnalyzer;
	friend class interface;

public:

	Interpreter();
	void interpretLine(Interface&, lexicalAnalyzer&, expEvaluator& EE, Interpreter&);
	void clear(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);

private:
	typedef vector<vector<pair<string, categoryType>>> tokenType;
	typedef vector<pair<string, categoryType>> tokenLineType;
	typedef pair<string, categoryType> pairType;
	tokenLineType tlt;
	tokenType interpTokenInfo;
	vector<pair<string, categoryType>> interpExpression;
	stack<int> interpDStack;
	string result;
};

#endif
