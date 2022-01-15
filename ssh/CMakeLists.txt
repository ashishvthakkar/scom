add_compile_definitions(SSH_NO_CPP_EXCEPTIONS)
add_executable(ssh_demo
        ssh_initiator.cc
        ssh_requestor.cc
        ssh_demo.cc)
target_link_libraries(ssh_demo
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE ssh)
target_include_directories(ssh_demo
        PRIVATE ${CMAKE_BINARY_DIR}/include)         
# target_compile_definitions(ssh_demo 
#         PUBLIC SSH_NO_CPP_EXCEPTIONS)

add_executable(ssh_responder
        ssh_responder_app.cc
        ssh_responder.cc)
target_link_libraries(ssh_responder
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        # PRIVATE expressions TODO(ashish): evaluate
        PRIVATE ssh)
target_include_directories(ssh_responder
        PRIVATE ${CMAKE_BINARY_DIR}/include
        PRIVATE ${CMAKE_BINARY_DIR}/deps/experiments/s/expressions/) # TODO(ashish): update