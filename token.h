#pragma once
#include <string>
#include <map>
#include <memory>

#include "namespaces.h"


//using namespace std;

enum tokenType
{
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    WHITESPACE,
    UNDEFINED,
    END_OF_FILE,
    EXPRESSION
};

class Token
{
  public:
    Token(tokenType type, string tokenContent, int line);
    // ~Token();
    
    string toString(); //prints out the string name of the token
    
    map<tokenType, string> tokenString //maps the token type to its name
    {
      {COMMA, "COMMA"},
      {PERIOD, "PERIOD"},
      {Q_MARK, "Q_MARK"},
      {LEFT_PAREN, "LEFT_PAREN"},
      {RIGHT_PAREN, "RIGHT_PAREN"},
      {COLON, "COLON"}, 
      {COLON_DASH, "COLON_DASH"},
      {MULTIPLY, "MULTIPLY"},
      {ADD, "ADD"},
      {SCHEMES, "SCHEMES"},
      {FACTS, "FACTS"},
      {RULES, "RULES"},
      {QUERIES, "QUERIES"},
      {ID, "ID"},
      {STRING, "STRING"}, 
      {COMMENT, "COMMENT"},
      {WHITESPACE, "WHITESPACE"},
      {UNDEFINED, "UNDEFINED"},
      {END_OF_FILE, "EOF"},
      {EXPRESSION, "EXPRESSION"}
      
    };
    
    
    tokenType getType();
    string getContent();
    string getTypeString();
    
  private:
    tokenType type;
    string tokenContent;
    int line;
};
