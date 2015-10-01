
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

# ------------------------------------------------------------------------------------------------ #
#  Unit tests                                                                                      #
# ------------------------------------------------------------------------------------------------ #

project(unit-tests)

# Dependencies ----------------------------------------------------------------------------------- #
addCatch()

# Unit tests files ------------------------------------------------------------------------------- #

set(
  test_SRCS
    main.cpp
    unMIDIfy.cpp
)

source_group("src"  FILES  ${test_SRCS})

set(
  Test_FILES
    ${test_SRCS}
)

add_executable(
  ${PROJECT_NAME}
  ${Test_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CATCH_INCLUDE_DIR} ${UNMIDIFY_INCLUDE_DIR})

add_test(NAME ${PROJECT_NAME}-runner
        COMMAND ${PROJECT_NAME} -b
        CONFIGURATIONS Debug Release
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)
