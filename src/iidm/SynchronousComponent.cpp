/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SynchronousComponent.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DcBus.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

SynchronousComponent::SynchronousComponent(Network& network, unsigned long num, unsigned long size) :
    Component(network, num, size) {
}

stdcxx::const_range<DcBus> SynchronousComponent::getDcBuses() const {
    return stdcxx::const_range<DcBus>();
}

stdcxx::range<DcBus> SynchronousComponent::getDcBuses() {
    return stdcxx::range<DcBus>();
}

bool SynchronousComponent::checkBus(const Bus& bus) const {
    const auto& component = bus.getSynchronousComponent();
    return static_cast<bool>(component) && stdcxx::areSame(component.get(), *this);
}

bool SynchronousComponent::checkDcBus(const DcBus& /*dcBus*/) const {
    return false;
}

}  // namespace iidm

}  // namespace powsybl
