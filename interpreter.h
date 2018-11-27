#pragma once 

#include <utility>

#include "token.h"
#include "datalogProgram.h"
#include "predicate.h"
#include "parameter.h"
#include "relationalDatabase.h"
#include "relation.h"
#include "rule.h"

#include "namespaces.h"

class Interpreter{
  public:
    Interpreter(shared_ptr<DatalogProgram>);
    //~Interpreter();
    void createDatabase();
    string runQueries();
  private:
    shared_ptr<DatalogProgram> program;
    shared_ptr<RelationalDatabase> database;
    
    void addSchemes();
    void addFacts();
    
    // new functions
    void runRules();
    bool runRule(shared_ptr<Rule>);
    std::pair<unsigned int, shared_ptr<Relation>> runQuery(shared_ptr<Relation>, shared_ptr<Predicate>);
  
};