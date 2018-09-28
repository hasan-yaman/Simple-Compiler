#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stack>
#include <unordered_map>

using namespace std;

// check whether given string has a "pow" or not
bool containsPowKeyword(string s1) {
    if (s1.find("pow") != std::string::npos) {
        return true;
    }
    return false;
}

// return index of "pow"
int indexOfPowKeyword(string s1) {
    return s1.find("pow");
}

// process statements with "pow" in a recursive way, and return string with $
string processPowKeyword(string line) {
    while (containsPowKeyword(line)) {
        int startIndex = indexOfPowKeyword(line);
        int finishIndex = startIndex;
        int indexComma = startIndex;

        stack<char> myStack;

        for (int i = startIndex + 3; i < line.size(); i++) {

            if (line[i] == '(') {
                myStack.push('(');
            } else if (line[i] == ')') {
                myStack.pop();
            } else if (line[i] == ',') {
                if (myStack.size() == 1) {
                    indexComma = i;
                }
            }

            if (i != startIndex && myStack.empty()) {
                finishIndex = i;
                break;
            }
        }

        // recursive part
        string leftPart = line.substr(startIndex + 4, indexComma - startIndex - 4);
        string rightPart = line.substr(indexComma + 1, finishIndex - indexComma - 1);

        leftPart = processPowKeyword(leftPart);
        rightPart = processPowKeyword(rightPart);

        line = line.substr(0, startIndex) + "(" + leftPart + ")" + " $ " + "(" + rightPart + ")"
               + line.substr(finishIndex + 1);
    }

    return line;

}

// print syntax error to output file
void printErrorInOutputFile(ofstream &outputFile, int lineCounter);

// print addition process to output file
void printAdditionInOutputFile(ofstream& outputFile);

// print multiplication process to output file
void printMultiplicationInOutputFile(ofstream & outputFile, int multiplicationCounter);

// print power process to output file
void printPowInOutputFile(ofstream & outputFile, int multiplicationCounter, int powerCounter);

/// print assembly print process to output file
void printAssemblyPrintInOutputFile(ofstream & outputFile, int printCounter);

// print giben variable printing process in assembly to output file
void printAssemblyPrintWithVariableNameInOutputFile(ofstream & outputFile, int printCounter,string currentVariableName);

// Function to convert Infix expression to postfix
string InfixToPostfix(string expression);

// Function to verify whether an operator has higher precedence over other
int HasHigherPrecedence(char operator1, char operator2);

// Function to verify whether a character is operator symbol or not.
bool IsOperator(char C);

// Function to verify whether a character is alphanumeric chanaracter (letter or numeric digit) or not.
bool IsOperand(char C);

// check whether given string have valid paranthesis or not
bool isValid(std::string s) {
    std::vector<char> stk;
    for (char c : s) {
        switch (c) {
            case '(':
                stk.push_back(c);
                break;
            case ')':
                if (stk.empty() || stk.back() != '(')
                    return false;
                stk.pop_back();
                break;
        }
    }
    return stk.empty();
}

