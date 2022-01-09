add_executable(
  basic_tests
  basic_tests.cc
)
target_link_libraries(
  basic_tests
  PRIVATE GTest::gtest
  PRIVATE GTest::gtest_main
  PRIVATE glog::glog  
)
gtest_discover_tests(basic_tests)