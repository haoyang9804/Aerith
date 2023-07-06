file(GLOB_RECURSE TESTS
  *.cpp
)

message("test files include " ${TESTS})

add_executable(test_${PROJECT_NAME} ${TESTS})
target_compile_features(test_${PROJECT_NAME} PRIVATE ${STD})
target_include_directories(test_${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/tests)
target_link_libraries(test_${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)