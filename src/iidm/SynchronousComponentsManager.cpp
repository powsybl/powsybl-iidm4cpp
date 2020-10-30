/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SynchronousComponentsManager.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

SynchronousComponentsManager::SynchronousComponentsManager(Network& network) :
    AbstractComponentsManager<SynchronousComponent>(network) {
}

std::unique_ptr<SynchronousComponent> SynchronousComponentsManager::createComponent(unsigned long num, unsigned long size) {
    return stdcxx::make_unique<SynchronousComponent>(num, size, getNetwork());
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
