#include "token.h"

#include <sstream>
#include <iostream>
#include <memory>

using std::string;
using std::stringstream;

Token::Token(tokenType type, string tokenContent, int line)
    : type(type)
    , tokenContent(tokenContent)
    , line(line) {};

string Token::toString()
{
    stringstream ss;
    ss << line;
    return "(" + tokenString[type] + ",\"" + tokenContent + "\"," + ss.str() + ")";
}

tokenType Token::getType()
{
    return type;
}

string Token::getContent()
{
    return tokenContent;
}

string Token::getTypeString()
{
    return tokenString[type];
}