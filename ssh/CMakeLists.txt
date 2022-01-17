add_compile_definitions(SSH_NO_CPP_EXCEPTIONS)

add_library(ssh_requestor_lib ssh_initiator.cc ssh_requestor.cc)
target_link_libraries(ssh_requestor_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE ssh)
target_include_directories(ssh_requestor_lib
        PRIVATE ${CMAKE_BINARY_DIR}/include)

add_executable(ssh_demo ssh_demo.cc)
target_link_libraries(ssh_demo
        PRIVATE ssh_requestor_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE ssh)
target_include_directories(ssh_demo
        PRIVATE ${CMAKE_BINARY_DIR}/include)

add_executable(ssh_timed_demo ssh_timed_demo.cc)
target_link_libraries(ssh_timed_demo
        PRIVATE ssh_requestor_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE ssh)
target_include_directories(ssh_timed_demo
        PRIVATE ${CMAKE_BINARY_DIR}/include)

add_executable(ssh_timed_bw_demo ssh_timed_bw_demo.cc)
target_link_libraries(ssh_timed_bw_demo
        PRIVATE ssh_requestor_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE ssh)
target_include_directories(ssh_timed_bw_demo
        PRIVATE ${CMAKE_BINARY_DIR}/include)

add_executable(ssh_responder 
        ssh_responder_app.cc 
        ssh_responder.cc 
        std_io_mgr.cc)
target_link_libraries(ssh_responder
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE expressions
        PRIVATE ssh)
target_include_directories(ssh_responder
        PRIVATE ${CMAKE_BINARY_DIR}/include
        PRIVATE ${CMAKE_BINARY_DIR}/_deps/experiments-src/) 

add_executable(ssh_echo_responder 
        ssh_echo_responder_app.cc 
        ssh_responder.cc 
        std_io_mgr.cc)
target_link_libraries(ssh_echo_responder
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE expressions
        PRIVATE ssh)
target_include_directories(ssh_echo_responder
        PRIVATE ${CMAKE_BINARY_DIR}/include
        PRIVATE ${CMAKE_BINARY_DIR}/_deps/experiments-src/) 

add_executable(ssh_tests ssh_tests.cc)
target_link_libraries(ssh_tests
        PRIVATE ssh_requestor_lib
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