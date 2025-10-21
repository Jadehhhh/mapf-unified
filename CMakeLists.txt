cmake_minimum_required(VERSION 3.16)

# Parent project that just orchestrates subprojects
project(mapf_unified LANGUAGES CXX)

# Use a modern C++ standard; subprojects can override if they want
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put all built executables in the top-level build/ folder
# (Subprojects may have their own defaults; we’ll override per-target below.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ------------------------------------------------------------------------------
# Add the two original projects EXACTLY as they are.
# This avoids re-compiling their sources ourselves (no *.c vs *.cpp issues, etc.).
# ------------------------------------------------------------------------------
add_subdirectory(extern/LaCAM)
add_subdirectory(extern/RHCR)

# ------------------------------------------------------------------------------
# Figure out the LaCAM executable target name from its original CMake.
# Common names we’ve seen: "driver" or "lacam".
# If yours is different, tweak the list below or open extern/LaCAM/CMakeLists.txt
# and check the name used in add_executable(<NAME> ...).
# ------------------------------------------------------------------------------
set(_LACAM_CANDIDATES driver lacam LaCAM lacam_cli)
set(LACAM_TARGET "")
foreach(name IN LISTS _LACAM_CANDIDATES)
  if (TARGET ${name})
    set(LACAM_TARGET ${name})
    break()
  endif()
endforeach()

if (LACAM_TARGET STREQUAL "")
  message(FATAL_ERROR
    "Could not find LaCAM executable target. "
    "Open extern/LaCAM/CMakeLists.txt and tell me the add_executable target name, "
    "then add it to _LACAM_CANDIDATES.")
endif()

# Normalize LaCAM executable name & output dir:
set_target_properties(${LACAM_TARGET} PROPERTIES
  OUTPUT_NAME              "lacam"            # final name: ./build/lacam
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ------------------------------------------------------------------------------
# Figure out the RHCR executable target name.
# From your snippet, RHCR already uses: add_executable(lifelong ...).
# We still detect and normalize output path just in case.
# ------------------------------------------------------------------------------
set(_RHCR_CANDIDATES lifelong rhcr RHCR)
set(RHCR_TARGET "")
foreach(name IN LISTS _RHCR_CANDIDATES)
  if (TARGET ${name})
    set(RHCR_TARGET ${name})
    break()
  endif()
endforeach()

if (RHCR_TARGET STREQUAL "")
  message(FATAL_ERROR
    "Could not find RHCR executable target. "
    "Open extern/RHCR/CMakeLists.txt and tell me the add_executable target name, "
    "then add it to _RHCR_CANDIDATES.")
endif()

# Normalize RHCR executable output dir (name is already 'lifelong'):
set_target_properties(${RHCR_TARGET} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}  # final path: ./build/lifelong
)

# ------------------------------------------------------------------------------
# Notes:
# - We don’t add Boost here; each subproject’s own CMake should find whatever it needs.
# - Python files inside extern/ are ignored by CMake, so they won’t affect the build.
# - If you want RelWithDebInfo by default, uncomment:
# set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "" FORCE)
# ------------------------------------------------------------------------------
