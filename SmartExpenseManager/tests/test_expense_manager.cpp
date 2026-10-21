#include "CsvStorage.h"
#include "ExpenseManager.h"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using namespace std;

int main() {
    ExpenseManager manager;

    const int incomeId = manager.addTransaction(
        TransactionType::Income, 5000.0, "Pocket Money", "2026-10", "Monthly allowance"
    );
    manager.addTransaction(
        TransactionType::Expense, 500.0, "Food", "2026-10", "Snacks"
    );
    manager.addTransaction(
        TransactionType::Expense, 200.0, "Travel", "2026-09", "Bus"
    );

    assert(incomeId == 1);
    assert(abs(manager.totalBalance() - 4300.0) < 0.001);

    const auto food = manager.filterByCategory("Food");
    assert(food.size() == 1);
    assert(food.front().amount() == 500.0);

    const auto october = manager.summaryForMonth("2026-10");
    assert(october.income == 5000.0);
    assert(october.expenses == 500.0);
    assert(october.balance() == 4500.0);

    assert(manager.deleteTransaction(2));
    assert(!manager.deleteTransaction(999));
    assert(abs(manager.totalBalance() - 4800.0) < 0.001);

    bool invalidMonthRejected = false;
    try {
        manager.summaryForMonth("2026-13");
    } catch (const invalid_argument&) {
        invalidMonthRejected = true;
    }
    assert(invalidMonthRejected);

    const string testPath = "test_transactions.csv";
    CsvStorage::save(testPath, manager.allTransactions());
    const auto loaded = CsvStorage::load(testPath);
    assert(loaded.size() == manager.allTransactions().size());
    assert(loaded[0].description() == "Monthly allowance");
    remove(testPath.c_str());

    cout << "All tests passed.\n";
    return 0;
}
