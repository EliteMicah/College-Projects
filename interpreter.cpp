#include<iostream>
#include "interpreter.h"
#include "interface.h"
#include "expevaluator.h"
#include "lexanalyzer.h"
using namespace std;

Interpreter::Interpreter() {
}

void Interpreter::interpretLine(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

	bool userStatement = false;
	bool ifPrinted = false;
	bool whileStatement = false;
	bool finishedWhile = false;

	// Printing each line from the vector of strings saved in 'read()'
	for (int i = 0; i < ITF.programCode.size(); i++) {

		for (int k = 0; k < LA.tokenLineCounter[i]; k++) {

			//cout << IP.interpTokenInfo[i][k].first << endl;

			if (IP.interpTokenInfo[i][k].second == categoryType::COMMENT)
				continue;

			//      NORMAL PRINT STATEMENT ' print() ' 
			else if (IP.interpTokenInfo[i][k].first == "print") {

				//		Print with If Statement
				if (k - 1 >= 0 && IP.interpTokenInfo[i][k - 1].second == categoryType::INDENT && userStatement == true && ifPrinted == false) {
					ifPrinted = true;

					if (IP.interpTokenInfo[i][k + 1].second == categoryType::LEFT_PAREN) {
						k += 2;
						int temp = 0;
						for (int j = k; j < LA.tokenLineCounter[i]; j++) {

							//		PRINT ' STRING ' " print("hello")
							if (IP.interpTokenInfo[i][j].second == categoryType::STRING_LITERAL) {
								cout << IP.interpTokenInfo[i][j].first << " ";
								j++;
							}

							//		PRINT ' IDENTIFIER ' " print(t) "
							else if (IP.interpTokenInfo[i][j].second == categoryType::IDENTIFIER) {
								cout << EE.varValue(IP.interpTokenInfo[i][j].first, ITF, LA, EE, IP);
								if (IP.interpTokenInfo[i][j + 1].second != categoryType::RIGHT_PAREN)
									cout << " ";
								j++;
							}

							//		ARITHMETIC OPERATOR ' + - * / '
							else if (IP.interpTokenInfo[i][j].second == categoryType::ARITH_OP) {
								if (IP.interpTokenInfo[i][j - 1].second == categoryType::IDENTIFIER && IP.interpTokenInfo[i][j + 1].second == categoryType::NUMERIC_LITERAL) {
									IP.tlt.push_back(IP.interpTokenInfo[i][j - 1]);
									IP.tlt.push_back(IP.interpTokenInfo[i][j]);
									IP.tlt.push_back(IP.interpTokenInfo[i][j + 1]);
									EE.infixToPostfix(ITF, LA, EE, IP);
									EE.newVar(IP.interpTokenInfo[i][j - 1].first, IP.result);
									cout << EE.varValue(IP.interpTokenInfo[i][j - 1].first, ITF, LA, EE, IP);
									j++;
								}
							}
							temp++;
						}
						k += temp;
					}
				}

				//		Print Statement for Interactive Mode
				else if (ITF.indentMode == true && userStatement == true && ifPrinted == false) {
					if (IP.interpTokenInfo[i][k + 1].second == categoryType::LEFT_PAREN) {
						k += 2;
						int temp = 0;
						for (int j = k; j < LA.tokenLineCounter[i]; j++) {

							//		PRINT ' STRING ' " print("hello")
							if (IP.interpTokenInfo[i][j].second == categoryType::STRING_LITERAL) {
								cout << IP.interpTokenInfo[i][j].first << " ";
								j++;
							}

							//		PRINT ' IDENTIFIER ' " print(t) "
							else if (IP.interpTokenInfo[i][j].second == categoryType::IDENTIFIER) {
								cout << EE.varValue(IP.interpTokenInfo[i][j].first, ITF, LA, EE, IP);
								if (IP.interpTokenInfo[i][j + 1].second != categoryType::RIGHT_PAREN)
									cout << " ";
								j++;
							}

							//		ARITHMETIC OPERATOR ' + - * / '
							else if (IP.interpTokenInfo[i][j].second == categoryType::ARITH_OP) {
								if (IP.interpTokenInfo[i][j - 1].second == categoryType::IDENTIFIER && IP.interpTokenInfo[i][j + 1].second == categoryType::NUMERIC_LITERAL) {
									IP.tlt.push_back(IP.interpTokenInfo[i][j - 1]);
									IP.tlt.push_back(IP.interpTokenInfo[i][j]);
									IP.tlt.push_back(IP.interpTokenInfo[i][j + 1]);
									EE.infixToPostfix(ITF, LA, EE, IP);
									EE.newVar(IP.interpTokenInfo[i][j - 1].first, IP.result);
									cout << EE.varValue(IP.interpTokenInfo[i][j - 1].first, ITF, LA, EE, IP);
									j++;
								}
							}
							temp++;
						}
						k += temp;
					}
				}

				//		If another if statement was already printed then break
				else if (ifPrinted == true)
					break;

				//		Normal Print() statement without an if statement
				else if (IP.interpTokenInfo[i][k + 1].second == categoryType::LEFT_PAREN && userStatement == false) {
					k += 2;
					int temp = 0;
					for (int j = k; j < LA.tokenLineCounter[i]; j++) {

						//		PRINT ' STRING ' " print("hello")
						if (IP.interpTokenInfo[i][j].second == categoryType::STRING_LITERAL) {
							cout << IP.interpTokenInfo[i][j].first << " ";
							j++;
						}

						//		PRINT ' IDENTIFIER ' " print(t) "
						else if (IP.interpTokenInfo[i][j].second == categoryType::IDENTIFIER) {
							cout << EE.varValue(IP.interpTokenInfo[i][j].first, ITF, LA, EE, IP);
							if (IP.interpTokenInfo[i][j].second != categoryType::RIGHT_PAREN)
								cout << " ";
							j++;
						}

						//		ARITHMETIC OPERATOR ' + - * / '
						else if (IP.interpTokenInfo[i][j].second == categoryType::ARITH_OP) {
							if (IP.interpTokenInfo[i][j - 1].second == categoryType::IDENTIFIER && IP.interpTokenInfo[i][j + 1].second == categoryType::NUMERIC_LITERAL) {
								IP.tlt.push_back(IP.interpTokenInfo[i][j - 1]);
								IP.tlt.push_back(IP.interpTokenInfo[i][j]);
								IP.tlt.push_back(IP.interpTokenInfo[i][j + 1]);
								EE.infixToPostfix(ITF, LA, EE, IP);
								EE.newVar(IP.interpTokenInfo[i][j - 1].first, IP.result);
								cout << EE.varValue(IP.interpTokenInfo[i][j - 1].first, ITF, LA, EE, IP);
								k = LA.tokenLineCounter[i];
							}
						}
						temp++;
					}
					k += temp;
				}

				k = LA.tokenLineCounter[i];
				cout << endl;
			}

			//		IF STATEMENT
			else if (IP.interpTokenInfo[i][k].first == "if") {
				k++;

				userStatement = false;

				//					IDENTIFIER ' x < 5 '   ||   ' x < y '
				if (IP.interpTokenInfo[i][k].second == categoryType::IDENTIFIER && k < IP.interpTokenInfo[i].size() - 2) {

					if (IP.interpTokenInfo[i][k + 1].second == categoryType::RELATIONAL_OP) {

						if (IP.interpTokenInfo[i][k + 1].first == "<") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) < stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) < EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == ">") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) > stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) > EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == "<=") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) <= stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) <= EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == ">=") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) >= stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) >= EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == "==") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) == stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) == EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == "!=") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) != stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) != EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}
					}

				}
				else {
					cout << "Error with if Statement" << endl;
					exit(1);
				}

			}

			//		ELIF STATEMENT
			else if (IP.interpTokenInfo[i][k].first == "elif") {

				if (userStatement == true)
					break;
				k += 2;

				//					IDENTIFIER ' x < 5 '   ||   ' x < y '
				if (IP.interpTokenInfo[i][k].second == categoryType::IDENTIFIER && k < IP.interpTokenInfo[i].size() - 2) {

					//cout << EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP);
					if (IP.interpTokenInfo[i][k + 1].second == categoryType::RELATIONAL_OP) {

						if (IP.interpTokenInfo[i][k + 1].first == "<") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) < stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) < EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == ">") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) > stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) > EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == "<=") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) <= stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) <= EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == ">=") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) >= stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) >= EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == "==") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) == stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) == EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}

						else if (IP.interpTokenInfo[i][k + 1].first == "!=") {
							if (IP.interpTokenInfo[i][k + 2].second == categoryType::NUMERIC_LITERAL) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) != stod(IP.interpTokenInfo[i][k + 2].first)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
							else if (IP.interpTokenInfo[i][k + 2].second == categoryType::IDENTIFIER) {
								if (EE.varValue(IP.interpTokenInfo[i][k].first, ITF, LA, EE, IP) != EE.varValue(IP.interpTokenInfo[i][k + 2].first, ITF, LA, EE, IP)) {
									userStatement = true;
									ifPrinted = false;
									k = LA.tokenLineCounter[i];
								}
							}
						}
					}

				}
				else {
					cout << "Error with elif Statement" << endl;
					exit(1);
				}
			}

			//		ELSE STATEMENT
			else if (IP.interpTokenInfo[i][k].first == "else") {

				if (userStatement == true)
					break;
				k++;

				userStatement = true;
				ifPrinted = false;
			}
			
			//		WHILE STATEMENT
			else if (IP.interpTokenInfo[i][k].first == "while") {
				k++;
				whileStatement = false;

				int tempi = i;
				int tempk = k;

				//cout << "k --> " << k << endl;
				//cout << "size-1 --> " << IP.interpTokenInfo[i].size()-1 << endl;

				if (k >= IP.interpTokenInfo[i].size() - 1) {
					cout << "Error with while statement" << endl;
					exit(1);
				}


				//		To keep the original condition
				string firstToken = IP.interpTokenInfo[tempi][tempk].first;
				string secondToken = IP.interpTokenInfo[tempi][tempk +1].first;
				string thirdToken1 = IP.interpTokenInfo[tempi][tempk + 2].first;
				categoryType thirdToken2 = IP.interpTokenInfo[tempi][tempk +2].second;

				//		goto point for checking if whileStatement is still true
			p:

				if (secondToken == "<") {
					if (thirdToken2 == categoryType::NUMERIC_LITERAL) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) < stod(thirdToken1)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
					else if (thirdToken2 == categoryType::IDENTIFIER) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) < EE.varValue(thirdToken1, ITF, LA, EE, IP)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
				}

				else if (secondToken == ">") {
					if (thirdToken2 == categoryType::NUMERIC_LITERAL) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) > stod(thirdToken1)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
					else if (thirdToken2 == categoryType::IDENTIFIER) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) > EE.varValue(thirdToken1, ITF, LA, EE, IP)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
				}

				else if (secondToken == "<=") {
					if (thirdToken2 == categoryType::NUMERIC_LITERAL) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) <= stod(thirdToken1)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
					else if (thirdToken2 == categoryType::IDENTIFIER) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) <= EE.varValue(thirdToken1, ITF, LA, EE, IP)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
				}

				else if (secondToken == ">=") {
					if (thirdToken2 == categoryType::NUMERIC_LITERAL) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) >= stod(thirdToken1)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
					else if (thirdToken2 == categoryType::IDENTIFIER) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) >= EE.varValue(thirdToken1, ITF, LA, EE, IP)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
				}

				else if (secondToken == "==") {
					if (thirdToken2 == categoryType::NUMERIC_LITERAL) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) == stod(thirdToken1)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
					else if (thirdToken2 == categoryType::IDENTIFIER) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) == EE.varValue(thirdToken1, ITF, LA, EE, IP)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
				}

				else if (secondToken == "!=") {
					if (thirdToken2 == categoryType::NUMERIC_LITERAL) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) != stod(thirdToken1)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
					else if (thirdToken2 == categoryType::IDENTIFIER) {
						if (EE.varValue(firstToken, ITF, LA, EE, IP) != EE.varValue(thirdToken1, ITF, LA, EE, IP)) {
							whileStatement = true;
							k = LA.tokenLineCounter[i];
						}
						else
							whileStatement = false;
					}
				}

				//		If none of the tokens line up then break
				else
					break;

				if (whileStatement) {
					i = tempi + 1;

				m:
					for (int j = 0; j < LA.tokenLineCounter[i]; j++) {

						//cout << "--> " << IP.interpTokenInfo[i][j].first << endl;

						if (IP.interpTokenInfo[i][j].first == "print") {
							j += 2;

							//		PRINT ' STRING ' " print("hello")
							if (IP.interpTokenInfo[i][j].second == categoryType::STRING_LITERAL) {
								cout << IP.interpTokenInfo[i][j].first << " ";
								if (IP.interpTokenInfo[i][j + 1].second == categoryType::RIGHT_PAREN)
									cout << endl;
								j++;
							}

							//		PRINT ' IDENTIFIER ' " print(t) "
							else if (IP.interpTokenInfo[i][j].second == categoryType::IDENTIFIER) {
								cout << EE.varValue(IP.interpTokenInfo[i][j].first, ITF, LA, EE, IP);
								if (IP.interpTokenInfo[i][j + 1].second != categoryType::RIGHT_PAREN)
									cout << " ";
								else
									cout << endl;
								j++;
							}

							//		ARITHMETIC OPERATOR ' + - * / '
							else if (IP.interpTokenInfo[i][j].second == categoryType::ARITH_OP) {
								if (IP.interpTokenInfo[i][j - 1].second == categoryType::IDENTIFIER && IP.interpTokenInfo[i][j + 1].second == categoryType::NUMERIC_LITERAL) {
									IP.tlt.push_back(IP.interpTokenInfo[i][j - 1]);
									IP.tlt.push_back(IP.interpTokenInfo[i][j]);
									IP.tlt.push_back(IP.interpTokenInfo[i][j + 1]);
									EE.infixToPostfix(ITF, LA, EE, IP);
									EE.newVar(IP.interpTokenInfo[i][j - 1].first, IP.result);
									cout << EE.varValue(IP.interpTokenInfo[i][j - 1].first, ITF, LA, EE, IP);
									k++;
								}
							}
						}

						//		ASSIGNMENT OPERATOR ' = '
						else if (IP.interpTokenInfo[i][j].second == categoryType::ASSIGNMENT_OP) {

							//		Adding Number ' x = x + 1 '
							if (IP.interpTokenInfo[i][j - 1].second == categoryType::IDENTIFIER
								&& IP.interpTokenInfo[i][j + 1].second == categoryType::IDENTIFIER
								&& IP.interpTokenInfo[i][j + 2].second == categoryType::ARITH_OP
								&& IP.interpTokenInfo[i][j + 3].second == categoryType::NUMERIC_LITERAL) {

								IP.tlt.clear();
								IP.tlt.push_back(IP.interpTokenInfo[i][j + 1]);
								IP.tlt.push_back(IP.interpTokenInfo[i][j + 2]);
								IP.tlt.push_back(IP.interpTokenInfo[i][j + 3]);
								EE.infixToPostfix(ITF, LA, EE, IP);
								EE.newVar(IP.interpTokenInfo[i][j - 1].first, IP.result);
								j = LA.tokenLineCounter[i];
							}
						}
					}

					// Checking for more lines within while loop
					if (i + 1 < ITF.programCode.size() && IP.interpTokenInfo[i + 1][0].second == categoryType::INDENT) {
						i++;
						goto m;
					}
					else if (i + 1 < ITF.programCode.size() && ITF.indentMode == true) {
						i++;
						goto m;
					}


					// No more lines? Check back to see if while loop is still true
					goto p;
				}

				//		Done with While Loop
				else 
					break;
			}


			//		ASSIGNMENT OPERATOR ' = '
			else if (IP.interpTokenInfo[i][k].second == categoryType::ASSIGNMENT_OP) {

				//		' x = 3 '
				if (IP.interpTokenInfo[i][k - 1].second == categoryType::IDENTIFIER && IP.interpTokenInfo[i][k + 1].second == categoryType::NUMERIC_LITERAL) {
					EE.newVar(IP.interpTokenInfo[i][k - 1].first, IP.interpTokenInfo[i][k + 1].first);
					k++;
				}

				//		Adding Number ' x = x + 1 '
				else if (IP.interpTokenInfo[i][k - 1].second == categoryType::IDENTIFIER 
					&& IP.interpTokenInfo[i][k + 1].second == categoryType::IDENTIFIER 
					&& IP.interpTokenInfo[i][k + 2].second == categoryType::ARITH_OP
					&& IP.interpTokenInfo[i][k + 3].second == categoryType::NUMERIC_LITERAL) {
					
					IP.tlt.push_back(IP.interpTokenInfo[i][k + 1]);
					IP.tlt.push_back(IP.interpTokenInfo[i][k + 2]);
					IP.tlt.push_back(IP.interpTokenInfo[i][k + 3]);
					EE.infixToPostfix(ITF, LA, EE, IP);
					EE.newVar(IP.interpTokenInfo[i][k - 1].first, IP.result);
					k += 3;
				}
			}

			//		ARITHMETIC OPERATOR ' + - * / '
			else if (IP.interpTokenInfo[i][k].second == categoryType::ARITH_OP) {
				if (IP.interpTokenInfo[i][k - 1].second == categoryType::IDENTIFIER && IP.interpTokenInfo[i][k + 1].second == categoryType::NUMERIC_LITERAL) {
					IP.tlt.push_back(IP.interpTokenInfo[i][k - 1]);
					IP.tlt.push_back(IP.interpTokenInfo[i][k]);
					IP.tlt.push_back(IP.interpTokenInfo[i][k + 1]);
					EE.infixToPostfix(ITF, LA, EE, IP);
					EE.newVar(IP.interpTokenInfo[i][k - 1].first, IP.result);
					k++;
				}
			}
		}

	}
}

void Interpreter::clear(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {
	IP.interpExpression.clear();
	IP.tlt.clear();
	IP.result.clear();
	IP.interpTokenInfo.clear();
}