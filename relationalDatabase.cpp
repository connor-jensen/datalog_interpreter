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
  createdRelations.push_back(relation);
  
}

shared_ptr<Relation> RelationalDatabase::select(shared_ptr<Relation> operand, vector<string> values){
  if (operand == nullptr) return nullptr;
  
  shared_ptr<Relation> result = make_shared<Relation> ("query", operand->schemeParameters);
  createdRelations.push_back(result);
  
  for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++){
  //for (auto row : operand->rows) {
    bool add_row = true;
    vector<string> row = *i;
    
    for (unsigned int j = 0; j < values.size(); j++){
      //looks for search terms
      if (values[j].find("'") != string::npos && values[j] != row[j]) {
        add_row = false;
        break;
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

shared_ptr<Relation> RelationalDatabase::project(shared_ptr<Relation> operand, vector<string> newScheme){
   
  if (operand == nullptr) {
    return nullptr;
  }
  
  shared_ptr<Relation> result = make_shared<Relation>("query", newScheme);
  createdRelations.push_back(result);
  //for (auto row : operand->rows){
  
  vector<int> operandIndexes;
  for (unsigned int i = 0; i < newScheme.size(); i++){
    for (unsigned int j = 0; j<operand->schemeParameters.size(); j++){
      if (operand->schemeParameters[j] == newScheme[i]){
        operandIndexes.push_back(j);
      }
    }
  }

  for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++){
    vector<string> operandRow = *i;
    vector<string> newRow;
    
    for(unsigned int j = 0; j < operandIndexes.size(); j++){
      unsigned int operandPosition = operandIndexes[j];
      newRow.push_back(operandRow[operandPosition]);      
      }
    
    if (newRow.size() > 0){
      result->addRow(newRow);
    }
  }
  
  return result;
  
}

shared_ptr<Relation> RelationalDatabase::rename(shared_ptr<Relation>operand, vector<string> schemeParameters){
  
  if (operand == nullptr) {
    return nullptr;
  }
  
  shared_ptr<Relation> result = make_shared<Relation>("query", schemeParameters);
  createdRelations.push_back(result);
  result->rows = operand->rows;
  
  return result;
  
}

shared_ptr<Relation> RelationalDatabase::relationUnion(shared_ptr<Relation> operand1, shared_ptr<Relation> operand2){
  if (operand1 == NULL && operand2 == NULL) return NULL;
  if (operand1 == NULL) return operand2;
  if (operand2 == NULL) return operand1;

  shared_ptr<Relation> result = make_shared<Relation>("query", operand1->schemeParameters);
  createdRelations.push_back(result);
  result->rows = operand1->rows;

  for (set<vector<string>>::iterator i = operand2->rows.begin(); i != operand2->rows.end(); i++){
    vector<string> row = *i;
    result->addRow(row);
  }

  return result;

}

shared_ptr<Relation>RelationalDatabase::join(shared_ptr<Relation> operand1, shared_ptr<Relation> operand2){
  if (operand1 == NULL || operand2 == NULL){
    return NULL;
  }
  
  vector<string>newScheme = operand1->schemeParameters;
  for (unsigned int i = 0; i < operand2->schemeParameters.size(); i++){
    if (find(newScheme.begin(), newScheme.end(), operand2->schemeParameters[i]) == newScheme.end()){
      newScheme.push_back(operand2->schemeParameters[i]);
    }
  }

  shared_ptr<Relation> result = make_shared<Relation>("query", newScheme);
  createdRelations.push_back(result);

  vector<int> matchIndexes;
  for (unsigned int i = 0; i < operand2->schemeParameters.size(); i++){
    bool matchFound = false;
    for (unsigned int j = 0; j < operand1->schemeParameters.size(); j++){
      if (operand2->schemeParameters[i] == operand1->schemeParameters[j]){
        matchIndexes.push_back(j);
        matchFound = true;
        break;
      }
    }

    if (!matchFound) matchIndexes.push_back(-1);

  }

  for (set<vector<string>>::iterator i = operand1->rows.begin(); i != operand1->rows.end(); i++){
    vector<string> op1Row = *i;

    for (set<vector<string>>::iterator j = operand2->rows.begin(); j != operand2->rows.end(); j++){
      vector<string> op2Row = *j;
      bool addRow = true;
      vector<string> newRow = op1Row;

      for (unsigned int k = 0; k < matchIndexes.size(); k++){
        if (matchIndexes[k] == -1){
          newRow.push_back(op2Row[k]);
        }
        else {
          if (op2Row[k] != op1Row[matchIndexes[k]]){
            addRow = false;
            break;
          }
        }
      }

      if (addRow) result->addRow(newRow);

    }
  }

  return result;
}


shared_ptr<Relation> RelationalDatabase::getRelation(string name){
  
  if (this->relations.find(name) == relations.end()){
    return nullptr;
  }
  
  //cout << "getRelation(" << name <<") rows.size " << this->relations[name]->rows.size() << endl;
  return this->relations[name];
  
}

