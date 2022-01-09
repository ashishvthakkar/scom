file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/scom)

protobuf_generate(
        PROTOS spec.proto
        OUT_VAR PROTO
        LANGUAGE cpp
        PROTOC_OUT_DIR ${CMAKE_BINARY_DIR}/include/scom)

add_library(protocol
        protocol_adapter.cc
        ${PROTO})
target_link_libraries(protocol
        PRIVATE glog::glog
        PRIVATE ${Protobuf_LIBRARIES}
        PRIVATE protobuf::libprotobuf-lite
        PRIVATE protobuf::libprotobuf)
target_include_directories(protocol
        PRIVATE ${CMAKE_BINARY_DIR}/include)
set_target_properties(protocol PROPERTIES CXX_CLANG_TIDY "")

add_executable(protocol_tests protocol_tests.cc)
target_link_libraries(protocol_tests
        PRIVATE glog::glog
        PRIVATE GTest::gtest
        PRIVATE GTest::gtest_main
        PRIVATE protocol
        PRIVATE temp_path)
target_include_directories(protocol_tests
        PRIVATE ${CMAKE_BINARY_DIR}/include)        
gtest_discover_tests(protocol_tests)