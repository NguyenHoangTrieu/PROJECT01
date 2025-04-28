cmake_minimum_required(VERSION 3.20)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Fetch Google test from github:
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# Build Test:
project(G_TestFile)

enable_testing()

add_executable(UnitTest
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/MessageReadWrite.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Src/Calculate_Function.cpp
        UnitTest-Gtest.cpp)
        
target_include_directories(UnitTest PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../HEAT_Routing/Inc/FunctionHeader
        ${CMAKE_CURRENT_LIST_DIR}/../Examples/Node01
        )

target_link_libraries(UnitTest gtest gtest_main)
add_test(NAME ReadTest COMMAND UnitTest)
add_test(NAME WriteTest COMMAND UnitTest)
add_test(NAME CalculateTest COMMAND UnitTest)
