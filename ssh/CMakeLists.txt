add_compile_definitions(SSH_NO_CPP_EXCEPTIONS)

add_library(ssh_io_lib 
        ssh_initiator.cc 
        ssh_requestor.cc 
        std_io_mgr.cc 
        ssh_responder.cc
        msg_io_mgr.cc)
target_link_libraries(ssh_io_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE ssh)
target_include_directories(ssh_io_lib
        PRIVATE ${CMAKE_BINARY_DIR}/include)

add_subdirectory(apps)

add_executable(ssh_tests ssh_tests.cc)
target_link_libraries(ssh_tests
        PRIVATE ssh_io_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE ssh
        PRIVATE GTest::gtest
        PRIVATE GTest::gtest_main)
target_include_directories(ssh_tests
        PRIVATE ${CMAKE_BINARY_DIR}/include)        
# Note: This is not automatically enabled as tests as it requires password for 
# ssh auth
# gtest_discover_tests(ssh_tests)