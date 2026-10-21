#include "CsvStorage.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

string CsvStorage::escapeCsv(const string& value) {
    bool needsQuotes = false;
    for (char c : value) {
        if (c == ',' || c == '"' || c == '\n' || c == '\r') {
            needsQuotes = true;
            break;
        }
    }
    if (!needsQuotes) return value;

    string result = "\"";
    for (char c : value) {
        if (c == '"') result += "\"\"";
        else result += c;
    }
    result += '"';
    return result;
}

vector<string> CsvStorage::parseCsvLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }

    if (inQuotes) {
        throw runtime_error("Malformed CSV: unmatched quote.");
    }
    fields.push_back(field);
    return fields;
}

vector<Transaction> CsvStorage::load(const string& path) {
    ifstream file(path);
    if (!file) {
        // A missing file is normal on the first run.
        return {};
    }

    vector<Transaction> transactions;
    string line;
    bool firstLine = true;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (firstLine && line == "id,type,amount,category,month,description") {
            firstLine = false;
            continue;
        }
        firstLine = false;

        const auto fields = parseCsvLine(line);
        if (fields.size() != 6) {
            throw runtime_error("Malformed CSV row: expected 6 columns.");
        }

        try {
            const int id = stoi(fields[0]);
            const auto type = Transaction::typeFromString(fields[1]);
            const double amount = stod(fields[2]);
            transactions.emplace_back(
                id, type, amount, fields[3], fields[4], fields[5]
            );
        } catch (const exception& e) {
            throw runtime_error(
                string("Could not load a transaction: ") + e.what()
            );
        }
    }

    return transactions;
}

void CsvStorage::save(const string& path,
                      const vector<Transaction>& transactions) {
    ofstream file(path, ios::trunc);
    if (!file) {
        throw runtime_error("Could not open file for writing: " + path);
    }

    file << "id,type,amount,category,month,description\n";
    file << fixed << setprecision(2);

    for (const auto& t : transactions) {
        file << t.id() << ','
             << Transaction::typeToString(t.type()) << ','
             << t.amount() << ','
             << escapeCsv(t.category()) << ','
             << t.month() << ','
             << escapeCsv(t.description()) << '\n';
    }

    if (!file) {
        throw runtime_error("An error occurred while saving transactions.");
    }
}
