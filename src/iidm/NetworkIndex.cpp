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
#include <boost/range/join.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/map.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "BusBreakerVoltageLevel.hpp"
#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

/**
 * This functor overrides the default deleter of the {@link std::unique_ptr}.
 * Delete the pointer for all {@link Identifiable} subclasses except {@link Network}.
 *
 * This hack allows to store the network instance in the NetworkIndex without a double-free corruption.
 */
void NetworkIndex::Deleter::operator()(Identifiable* ptr) const {
    if (!stdcxx::isInstanceOf<Network, Identifiable>(ptr)) {
        delete ptr;
    }
}

NetworkIndex::NetworkIndex(Network& network, NetworkIndex&& networkIndex) noexcept :
    m_objectsById(std::move(networkIndex.m_objectsById)),
    m_objectsByType(std::move(networkIndex.m_objectsByType)) {

    // Replace the references to the old network by the new one
    m_objectsById[network.getId()].reset(&network);
    m_objectsByType[typeid(Network)][0] = std::ref<Identifiable>(network);

    for (Substation& substation : getAll<Substation>()) {
        substation.setNetworkRef(network);
    }
}

bool NetworkIndex::addAlias(const Identifiable& obj, const std::string& alias) {
    auto objectIter = m_objectsById.find(alias);
    if (objectIter != m_objectsById.end()) {
        const Identifiable& aliasConflict = *objectIter->second;
        if (stdcxx::areSame(aliasConflict, obj)) {
            // Silently ignore affecting the objects id to its own aliases
            return false;
        }
        const std::string& message = stdcxx::format("Object (%1%) with alias '%2%' cannot be created because alias already refers to object (%3%) with ID '%4%'",
                                                    stdcxx::demangle(obj), alias, stdcxx::demangle(aliasConflict), aliasConflict.getId());
        throw PowsyblException(message);
    }
    auto aliasIter = m_idByAlias.find(alias);
    if (aliasIter != m_idByAlias.end()) {
        const std::string& id = aliasIter->second;
        const Identifiable& aliasConflict = *m_objectsById[id];
        if (stdcxx::areSame(aliasConflict, obj)) {
            // Silently ignore affecting the same alias twice to an object
            return false;
        }
        const std::string& message = stdcxx::format("Object (%1%) with alias '%2%' cannot be created because alias already refers to object (%3%) with ID '%4%'",
                                                    stdcxx::demangle(obj), alias, stdcxx::demangle(aliasConflict), aliasConflict.getId());
        throw PowsyblException(message);
    }
    m_idByAlias.emplace(alias, obj.getId());
    return true;
}

void NetworkIndex::checkId(const std::string& id) {
    checkNotEmpty(id, "Invalid id '" + id + "'");
}

template <>
stdcxx::CReference<Identifiable> NetworkIndex::find(const std::string& id) const {
    const auto& resolvedId = stdcxx::getOrDefault(m_idByAlias, id, id);
    checkId(resolvedId);

    const auto& it = m_objectsById.find(resolvedId);
    return it != m_objectsById.end() ? stdcxx::cref(*it->second) : stdcxx::cref<Identifiable>();
}

template <>
const Identifiable& NetworkIndex::get(const std::string& id) const {
    const auto& obj = find<Identifiable>(id);
    if (!obj) {
        throw PowsyblException(stdcxx::format("Unable to find to the identifiable '%1%'", id));
    }
    return obj.get();
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
stdcxx::const_range<VoltageLevel> NetworkIndex::getAll<VoltageLevel, VoltageLevel>() const {
    return boost::range::join(getAll<BusBreakerVoltageLevel, VoltageLevel>(), getAll<NodeBreakerVoltageLevel, VoltageLevel>());
}

template <>
stdcxx::range<VoltageLevel> NetworkIndex::getAll<VoltageLevel, VoltageLevel>() {
    return boost::range::join(getAll<BusBreakerVoltageLevel, VoltageLevel>(), getAll<NodeBreakerVoltageLevel, VoltageLevel>());
}

template <>
unsigned long NetworkIndex::getObjectCount<Identifiable>() const {
    return m_objectsById.size();
}

template <>
unsigned long NetworkIndex::getObjectCount<VoltageLevel>() const {
    return getObjectCount<BusBreakerVoltageLevel>() + getObjectCount<NodeBreakerVoltageLevel>();
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
            for (const std::string& alias : identifiable.getAliases()) {
                m_idByAlias.erase(alias);
            }

            identifiables.erase(itIdentifiable);
            m_objectsById.erase(identifiable.getId());
        }
    }
}

void NetworkIndex::removeAlias(const Identifiable& obj, const std::string& alias) {
    auto it = m_idByAlias.find(alias);
    if (it == m_idByAlias.end()) {
        throw PowsyblException(stdcxx::format("No alias '%1%' found in the network", alias));
    }
    if (it->second != obj.getId()) {
        throw PowsyblException(stdcxx::format("Alias '%1%' does not correspond to object '%2%'", alias, obj.getId()));
    }
    m_idByAlias.erase(it);
}

}  // namespace iidm

}  // namespace powsybl
