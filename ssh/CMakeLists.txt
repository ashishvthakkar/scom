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
# target_compile_definitions(ssh_demo 
#         PUBLIC SSH_NO_CPP_EXCEPTIONS)

add_executable(ssh_responder 
        ssh_responder_app.cc 
        ssh_responder.cc 
        std_io_mgr.cc)
target_link_libraries(ssh_responder
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE expressions # TODO(ashish): change to package
        PRIVATE ssh)
target_include_directories(ssh_responder
        PRIVATE ${CMAKE_BINARY_DIR}/include
        # PRIVATE ${CMAKE_BINARY_DIR}/deps/experiments/s/expressions/ # TODO(ashish): update
        ) 

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
# Note: This is not automatically enabled as tests as it requires password for ssh connection
# gtest_discover_tests(ssh_tests)