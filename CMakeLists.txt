cmake_minimum_required(VERSION 3.16)

project(LogicMinimizingTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LogicMinimizingTest main.cpp)

include(GNUInstallDirs)
install(TARGETS LogicMinimizingTest
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
