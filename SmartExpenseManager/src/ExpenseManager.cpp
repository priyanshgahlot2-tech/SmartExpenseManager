#include "ExpenseManager.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

using namespace std;

ExpenseManager::ExpenseManager(vector<Transaction> existingTransactions)
    : transactions_(move(existingTransactions)) {
    for (const auto& t : transactions_) {
        if (t.id() >= nextId_) nextId_ = t.id() + 1;
    }
}

int ExpenseManager::addTransaction(TransactionType type,
                                   double amount,
                                   const string& category,
                                   const string& month,
                                   const string& description) {
    Transaction transaction(nextId_, type, amount, category, month, description);
    transactions_.push_back(transaction);
    return nextId_++;
}

bool ExpenseManager::deleteTransaction(int id) {
    const auto it = find_if(
        transactions_.begin(), transactions_.end(),
        [id](const Transaction& t) { return t.id() == id; }
    );
    if (it == transactions_.end()) return false;
    transactions_.erase(it);
    return true;
}

vector<Transaction> ExpenseManager::filterByCategory(
    const string& category) const {
    vector<Transaction> result;
    for (const auto& t : transactions_) {
        if (t.category() == category) result.push_back(t);
    }
    return result;
}

MonthlySummary ExpenseManager::summaryForMonth(const string& month) const {
    if (!Transaction::isValidMonth(month)) {
        throw invalid_argument("Month must use YYYY-MM format.");
    }

    MonthlySummary summary;
    for (const auto& t : transactions_) {
        if (t.month() != month) continue;
        if (t.type() == TransactionType::Income) summary.income += t.amount();
        else summary.expenses += t.amount();
    }
    return summary;
}

double ExpenseManager::totalBalance() const {
    double balance = 0.0;
    for (const auto& t : transactions_) {
        if (t.type() == TransactionType::Income) balance += t.amount();
        else balance -= t.amount();
    }
    return balance;
}

const vector<Transaction>& ExpenseManager::allTransactions() const {
    return transactions_;
}
