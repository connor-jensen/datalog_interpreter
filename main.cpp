#include <iostream>
#include <fstream>
#include <vector>
#include <memory>


#include "namespaces.h"
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "datalogProgram.h"
#include "interpreter.h"

int main(int argc, char* argv[])
{
    // part 1 testing:

    // vector<string> inputFileNames
    // {
    //     "in1.txt", // schedule('M', C)?) - select on 'M' before variable
    //     "in2.txt", // schedule(D, '236')? - select on '236' after variable
    //     "in3.txt", // schedule('M', Y)? - rename after constant
    //     "in4.txt", // schedule(X, '236')? - rename before constant
    //     "in5.txt", // schedule(D)? - project on D
    //     "in6.txt", // schedule(C)? - project on C
    //     "in7.txt", // schedule(X, '113')? - QUERY: select on '113', rename D,
    //     "in8.txt", // schedule('M', C)? - QUERY: select on 'M', project on C
    //     "in9.txt", // schedule('M', X, Y) - QUERY: select on 'M', rename on X and Y 
    //     "in10.txt" // schedule(A, B, B) - QUERY: select on B B, rename to B
    // };

    // vector<string> expectedOutputFileNames
    // {
    //     "out1.txt",
    //     "out2.txt",
    //     "out3.txt",
    //     "out4.txt",
    //     "out5.txt",
    //     "out6.txt",
    //     "out7.txt",
    //     "out8.txt",
    //     "out9.txt",
    //     "out10.txt"
    // };

    // for(int i = 0; i < inputFileNames.size(); i++) {
    //     Scanner scanner("tests/" + inputFileNames[i]);
    //     vector<shared_ptr<Token>> tokens = scanner.lexicalAnalyzer();

    //     Parser parser = Parser(tokens);
    //     shared_ptr<DatalogProgram> program = parser.datalogParsing();

    //     Interpreter interpreter = Interpreter(program);
    //     interpreter.createDatabase();

    //     string output = interpreter.runQueries();

    //     ifstream expectedOutputFile("expectedOutputs/" + expectedOutputFileNames[i]);
    //     std::ofstream actualOutputFile("actualOutputs/" + expectedOutputFileNames[i]);

    //     stringstream ss;
    //     ss << expectedOutputFile.rdbuf();
    //     string expectedOutput = ss.str();

    //     actualOutputFile << output;

    //     cout << "Test " << i + 1 << ": ";
    //     if (expectedOutput == output) {
    //         cout << "PASS" << endl;
    //     }
    //     else cout << "FAIL" << endl;

    //     cout << "output:" << endl;
    //     cout << output << endl << endl;
    // }

    // part 2
    Scanner scanner(argv[1]);
    std::vector<shared_ptr<Token>> tokens = scanner.lexicalAnalyzer();
    
    Parser parser = Parser(tokens);
    shared_ptr<DatalogProgram> program = parser.datalogParsing();
    
    Interpreter interpreter = Interpreter(program);
    interpreter.createDatabase();
    
    std::cout << interpreter.runQueries();
    
    return 0;
}