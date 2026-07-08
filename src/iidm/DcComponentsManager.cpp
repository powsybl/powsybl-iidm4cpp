/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcComponentsManager.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/DcBus.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "DcComponent.hpp"

namespace powsybl {

namespace iidm {

DcComponentsManager::DcComponentsManager(Network& network) :
    AbstractComponentsManager(network, &DcComponentsManager::createComponent, false, true) {
}

DcComponentsManager::DcComponentsManager(Network& network, DcComponentsManager&& manager) noexcept :
        AbstractComponentsManager(network, std::move(manager), false, true) {
}

std::unique_ptr<Component> DcComponentsManager::createComponent(Network& network, unsigned long num, unsigned long size) {
    return stdcxx::make_unique<Component, DcComponent>(network, num, size);
}

const std::string& DcComponentsManager::getComponentLabel() const {
    static std::string s_label = "DC";
    return s_label;
}

void DcComponentsManager::setComponentNumber(Bus& /*bus*/, const stdcxx::optional<unsigned long>& /*num*/) {
    throw PowsyblException("DcComponentsManager should not compute AC buses component number");
}

void DcComponentsManager::setComponentNumber(DcBus& dcBus, const stdcxx::optional<unsigned long>& num) {
    dcBus.setDcComponentNumber(num);
}

}  // namespace iidm

}  // namespace powsybl
