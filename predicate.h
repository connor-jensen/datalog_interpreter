#pragma once 

#include <vector>
#include <memory>
#include "token.h"
#include "parameter.h"

using std::string;

class Predicate{
    private:
    public: 
        Predicate(shared_ptr<Token> id);
        ~Predicate();
        void newParameter(shared_ptr<Parameter> parameter);
        string toString();
        
        shared_ptr<Token> id;
        vector<shared_ptr<Parameter>> parameterList;
};