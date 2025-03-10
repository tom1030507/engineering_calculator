#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;

// Constants for conversion
const double PI = 3.14159265358979323846;
const double DEG_TO_RAD = PI / 180.0;

// Enum for variable types
enum VarType {
    INT_TYPE,
    FLOAT_TYPE
};

// Structure to store variable information
struct Variable {
    VarType type;
    double value;
};

// Function to check if character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Function to get operator precedence
int getPrecedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

// Function to check if token is a function
bool isFunction(const string& token) {
    return (token == "sin" || token == "cos" || token == "tan" || 
            token == "exp" || token == "log");
}

// Function to remove comments from input
string removeComments(const string& input) {
    string result = input;
    size_t pos = result.find("//");
    if (pos != string::npos) {
        result = result.substr(0, pos);
    }
    pos = result.find("/*");
    if (pos != string::npos) {
        size_t end = result.find("*/", pos);
        if (end != string::npos) {
            result.erase(pos, end - pos + 2);
        } else {
            result = result.substr(0, pos); // Handle unclosed comment
        }
    }
    // Trim trailing whitespace
    result.erase(result.find_last_not_of(" \t\r\n") + 1);
    return result;
}

// Function to tokenize the expression
vector<string> tokenize(const string& expression) {
    vector<string> tokens;
    string current;
    
    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];
        
        if (isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else if (c == '(' || c == ')') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(string(1, c));
        } else if (isOperator(c)) {
            if (c == '-' && (i == 0 || expression[i-1] == '(' || isOperator(expression[i-1]))) {
                current += c;
            } else {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                tokens.push_back(string(1, c));
            }
        } else {
            current += c;
            if (isFunction(current) && (i + 1 < expression.length() && expression[i + 1] != '(')) {
                tokens.push_back(current); // Function without parens
                current.clear();
            }
        }
    }
    
    if (!current.empty()) {
        tokens.push_back(current);
    }
    
    return tokens;
}

// Function to evaluate an expression
double evaluateExpression(const string& expression, map<string, Variable>& variables) {
    vector<string> tokens = tokenize(expression);
    stack<double> values;
    stack<string> operators;
    
    for (size_t i = 0; i < tokens.size(); i++) {
        const string& token = tokens[i];
        
        // If token is a number
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
            values.push(stod(token));
        }
        // If token is a variable
        else if (isalpha(token[0])) {
            if (isFunction(token)) {
                if (i + 1 < tokens.size() && !isOperator(tokens[i + 1][0]) && tokens[i + 1] != "(" && tokens[i + 1] != ")") {
                    // Function without parens, apply to next token
                    i++;
                    double val = stod(tokens[i]);
                    if (token == "sin") values.push(sin(val * DEG_TO_RAD));
                    else if (token == "cos") values.push(cos(val * DEG_TO_RAD));
                    else if (token == "tan") values.push(tan(val * DEG_TO_RAD));
                    else if (token == "exp") values.push(exp(val));
                    else if (token == "log") {
                        if (val <= 0) throw runtime_error("Log of non-positive number");
                        values.push(log10(val));
                    }
                } else {
                    operators.push(token); // Expect parens
                }
            } else {
                if (variables.find(token) == variables.end()) {
                    throw runtime_error("Variable " + token + " not defined");
                }
                values.push(variables[token].value);
            }
        }
        // If token is an opening parenthesis
        else if (token == "(") {
            operators.push(token);
        }
        // If token is a closing parenthesis
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                string op = operators.top();
                operators.pop();
                
                if (isFunction(op)) {
                    double val = values.top();
                    values.pop();
                    
                    if (op == "sin") {
                        values.push(sin(val * DEG_TO_RAD));
                    } else if (op == "cos") {
                        values.push(cos(val * DEG_TO_RAD));
                    } else if (op == "tan") {
                        values.push(tan(val * DEG_TO_RAD));
                    } else if (op == "exp") {
                        values.push(exp(val));
                    } else if (op == "log") {
                        if (val <= 0) {
                            throw runtime_error("Log of non-positive number");
                        }
                        values.push(log10(val));
                    }
                } else {
                    double val2 = values.top();
                    values.pop();
                    double val1 = values.top();
                    values.pop();
                    
                    if (op == "+") values.push(val1 + val2);
                    else if (op == "-") values.push(val1 - val2);
                    else if (op == "*") values.push(val1 * val2);
                    else if (op == "/") {
                        if (val2 == 0) throw runtime_error("Division by zero");
                        values.push(val1 / val2);
                    }
                    else if (op == "^") values.push(pow(val1, val2));
                }
            }
            if (!operators.empty()) {
                operators.pop(); // Remove the "("
                // If a function precedes "(", apply it now
                if (!operators.empty() && isFunction(operators.top())) {
                    string func = operators.top();
                    operators.pop();
                    double val = values.top();
                    values.pop();
                    if (func == "sin") values.push(sin(val * DEG_TO_RAD));
                    else if (func == "cos") values.push(cos(val * DEG_TO_RAD));
                    else if (func == "tan") values.push(tan(val * DEG_TO_RAD));
                    else if (func == "exp") values.push(exp(val));
                    else if (func == "log") {
                        if (val <= 0) throw runtime_error("Log of non-positive number");
                        values.push(log10(val));
                    }
                }
            } else {
                throw runtime_error("Mismatched parentheses");
            }
        }
        // If token is an operator
        else if (isOperator(token[0])) {
            while (!operators.empty() && operators.top() != "(" && 
                   getPrecedence(operators.top()[0]) >= getPrecedence(token[0])) {
                string op = operators.top();
                operators.pop();
                
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                
                if (op == "+") values.push(val1 + val2);
                else if (op == "-") values.push(val1 - val2);
                else if (op == "*") values.push(val1 * val2);
                else if (op == "/") {
                    if (val2 == 0) {
                        throw runtime_error("Division by zero");
                    }
                    values.push(val1 / val2);
                }
                else if (op == "^") values.push(pow(val1, val2));
            }
            
            operators.push(token);
        }
    }
    
    // Process any remaining operators
    while (!operators.empty()) {
        string op = operators.top();
        operators.pop();
        
        if (op == "(") {
            throw runtime_error("Mismatched parentheses");
        }
        
        if (isFunction(op)) {
            double val = values.top();
            values.pop();
            
            if (op == "sin") {
                values.push(sin(val * DEG_TO_RAD));
            } else if (op == "cos") {
                values.push(cos(val * DEG_TO_RAD));
            } else if (op == "tan") {
                values.push(tan(val * DEG_TO_RAD));
            } else if (op == "exp") {
                values.push(exp(val));
            } else if (op == "log") {
                if (val <= 0) {
                    throw runtime_error("Log of non-positive number");
                }
                values.push(log10(val)); // Common logarithm (base 10)
            }
        } else {
            double val2 = values.top();
            values.pop();
            double val1 = values.top();
            values.pop();
            
            if (op == "+") values.push(val1 + val2);
            else if (op == "-") values.push(val1 - val2);
            else if (op == "*") values.push(val1 * val2);
            else if (op == "/") {
                if (val2 == 0) {
                    throw runtime_error("Division by zero");
                }
                values.push(val1 / val2);
            }
            else if (op == "^") values.push(pow(val1, val2));
        }
    }
    
    if (values.size() != 1) {
        throw runtime_error("Invalid expression");
    }
    
    return values.top();
}

