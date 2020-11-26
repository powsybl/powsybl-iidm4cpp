/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SynchronousComponentsManager.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "SynchronousComponent.hpp"

namespace powsybl {

namespace iidm {

SynchronousComponentsManager::SynchronousComponentsManager(Network& network) :
    AbstractComponentsManager(network, &SynchronousComponentsManager::createComponent) {
}

SynchronousComponentsManager::SynchronousComponentsManager(Network& network, SynchronousComponentsManager&& manager) noexcept :
        AbstractComponentsManager(network, std::move(manager)) {
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

}  // namespace iidm

}  // namespace powsybl
