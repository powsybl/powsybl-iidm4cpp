/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DcComponent.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DcBus.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

DcComponent::DcComponent(Network& network, unsigned long num, unsigned long size) :
    Component(network, num, size) {
}

stdcxx::const_range<Bus> DcComponent::getBuses() const {
    return stdcxx::const_range<Bus>();
}

stdcxx::range<Bus> DcComponent::getBuses() {
    return stdcxx::range<Bus>();
}

bool DcComponent::checkBus(const Bus& /*bus*/) const {
    return false;
    
}

bool DcComponent::checkDcBus(const DcBus& dcBus) const {
    const auto& component = dcBus.getDcComponent();
    return static_cast<bool>(component) && stdcxx::areSame(component.get(), *this);
}

}  // namespace iidm

}  // namespace powsybl
