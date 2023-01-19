#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <stack>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
using namespace std;

expEvaluator::expEvaluator() {
}

int expEvaluator::calculator(string s, int x, int y) {

	if (s == "*")
		return x * y;
	else if (s == "/")
		return x / y;
	else if (s == "%")
		return x % y;
	else if (s == "+")
		return x + y;
	else if (s == "-")
		return x - y;
	else if (s == "<")	{
		if (x < y)
			return 1;
		else
			return 0;
	}
	else if (s == "<=") {
		if (x <= y)
			return 1;
		else
			return 0;
	}
	else if (s == ">")	{
		if (x > y)
			return 1;
		else
			return 0;
	}
	else if (s == ">=") {
		if (x >= y)
			return 1;
		else
			return 0;
	}
	else if (s == "!=") {
		if (x != y)
			return 1;
		else
			return 0;
	}
	else if (s == "==") {
		if (x == y)
			return 1;
		else
			return 0;
	}
	else if (s == "and") {
		if (x == y)
			return 1;
		else
			return 0;
	}
	else if (s == "or") {
		if (x == 1 || y == 1)
			return 1;
		else
			return 0;
	}
}

void expEvaluator::infixToPostfix(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

	stack<pair<string, categoryType>> tempStack;

	if (!ITF.interp) {
		for (int i = 0; i < LA.tokenInfo[LA.tokenInfo.size() - 1].size(); i++) {
			string c = "";
			//      NUMERIC_LITERAL - DIGITS
			if (LA.tokenInfo[LA.tokenInfo.size() - 1][i].second == categoryType::NUMERIC_LITERAL)
				EE.expression.push_back(LA.tokenInfo[LA.tokenInfo.size() - 1][i]);

			//		LEFT PARENTHESIS
			else if (LA.tokenInfo[LA.tokenInfo.size() - 1][i].second == categoryType::LEFT_PAREN)
				tempStack.push(LA.tokenInfo[LA.tokenInfo.size() - 1][i]);

			//		RIGHT PARENTHESIS
			else if (LA.tokenInfo[LA.tokenInfo.size() - 1][i].second == categoryType::RIGHT_PAREN) {

				if (tempStack.empty()) {
					cout << "Not a valid expression" << endl;
					exit(1);
				}
				while (true) {
					if (tempStack.top().second == categoryType::LEFT_PAREN) {
						tempStack.pop();
						break;
					}
					EE.expression.push_back(tempStack.top());
					tempStack.pop();
				}
			}

			//		Checking for any CAT TYPE 0 - 5
			else if (LA.tokenInfo[LA.tokenInfo.size() - 1][i].second == categoryType::ARITH_OP
				|| LA.tokenInfo[LA.tokenInfo.size() - 1][i].second == categoryType::RELATIONAL_OP
				|| LA.tokenInfo[LA.tokenInfo.size() - 1][i].second == categoryType::LOGICAL_OP) {

				//		If stack is empty   &&   stack.top() != '('   &&   Checking precidence order
				while (!tempStack.empty() && tempStack.top().second != categoryType::LEFT_PAREN &&
					higherPrec(tempStack.top(), LA.tokenInfo[LA.tokenInfo.size() - 1][i])) {
					EE.expression.push_back(tempStack.top());
					tempStack.pop();
				}

				//		Push if otherwise ^
				tempStack.push(LA.tokenInfo[LA.tokenInfo.size() - 1][i]);
			}
		}

		//		Clearing the Stack   &&   Adding to expression
		while (!tempStack.empty()) {
			EE.expression.push_back(tempStack.top());
			tempStack.pop();
		}
	}
	else {
		for (int i = 0; i < IP.tlt.size(); i++) {
			string c = "";

			//      NUMERIC_LITERAL - DIGITS
			if (IP.tlt[i].second == categoryType::NUMERIC_LITERAL)
				IP.interpExpression.push_back(IP.tlt[i]);

			//		IDENTIFIER
			else if (IP.tlt[i].second == categoryType::IDENTIFIER) {

				// Return true if key is inside
				if (EE.symbolTable.count(IP.tlt[i].first)) {

					//		Find key and place its value in the expression
					map<string, string>::iterator it = EE.symbolTable.begin();

					while (it != EE.symbolTable.end()) {
						if (it == EE.symbolTable.find(IP.tlt[i].first)) {
							vector<pair<string, categoryType>> tempE;
							string s = to_string(EE.varValue(IP.tlt[i].first, ITF, LA, EE, IP));
							tempE.push_back(make_pair(s, categoryType::NUMERIC_LITERAL));
							IP.interpExpression.push_back(tempE[0]);
						}
						it++;
					}
				}
			}

			//		LEFT PARENTHESIS
			else if (IP.tlt[i].second == categoryType::LEFT_PAREN)
				tempStack.push(IP.interpTokenInfo[IP.interpTokenInfo.size() - 1][i]);

			//		RIGHT PARENTHESIS
			else if (IP.tlt[i].second == categoryType::RIGHT_PAREN) {

				if (tempStack.empty()) {
					cout << "Not a valid expression" << endl;
					exit(1);
				}
				while (true) {
					if (tempStack.top().second == categoryType::LEFT_PAREN) {
						tempStack.pop();
						break;
					}
					IP.interpExpression.push_back(tempStack.top());
					tempStack.pop();
				}
			}

			//		Checking for any CAT TYPE 0 - 5
			else if (IP.tlt[i].second == categoryType::ARITH_OP
				|| IP.tlt[i].second == categoryType::RELATIONAL_OP
				|| IP.tlt[i].second == categoryType::LOGICAL_OP) {

				//		If stack is empty   &&   stack.top() != '('   &&   Checking precidence order
				while (!tempStack.empty() && tempStack.top().second != categoryType::LEFT_PAREN &&
					higherPrec(tempStack.top(), IP.interpExpression[i])) {
					IP.interpExpression.push_back(tempStack.top());
					tempStack.pop();
				}

				//		Push if otherwise ^
				tempStack.push(IP.tlt[i]);
			}
		}

		//		Clearing the Stack   &&   Adding to expression
		while (!tempStack.empty()) {
			IP.interpExpression.push_back(tempStack.top());
			tempStack.pop();
		}
	}

	/*cout << "infix to Postfix: ";
	for (int i = 0; i < expression.size(); i++)
		cout << expression[i].first;
	cout << endl; */

    postFixEval(ITF, LA, EE, IP);
}

