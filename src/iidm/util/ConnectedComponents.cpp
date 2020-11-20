/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/ConnectedComponents.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Component.hpp>

namespace powsybl {

namespace iidm {

namespace ConnectedComponents {

stdcxx::optional<unsigned long> getCcNum(const stdcxx::CReference<Bus>& bus) {
    if (static_cast<bool>(bus)) {
        return getCcNum(bus.get());
    }
    return {};
}

stdcxx::optional<unsigned long> getCcNum(const stdcxx::Reference<Bus>& bus) {
    if (static_cast<bool>(bus)) {
        return getCcNum(bus.get());
    }
    return {};
}

stdcxx::optional<unsigned long> getCcNum(const Bus& bus) {
    stdcxx::optional<unsigned long> ccNum;

    const auto& cc = bus.getConnectedComponent();
    if (static_cast<bool>(cc)) {
        ccNum = cc.get().getNum();
    }

    return ccNum;
}

}  // namespace ConnectedComponents

}  // namespace iidm

}  // namespace powsybl
