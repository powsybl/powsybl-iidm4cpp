/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkIndex.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/stdcxx/memory.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

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
NetworkIndex::const_range<Identifiable, Identifiable> NetworkIndex::getAll<Identifiable, Identifiable>() const {
    typename network_index::range_traits<Identifiable, Identifiable>::const_mapper mapper = [](const network_index::IdentifiableById::value_type& it) {
        return std::cref(*it.second);
    };

    return m_objectsById | boost::adaptors::transformed(mapper);
}

template <>
NetworkIndex::range<Identifiable, Identifiable> NetworkIndex::getAll<Identifiable, Identifiable>() {
    typename network_index::range_traits<Identifiable, Identifiable>::mapper mapper = [](const network_index::IdentifiableById::value_type& it) {
        return std::ref(*it.second);
    };

    return m_objectsById | boost::adaptors::transformed(mapper);
}

template <>
NetworkIndex::const_range<Identifiable, MultiVariantObject> NetworkIndex::getAll<Identifiable, MultiVariantObject>() const {
    typename network_index::range_traits<Identifiable, MultiVariantObject>::filter filter = [](const network_index::IdentifiableById::value_type& it) {
        return stdcxx::isInstanceOf<MultiVariantObject>(it.second);
    };
    typename network_index::range_traits<Identifiable, MultiVariantObject>::const_mapper mapper = [](const network_index::IdentifiableById::value_type& it) {
        return std::cref(dynamic_cast<MultiVariantObject&>(*it.second));
    };

    return m_objectsById | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

template <>
NetworkIndex::range<Identifiable, MultiVariantObject> NetworkIndex::getAll<Identifiable, MultiVariantObject>() {
    typename network_index::range_traits<Identifiable, MultiVariantObject>::filter filter = [](const network_index::IdentifiableById::value_type& it) {
        return stdcxx::isInstanceOf<MultiVariantObject>(it.second);
    };
    typename network_index::range_traits<Identifiable, MultiVariantObject>::mapper mapper = [](const network_index::IdentifiableById::value_type& it) {
        return std::ref(dynamic_cast<MultiVariantObject&>(*it.second));
    };

    return m_objectsById | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

template <>
unsigned long NetworkIndex::getObjectCount<Identifiable>() const {
    return m_objectsById.size();
}

void NetworkIndex::remove(Identifiable& identifiable) {
    const auto& it = m_objectsById.find(identifiable.getId());

    // assert that the two instances are identical (not namesake)
    if ((it != m_objectsById.end()) && stdcxx::areSame(*it->second, identifiable)) {
        network_index::Identifiables& identifiables = m_objectsByType.find(typeid(identifiable))->second;

        const auto& itIdentifiable = std::find_if(identifiables.begin(), identifiables.end(), [&identifiable](std::reference_wrapper<Identifiable>& item) {
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
