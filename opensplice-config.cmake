# - Try to find OpenSplice

if(NOT opensplice_FIND_COMPONENTS)
  set(lang CXX)
else()
  list(LENGTH opensplice_FIND_COMPONENTS num_components)
  if(NOT ${num_components} EQUAL 1)
    message(FATAL_ERROR "Only one language at a time is currently supported")
  endif()
  list(GET opensplice_FIND_COMPONENTS 0 lang_in)
  if(${lang_in} STREQUAL "C")
    set(lang ${lang_in})
  elseif(${lang_in} STREQUAL "CXX")
    set(lang ${lang_in})
  else()
    message(FATAL_ERROR "Unsupported language requested: ${lang_in}; should be one of C or CXX.")
  endif()
endif()
  

set(OPENSPLICE_PREFIX ${CMAKE_CURRENT_LIST_DIR}/../../..)

# Include paths
if(${lang} STREQUAL "C")
  set(OPENSPLICE_INCLUDE_DIR ${OPENSPLICE_PREFIX}/include/opensplice ${OPENSPLICE_PREFIX}/include/opensplice/sys ${OPENSPLICE_PREFIX}/include/opensplice/dcps/C/SAC)
  set(OPENSPLICE_INCLUDE_DIRS ${OPENSPLICE_INCLUDE_DIR})
elseif(${lang} STREQUAL "CXX")
  set(OPENSPLICE_INCLUDE_DIR ${OPENSPLICE_PREFIX}/include/opensplice ${OPENSPLICE_PREFIX}/include/opensplice/sys ${OPENSPLICE_PREFIX}/include/opensplice/dcps/C++/SACPP)
  set(OPENSPLICE_INCLUDE_DIRS ${OPENSPLICE_INCLUDE_DIR})
endif()

set(c_libs cmxml commonserv dcpsgapi dcpssac ddsconfparser ddsconf ddsdatabase ddsi2 ddskernel ddsloccollections ddslocutil ddsosnet ddsos ddsserialization ddsuser ddsutil durability spliced)
set(cxx_libs ${c_libs} dcpsisocpp dcpssacpp)

if(${lang} STREQUAL "C")
  # C Libraries
  set(OPENSPLICE_LIBRARY "")
  foreach(lib ${c_libs})
    set(tmplib tmplib-NOTFOUND)
    find_library(tmplib ${lib} HINTS ${OPENSPLICE_PREFIX}/lib)
    if(NOT tmplib)
      message(FATAL_ERROR "Couldn't find OpenSplice C library ${lib}")
    endif()
    set(OPENSPLICE_LIBRARY ${OPENSPLICE_LIBRARY} ${tmplib})
  endforeach()
  set(OPENSPLICE_LIBRARIES ${OPENSPLICE_LIBRARY})
  set(OPENSPLICE_ONLY_LIBRARIES ${OPENSPLICE_LIBRARY})
  set(OPENSPLICE_DEFINITIONS)
elseif(${lang} STREQUAL "CXX")
  # C++ Libraries
  set(OPENSPLICE_LIBRARY "")
  foreach(lib ${cxx_libs})
    set(tmplib tmplib-NOTFOUND)
    find_library(tmplib ${lib} HINTS ${OPENSPLICE_PREFIX}/lib)
    if(NOT tmplib)
      message(FATAL_ERROR "Couldn't find OpenSplice C/C++ library ${lib}")
    endif()
    set(OPENSPLICE_LIBRARY ${OPENSPLICE_LIBRARY} ${tmplib})
  endforeach()
  set(OPENSPLICE_LIBRARIES ${OPENSPLICE_LIBRARY})
  set(OPENSPLICE_ONLY_LIBRARIES ${OPENSPLICE_LIBRARY})
  set(OPENSPLICE_DEFINITIONS)
endif()

# hack
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/setup.sh "export OSPL_URI=file:///${OPENSPLICE_PREFIX}/etc/opensplice/config/ospl.xml\n")
if(APPLE)
  file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/setup.sh "export DYLD_LIBRARY_PATH=${OPENSPLICE_PREFIX}/lib:\$DYLD_LIBRARY_PATH\n")
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(OpenSplice  DEFAULT_MSG
                                  OPENSPLICE_LIBRARY OPENSPLICE_INCLUDE_DIR)

mark_as_advanced(OPENSPLICE_INCLUDE_DIR OPENSPLICE_LIBRARY)

find_program(OPENSPLICE_IDLPP idlpp ${OPENSPLICE_PREFIX}/bin)
if(NOT OPENSPLICE_IDLPP)
  message(FATAL_ERROR "Failed to find idlpp code generator")
endif()

