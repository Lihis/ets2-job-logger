cmake_minimum_required(VERSION 3.18)

include(GoogleTest)

find_package(GTest REQUIRED)

add_executable(Json_test Json_test.cpp)
target_link_libraries(Json_test
                      PRIVATE
                      GTest::Main
                      GTest::GTest
                      ets2joblogger::serialization
                      )
gtest_discover_tests(Json_test
                     XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/results
                     DISCOVERY_TIMEOUT 60
                     )
