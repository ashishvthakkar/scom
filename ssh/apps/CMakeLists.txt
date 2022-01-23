add_compile_definitions(SSH_NO_CPP_EXCEPTIONS)

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

add_executable(ssh_responder ssh_responder_app.cc)
target_link_libraries(ssh_responder
        PRIVATE ssh_responder_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE expressions
        PRIVATE ssh)
target_include_directories(ssh_responder
        PRIVATE ${CMAKE_BINARY_DIR}/include
        PRIVATE ${CMAKE_BINARY_DIR}/_deps/experiments-src/) 

add_executable(ssh_echo_responder ssh_echo_responder_app.cc)
target_link_libraries(ssh_echo_responder
        PRIVATE ssh_responder_lib
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE expressions
        PRIVATE ssh)
target_include_directories(ssh_echo_responder
        PRIVATE ${CMAKE_BINARY_DIR}/include
        PRIVATE ${CMAKE_BINARY_DIR}/_deps/experiments-src/) 
