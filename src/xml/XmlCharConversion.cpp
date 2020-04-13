/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/xml/XmlCharConversion.hpp>

namespace powsybl {

namespace xml {

std::string XML2S(const xmlChar* str) {
    // NOLINTNEXTLINE: signed/unsigned bitwise representation are equals
    return reinterpret_cast<const char*>(str);
}

const xmlChar* S2XML(const std::string& str) {
    // NOLINTNEXTLINE: signed/unsigned bitwise representation are equals
    return reinterpret_cast<const xmlChar*>(str.c_str());
}

}  // namespace xml

}  // namespace powsybl
