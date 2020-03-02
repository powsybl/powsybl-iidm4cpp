#
# Copyright (c) 2020, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

include(CMakeParseArguments)

function(doxygen)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        set(options NONE)
        set(oneValueArgs NAME CONFIG OUTPUT_DIR)
        set(multiValueArgs MULTI_NONE)
        cmake_parse_arguments(Doxygen "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        # Configuration doxygen
        set(DOXYGEN_IN ${Doxygen_CONFIG})
        set(DOXYGEN_OUTPUT_DIRECTORY ${Doxygen_OUTPUT_DIR})
        configure_file(${DOXYGEN_IN} Doxyfile @ONLY)

        add_custom_target(${Doxygen_NAME}
            COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
            COMMENT "Generating API documentation with Doxygen"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

        message("${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}")
        install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}"
            DESTINATION ${INSTALL_DOC_DIR}
        )

    endif ()

endfunction ()