// Function to parse variable declaration
void parseVariableDeclaration(const string& line, map<string, Variable>& variables) {
    string cleanLine = line;
    cleanLine.erase(remove_if(cleanLine.begin(), cleanLine.end(), ::isspace), cleanLine.end());
    if (cleanLine.empty() || (cleanLine.back() != ';' && !cleanLine.substr(0, 3).compare("int") && !cleanLine.substr(0, 5).compare("float"))) {
        return; // Skip invalid lines
    }
    if (cleanLine.back() == ';') {
        cleanLine.pop_back();
    }

    size_t typeEnd = (cleanLine.substr(0, 3) == "int") ? 3 : 5; // "int" or "float"
    string varType = cleanLine.substr(0, typeEnd);
    string varList = cleanLine.substr(typeEnd);

    stringstream ss(varList);
    string var;

    while (getline(ss, var, ',')) {
        if (!var.empty() && isalpha(var[0])) {
            VarType type = (varType == "int") ? INT_TYPE : FLOAT_TYPE;
            variables[var] = {type, 0.0};
        }
    }
}


// Function to parse variable assignment
void parseVariableAssignment(const string& line, map<string, Variable>& variables) {
    // Remove any whitespace and extract variable name and expression
    string cleanLine = line;
    cleanLine.erase(remove_if(cleanLine.begin(), cleanLine.end(), ::isspace), cleanLine.end());
    
    // Find the equals sign
    size_t equalsPos = cleanLine.find('=');
    if (equalsPos == string::npos || equalsPos == 0) {
        throw runtime_error("Invalid variable assignment");
    }
    
    string varName = cleanLine.substr(0, equalsPos);
    string expression = cleanLine.substr(equalsPos + 1);
    
    // Remove semicolon if present
    if (!expression.empty() && expression.back() == ';') {
        expression.pop_back();
    }
    
    // Check if variable exists
    if (variables.find(varName) == variables.end()) {
        throw runtime_error("Variable " + varName + " not declared");
    }
    
    // Evaluate the expression and assign to the variable
    double value = evaluateExpression(expression, variables);
    
    // Apply type-specific handling
    if (variables[varName].type == INT_TYPE) {
        variables[varName].value = static_cast<int>(value); // Convert to int by truncation
    } else {
        variables[varName].value = value; // Keep as float
    }
}

