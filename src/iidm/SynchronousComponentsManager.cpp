/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SynchronousComponentsManager.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/SynchronousComponent.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

SynchronousComponentsManager::SynchronousComponentsManager(Network& network) :
    ComponentsManager(network) {
}

std::unique_ptr<Component> SynchronousComponentsManager::createComponent(long num, unsigned long size) {
    return stdcxx::make_unique<SynchronousComponent>(num, size, getNetwork());
}

const std::string& SynchronousComponentsManager::getComponentLabel() const {
    static std::string s_componentLabel = "Synchronous";

    return s_componentLabel;
}

void SynchronousComponentsManager::setComponentNumber(Bus& bus, long num) const {
    bus.setSynchronousComponentNumber(num);
}

}  // namespace iidm

}  // namespace powsybl
