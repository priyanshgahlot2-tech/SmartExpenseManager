#include "CsvStorage.h"
#include "ExpenseManager.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;
namespace fs = std::filesystem;

namespace {
const string DATA_PATH = "data/transactions.csv";

void printTransaction(const Transaction& t) {
    cout << '#' << t.id() << " | "
         << Transaction::typeToString(t.type()) << " | "
         << fixed << setprecision(2) << t.amount() << " | "
         << t.category() << " | " << t.month() << " | "
         << t.description() << '\n';
}

double readAmount() {
    double amount;
    while (true) {
        cout << "Amount: ";
        if (cin >> amount && amount > 0.0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return amount;
        }
        cout << "Enter a number greater than zero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin >> ws, value);
    return value;
}

void saveManager(const ExpenseManager& manager) {
    fs::create_directories("data");
    CsvStorage::save(DATA_PATH, manager.allTransactions());
}
}

int main() {
    try {
        fs::create_directories("data");
        ExpenseManager manager(CsvStorage::load(DATA_PATH));

        cout << "Loaded " << manager.allTransactions().size()
             << " transaction(s).\n";

        bool running = true;
        while (running) {
            cout << "\n=== SMART EXPENSE TRACKER ===\n"
                 << "1. Add income\n"
                 << "2. Add expense\n"
                 << "3. Show all transactions\n"
                 << "4. Delete transaction\n"
                 << "5. Filter by category\n"
                 << "6. Monthly summary\n"
                 << "7. Show overall balance\n"
                 << "8. Save and exit\n"
                 << "Choice: ";

            int choice;
            if (!(cin >> choice)) {
                cout << "Please enter a menu number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            try {
                if (choice == 1 || choice == 2) {
                    const auto type = choice == 1
                        ? TransactionType::Income : TransactionType::Expense;
                    const double amount = readAmount();
                    const string category = readLine("Category: ");
                    const string month = readLine("Month (YYYY-MM): ");
                    const string description = readLine("Description: ");

                    const int id = manager.addTransaction(
                        type, amount, category, month, description
                    );
                    saveManager(manager);
                    cout << "Added transaction #" << id << ".\n";
                } else if (choice == 3) {
                    if (manager.allTransactions().empty()) {
                        cout << "No transactions yet.\n";
                    }
                    for (const auto& t : manager.allTransactions()) printTransaction(t);
                } else if (choice == 4) {
                    int id;
                    cout << "Transaction ID to delete: ";
                    if (!(cin >> id)) {
                        cout << "Invalid ID.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (manager.deleteTransaction(id)) {
                        saveManager(manager);
                        cout << "Transaction deleted.\n";
                    } else {
                        cout << "No transaction found with that ID.\n";
                    }
                } else if (choice == 5) {
                    const string category = readLine("Category to find: ");
                    const auto matches = manager.filterByCategory(category);
                    for (const auto& t : matches) printTransaction(t);
                    if (matches.empty()) cout << "No matches found.\n";
                } else if (choice == 6) {
                    const string month = readLine("Month (YYYY-MM): ");
                    const auto summary = manager.summaryForMonth(month);
                    cout << fixed << setprecision(2)
                         << "Income:   " << summary.income << '\n'
                         << "Expenses: " << summary.expenses << '\n'
                         << "Net:      " << summary.balance() << '\n';
                } else if (choice == 7) {
                    cout << fixed << setprecision(2)
                         << "Overall balance: " << manager.totalBalance() << '\n';
                } else if (choice == 8) {
                    saveManager(manager);
                    cout << "Saved. Goodbye!\n";
                    running = false;
                } else {
                    cout << "Choose a number from 1 to 8.\n";
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << '\n';
            }
        }
    } catch (const exception& e) {
        cerr << "Startup error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
