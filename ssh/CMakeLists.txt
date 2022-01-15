add_executable(ssh_demo
        ssh_demo.cc)
target_link_libraries(ssh_demo
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE ssh)
target_include_directories(ssh_demo
        PRIVATE ${CMAKE_BINARY_DIR}/include)         

add_executable(ssh_responder
        ssh_responder.cc)
target_link_libraries(ssh_responder
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE protocol
        PRIVATE ssh)
target_include_directories(ssh_responder
        PRIVATE ${CMAKE_BINARY_DIR}/include)         