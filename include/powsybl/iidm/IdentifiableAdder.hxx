/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLEADDER_HXX
#define POWSYBL_IIDM_IDENTIFIABLEADDER_HXX

#include <powsybl/iidm/IdentifiableAdder.hpp>

namespace powsybl {

namespace iidm {

template <typename Adder>
std::string IdentifiableAdder<Adder>::getMessageHeader() const {
    return getTypeDescription() + " '" + m_id + "': ";
}

template <typename Adder>
const std::string& IdentifiableAdder<Adder>::getId() const {
    return m_id;
}

template <typename Adder>
const std::string& IdentifiableAdder<Adder>::getName() const {
    return m_name;
}

template <typename Adder>
Adder& IdentifiableAdder<Adder>::setId(const std::string& id) {
    m_id = id;
    return static_cast<Adder&>(*this);
}

template<typename Adder>
Adder& IdentifiableAdder<Adder>::setName(const std::string& name) {
    m_name = name;
    return static_cast<Adder&>(*this);
}

}

}

#endif  // POWSYBL_IIDM_IDENTIFIABLEADDER_HXX
