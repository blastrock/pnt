set(CMAKE_CXX_FLAGS "-std=c++0x -O2 -g")

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_executable(bench
  bench.cpp
)
