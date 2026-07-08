/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SynchronousComponentsManager.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "SynchronousComponent.hpp"

namespace powsybl {

namespace iidm {

SynchronousComponentsManager::SynchronousComponentsManager(Network& network) :
    AbstractComponentsManager(network, &SynchronousComponentsManager::createComponent, true, false) {
}

SynchronousComponentsManager::SynchronousComponentsManager(Network& network, SynchronousComponentsManager&& manager) noexcept :
        AbstractComponentsManager(network, std::move(manager), true, false) {
}

std::unique_ptr<Component> SynchronousComponentsManager::createComponent(Network& network, unsigned long num, unsigned long size) {
    return stdcxx::make_unique<Component, SynchronousComponent>(network, num, size);
}

const std::string& SynchronousComponentsManager::getComponentLabel() const {
    static std::string s_label = "Synchronous";
    return s_label;
}

void SynchronousComponentsManager::setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) {
    bus.setSynchronousComponentNumber(num);
}

void SynchronousComponentsManager::setComponentNumber(DcBus& /*dcBus*/, const stdcxx::optional<unsigned long>& /*num*/) {
    throw PowsyblException("SynchronousComponentsManager should not compute DC buses component number");
}

}  // namespace iidm

}  // namespace powsybl
