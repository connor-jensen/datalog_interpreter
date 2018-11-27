#include "datalogProgram.h"
#include <iostream>
#include <memory>

using std::string;

DatalogProgram::DatalogProgram(){
  errorToken = NULL;
}

DatalogProgram::~DatalogProgram(){
  for (unsigned int i = 0; i < rules.size(); i++){
    
    //delete this->rules[i];
    
  }
   for (unsigned int i = 0; i < facts.size(); i++){
     
    //delete this->facts[i];
    
  }
  for (unsigned int i=0; i < queries.size(); i++){
    
    //delete this->queries[i];
    
  }
  for (unsigned int i = 0; i < schemes.size(); i++){
    
    //delete this->schemes[i];
    
  }
  
  //delete errorToken;
  
}


void DatalogProgram::newFact(shared_ptr<Predicate> fact){
  facts.push_back(fact);
}
void DatalogProgram::newRule(shared_ptr<Rule> rule){
  rules.push_back(rule);
}
void DatalogProgram::newQuery(shared_ptr<Predicate> query){
  queries.push_back(query);
}
void DatalogProgram::newScheme(shared_ptr<Predicate> scheme){
  schemes.push_back(scheme);
}

void DatalogProgram::newDomainValue(string domainValue){
  bool found = false;
  for (unsigned int i = 0; i < domain.size(); i++){
    if (domain[i] == domainValue) {
      found = true;
      break;
    }
  }
  if (!found) domain.push_back(domainValue);
}

void DatalogProgram::getError(shared_ptr<Token> t){
  errorToken = t;
}

string DatalogProgram::toString(){
  
  //cout << "inside datalogProgram.toString()" << endl;
  
  if (errorToken != NULL){
    return "Failure!\n  " + errorToken->toString() + "\n";
  } 
  
  std::stringstream ss;
  string output = "Success!\n";
  
  // prints out schemes
  
  ss << schemes.size();
  output += "Schemes(" + ss.str() + "):\n";
  ss.str("");
  for (unsigned int i = 0; i < schemes.size(); i++){
    output += "  " + schemes[i]->toString() + "\n";
  }
  
  //prints out facts
  
  ss << facts.size();
	output += "Facts(" + ss.str() + "):\n";
	ss.str("");
	for (unsigned int i = 0; i < facts.size(); i++) {
		output += "  " + facts[i]->toString() + ".\n";
	}
	
	//prints out rules
	
	ss << rules.size(); 
	
	output += "Rules(" + ss.str() + "):\n";
	
	ss.str("");
	for (unsigned int i = 0; i < rules.size(); i++) {
	 //cout << "calling rules[i].tostring()  on i = " << i << endl;  	
		output += "  " + rules[i]->toString() + ".\n"; //in rules to string
	}
	//prints out queries
	
	ss << queries.size();
	output += "Queries(" + ss.str() + "):\n";
	ss.str("");
	for (unsigned int i = 0; i < queries.size(); i++) {
		output += "  " + queries[i]->toString() + "?\n";
	}
	//cout << "test point 4" << endl;
	//prints out domains
	
	sortDomain();
	ss << domain.size();
	output += "Domain(" + ss.str() + "):";
	for (unsigned int i = 0; i < domain.size(); i++) {
		output += "\n  " + domain[i];
	}
	output += "\n";
	//cout << "test point 5" << endl;
	
	return output;

}

void DatalogProgram::sortDomain(){
  sort(domain.begin(), domain.end());
}