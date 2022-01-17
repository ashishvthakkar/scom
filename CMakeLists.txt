cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)
project(scom VERSION 0.1.0)

# Specify settings for clang-tidy
string(REGEX REPLACE "[\\\\|/]" "[\\\\/]" HEADERS "${CMAKE_BINARY_DIR}/../.*")
set(CMAKE_CXX_CLANG_TIDY
        "clang-tidy"
        "--header-filter=${HEADERS}"
        "--export-fixes=${CMAKE_BINARY_DIR}/clang-tidy.fixes")

# kydeps based bootstrap
set(KYDEPS gflags glog GTest zlib protobuf OpenSSL libssh)
add_subdirectory(deps)
find_package(gflags REQUIRED NO_MODULE)
find_package(glog REQUIRED NO_MODULE)
find_package(GTest REQUIRED NO_MODULE)
find_package(zlib REQUIRED NO_MODULE)
find_package(protobuf REQUIRED NO_MODULE)
find_package(OpenSSL REQUIRED) # Note lack of NO_MODULE
find_package(libssh REQUIRED NO_MODULE)

include(FetchContent)
FetchContent_Declare(
  experiments
  GIT_REPOSITORY https://github.com/ashishvthakkar/experiments.git
  GIT_TAG 44a06c9da1cb2871c88abfbd7d0d2437adae429d # v20220117
  )
FetchContent_MakeAvailable(experiments)

# Set up testing
include(CTest)
enable_testing()
include(GoogleTest)

configure_file(config.h.in ${CMAKE_BINARY_DIR}/include/config.h)

add_subdirectory(tests)
add_subdirectory(temp_path)
add_subdirectory(protocol)
add_subdirectory(ssh)

add_executable(main main.cc)
target_link_libraries(main 
    PRIVATE gflags
    PRIVATE glog::glog
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
