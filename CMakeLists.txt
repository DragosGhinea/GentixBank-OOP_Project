cmake_minimum_required(VERSION 3.24)
project(GentixBankOOP)

set(CMAKE_CXX_STANDARD 17)

add_executable(GentixBankOOP src/main.cpp src/card.cpp src/account.cpp include/inputHandle.h src/menuLeaderboard.cpp include/menuLeaderboard.h src/menuMain.cpp src/menuProgressBar.cpp src/menuUser.cpp src/menuUtils.cpp src/controlPanel.cpp src/transactions.cpp src/user.cpp src/currency.cpp)
