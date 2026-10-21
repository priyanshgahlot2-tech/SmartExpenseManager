# Smart Expense Manager (C++)

A command-line personal finance tracker built as a learning project in modern C++.
It demonstrates object-oriented design, STL containers, CSV file persistence,
input validation, and basic automated tests.

## Features

- Add income and expense transactions
- Store category, month (`YYYY-MM`), amount, and description
- List transactions with unique IDs
- Delete a transaction by ID
- Filter transactions by exact category
- Calculate monthly income, expenses, and net balance
- Calculate overall balance
- Automatically load and save transactions in `data/transactions.csv`
- Quote commas and double quotes correctly in CSV text fields

## Requirements

- A C++17 compiler (GCC, Clang, or a recent MSVC)
- CMake 3.16 or newer

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

Run the application from the project root so its relative `data/` path is predictable:

```bash
./build/expense_tracker
```

On Windows, the executable may be located at `build/Debug/expense_tracker.exe`
or `build/expense_tracker.exe`, depending on the generator.

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

Or run the `expense_tests` executable produced by the build.

## CSV format

The application creates `data/transactions.csv` with these columns:

```text
id,type,amount,category,month,description
```

The `data/` file is local application data and is intentionally ignored by Git.

## Example workflow

1. Choose `1` to add income or `2` to add an expense.
2. Enter a positive amount, category, month such as `2026-10`, and description.
3. Choose `3` to list records or `5` to filter by category.
4. Choose `6` to view a month's totals.
5. Choose `8` to save and exit.

## Project structure

```text
include/                  Public class declarations
  Transaction.h
  ExpenseManager.h
  CsvStorage.h
src/                      Implementations and command-line interface
  Transaction.cpp
  ExpenseManager.cpp
  CsvStorage.cpp
  main.cpp
tests/
  test_expense_manager.cpp
CMakeLists.txt
README.md
```

## Design notes

- `Transaction` represents one income or expense record and validates its core data.
- `ExpenseManager` owns the transaction collection and implements business operations.
- `CsvStorage` handles reading and writing CSV data, keeping file I/O separate from business logic.
- `main.cpp` handles the interactive menu and calls the other components.

## Current limitations / possible improvements

- Amounts use `double`; a production financial application should use integer minor units
  (such as paise) or a decimal money type to avoid floating-point rounding issues.
- Category filtering is an exact, case-sensitive match.
- Months are entered manually rather than derived from a full transaction date.
- CSV storage is suitable for this local learning project, not concurrent multi-user access.

## Future ideas

- Edit transactions
- Case-insensitive category search
- Export filtered reports
- Add a graphical interface
- Add more edge-case and persistence tests
- Add a database backend
