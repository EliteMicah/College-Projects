#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"
using namespace std;


Interface::Interface(){
}

void Interface::startInterface() {

    Interface ITF;
    lexicalAnalyzer LA;
    expEvaluator EE;
    Interpreter IP;


    cout << "PySUB Interpreter 1.0 on Windows (September 2022)" << endl;
    cout << "Enter program lines or read (<filename>.py) at command line interface" << endl;
    cout << "Type 'help' for more information or 'quit' to exit" << endl;

    userInput(ITF, LA, EE, IP);

}

void Interface::userInput(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

p: // Waypoint

    ITF.exp = false;
    ITF.interp = false;

    string s = "";

    // Getting Input

    if (ITF.intMode && !ITF.indentMode)
        cout << ">>> ";
    else if (ITF.indentMode)
        cout << "...\t";
    else
        cout << endl << ">>> ";

    getline(cin, s);
    int stringsize = s.length();

    //  QUIT
    if (s == "quit" || s == "quit()")
        exit(0);

    // EXPRESSION EVALUATOR starting with DIGIT or ' ( '
    else if (s.length() >= 2 && ((isdigit(s[0]) || s[0] == '(') || (s[0] == 'n' && s[1] == 'o' && s[2] == 't'))) {
        ITF.programCode.push_back(s);
        ITF.exp = true;
        LA.getTokens(ITF, LA, EE, IP);
        ITF.programCode.clear();
        goto p;
    }

    //  HELP & HELP(_)
    else if (s == "help" || s == "help()")
        userHelp(ITF, LA, EE, IP);
    else if (s[0] == 'h' && s[1] == 'e' && s[2] == 'l' && s[3] == 'p' && s[4] == '(' && s[stringsize - 1] == ')') {
        string t;
        for (int i = 5; i < s.length() - 1; i++)
            t.push_back(s[i]);

        // Gives info for each of the commands
        if (t == "quit") {
            cout << "Quits the program" << endl;
            goto p;
        }
        else if (t == "show") {
            cout << "Shows the lines of the program that are stored in program data structure" << endl;
            goto p;
        }
        else if (t == "clear") {
            cout << "Clears out or deletes any lines that are stored in the program data structure" << endl;
            goto p;
        }
        else if (t == "run") {
            cout << "Runs the code saved in the user's read file" << endl;
            goto p;
        }
        else if (t == "read") {
            cout << "Reads a file and inserts each line into a vector of strings" << endl;
            goto p;
        }
        else if (t == "find") {
            cout << "User may enter an operand to find the Precidence of" << endl;
            goto p;
        }
        else {
            cout << "Please enter a proper command" << endl;
            goto p;
        }
    }


    //  READ
    //          If ' read ' with nothing else typed
    else if (s[0] == 'r' && s[1] == 'e' && s[2] == 'a' && s[3] == 'd' && stringsize < 5) {
        cout << "Please enter a file to 'read' from" << endl;
        goto p;
    }
    //          If ' read text.py ' without parenthesis
    else if (s[0] == 'r' && s[1] == 'e' && s[2] == 'a' && s[3] == 'd' && stringsize > 5 && s[stringsize - 1] != ')') {
        for (int i = 5; i < stringsize; i++)
            filename.push_back(s[i]);
        userRead(ITF, LA, EE, IP);
        goto p;
    }
    //          If ' read(text.py) ' with parenthesis
    else if (s[0] == 'r' && s[1] == 'e' && s[2] == 'a' && s[3] == 'd' && s[4] == '(' && s[stringsize - 1] == ')') {
        filename = "";
        for (int i = 5; i < stringsize - 1; i++)
            filename.push_back(s[i]);
        userRead(ITF, LA, EE, IP);
        goto p;
    }


    //  SHOW 
    else if (s == "show" || s == "show()") {
        if (ITF.programCode.empty() == true) {
            cout << "Please use the 'read' command first or enter the file name" << endl;
            goto p;
        }
        else {
            userShow(ITF, LA, EE, IP);
            goto p;
        }
    }

    // If ' show(tokens) ' with parenthesis
    else if (s == "show(tokens)") {
        LA.getTokens(ITF, LA, EE, IP);
        goto p;
    }

    // If ' show(variables) ' with parenthesis
    else if (s == "show(variables)" || s == "show(var)") {
        EE.showVar(ITF, LA, EE, IP);
        goto p;
    }

    // If ' show tokens ' without parenthesis
    else if (s[0] == 's' && s[1] == 'h' && s[2] == 'o' && s[3] == 'w' && stringsize > 5 && s[stringsize - 1] != ')') {
        filename = "";
        for (int i = 5; i < stringsize; i++)
            filename.push_back(s[i]);
        if (filename == "tokens") {
            LA.getTokens(ITF, LA, EE, IP);
        }
        else if (filename == "variables" || filename == "var") {
            if (EE.symbolTable.empty()) {
                cout << "Symboltable is empty" << endl;
                cout << "Please use the read() first" << endl;
                goto p;
            }
            EE.showVar(ITF, LA, EE, IP);
            goto p;
        }
        else
            cout << "Please enter 'tokens' or another command" << endl;
        goto p;
    }

    // RUN
    else if (s == "run" || s == "run()") {
        ITF.interp = true;
        if (ITF.programCode.empty())
            cout << "Please use the read() command first" << endl;
        else
            LA.getTokens(ITF, LA, EE, IP);
        goto p;
    }

    //  CLEAR
    else if (s == "clear" || s == "clear()") {
        userClear(ITF, LA, EE, IP);
        cout << "Cleared" << endl;
        goto p;
    }

    //  FIND
    else if (s == "find" || s == "find()") {
        cout << "Please enter ' find precidence ' or ' find(precidence) '" << endl;
        goto p;
    }

    //              If ' find % ' without parenthesis
    else if (s[0] == 'f' && s[1] == 'i' && s[2] == 'n' && s[3] == 'd' && stringsize > 5 && s[stringsize - 1] != ')') {
        precidence = "";
        for (int i = 5; i < stringsize; i++)
            precidence.push_back(s[i]);
            if (precidence == "*" || precidence == "/" || precidence == "%" || precidence == "+" || 
                precidence == "-" || precidence == "<" || precidence == "<="
                || precidence == ">" || precidence == ">=" || precidence == "!=" 
                || precidence == "==" || precidence == "not" || precidence == "and" 
                || precidence == "or")
                findPrec(EE);
            else
                cout << "Please enter a proper precidence or another command" << endl;
            goto p;
    }

    //              If ' find(precidence) ' with parenthesis
    else if (s[0] == 'f' && s[1] == 'i' && s[2] == 'n' && s[3] == 'd' && s[4] == '(' && s[stringsize - 1] == ')') {
        precidence = "";
        for (int i = 5; i < stringsize - 1; i++)
            precidence.push_back(s[i]);
        findPrec(EE);
        goto p;
    }

    //    INTERACTIVE MODE  ' print() '
    else if (s[0] == 'p' && s[1] == 'r' && s[2] == 'i' && s[3] == 'n' && s[4] == 't' && stringsize > 5 && s[5] == '(' && s[stringsize - 1] == ')') {
        if (!ITF.indentMode) {
            ITF.programCode.clear();
            ITF.programCode.push_back(s);
        }
        else {
            ITF.programCode.push_back(s);
            goto p;
        }
        interactiveMode(ITF, LA, EE, IP);
        goto p;
    }

    //    INTERACTIVE MODE  ' while ' Statement
    else if (s[0] == 'w' && s[1] == 'h' && s[2] == 'i' && s[3] == 'l' && s[4] == 'e' && stringsize > 5) {
        ITF.programCode.clear();
        ITF.programCode.push_back(s);
        ITF.indentMode = true;
        goto p;
    }

    //    INTERACTIVE MODE  ' if ' Statement
    else if (s[0] == 'i' && s[1] == 'f' && stringsize > 2) {
        ITF.programCode.clear();
        ITF.programCode.push_back(s);
        ITF.indentMode = true;
        goto p;
    }

    //    INTERACTIVE MODE    Run program if nothing is typed in indentMode
    else if (ITF.indentMode && stringsize <= 0) {
        interactiveMode(ITF, LA, EE, IP);
    }


    //  ELSE
    //      Testing for Interactive Mode
    //      If not then error
    else {
        for (int i = 0; i < stringsize; i++) {
            if (s[i] == '=') {
                if (ITF.indentMode) {
                    ITF.programCode.push_back(s);
                    goto p;
                }
                else {
                    ITF.programCode.clear();
                    ITF.programCode.push_back(s);
                    interactiveMode(ITF, LA, EE, IP);
                    goto p;
                }
            }
            else if (s[i] == '#') {
                if (ITF.indentMode) {
                    ITF.programCode.push_back(s);
                    goto p;
                }
                else {
                    ITF.programCode.clear();
                    ITF.programCode.push_back(s);
                    interactiveMode(ITF, LA, EE, IP);
                    goto p;
                }
            }
        }
    }

    if (!ITF.intMode && !ITF.indentMode) {
        cout << "Please enter a proper command" << endl;
        goto p;
    }

    ITF.intMode = false;
    ITF.indentMode = false;
    goto p;
}

