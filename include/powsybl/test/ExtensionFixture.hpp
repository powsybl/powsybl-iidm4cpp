/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_EXTENSIONFIXTURE_HPP
#define POWSYBL_TEST_EXTENSIONFIXTURE_HPP

#include <powsybl/test/ResourceFixture.hpp>

namespace powsybl {

namespace test {

class ExtensionFixture : public ResourceFixture {
public:
    ExtensionFixture();

    ExtensionFixture(const ExtensionFixture& fixture) = default;

    ExtensionFixture(ExtensionFixture&& fixture) = default;

    ~ExtensionFixture() = default;

    ExtensionFixture& operator=(const ExtensionFixture& fixture) = default;

    ExtensionFixture& operator=(ExtensionFixture&& fixture) = default;
};

}  // namespace test

}  // namespace powsybl

#endif //POWSYBL_TEST_EXTENSIONFIXTURE_HPP

