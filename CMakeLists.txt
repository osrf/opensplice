# Minimal CMake wrapper for the opensplice build
cmake_minimum_required(VERSION 3.5)
project(opensplice)

include (TargetArch.cmake)
target_architecture(ARCH)

if(APPLE)
  set(platform "x86_64.darwin10_clang-release")
elseif(WIN32)
  message(FATAL_ERROR "Windows not yet supported")
else()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(ARCH STREQUAL "i386" OR ARCH STREQUAL "x86_64")
      set(_build_suffix "-debug")
    else()
      set(_build_suffix "-dev")
    endif()
  else()
    set(_build_suffix "-release")
  endif()
  if (ARCH STREQUAL "i386")
    set(platform "x86.linux${_build_suffix}")
  elseif (ARCH STREQUAL "armv7")
    set(platform "armv7l.linux${_build_suffix}")
  elseif (ARCH STREQUAL "armv8") # armv8 works with armv7l profile
    set(platform "armv7l.linux${_build_suffix}")
  elseif (ARCH STREQUAL "aarch64") # aarch64 works with armv7l profile
    set(platform "armv7l.linux${_build_suffix}")
  elseif (ARCH STREQUAL "x86_64")
    set(platform "x86_64.linux${_build_suffix}")
  else()
    message(FATAL_ERROR "Plaform unsupported: ${ARCH}")
  endif()
endif()

message(STATUS "Building for arch: ${ARCH} (profile: ${platform})")

# To build, run our bash wrapper script
add_custom_target(build ALL
                  COMMAND ${CMAKE_COMMAND} -E env MAKE="$(MAKE)" ${CMAKE_CURRENT_SOURCE_DIR}/minimal_build.sh ${platform}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# To install, cherry-pick some stuff
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install/minimal/lib/
        DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install/minimal/include/
        DESTINATION include)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install/minimal/etc/
        DESTINATION etc)
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/install/minimal/bin/cppgen ${CMAKE_CURRENT_SOURCE_DIR}/install/minimal/bin/idlpp
        DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install/minimal/share/
        DESTINATION share)

# Set up CPack to produce a source tarball.
list (APPEND CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION "6.9.0")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "6")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "0")
include(CPack)
