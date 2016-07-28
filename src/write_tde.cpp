// rtableau: Rcpp bindings for the tableau SDK
//
// Copyright (C) 2016 Mark Hayden
//
// This file is part of rtableau.
//
// rtableau is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// rtableau is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with rtableau.  If not, see <http://www.gnu.org/licenses/>.

#include <Rcpp.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include "../inst/include/TableauCommon_cpp.h"
#include "../inst/include/TableauExtract_cpp.h"

// Helper function to map to internal Tableau types
Tableau::Type GetTableauType(std::string type) {
  if (type == "CharString") return Tableau::Type_CharString;
  else if (type == "Double") return Tableau::Type_Double;
  else if (type == "Integer") return Tableau::Type_Integer;
  else if (type == "Boolean") return Tableau::Type_Boolean;
  else if (type == "Date") return Tableau::Type_Date;
  else if (type == "DateTime") return Tableau::Type_DateTime;
  else {
    std::wcerr << "Error: not a valid type: " << type.c_str() << " (allowed: CharString, Double, Integer, Boolean, Date, DateTime)" << std::endl;
    return Tableau::Type_CharString;
  }
}

// [[Rcpp::export(name = ".WriteTDE")]]
void WriteTDE(Rcpp::DataFrame df, std::vector<std::string> coltypes, std::wstring filename) {
  Rcpp::CharacterVector colNames = df.names();
  try {
    Tableau::Extract extract(filename);
    std::shared_ptr<Tableau::Table> table;

    if (!extract.HasTable(L"Extract")) {
      // create a new table
      Tableau::TableDefinition tableDef;

      // define the schema
      tableDef.SetDefaultCollation(Tableau::Collation_en_GB);
      for(int i=0; i < df.length(); ++i) {
        tableDef.AddColumn(Rcpp::as<std::wstring>(colNames[i]), GetTableauType(coltypes[i]));
      }
      table = extract.AddTable(L"Extract", tableDef);
    }
    else {
      // open an existing table to add more rows
      table = extract.OpenTable(L"Extract");
    }

    std::shared_ptr<Tableau::TableDefinition> tableDef = table->GetTableDefinition();
    Tableau::Row row(*tableDef);

    // the Tableau SDK requires us to loop rows, so we set up vectors of vectors for each type
    std::vector<Rcpp::CharacterVector> stringCols;
    std::vector<Rcpp::IntegerVector> integerCols;
    std::vector<Rcpp::NumericVector> doubleCols;
    std::vector<Rcpp::LogicalVector> booleanCols;
    std::vector<Rcpp::NumericVector> dateCols; // numeric representation allows for better NA handling
    std::vector<Rcpp::NumericVector> datetimeCols; // numeric representation allows for better NA handling

    // these are useful for the insert functions
    std::vector<int> stringColNums;
    std::vector<int> integerColNums;
    std::vector<int> doubleColNums;
    std::vector<int> booleanColNums;
    std::vector<int> dateColNums;
    std::vector<int> datetimeColNums;

    // organize data frame columns. TODO: find more efficient way of doing this
    for(int j=0; j < df.length(); ++j) {
      std::string curColType = coltypes[j];
      if (curColType == "CharString") {
        stringCols.push_back(df[j]);
        stringColNums.push_back(j);
      }
      else if (curColType == "Double") {
        doubleCols.push_back(df[j]);
        doubleColNums.push_back(j);
      }
      else if (curColType == "Integer") {
        integerCols.push_back(df[j]);
        integerColNums.push_back(j);
      }
      else if (curColType == "Boolean") {
        booleanCols.push_back(df[j]);
        booleanColNums.push_back(j);
      }
      else if (curColType == "Date") {
        dateCols.push_back(df[j]);
        dateColNums.push_back(j);
      }
      else if (curColType == "DateTime") {
        datetimeCols.push_back(df[j]);
        datetimeColNums.push_back(j);
      }
    }

    // build a tableaur row, note that columns are inserted by type instead of order
    for (int i = 0; i < df.nrows(); ++i) {
      // insert string columns
      for (int stringnum = 0; stringnum < stringColNums.size(); ++stringnum) {
        if (stringCols[stringnum][i] == NA_STRING) {
          row.SetNull(stringColNums[stringnum]);
        }
        else {
          row.SetCharString(stringColNums[stringnum], Rcpp::as<std::string>(stringCols[stringnum][i]));
        }
      }

      // insert double columns. NA_REAL was not working for me, so switched to ISNAN for now
      for (int doublenum = 0; doublenum < doubleColNums.size(); ++doublenum) {
        if (ISNAN(doubleCols[doublenum][i])) {
          row.SetNull(doubleColNums[doublenum]);
        }
        else {
          row.SetDouble(doubleColNums[doublenum],doubleCols[doublenum][i]);
        }
      }

      // insert integer columns
      for (int integernum = 0; integernum < integerColNums.size(); ++integernum) {
        if (integerCols[integernum][i] == NA_INTEGER) {
          row.SetNull(integerColNums[integernum]);
        }
        else {
          row.SetInteger(integerColNums[integernum], integerCols[integernum][i]);
        }
      }

      // insert boolean columns
      for (int booleannum = 0; booleannum < booleanColNums.size(); ++booleannum) {
        if (booleanCols[booleannum][i] == NA_LOGICAL) {
          row.SetNull(booleanColNums[booleannum]);
        }
        else {
          row.SetBoolean(booleanColNums[booleannum], booleanCols[booleannum][i]);
        }
      }

      // insert date columns
      for (int datenum = 0; datenum < dateColNums.size(); ++datenum) {
        if (ISNAN(dateCols[datenum][i])) {
          row.SetNull(dateColNums[datenum]);
        }
        else {
          Rcpp::Date myDate = dateCols[datenum][i];
          row.SetDate(dateColNums[datenum], myDate.getYear(), myDate.getMonth(), myDate.getDay());
        }
      }

      // insert datetime columns
      for (int datetimenum = 0; datetimenum < datetimeColNums.size(); ++datetimenum) {
        if (ISNAN(datetimeCols[datetimenum][i])) {
          row.SetNull(datetimeColNums[datetimenum]);
        }
        else {
          Rcpp::Datetime myDatetime = datetimeCols[datetimenum][i];
          row.SetDateTime(datetimeColNums[datetimenum], myDatetime.getYear(), myDatetime.getMonth(), myDatetime.getDay(),
            myDatetime.getHours(), myDatetime.getMinutes(), myDatetime.getSeconds(), myDatetime.getMicroSeconds()/100);
        }
      }
      // push the row into the extract
      table->Insert(row);
    }
  }
  catch (const Tableau::TableauException& e) {
    std::wcerr << "Something bad happened: " << e.GetMessage() << std::endl;
    return;
  }
}