// trim function for delete spaces
string trim(string str) {
    std::string word;
    std::stringstream stream(str);
    stream >> word;
    return word;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        // if program is executed with zero or more than 2 file, give an error!
        cout << "Give input file for compiler!" << endl;
        return 1;
    }
    // open output file
    ofstream outputFile;
    outputFile.open("example.asm");
    // initialization of assembly variables.
    outputFile << "code segment" << endl;
    outputFile << "tempVar dd 0" << endl;

    outputFile << "addVar1 dd 0" << endl;
    outputFile << "addVar2 dd 0" << endl;
    outputFile << "addVarRes dd 0" << endl;

    outputFile << "multVar1 dd 0" << endl;
    outputFile << "multVar2 dd 0" << endl;
    outputFile << "multVarRes dd 0" << endl;

    outputFile << "printVariable dd 0" << endl;

    outputFile << "powerVar1 dd 0" << endl;
    outputFile << "powerVar2 dd 0" << endl;
    outputFile << "powerResult3 dd 1" << endl;

    // store variables - index pairs
    unordered_map<string, int> variablesMap;

    // initialization of counters
    int variablesCounter = 0;
    int multiplicationCounter = 0;
    int printCounter = 0;
    int lineCounter = 0;
    int powerCounter = 0;

    // open input file
    ifstream inputFile(argv[1]);
    string line;
    string assigmentSign = "=";
    // get line from input file
    while (getline(inputFile, line)) {
        lineCounter++;

        if (!line.empty()) {
            if (isValid(line)) {
                // if line is valid in terms of paranthesis and not empty

                int index = line.find(assigmentSign);
                if (index == -1) {
                    // not found, statement is print statement

                    string currentVariableName = line;

                    auto mapIte = variablesMap.find(currentVariableName);
                    // check whether we have this variable in the map or not
                    if (mapIte == variablesMap.end()) {

                        line = processPowKeyword(line);

                        string postfixString = InfixToPostfix(line);

                       // cout << "postfix hali: " << postfixString << endl;

                        string buf; // Have a buffer string
                        stringstream ss(postfixString); // Insert the string into a stream

                        vector<string> tokens; // Create vector to hold our words

                        // split by space
                        while (ss >> buf) {
                            tokens.push_back(buf);
                        }

                        //store postfix notation of current statement
                        vector<string> postfixVec;

                        // get postfix vector by processing postfix string
                        for (int i = 0; i < tokens.size(); i++) {

                            int index1 = tokens[i].find("+");
                            int index2 = tokens[i].find("*");
                            int index3 = tokens[i].find("$");

                            if (index1 == -1 && index2 == -1 && index3 == -1) {
                                postfixVec.push_back(tokens[i]);
                            } else {

                                int smallestIndex = INT32_MAX;

                                vector<string> tempOperators;

                                for (int j = 0; j < tokens[i].size(); j++) {
                                    if (tokens[i][j] == '+') {
                                        tempOperators.push_back("+");
                                        if (j < smallestIndex) {
                                            smallestIndex = j;
                                        }
                                    } else if (tokens[i][j] == '*') {
                                        tempOperators.push_back("*");
                                        if (j < smallestIndex) {
                                            smallestIndex = j;
                                        }
                                    } else if (tokens[i][j] == '$') {
                                        tempOperators.push_back("$");
                                        if (j < smallestIndex) {
                                            smallestIndex = j;
                                        }
                                    }
                                }


                                postfixVec.push_back(tokens[i].substr(0, smallestIndex));
                                for (int j = 0; j < tempOperators.size(); j++) {
                                    postfixVec.push_back(tempOperators[j]);
                                }
                            }
                        }

                      /*  cout << "Postfix notation of expression: " << endl;
                        for (int i = 0; i < postfixVec.size(); i++) {
                            cout << postfixVec[i] << endl;
                        }

                        cout << endl; */
                        // stack to store postfix notation of statements and to detect syntax errors
                        stack<string> stck;

                        for (int i = 0; i < postfixVec.size(); i++) {
                            if (postfixVec[i] == "+") {
                                // if current operator is plus operator, pop last two element from
                                // stack and make addition, if the stack empty it means we have
                                // synax error, so we should check whether stack is empty or not!
                                if (!stck.empty()) {
                                    stck.pop();
                                } else {
                                    printErrorInOutputFile(outputFile, lineCounter);
                                    return 0;
                                }
                                if (!stck.empty()) {
                                    stck.pop();
                                } else {
                                    printErrorInOutputFile(outputFile, lineCounter);
                                    return 0;
                                }
                                // print addition process to output file and push element to stack as a result
                                // of addition
                                printAdditionInOutputFile(outputFile);
                                stck.push("ADD RESULT");

                            } else if (postfixVec[i] == "*") {
                                // if current operator is multiplication operator, pop last two element from
                                // stack and make multiplication, if the stack empty it means we have
                                // synax error, so we should check whether stack is empty or not!

                                if (!stck.empty()) {
                                    stck.pop();
                                } else {
                                    printErrorInOutputFile(outputFile, lineCounter);
                                    return 0;
                                }
                                if (!stck.empty()) {
                                    stck.pop();
                                } else {
                                    printErrorInOutputFile(outputFile, lineCounter);
                                    return 0;
                                }
                                // print multiplication process to output file and push element to stack as a result
                                // of multiplication
                                printMultiplicationInOutputFile(outputFile, multiplicationCounter);
                                multiplicationCounter++;
                                stck.push("MULT RESULT");

                            } else if (postfixVec[i] == "$") {
                                // if current operator is power operator, pop last two element from
                                // stack and make power operation, if the stack empty it means we have
                                // synax error, so we should check whether stack is empty or not!

                                if (!stck.empty()) {
                                    stck.pop();
                                } else {
                                    printErrorInOutputFile(outputFile, lineCounter);
                                    return 0;
                                }
                                if (!stck.empty()) {
                                    stck.pop();
                                } else {
                                    printErrorInOutputFile(outputFile, lineCounter);
                                    return 0;
                                }
                                // print power process to output file and push element to stack as a result
                                // of power
                                printPowInOutputFile(outputFile,multiplicationCounter,powerCounter);
                                multiplicationCounter++;
                                powerCounter++;
                                stck.push("POW RESULT");
                            } else {
                                if (!postfixVec[i].empty()) {
                                    // if the current element of vector is not operator, just push it to the stack
                                    // and create related variabe in assembly
                                    stck.push(postfixVec[i]);

                                    auto mapIte = variablesMap.find(postfixVec[i]);
                                    if (mapIte == variablesMap.end()) {

                                        string left;
                                        string right;

                                        if (postfixVec[i].size() > 4) {

                                            int cutIndex = postfixVec[i].size() - 4;
                                            right = "0" + postfixVec[i].substr(cutIndex, 4) + "h";
                                            left = "0" + postfixVec[i].substr(0, cutIndex) + "h";

                                        } else {
                                            left = "0h";
                                            right = "0" + postfixVec[i] + "h";
                                        }

                                        outputFile << "push " << right << endl;
                                        outputFile << "push " << left << endl;
                                    } else {
                                        int currentVariableIndex = variablesMap.at(postfixVec[i]);

                                        outputFile << "push word ptr " <<  postfixVec[i] << to_string(currentVariableIndex) << endl;
                                        outputFile << "push word ptr " <<  postfixVec[i] << to_string(currentVariableIndex) << " + 2" << endl;
                                    }
                                }
                            }
                        }

                        if (stck.size() != 1) {
                            // if the stack size is not 1 (because right hand side of
                            // assigment should evaluate to one thing) it means we have syntax error
                            printErrorInOutputFile(outputFile, lineCounter);
                            return 0;
                        }

                        // print in assembly

                        printAssemblyPrintInOutputFile(outputFile,printCounter);
                        printCounter++;
                    } else {
                        // print current variable in assembly
                        int currentVariableNameIndex = variablesMap.at(currentVariableName);
                        currentVariableName += to_string(currentVariableNameIndex);

                        printAssemblyPrintWithVariableNameInOutputFile(outputFile,printCounter,currentVariableName);
                        printCounter++;
                    }
                } else {
                    string currentVariableName = trim(line.substr(0, index));

                    auto mapItera = variablesMap.find(currentVariableName);

                    if (mapItera == variablesMap.end()) {
                        variablesMap.insert(pair<string, int>(currentVariableName, variablesCounter));
                        currentVariableName += to_string(variablesCounter);
                        variablesCounter++;
                        outputFile << currentVariableName << " dd 0" << endl;
                    } else {
                        currentVariableName += to_string(variablesMap.at(currentVariableName));
                    }

                    outputFile << "push offset " << currentVariableName << endl;

                    line = line.substr(index);
                    // process pow
                    line = processPowKeyword(line);

                  //  cout << "line: " << line << endl;

                    string postfixString = InfixToPostfix(line);

                 //   cout << "postfix hali: " << postfixString << endl;

                    string buf; // Have a buffer string
                    stringstream ss(postfixString); // Insert the string into a stream

                    vector<string> tokens; // Create vector to hold our words

                    while (ss >> buf) {
                        tokens.push_back(buf);
                    }

                    vector<string> postfixVec;

                    for (int i = 0; i < tokens.size(); i++) {

                        int index1 = tokens[i].find("+");
                        int index2 = tokens[i].find("*");
                        int index3 = tokens[i].find("$");

                        if (index1 == -1 && index2 == -1 && index3 == -1) {
                            postfixVec.push_back(tokens[i]);
                        } else {

                            int smallestIndex = INT32_MAX;

                            vector<string> tempOperators;

                            for (int j = 0; j < tokens[i].size(); j++) {
                                if (tokens[i][j] == '+') {
                                    tempOperators.push_back("+");
                                    if (j < smallestIndex) {
                                        smallestIndex = j;
                                    }
                                } else if (tokens[i][j] == '*') {
                                    tempOperators.push_back("*");
                                    if (j < smallestIndex) {
                                        smallestIndex = j;
                                    }
                                } else if (tokens[i][j] == '$') {
                                    tempOperators.push_back("$");
                                    if (j < smallestIndex) {
                                        smallestIndex = j;
                                    }
                                }
                            }
                            postfixVec.push_back(tokens[i].substr(0, smallestIndex));
                            for (int j = 0; j < tempOperators.size(); j++) {
                                postfixVec.push_back(tempOperators[j]);
                            }
                        }
                    }

                  /*  cout << "Postfix notation of expression: " << endl;
                    for (int i = 0; i < postfixVec.size(); i++) {
                        cout << "-" << postfixVec[i] << "-" << endl;
                    }

                    cout << endl; */

                    stack<string> stck;

                    for (int i = 0; i < postfixVec.size(); i++) {

                        if (postfixVec[i] == "+") {
                            // if current operator is plus operator, pop last two element from
                            // stack and make addition, if the stack empty it means we have
                            // synax error, so we should check whether stack is empty or not!
                            if (!stck.empty()) {
                                stck.pop();
                            } else {
                                printErrorInOutputFile(outputFile, lineCounter);
                                return 0;
                            }
                            if (!stck.empty()) {
                                stck.pop();
                            } else {
                                printErrorInOutputFile(outputFile, lineCounter);
                                return 0;
                            }

                            printAdditionInOutputFile(outputFile);
                            stck.push("ADD RESULT");

                        } else if (postfixVec[i] == "*") {
                            // if current operator is multiplication operator, pop last two element from
                            // stack and make multiplication, if the stack empty it means we have
                            // synax error, so we should check whether stack is empty or not!
                            if (!stck.empty()) {
                                stck.pop();
                            } else {
                                printErrorInOutputFile(outputFile, lineCounter);
                                return 0;
                            }
                            if (!stck.empty()) {
                                stck.pop();
                            } else {
                                printErrorInOutputFile(outputFile, lineCounter);
                                return 0;
                            }

                            printMultiplicationInOutputFile(outputFile, multiplicationCounter);
                            multiplicationCounter++;
                            stck.push("MULT RESULT");

                        } else if (postfixVec[i] == "$") {
                            // if current operator is power operator, pop last two element from
                            // stack and make power operation, if the stack empty it means we have
                            // synax error, so we should check whether stack is empty or not!

                            if (!stck.empty()) {
                                stck.pop();
                            } else {
                                printErrorInOutputFile(outputFile, lineCounter);
                                return 0;
                            }
                            if (!stck.empty()) {
                                stck.pop();
                            } else {
                                printErrorInOutputFile(outputFile, lineCounter);
                                return 0;
                            }

                            printPowInOutputFile(outputFile,multiplicationCounter,powerCounter);
                            multiplicationCounter++;
                            powerCounter++;
                            stck.push("POW RESULT");

                        } else {

                            if (!postfixVec[i].empty()) {
                                // create variable in assembly language then push it to the stack
                                stck.push(postfixVec[i]);

                                auto iteMap = variablesMap.find(postfixVec[i]);

                                if (iteMap == variablesMap.end()) {
                                    string left;
                                    string right;

                                    if (postfixVec[i].size() > 4) {
                                        int cutIndex = postfixVec[i].size() - 4;
                                        right = "0" + postfixVec[i].substr(cutIndex, 4) + "h";
                                        left = "0" + postfixVec[i].substr(0, cutIndex) + "h";

                                    } else {
                                        left = "0h";
                                        right = "0" + postfixVec[i] + "h";
                                    }

                                    outputFile << "push " << right << endl;
                                    outputFile << "push " << left << endl;

                                } else {
                                    // initialize variable in assembly language
                                    int currentVariableIndex = variablesMap.at(postfixVec[i]);

                                    outputFile << "push word ptr " << postfixVec[i] << to_string(currentVariableIndex)
                                               << endl;
                                    outputFile << "push word ptr " << postfixVec[i] << to_string(currentVariableIndex)
                                               << " + 2" << endl;
                                }
                            }
                        }
                    }

                    if (stck.size() != 1) {
                        printErrorInOutputFile(outputFile, lineCounter);
                        return 0;
                    }

                    outputFile << "pop word ptr tempVar + 2" << endl;
                    outputFile << "pop word ptr tempVar" << endl;
                    outputFile << "pop bx" << endl;
                    outputFile << "mov [bx],word ptr tempVar" << endl;
                    outputFile << "mov [bx + 2],word ptr tempVar + 2" << endl;
                    outputFile << "xor bx,bx" << endl; // clean bx register
                }
            } else {
                printErrorInOutputFile(outputFile, lineCounter);
                return 0;
            }
        }
    }

    cout << "example.asm was generated." << endl;
    outputFile << "int 20h" << endl;
    outputFile << "code ends" << endl;
    outputFile.close();
    return 0;
}

