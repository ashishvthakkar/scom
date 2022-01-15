# ==============================================================================
# ===== [BEGIN] KyDeps CMakeLists.txt preamble ===== !!! DO NOT EDIT !!! =======
#
cmake_minimum_required(VERSION 3.20)
project(kydeps)
if (CMAKE_SOURCE_DIR STREQUAL kydeps_SOURCE_DIR)
    include(kydep)
else()
    include(FetchContent)
    FetchContent_Declare(kydep_bootstrap
        GIT_REPOSITORY https://github.com/kyotov/kydep-bootstrap.git
        GIT_TAG v2.3
    )
    FetchContent_MakeAvailable(kydep_bootstrap)
endif()
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
    
KyDep(
  OpenSSL
  GIT_REPOSITORY https://github.com/openssl/openssl.git
  GIT_TAG fd78df59b0f656aefe96e39533130454aa957c00 # OpenSSL_1_1_1k

  PATCH_COMMAND echo <INSTALL_DIR>
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> ./config no-shared no-dso --prefix=<INSTALL_DIR> --openssldir=<INSTALL_DIR>
  BUILD_COMMAND make -C <SOURCE_DIR> build_libs
  INSTALL_COMMAND make -C <SOURCE_DIR> install_dev

  ${DEPENDS_ON_zlib})

KyDep(
  libssh
  GIT_REPOSITORY https://git.libssh.org/projects/libssh.git
  GIT_TAG da6d026c125712d197479a7930b4efc117bfe7af # libssh-0.9.6

  ${DEPENDS_ON_zlib}
  ${DEPENDS_ON_OpenSSL})

KyDep(
  experiments
  GIT_REPOSITORY https://github.com/ashishvthakkar/experiments.git
  GIT_TAG 2805a4d4d896013a6a4421fb101813c5e719a12f # v20220115
  )
