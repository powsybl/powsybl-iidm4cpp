/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_XMLFIXTURE_HPP
#define POWSYBL_TEST_XMLFIXTURE_HPP

namespace powsybl {

namespace test {

class XmlFixture {
public:
    XmlFixture();

    XmlFixture(const XmlFixture&) = default;

    XmlFixture(XmlFixture&&) = default;

    ~XmlFixture();

    XmlFixture& operator=(const XmlFixture&) = default;

    XmlFixture& operator=(XmlFixture&&) = default;
};

}  // namespace test

}  // namespace powsybl

#endif  // POWSYBL_TEST_XMLFIXTURE_HPP
