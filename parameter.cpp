#include "parameter.h"
#include <iostream>
#include <memory>

using std::string;
Parameter::Parameter(){}

Parameter::Parameter(shared_ptr<Token> token){
  value = NULL;
  firstParameter = NULL;
  lastParameter = NULL;
  operatorToken = NULL;
  if (token != nullptr) {
    terminal(token);
  }
}

Parameter::~Parameter(){
  if(value != NULL){
    //delete value;
  }
  if (firstParameter != NULL){
    //delete firstParameter;
  }
  if (lastParameter != NULL){
    //delete lastParameter;
  }
  if (operatorToken != NULL){
    //delete operatorToken;
  }
}

void Parameter::expression
    (shared_ptr<Parameter> firstParameter2,
    shared_ptr<Token> operatorToken2,
    shared_ptr<Parameter> lastParameter2){
  type = EXPRESSION;
  firstParameter = firstParameter2;
  lastParameter = lastParameter2;
  operatorToken = operatorToken2;
  
}

void Parameter::terminal(shared_ptr<Token> token){
  type = token->getType();
  value = token;
}

string Parameter::toString() {
  //cout << "inside parameter tostring" << endl;
  if (type == EXPRESSION){
    
    return "(" + firstParameter->toString() + operatorToken->getContent() + lastParameter->toString() + ")";
  }
  return value->getContent();
}