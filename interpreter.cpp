#include "interpreter.h"

Interpreter::Interpreter(shared_ptr<DatalogProgram> p) {
  this->program = p;
  this->database = make_shared<RelationalDatabase>();
}

void Interpreter::createDatabase() {
  this->addSchemes();
  this->addFacts();
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
    string relation_name = query->id->getContent();
    vector<shared_ptr<Parameter>> parameter_list = query->parameterList;
    
    shared_ptr<Relation> relation = this->database->getRelation(relation_name);
    if (relation == nullptr) continue;
    
    vector<string> parameter_values;
    for(auto parameter : parameter_list) {
      parameter_values.push_back(parameter->value->getContent());
    }
    
    shared_ptr<Relation> selected_relation = this->database->select(relation_name, parameter_values);
    this->database->addRelation("selected_relation", selected_relation);
    
    vector<int> column_indexes;
    for (unsigned int i = 0; i < parameter_list.size(); i++) {
      if(parameter_list[i]->type == ID) {
        column_indexes.push_back(i);
      }
    }
    //cout << "selected_relation before passed rows.size() " << selected_relation->rows.size() << endl;
    shared_ptr<Relation> projected_relation = this->database->project("selected_relation", column_indexes);
    this->database->addRelation("projected_relation", projected_relation);
    
    vector<string> scheme_parameters;
    for (auto parameter: parameter_list) {
      if (parameter->type == ID) {
        scheme_parameters.push_back(parameter->value->getContent());
      }
    }
    shared_ptr<Relation> renamed_relation = this->database->rename("projected_relation", scheme_parameters);
    this->database->addRelation("renamed_relation", renamed_relation);
    
    //cout << "selected rows.size() = " << selected_relation->rows.size() << endl;
    //cout << "projected rows.size() = " << projected_relation->rows.size() << endl;
    //cout << "renamed rows.size() = " << renamed_relation->rows.size() << endl << endl;
    
    output += query->toString() + "?";
    if (renamed_relation->rows.size() == 0 && selected_relation->rows.size() == 0) {
      output += " No\n";
    } else {
      stringstream ss;
      ss << selected_relation->rows.size();
      output += " Yes(" + ss.str() + ")\n";
    }
    
    if (renamed_relation->rows.size() > 0) {
      output += renamed_relation->toString();
    }
  }
  if (!output.empty()) {
    output.pop_back(); //remove last newline
  }
  
  return output;
}