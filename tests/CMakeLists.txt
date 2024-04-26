cmake_minimum_required(VERSION 3.5)
project(tests)

add_subdirectory(Catch2)

add_executable(tests option/option-value.cpp option/option-mutable-ref.cpp option/option-const-ref.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/../)

list(APPEND CMAKE_MODULE_PATH Catch2/extras)
include(Catch)
catch_discover_tests(tests)
