#include "relationalDatabase.h"

RelationalDatabase::RelationalDatabase() {
  
}

RelationalDatabase::~RelationalDatabase() {}

void RelationalDatabase::addRelation(string name, shared_ptr<Relation> relation){
  if (this->getRelation(name)!= nullptr) {
    this->relations.erase(this->relations.find(name));
  }
  //cout << "addRelation relation.rows.size() " << relation->rows.size() << endl;
  relation->name = name;
  relations.insert(pair<string, shared_ptr<Relation>>(name, relation));
  
}

shared_ptr<Relation> RelationalDatabase::select(string relation, vector<string> values){
  
  shared_ptr<Relation> operand = getRelation(relation);
  
  if (operand == nullptr) {
    return nullptr;
  }
  
  shared_ptr<Relation> result = make_shared<Relation> ("query", operand->schemeParameters);
  
  for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++){
  //for (auto row : operand->rows) {
    bool add_row = true;
    vector<string> row = *i;
    
    for (unsigned int j = 0; j < values.size(); j++){
      //looks for search terms
      if (values[j].find("'") != string::npos && values[j] != row[j]) {
        add_row = false;
      }
      
      if (values[j].find("'") == string::npos) {
        //searches
        for (unsigned int k = j + 1; k < values.size(); k++){
          if (values [j] == values[k] && row[j] != row[k]){
            add_row = false;
            break;
          }
        }
      }
      
      if (!add_row){
        break;
      }
      
    }
    
    if (add_row){
      result->addRow(row);
    }
  }
  
  return result;
  
}

shared_ptr<Relation> RelationalDatabase::project(string relation, vector<int> indexes){
 
  shared_ptr<Relation> operand = getRelation(relation);
  
  if (operand == nullptr) {
    return nullptr;
  }
  
  shared_ptr<Relation> result = make_shared<Relation>("query", operand->schemeParameters);
  //for (auto row : operand->rows){
  for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++){
    vector<string> operand_row = *i;
    vector<string> result_row;
    
    for(unsigned int j = 0; j < indexes.size(); j++){
      result_row.push_back(operand_row[indexes[j]]);
    }
    
    if (result_row.size() > 0){
      result->addRow(result_row);
    }
  }
  
  return result;
  
}

shared_ptr<Relation> RelationalDatabase::rename(string relation, vector<string> schemeParameters){
  shared_ptr<Relation> operand = getRelation(relation);
  
  if (operand == nullptr) {
    return nullptr;
  }
  
  shared_ptr<Relation> result = make_shared<Relation>("query", schemeParameters);
  result->rows = operand->rows;
  
  return result;
  
}

shared_ptr<Relation> RelationalDatabase::getRelation(string name){
  
  if (this->relations.find(name) == relations.end()){
    return nullptr;
  }
  
  //cout << "getRelation(" << name <<") rows.size " << this->relations[name]->rows.size() << endl;
  return this->relations[name];
  
}

