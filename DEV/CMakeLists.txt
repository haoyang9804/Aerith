file(GLOB_RECURSE SRCFILES *.cpp)

add_library(${PROJECT_NAME}_dev_lib ${SRCFILES})

target_include_directories(${PROJECT_NAME}_dev_lib PRIVATE ${PROJECT_SOURCE_DIR}/DEV)
set_target_properties(${PROJECT_NAME}_dev_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
target_link_libraries(${PROJECT_NAME}_dev_lib
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
)

target_link_libraries(${PROJECT_NAME}_dev_lib PUBLIC ${PROJECT_NAME}_lib)