#include "rule.h"

#include <iostream>
#include <memory>

using std::string;

Rule::Rule()
{
  headPredicate = NULL;
}

Rule::~Rule()
{
  //delete this->headPredicate;
  // for (unsigned int i = 0; i < this->predicateList.size(); i++)
  // {
  //   delete this->predicateList[i];
  // }
}

void Rule::setHeadPredicate(shared_ptr<Predicate> p)
{
  headPredicate = p;
}

void Rule::addPredicate(shared_ptr<Predicate> p)
{
  //cout << "Adding predicate p to rule.predicateList<" << predicateList.size() << ">: " << p->toString() << endl;
  predicateList.push_back(p);
}

string Rule::toString()
{
 // cout << "inside rule.toString()" << endl;
  string output = "";
  output += this->headPredicate->toString() + " :- " + this->predicateList[0]->toString();
  //cout << "predicatList in 'rule' is size: " << predicateList.size() << endl;
  for (unsigned int i = 1; i < this->predicateList.size(); i++)
  {
    //cout << "from rule.cpp calling predicateList[i].toString() for i = " << i << endl;
    output += "," + predicateList[i]->toString();
  }
  return output;
}