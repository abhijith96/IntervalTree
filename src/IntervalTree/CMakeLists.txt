
add_subdirectory(Test)
add_library(IntervalTree IntervalTree.h IntervalTreeNode.cpp IntervalTree.cpp IntervalTreeNode.h)

add_executable(TestIntervalTree TestIntervalTree.cpp)