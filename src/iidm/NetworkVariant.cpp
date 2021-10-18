/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkVariant.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ConnectedComponentsManager.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/SynchronousComponentsManager.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/flattened.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace network {

const auto& busViewMapper = [](const VoltageLevel& voltageLevel) {
    return voltageLevel.getBusView().getBuses();
};

const auto& busBreakerViewMapper = [](const VoltageLevel& voltageLevel) {
    return voltageLevel.getBusBreakerView().getBuses();
};

const auto& filterNonBusBreaker = [](const VoltageLevel& voltageLevel) {
    return voltageLevel.getTopologyKind() != TopologyKind::BUS_BREAKER;
};

VariantImpl::VariantImpl(Network& network) :
    Variant(network),
    m_connectedComponentsManager(network),
    m_synchronousComponentsManager(network),
    m_busViewCache([&network]() {
        return network.getVoltageLevels() | boost::adaptors::transformed(busViewMapper) | stdcxx::flattened;
    }),
    m_busBreakerViewCache([&network]() {
        return network.getVoltageLevels() | boost::adaptors::filtered(filterNonBusBreaker) | boost::adaptors::transformed(busBreakerViewMapper) | stdcxx::flattened;
    }) {

}

VariantImpl::VariantImpl(Network& network, VariantImpl&& variant) noexcept :
    Variant(network),
    m_connectedComponentsManager(network, std::move(variant.m_connectedComponentsManager)),
    m_synchronousComponentsManager(network, std::move(variant.m_synchronousComponentsManager)),
    m_busViewCache([&network]() {
        return network.getVoltageLevels() | boost::adaptors::transformed(busViewMapper) | stdcxx::flattened;
    }),
    m_busBreakerViewCache([&network]() {
        return network.getVoltageLevels() | boost::adaptors::filtered(filterNonBusBreaker) | boost::adaptors::transformed(busBreakerViewMapper) | stdcxx::flattened;
    }) {
}

std::unique_ptr<VariantImpl> VariantImpl::copy() const {
    return stdcxx::make_unique<VariantImpl>(m_owner);
}

const BusCache& VariantImpl::getBusBreakerViewCache() const {
    return m_busBreakerViewCache;
}

BusCache& VariantImpl::getBusBreakerViewCache() {
    return m_busBreakerViewCache;
}

const BusCache& VariantImpl::getBusViewCache() const {
    return m_busViewCache;
}

BusCache& VariantImpl::getBusViewCache() {
    return m_busViewCache;
}

const ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() const {
    return m_connectedComponentsManager;
}

ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() {
    return m_connectedComponentsManager;
}

const SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() const {
    return m_synchronousComponentsManager;
}

SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() {
    return m_synchronousComponentsManager;
}

}  // namespace network

}  // namespace iidm

}  // namespace powsybl
