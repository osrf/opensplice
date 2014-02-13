# - Try to find OpenSplice

set(OPENSPLICE_PREFIX ${CMAKE_CURRENT_LIST_DIR}/../../..)

set(OPENSPLICE_INCLUDE_DIR ${OPENSPLICE_PREFIX}/include/opensplice ${OPENSPLICE_PREFIX}/include/opensplice/sys ${OPENSPLICE_PREFIX}/include/opensplice/dcps/C++/SACPP)
set(OPENSPLICE_INCLUDE_DIRS ${OPENSPLICE_INCLUDE_DIR})
set(libs cmjni cmxml commonserv dcpsgapi dcpsisocpp dcpsjni dcpssacpp dcpssac dcpssaj ddsconfparser ddsconf ddsdatabase ddsi2 ddskernel ddsloccollections ddslocutil ddsosnet ddsos ddsserialization ddsuser ddsutil durability spliced)
set(OPENSPLICE_LIBRARY "")
foreach(lib ${libs})
  set(tmplib tmplib-NOTFOUND)
  find_library(tmplib ${lib} HINTS ${OPENSPLICE_PREFIX}/lib)
  if(NOT tmplib)
    message(FATAL_ERROR "Couldn't find OpenSplice library ${lib}")
  endif()
  set(OPENSPLICE_LIBRARY ${OPENSPLICE_LIBRARY} ${tmplib})
endforeach()
set(OPENSPLICE_LIBRARIES ${OPENSPLICE_LIBRARY})
set(OPENSPLICE_DEFINITIONS)

# hack
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/setup.sh "export OSPL_URI=file:///${OPENSPLICE_PREFIX}/etc/opensplice/config/ospl.xml\n")

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
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/idl)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/idl)
  foreach(idl ${ARGN})
    get_filename_component(idlbase ${idl} NAME_WE)
    set(outsrcs ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}Dcps.cpp ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}Dcps_impl.cpp ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}SplDcps.cpp ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}.cpp)
    set(outhdrs ${CMAKE_CURRENT_BINARY_DIR}/idl/ccpp_${idlbase}.h ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}.h ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}Dcps.h ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}Dcps_impl.h ${CMAKE_CURRENT_BINARY_DIR}/idl/${idlbase}SplDcps.h)
    set(outfiles ${outsrcs} ${outhdrs})
    add_custom_command(OUTPUT ${outfiles}
                       COMMAND PATH=${OPENSPLICE_PREFIX}/bin:$ENV{PATH} OSPL_TMPL_PATH=${OPENSPLICE_PREFIX}/etc/opensplice/idlpp LD_LIBRARY_PATH=${OPENSPLICE_PREFIX}/lib:$ENV{LD_LIBRARY_PATH} ${OPENSPLICE_IDLPP} -l c++ -S ${CMAKE_CURRENT_SOURCE_DIR}/${idl}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/idl
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${idl})
    add_custom_target(idl_${idlbase} ALL DEPENDS ${outfiles})
    add_library(idl_${idlbase}lib SHARED ${outsrcs})
    set(OPENSPLICE_LIBRARY idl_${idlbase}lib ${OPENSPLICE_LIBRARY})
    set(OPENSPLICE_LIBRARIES ${OPENSPLICE_LIBRARY})
  endforeach()
endmacro()
