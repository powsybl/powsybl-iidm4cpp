/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_XML_XMLSTREAMEXCEPTION_HPP
#define POWSYBL_XML_XMLSTREAMEXCEPTION_HPP

#include <string>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace xml {

/**
 * @brief XML library typed exception
 */
class XmlStreamException : public PowsyblException {
public:
    explicit XmlStreamException(const std::string& msg);

    ~XmlStreamException() override = default;
};

}  // namespace xml

}  // namespace powsybl

#endif  // POWSYBL_XML_XMLSTREAMEXCEPTION_HPP
