add_library(expressions operator_node.cc expression.cc utils.cc alg_terms.cc)
target_link_libraries(expressions
    PRIVATE glog::glog
    PRIVATE gflags)

add_executable(expressions_demo expressions_demo.cc)
target_link_libraries(expressions_demo
    PRIVATE expressions
    PRIVATE glog::glog
    PRIVATE gflags)

add_executable(expressions_tests expressions_tests.cc)
target_link_libraries(expressions_tests
    PRIVATE glog::glog
    PRIVATE gflags    
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main
    PRIVATE expressions)
gtest_discover_tests(expressions_tests)
