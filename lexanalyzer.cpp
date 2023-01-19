#include <iostream>
#include <cctype>
#include <utility>
#include <string>
#include <vector>
#include "lexanalyzer.h"
#include "interface.h"
#include "expevaluator.h"

using namespace std;

lexicalAnalyzer::lexicalAnalyzer() {
}

void lexicalAnalyzer::userShowTokens(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

    // Seperate counter for 
    int catCounter = 0;

    // Printing each line from the vector of strings saved in 'read()'
    for (int i = 0; i < ITF.programCode.size(); i++) {
        cout << "Line #" << i << ":" << endl;

        for (int k = 0; k < LA.tokenLineCounter[i]; k++) {
            cout << "Token[" << k << "]: ";
            cout << LA.tokenInfo[i][k].first << " - " << LA.vecCategory[catCounter] << endl;
            catCounter++;
        }
        cout << "----------------------------------------------------------" << endl;
    }
}

void lexicalAnalyzer::getTokens(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

    if (ITF.indentMode)
        IP.interpTokenInfo.clear();

    string s = "";
    char ch = 'a';

    for (int i = 0; i < ITF.programCode.size(); i++) {
        s = ITF.programCode[i];
        pairType pt;
        tokenLineType tlt;

        int dCounter = 0;
        int tokenCount = 0;
        string ipr;
        
        for (int k = 0; k < ITF.programCode[i].length(); k++) {
            ch = s[k];

            //      NUMERIC_LITERAL - DIGITS
            if (isdigit(s[k])) {
                string lineDigits;

                while (isdigit(s[k]) && isdigit(s[k+1])) {
                    lineDigits += s[k];
                    k++;
                }

                lineDigits += s[k];
                pt = make_pair(lineDigits, categoryType::NUMERIC_LITERAL);
                vecCategory.push_back("NUMERIC_LITERAL");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      QUOTE "" - STRING_LITERAL
            else if (s[k] == int(34)) {
                if (!ITF.interp) {
                    string str = "\"";
                    k++;

                    while (s[k] != int(34)) {
                        if (s[k] == ')' && k >= s.size() - 1) {
                            cout << "Error no ending quote found" << endl;
                            exit(1);
                        }
                        str += s[k];
                        k++;
                    }

                    str += "\"";
                    pt = make_pair(str, categoryType::STRING_LITERAL);
                    vecCategory.push_back("STRING_LITERAL");
                    tlt.push_back(pt);
                    tokenCount++;
                }
                else {
                    string str = "";
                    k++;

                    while (s[k] != int(34)) {
                        if (s[k] == ')' && k >= s.size() - 1) {
                            cout << "Error no ending quote found" << endl;
                            exit(1);
                        }
                        str += s[k];
                        k++;
                    }

                    str += "";
                    pt = make_pair(str, categoryType::STRING_LITERAL);
                    vecCategory.push_back("STRING_LITERAL");
                    tlt.push_back(pt);
                    tokenCount++;
                }
            }

            //      SINGLE QUOTE '' - STRING_LITERAL
            else if (s[k] == int(39)) {
                if (!ITF.interp) {
                    string str = "\'";
                    k++;

                    while (s[k] != int(39)) {
                        if (s[k] == ')' && k >= s.size() - 1) {
                            cout << "Error no ending quote found" << endl;
                            exit(1);
                        }
                        str += s[k];
                        k++;
                    }

                    str += "\'";
                    pt = make_pair(str, categoryType::STRING_LITERAL);
                    vecCategory.push_back("STRING_LITERAL");
                    tlt.push_back(pt);
                    tokenCount++;
                }
                else {
                    string str = "";
                    k++;

                    while (s[k] != int(39)) {
                        if (s[k] == ')' && k >= s.size() - 1) {
                            cout << "Error no ending quote found" << endl;
                            exit(1);
                        }
                        str += s[k];
                        k++;
                    }

                    str += "";
                    pt = make_pair(str, categoryType::STRING_LITERAL);
                    vecCategory.push_back("STRING_LITERAL");
                    tlt.push_back(pt);
                    tokenCount++;
                }
            }

            //      COMMENT
            else if (ch == '#') {
                string comm = "#";
                k++;

                while (k < ITF.programCode[i].length()) {
                    comm += s[k];
                    k++;
                }

                pt = make_pair(comm, categoryType::COMMENT);
                vecCategory.push_back("COMMENT");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      RELATIONAL OPERATOR ' == '
            else if (ch == '=' && s[k + 1] == '=' && k < (ITF.programCode[i].size() - 1)) {
                pt = make_pair("==", categoryType::RELATIONAL_OP);
                vecCategory.push_back("RELATIONAL_OPERATOR");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ASSIGNMENT OPERATOR ' = '
            else if (ch == '=') {
                pt = make_pair(ch, categoryType::ASSIGNMENT_OP);
                vecCategory.push_back("ASSIGNMENT_OPERATOR");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      COLON ' : '
            else if (ch == ':') {
                pt = make_pair(ch, categoryType::COLON);
                vecCategory.push_back("COLON");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      COMMA ' , '
            else if (ch == ',') {
                pt = make_pair(ch, categoryType::COMMA);
                vecCategory.push_back("COMMA");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      KEYWORD ' print '
            else if ((k + 3 < (ITF.programCode[i].size() - 1)) && s[k] == 'p' && s[k + 1] == 'r' && s[k + 2] == 'i' && s[k + 3] == 'n' && s[k + 4] == 't') {
                string kpr = "print";
                pt = make_pair(kpr, categoryType::KEYWORD);
                vecCategory.push_back("KEYWORD");
                k += 4;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      KEYWORD ' input '
            else if ((k + 3 < (ITF.programCode[i].size() - 1)) && s[k] == 'i' && s[k + 1] == 'n' && s[k + 2] == 'p' && s[k + 3] == 'u' && s[k + 4] == 't') {
                string kpr = "input";
                pt = make_pair(kpr, categoryType::KEYWORD);
                vecCategory.push_back("KEYWORD");
                k += 4;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      KEYWORD ' while '
            else if ((k + 3 < (ITF.programCode[i].size() - 1)) && s[k] == 'w' && s[k + 1] == 'h' && s[k + 2] == 'i' && s[k + 3] == 'l' && s[k + 4] == 'e') {
                string kpr = "while";
                pt = make_pair(kpr, categoryType::KEYWORD);
                vecCategory.push_back("KEYWORD");
                k += 4;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      KEYWORD ' if '
            else if (ch == 'i' && (k < (ITF.programCode[i].size() - 1)) && s[k + 1] == 'f') {
                string kpr = "if";
                pt = make_pair(kpr, categoryType::KEYWORD);
                vecCategory.push_back("KEYWORD");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      KEYWORD ' elif '
            else if (ch == 'e' && s[k + 1] == 'l' && s[k + 2] == 'i' && s[k + 3] == 'f' && (k + 2 < (ITF.programCode[i].size() - 1))) {
                string kpr = "elif";
                pt = make_pair(kpr, categoryType::KEYWORD);
                vecCategory.push_back("KEYWORD");
                k+= 3;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      KEYWORD ' else '
            else if (ch == 'e' && (k + 2 < (ITF.programCode[i].size() - 1)) && s[k + 1] == 'l' && s[k + 2] == 's' && s[k + 3] == 'e') {
                string kpr = "else";
                pt = make_pair(kpr, categoryType::KEYWORD);
                vecCategory.push_back("KEYWORD");
                k += 3;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      KEYWORD ' int '
            else if (ch == 'i' && (k + 1 < (ITF.programCode[i].size() - 1)) && s[k + 1] == 'n' && s[k + 2] == 't') {
                string kpr = "int";
                pt = make_pair(kpr, categoryType::KEYWORD);
                vecCategory.push_back("KEYWORD");
                k += 2;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      LOGICAL OPERATOR ' and '
            else if (ch == 'a' && (k + 1 < (ITF.programCode[i].size() - 1)) && s[k + 1] == 'n' && s[k + 2] == 'd') {
                string kpr = "and";
                pt = make_pair(kpr, categoryType::LOGICAL_OP);
                vecCategory.push_back("LOGICAL_OPERATOR");
                k += 2;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      LOGICAL_OPERATOR ' or '
            else if ((k < (ITF.programCode[i].size() - 1)) && s[k] == 'o' && s[k + 1] == 'r') {
                string kpr = "or";
                pt = make_pair(kpr, categoryType::LOGICAL_OP);
                vecCategory.push_back("LOGICAL_OPERATOR");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      LOGICAL OPERATOR ' not '
            else if (ch == 'n' && (k + 1 < (ITF.programCode[i].size() - 1)) && s[k + 1] == 'o' && s[k + 2] == 't') {
                string kpr = "not";
                pt = make_pair(kpr, categoryType::LOGICAL_OP);
                vecCategory.push_back("LOGICAL_OPERATOR");
                k += 2;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      IDENTIFIER
            else if (isalpha(s[k])) {
                string linealp;

                while (isalpha(s[k]) && isalpha(s[k + 1])) {
                    linealp += s[k];
                    k++;
                }

                linealp += s[k];
                pt = make_pair(linealp, categoryType::IDENTIFIER);
                vecCategory.push_back("IDENTIFIER");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      INDENT OR 4 SPACES
            else if (ch == '\t' || (k + 3 < (ITF.programCode[i].size() - 1)) && s[k] == ' ' && s[k + 1] == ' ' && s[k + 2] == ' ' && s[k + 3] == ' ') {
                pt = make_pair(ch, categoryType::INDENT);
                vecCategory.push_back("INDENT");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      RELATIONAL OPERATOR ' != '
            else if (ch == '!' && s[k + 1] == '=' && k < (ITF.programCode[i].size() - 1)) {
                pt = make_pair("!=", categoryType::RELATIONAL_OP);
                vecCategory.push_back("RELATIONAL_OPERATOR");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ARITHMETIC OPERATOR ' += '
            else if (ch == '+' && s[k + 1] == '=' && k < (ITF.programCode[i].size() - 1)) {
                pt = make_pair("+=", categoryType::ARITH_OP);
                vecCategory.push_back("ARITHMETIC_OPERATOR");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ARITHMETIC OPERATOR ' -= '
            else if (s[k] == '-' && s[k + 1] == '=' && k < (ITF.programCode[i].size() - 1)) {
                pt = make_pair("-=", categoryType::ARITH_OP);
                vecCategory.push_back("ARITHMETIC_OPERATOR");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      RELATIONAL OPERATOR ' <= '
            else if (ch == '<' && s[k + 1] == '=' && k < (ITF.programCode[i].size() - 1)) {
                pt = make_pair("<=", categoryType::RELATIONAL_OP);
                vecCategory.push_back("RELATIONAL_OPERATOR");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      RELATIONAL OPERATOR ' >= '
            else if (s[k] == '>' && s[k + 1] == '=' && k < (ITF.programCode[i].size() - 1)) {
                pt = make_pair(">=", categoryType::RELATIONAL_OP);
                vecCategory.push_back("RELATIONAL_OPERATOR");
                k++;
                tlt.push_back(pt);
                tokenCount++;
            }

            //      RELATIONAL OPERATOR ' < '
            else if (ch == '<') {
                pt = make_pair(ch, categoryType::RELATIONAL_OP);
                vecCategory.push_back("RELATIONAL_OPERATOR");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      RELATIONAL OPERATOR ' > '
            else if (ch == '>') {
                pt = make_pair(ch, categoryType::RELATIONAL_OP);
                vecCategory.push_back("RELATIONAL_OPERATOR");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ARITHMETIC OPERATOR ' + '
            else if (ch == '+') {
                pt = make_pair(ch, categoryType::ARITH_OP);
                vecCategory.push_back("ARITH_OP");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ARITHMETIC OPERATOR ' - '
            else if (ch == '-') {
                pt = make_pair(ch, categoryType::ARITH_OP);
                vecCategory.push_back("ARITH_OP");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ARITHMETIC OPERATOR ' * '
            else if (ch == '*') {
                pt = make_pair(ch, categoryType::ARITH_OP);
                vecCategory.push_back("ARITH_OP");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ARITHMETIC OPERATOR ' / '
            else if (ch == '/') {
                pt = make_pair(ch, categoryType::ARITH_OP);
                vecCategory.push_back("ARITH_OP");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      ARITHMETIC OPERATOR ' % '
            else if (ch == '%') {
                pt = make_pair(ch, categoryType::ARITH_OP);
                vecCategory.push_back("ARITH_OP");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      LEFT PAREN
            else if (ch == '(') {
                pt = make_pair(ch, categoryType::LEFT_PAREN);
                vecCategory.push_back("LEFT_PARENTHESIS");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      RIGHT PAREN
            else if (ch == ')') {
                pt = make_pair(ch, categoryType::RIGHT_PAREN);
                vecCategory.push_back("RIGHT_PARENTHESIS");
                tlt.push_back(pt);
                tokenCount++;
            }

            //      UNKNOWN
            else if (!isspace(ch) && !isblank(ch)) {
                pt = make_pair(ch, categoryType::UNKNOWN);
                vecCategory.push_back("UNKNOWN");
                tlt.push_back(pt);
                tokenCount++;
            }
        }

        if (!ITF.interp)
            LA.tokenInfo.push_back(tlt);
        else if (ITF.intMode && !ITF.indentMode) {
            IP.interpTokenInfo.clear();
            LA.tokenLineCounter.clear();
            IP.interpTokenInfo.push_back(tlt);
        }
        else if (ITF.intMode && ITF.indentMode)
            IP.interpTokenInfo.push_back(tlt);
        else
            IP.interpTokenInfo.push_back(tlt);
        tokenLineCounter.push_back(1);
        tokenLineCounter[i] = tokenCount;
    }

    //      Checks if user wants Infix to Postfix or Show Tokens
    if (ITF.exp)
        EE.infixToPostfix(ITF, LA, EE, IP);
    else if (ITF.interp)
        IP.interpretLine(ITF, LA, EE, IP);
    else
        userShowTokens(ITF, LA, EE, IP);
}

void lexicalAnalyzer::tkClear() {
    tokenInfo.clear();
}