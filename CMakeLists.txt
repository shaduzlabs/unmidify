
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

cmake_minimum_required (VERSION 2.8.0)

project(unMIDIfy)

# The version number.
set (UNMIDIFY_VER_MAJOR  0)
set (UNMIDIFY_VER_MINOR  1)
set (UNMIDIFY_VER_MICRO  0)
set (UNMIDIFY_VER_STRING ${UNMIDIFY_VER_MAJOR}.${UNMIDIFY_VER_MINOR}.${UNMIDIFY_VER_MICRO})

enable_testing()

# Libraries -------------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

endif()

# Library --------------------------------------------------------------------------------------- #

add_library(HEADER_ONLY_TARGET STATIC include/unMIDIfy.hpp)
set_target_properties(HEADER_ONLY_TARGET PROPERTIES LINKER_LANGUAGE CXX)

# Unit tests ------------------------------------------------------------------------------------- #

add_subdirectory(test)


# Documentation (doxygen) ------------------------------------------------------------------------ #

find_package(Doxygen)
if(DOXYGEN_FOUND)

  set( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/support/doxygen/Doxyfile.in )
  set( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile                          )

  set( doxy_project_name    ${PROJECT_NAME}                                         )
  set( doxy_input_folder    ${PROJECT_SOURCE_DIR}/include                           )
  set( doxy_output_root     ${PROJECT_SOURCE_DIR}/docs                              )
  set( doxy_readme_md       ${PROJECT_SOURCE_DIR}/README.md                         )

  set( doxy_html_root       ${PROJECT_SOURCE_DIR}/support/doxygen                   )

  set( doxy_project_version "v. ${UNMIDIFY_VER_MAJOR}.${UNMIDIFY_VER_MINOR}.${UNMIDIFY_VER_MICRO}")

  configure_file( ${doxyfile_in} ${doxyfile} @ONLY )

  add_custom_target(
    doc
      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
  )
else()
  message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif(DOXYGEN_FOUND)


# Examples --------------------------------------------------------------------------------------- #

#add_subdirectory(examples)