string InfixToPostfix(string expression) {
    // Declaring a Stack from Standard template library in C++.
    stack<char> S;
    string postfix = ""; // Initialize postfix as empty string.
    for (int i = 0; i < expression.length(); i++) {

        //cout << "expression[i]: " << expression[i] << endl;

        if (expression[i] == ' ') {
            postfix += " ";
        }

// Scanning each character from left.
        // If character is a delimitter, move on.
        if (expression[i] == ' ' || expression[i] == ',') continue;

            // If is operator, pop two elements from stack, perform operation and push the result back.
        else if (IsOperator(expression[i])) {
            while (!S.empty() && S.top() != '(' && HasHigherPrecedence(S.top(), expression[i])) {
                postfix += S.top();
                S.pop();
            }
            postfix += " ";
            S.push(expression[i]);
        }
            // Else if character is an operand
        else if (IsOperand(expression[i])) {
            postfix += expression[i];
        } else if (expression[i] == '(') {
            S.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!S.empty() && S.top() != '(') {
                postfix += S.top();
                S.pop();
            }
            S.pop();
        }
    }

    while (!S.empty()) {
        postfix += S.top();
        S.pop();
    }
    return postfix;
}

// Function to verify whether a character is english letter or numeric digit.
bool IsOperand(char C) {
    if (C >= '0' && C <= '9') return true;
    if (C >= 'a' && C <= 'z') return true;
    if (C >= 'A' && C <= 'Z') return true;
    return false;
}

