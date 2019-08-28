/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/ConnectedComponents.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/ComponentConstants.hpp>

namespace powsybl {

namespace iidm {

namespace ConnectedComponents {

long getCcNum(const stdcxx::CReference<Bus>& bus) {
    long ccNum = ComponentConstants::INVALID_NUM;

    if (static_cast<bool>(bus)) {
        const auto& cc = bus.get().getConnectedComponent();
        if (static_cast<bool>(cc)) {
            ccNum = cc.get().getNum();
        }
    }

    return ccNum;
}

long getCcNum(const stdcxx::Reference<Bus>& bus) {
    return getCcNum(stdcxx::cref<Bus>(bus));
}

long getCcNum(const Bus& bus) {
    return getCcNum(stdcxx::cref<Bus>(bus));
}

}  // namespace ConnectedComponents

}  // namespace iidm

}  // namespace powsybl
