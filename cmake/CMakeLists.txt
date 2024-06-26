if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

cmake_minimum_required(VERSION 3.20)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(libdexfile LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(libartbase-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libartbase-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libartbase-ndk)
FetchContent_GetProperties(libartbase-ndk)

FetchContent_Declare(libartpalette-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libartpalette-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libartpalette-ndk)
FetchContent_GetProperties(libartpalette-ndk)

FetchContent_Declare(libziparchive-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libziparchive-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libziparchive-ndk)
FetchContent_GetProperties(libziparchive-ndk)

FetchContent_Declare(libbase-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libbase-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libbase-ndk)
FetchContent_GetProperties(libbase-ndk)

set(LIBDEXFILE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../)

set(ANDROID_SOURCES ${LIBDEXFILE_ROOT}/android-base/errno_restorer.h
                    ${LIBDEXFILE_ROOT}/android-base/file.cpp
                    ${LIBDEXFILE_ROOT}/android-base/file.h
                    ${LIBDEXFILE_ROOT}/android-base/log_main.h
                    ${LIBDEXFILE_ROOT}/android-base/logging.h
                    ${LIBDEXFILE_ROOT}/android-base/macros.h
                    ${LIBDEXFILE_ROOT}/android-base/off64_t.h
                    ${LIBDEXFILE_ROOT}/android-base/parseint.h
                    ${LIBDEXFILE_ROOT}/android-base/stringprintf.cpp
                    ${LIBDEXFILE_ROOT}/android-base/stringprintf.h
                    ${LIBDEXFILE_ROOT}/android-base/strings.cpp
                    ${LIBDEXFILE_ROOT}/android-base/strings.h
                    ${LIBDEXFILE_ROOT}/android-base/threads.h
                    ${LIBDEXFILE_ROOT}/android-base/thread_annotations.h
                    ${LIBDEXFILE_ROOT}/android-base/unique_fd.h
                    ${LIBDEXFILE_ROOT}/android-base/utf8.h
)

set(LIBDEXFILE_SOURCES ${LIBDEXFILE_ROOT}/dex/art_dex_file_loader.cc
                       ${LIBDEXFILE_ROOT}/dex/compact_dex_file.cc
                       ${LIBDEXFILE_ROOT}/dex/compact_offset_table.cc
                       ${LIBDEXFILE_ROOT}/dex/descriptors_names.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file_exception_helpers.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file_layout.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file_loader.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file_tracking_registrar.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file_verifier.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_instruction.cc
                       ${LIBDEXFILE_ROOT}/dex/modifiers.cc
                       ${LIBDEXFILE_ROOT}/dex/primitive.cc
                       ${LIBDEXFILE_ROOT}/dex/signature.cc
                       ${LIBDEXFILE_ROOT}/dex/standard_dex_file.cc
                       ${LIBDEXFILE_ROOT}/dex/type_lookup_table.cc
                       ${LIBDEXFILE_ROOT}/dex/utf.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file.operator_out.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_file_layout.operator_out.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_instruction.operator_out.cc
                       ${LIBDEXFILE_ROOT}/dex/dex_instruction_utils.operator_out.cc
                       ${LIBDEXFILE_ROOT}/dex/invoke_type.operator_out.cc
                       ${LIBDEXFILE_ROOT}/external/dex_file_ext.cc
                       ${LIBDEXFILE_ROOT}/external/dex_file_supp.cc
)

add_library(dexfile STATIC ${ANDROID_SOURCES} ${LIBDEXFILE_SOURCES})
target_link_libraries(dexfile artbase artpalette ziparchive base)
target_include_directories(dexfile PRIVATE ${LIBDEXFILE_ROOT}
                                           ${libartbase-ndk_SOURCE_DIR}
                                           ${libartpalette-ndk_SOURCE_DIR}/include
                                           ${libziparchive-ndk_SOURCE_DIR}/include
                                           ${libbase-ndk_SOURCE_DIR}/include
)
target_include_directories(dexfile PUBLIC ${LIBDEXFILE_ROOT}/external/include)
target_compile_definitions(dexfile PUBLIC ART_STACK_OVERFLOW_GAP_arm=8192
                                          ART_STACK_OVERFLOW_GAP_arm64=8192
                                          ART_STACK_OVERFLOW_GAP_riscv64=8192
                                          ART_STACK_OVERFLOW_GAP_x86=8192
                                          ART_STACK_OVERFLOW_GAP_x86_64=8192
                                          ART_NDEBUG_OPT_FLAG=-O3
                                          ART_DEFAULT_GC_TYPE=CMC
                                          ART_DEFAULT_COMPACT_DEX_LEVEL=fast
                                          ART_FRAME_SIZE_LIMIT=7400
                                          ART_BASE_ADDRESS=32768
)

set_property(TARGET dexfile PROPERTY CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(dexfile PRIVATE $<BUILD_INTERFACE:-Wno-unknown-attributes>)
endif()
