/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/test/ResourceFixture.hpp>

#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/config.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace test {

ResourceFixture::ResourceFixture() {
    boost::program_options::options_description desc;
    desc.add_options()
        ("resources", boost::program_options::value<std::string>()->required(),
         "Path where the test resources are stored");

    parse(desc);
}

std::string getOSName() {
    std::string osName = BOOST_PLATFORM;
    boost::replace_all(osName, " ", "");
    boost::replace_all(osName, "/", "");
    boost::to_lower(osName);
    return osName;
}

const std::string& getNormalizedOSName() {
    static std::string s_osName = getOSName();
    return s_osName;
}

std::string ResourceFixture::getResource(const std::string& name) const {
    const boost::filesystem::path& path = getResourcePath(name);

    if (!boost::filesystem::exists(path)) {
        throw powsybl::AssertionError(stdcxx::format("Unable to find the resource: %1%", path.string()));
    }

    std::stringstream buffer;
    std::ifstream stream(path.string());
    if (!stream) {
        throw powsybl::AssertionError(
            stdcxx::format("Unable to access to the resource: %1%", path.string()));
    }
    buffer << stream.rdbuf();

    return buffer.str();
}

boost::filesystem::path ResourceFixture::getResourcePath(const std::string& name) const {
    boost::filesystem::path path(getOptionValue("resources").as<std::string>());
    const std::string& osCustomName = (path / name).string() + "." + getNormalizedOSName();
    if (boost::filesystem::exists(osCustomName)) {
        return osCustomName;
    }
    return path / name;
}

}  // namespace test

}  // namespace powsybl