void Interface::userHelp(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

    // Enters and displays the help interface
    cout << endl << "Welcome to the help utility!" << endl;
    cout << "* To exit and return to the interpreter, type 'exit'" << endl;
    cout << "* To get a list of commands and their description, type 'commands' at the help> prompt" << endl << endl;

p:
    string v = "";
    cout << "help> ";


    // User typed 'commands'
    getline(cin, v);

    // Gets help for each of the commands
    if (v == "commands") {
        cout << "Below is a list of commands. Enter any command at the prompt to get more help" << endl << endl;
        cout << "clear" << setw(7) << "help" << setw(7) << "quit" << endl;
        cout << "read" << setw(8) << "show" << setw(7) << "find" << endl << endl;
        cout << "Note: ALL commands can be entered as functions: " << endl << endl;
        cout << "clear()" << setw(9) << "help()" << setw(9) << "quit()" << endl;
        cout << "read()" << setw(10) << "show()" << setw(9) << "find()" << endl << endl;
        goto p;
    }
    else if (v == "quit" || v == "quit()") {
        cout << "Quits the program" << endl << endl;
        goto p;
    }
    else if (v == "help" || v == "help()") {
        cout << "Gives 'help' to the user" << endl << endl;
        goto p;
    }
    else if (v == "show" || v == "show()") {
        cout << "Shows the lines of the program that are stored in program data structure" << endl << endl;
        goto p;
    }
    else if (v == "clear" || v == "clear()") {
        cout << "Clears out or deletes any lines that are stored in the program data structure" << endl << endl;
        goto p;
    }
    else if (v == "read" || v == "read()") {
        cout << "Reads a file and inserts each line into a vector of strings" << endl << endl;
        goto p;
    }
    else if (v == "find" || v == "find()") {
        cout << "User may enter an operand to find the Precidence of" << endl;
        goto p;
    }
    else if (v == "run" || v == "run()") {
        cout << "Runs the code saved in the user's read file" << endl;
        goto p;
    }
    //          Returns back to main interface
    else if (v == "exit") {
        cout << "Returned back to main interface" << endl;
        userInput(ITF, LA, EE, IP);
    }

    else {
        cout << "Please type a proper command" << endl;
        goto p;
    }
}