// Function to verify whether a character is operator symbol or not.
bool IsOperator(char C) {
    if (C == '+' || C == '-' || C == '*' || C == '/' || C == '$') {
        return true;
    }
    return false;
}

// Function to verify whether an operator is right associative or not.
int IsRightAssociative(char op) {
    if (op == '$') return true;
    return false;
}

// Function to get weight of an operator. An operator with higher weight will have higher precedence.
int GetOperatorWeight(char op) {

    int weight = -1;
    switch (op) {
        case '+':
            weight = 1;
            break;
        case '*':
            weight = 2;
            break;
        case '$':
            weight = 3;
            break;
    }
    return weight;
}

// Function to perform an operation and return output.
int HasHigherPrecedence(char op1, char op2) {
    int op1Weight = GetOperatorWeight(op1);
    int op2Weight = GetOperatorWeight(op2);

    // If operators have equal precedence, return true if they are left associative.
    // return false, if right associative.
    // if operator is left-associative, left one should be given priority.
    if (op1Weight == op2Weight) {
        if (IsRightAssociative(op1)) return false;
        else return true;
    }
    return op1Weight > op2Weight ? true : false;
}

void printErrorInOutputFile(ofstream &outputFile, int lineCounter) {
    outputFile.close();

    outputFile.open("example.asm");

    outputFile << "code segment" << endl;
    outputFile << "jmp start" << endl;
    string errorline = "Line " + to_string(lineCounter) + ": Syntax error.$";
    outputFile << "errorTextVariable db \"" << errorline << "\"" << endl;
    outputFile << "start: " << endl;
    outputFile << "mov ah,09" << endl;
    outputFile << "mov dx,offset errorTextVariable" << endl;
    outputFile << "int 21h" << endl;

    outputFile << "int 20h" << endl;  // TODO burası eklenecek
    outputFile << "code ends" << endl;

    outputFile.close();
}

