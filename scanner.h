#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <memory>

#include "token.h"

//using namespace std;
using std::string;

class Scanner
{
public:
    Scanner(string filename);
    std::vector<shared_ptr<Token>> lexicalAnalyzer();  

private:
    ifstream in;
    int lineNumber = 1;
    shared_ptr<Token> getStringToken();
	shared_ptr<Token> getCommentToken();
	shared_ptr<Token> getBlockCommentToken();
	shared_ptr<Token> getAlphanumericToken();
    map<string, tokenType> identifier = {{"Schemes", SCHEMES}, {"Facts", FACTS}, {"Rules", RULES}, {"Queries", QUERIES}};
};
