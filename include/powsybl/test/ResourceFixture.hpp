/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_RESOURCEFIXTURE_HPP
#define POWSYBL_TEST_RESOURCEFIXTURE_HPP

#include <fstream>
#include <sstream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/test/CommandLine.hpp>

namespace powsybl {

namespace test {

class ResourceFixture : private CommandLine {
public:
    ResourceFixture() {
        boost::program_options::options_description desc;
        desc.add_options()
                ("resources", boost::program_options::value<std::string>()->required(),
                 "Path where the test resources are stored");

        parse(desc);
    }

    std::string getResource(const std::string &name) {
        boost::filesystem::path path(getOptionValue("resources").as<std::string>());
        path /= name;

        if (!boost::filesystem::exists(path)) {
            throw powsybl::AssertionError(powsybl::logging::format("Unable to find the resource: %1%", path.string()));
        }

        std::stringstream buffer;
        std::ifstream stream(path.string());
        if (!stream) {
            throw powsybl::AssertionError(
                    powsybl::logging::format("Unable to access to the resource: %1%", path.string()));
        }
        buffer << stream.rdbuf();

        return buffer.str();
    }
};

}  // namespace test

}  // namespace powsybl

#endif  // POWSYBL_TEST_RESOURCEFIXTURE_HPP
