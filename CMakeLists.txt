cmake_minimum_required(VERSION 3.13.4)
project(Arthur)

find_package(Clang REQUIRED)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
find_program(CLANG_FORMAT_EXE NAMES "clang-format" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,clang-analyzer-*")
set(CLANG_FORMAT_COMMAND "${CLANG_FORMAT_EXE}")

message(STATUS "ClangConfig.cmake: ${Clang_DIR}/ClangConfig.cmake")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include(AddLLVM)
include(${Clang_DIR}/AddClang.cmake)


option(DEV "Examples for further development" ON)


file(GLOB_RECURSE SRCS 
    src/*.cpp
)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
add_library(${PROJECT_NAME}_lib SHARED ${SRCS})
target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
if(DEV)
  set_target_properties(${PROJECT_NAME}_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

target_link_libraries(${PROJECT_NAME}_lib
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
)

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/*.[ch]pp
)

add_custom_target(
    format 
    COMMAND ${CLANG_FORMAT_COMMAND}
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
)

# From Jul/25/2023, I used GOOGLE TEST instead of a custom unit test framework.

# enable_testing()
# add_subdirectory(tests)
# add_test(NAME test COMMAND 
# ${CMAKE_CURRENT_BINARY_DIR}/tests/test_${PROJECT_NAME}
# )

if(DEV)
  add_subdirectory(DEV)
endif()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_subdirectory(tests)