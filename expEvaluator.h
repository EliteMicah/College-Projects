#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include <string>
#include <map>
#include <utility>
#include <stack>
#include "lexanalyzer.h"

using std::string;
using std::map;
using std::stack;
using std::pair;
using std::vector;

class expEvaluator {

	friend class lexicalAnalyzer;
	friend class Interface;
	friend class Interpreter;

public:

	expEvaluator();
	int calculator(string, int, int);
	void infixToPostfix(Interface&, lexicalAnalyzer&, expEvaluator& EE, Interpreter&);
	void postFixEval(Interface&, lexicalAnalyzer&, expEvaluator& EE, Interpreter&);
	int getPrecidence(string);
	bool higherPrec(pair<string, categoryType>, pair<string, categoryType>);
	void clear();
	void newVar(string, string);
	int varValue(string, Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
	void showVar(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);

private:
	vector<pair<string, categoryType>> expression;
	stack<double> dStack;
	typedef map<string, string> symbolTableType;
	symbolTableType symbolTable;

};

#endif