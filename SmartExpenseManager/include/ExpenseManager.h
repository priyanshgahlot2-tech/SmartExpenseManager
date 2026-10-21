#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include "Transaction.h"
#include <string>
#include <vector>

using namespace std;

struct MonthlySummary {
    double income = 0.0;
    double expenses = 0.0;
    double balance() const { return income - expenses; }
};

class ExpenseManager {
private:
    vector<Transaction> transactions_;
    int nextId_ = 1;

public:
    ExpenseManager() = default;
    explicit ExpenseManager(vector<Transaction> existingTransactions);

    int addTransaction(TransactionType type,
                      double amount,
                      const string& category,
                      const string& month,
                      const string& description);

    bool deleteTransaction(int id);
    vector<Transaction> filterByCategory(const string& category) const;
    MonthlySummary summaryForMonth(const string& month) const;
    double totalBalance() const;

    const vector<Transaction>& allTransactions() const;
};

#endif
