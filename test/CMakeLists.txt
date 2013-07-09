set(CATCH_PATH "" CACHE PATH "Path to Catch framework")

find_path(
  CATCH_INCLUDE_DIR
  NAMES catch.hpp
  PATHS "${CATCH_PATH}"
  PATH_SUFFIXES include single_include
)

add_definitions(-std=c++0x -g)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${CATCH_INCLUDE_DIR}
)

add_executable(test
  test.cpp
)
