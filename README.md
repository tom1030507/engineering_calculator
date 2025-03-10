# Engineering Calculator

## Overview
The Engineering Calculator is a C++ program designed to evaluate mathematical expressions, manage variables, and process input either interactively (direct mode) or from a file (file mode). It supports basic arithmetic operations, trigonometric functions (in degrees), exponentiation, logarithms, and variable declarations/assignments. The program is robust, handling errors like division by zero, undefined variables, and mismatched parentheses, while also supporting comments in the input.

## Features
- **Mathematical Operations**: Supports addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), and exponentiation (`^`).
- **Trigonometric Functions**: `sin`, `cos`, `tan` (input in degrees, converted to radians internally).
- **Exponential and Logarithmic Functions**: `exp` (exponential) and `log` (base-10 logarithm).
- **Variable Support**: Declare variables as `int` or `float`, assign values via expressions, and use them in calculations.
- **Expression Evaluation**: Uses a stack-based algorithm to evaluate expressions with proper operator precedence.
- **Comment Handling**: Ignores single-line (`//`) and multi-line (`/* */`) comments.
- **Input Modes**:
  - **Direct Mode**: Interactive command-line interface.
  - **File Mode**: Process input from a file and write results to an output file.
- **Error Handling**: Catches and reports errors like division by zero, undefined variables, invalid expressions, and mismatched parentheses.
- **Output Formatting**: Integers are displayed without decimals; floating-point numbers are shown with up to 5 decimal places for expressions and 3 for variable summaries.

## Requirements
- A C++ compiler (e.g., g++, clang++).
- Standard C++ libraries (included with most compilers).

## Installation
1. Clone or download the source code:
   ```
   git clone <repository-url>
   ```
2. Navigate to the directory containing the source code.
3. Compile the program using a C++ compiler:
   ```
   g++ -o calculator calculator.cpp
   ```
   Replace `calculator.cpp` with the name of your source file.

## Usage

### Running the Program
Execute the compiled program:
```
./calculator
```
You'll be prompted to choose a mode: `direct` or `file`.

#### Direct Mode
1. Select `direct` when prompted.
2. Enter expressions, variable declarations, or assignments at the `>` prompt.
3. Type `exit` or `quit` to stop the program.
4. Example session:
   ```
   Engineering Calculator
   Choose your mode (direct/file): direct
   Direct mode selected. Type 'exit' or 'quit' to exit.
   > int a, b;
   > a = 5;
   > b = 7;
   > a + b
   12
   > float x;
   > x = sin(90) + cos(0);
   2.00000
   > exit
   Variables:
   (a, b, x) = (5, 7, 2.000)
   ```

#### File Mode
1. Select `file` when prompted.
2. Provide the input and output file names.
3. The program processes the input file and writes results to the output file.
4. Example input file (`input.txt`):
   ```
   int a, b;
   a = 5;
   b = 7;
   a + b
   float x;
   x = sin(90) + cos(0); // Calculate trig functions
   x * 2
   ```
5. Run:
   ```
   Engineering Calculator
   Choose your mode (direct/file): file
   Input file name: input.txt
   Output file name: output.txt
   Processing complete. Results written to output.txt
   ```
6. Output file (`output.txt`):
   ```
   12
   4.00000
   Variables:
   (a, b, x) = (5, 7, 2.000)
   ```

### Syntax
- **Variable Declaration**: `int var1, var2;` or `float var1, var2;`
- **Assignment**: `var = expression;` (semicolon optional in direct mode)
- **Expressions**: Combine numbers, variables, operators, and functions (e.g., `sin(90)`, `a + b * 2`).
- **Comments**: Use `//` for single-line or `/* */` for multi-line comments.

## Limitations
- Trigonometric functions assume input in degrees.
- `log` is base-10 only (not natural log).
- No support for complex numbers or advanced calculus operations.
- Variables must be declared before use.

## Error Messages
- `Error: Division by zero`: Attempted division by zero.
- `Error: Variable <name> not defined`: Used an undeclared variable.
- `Error: Log of non-positive number`: Logarithm applied to zero or negative value.
- `Error: Mismatched parentheses`: Unbalanced `(` or `)`.
- `Error: Invalid expression`: Malformed input that cannot be evaluated.

## Contributing
Feel free to fork the repository, make improvements, and submit pull requests. Suggestions for new features (e.g., more functions, natural log, or enhanced error reporting) are welcome!

## License
This project is open-source and available under the MIT License.

