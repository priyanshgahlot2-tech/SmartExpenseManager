cmake_minimum_required(VERSION 3.16)
project(SmartExpenseManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(expense_core
    src/Transaction.cpp
    src/CsvStorage.cpp
    src/ExpenseManager.cpp
)
target_include_directories(expense_core PUBLIC include)

add_executable(expense_tracker src/main.cpp)
target_link_libraries(expense_tracker PRIVATE expense_core)

enable_testing()
add_executable(expense_tests tests/test_expense_manager.cpp)
target_link_libraries(expense_tests PRIVATE expense_core)
add_test(NAME expense_manager_tests COMMAND expense_tests)
