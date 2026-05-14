cmake_minimum_required(VERSION 3.16)

project(dayforge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(dayforge_core
    src/Entry.cpp
    src/Ledger.cpp
    src/Query.cpp
)

target_include_directories(dayforge_core PUBLIC include)

add_executable(dayforge src/main.cpp)
target_link_libraries(dayforge PRIVATE dayforge_core)

enable_testing()

add_executable(dayforge_tests tests/test_ledger.cpp)
target_link_libraries(dayforge_tests PRIVATE dayforge_core)

add_test(NAME dayforge_tests COMMAND dayforge_tests)
