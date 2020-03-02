/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/test/ResourceFixture.hpp>

#include <fstream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace test {

ResourceFixture::ResourceFixture() {
    boost::program_options::options_description desc;
    desc.add_options()
        ("resources", boost::program_options::value<std::string>()->required(),
         "Path where the test resources are stored")
        ("ext-path", boost::program_options::value<std::string>()->implicit_value(""),
         "Path where extensions are stored")
        ("files", boost::program_options::value<std::string>()->implicit_value(""),
         "Identify files to be loaded in the directory");

    parse(desc);
}

std::string ResourceFixture::getResource(const std::string& name) const {
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

}  // namespace test

}  // namespace powsybl
