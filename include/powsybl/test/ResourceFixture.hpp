/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_RESOURCEFIXTURE_HPP
#define POWSYBL_TEST_RESOURCEFIXTURE_HPP

#include <string>

#include <boost/filesystem/path.hpp>

#include <powsybl/test/CommandLine.hpp>

namespace powsybl {

namespace test {

class ResourceFixture : public CommandLine {
public:
    ResourceFixture();

    std::string getResource(const std::string& name) const;

    boost::filesystem::path getResourcePath(const std::string& name) const;
};

}  // namespace test

}  // namespace powsybl

#endif  // POWSYBL_TEST_RESOURCEFIXTURE_HPP
