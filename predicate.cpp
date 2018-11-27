#include "predicate.h"
#include <iostream>
#include <memory>

using std::string;


Predicate::Predicate(shared_ptr<Token> id2){
  id = id2;
}

Predicate::~Predicate(){
  //delete id;
  // for (unsigned int i=0; i < parameterList.size(); i++){
  //   delete parameterList[i];
  // }
}

void Predicate::newParameter(shared_ptr<Parameter> parameter){
  parameterList.push_back(parameter);
}

string Predicate::toString(){
  //cout << "inside predicate tostring" << endl;
  
  string output = "";
  output += this->id->getContent();
  output += "(" + parameterList[0]->toString(); 
  for (unsigned int i = 1; i < parameterList.size(); i++){
    //cout << "calling parameterList[i].toString() for i = " << i << endl;
    output += "," + parameterList[i]->toString();
  }
  output += ")";
  
  return output;
}