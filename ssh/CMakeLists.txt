add_executable(ssh_exec
        ssh_exec.cc)
target_link_libraries(ssh_exec
        PRIVATE glog::glog
        PRIVATE ssh)

add_executable(ssh_requestor
        ssh_requestor.cc)
target_link_libraries(ssh_requestor
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE ssh)

add_executable(ssh_responder
        ssh_responder.cc)
target_link_libraries(ssh_responder
        PRIVATE glog::glog
        PRIVATE gflags
        PRIVATE ssh)