void printAdditionInOutputFile(ofstream & outputFile) {
    outputFile << "pop word ptr addVar1 + 2" << endl;
    outputFile << "pop word ptr addVar1" << endl;

    outputFile << "pop word ptr addVar2 + 2" << endl;
    outputFile << "pop word ptr addVar2" << endl;

    outputFile << "mov dx, word ptr addVar1" << endl;
    outputFile << "add dx, word ptr addVar2" << endl;
    outputFile << "mov word ptr addVarRes, dx" << endl;

    outputFile << "mov dx, word ptr addVar1 + 2" << endl;
    outputFile << "adc dx, word ptr addVar2 + 2" << endl;
    outputFile << "mov word ptr addVarRes + 2, dx" << endl;

    outputFile << "push word ptr addVarRes" << endl;
    outputFile << "push word ptr addVarRes + 2" << endl;
}

void printMultiplicationInOutputFile(ofstream & outputFile, int multiplicationCounter) {
    outputFile << "pop word ptr multVar2 + 2" << endl;
    outputFile << "pop word ptr multVar2" << endl;

    outputFile << "pop word ptr multVar1 + 2" << endl;
    outputFile << "pop word ptr multVar1" << endl;

    outputFile << "mov SI, word[multVar1]" << endl;
    outputFile << "mov DI, word[multVar1 + 2]" << endl;
    outputFile << "xor ax,ax" << endl;
    outputFile << "xor bx,bx" << endl;
    outputFile << "xor cx,cx" << endl;
    outputFile << "xor dx,dx" << endl;
    outputFile << "mov BP, 32" << endl;

    string labelName1 = "myloop" + to_string(multiplicationCounter);
    string labelFullName1 = labelName1 + ":";

    string labelName2 = "cont" + to_string(multiplicationCounter);
    string labelFullName2 = labelName2 + ":";

    outputFile << labelFullName1 << endl;
    outputFile << "test SI, 1" << endl;
    outputFile << "jz short " << labelName2 << endl;
    outputFile << "add cx, word[multVar2]" << endl;
    outputFile << "add dx, word[multVar2 + 2]" << endl;

    outputFile << labelFullName2 << endl;
    outputFile << "rcr dx, 1" << endl;
    outputFile << "rcr cx, 1" << endl;
    outputFile << "rcr bx, 1" << endl;
    outputFile << "rcr ax, 1" << endl;
    outputFile << "rcr DI, 1" << endl;
    outputFile << "rcr SI, 1" << endl;
    outputFile << "dec BP" << endl;
    outputFile << "jnz short " << labelName1 << endl;

    outputFile << "mov word ptr multVarRes, ax" << endl;
    outputFile << "mov word ptr multVarRes + 2, bx" << endl;

    outputFile << "push word ptr multVarRes" << endl;
    outputFile << "push word ptr multVarRes + 2" << endl;
}

