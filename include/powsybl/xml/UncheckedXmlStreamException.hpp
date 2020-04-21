/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_XML_UNCHECKEDXMLSTREAMEXCEPTION_HPP
#define POWSYBL_XML_UNCHECKEDXMLSTREAMEXCEPTION_HPP

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace xml {

class UncheckedXmlStreamException : public PowsyblException {
public:
    explicit UncheckedXmlStreamException(const std::string& msg);

    UncheckedXmlStreamException(const UncheckedXmlStreamException&) = default;

    UncheckedXmlStreamException(UncheckedXmlStreamException&&) = default;

    ~UncheckedXmlStreamException() override = default;

    UncheckedXmlStreamException& operator=(const UncheckedXmlStreamException&) = default;

    UncheckedXmlStreamException& operator=(UncheckedXmlStreamException&&) = default;
};

}  // namespace xml

}  // namespace powsybl

#endif  // POWSYBL_XML_UNCHECKEDXMLSTREAMEXCEPTION_HPP
