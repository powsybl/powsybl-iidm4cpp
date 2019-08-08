/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_XML_XMLFIXTURE_HPP
#define POWSYBL_XML_XMLFIXTURE_HPP

#include <libxml/parser.h>

class XmlFixture {
public:
    XmlFixture() {
        xmlInitParser();
    }

    ~XmlFixture() {
        xmlCleanupParser();
    }
};

#endif  // POWSYBL_XML_XMLFIXTURE_HPP