void printPowInOutputFile(ofstream & outputFile, int multiplicationCounter, int powerCounter) {
    outputFile << "pop word ptr powerVar2 + 2" << endl;
    outputFile << "pop word ptr powerVar2" << endl;

    outputFile << "pop word ptr powerVar1 + 2" << endl;
    outputFile << "pop word ptr powerVar1" << endl;

    string labelName1 = "power" + to_string(powerCounter);
    string labelFullName1 = labelName1 + ":";

    string labelName2 = "power_done" + to_string(powerCounter);
    string labelFullName2 = labelName2 + ":";

    outputFile << labelFullName1 << endl;
    outputFile << "cmp powerVar2, 0" << endl;
    outputFile << "je " << labelName2 << endl;
    outputFile << "mov SI, word[powerVar1]" << endl;
    outputFile << "mov DI, word[powerVar1 + 2]" << endl;
    outputFile << "xor ax, ax" << endl;
    outputFile << "xor bx, bx" << endl;
    outputFile << "xor cx, cx" << endl;
    outputFile << "xor dx, dx" << endl;
    outputFile << "mov BP,32" << endl;

    string labelProductName1 = "myloop" + to_string(multiplicationCounter);
    string labelProductFullName1 = labelProductName1 + ":";

    string labelProductName2 = "cont" + to_string(multiplicationCounter);
    string labelProductFullName2 = labelProductName2 + ":";

    outputFile << labelProductFullName1 << endl;
    outputFile << "test SI, 1" << endl;
    outputFile << "jz short " << labelProductName2 << endl;
    outputFile << "add cx, word[powerResult3]" << endl;
    outputFile << "add dx, word[powerResult3 + 2]" << endl;

    outputFile << labelProductFullName2 << endl;
    outputFile << "rcr dx, 1" << endl;
    outputFile << "rcr cx, 1" << endl;
    outputFile << "rcr bx, 1" << endl;
    outputFile << "rcr ax, 1" << endl;
    outputFile << "rcr DI, 1" << endl;
    outputFile << "rcr SI, 1" << endl;
    outputFile << "dec BP" << endl;
    outputFile << "jnz short " << labelProductName1 << endl;

    outputFile << "mov word ptr powerResult3, ax" << endl;
    outputFile << "mov word ptr powerResult3 + 2, bx" << endl;

    outputFile << "dec word ptr powerVar2" << endl;
    outputFile << "loop " << labelName1 << endl;

    outputFile << labelFullName2 << endl;
    outputFile << "push word ptr powerResult3" << endl;
    outputFile << "push word ptr powerResult3 + 2" << endl;
}

