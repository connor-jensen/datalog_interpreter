#pragma once

#include <memory>
#include "predicate.h"
#include "token.h"

using std::string;

class Rule{
  private:
  public:
    Rule();
    ~Rule();
    
    void setHeadPredicate(shared_ptr<Predicate> headPredicate);
    void addPredicate(shared_ptr<Predicate> predicate);
    string toString();
    
    shared_ptr<Predicate> headPredicate;
    vector<shared_ptr<Predicate>> predicateList;
    
};