void Interface::userRead(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

    userClear(ITF, LA, EE, IP);
    ITF.intMode = false;
    ITF.indentMode = false;

    // Opening the file and checking if open
    ifstream ifile(filename);
    if (!ifile) {
        cout << "Error opening file..." << endl;
        userInput(ITF, LA, EE, IP);
    }

    string s = "";

    // Putting each line into 's'
    while (getline(ifile, s)) {
        ITF.programCode.push_back(s);
        if (!ifile)
            break;
    }
    cout << "Read complete" << endl;

    ifile.close();
}

void Interface::userShow(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

    // Printing each line from the vector of strings saved in 'read()'
    for (int i = 0; i < ITF.programCode.size(); i++) {
        cout << "[" << i << "] " << ITF.programCode[i] << endl;
    }
}

void Interface::findPrec(expEvaluator& EE) {

    cout << "Precidence level of '" << precidence << "' is: " << EE.getPrecidence(precidence) << endl;
}

void Interface::userClear(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {
    ITF.programCode.clear();
    EE.expression.clear();
    EE.symbolTable.clear();
    IP.clear(ITF, LA, EE, IP);
    LA.tkClear();
    EE.clear();
}

void Interface::interactiveMode(Interface& ITF, lexicalAnalyzer& LA, expEvaluator& EE, Interpreter& IP) {

    ITF.intMode = true;
    ITF.interp = true;
    LA.getTokens(ITF, LA, EE, IP);

}
