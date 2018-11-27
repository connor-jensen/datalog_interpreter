#pragma once

#include "namespaces.h"


class Relation {
  public:
    Relation(string, vector<string>);
    void addRow(vector<string>);
    string toString();
    string name;
    vector<string> schemeParameters;
    set<vector<string>> rows;
};