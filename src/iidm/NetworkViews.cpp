/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkViews.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/flattened.hpp>

#include "BusBreakerVoltageLevelViews.hpp"

namespace powsybl {

namespace iidm {

namespace network {

BusBreakerView::BusBreakerView(Network& network) :
    m_network(network) {
}

stdcxx::CReference<Bus> BusBreakerView::getBus(const std::string& id) const {
    stdcxx::CReference<Bus> bus;

    for (const auto& vl : m_network.getVoltageLevels()) {
        const auto& vlBus = vl.getBusBreakerView().getBus(id);
        if (static_cast<bool>(vlBus)) {
            bus = stdcxx::cref<Bus>(vlBus);
            break;
        }
    }

    return bus;
}

stdcxx::Reference<Bus> BusBreakerView::getBus(const std::string& id) {
    const auto& bus = static_cast<const BusBreakerView*>(this)->getBus(id);

    return stdcxx::ref(bus);
}

stdcxx::const_range<Bus> BusBreakerView::getBuses() const {
    const auto& mapper = [](const VoltageLevel& voltageLevel) {
        return voltageLevel.getBusBreakerView().getBuses();
    };

    return m_network.getVoltageLevels() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::range<Bus> BusBreakerView::getBuses() {
    const auto& mapper = [](const VoltageLevel& voltageLevel) {
        return voltageLevel.getBusBreakerView().getBuses();
    };

    return m_network.getVoltageLevels() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

unsigned long BusBreakerView::getSwitchCount() const {
    unsigned long res = 0UL;

    for (const auto& vl : m_network.getVoltageLevels()) {
        res += vl.getBusBreakerView().getSwitchCount();
    }

    return res;
}

stdcxx::const_range<Switch> BusBreakerView::getSwitches() const {
    const auto& mapper = [](const VoltageLevel& voltageLevel) {
        return voltageLevel.getBusBreakerView().getSwitches();
    };

    return m_network.getVoltageLevels() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::range<Switch> BusBreakerView::getSwitches() {
    const auto& mapper = [](const VoltageLevel& voltageLevel) {
        return voltageLevel.getBusBreakerView().getSwitches();
    };

    return m_network.getVoltageLevels() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

BusView::BusView(Network& network) :
    m_network(network) {
}

stdcxx::CReference<Bus> BusView::getBus(const std::string& id) const {
    stdcxx::CReference<Bus> bus;

    for (const auto& vl : m_network.getVoltageLevels()) {
        const auto& vlBus = vl.getBusView().getBus(id);
        if (static_cast<bool>(vlBus)) {
            bus = stdcxx::cref<Bus>(vlBus);
            break;
        }
    }

    return bus;
}

stdcxx::Reference<Bus> BusView::getBus(const std::string& id) {
    const auto& bus = static_cast<const BusView*>(this)->getBus(id);

    return stdcxx::ref(bus);
}

stdcxx::const_range<Bus> BusView::getBuses() const {
    const auto& mapper = [](const VoltageLevel& voltageLevel) {
        return voltageLevel.getBusView().getBuses();
    };

    return m_network.getVoltageLevels() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::range<Bus> BusView::getBuses() {
    const auto& mapper = [](const VoltageLevel& voltageLevel) {
        return voltageLevel.getBusView().getBuses();
    };

    return m_network.getVoltageLevels() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::const_range<Component> BusView::getConnectedComponents() const {
    return m_network.getConnectedComponentsManager().getConnectedComponents();
}

stdcxx::range<Component> BusView::getConnectedComponents() {
    return m_network.getConnectedComponentsManager().getConnectedComponents();
}

}  // namespace network

}  // namespace iidm

}  // namespace powsybl
