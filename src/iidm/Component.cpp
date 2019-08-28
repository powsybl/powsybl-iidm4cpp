/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Component.hpp>

#include <boost/range/adaptor/filtered.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

Component::Component(long num, unsigned long size, Network& network) :
    m_num(num),
    m_size(size),
    m_network(network) {
}

stdcxx::const_range<Bus> Component::getBuses() const {
    const auto& filter = [this](const Bus& bus) {
        const auto& comp = getFilterComponent(bus);
        return static_cast<bool>(comp) && stdcxx::areSame(comp.get(), *this);
    };

    return m_network.getBusView().getBuses() | boost::adaptors::filtered(filter);
}

stdcxx::range<Bus> Component::getBuses() {
    const auto& filter = [this](const Bus& bus) {
        const auto& comp = getFilterComponent(bus);
        return static_cast<bool>(comp) && stdcxx::areSame(comp.get(), *this);
    };

    return m_network.getBusView().getBuses() | boost::adaptors::filtered(filter);
}

long Component::getNum() const {
    return m_num;
}

unsigned long Component::getSize() const {
    return m_size;
}

}  // namespace iidm

}  // namespace powsybl
