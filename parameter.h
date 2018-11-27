#pragma once

#include "token.h"
#include <memory>

using std::string;

class Parameter{
    private:
    public:
        Parameter();
        Parameter(shared_ptr<Token> token);
        ~Parameter();
        
        void expression(shared_ptr<Parameter> firstParameter,
                        shared_ptr<Token> operatorToken,
                        shared_ptr<Parameter> lastParameter);
        void terminal(shared_ptr<Token> token);
        string toString();
        
       tokenType type;
       shared_ptr<Token> value;
       shared_ptr<Parameter> firstParameter;
       shared_ptr<Parameter> lastParameter;
       shared_ptr<Token> operatorToken;
};