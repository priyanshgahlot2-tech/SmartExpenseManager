#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

enum class TransactionType {
    Income,
    Expense
};

class Transaction {
private:
    int id_;
    TransactionType type_;
    double amount_;
    string category_;
    string month_;       // YYYY-MM
    string description_;

public:
    Transaction(int id,
                TransactionType type,
                double amount,
                string category,
                string month,
                string description);

    int id() const;
    TransactionType type() const;
    double amount() const;
    const string& category() const;
    const string& month() const;
    const string& description() const;

    static bool isValidMonth(const string& month);
    static string typeToString(TransactionType type);
    static TransactionType typeFromString(const string& value);
};

#endif
