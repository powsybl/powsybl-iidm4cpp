/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_HXX
#define POWSYBL_IIDM_NETWORK_HXX

#include <powsybl/iidm/Network.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
T& Network::checkAndAdd(std::unique_ptr<T>&& identifiable) {
    return m_networkIndex.checkAndAdd(std::move(identifiable));
}

template <typename T, typename>
const T& Network::get(const std::string& id) const {
    return m_networkIndex.get<T>(id);
}

template <typename T, typename>
T& Network::get(const std::string& id) {
    return m_networkIndex.get<T>(id);
}

template <typename T, typename>
unsigned long Network::getConnectableCount() const {
    return boost::size(getConnectables<T>());
}

template <typename T, typename>
stdcxx::const_range<T> Network::getConnectables() const {
    const auto& filter = [](const Identifiable& identifiable) {
        return stdcxx::isInstanceOf<T>(identifiable);
    };

    return m_networkIndex.getAll<Identifiable, Identifiable>() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(map<const T>);
}

template <typename T, typename>
stdcxx::range<T> Network::getConnectables() {
    const auto& filter = [](const Identifiable& identifiable) {
        return stdcxx::isInstanceOf<T>(identifiable);
    };

    return m_networkIndex.getAll<Identifiable, Identifiable>() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(map<T>);
}

template <typename T, typename>
unsigned long Network::getObjectCount() const {
    return m_networkIndex.getObjectCount<T>();
}

template <typename T, typename>
stdcxx::CReference<T> Network::find(const std::string& id) const {
    return m_networkIndex.find<T>(id);
}

template <typename T, typename>
stdcxx::Reference<T> Network::find(const std::string& id) {
    return m_networkIndex.find<T>(id);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORK_HXX
