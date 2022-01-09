# ==============================================================================
# ===== [BEGIN] KyDeps CMakeLists.txt preamble ===== !!! DO NOT EDIT !!! =======
#
cmake_minimum_required(VERSION 3.20)
project(kydeps)

set(DEPS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
if (NOT (CMAKE_SOURCE_DIR STREQUAL DEPS_SOURCE_DIR))
    include(FetchContent)
    FetchContent_Declare(kydep_bootstrap
        GIT_REPOSITORY https://github.com/kyotov/kydep-bootstrap.git
        GIT_TAG rejigger # TODO(ashish): Update to specific commit tag
    )
    FetchContent_MakeAvailable(kydep_bootstrap)
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" PARENT_SCOPE)
    return()
endif()

include(kydep)
#
# ===== [END] KyDeps CMakeLists.txt preamble ===================================
# ==============================================================================

KyDep(gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG f8a0efe03aa69b3336d8e228b37d4ccb17324b88 # v2.2.0

    CMAKE_ARGS
    -DREGISTER_INSTALL_PREFIX=FALSE
    -DGFLAGS_BUILD_STATIC_LIBS=TRUE
    -DGFLAGS_BUILD_SHARED_LIBS=FALSE
    -DGFLAGS_BUILD_TESTING=FALSE    
)

KyDep(glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG 96a2f23dca4cc7180821ca5f32e526314395d26a # v0.4.0

    CMAKE_ARGS
    -DBUILD_TESTING=FALSE

    ${DEPENDS_ON_gflags}
)

# Fetch googletest
KyDep(
  GTest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 # v1.11.0
)
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

KyDep(
    zlib
    GIT_REPOSITORY https://github.com/kyotov/zlib.git
    GIT_TAG 68cf39d20a32cf9659e62b10f95fccc0575b0b31 # kyotov-fix

    CMAKE_ARGS
    -DAMD64=TRUE)

KyDep(
    protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG 436bd7880e458532901c58f4d9d1ea23fa7edd52 # v3.15.8

    SOURCE_SUBDIR cmake

    CMAKE_ARGS
    -Dprotobuf_BUILD_TESTS=OFF
    -Dprotobuf_MSVC_STATIC_RUNTIME=ON

    ${DEPENDS_ON_zlib})