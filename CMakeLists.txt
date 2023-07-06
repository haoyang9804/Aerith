cmake_minimum_required(VERSION 3.13.4)
project(Aerith)

find_package(Clang REQUIRED)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
find_program(CLANG_FORMAT_EXE NAMES "clang-format" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,clang-analyzer-*")
set(CLANG_FORMAT_COMMAND "${CLANG_FORMAT_EXE}")
set(Clang_ROOT ${CLANG_CMAKE_DIR})

file(GLOB_RECURSE SRCS 
    src/*.cpp
)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(STD cxx_std_20)
add_library(${PROJECT_NAME}_lib SHARED ${SRCS})
target_compile_features(${PROJECT_NAME}_lib PRIVATE ${STD})
target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
set_target_properties(${PROJECT_NAME}_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

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

enable_testing()
add_subdirectory(tests)
add_test(NAME test COMMAND 
${CMAKE_CURRENT_BINARY_DIR}/tests/test_${PROJECT_NAME}
)