void printAssemblyPrintInOutputFile(ofstream & outputFile, int printCounter) {
    outputFile << "pop word ptr printVariable + 2" << endl;
    outputFile << "pop word ptr printVariable" << endl;

    string labelName0 = "loop1" + to_string(printCounter);
    string labelFullname0 = labelName0 + ":";

    string labelName1 = "hexdigit" + to_string(printCounter);
    string labelFullname1 = labelName1 + ":";

    string labelName2 = "output" + to_string(printCounter);
    string labelFullname2 = labelName2 + ":";

    string labelName3 = "loop2" + to_string(printCounter);
    string labelFullname3 = labelName3 + ":";

    string labelName4 = "hexdigit2" + to_string(printCounter);
    string labelFullname4 = labelName4 + ":";

    string labelName5 = "output2" + to_string(printCounter);
    string labelFullname5 = labelName5 + ":";


    outputFile << "\n"
                          "\tmov bx, word ptr printVariable + 2\n"
                          "\tmov  cx,4h\n"
                          "   \tmov  ah,2h\n"
                  + labelFullname0 + "\n"
                          "   \tmov  dx,0fh\n"
                          "   \trol  bx,4h\n"
                          "   \tand  dx,bx\n"
                          "   \tcmp  dl,0ah\n"
                          "   \tjae  " + labelName1 + "\n"
                          "   \tadd  dl,'0'\n"
                          "   \tjmp  " + labelName2 + "\n"
                  + labelFullname1 + "\n"
                          "   \tadd  dl,'A'\n"
                          "   \tsub  dl,0ah\n"
                  + labelFullname2 + "\n"
                          "   \tint  21h\n"
                          "   \tdec  cx\n"
                          "   \tjnz  " + labelName0 + "\n"
                          "\n"
                          "\tmov bx, word ptr printVariable \n"
                          "\tmov  cx,4h\n"
                          "   \tmov  ah,2h\n"
                  + labelFullname3 + "\n"
                          "   \tmov  dx,0fh\n"
                          "   \trol  bx,4h\n"
                          "   \tand  dx,bx\n"
                          "   \tcmp  dl,0ah\n"
                          "   \tjae  " + labelName4 + "\n"
                          "   \tadd  dl,'0'\n"
                          "   \tjmp  " + labelName5 + "\n"
                  + labelFullname4 + "\n"
                          "   \tadd  dl,'A'\n"
                          "   \tsub  dl,0ah\n"
                  + labelFullname5 + "\n"
                          "   \tint  21h\n"
                          "   \tdec  cx\n"
                          "   \tjnz  " + labelName3 << endl;

    // Print new line
    outputFile << "mov dl, 10" << endl;
    outputFile << "mov ah, 02h" << endl;
    outputFile << "int 21h" << endl;
    outputFile << "mov dl, 13" << endl;
    outputFile << "mov ah, 02h" << endl;
    outputFile << "int 21h" << endl;
}

