#pragma once

#include "relation.h"
#include "namespaces.h"

class RelationalDatabase{
  public:
    RelationalDatabase();
    ~RelationalDatabase();
    
    shared_ptr<Relation> select(shared_ptr<Relation>, vector<string>);
    shared_ptr<Relation> project(shared_ptr<Relation>, vector<string>);
    shared_ptr<Relation> rename(shared_ptr<Relation>, vector<string>);
    shared_ptr<Relation> getRelation(string);
    shared_ptr<Relation> relationUnion(shared_ptr<Relation>, shared_ptr<Relation>);
	  shared_ptr<Relation> join(shared_ptr<Relation>, shared_ptr<Relation>);
    
    void addRelation(string, shared_ptr<Relation>);
 
  private:
    map<string, shared_ptr<Relation>> relations;
    vector<shared_ptr<Relation>> createdRelations;
};