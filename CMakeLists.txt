cmake_minimum_required(VERSION 3.16)
project(mapf_unified_all CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 根 include（放 Args.h 等可选）
if(EXISTS "${CMAKE_SOURCE_DIR}/include")
  include_directories("${CMAKE_SOURCE_DIR}/include")
endif()

# ===== macOS only: 解决 <utility>/<array> 头找不到问题 =====
if(APPLE)
  if(NOT DEFINED OSX_SYSROOT)
    execute_process(
      COMMAND xcrun --sdk macosx --show-sdk-path
      OUTPUT_VARIABLE OSX_SYSROOT
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )
  endif()
  if(OSX_SYSROOT)
    message(STATUS "Using macOS SDK: ${OSX_SYSROOT}")
    add_compile_options(-isysroot ${OSX_SYSROOT} -stdlib=libc++)
    add_link_options(   -isysroot ${OSX_SYSROOT} -stdlib=libc++)
    include_directories(SYSTEM
      "${OSX_SYSROOT}/usr/include/c++/v1"
      "/Library/Developer/CommandLineTools/usr/include/c++/v1"
    )
  else()
    message(WARNING "未能探测到 macOS SDK，若出现 <utility>/<array> 缺失，请传 -DOSX_SYSROOT=/path/to/sdk")
  endif()
  if(NOT DEFINED CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "" FORCE)
  endif()
endif()

# ===== 子仓: LaCAM（内部 CMake 定义了 add_library(lacam) + add_executable(lacam_bridge ...)）=====
add_subdirectory(extern/LaCAM/lacam)

# 不改名！仅把可执行输出目录设为 ./build/lacam/lacam_bridge
if(TARGET lacam_bridge)
  set_target_properties(lacam_bridge PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lacam"
  )
  # 根 include（若需要 Args.h）
  if(EXISTS "${CMAKE_SOURCE_DIR}/include")
    target_include_directories(lacam_bridge PRIVATE "${CMAKE_SOURCE_DIR}/include")
  endif()
endif()

# ===== 子仓: RHCR（内部 CMake 里 add_executable(lifelong ...)）=====
add_subdirectory(extern/RHCR)

# lifelong 放到 ./build/lifelong
if(TARGET lifelong)
  set_target_properties(lifelong PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  )
endif()
