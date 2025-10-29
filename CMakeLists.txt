cmake_minimum_required(VERSION 3.16)
project(unified_runner_proj CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# add root folder include
if(EXISTS "${CMAKE_SOURCE_DIR}/include")
  include_directories("${CMAKE_SOURCE_DIR}/include")
endif()

#can't find <utility>/<array> 
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
    message(WARNING "-DOSX_SYSROOT=/path/to/sdk")
  endif()
  if(NOT DEFINED CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "" FORCE)
  endif()
endif()

add_executable(unified_runner
  src/main_unified.cpp
  src/RHCRPlanner.cpp
  src/LaCAMPlanner.cpp
)

# put executable in the build
set_target_properties(unified_runner PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# ===== extern: LaCAM（add_library(lacam) + add_executable(lacam_bridge ...)）=====
add_subdirectory(extern/LaCAM/lacam)

# set output path to ./build/lacam/lacam_bridge
if(TARGET lacam_bridge)
  set_target_properties(lacam_bridge PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lacam"
  )
  # include
  if(EXISTS "${CMAKE_SOURCE_DIR}/include")
    target_include_directories(lacam_bridge PRIVATE "${CMAKE_SOURCE_DIR}/include")
  endif()
endif()

# ===== RHCR（add_executable(lifelong ...)）=====
add_subdirectory(extern/RHCR)

# put lifelong to ./build/lifelong
if(TARGET lifelong)
  set_target_properties(lifelong PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  )
endif()
