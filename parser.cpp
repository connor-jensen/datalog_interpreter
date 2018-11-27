#include "parser.h"
#include <iostream>
#include <memory>


using std::string;

Parser::Parser(vector<shared_ptr<Token>> t)
{
  tokens = t;
  datalogProgram = NULL;
}

Parser::~Parser(){
  
}

shared_ptr<DatalogProgram> Parser::datalogParsing() 
{
  if(datalogProgram != NULL)
  {
    //delete datalogProgram;
  }
  datalogProgram = make_shared<DatalogProgram>();
  try 
  {
    datalog();
    for (auto p : tokens) 
    {
      //delete p;
    }
  }
  catch (shared_ptr<Token> errorToken)
  {
    datalogProgram->getError(errorToken);
    for (unsigned int i = 1; i < tokens.size(); ++i)
    {
      //delete tokens[i];
    }
  }
  return datalogProgram;
}


void Parser::datalog()
{
  //cout << "test" << endl;
  consumeToken(SCHEMES);
  consumeToken(COLON);
  
  //cout << "success before schemes" << endl;
  scheme();
  //cout << "success before schemesList" << endl;
  schemeList();
  //cout << "success before FACTS" << endl;
  consumeToken(FACTS);
  //cout << "success before COLON" << endl;
  consumeToken(COLON);
  
  //cout << "success before factList" << endl;
  factList();
  
  consumeToken(RULES);
  consumeToken(COLON);
  
  //cout << "success before ruleList" << endl;
  ruleList();
  
  consumeToken(QUERIES);
  consumeToken(COLON);
  
  //cout << "success before query" << endl;
  
  query();
  queryList();
  
  consumeToken(END_OF_FILE);
  
  //cout << "success after query" << endl;
}

// example scheme: snap(S,N,A,P)
void Parser::scheme()
{
  shared_ptr<Predicate> scheme = make_shared<Predicate>(consumeToken(ID)); // snap
  datalogProgram->newScheme(scheme);
  consumeToken(LEFT_PAREN); // (
  scheme->newParameter(make_shared<Parameter>(consumeToken(ID))); // S,N,A,P
  
  idList(scheme);
  
  
  consumeToken(RIGHT_PAREN); // )
}

// recursively adds schemes until the end of schemes is reached
void Parser::schemeList()
{
  if(nextTokenType() == ID)
  {
    scheme();
    schemeList();
  }
}

// example fact: snap('12345','C. Brown','12 Apple','555-1234').
void Parser::fact()
{
  shared_ptr<Predicate> fact = make_shared<Predicate>(consumeToken(ID)); //snap
  datalogProgram->newFact(fact);
  consumeToken(LEFT_PAREN); // (
  shared_ptr<Token> stringToken = consumeToken(STRING);
  datalogProgram->newDomainValue(stringToken->getContent());
  fact->newParameter(make_shared<Parameter>(stringToken));
  stringList(fact);
  consumeToken(RIGHT_PAREN);
  consumeToken(PERIOD);// )
}

void Parser::factList()
{
  if(nextTokenType() == ID)
  {
    fact();
    factList();
  }
}

// example rule: HasSameAddress(X,Y) :- snap(A,X,B,C),snap(D,Y,B,(E+C)).
void Parser::rule()
{
  shared_ptr<Rule> rule = make_shared<Rule>();
  datalogProgram->newRule(rule);
  rule->setHeadPredicate(headPredicate());
  
  consumeToken(COLON_DASH);
  
  rule->addPredicate(predicate());
  predicateList(rule);
  
  consumeToken(PERIOD);
}

void Parser::ruleList()
{
  if (nextTokenType() == ID)
  {
    rule();
    ruleList();
  }
}

shared_ptr<Predicate> Parser::headPredicate()
{
  shared_ptr<Token> id = consumeToken(ID);
  shared_ptr<Predicate> headPredicate = make_shared<Predicate>(id);
  consumeToken(LEFT_PAREN);
  
  headPredicate->newParameter(make_shared<Parameter>(consumeToken(ID)));
  idList(headPredicate);
  consumeToken(RIGHT_PAREN);
  return headPredicate;
}

shared_ptr<Predicate> Parser::predicate()
{
  shared_ptr<Predicate> predicate =  make_shared<Predicate>(consumeToken(ID));
  
  consumeToken(LEFT_PAREN);
  
  predicate->newParameter(parameter());
  parameterList(predicate);
  
  consumeToken(RIGHT_PAREN);
  
  return predicate;
}

void Parser::predicateList(shared_ptr<Rule> rule) 
{
  if (nextTokenType() == COMMA)
  {
    consumeToken(COMMA);
    rule->addPredicate(predicate());
    predicateList(rule);
  }
}

shared_ptr<Parameter> Parser::parameter()
{
  if (nextTokenType() == STRING)
  {
    return make_shared<Parameter>(consumeToken(STRING));
  }
  else if (nextTokenType() == ID)
  {
    return make_shared<Parameter>(consumeToken(ID));
  }
  else return expression();
}

tokenType Parser::nextTokenType() 
{
  return tokens.front()->getType();
}

void Parser::parameterList(shared_ptr<Predicate> predicate)
{
  if (nextTokenType() == COMMA)
  {
    consumeToken(COMMA);
    predicate->newParameter(parameter());
    parameterList(predicate);
  }
}

shared_ptr<Parameter> Parser::expression()
{
  consumeToken(LEFT_PAREN);
  shared_ptr<Parameter> expression = make_shared<Parameter>(nullptr); //leak
  shared_ptr<Parameter> firstParameter = parameter();
  shared_ptr<Token> operatorToken = operatorGrammar();
  shared_ptr<Parameter> secondParameter = parameter();
  expression->expression(firstParameter, operatorToken, secondParameter);
  
  consumeToken(RIGHT_PAREN);
  
  return expression;
}

shared_ptr<Token> Parser::operatorGrammar()
{
  if (nextTokenType() == ADD)
  {
    return consumeToken(ADD);
  }
  else return consumeToken(MULTIPLY);
}

void Parser::query()
{
  shared_ptr<Predicate> query = predicate();
  datalogProgram->newQuery(query);
  
  consumeToken(Q_MARK);
}

void Parser::queryList()
{
  if (nextTokenType() == ID)
  {
    query();
    queryList();
  }
}

shared_ptr<Token> Parser::consumeToken(tokenType type)
{
  if (nextTokenType() != type)
  {
    throw tokens.front();
  }
  shared_ptr<Token> tmp = tokens.front();
  tokens.erase(tokens.begin());
  
  if (type != ID && type != STRING && type != ADD && type != MULTIPLY)
  {
    //delete tmp;
  }
  return tmp;
}

void Parser::stringList(shared_ptr<Predicate> predicate) {
  if (nextTokenType() == COMMA){
    consumeToken(COMMA);
    
    shared_ptr<Token> stringToken = consumeToken(STRING);
    datalogProgram->newDomainValue(stringToken->getContent());
    predicate->newParameter(make_shared<Parameter>(stringToken));
    stringList(predicate);
  }
}

void Parser::idList(shared_ptr<Predicate> predicate)
{
  if (nextTokenType() == COMMA)
  {
    consumeToken(COMMA);
    predicate->newParameter(make_shared<Parameter>(consumeToken(ID)));
    idList(predicate);
  }
}