#pragma once

#include "relation.h"
#include "namespaces.h"

class RelationalDatabase{
  public:
    RelationalDatabase();
    ~RelationalDatabase();
    
    shared_ptr<Relation> select(string, vector<string>);
    shared_ptr<Relation> project(string, vector<int>);
    shared_ptr<Relation> rename(string, vector<string>);
    shared_ptr<Relation> getRelation(string);
    
    void addRelation(string, shared_ptr<Relation>);
 
  private:
    map<string, shared_ptr<Relation>> relations;
};