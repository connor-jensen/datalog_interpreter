#pragma once

#include <vector>
#include <memory>

#include "datalogProgram.h"
#include "token.h"
#include "predicate.h"
#include "rule.h"
#include "parameter.h"

using std::string;

class Parser{
  public:
    Parser(vector<shared_ptr<Token>>);
    ~Parser();
    shared_ptr<DatalogProgram> datalogParsing();
  private:
    vector<shared_ptr<Token>> tokens;
    shared_ptr<DatalogProgram> datalogProgram;
   
    //Our grammar functions
    void datalog();
    void scheme();
    void schemeList();
    void idList(shared_ptr<Predicate> predicate);
    void fact();
    void factList();
    void rule();
    void ruleList();
    
    shared_ptr<Predicate> headPredicate();
    shared_ptr<Predicate> predicate();
    void predicateList(shared_ptr<Rule> rule);
    
    shared_ptr<Parameter> parameter();
    void parameterList(shared_ptr<Predicate> predicate);
    
    shared_ptr<Parameter> expression();
    shared_ptr<Token> operatorGrammar();
    
    void query();
    void queryList();
    void stringList(shared_ptr<Predicate> predicate);
    //end grammar functions
    
    //Token functions
    tokenType nextTokenType();
    shared_ptr<Token> consumeToken(tokenType);
    void putbackToken(shared_ptr<Token>);
  
};