#pragma once

#include "predicate.h"
#include "token.h"
#include "rule.h"

using std::string;

#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>

class DatalogProgram {
  public:
    DatalogProgram();
    ~DatalogProgram();
    
    void newRule(shared_ptr<Rule> rule);
    void newFact(shared_ptr<Predicate> fact);
    void newScheme(shared_ptr<Predicate> scheme);
    void newQuery(shared_ptr<Predicate> query);
    void newDomainValue(string domainValue);
    void getError(shared_ptr<Token> errorToken);
    string toString();
    
    
    vector<shared_ptr<Predicate>> schemes;
    vector<shared_ptr<Predicate>> queries;
    vector<shared_ptr<Predicate>> facts;
    vector<shared_ptr<Rule>> rules;
    vector<string> domain;
    shared_ptr<Token> errorToken;
    
  
  private:
    void sortDomain();
    
};
