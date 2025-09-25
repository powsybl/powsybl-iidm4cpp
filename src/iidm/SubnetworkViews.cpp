/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SubnetworkViews.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/flattened.hpp>

#include "BusBreakerVoltageLevelViews.hpp"

namespace powsybl {

namespace iidm {

namespace subnetwork {

BusBreakerView::BusBreakerView(Network& network) :
    network::BusBreakerView(network) {
}

stdcxx::CReference<Bus> BusBreakerView::getBus(const std::string& id) const {
    const auto& bbv = m_network.getRootNetwork().getBusBreakerView();
    stdcxx::CReference<Bus> bus = bbv.getBus(id);
    if(bus && m_network.contains(bus.get())) {
        return bus;
    } else {
        return stdcxx::CReference<Bus>();
    }
}

stdcxx::Reference<Bus> BusBreakerView::getBus(const std::string& id) {
    auto& bbv = m_network.getRootNetwork().getBusBreakerView();
    stdcxx::Reference<Bus> bus = bbv.getBus(id);
    if(bus && m_network.contains(bus.get())) {
        return bus;
    } else {
        return stdcxx::Reference<Bus>();
    }
}


BusView::BusView(Network& network) :
    network::BusView(network) {
}

stdcxx::CReference<Bus> BusView::getBus(const std::string& id) const {
    const auto& bv = m_network.getRootNetwork().getBusView();
    stdcxx::CReference<Bus> bus = bv.getBus(id);
    if(bus && m_network.contains(bus.get())) {
        return bus;
    } else {
        return stdcxx::CReference<Bus>();
    }
}

stdcxx::Reference<Bus> BusView::getBus(const std::string& id) {
    auto& bv = m_network.getRootNetwork().getBusView();
    stdcxx::Reference<Bus> bus = bv.getBus(id);
    if(bus && m_network.contains(bus.get())) {
        return bus;
    } else {
        return stdcxx::Reference<Bus>();
    }
}

stdcxx::const_range<Component> BusView::getConnectedComponents() const {
    const auto& filter = [this](const Component& comp) {
        for (const auto& b : comp.getBuses()){
            if(m_network.contains(b)){
                return true;
            }
        }
        return false;
    };
    return m_network.getRootNetwork().getBusView().getConnectedComponents() | boost::adaptors::filtered(filter);
}

stdcxx::range<Component> BusView::getConnectedComponents() {
    const auto& filter = [this](const Component& comp) {
        for (auto& b : comp.getBuses()){
            if(m_network.contains(b)){
                return true;
            }
        }
        return false;
    };
    return m_network.getRootNetwork().getBusView().getConnectedComponents() | boost::adaptors::filtered(filter);
}

stdcxx::const_range<Component> BusView::getSynchronousComponents() const {
    const auto& filter = [this](const Component& comp) {
        for (const auto& b : comp.getBuses()){
            if(m_network.contains(b)){
                return true;
            }
        }
        return false;
    };
    return m_network.getRootNetwork().getBusView().getConnectedComponents() | boost::adaptors::filtered(filter);
}

stdcxx::range<Component> BusView::getSynchronousComponents() {
    const auto& filter = [this](const Component& comp) {
        for (auto& b : comp.getBuses()){
            if(m_network.contains(b)){
                return true;
            }
        }
        return false;
    };
    return m_network.getRootNetwork().getBusView().getConnectedComponents() | boost::adaptors::filtered(filter);
}

}  // namespace network

}  // namespace iidm

}  // namespace powsybl
