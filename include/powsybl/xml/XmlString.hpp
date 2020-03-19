/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_XMLSTRING_HPP
#define POWSYBL_IIDM_XMLSTRING_HPP

#include <functional>
#include <memory>

#include <libxml/parser.h>

#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace xml {

class IIDM_DECLSPEC XmlString : public std::unique_ptr<xmlChar, std::function<void(void*)>> {
public:
    explicit XmlString(xmlChar* ptr) noexcept :
        unique_ptr(ptr, &std::free) {
    }

    XmlString(XmlString&&) noexcept = default;

    virtual ~XmlString() = default;
};

}  // namespace xml

}  // namespace powsybl

#endif  // POWSYBL_IIDM_XMLSTRING_HPP
