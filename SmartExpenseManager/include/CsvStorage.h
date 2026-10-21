#ifndef CSV_STORAGE_H
#define CSV_STORAGE_H

#include "Transaction.h"
#include <string>
#include <vector>

using namespace std;

class CsvStorage {
public:
    static vector<Transaction> load(const string& path);
    static void save(const string& path,
                     const vector<Transaction>& transactions);

private:
    static string escapeCsv(const string& value);
    static vector<string> parseCsvLine(const string& line);
};

#endif
