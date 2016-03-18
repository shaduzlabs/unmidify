
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

cmake_minimum_required (VERSION 3.1)
project(unmidify)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
include(sl-main)

# The version number.
set (unmidify_VER_MAJOR  0)
set (unmidify_VER_MINOR  1)
set (unmidify_VER_MICRO  0)
set (unmidify_VER_STRING ${unmidify_VER_MAJOR}.${unmidify_VER_MINOR}.${unmidify_VER_MICRO})

if(CMAKE_PROJECT_NAME STREQUAL unmidify)
  set(IS_unmidify TRUE)
else()
  set(IS_unmidify FALSE)
endif()

option(unmidify_TEST  "Build unmidify tests" ${IS_unmidify})
option(unmidify_DOCS  "Build unmidify docs"  ${IS_unmidify})


# Library --------------------------------------------------------------------------------------- #

add_library(unmidify INTERFACE)
target_sources(unmidify INTERFACE include/unmidify.hpp)
set(unmidify_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Unit tests ------------------------------------------------------------------------------------- #

if(unmidify_TEST)
  message(STATUS "Unit test project enabled")
  add_subdirectory(test)
endif()


# Documentation (doxygen) ------------------------------------------------------------------------ #

if(unmidify_DOCS)
  find_package(Doxygen)
    if(DOXYGEN_FOUND)

    set( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/support/doxygen/Doxyfile.in )
    set( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile                          )

    set( doxy_project_name    ${PROJECT_NAME}                                         )
    set( doxy_input_folder    ${PROJECT_SOURCE_DIR}/include                           )
    set( doxy_output_root     ${PROJECT_SOURCE_DIR}/docs                              )
    set( doxy_readme_md       ${PROJECT_SOURCE_DIR}/README.md                         )

    set( doxy_html_root       ${PROJECT_SOURCE_DIR}/support/doxygen                   )

    set( doxy_project_version "v. ${unmidify_VER_MAJOR}.${unmidify_VER_MINOR}.${unmidify_VER_MICRO}")

    configure_file( ${doxyfile_in} ${doxyfile} @ONLY )

    add_custom_target(
      doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
  else()
    message(STATUS "Doxygen is needed to build the documentation.")
  endif(DOXYGEN_FOUND)
endif(unmidify_DOCS)


# Examples --------------------------------------------------------------------------------------- #

#add_subdirectory(examples)
