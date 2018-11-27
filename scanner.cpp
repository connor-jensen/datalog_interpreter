#include "scanner.h"
#include <iostream>
#include <ctype.h> //to check for whitespace chars

using std::string;

Scanner::Scanner(string fileName)
{
    in.open(fileName);
    if (!in.good())
    {
        cout << "error: unable to open file" << endl;
    }
}

vector<shared_ptr<Token>> Scanner::lexicalAnalyzer()
{
    vector<shared_ptr<Token>> tokens;
    
    lineNumber = 1;
    char c;
    
    // finite state machine
    while(true)
    {
        // get char from file
        c = in.get();
        
        // check end of file, return tokens if true
        if (in.eof())
        {
            tokens.push_back(make_shared<Token>(END_OF_FILE, "", lineNumber));
            break;
        }
        
        // check for whitespace + newline, ignore whitespace and restart loop if newline
        if (isspace(c))
        {
            if (c == '\n')
            {
                lineNumber++;
            }
            continue;
        }
        
        // switch statements
        switch(c)
        {
            case ',':
            tokens.push_back(make_shared<Token>(COMMA, ",", lineNumber));
            break;
            
            case '.':
            tokens.push_back(make_shared<Token>(PERIOD, ".", lineNumber));
            break;
            
            case '?':
            tokens.push_back(make_shared<Token>(Q_MARK, "?", lineNumber));
            break;
            
            case '(':
            tokens.push_back(make_shared<Token>(LEFT_PAREN, "(", lineNumber));
            break;
            
            case ')':
            tokens.push_back(make_shared<Token>(RIGHT_PAREN, ")", lineNumber));
            break;
            
            case ':':
            if (in.peek() == '-') // check for colondash
            {
                in.get();
                tokens.push_back(make_shared<Token>(COLON_DASH, ":-", lineNumber));
            }
            else tokens.push_back(make_shared<Token>(COLON, ":", lineNumber));
            break;
            
            case '*':
            tokens.push_back(make_shared<Token>(MULTIPLY, "*", lineNumber));
            break;
            
            case '+':
            tokens.push_back(make_shared<Token>(ADD, "+", lineNumber));
            break;
            
            case '\'':
            tokens.push_back(getStringToken());
            break;
            
            case '#':
            if (in.peek() =='|') // check for block comment
            {
                in.get();
                //tokens.push_back(getBlockCommentToken());
                getBlockCommentToken();
            }
            else getCommentToken();//tokens.push_back(getCommentToken()); // line comment
            break;
            
            default:
            if(!isalpha(c)) //check for undefined
            {
                tokens.push_back(make_shared<Token>(UNDEFINED, string(1, c), lineNumber));
            }
            else //
            {
                in.putback(c);
                tokens.push_back(getAlphanumericToken());
            }
        }
    }
    return tokens;
}
    
shared_ptr<Token> Scanner::getStringToken() 
{
    int startLine = lineNumber;
    char c;
    string tokenContent = "'";
    
    while (true)
    {
        c = in.get();
        if (in.eof())
        {
            return make_shared<Token>(UNDEFINED, tokenContent, startLine);
        }
        else if (c == '\'')
        {
            if (in.peek() == '\'')
            {
                tokenContent += c;
                c = in.get();
            }
            else
            {
                tokenContent += '\'';
                return make_shared<Token>(STRING, tokenContent, startLine);
            }
        }
            
        if (c == '\n') 
        {
            lineNumber++;
        }
            
        tokenContent += c;
    }
}

shared_ptr<Token> Scanner::getCommentToken()
{
    int startLine = lineNumber;
    char c;
    string tokenContent = "#";
    
    while (true)
    {
        c = in.get();
        if (c == '\n')
        {
            lineNumber++;
            break;
        }
        if (in.eof())
        {
            break;
        }
        
        tokenContent += c;
    }
    return make_shared<Token>(COMMENT, tokenContent, startLine);
}

shared_ptr<Token> Scanner::getBlockCommentToken()
{
    int startLine = lineNumber;
    char c;
    string tokenContent = "#|";
    
    while(true){
        c = in.get();
        if (in.eof())
        {
            return make_shared<Token>(UNDEFINED, tokenContent, startLine);
        }
        if (c == '|' && in.peek() == '#')
        {
            in.get();
            tokenContent += "|#";
            return make_shared<Token>(COMMENT, tokenContent, startLine);
        }
        if (c == '\n')
        {
            lineNumber++;
        }
        
        tokenContent += c;
    }
}

shared_ptr<Token> Scanner::getAlphanumericToken()
{
    string tokenContent = "";
    while (isalnum(in.peek()))
    {
        tokenContent += in.get();
    }
    
    if(identifier.find(tokenContent) != identifier.end())
    {
        return make_shared<Token>(identifier[tokenContent], tokenContent, lineNumber);
    }
    else return make_shared<Token>(ID, tokenContent, lineNumber);
}