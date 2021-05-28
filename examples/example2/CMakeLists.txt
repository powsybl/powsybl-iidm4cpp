#
# Copyright (c) 2020, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Force a minimum CMake version
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Declare the project
project(my-project)

# Find the required dependencies: Boost, LibXML2 and IIDM and import the targets
find_package(Boost 1.65 REQUIRED COMPONENTS date_time filesystem program_options system unit_test_framework)
find_package(Threads REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibIIDM REQUIRED)

# Create an executable
add_executable(my-executable ../example1/example1.cpp)

# Link the executable with the IIDM shared library
target_link_libraries(my-executable PRIVATE IIDM::iidm)
