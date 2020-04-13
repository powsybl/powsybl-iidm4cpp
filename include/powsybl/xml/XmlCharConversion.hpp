/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_XML_XMLCHARCONVERSION_HPP
#define POWSYBL_XML_XMLCHARCONVERSION_HPP

#include <string>

#include <libxml/xmlstring.h>

namespace powsybl {

namespace xml {

std::string XML2S(const xmlChar* str);

const xmlChar* S2XML(const std::string& str);

}  // namespace xml

}  // namespace powsybl

#endif  // POWSYBL_XML_XMLCHARCONVERSION_HPP

