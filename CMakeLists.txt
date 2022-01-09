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
set(KYDEPS gflags glog GTest)
add_subdirectory(deps)
find_package(gflags REQUIRED NO_MODULE)
find_package(glog REQUIRED NO_MODULE)
find_package(GTest REQUIRED NO_MODULE)

# Set up testing
include(CTest)
enable_testing()
include(GoogleTest)

add_subdirectory(tests)
        
add_executable(main main.cc)
target_link_libraries(main 
    PRIVATE gflags
    PRIVATE glog::glog
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
