#ifndef LEXANALYZER_H
#define LEXANALYZER_H

#include <string>
#include <vector>
#include <utility>

using std::vector;
using std::string;
using std::pair;


enum class categoryType {

    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN

};

class lexicalAnalyzer {

    friend class expEvaluator;
    friend class Interface;
    friend class Interpreter;

public:

    void userShowTokens(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
    void getTokens(Interface&, lexicalAnalyzer&, expEvaluator&, Interpreter&);
    void tkClear();

    lexicalAnalyzer();

private:
    typedef vector<vector<pair<string, categoryType>>> tokenType;
    typedef vector<pair<string, categoryType>> tokenLineType;
    typedef pair<string, categoryType> pairType;
    tokenType tokenInfo;
    
    vector<string> vecCategory;
    vector<int> tokenLineCounter;
};

#endif