void expEvaluator::postFixEval(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

	int var1, var2;
	if (!ITF.interp) {

		for (int i = 0; i < EE.expression.size(); i++) {

			//		Checking for Digits
			if (EE.expression[i].second == categoryType::NUMERIC_LITERAL)
				EE.dStack.push(stod(expression[i].first));

			// Checking for " NOT "
			else if (EE.expression[i].first == "not") {
				if (EE.dStack.empty()) {
					cout << "Error not a valid expression." << endl;
					exit(1);
				}

				var1 = EE.dStack.top();

				if (var1 == 1)
					EE.dStack.push(0);
				else if (var1 == 0)
					EE.dStack.push(1);
				else {
					cout << "Not a valid expression." << endl;
					exit(1);
				}
			}

			else {

				//		Check for Errors
				if (EE.dStack.empty()) {
					cout << "Not a valid expression." << endl;
					exit(1);
				}
				var1 = EE.dStack.top();
				EE.dStack.pop();

				//		Check for Errors
				if (EE.dStack.empty()) {
					cout << "Not a valid expression." << endl;
					exit(1);
				}
				var2 = EE.dStack.top();
				EE.dStack.pop();

				//		RETURNING ' x op y '
				EE.dStack.push(calculator(expression[i].first, var2, var1));
			}
		}

		//		CHECKING FOR LAST ERRORS
		if (EE.dStack.empty()) {
			cout << "Error" << endl;
			exit(1);
		}
	}
	else {
		for (int i = 0; i < IP.interpExpression.size(); i++) {

			//		Checking for Digits
			if (IP.interpExpression[i].second == categoryType::NUMERIC_LITERAL)
				IP.interpDStack.push(stoi(IP.interpExpression[i].first));

			// Checking for " NOT "
			else if (IP.interpExpression[i].first == "not") {
				if (IP.interpDStack.empty()) {
					cout << "Error not a valid expression." << endl;
					exit(1);
				}

				var1 = IP.interpDStack.top();

				if (var1 == 1)
					IP.interpDStack.push(0);
				else if (var1 == 0)
					IP.interpDStack.push(1);
				else {
					cout << "Not a valid expression." << endl;
					exit(1);
				}
			}

			else {

				//		Check for Errors
				if (IP.interpDStack.empty()) {
					cout << "Not a valid expression." << endl;
					exit(1);
				}
				var1 = IP.interpDStack.top();
				IP.interpDStack.pop();

				//		Check for Errors
				if (IP.interpDStack.empty()) {
					cout << "Not a valid expression." << endl;
					exit(1);
				}
				var2 = IP.interpDStack.top();
				IP.interpDStack.pop();

				//		RETURNING ' x op y '
				IP.interpDStack.push(calculator(IP.interpExpression[i].first, var2, var1));
			}
		}

		//		CHECKING FOR LAST ERRORS
		if (IP.interpDStack.empty()) {
			cout << "Error" << endl;
			exit(1);
		}
	}

	if (!ITF.interp)
		//		Print the result
		cout << "\t|  " << EE.dStack.top() << "  |" << endl;
	else
		IP.result = to_string(IP.interpDStack.top());

}

int expEvaluator::getPrecidence(string s) {
	
	//		CAT 5
    if (s == "*" || s == "/" || s == "%")
        return 5;

    //		CAT 4
    else if (s == "+" || s == "-")
        return 4;

    //		CAT 3
    else if (s == "<" || s == "<=" || s == ">" || s == ">=" || s == "!=" || s == "==")
        return 3;

    //		CAT 2
    else if (s == "not")
        return 2;

    //		CAT 1
    else if (s == "and")
        return 1;

    //		CAT 0
    else if (s == "or")
        return 0;
}

bool expEvaluator::higherPrec(pair<string, categoryType> tempStack1, pair<string, categoryType> topStack) {
	if (getPrecidence(tempStack1.first) >= getPrecidence(topStack.first))
		return true;
	else
		return false;
}

void expEvaluator::clear() {

	symbolTable.clear();
}

void expEvaluator::newVar(string var, string val) {

	symbolTable[var] = val;
}

int expEvaluator::varValue(string var, Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

	//		Returns integer value of the value for the variable
	if (symbolTable.count(var))
		return stoi(symbolTable[var]);

	else {
		cout << "Value not found" << endl;
		ITF.indentMode = false;
		ITF.intMode = false;
		ITF.userInput(ITF, LA, EE, IP);
	}
}

void expEvaluator::showVar(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

	map<string, string>::iterator it = EE.symbolTable.begin();
	cout << "| VARIABLES |" << endl;
	int count = 0;
	cout << "\t\tKEY\tVALUE" << endl;
	while (it != EE.symbolTable.end())	{
		cout << "SYMBOL[" << count << "]" << "\t" << it->first << "\t" << it->second << endl;
		it++;
		count++;
	}
}