/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLEADDER_HXX
#define POWSYBL_IIDM_IDENTIFIABLEADDER_HXX

#include <powsybl/iidm/IdentifiableAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/util/Identifiables.hpp>

namespace powsybl {

namespace iidm {

template <typename Adder>
std::string IdentifiableAdder<Adder>::getMessageHeader() const {
    return getTypeDescription() + " '" + m_id + "': ";
}

template <typename Adder>
std::string IdentifiableAdder<Adder>::checkAndGetUniqueId() const {
    if (m_id.empty()) {
        throw PowsyblException(stdcxx::format("%1% id is not set", getTypeDescription()));
    }

    const auto& network = getNetwork();

    std::string uniqueId;
    if (m_ensureIdUnicity) {
        uniqueId = util::Identifiables::getUniqueId(m_id, [&network](const std::string& id) {
            return static_cast<bool>(network.find(id));
        });
    } else {
        const auto& obj = network.find(m_id);
        if (obj) {
            throw PowsyblException(stdcxx::format("The network %1% already contains an object '%2%' with the id '%3%'", network.getId(), stdcxx::simpleClassName(obj.get()), m_id));
        }
        uniqueId = m_id;
    }

    return uniqueId;
}

template <typename Adder>
const std::string& IdentifiableAdder<Adder>::getName() const {
    return m_name;
}

template <typename Adder>
bool IdentifiableAdder<Adder>::isFictitious() const {
    return m_fictitious;
}

template <typename Adder>
Adder& IdentifiableAdder<Adder>::setEnsureIdUnicity(bool ensureIdUnicity) {
    m_ensureIdUnicity = ensureIdUnicity;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& IdentifiableAdder<Adder>::setFictitious(bool fictitious) {
    m_fictitious = fictitious;
    return static_cast<Adder&>(*this);
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

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_IDENTIFIABLEADDER_HXX
