# This copy is required. Folder code_pool contains
# all files that are ready to be analyzed or mutated.
# Without COPY, unit tests cannot find the path to codes.
file(COPY 
  ${CMAKE_CURRENT_SOURCE_DIR}/code_pool
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)

file(GLOB_RECURSE TESTS
  *.cpp
)

list(FILTER TESTS EXCLUDE REGEX "code_pool/.*")
list(FILTER TESTS EXCLUDE REGEX "test_DEV.cpp")

message(STATUS "google test cases include " ${TESTS})

add_executable(
  googletest_${PROJECT_NAME}
  ${TESTS}
)

target_compile_features(googletest_${PROJECT_NAME} PRIVATE ${STD})

target_include_directories(
  googletest_${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  googletest_${PROJECT_NAME}
  PRIVATE
  ${PROJECT_NAME}_lib
  GTest::gtest_main
)

if (DEV)
  add_executable(
    googletest_${PROJECT_NAME}_dev
    test_DEV.cpp
  )
  target_compile_features(
    googletest_${PROJECT_NAME}_dev
    PRIVATE
    ${STD}
  )
  target_include_directories(
    googletest_${PROJECT_NAME}_dev
    PRIVATE
    ${PROJECT_SOURCE_DIR}/DEV
  )
  target_link_libraries(
    googletest_${PROJECT_NAME}_dev
    PRIVATE
    ${PROJECT_NAME}_dev_lib
    GTest::gtest_main
  )
endif()

include(GoogleTest)

gtest_discover_tests(googletest_${PROJECT_NAME})

if (DEV)
  gtest_discover_tests(googletest_${PROJECT_NAME}_dev)
endif()