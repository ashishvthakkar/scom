find_package(libssh REQUIRED NO_MODULE)

add_executable(ssh_exec
        ssh_exec.cc)
target_link_libraries(ssh_exec
        PRIVATE glog::glog
        PRIVATE ssh)