#include "relation.h"

Relation::Relation(string relationName, vector<string> schemeParameters) {
  this->name = relationName;
  this->schemeParameters = schemeParameters;
}

void Relation::addRow(vector<string> row) {
  this->rows.insert(row);
}

string Relation::toString() {
  string output = "";
  for (auto row : rows) {
    for(unsigned int j = 0; j < row.size(); j++)
    {
      bool printColumn = true;
      for (unsigned int k = 0; k < j; k++) {
        if (this->schemeParameters[j] == this->schemeParameters[k]) {
          printColumn = false;
          break;
        }
      }
      if (!printColumn) continue;
      
      if (j > 0) {
        output += ", ";
      } else {
        output += "  ";
      }
      output += this->schemeParameters[j] + "=" + row[j];
    }
    output += "\n";
  }
  
  return output;
}