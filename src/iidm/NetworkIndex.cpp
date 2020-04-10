/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkIndex.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

NetworkIndex::NetworkIndex(Network& network, NetworkIndex&& networkIndex) noexcept :
    m_objectsById(std::move(networkIndex.m_objectsById)),
    m_objectsByType(std::move(networkIndex.m_objectsByType)) {

    for (Substation& substation : getAll<Substation>()) {
        substation.setNetworkRef(network);
    }
}

void NetworkIndex::checkId(const std::string& id) {
    checkNotEmpty(id, "Invalid id '" + id + "'");
}

template <>
const Identifiable& NetworkIndex::get(const std::string& id) const {
    checkId(id);

    const auto& it = m_objectsById.find(id);
    if (it == m_objectsById.end()) {
        throw PowsyblException(logging::format("Unable to find to the identifiable '%1%'", id));
    }

    return *(it->second.get());
}

template <>
Identifiable& NetworkIndex::get(const std::string& id) {
    return const_cast<Identifiable&>(static_cast<const NetworkIndex*>(this)->get<Identifiable>(id));
}

template <>
stdcxx::const_range<Identifiable> NetworkIndex::getAll<Identifiable, Identifiable>() const {
    return boost::adaptors::values(m_objectsById) | boost::adaptors::indirected;
}

template <>
stdcxx::range<Identifiable> NetworkIndex::getAll<Identifiable, Identifiable>() {
    return boost::adaptors::values(m_objectsById) | boost::adaptors::indirected;
}

template <>
stdcxx::const_range<MultiVariantObject> NetworkIndex::getAll<Identifiable, MultiVariantObject>() const {
    const auto& filter = [](const Identifiable& identifiable) {
        return stdcxx::isInstanceOf<MultiVariantObject>(identifiable);
    };

    const auto& mapper = &map<MultiVariantObject>;

    return boost::adaptors::values(m_objectsById) | boost::adaptors::indirected | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

template <>
stdcxx::range<MultiVariantObject> NetworkIndex::getAll<Identifiable, MultiVariantObject>() {
    const auto& filter = [](const Identifiable& identifiable) {
        return stdcxx::isInstanceOf<MultiVariantObject>(identifiable);
    };
    const auto& mapper = &map<MultiVariantObject>;

    return boost::adaptors::values(m_objectsById) | boost::adaptors::indirected | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

template <>
unsigned long NetworkIndex::getObjectCount<Identifiable>() const {
    return m_objectsById.size();
}

void NetworkIndex::remove(Identifiable& identifiable) {
    const auto& it = m_objectsById.find(identifiable.getId());

    // assert that the two instances are identical (not namesake)
    if ((it != m_objectsById.end()) && stdcxx::areSame(*it->second, identifiable)) {
        Identifiables& identifiables = m_objectsByType.find(typeid(identifiable))->second;

        const auto& itIdentifiable = std::find_if(identifiables.begin(), identifiables.end(), [&identifiable](const std::reference_wrapper<Identifiable>& item) {
            return stdcxx::areSame(identifiable, item.get());
        });

        if (itIdentifiable != identifiables.end()) {
            identifiables.erase(itIdentifiable);
            m_objectsById.erase(identifiable.getId());
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
