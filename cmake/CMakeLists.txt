cmake_minimum_required(VERSION 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include("./CMakeListsConan.txt")
endif()

project(libdexfile LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(libartbase-ndk
                     GIT_REPOSITORY git@github.com:MaDeG/libartbase-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libartbase-ndk)
FetchContent_GetProperties(libartbase-ndk)

FetchContent_Declare(libartpalette-ndk
                     GIT_REPOSITORY git@github.com:MaDeG/libartpalette-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libartpalette-ndk)
FetchContent_GetProperties(libartpalette-ndk)

FetchContent_Declare(libziparchive-ndk
                     GIT_REPOSITORY git@github.com:MaDeG/libziparchive-ndk.git
                     GIT_TAG integration
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libziparchive-ndk)
FetchContent_GetProperties(libziparchive-ndk)

FetchContent_Declare(libbase-ndk
                     GIT_REPOSITORY git@github.com:MaDeG/libbase-ndk.git
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
)

add_library(libdexfile STATIC ${ANDROID_SOURCES} ${LIBDEXFILE_SOURCES})
target_link_libraries(libdexfile libartbase libartpalette libziparchive libbase)
target_include_directories(libdexfile PRIVATE ${LIBDEXFILE_ROOT}
                                              ${libartbase-ndk_SOURCE_DIR}
                                              ${libartpalette-ndk_SOURCE_DIR}/include
                                              ${libziparchive-ndk_SOURCE_DIR}/include
                                              ${libbase-ndk_SOURCE_DIR}/include
)
target_include_directories(libdexfile PUBLIC ${LIBDEXFILE_ROOT}/external/include)
add_compile_definitions(ART_STACK_OVERFLOW_GAP_arm=8192
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

set_property(TARGET libdexfile PROPERTY CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(libdexfile PRIVATE $<BUILD_INTERFACE:-Wno-unknown-attributes>)
endif()