macro(ospl_add_idl)
  if(${lang} STREQUAL "C")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/idl/c)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/idl/c)
  elseif(${lang} STREQUAL "CXX")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/idl/cpp)
  endif()

  foreach(idl ${ARGN})
    get_filename_component(idlbase ${idl} NAME_WE)

    if(${lang} STREQUAL "C")
      # C
      set(outsrcs ${CMAKE_CURRENT_BINARY_DIR}/idl/c/${idlbase}SacDcps.c ${CMAKE_CURRENT_BINARY_DIR}/idl/c/${idlbase}SplDcps.c)
      set(outhdrs ${CMAKE_CURRENT_BINARY_DIR}/idl/c/${idlbase}.h ${CMAKE_CURRENT_BINARY_DIR}/idl/c/${idlbase}Dcps.h ${CMAKE_CURRENT_BINARY_DIR}/idl/c/${idlbase}SacDcps.h ${CMAKE_CURRENT_BINARY_DIR}/idl/c/${idlbase}SplDcps.h)
      set(outfiles ${outsrcs} ${outhdrs})
      if(APPLE)
        set(idlcmd PATH=${OPENSPLICE_PREFIX}/bin:$ENV{PATH} OSPL_TMPL_PATH=${OPENSPLICE_PREFIX}/etc/opensplice/idlpp DYLD_LIBRARY_PATH=${OPENSPLICE_PREFIX}/lib:$ENV{LD_LIBRARY_PATH} ${OPENSPLICE_IDLPP} -l c -S ${CMAKE_CURRENT_SOURCE_DIR}/${idl})
      else()
        set(idlcmd PATH=${OPENSPLICE_PREFIX}/bin:$ENV{PATH} OSPL_TMPL_PATH=${OPENSPLICE_PREFIX}/etc/opensplice/idlpp LD_LIBRARY_PATH=${OPENSPLICE_PREFIX}/lib:$ENV{LD_LIBRARY_PATH} ${OPENSPLICE_IDLPP} -l c -S ${CMAKE_CURRENT_SOURCE_DIR}/${idl})
      endif()
      add_custom_command(OUTPUT ${outfiles}
                         COMMAND ${idlcmd}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/idl/c
                         DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${idl})
      add_custom_target(idl_${idlbase} ALL DEPENDS ${outfiles})
      add_library(idl_${idlbase}lib SHARED ${outsrcs})
      target_link_libraries(idl_${idlbase}lib ${OPENSPLICE_ONLY_LIBRARIES})
      set(OPENSPLICE_LIBRARY idl_${idlbase}lib ${OPENSPLICE_LIBRARY})
      set(OPENSPLICE_LIBRARIES ${OPENSPLICE_LIBRARY})
    elseif(${lang} STREQUAL "CXX")
      # C++
      set(outsrcs ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}Dcps.cpp ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}Dcps_impl.cpp ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}SplDcps.cpp ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}.cpp)
      set(outhdrs ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/ccpp_${idlbase}.h ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}.h ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}Dcps.h ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}Dcps_impl.h ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp/${idlbase}SplDcps.h)
      set(outfiles ${outsrcs} ${outhdrs})
      if(APPLE)
        set(idlcmd PATH=${OPENSPLICE_PREFIX}/bin:$ENV{PATH} OSPL_TMPL_PATH=${OPENSPLICE_PREFIX}/etc/opensplice/idlpp DYLD_LIBRARY_PATH=${OPENSPLICE_PREFIX}/lib:$ENV{LD_LIBRARY_PATH} ${OPENSPLICE_IDLPP} -l c++ -S ${CMAKE_CURRENT_SOURCE_DIR}/${idl})
      else()
        set(idlcmd PATH=${OPENSPLICE_PREFIX}/bin:$ENV{PATH} OSPL_TMPL_PATH=${OPENSPLICE_PREFIX}/etc/opensplice/idlpp LD_LIBRARY_PATH=${OPENSPLICE_PREFIX}/lib:$ENV{LD_LIBRARY_PATH} ${OPENSPLICE_IDLPP} -l c++ -S ${CMAKE_CURRENT_SOURCE_DIR}/${idl})
      endif()
      add_custom_command(OUTPUT ${outfiles}
                         COMMAND ${idlcmd}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/idl/cpp
                         DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${idl})
      add_custom_target(idl_${idlbase} ALL DEPENDS ${outfiles})
      add_library(idl_${idlbase}lib SHARED ${outsrcs})
      target_link_libraries(idl_${idlbase}lib ${OPENSPLICE_ONLY_LIBRARIES})
      set(OPENSPLICE_LIBRARY idl_${idlbase}lib ${OPENSPLICE_LIBRARY})
      set(OPENSPLICE_LIBRARIES ${OPENSPLICE_LIBRARY})
    endif()
  endforeach()
endmacro()