void printAssemblyPrintWithVariableNameInOutputFile(ofstream & outputFile, int printCounter, string currentVariableName) {

    string labelName0 = "loop1" + to_string(printCounter);
    string labelFullname0 = labelName0 + ":";

    string labelName1 = "hexdigit" + to_string(printCounter);
    string labelFullname1 = labelName1 + ":";

    string labelName2 = "output" + to_string(printCounter);
    string labelFullname2 = labelName2 + ":";

    string labelName3 = "loop2" + to_string(printCounter);
    string labelFullname3 = labelName3 + ":";

    string labelName4 = "hexdigit2" + to_string(printCounter);
    string labelFullname4 = labelName4 + ":";

    string labelName5 = "output2" + to_string(printCounter);
    string labelFullname5 = labelName5 + ":";

    outputFile << "\n"
                          "\tmov bx, word ptr " + currentVariableName + " + 2\n"
                          "\tmov  cx,4h\n"
                          "   \tmov  ah,2h\n"
                  + labelFullname0 + "\n"
                          "   \tmov  dx,0fh\n"
                          "   \trol  bx,4h\n"
                          "   \tand  dx,bx\n"
                          "   \tcmp  dl,0ah\n"
                          "   \tjae  " + labelName1 + "\n"
                          "   \tadd  dl,'0'\n"
                          "   \tjmp  " + labelName2 + "\n"
                  + labelFullname1 + "\n"
                          "   \tadd  dl,'A'\n"
                          "   \tsub  dl,0ah\n"
                  + labelFullname2 + "\n"
                          "   \tint  21h\n"
                          "   \tdec  cx\n"
                          "   \tjnz  " + labelName0 + "\n"
                          "\n"
                          "\tmov bx, word ptr " + currentVariableName + "\n"
                          "\tmov  cx,4h\n"
                          "   \tmov  ah,2h\n"
                  + labelFullname3 + "\n"
                          "   \tmov  dx,0fh\n"
                          "   \trol  bx,4h\n"
                          "   \tand  dx,bx\n"
                          "   \tcmp  dl,0ah\n"
                          "   \tjae  " + labelName4 + "\n"
                          "   \tadd  dl,'0'\n"
                          "   \tjmp  " + labelName5 + "\n"
                  + labelFullname4 + "\n"
                          "   \tadd  dl,'A'\n"
                          "   \tsub  dl,0ah\n"
                  + labelFullname5 + "\n"
                          "   \tint  21h\n"
                          "   \tdec  cx\n"
                          "   \tjnz  " + labelName3 << endl;

    // Print new line
    outputFile << "mov dl, 10" << endl;
    outputFile << "mov ah, 02h" << endl;
    outputFile << "int 21h" << endl;
    outputFile << "mov dl, 13" << endl;
    outputFile << "mov ah, 02h" << endl;
    outputFile << "int 21h" << endl;
}