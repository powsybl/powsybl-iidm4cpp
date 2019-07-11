/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKINDEX_HXX
#define POWSYBL_IIDM_NETWORKINDEX_HXX

#include <powsybl/iidm/NetworkIndex.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
T& NetworkIndex::checkAndAdd(std::unique_ptr<T>&& identifiable) {
    assert(identifiable);
    checkId(identifiable->getId());

    auto other = m_objectsById.find(identifiable->getId());
    if (other != m_objectsById.end()) {
        throw PowsyblException(logging::format("Object '%1%' already exists (%2%)", identifiable->getId(), stdcxx::demangle(*other->second)));
    }

    auto it = m_objectsById.emplace(std::make_pair(identifiable->getId(), std::move(identifiable)));

    std::reference_wrapper<Identifiable> refIdentifiable = *it.first->second;
    m_objectsByType[typeid(T)].emplace_back(refIdentifiable);

    return dynamic_cast<T&>(*it.first->second);
}

template<typename T>
const T& NetworkIndex::get(const std::string& id) const {
    checkId(id);

    const auto& it = m_objectsById.find(id);
    if (it == m_objectsById.end()) {
        throw PowsyblException(logging::format("Unable to find to the identifiable '%1%'", id));
    }

    auto* identifiable = dynamic_cast<T*>(it->second.get());
    if (identifiable == nullptr) {
        throw PowsyblException(logging::format("Identifiable '%1%' is not a %2%", id, stdcxx::demangle<T>()));
    }

    return *identifiable;
}

template<typename T>
T& NetworkIndex::get(const std::string& id) {
    return const_cast<T&>(static_cast<const NetworkIndex*>(this)->get<T>(id));
}

template <>
NetworkIndex::const_range<Identifiable, MultiVariantObject> NetworkIndex::getAll<Identifiable, MultiVariantObject>() const;

template <>
NetworkIndex::range<Identifiable, MultiVariantObject> NetworkIndex::getAll<Identifiable, MultiVariantObject>();

template <>
NetworkIndex::const_range<Identifiable, Identifiable> NetworkIndex::getAll<Identifiable, Identifiable>() const;

template <>
NetworkIndex::range<Identifiable, Identifiable> NetworkIndex::getAll<Identifiable, Identifiable>();

template <typename T, typename U>
NetworkIndex::const_range<T, U> NetworkIndex::getAll() const {
    const auto& it = m_objectsByType[typeid(T)];

    typename network_index::range_traits<T, U>::const_downcaster downcaster = [](const std::reference_wrapper<Identifiable>& identifiable) {
        return std::cref(dynamic_cast<const U&>(identifiable.get()));
    };

    return it | boost::adaptors::transformed(downcaster);
}

template <typename T, typename U>
NetworkIndex::range<T, U> NetworkIndex::getAll() {
    auto& it = m_objectsByType[typeid(T)];

    typename network_index::range_traits<T, U>::downcaster downcaster = [](const std::reference_wrapper<Identifiable>& identifiable) {
        return std::ref(dynamic_cast<U&>(identifiable.get()));
    };

    return it | boost::adaptors::transformed(downcaster);
}

template <typename T>
unsigned long NetworkIndex::getObjectCount() const {
    const auto& it = m_objectsByType.find(typeid(T));
    if (it != m_objectsByType.end()) {
        return it->second.size();
    }
    return 0;
}

template <typename T>
stdcxx::CReference<T> NetworkIndex::find(const std::string& id) const {
    checkId(id);

    const auto& it = m_objectsById.find(id);
    if (it != m_objectsById.end()) {
        auto* identifiable = dynamic_cast<T*>(it->second.get());
        if (identifiable != nullptr) {
            return stdcxx::cref<T>(*identifiable);
        }
    }

    return stdcxx::CReference<T>();
}

template <typename T>
stdcxx::Reference<T> NetworkIndex::find(const std::string& id) {
    const auto& res = static_cast<const NetworkIndex*>(this)->find<T>(id);

    return static_cast<bool>(res) ? stdcxx::ref(const_cast<T&>(res.get())) : stdcxx::ref<T>();
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKINDEX_HXX
