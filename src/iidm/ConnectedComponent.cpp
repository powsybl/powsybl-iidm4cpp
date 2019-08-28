/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ConnectedComponent.hpp>

#include <powsybl/iidm/Bus.hpp>

namespace powsybl {

namespace iidm {

ConnectedComponent::ConnectedComponent(long num, unsigned long size, Network& network) :
    Component(num, size, network) {
}

stdcxx::CReference<Component> ConnectedComponent::getFilterComponent(const Bus& bus) const {
    return bus.getConnectedComponent();
}

}  // namespace iidm

}  // namespace powsybl
