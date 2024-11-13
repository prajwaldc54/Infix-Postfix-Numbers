/*
Author Name: Prajwal D C
--------------------------------------------------------------------------------------------------------
Program Information: a C++ program that (a) read infix expressions from keyboard and print the infix,
(b) convert each expression to postfix notation print each postfix, and
(c) evaluate each postfix expression to determine its value and print each value.
---------------------------------------------------------------------------------------------------------

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>  // For tolower
using namespace std;

bool isOperator(char c) { // check if we have valid binary operator
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int priority(char op) {    // Division and multiplication get top priority 
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

string infixtopost(const string& infix) {
    stack <char> s; // storing operations
    string postfix;

    for (int i = 0; i < infix.length(); i++) {  // checking everything in the infix expression and its in ch
        char ch = infix[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            postfix += ch;  // if it's an operand, add it to postfix
        }
        else if (ch == '(') {
            s.push(ch);  // checking if it is opening parenthesis and pushing it to the stack 
        }
        else if (ch == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();  // pop operators till we find '('
                s.pop();
            }
            if (!s.empty()) s.pop(); // pop '('
        }
        else if (isOperator(ch)) {  // for the correct order
            while (!s.empty() && priority(s.top()) >= priority(ch)) {
                postfix += s.top();
                s.pop();
            }
            s.push(ch);
        }
    }

    while (!s.empty()) {  // pop remaining operators
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

void readValues(vector<double>& value) {
    value.resize(26, -1); // Resize the vector to store values for 26 alphabets (default -1 for values not given to throw errors)

    ifstream file("symbol.dat");
    if (!file) {
        cout << "Error: Unable to open symbol.dat file" << endl;
        return;
    }

    int comingValue;
    char ch;

    while (file >> ch >> comingValue) {
        char sensetive = tolower(ch);  // Convert to lowercase for case insensitivity
        value[sensetive - 'a'] = comingValue;
    }
    file.close(); // close the file after reading
}

double rpNotation(const string& postfix, const vector<double>& countValue) {
    stack <double> operStack; // to store operands 
    for (int i = 0; i < postfix.length(); i++) {
        char ch = postfix[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            int index = tolower(ch) - 'a'; // for case sensitivity and index values
            if (index >= 0 && index < countValue.size()) {
                if (countValue[index] == -1) {  // Check if the value is undefined
                    cout << "Error: Variable '" << ch << "' not found in symbol.dat" << endl;
                    return -1;
                }
                operStack.push(countValue[index]);  // push corresponding value for operand
            }
            else {
                cout << "Invalid variable: " << ch << endl;
                return -1;
            }
        }
        else if (isOperator(ch)) {
            if (operStack.size() < 2) {
                cout << "Not enough operands for operator " << ch << endl;
                return -1;
            }
            double val2 = operStack.top(); operStack.pop();
            double val1 = operStack.top(); operStack.pop(); // it's reverse Polish so 

            switch (ch) {
            case '+': operStack.push(val1 + val2); break;
            case '-': operStack.push(val1 - val2); break;
            case '*': operStack.push(val1 * val2); break;
            case '/':
                if (val2 != 0) {
                    operStack.push(val1 / val2); break;
                }
                else {
                    cout << "Division by zero error" << endl;
                    return -1;
                }
            }
        }
        else {
            cout << "Invalid character: " << ch << endl;
            return -1;
        }
    }

    if (operStack.size() == 1) {  // result should be on top of the stack
        return operStack.top();
    }
    else {
        cout << "Error: Invalid expression" << endl;
        return -1;
    }
}


int main() {
    vector<double> alphabetValues;
    readValues(alphabetValues); // read the values from symbol.dat file

    string infix;
    while (true) {
        cout << "Enter an infix expression or 'E' to exit: ";
        getline(cin, infix);
        if (infix == "E") break; // close if E

        string postfix = infixtopost(infix); // infix to postfix
        cout << "Postfix: " << postfix << endl;

        double result = rpNotation(postfix, alphabetValues); // postfix to value
        if (result != -1) {
            cout << "Result: " << result << endl;
        }
    }

    return 0;
}
