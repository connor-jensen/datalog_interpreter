#include "interpreter.h"

Interpreter::Interpreter(shared_ptr<DatalogProgram> p) {
  this->program = p;
  this->database = make_shared<RelationalDatabase>();
}

void Interpreter::createDatabase() {
  this->addSchemes();
  this->addFacts();
  this->runRules();
}

void Interpreter::addSchemes() {
  vector<shared_ptr<Predicate>> schemes = program->schemes;
  
  for (auto scheme : schemes) {
    string relation_name = scheme->id->getContent();
    vector<string> relation_scheme;
    
    vector<shared_ptr<Parameter>> parameter_list = scheme->parameterList;
    for (auto parameter : parameter_list) {
      relation_scheme.push_back(parameter->value->getContent());
    }
    
    shared_ptr<Relation> relation = make_shared<Relation>(relation_name, relation_scheme);
    
    this->database->addRelation(relation_name, relation);
  }
}

void Interpreter::addFacts() {
  vector<shared_ptr<Predicate>> facts = program->facts;
  
  for (auto fact : facts) {
    string relation_name = fact->id->getContent();
    
    shared_ptr<Relation> relation = this->database->getRelation(relation_name);
    
    if (relation == nullptr) continue;
    
    vector<string> relation_row;
    
    vector<shared_ptr<Parameter>> parameter_list = fact->parameterList;
    for (auto parameter : parameter_list) {
      relation_row.push_back(parameter->value->getContent());
    }
    
    relation->addRow(relation_row);
  }
}

string Interpreter::runQueries() {
  string output = "";
  
  vector<shared_ptr<Predicate>> queries = program->queries;
  
  for (auto query : queries) {
    string relationName = query->id->getContent();

    shared_ptr<Relation> relation = this->database->getRelation(relationName);
    if (relation == nullptr) continue;

    std::pair<unsigned int, shared_ptr<Relation>> queryResult = this->runQuery(relation, query);
    unsigned int selectSize = std::get<0>(queryResult);
    shared_ptr<Relation> result = std::get<1>(queryResult);
    
    output += query->toString() + "?";
    if (selectSize == 0) {
      output += " No\n";
    }
    else {
      stringstream ss;
      ss << selectSize;
      output += " Yes(" + ss.str() + ")\n";
    }
    if (result->rows.size() > 0) {
      output += result->toString();
    }

    
    // vector<string> parameter_values;
    // for(auto parameter : parameter_list) {
    //   parameter_values.push_back(parameter->value->getContent());
    // }
    
    // shared_ptr<Relation> selected_relation = this->database->select(relation, parameter_values);
    // this->database->addRelation("selected_relation", selected_relation);
    
    // vector<int> column_indexes;
    // for (unsigned int i = 0; i < parameter_list.size(); i++) {
    //   if(parameter_list[i]->type == ID) {
    //     column_indexes.push_back(i);
    //   }
    // }

    // shared_ptr<Relation> projected_relation = this->database->project(selected_relation, column_indexes);
    // this->database->addRelation("projected_relation", projected_relation);
    
    // vector<string> scheme_parameters;
    // for (auto parameter: parameter_list) {
    //   if (parameter->type == ID) {
    //     scheme_parameters.push_back(parameter->value->getContent());
    //   }
    // }
    // shared_ptr<Relation> renamed_relation = this->database->rename(projected_relation, scheme_parameters);
    // this->database->addRelation("renamed_relation", renamed_relation);
    
    // output += query->toString() + "?";
    // if (renamed_relation->rows.size() == 0 && selected_relation->rows.size() == 0) {
    //   output += " No\n";
    // } else {
    //   stringstream ss;
    //   ss << selected_relation->rows.size();
    //   output += " Yes(" + ss.str() + ")\n";
    // }
    
    // if (renamed_relation->rows.size() > 0) {
    //   output += renamed_relation->toString();
    // }
  }
  // if newline at end is a problem:
  if (!output.empty()) {
    output.pop_back();
  }
  
  return output;
}

void Interpreter::runRules() {
  std::vector<shared_ptr<Rule>> rules = this->program->rules;

  int passes = 0;
  bool change;
  // while there is change, keep going
   do {
    change = false;
    for (auto r : rules) {
      // change = false;
      if (runRule(r)) {
        change = true;
      }
    }
    passes++;
  } while(change);

  std::cout << "Schemes populated after " << passes << " passes through the Rules." << std::endl;
}

bool Interpreter::runRule(shared_ptr<Rule> r) {
  shared_ptr<Predicate> headPredicate = r->headPredicate;
  std::vector<shared_ptr<Predicate>> predicateList = r->predicateList;

  shared_ptr<Relation> joinResult = nullptr;
  for (auto p : predicateList) {
    std::string relationName = p->id->getContent();
    shared_ptr<Relation> operand = this->database->getRelation(relationName);

    if (operand == nullptr){
      continue;
    }

    std::pair<unsigned int, shared_ptr<Relation>> queryResult = this->runQuery(operand, p);

    if (joinResult == nullptr){
      joinResult = std::get<1>(queryResult);
    }
    else {
      joinResult = database->join(joinResult, std::get<1>(queryResult));

    }
  }

  if (joinResult == nullptr){
    return false;
  } 
  
  vector<shared_ptr<Parameter>>paramList = headPredicate->parameterList;
  vector<string> newScheme;
  
  for (auto p : paramList){
      if (p->type == ID){
        newScheme.push_back(p->value->getContent());
      }
  }

  shared_ptr<Relation> projectResult = database->project(joinResult, newScheme);

  string relationName = headPredicate->id->getContent();
  shared_ptr<Relation> originalRelation = database->getRelation(relationName);
  if (originalRelation == nullptr){
    cout << "uninitialized relation; will result in segfault" << endl;
  }
  shared_ptr<Relation> renameResult = database->rename(projectResult, originalRelation->schemeParameters);

  unsigned int sizeBefore = 0;
  
  if (originalRelation != nullptr){
    sizeBefore = originalRelation->rows.size();
  }

  shared_ptr<Relation>unionResult = database->relationUnion(originalRelation, renameResult);
  database->addRelation(relationName, unionResult);

  bool changed = (unionResult->rows.size() != sizeBefore);
  return changed;



}

std::pair<unsigned int, shared_ptr<Relation>> Interpreter::runQuery(shared_ptr<Relation> r, shared_ptr<Predicate> p) {
  bool testError = false;
  std::vector<shared_ptr<Parameter>> parameterList = p->parameterList;
  std::vector<std::string> values;

  for (auto param : parameterList) {
    values.push_back(param->value->getContent());
  }

  shared_ptr<Relation> selectResult = this->database->select(r, values);
  if (selectResult->schemeParameters.size() == 0) {
    testError = true;
  }
  
  std::vector<std::string> newScheme;
  for (unsigned int i = 0; i < parameterList.size(); i++) {
    if (parameterList[i]->type == ID) {
      newScheme.push_back(r->schemeParameters[i]);
    }
  }
  shared_ptr<Relation> projectResult = this->database->project(selectResult, newScheme);
  if (projectResult->schemeParameters.size() == 0) {
    testError = true;
  }

  std::vector<std::string> scheme;
  for (auto param : parameterList) {
    if (param->type == ID) {
      scheme.push_back(param->value->getContent());
    }
  }
  shared_ptr<Relation> renameResult = this->database->rename(projectResult, scheme);
  if (renameResult->schemeParameters.size() == 0) {
    testError = true;
  }

  return std::pair<unsigned int, shared_ptr<Relation>> (selectResult->rows.size(), renameResult);
}