// Function to handle a single line of input
string processLine(const string& line, map<string, Variable>& variables) {
    // Remove comments first
    string cleanLine = removeComments(line);
    
    // Skip empty lines
    if (cleanLine.empty() || cleanLine.find_first_not_of(" \t\r\n") == string::npos) {
        return "";
    }
    
    // Check if line is a variable declaration
    if (cleanLine.find("int ") == 0 || cleanLine.find("float ") == 0) {
        try {
            parseVariableDeclaration(cleanLine, variables);
            return "";
        } catch (const exception& e) {
            return "Error: " + string(e.what());
        }
    }
    // Check if line is a variable assignment
    else if (cleanLine.find('=') != string::npos) {
        try {
            parseVariableAssignment(cleanLine, variables);
            return "";
        } catch (const exception& e) {
            return "Error: " + string(e.what());
        }
    } else {
        // Evaluate expression
        try {
            double result = evaluateExpression(cleanLine, variables);
            ostringstream oss;
            if (result == static_cast<int>(result)) {
                oss << static_cast<int>(result);
            } else {
                oss << fixed << setprecision(5) << result;
            }
            return oss.str();
        } catch (const exception& e) {
            return "Error: " + string(e.what());
        }
    }
}

// Function to print all variables
string printVariables(const map<string, Variable>& variables) {
    if (variables.empty()) {
        return "";
    }
    
    ostringstream oss;
    oss << "Variables:" << endl;
    
    // Collect names and values for printing in proper order
    vector<string> intVars, floatVars;
    map<string, double> intValues, floatValues;
    
    for (const auto& var : variables) {
        if (var.second.type == INT_TYPE) {
            intVars.push_back(var.first);
            intValues[var.first] = var.second.value;
        } else {
            floatVars.push_back(var.first);
            floatValues[var.first] = var.second.value;
        }
    }
    
    // Sort variable names alphabetically
    sort(intVars.begin(), intVars.end());
    sort(floatVars.begin(), floatVars.end());
    
    // Format the output as in the example: (a, b, c, d, e) = (5, 7, 30, 31.907, 71.107)
    vector<string> allVars;
    vector<string> allValues;
    
    // Add integer variables
    for (const auto& var : intVars) {
        allVars.push_back(var);
        ostringstream valStr;
        valStr << static_cast<int>(intValues[var]);
        allValues.push_back(valStr.str());
    }
    
    // Add float variables
    for (const auto& var : floatVars) {
        allVars.push_back(var);
        ostringstream valStr;
        valStr << fixed << setprecision(3) << floatValues[var];
        allValues.push_back(valStr.str());
    }
    
    // Format the output
    oss << "(";
    for (size_t i = 0; i < allVars.size(); ++i) {
        oss << allVars[i];
        if (i < allVars.size() - 1) oss << ", ";
    }
    oss << ") = (";
    for (size_t i = 0; i < allValues.size(); ++i) {
        oss << allValues[i];
        if (i < allValues.size() - 1) oss << ", ";
    }
    oss << ")";
    
    return oss.str();
}

// Function to process file input
bool processFile(const string& inputFilename, const string& outputFilename) {
    ifstream inFile(inputFilename);
    if (!inFile) {
        cerr << "Error: Could not open input file " << inputFilename << endl;
        return false;
    }
    
    ofstream outFile(outputFilename);
    if (!outFile) {
        cerr << "Error: Could not open output file " << outputFilename << endl;
        return false;
    }
    
    map<string, Variable> variables;
    string line;
    
    while (getline(inFile, line)) {
        string result = processLine(line, variables);
        if (!result.empty()) {
            outFile << result << endl;
        }
    }
    
    // Output all variable values at the end
    string varOutput = printVariables(variables);
    if (!varOutput.empty()) {
        outFile << varOutput << endl;
    }
    
    return true;
}

// Function for direct mode processing
void runDirectMode() {
    map<string, Variable> variables;
    string line;
    
    while (true) {
        cout << "> ";
        getline(cin, line);
        
        if (line == "exit" || line == "quit") {
            break;
        }
        
        string result = processLine(line, variables);
        if (!result.empty()) {
            cout << result << endl;
        }
    }
    
    // Print all variables at the end
    string varOutput = printVariables(variables);
    if (!varOutput.empty()) {
        cout << varOutput << endl;
    }
}

int main() {
    cout << "Engineering Calculator" << endl;
    cout << "Choose your mode (direct/file): ";
    string mode;
    cin >> mode;
    cin.ignore(); // Clear the newline from the buffer
    
    if (mode == "direct") {
        cout << "Direct mode selected. Type 'exit' or 'quit' to exit." << endl;
        runDirectMode();
    } else if (mode == "file") {
        string inputFile, outputFile;
        cout << "Input file name: ";
        getline(cin, inputFile);
        cout << "Output file name: ";
        getline(cin, outputFile);
        
        if (processFile(inputFile, outputFile)) {
            cout << "Processing complete. Results written to " << outputFile << endl;
        } else {
            cout << "Error processing files." << endl;
        }
    } else {
        cout << "Invalid mode. Please run the program again and choose 'direct' or 'file'." << endl;
    }
    
    return 0;
}