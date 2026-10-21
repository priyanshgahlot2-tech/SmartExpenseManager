#include "Transaction.h"

#include <stdexcept>
#include <utility>

using namespace std;

Transaction::Transaction(int id,
                         TransactionType type,
                         double amount,
                         string category,
                         string month,
                         string description)
    : id_(id),
      type_(type),
      amount_(amount),
      category_(move(category)),
      month_(move(month)),
      description_(move(description)) {
    if (id_ <= 0) {
        throw invalid_argument("Transaction ID must be positive.");
    }
    if (amount_ <= 0.0) {
        throw invalid_argument("Amount must be greater than zero.");
    }
    if (category_.empty()) {
        throw invalid_argument("Category cannot be empty.");
    }
    if (!isValidMonth(month_)) {
        throw invalid_argument("Month must use YYYY-MM format.");
    }
}

int Transaction::id() const { return id_; }
TransactionType Transaction::type() const { return type_; }
double Transaction::amount() const { return amount_; }
const string& Transaction::category() const { return category_; }
const string& Transaction::month() const { return month_; }
const string& Transaction::description() const { return description_; }

bool Transaction::isValidMonth(const string& month) {
    if (month.size() != 7 || month[4] != '-') {
        return false;
    }
    for (size_t i = 0; i < month.size(); ++i) {
        if (i == 4) continue;
        if (month[i] < '0' || month[i] > '9') return false;
    }
    const int year = (month[0] - '0') * 1000
                   + (month[1] - '0') * 100
                   + (month[2] - '0') * 10
                   + (month[3] - '0');
    const int monthNumber = (month[5] - '0') * 10 + (month[6] - '0');
    return year >= 1 && monthNumber >= 1 && monthNumber <= 12;
}

string Transaction::typeToString(TransactionType type) {
    return type == TransactionType::Income ? "INCOME" : "EXPENSE";
}

TransactionType Transaction::typeFromString(const string& value) {
    if (value == "INCOME") return TransactionType::Income;
    if (value == "EXPENSE") return TransactionType::Expense;
    throw invalid_argument("Unknown transaction type: " + value);
}
